from flask import Flask, request, render_template

app = Flask(__name__, template_folder='./templates')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/process_text', methods=['POST'])
def process_text():
    if request.method == 'POST':
        text_input = request.form['text_input']
        # Process the text input here (you can replace this with your processing logic)
        processed_text = text_input.upper()  # Example: Convert text to uppercase
        return process_text

if __name__ == '__main__':
    app.run(debug=True)
