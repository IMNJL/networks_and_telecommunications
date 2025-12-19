#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

constexpr int INF = 999999;
constexpr int N = 7;

using Matrix = vector<vector<int>>;

static ofstream output("matrix.csv");

void displayMatrix(const Matrix &m, const string &label = "") {
    cout << "\nMatrix: " << label << '\n';
    output << label << '\n';
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            string value = (m[i][j] == INF) ? string("inf") : to_string(m[i][j]);
            cout << setw(6) << value << "\t";
            output << value << ";";
        }
        cout << '\n';
        output << '\n';
    }
}

void displayMatrixPositions(const Matrix &m, const string &label = "") {
    cout << "\nMatrix: " << label << '\n';
    output << label << '\n';
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            char pos;
            if (m[i][j] == INF) pos = '-';
            else pos = static_cast<char>(m[i][j] + 'A');
            cout << pos << ' ';
            output << pos << ";";
        }
        cout << '\n';
        output << '\n';
    }
}

Matrix makeEmpty() {
    return Matrix(N, vector<int>(N, INF));
}

Matrix copyMatrix(const Matrix &src) { return src; }

Matrix minPlusMultiply(const Matrix &a, const Matrix &b) {
    Matrix res = makeEmpty();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                if (a[i][k] == INF || b[k][j] == INF) continue;
                res[i][j] = min(res[i][j], a[i][k] + b[k][j]);
            }
        }
    }
    return res;
}

// For each (i,j) pick the k that minimizes a[i][k] + b[k][j], store the k index in positions
Matrix multiplyMatrixPositions(const Matrix &a, const Matrix &b) {
    Matrix pos = makeEmpty();
    Matrix best = makeEmpty();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            best[i][j] = INF;
            pos[i][j] = INF; // INF means no position
            for (int k = 0; k < N; ++k) {
                if (a[i][k] == INF || b[k][j] == INF) continue;
                int val = a[i][k] + b[k][j];
                if (val < best[i][j]) {
                    best[i][j] = val;
                    pos[i][j] = k;
                }
            }
        }
    }
    return pos;
}

// updateMatrix keeps original a but invalidates some entries following original logic
Matrix updateMatrix(const Matrix &a, const Matrix &b) {
    Matrix c = copyMatrix(a);
    for (int i = 0; i < N; ++i) {
        for (int j = i; j < N; ++j) {
            if (i == j || (a[i][j] != b[i][j] && i > j)) c[i][j] = INF;
        }
    }
    return c;
}

bool areMatricesEqual(const Matrix &a, const Matrix &b) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (a[i][j] != b[i][j]) return false;
    return true;
}

int main() {
    Matrix a = {
        {0, 20, INF, 30, INF, INF, INF},
        {20, 0, 30, 20, INF, INF, INF},
        {INF, INF, 0, INF, 5, INF, INF},
        {INF, 20, 15, 0, 35, 10, 15},
        {INF, INF, INF, 20, 0, INF, INF},
        {INF, INF, INF, 10, 15, 0, 15},
        {35, INF, INF, 15, INF, 15, 0}
    };

    // L1
    displayMatrix(a, "L1");

    // compute L2 = L1 * L1 (min-plus)
    Matrix d = minPlusMultiply(a, a);

    // prepare for iterative multiplication: Lk+1 = Lk * L1
    Matrix prev = a;
    int l = 1;
    while (true) {
        ++l;
        string label = "L" + to_string(l);
        displayMatrix(d, label);

        if (areMatricesEqual(prev, d) || l == N - 1) break;

        Matrix next = minPlusMultiply(d, a);
        prev = d;
        d = next;
    }

    Matrix g = updateMatrix(a, d);
    displayMatrix(g, "G");

    Matrix positions = multiplyMatrixPositions(g, d);
    displayMatrixPositions(positions, "D");

    output.close();

    cout << "Done. Output written to matrix.csv" << endl;
    return 0;
}