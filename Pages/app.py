from flask import Flask, request, render_template, redirect, url_for, send_file
import subprocess
import shutil
import os

app = Flask(__name__, template_folder='./templates/')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/process_text', methods=['POST'])
def process_text():
    if request.method == 'POST':
        text_input = request.form['text_input']

        try:

            file_path = "static/images/rna.svg"

            if os.path.exists(file_path):
                os.remove(file_path)
                print(f"File '{file_path}' deleted successfully.")
            else:
                print(f"File '{file_path}' does not exist.")
            # Compile the C++ program
            run_command = ['./a.out']
            # compilation_process =  subprocess.run(run_command, input=text_input, text=True, capture_output=True)
            # if compilation_process.returncode != 0:
            #     return f"Compilation Error:\n{compilation_process.stderr}"

            # Execute the compiled program with input and write output to a file
            with open('output.txt', 'w') as output_file:
                execution_process = subprocess.run(run_command, input=text_input, text=True, capture_output=True)
                if execution_process.returncode != 0:
                    return f"Execution Error:\n{execution_process.stderr}"
                output_file.write(execution_process.stdout)

            with open('output.txt', 'r') as output_file:
                first_line = output_file.readline().strip()

            # Write the first line of output to a file
            with open('first_line.txt', 'w') as first_line_file:
                first_line_file.write(first_line )

            # Write the input text to a file
            with open('input_text.txt', 'w') as input_text_file:
                input_text_file.write(text_input)

            with open('a.txt','w') as file:
                file.write(text_input)
                file.write("\n")
                file.write(first_line)
            
            command = f"cat a.txt | RNAplot -o svg "
            subprocess.run(command, shell=True)
            source_file = 'rna.svg'
            destination_directory = './static/images'
            svg_file_path = './static/images/rna.svg'


            # Move the file
            shutil.move(source_file, destination_directory)

            # return render_template('result.html', svg_file=svg_file_path)

            # if result_process.returncode != 0:
            #     return f"Command Execution Error:\n{result_process.stderr}"

            # result_output = result_process.stdout.strip()

            # Redirect to the result page with the command result
            # return redirect(url_for('show_command_result', result=result_output))
            return redirect(request.referrer)
        except Exception as e:
            return f"Error: {e}"
            # # Redirect to the result page
            # return redirect(url_for('show_result'))
        except Exception as e:
            return f"Error: {e}"

    else:
        return 'Method Not Allowed', 405




@app.route('/result')
def show_result():
    svg_file_path = request.args.get('svg_file', '')
    return render_template('result.html', svg_file=svg_file_path)

@app.route('/display_svg')
def display_svg():
    svg_file_path = request.args.get('svg_file', '')
    return send_file(svg_file_path, mimetype='image/svg+xml')


@app.route('/show_command_result')
def show_command_result():
    result_output = request.args.get('result', '')
    return render_template('command_result.html', result=result_output)


# @app.route('/result')
# def show_result():
#     try:
#         with open('output.txt', 'r') as output_file:
#             output_content = output_file.read()
#             return render_template('result.html', output=output_content)
#     except FileNotFoundError:
#         return "Output file not found"

def run_cpp_program(input_text):
    # Compile and run the C++ program
    try:
        # # Compile the C++ program
        # compile_command = ['g++', 'path/to/your/cpp_file.cpp', '-o', 'output']
        # subprocess.run(compile_command, check=True)

        # Run the compiled program with input_text as input
        run_command = ['./a.out']
        result = subprocess.run(run_command, input=input_text, text=True, capture_output=True)

        return result.stdout.strip()  # Return the output of the C++ program
    except subprocess.CalledProcessError as e:
        return f'Error: {e}'


if __name__ == '__main__':
    app.run(debug=True)
