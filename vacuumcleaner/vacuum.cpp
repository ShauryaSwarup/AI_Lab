#include <bits/stdc++.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct agent
{
        int type;
        int xpos;
        int ypos;
        int direction;
        int score;
};

class world
{
private:
        int height;
        int width;

public:
        vector<vector<int>> contents;
        world(int h, int w)
        {
                this->height = h;
                this->width = w;
                contents.resize(height, vector<int>(width, 0));
        }
        void create_dirt(int num_dirt)
        {
                // Clear existing dirt
                contents.assign(height, vector<int>(width, 0));

                for (int i = 0; i < num_dirt; ++i)
                {
                        int x, y;
                        do
                        {
                                x = rand() % width;
                                y = rand() % height;
                        } while (contents[y][x] == 1); // Ensure a dirt cell is not overwritten

                        contents[y][x] = 1;
                }
        }
        void add_obstacles(int num_obstacles)
        {
                for (int i = 0; i < num_obstacles; ++i)
                {
                        int x, y;
                        do
                        {
                                x = rand() % width;
                                y = rand() % height;
                        } while (contents[y][x] == 1); // Ensure the cell is not already dirt

                        contents[y][x] = 2; // Use a different value (e.g., 2) to represent obstacles
                }
        }

        void display()
        {
                for (int i = 0; i < height; i++)
                {
                        for (int j = 0; j < width; j++)
                        {
                                cout << contents[i][j];
                        }
                        cout << endl;
                }
        }
        // This brings up a display of the grid and reports the agent's location
        void report_dims(agent x)
        {
                for (int i = 0; i < height; i++)
                {
                        for (int j = 0; j < width; j++)
                        {
                                cout << contents[j][i];
                        }
                        cout << "\n";
                }
                cout << "Agent is currently in (" << x.xpos << ", " << x.ypos << ") and facing "
                     << x.direction << " with score " << x.score << "\n";
        }
        inline int report(agent x)
        {
                return (contents[x.ypos][x.xpos]);
        }
        inline void turn_right(agent x)
        {
                x.direction++;
                if (x.direction > 3)
                        x.direction = 0;
        }
        inline void turn_left(agent x)
        {
                x.direction--;
                if (x.direction < 0)
                        x.direction = 3;
        }
        void move_forward(agent x)
        {
                switch (x.direction)
                {
                case 0:
                        x.ypos++;
                case 1:
                        x.xpos++;
                case 2:
                        x.ypos--;
                case 3:
                        x.xpos--;
                }
        }
        inline void eat_here(agent x)
        {
                contents[x.ypos][x.xpos] = 0;
        }
        // Function to check if a given position is inside the grid
        bool is_valid_move(int x, int y)
        {
                return (x >= 0 && x < width && y >= 0 && y < height && contents[y][x] != 2);
        }
        // Function to perform BFS cleaning with obstacle detection and avoidance
        void bfs(agent x)
        {
                queue<pair<int, int>> q;
                q.push({x.xpos, x.ypos});
                vector<vector<bool>> visited(height, vector<bool>(width, false));
                visited[x.ypos][x.xpos] = true;

                int steps = 0;

                while (!q.empty())
                {
                        int sz = q.size();
                        while (sz--)
                        {
                                int curX = q.front().first;
                                int curY = q.front().second;
                                q.pop();

                                // Clean dirt if present
                                if (contents[curY][curX] == 1)
                                {
                                        contents[curY][curX] = 0;
                                        x.score++;
                                }

                                // Explore adjacent cells
                                int dx[] = {-1, 1, 0, 0};
                                int dy[] = {0, 0, -1, 1};

                                for (int i = 0; i < 4; ++i)
                                {
                                        int newX = curX + dx[i];
                                        int newY = curY + dy[i];

                                        if (is_valid_move(newX, newY) && !visited[newY][newX])
                                        {
                                                q.push({newX, newY});
                                                visited[newY][newX] = true;
                                        }
                                }
                        }
                        steps++;
                }

                cout << "BFS Cleaning Complete in " << steps << " steps. Score: " << x.score << endl;
        }
        void dfs(agent &x, int curX, int curY, vector<vector<bool>> &visited, int &steps)
        {
                // Clean dirt if present
                if (contents[curY][curX] == 1)
                {
                        contents[curY][curX] = 0;
                        x.score++;
                }

                // Explore adjacent cells
                int dx[] = {-1, 1, 0, 0};
                int dy[] = {0, 0, -1, 1};

                for (int i = 0; i < 4; ++i)
                {
                        int newX = curX + dx[i];
                        int newY = curY + dy[i];

                        if (is_valid_move(newX, newY) && !visited[newY][newX])
                        {
                                visited[newY][newX] = true;
                                dfs(x, newX, newY, visited, steps);
                        }
                }

                steps++;
        }

        // Function to initiate DFS cleaning with obstacle detection and avoidance
        void startDFS(agent x)
        {
                vector<vector<bool>> visited(height, vector<bool>(width, false));
                visited[x.ypos][x.xpos] = true;
                int steps = 0;
                dfs(x, x.xpos, x.ypos, visited, steps);
                cout << "DFS Cleaning Complete in " << steps << " steps. Score: " << x.score << endl;
        }
};

int main()
{
        srand(time(0)); // Seed the random number generator with current time

        int width = 10, height = 10;
        world grid1(height, width); // create a world named 'grid'
        grid1.create_dirt(rand() % (width * height));
        grid1.add_obstacles(rand() % (width * height));
        world grid2 = grid1;
        agent vacuum_cleaner = {2, 0, 0, 2, 0}; // create a cleaner at (0,0) facing south

        grid1.report_dims(vacuum_cleaner);
        cout << "BFS Cleaning:" << endl;
        grid1.bfs(vacuum_cleaner);

        cout << "======================================================" << endl;

        grid2.report_dims(vacuum_cleaner);
        cout << "DFS Cleaning:" << endl;
        grid2.startDFS(vacuum_cleaner);
        return (0);
}