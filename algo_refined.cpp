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
    // Matrix to store optimal solutions
    vector<vector<int>> OPT; 
    // Matrix to store traceback
    vector<vector<int>> traceback; 

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

                // Initialize max with the previous value
                int max = OPT[i][j - 1]; 
                
                //Initialize split point
                int splitPoint = -1; 
            
                // Try different split points and update max.
                for (int t = i; t < j - 4; t++) {
                    if (canPair(rna[t], rna[j])) {
                        int c = 1;
                        if (t > i) {
                            // Update with left segment.
                            c += OPT[i][t - 1]; 
                        }
                        if (t + 1 < j) {
                            // Update with right segment.
                            c += OPT[t + 1][j - 1]; 
                        }
                        if (c > max) {
                            max = c;
                            splitPoint = t;
                        }
                    }
                }
                // Update OPT matrix with the maximum value.
                OPT[i][j] = max; 
                // Update traceback matrix.
                traceback[i][j] = splitPoint; 
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
    // Reference to the RNA sequence.
    RNASequence& rna; 
    // Reference to traceback matrix. 
    vector<vector<int>>& traceback; 

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
            // Ignore if i is greater than or equal to j.
            if (i >= j) continue; 

            int splitPoint = traceback[i][j];

            if (splitPoint == -1) {
                segments.push(make_pair(i, j - 1));
            } else {
                structure[splitPoint] = '('; 
                structure[j] = ')';
                pairs.push_back(make_pair(splitPoint, j));
                 //Push left Segment
                segments.push(make_pair(i, splitPoint - 1));
                //Push right Segment
                segments.push(make_pair(splitPoint + 1, j - 1)); 
            }
        }
        // Return pairs representing secondary structure.
        return pairs; 
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
    //Output the generated secondary structure.
    cout<<structure<<endl; 
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
