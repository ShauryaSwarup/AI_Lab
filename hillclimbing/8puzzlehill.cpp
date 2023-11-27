#include <bits/stdc++.h>
using namespace std;

int steps = 0;
// Class representing the state of the puzzle
class PuzzleState
{
public:
    vector<vector<int>> board;
    int heuristic;       // Heuristic value for the current state
    vector<string> path; // Store the path from initial state to this state
    PuzzleState *parent;

    // Constructor
    PuzzleState(vector<vector<int>> b, int h, vector<string> p, PuzzleState *N) : board(b), heuristic(h), path(p), parent(N) {}

    // Function to check if the current state is the goal state
    bool isGoalState()
    {
        // Define your goal state here (e.g., a 2D vector representing the solved puzzle configuration)
        vector<vector<int>> goalState = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 0} // 0 represents the blank tile in the goal state
        };

        return board == goalState;
    }
    bool operator<(const PuzzleState &other) const
    {
        return this->heuristic < other.heuristic;
    }
};

// Function to calculate the heuristic value for a given puzzle state
int calculateHeuristic(const vector<vector<int>> &board)
{
    int heuristic = 0;
    for (int i = 0; i < board.size(); ++i)
    {
        for (int j = 0; j < board[i].size(); ++j)
        {
            int value = board[i][j];
            if (value != 0)
            {
                int targetRow = (value - 1) / board.size();
                int targetCol = (value - 1) % board.size();
                heuristic += abs(targetRow - i) + abs(targetCol - j);
            }
        }
    }
    return heuristic;
}
set<PuzzleState> moveGen(PuzzleState current)
{
    set<PuzzleState> neighbors;
    // Generate neighbors
    int dx[] = {-1, 1, 0, 0}; // Possible moves: up, down, left, right
    int dy[] = {0, 0, -1, 1};
    int blankRow = -1, blankCol = -1;

    // Find the position of the blank tile in the current state
    for (int i = 0; i < current.board.size(); ++i)
    {
        for (int j = 0; j < current.board[i].size(); ++j)
        {
            if (current.board[i][j] == 0)
            {
                blankRow = i;
                blankCol = j;
                break;
            }
        }
    }
    // Generate neighbors by swapping the blank tile with its valid neighbors
    for (int i = 0; i < 4; ++i)
    {
        int newRow = blankRow + dx[i];
        int newCol = blankCol + dy[i];

        // Check if the new position is valid
        if (newRow >= 0 && newRow < current.board.size() &&
            newCol >= 0 && newCol < current.board[newRow].size())
        {
            // Create a new board configuration by swapping the blank tile with its neighbor
            vector<vector<int>> neighborBoard = current.board;
            swap(neighborBoard[blankRow][blankCol], neighborBoard[newRow][newCol]);

            // Check if the neighbor is not visited
            string neighborString = "";
            for (int r = 0; r < 3; ++r)
            {
                for (int c = 0; c < 3; ++c)
                {
                    neighborString += to_string(neighborBoard[r][c]);
                }
            }
            // Calculate heuristic for the neighbor and push it into the priority queue
            int neighborHeuristic = calculateHeuristic(neighborBoard);
            vector<string> newPath = current.path;
            newPath.push_back(neighborString);
            string curParent = "";
            if ((current.parent))
            {
                // Check if move went up
                vector<vector<int>> b = (current.parent)->board;
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        curParent += to_string(b[i][j]);
                    }
                }
            }
            if (neighborString != curParent)
            {
                PuzzleState temp = PuzzleState(neighborBoard, neighborHeuristic, newPath, &current);
                neighbors.insert(temp);
            }
        }
    }
    return neighbors;
}
// Hill Climbing algorithm to solve the 8 Puzzle problem
bool hillClimbing(PuzzleState initialState)
{
    PuzzleState N = initialState;
    set<PuzzleState> neighbors = moveGen(N);

    // Check if there are neighbors
    if (neighbors.empty())
    {
        cout << "No neighbors found." << endl;
        return false;
    }

    PuzzleState newN = *(neighbors.begin());

    while (newN.heuristic < N.heuristic)
    {
        N = newN;
        neighbors = moveGen(N);
        steps++;
        // Check if there are neighbors
        if (neighbors.empty())
        {
            cout << "No more neighbors found." << endl;
            return false;
        }
        newN = *(neighbors.begin());
    }
    if (N.isGoalState())
    {
        // Puzzle is solved
        cout << "Solution Path:" << endl;
        for (const auto &step : N.path)
        {
            cout << step << endl;
        }
        return true;
    }
    // No solution found
    cout<<N.heuristic<<"<-Old    New->"<<newN.heuristic<<endl;
    return false;
}

int main()
{
    // Initialize the initial state of the puzzle
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 5, 8},
        {0, 7, 6} // 0 represents the blank tile
    };
    int initialHeuristic = calculateHeuristic(initialBoard);
    vector<string> initialPath;
    initialPath.push_back("123456078"); // String representation of initial board
    PuzzleState *parent = nullptr;      // Initially, there is no parent
    PuzzleState initialState(initialBoard, initialHeuristic, initialPath, parent);

    // Solve the puzzle using Hill Climbing algorithm
    bool solved = hillClimbing(initialState);

    if (!solved)
    {
        cout << "No solution found." << endl;
    }
    cout << steps << " STEPS." << endl;

    return 0;
}
