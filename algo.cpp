#include <iostream>
#include <vector>
#include <stack>

using namespace std;

//valid pair condition
bool canPair(char base1, char base2) {
    return (base1 == 'A' && base2 == 'U') ||
           (base1 == 'U' && base2 == 'A') ||
           (base1 == 'C' && base2 == 'G') ||
           (base1 == 'G' && base2 == 'C');
}

//this is the dp algorithm taught in class
int solve(string rna, vector<vector<int> >& traceback) {
    int n = rna.size();
    vector<vector<int> > OPT(n, vector<int>(n, 0));

    for (int k=4; k<n; k++) {
        for (int i=0; i<n-k; i++) {
            int j=i+k;

            int max = OPT[i][j-1]; // case 1
            int splitPoint = -1;
            // case 2
            for (int t=i; t<j-4; t++) {
                if (canPair(rna[t], rna[j])) {
                    int c = 1;
                    if (t > i) {
                        c += OPT[i][t-1]; // left
                    }
                    if (t + 1 < j) {
                        c += OPT[t+1][j-1]; // right
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

//traceback, we use the splitpoint we saved in the traceback vector to put together our steps from the begining wjile getting each valid pair
vector <pair<int, int> > tracebackSecondaryStructure(string rna, vector<vector<int> > traceback, string & structure) {
    int n = rna.size();
    stack<pair<int, int> > segments;
    segments.push(make_pair(0, n - 1));
    // string structure(n, '.');
    vector <pair<int, int> > pairs;
    while (!segments.empty()) {
        int i = segments.top().first;
        int j = segments.top().second;
        segments.pop();

        if (i >= j) continue;

        int splitPoint = traceback[i][j];

        if (splitPoint == -1) {
            segments.push(make_pair(i, j-1)); // case 1
        } else {
            structure[splitPoint] = '(';
            structure[j] = ')';
            pairs.push_back(make_pair(splitPoint, j));
            segments.push(make_pair(i, splitPoint-1)); // left
            segments.push(make_pair(splitPoint+1, j-1)); // right
        }
    }

    return pairs;
}
int actualPairs(string structureActual){
    int c = 0;
    for(auto x: structureActual){
        if(x=='('){
            c++;
        }
    }
    return c;
}
int main() {
    string rna;
    string actual;
    cin>>rna;
    // cin>>actual;
    int n = rna.size();
    vector<vector<int> > traceback(n, vector<int>(n, -1));

    int ans = solve(rna, traceback);
    string structure(n, '.');
    vector <pair<int, int> > pairs = tracebackSecondaryStructure(rna, traceback, structure);
    cout << structure << endl;
    cout <<"optimal pairs " << ans << endl;
    // cout <<"actual pairs " << actualPairs(actual) << endl;
    for(auto x: pairs){
        cout<<x.first<<" "<<x.second<<endl;
    }
    return 0;
}
