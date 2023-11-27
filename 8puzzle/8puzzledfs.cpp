#include <iostream>
#include <stack>
#include <unordered_set>
#include <vector>
using namespace std;

struct PuzzleState {
    vector<vector<int>> board;
    int zeroRow, zeroCol;

    bool operator==(const PuzzleState& other) const {
        return board == other.board;
    }
};

namespace std {
    template<>
    struct hash<PuzzleState> {
        size_t operator()(const PuzzleState& state) const {
            size_t hashValue = 0;
            for (const auto& row : state.board) {
                for (int num : row) {
                    hashValue = hashValue * 10 + num;
                }
            }
            return hashValue;
        }
    };
}

bool isGoalState(const PuzzleState& state) {
    vector<vector<int>> goal = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    return state.board == goal;
}

vector<PuzzleState> getPossibleMoves(const PuzzleState& state) {
    static const int rowChange[] = {-1, 1, 0, 0};
    static const int colChange[] = {0, 0, -1, 1};
    vector<PuzzleState> possibleMoves;
    for (int i = 0; i < 4; ++i) {
        int newRow = state.zeroRow + rowChange[i];
        int newCol = state.zeroCol + colChange[i];

        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3) {
            PuzzleState newMove = state;
            swap(newMove.board[state.zeroRow][state.zeroCol], newMove.board[newRow][newCol]);
            newMove.zeroRow = newRow;
            newMove.zeroCol = newCol;
            possibleMoves.push_back(newMove);
        }
    }
    return possibleMoves;
}

void printBoard(const vector<vector<int>>& board) {
    for (const auto& row : board) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void DFS(const PuzzleState& currentState, unordered_set<PuzzleState>& visited, int& moves) {
    if (isGoalState(currentState)) {
        cout << "Goal State Found in " << moves << " moves!" << endl;
        printBoard(currentState.board);
        return;
    }
    vector<PuzzleState> possibleMoves = getPossibleMoves(currentState);
    for (const PuzzleState& nextState : possibleMoves) {
        if (visited.find(nextState) == visited.end()) {
            visited.insert(nextState);
            moves++;
            DFS(nextState, visited, moves);
            moves--; // Backtrack the move when exploring other possibilities
        }
    }
}


int main() {
    PuzzleState initialState;
    initialState.board = {{1, 2, 6}, {4, 0, 5}, {7, 8, 3}};
    cout << "Initial State:" << endl;
    printBoard(initialState.board);

    unordered_set<PuzzleState> visited;
    visited.insert(initialState);
    int moves = 0;
    DFS(initialState, visited, moves);
    return 0;
}