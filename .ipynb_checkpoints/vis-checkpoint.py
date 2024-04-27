pip install rna_draw

from rna_draw import rna_drawer

def visualize_rna_structure(dot_bracket):
    rna_drawer.draw(dot_bracket)

# Example usage:
dot_bracket_structure = "(((....)))...((..))."
visualize_rna_structure(dot_bracket_structure)
