#include<bits/stdc++.h>
using namespace std;

int N; // Puzzle size (N x N)
vector<vector<int>> goal; // Goal state

vector<pair<int, int>> directions = {{-1,0},{1,0},{0,-1},{0,1}};
vector<char> dirChar = {'U','D','L','R'};

struct Puzzle {
    vector<vector<int>> board;
    int x, y; // position of 0 (blank)
    int g, h, f;
    string path;

    Puzzle(vector<vector<int>> b, int g = 0, string path = "") : board(b), g(g), path(path) {
        h = calcHeuristic();
        f = g + h;
        findBlank();
    }

    void findBlank() {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (board[i][j] == 0) {
                    x = i;
                    y = j;
                    return;
                }
    }

    int calcHeuristic() const {
        // Manhattan distance heuristic
        int dist = 0;
        unordered_map<int, pair<int, int>> goalPos;
        
        // Create a map of value to position in goal state
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                goalPos[goal[i][j]] = {i, j};

        // Calculate Manhattan distance for each tile
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] != 0) {
                    int gi = goalPos.at(board[i][j]).first;
                    int gj = goalPos.at(board[i][j]).second;
                    dist += abs(i - gi) + abs(j - gj);
                }
            }
        }

        return dist;
    }

    bool isGoal() const {
        return board == goal;
    }

    string boardToString() const {
        string s;
        for (const auto& row : board)
            for (int val : row)
                s += to_string(val) + ",";
        return s;
    }

    bool operator>(const Puzzle& other) const {
        return f > other.f;
    }

    void printState() const {
        cout << "\nStep #" << g << " (f=" << f << ", g=" << g << ", h=" << h << ")\n";
        cout << "Path: " << (path.empty() ? "Start" : path) << "\n";
        cout << "Board:\n";
        
        // Determine the width needed for proper alignment
        int maxDigits = to_string(N*N-1).length();
        
        for (const auto& row : board) {
            for (int val : row) {
                if (val == 0) {
                    cout << string(maxDigits, ' ') << " ";
                } else {
                    cout << setw(maxDigits) << val << " ";
                }
            }
            cout << "\n";
        }
    }
};

// Function to count inversions for solvability check
int countInversions(const vector<vector<int>>& board) {
    vector<int> linear;
    for (const auto& row : board)
        for (int val : row)
            if (val != 0) linear.push_back(val);
    
    int inv = 0;
    for (int i = 0; i < linear.size(); ++i)
        for (int j = i + 1; j < linear.size(); ++j)
            if (linear[i] > linear[j]) inv++;
    
    return inv;
}

bool isSolvable(const vector<vector<int>>& start, const vector<vector<int>>& target) {
    int startInv = countInversions(start);
    int goalInv = countInversions(target);
    
    // For odd N, puzzle is solvable if inversions are even
    if (N % 2 == 1) {
        return (startInv % 2) == (goalInv % 2);
    }
    // For even N, need to consider blank row position
    else {
        int startBlankRow = -1, goalBlankRow = -1;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (start[i][j] == 0) startBlankRow = N - i;
                if (target[i][j] == 0) goalBlankRow = N - i;
            }
        }
        
        return ((startInv % 2) == (goalInv % 2)) == ((startBlankRow % 2) == (goalBlankRow % 2));
    }
}

void solveAStar(const vector<vector<int>>& startBoard) {
    if (!isSolvable(startBoard, goal)) {
        cout << "\n This puzzle is not solvable!\n";
        cout << "The given initial configuration cannot be transformed into the goal configuration.\n";
        return;
    }

    priority_queue<Puzzle, vector<Puzzle>, greater<Puzzle>> pq;
    set<string> visited;

    Puzzle start(startBoard);
    pq.push(start);
    visited.insert(start.boardToString());

    int stepCount = 0;
    const int maxSteps = 100000; // Safety limit to prevent infinite loops

    cout << "\nStarting A* Search...\n";
    cout << "Initial state:\n";
    start.printState();

    while (!pq.empty()) {
        Puzzle current = pq.top();
        pq.pop();

        if (current.isGoal()) {
            cout << "\n Puzzle Solved Successfully!\n";
            cout << "Total moves: " << current.g << "\n";
            cout << "Solution path: " << current.path << "\n";
            return;
        }

        if (++stepCount > maxSteps) {
            cout << "\n Warning: Search halted after " << maxSteps << " steps.\n";
            cout << "The puzzle might be too complex or the heuristic might not be strong enough.\n";
            return;
        }

        cout << "\nExploring state (Step " << stepCount << "):";
        current.printState();

        for (int i = 0; i < 4; ++i) {
            int newX = current.x + directions[i].first;
            int newY = current.y + directions[i].second;

            if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
                auto newBoard = current.board;
                swap(newBoard[current.x][current.y], newBoard[newX][newY]);
                Puzzle next(newBoard, current.g + 1, current.path + dirChar[i]);

                if (visited.find(next.boardToString()) == visited.end()) {
                    pq.push(next);
                    visited.insert(next.boardToString());
                    
                    // Optional: Print info about this new state
                    cout << "  Generated move: " << dirChar[i] 
                         << " (f=" << next.f << ", g=" << next.g << ", h=" << next.h << ")\n";
                }
            }
        }
    }

    cout << "\nNo solution found within the explored state space.\n";
}

int main() {
    cout << "========================================\n";
    cout << "        N x N Puzzle Solver\n";
    cout << "========================================\n\n";

    cout << "Enter the puzzle size N (e.g., 3 for 3x3): ";
    cin >> N;

    if (N < 2) {
        cout << "Error: Puzzle size must be at least 2x2.\n";
        return 1;
    }

    vector<vector<int>> startBoard(N, vector<int>(N));
    goal.assign(N, vector<int>(N));

    cout << "\nEnter the initial board (use 0 for blank):\n";
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> startBoard[i][j];

    cout << "\nEnter the goal board:\n";
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> goal[i][j];

    // Validate inputs
    set<int> startValues, goalValues;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            startValues.insert(startBoard[i][j]);
            goalValues.insert(goal[i][j]);
        }
    }

    // Check if both boards contain the same set of numbers
    if (startValues != goalValues) {
        cout << "\nError: The initial and goal boards must contain the same set of numbers.\n";
        return 1;
    }

    solveAStar(startBoard);

    return 0;
}