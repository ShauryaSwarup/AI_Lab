#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

// 8-puzzle state representation
struct State
{
    vector<vector<int>> puzzle;
    int zeroRow, zeroCol;

    bool operator==(const State &other) const
    {
        return puzzle == other.puzzle;
    }
};

namespace std
{
    template <>
    struct hash<State>
    {
        size_t operator()(const State &s) const
        {
            size_t hashVal = 0;
            size_t multiplier = 1;
            for (const auto &row : s.puzzle)
            {
                for (int num : row)
                {
                    hashVal += num * multiplier;
                    multiplier *= 10;
                }
            }
            return hashVal;
        }
    };
}

// Function to print the puzzle board
void printPuzzle(const vector<vector<int>> &puzzle)
{
    for (const auto &row : puzzle)
    {
        for (int num : row)
        {
            cout << num << " ";
        }
        cout << endl;
    }
}

// Function to check if a state is valid
bool isValid(int row, int col)
{
    return (row >= 0 && row < 3 && col >= 0 && col < 3);
}

// Function to generate successor states
vector<State> getSuccessors(const State &currentState)
{
    vector<State> successors;
    int dr[] = {1, -1, 0, 0};
    int dc[] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++)
    {
        int newRow = currentState.zeroRow + dr[i];
        int newCol = currentState.zeroCol + dc[i];

        if (isValid(newRow, newCol))
        {
            State nextState = currentState;
            swap(nextState.puzzle[currentState.zeroRow][currentState.zeroCol], nextState.puzzle[newRow][newCol]);
            nextState.zeroRow = newRow;
            nextState.zeroCol = newCol;
            successors.push_back(nextState);
        }
    }

    return successors;
}

// Depth-First Search
bool dfs(const State &initialState, const State &goalState, vector<State> &path)
{
    stack<pair<State, int>> frontier; // Pair of state and depth
    unordered_set<State> visited;

    frontier.push({initialState, 0}); // Initial state has depth 0

    while (!frontier.empty())
    {
        pair<State, int> currentPair = frontier.top();
        State currentState = currentPair.first;
        int currentDepth = currentPair.second;
        frontier.pop();

        if (currentState == goalState)
        {
            cout << "Solution found using DFS:" << endl;
            printPuzzle(currentState.puzzle);
            cout << "Steps required: " << currentDepth << endl;
            path.push_back(currentState); // Add the goal state to the path
            return true;
        }

        visited.insert(currentState);

        vector<State> successors = getSuccessors(currentState);
        for (const State &successor : successors)
        {
            if (visited.find(successor) == visited.end())
            {
                frontier.push({successor, currentDepth + 1}); // Increase depth
                path.push_back(currentState);                 // Add the current state to the path
            }
        }
    }

    cout << "Solution not found using DFS." << endl;
    return false;
}

// Breadth-First Search
bool bfs(const State &initialState, const State &goalState)
{
    queue<pair<State, int>> frontier; // Pair of state and depth
    unordered_set<State> visited;

    frontier.push({initialState, 0}); // Initial state has depth 0

    while (!frontier.empty())
    {
        pair<State, int> currentPair = frontier.front();
        State currentState = currentPair.first;
        int currentDepth = currentPair.second;
        frontier.pop();

        if (currentState == goalState)
        {
            cout << "Solution found using BFS:" << endl;
            printPuzzle(currentState.puzzle);
            cout << "Steps required: " << currentDepth << endl;
            return true;
        }

        visited.insert(currentState);

        vector<State> successors = getSuccessors(currentState);
        for (const State &successor : successors)
        {
            if (visited.find(successor) == visited.end())
            {
                frontier.push({successor, currentDepth + 1}); // Increase depth
            }
        }
    }

    cout << "Solution not found using BFS." << endl;
    return false;
}
int main()
{
    State initial[2] = {{{{1, 0, 3},
                          {2, 4, 6},
                          {7, 5, 8}},
                         1,
                         0},
                        {{{1, 3, 4},
                          {8, 0, 5},
                          {7, 2, 6}},
                         1,
                         1}};

    State goal[2] =
        {{{{1, 2, 3},
           {4, 5, 6},
           {7, 8, 0}},
          2,
          2},
         {{{1, 2, 3}, {8, 0, 4}, {7, 6, 5}}, 1, 1}};

    for (int i = 0; i < 2; i++)
    {
        vector<State> dfsPath; // Create a vector to store the DFS path
        bool dfsResult = dfs(initial[i], goal[i], dfsPath);
        bool bfsResult = bfs(initial[i], goal[i]);

        if (dfsResult && bfsResult)
        {
            cout << "Solution found using both DFS and BFS." << endl;
            cout << "DFS Path:" << endl;
        }
        else
        {
            cout << "No solution found using either DFS or BFS." << endl;
        }
    }
    return 0;
}