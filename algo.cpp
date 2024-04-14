#include <iostream>
#include <vector>
#include <stack>

using namespace std;


bool canPair(char base1, char base2) {
    return (base1 == 'A' && base2 == 'U') ||
           (base1 == 'U' && base2 == 'A') ||
           (base1 == 'C' && base2 == 'G') ||
           (base1 == 'G' && base2 == 'C');
}

int solve(string rna, vector<vector<int> >& traceback) {
    int n = rna.size();
    vector<vector<int> > OPT(n, vector<int>(n, 0));

    for (int k = 5; k < n; ++k) {
        for (int i = 0; i < n - k; ++i) {
            int j = i + k;

            int max = OPT[i][j - 1]; // case 1
            int splitPoint = -1;
            // case 2
            for (int t = i; t < j - 4; ++t) {
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

string tracebackSecondaryStructure(const string& rna, const vector<vector<int> >& traceback) {
    int n = rna.size();
    stack<pair<int, int> > segments;
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

int main() {
    string rna;
    cin>>rna;
    int n = rna.size();
    vector<vector<int> > traceback(n, vector<int>(n, -1));

    int ans = solve(rna, traceback);
    string secondaryStructure = tracebackSecondaryStructure(rna, traceback);

    cout <<"optimal pairs " << ans << endl;
    cout << "Secondary Structure " << secondaryStructure << endl;
    return 0;
}
