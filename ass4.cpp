#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <limits>
#include <queue>
#include <set>

using namespace std;
using namespace chrono;

struct QueensState {
    vector<int> queens;
    int row;
    int bound;

    QueensState(int n) : queens(n, -1), row(0), bound(0) {}

    QueensState(const QueensState& other) : queens(other.queens), row(other.row), bound(other.bound) {}

    bool operator>(const QueensState& other) const {
        return bound > other.bound;
    }
};

class NQueensSolver {
private:
    int n;
    vector<int> solution;
    long long nodesExplored;
    bool solutionFound;

public:
    NQueensSolver(int boardSize) : n(boardSize), nodesExplored(0), solutionFound(false) {
        solution.resize(n, -1);
    }

    bool isSafe(const vector<int>& queens, int row, int col) {
        for (int i = 0; i < row; ++i) {
            if (queens[i] == col || 
                queens[i] - i == col - row || 
                queens[i] + i == col + row) {
                return false;
            }
        }
        return true;
    }

    int countConflicts(const vector<int>& queens, int row, int col) {
        int conflicts = 0;
        for (int i = 0; i < row; ++i) {
            if (queens[i] == col || 
                queens[i] - i == col - row || 
                queens[i] + i == col + row) {
                conflicts++;
            }
        }
        return conflicts;
    }

    int calculateLowerBound(const QueensState& state) {
        int conflicts = 0;
        for (int i = 0; i < state.row; ++i) {
            for (int j = i + 1; j < state.row; ++j) {
                if (state.queens[i] == state.queens[j] || 
                    abs(state.queens[i] - state.queens[j]) == abs(i - j)) {
                    conflicts++;
                }
            }
        }
        return conflicts;
    }

    void printBoard(const vector<int>& queens) {
        cout << "\nSolution Board: \n";
        cout << "+";
        for (int i = 0; i < n; ++i) {
            cout << "---+";
        }
        cout << "\n";

        for (int i = 0; i < n; ++i) {
            cout << "|";
            for (int j = 0; j < n; ++j) {
                cout << (queens[i] == j ? " Q |" : "   |");
            }
            cout << "\n+";
            for (int j = 0; j < n; ++j) {
                cout << "---+";
            }
            cout << "\n";
        }
    }

    bool solveBacktracking() {
        nodesExplored = 0;
        solutionFound = false;
        vector<int> queens(n, -1);

        auto startTime = high_resolution_clock::now();
        backtrack(queens, 0);
        auto endTime = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(endTime - startTime).count();
        cout << "Backtracking: " << (solutionFound ? "Solution found" : "No solution") 
             << " in " << duration << " ms after exploring " << nodesExplored << " nodes.\n";

        if (solutionFound) {
            printBoard(solution);
            return true;
        }
        return false;
    }

    void backtrack(vector<int>& queens, int row) {
        if (row == n) {
            solution = queens;
            solutionFound = true;
            return;
        }

        for (int col = 0; col < n; ++col) {
            nodesExplored++;

            if (isSafe(queens, row, col)) {
                queens[row] = col;
                backtrack(queens, row + 1);
                if (solutionFound) return;
                queens[row] = -1;
            }
        }
    }

    bool solveBranchAndBound() {
        nodesExplored = 0;
        solutionFound = false;

        priority_queue<QueensState, vector<QueensState>, greater<QueensState>> pq;

        QueensState initialState(n);
        pq.push(initialState);

        auto startTime = high_resolution_clock::now();

        while (!pq.empty() && !solutionFound) {
            QueensState current = pq.top();
            pq.pop();

            nodesExplored++;

            if (current.row == n) {
                solution = current.queens;
                solutionFound = true;
                break;
            }

            for (int col = 0; col < n; ++col) {
                if (isSafe(current.queens, current.row, col)) {
                    QueensState next = current;
                    next.queens[next.row] = col;
                    next.row++;
                    next.bound = calculateLowerBound(next);
                    pq.push(next);
                }
            }
        }

        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(endTime - startTime).count();

        cout << "Branch and Bound: " << (solutionFound ? "Solution found" : "No solution") 
             << " in " << duration << " ms after exploring " << nodesExplored << " nodes.\n";

        if (solutionFound) {
            printBoard(solution);
            return true;
        }
        return false;
    }

