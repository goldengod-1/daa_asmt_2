#include <iostream>
#include <bits/stdc++.h>



using namespace std;

class RNASequence {
private:
    string sequence;

public:
    RNASequence(string& seq) : sequence(seq) {}

    int length(){
        return sequence.size();
    }

    char operator[](int index){
        return sequence[index];
    }

};

class DynamicProgrammingSolver {
private:
    vector<vector<int>> OPT;
    vector<vector<int>> traceback;

    bool canPair(char base1, char base2) {
        return (base1 == 'A' && base2 == 'U') ||
               (base1 == 'U' && base2 == 'A') ||
               (base1 == 'C' && base2 == 'G') ||
               (base1 == 'G' && base2 == 'C');
    }

public:
    DynamicProgrammingSolver(int n) {
        OPT.resize(n, vector<int>(n, 0));
        traceback.resize(n, vector<int>(n, -1));
    }

    int solve(RNASequence& rna) {
        int n = rna.length();

        for (int k = 4; k < n; k++) {
            for (int i = 0; i < n - k; i++) {
                int j = i + k;

                int max = OPT[i][j - 1]; // case 1
                int splitPoint = -1;
                // case 2
                for (int t = i; t < j - 4; t++) {
                    if (canPair(rna[t], rna[j])) {
                        int c = 1;
                        if (t > i) {
                            c += OPT[i][t - 1]; // left
                        }
                        if (t + 1 < j) {
                            c += OPT[t + 1][j - 1]; // right
                        }
                        if (c > max) {
                            max = c;
                            splitPoint = t;
                        }
                    }
                }
                OPT[i][j] = max;
                traceback[i][j] = splitPoint;
            }
        }

        return OPT[0][n - 1];
    }

      vector<vector<int>>& getTraceback()   {
        return traceback;
    }
};

class RNAFolding {
private:
      RNASequence& rna;
      vector<vector<int>>& traceback;

public:
    RNAFolding(RNASequence& r, vector<vector<int>>& tb) : rna(r), traceback(tb) {}

    string tracebackSecondaryStructure() {
        int n = rna.length();
        stack<pair<int, int>> segments;
        segments.push(make_pair(0, n - 1));
        string structure(n, '.');
        while (!segments.empty()) {
            int i = segments.top().first;
            int j = segments.top().second;
            segments.pop();

            if (i >= j) continue;

            int splitPoint = traceback[i][j];

            if (splitPoint == -1) {
                segments.push(make_pair(i, j - 1)); // case 1
            } else {
                structure[splitPoint] = '(';
                structure[j] = ')';
                segments.push(make_pair(i, splitPoint - 1)); // left
                segments.push(make_pair(splitPoint + 1, j - 1)); // right
            }
        }

        return structure;
    }
};

class RNAFoldingAnalyzer {
public:
    int countActualPairs(string& structureActual) {
        int c = 0;
        for (char x : structureActual) {
            if (x == '(') {
                c++;
            }
        }
        return c;
    }
};

int main() {
    string rna, actual;
    cin >> rna; 
    //cin >> actual;

    RNASequence rnaSeq(rna);
    DynamicProgrammingSolver solver(rnaSeq.length());
    int optimal = solver.solve(rnaSeq);

    vector<vector<int>>& traceback = solver.getTraceback();

    RNAFolding folding(rnaSeq, traceback);
    string secondaryStructure = folding.tracebackSecondaryStructure();

    cout << secondaryStructure << endl;
    cout << "optimal pairs " << optimal << endl;
    // cout << "actual pairs " << RNAFoldingAnalyzer::countActualPairs(actual) << endl;

    return 0;
}
