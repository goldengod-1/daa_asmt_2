#include <iostream>
#include <bits/stdc++.h>



using namespace std;
//class for RNA sequences.
class RNASequence {
private:
    string sequence;

public:
    // Constructor to initialize the sequence.
    RNASequence(string& seq) : sequence(seq) {}

    // Method to get the length of the sequence.
    int length(){
        return sequence.size();
    }

    // Overloaded subscript operator to access individual characters in the sequence.
    char operator[](int index){
        return sequence[index];
    }

};

class DynamicProgrammingSolver {
private:
    vector<vector<int>> OPT; // Matrix to store optimal solutions
    vector<vector<int>> traceback; // Matrix to store traceback

    // Helper function to check if two bases can pair.
    bool canPair(char base1, char base2) {
        return (base1 == 'A' && base2 == 'U') ||
               (base1 == 'U' && base2 == 'A') ||
               (base1 == 'C' && base2 == 'G') ||
               (base1 == 'G' && base2 == 'C');
    }

public:
    //Constructor to initialise the solver.
    DynamicProgrammingSolver(int n) {
        OPT.resize(n, vector<int>(n, 0));
        traceback.resize(n, vector<int>(n, -1));
    }

    // Method to solve the RNA folding problem.
    int solve(RNASequence& rna) {
        int n = rna.length();

        // Loop through different lengths of sequences.
        for (int k = 4; k < n; k++) {
            for (int i = 0; i < n - k; i++) {
                int j = i + k;

                int max = OPT[i][j - 1]; // Initialize max with the previous value
                int splitPoint = -1; //Initialize split point
            
                // Try different split points and update max.
                for (int t = i; t < j - 4; t++) {
                    if (canPair(rna[t], rna[j])) {
                        int c = 1;
                        if (t > i) {
                            c += OPT[i][t - 1]; // Update with left segment.
                        }
                        if (t + 1 < j) {
                            c += OPT[t + 1][j - 1]; // Update with right segment.
                        }
                        if (c > max) {
                            max = c;
                            splitPoint = t;
                        }
                    }
                }
                OPT[i][j] = max; // Update OPT matrix with the maximum value.
                traceback[i][j] = splitPoint; // Update traceback matrix.
            }
        }

        return OPT[0][n - 1];
    }
    // Method to get the traceback matrix.
      vector<vector<int>>& getTraceback()   {
        return traceback;
    }
};

class RNAFolding {
private:
      RNASequence& rna;  // Reference to the RNA sequence.
      vector<vector<int>>& traceback; // Reference to traceback matrix.

public:
    RNAFolding(RNASequence& r, vector<vector<int>>& tb) : rna(r), traceback(tb) {}

    //Method to perform traceback and generate secondary structure.
    vector<pair<int,int>> tracebackSecondaryStructure(string &structure) {
        int n = rna.length();
        stack<pair<int, int>> segments;
        segments.push(make_pair(0, n - 1));
        vector<pair<int,int>> pairs;

        while (!segments.empty()) {
            int i = segments.top().first;
            int j = segments.top().second;
            segments.pop();

            if (i >= j) continue; // Ignore if i is greater than or equal to j.

            int splitPoint = traceback[i][j];

            if (splitPoint == -1) {
                segments.push(make_pair(i, j - 1)); // Push left segment
            } else {
                structure[splitPoint] = '('; 
                structure[j] = ')';
                pairs.push_back(make_pair(splitPoint, j));
                segments.push(make_pair(i, splitPoint - 1)); //Push left Segment
                segments.push(make_pair(splitPoint + 1, j - 1)); //Push right Segment
            }
        }

        return pairs; // Return pairs representing secondary structure.
    }
};

// A class for analyzing RNA folding.
class RNAFoldingAnalyzer {
public:
    // Method to count the actual pairs in the structure.
    int countActualPairs(string& structureActual) {
        int c = 0;
        for (char x : structureActual) {
            if (x == '(') {
                c++;
            }
        }
        return c; //Return count
    }
};

int main() {
    string rna, actual;
    // Input RNA sequence
    cin >> rna; 

    //cin>>actual;

    //Create RNA sequence object
    RNASequence rnaSeq(rna);
    DynamicProgrammingSolver solver(rnaSeq.length());
    int optimal = solver.solve(rnaSeq);
    int n = rna.size();
    vector<vector<int>>& traceback = solver.getTraceback();
    //RNAFoldingAnalyzer analyse = RNAFoldingAnalyzer(); 
    //int actual_pairs = analyse.countActualPairs(actual);  
    RNAFolding folding(rnaSeq, traceback);
    vector<pair<int, int>> pairs;
    string structure(n,'.');
    pairs = folding.tracebackSecondaryStructure(structure);
    cout<<structure<<endl; //Output the generated secondary structure.
    cout << "optimal pairs " << optimal << endl;
    //cout << "actual pairs" << actual_pairs << endl;
    if(optimal!=0)
    {
        cout << "Optimal pairing according to the indices:"<<endl;
        for(auto x: pairs){
            cout<<"("<<x.first<<","<<x.second<<")"<<" "<<rna[x.first]<<" "<<rna[x.second]<<endl;
        }
    }
    else{
        cout << "No pairing possible";
    }

    return 0;
}
