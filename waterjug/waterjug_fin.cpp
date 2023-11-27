#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;

void printpath(map<pii, pii> mp, pii u)
{
    if (u.first == 0 && u.second == 0)
    {
        cout << 0 << " " << 0 << endl;
        return;
    }
    printpath(mp, mp[u]);
    cout << u.first << " " << u.second << endl;
}
void BFS(int a, int b, int target)
{
    map<pii, int> m;
    bool isSolvable = false;
    map<pii, pii> mp;

    queue<pii> q;

    q.push(make_pair(0, 0));
    while (!q.empty())
    {

        auto u = q.front();
        // cout<<u.first<<" "<<u.second<<endl;
        q.pop();
        if (m[u] == 1)
            continue;

        if ((u.first > a || u.second > b || u.first < 0 || u.second < 0))
            continue;
        // cout<<u.first<<" "<<u.second<<endl;

        m[{u.first, u.second}] = 1;

        if (u.first == target || u.second == target)
        {
            isSolvable = true;
            printpath(mp, u);
            if (u.first == target)
            {
                if (u.second != 0)
                    cout << u.first << " " << 0 << endl;
            }
            else
            {
                if (u.first != 0)
                    cout << 0 << " " << u.second << endl;
            }
            return;
        }
        // completely fill the jug 2
        if (m[{u.first, b}] != 1)
        {
            q.push({u.first, b});
            mp[{u.first, b}] = u;
        }

        // completely fill the jug 1
        if (m[{a, u.second}] != 1)
        {
            q.push({a, u.second});
            mp[{a, u.second}] = u;
        }

        // transfer jug 1 -> jug 2
        int d = b - u.second;
        if (u.first >= d)
        {
            int c = u.first - d;
            if (m[{c, b}] != 1)
            {
                q.push({c, b});
                mp[{c, b}] = u;
            }
        }
        else
        {
            int c = u.first + u.second;
            if (m[{0, c}] != 1)
            {
                q.push({0, c});
                mp[{0, c}] = u;
            }
        }
        // transfer jug 2 -> jug 1
        d = a - u.first;
        if (u.second >= d)
        {
            int c = u.second - d;
            if (m[{a, c}] != 1)
            {
                q.push({a, c});
                mp[{a, c}] = u;
            }
        }
        else
        {
            int c = u.first + u.second;
            if (m[{c, 0}] != 1)
            {
                q.push({c, 0});
                mp[{c, 0}] = u;
            }
        }

        // empty the jug 2
        if (m[{u.first, 0}] != 1)
        {
            q.push({u.first, 0});
            mp[{u.first, 0}] = u;
        }

        // empty the jug 1
        if (m[{0, u.second}] != 1)
        {
            q.push({0, u.second});
            mp[{0, u.second}] = u;
        }
    }
    if (!isSolvable)
        cout << "No solution";
}
bool DFS(int a, int b, int target, map<pii, int> &visited, map<pii, pii> &mp, pii u)
{
    if ((u.first > a || u.second > b || u.first < 0 || u.second < 0))
            return false;
    if (u.first == target || u.second == target)
    {
        printpath(mp, u);
        if (u.first == target)
        {
            if (u.second != 0)
                cout << u.first << " " << 0 << endl;
        }
        else
        {
            if (u.first != 0)
                cout << 0 << " " << u.second << endl;
        }
        return true;
    }

    visited[u] = 1;
    bool res = false;

    // Fill Jug 1
    if (!visited[{a, u.second}]){
        mp[{a,u.second}]=u;
        res = res || DFS(a, b, target, visited, mp, {a, u.second});
    }

    // Fill Jug 2
    if (!visited[{u.first, b}]){
        mp[{u.first, b}]=u;
        res = res || DFS(a, b, target, visited, mp, {u.first, b});
    }

    // Transfer water from Jug 1 to Jug 2
    int pour = min(u.first, b - u.second);
    if (!visited[{u.first - pour, u.second + pour}]){
        mp[{u.first - pour, u.second + pour}]=u;
        res = res || DFS(a, b, target, visited, mp, {u.first - pour, u.second + pour});
    }

    // Transfer water from Jug 2 to Jug 1
    pour = min(u.second, a - u.first);
    if (!visited[{u.first + pour, u.second - pour}]){
        mp[{u.first + pour, u.second - pour}]=u;
        res = res || DFS(a, b, target, visited, mp, {u.first + pour, u.second - pour});
    }

    // Empty Jug 1
    if (!visited[{0, u.second}]){
        mp[{0, u.second}]=u;
        res = res || DFS(a, b, target, visited, mp, {0, u.second});
    }

    // Empty Jug 2
    if (!visited[{u.first, 0}]){
        mp[{u.first, 0}]=u;
        res = res || DFS(a, b, target, visited, mp, {u.first, 0});
    }

    return res;
}

void DFS(int a, int b, int target)
{
    map<pii, int> visited;
    map<pii, pii> mp;
    cout << "Path from initial state "
            "to solution state ::\n";

    bool found = DFS(a, b, target, visited, mp, {0, 0});
    if (!found)
        cout << "No solution";
}

int main()
{
    int Jug1 = 3, Jug2 = 5, target = 4
    ;

    cout << "BFS Path: \n";
    BFS(Jug1, Jug2, target);

    cout << "\nDFS Path: \n";
    DFS(Jug1, Jug2, target);

    return 0;
}
