import pandas as pd
import seaborn as sns


def dot_bracket_to_pairs(ss_string):
# x coordinate represents the opening bracket and y coordinate represents closing bracket
    index_list = []
    pairs = {}

    for index, char in enumerate(ss_string):
        if char == '(':
            index_list.append(index)
        if char == ')':
            try:
                # pair to last item in the list in dictionary
                pairs[index_list.pop()] = index
            except IndexError:
                print(f'Invalid structure, found extra \')\' in position {index}')

    if len(index_list) != 0:
        for item in index_list:
            print(f'Invalid structure, found extra in \'(\' in position {item}')

    df_pairs = pd.DataFrame(pairs.items(), columns=['Y', 'X'])

    return df_pairs

db_string = '((((((((((..(((((((.......)))))))......).((((((.......))))))..)))))))))'
df = dot_bracket_to_pairs(db_string)

sns.scatterplot(data=df, x="X", y="Y")