#include <iostream>
#include <vector>

using namespace std;

bool canPair(char base1, char base2) {
    return (base1 == 'A' && base2 == 'U') ||
           (base1 == 'U' && base2 == 'A') ||
           (base1 == 'C' && base2 == 'G') ||
           (base1 == 'G' && base2 == 'C');
}

int solve(string rna) {
    int n = rna.size();
    vector<vector<int> > OPT(n, vector<int>(n, 0));

    for (int k = 5; k < n; ++k) {
        for (int i = 0; i < n - k; ++i) {
            int j = i + k;

            OPT[i][j] = OPT[i][j - 1]; // case 1

            // case 2
            for (int t = i; t < j; ++t) {
                if (canPair(rna[t], rna[j])) {
                    int c = 1;
                    if (t > i) {
                        c += OPT[i][t - 1]; // left
                    }
                    if (t + 1 < j) {
                        c += OPT[t + 1][j - 1]; // right
                    }
                    OPT[i][j] = max(OPT[i][j], c);
                }
            }
        }
    }

    return OPT[0][n - 1];
}

int main() {
    string rna;
    cin>>rna;

    int ans = solve(rna);

    cout << ans << endl;

    return 0;
}