    bool solveConstraintBacktracking() {
        nodesExplored = 0;
        solutionFound = false;

        vector<int> queens(n, -1);
        vector<bool> col_used(n, false);
        vector<bool> diag1_used(2*n-1, false);
        vector<bool> diag2_used(2*n-1, false);

        auto startTime = high_resolution_clock::now();
        constraintBacktrack(queens, 0, col_used, diag1_used, diag2_used);
        auto endTime = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(endTime - startTime).count();
        cout << "Constraint Backtracking: " << (solutionFound ? "Solution found" : "No solution") 
             << " in " << duration << " ms after exploring " << nodesExplored << " nodes.\n";

        if (solutionFound) {
            printBoard(solution);
            return true;
        }
        return false;
    }

    void constraintBacktrack(vector<int>& queens, int row, 
                             vector<bool>& col_used,
                             vector<bool>& diag1_used,
                             vector<bool>& diag2_used) {
        if (row == n) {
            solution = queens;
            solutionFound = true;
            return;
        }

        for (int col = 0; col < n; ++col) {
            nodesExplored++;

            int diag1_idx = row + col;
            int diag2_idx = row - col + n - 1;

            if (!col_used[col] && !diag1_used[diag1_idx] && !diag2_used[diag2_idx]) {
                queens[row] = col;
                col_used[col] = true;
                diag1_used[diag1_idx] = true;
                diag2_used[diag2_idx] = true;

                constraintBacktrack(queens, row + 1, col_used, diag1_used, diag2_used);
                if (solutionFound) return;

                queens[row] = -1;
                col_used[col] = false;
                diag1_used[diag1_idx] = false;
                diag2_used[diag2_idx] = false;
            }
        }
    }

    bool solveCSPWithBranchAndBound() {
        nodesExplored = 0;
        solutionFound = false;

        priority_queue<QueensState, vector<QueensState>, greater<QueensState>> pq;
        set<vector<int>> visited;

        QueensState initialState(n);
        pq.push(initialState);

        int bestBound = numeric_limits<int>::max();

        auto startTime = high_resolution_clock::now();

        while (!pq.empty() && !solutionFound) {
            QueensState current = pq.top();
            pq.pop();

            nodesExplored++;

            if (visited.find(current.queens) != visited.end() || current.bound > bestBound) {
                continue;
            }

            visited.insert(current.queens);

            if (current.row == n) {
                if (current.bound == 0) {
                    solution = current.queens;
                    solutionFound = true;
                    break;
                }
                continue;
            }

            vector<int> colOrder(n);
            for (int i = 0; i < n; ++i) colOrder[i] = i;

            sort(colOrder.begin(), colOrder.end(), 
                 [n=this->n](int a, int b) {
                     return abs(a - (n-1)/2.0) < abs(b - (n-1)/2.0);
                 });

            for (int col : colOrder) {
                if (isSafe(current.queens, current.row, col)) {
                    QueensState next = current;
                    next.queens[next.row] = col;
                    next.row++;
                    next.bound = calculateLowerBound(next);

                    if (next.bound < bestBound) {
                        bestBound = next.bound;
                    }

                    if (next.bound == 0) {
                        pq.push(next);
                    }
                }
            }
        }

        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(endTime - startTime).count();

        cout << "CSP with Branch & Bound: " << (solutionFound ? "Solution found" : "No solution") 
             << " in " << duration << " ms after exploring " << nodesExplored << " nodes.\n";

        if (solutionFound) {
            printBoard(solution);
            return true;
        }
        return false;
    }
};

int main() {
    int n;
    cout << "Enter the board size (N) for N-Queens problem: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid board size. Please enter a positive integer.\n";
        return 1;
    }

    if (n == 2 || n == 3) {
        cout << "Warning: No solution exists for N=" << n << "\n";
    }

    NQueensSolver solver(n);

    cout << "\n=== Solving " << n << "-Queens Problem ===\n";

    solver.solveBacktracking();
    solver.solveConstraintBacktracking();
    solver.solveBranchAndBound();
    solver.solveCSPWithBranchAndBound();

    return 0;
}
