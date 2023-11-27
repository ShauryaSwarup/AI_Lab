#include <bits/stdc++.h>
using namespace std;
map<vector<int>, bool> visited;
class kSAT
{
private:
    vector<int> createClause();
    void printClause(vector<int> a);

public:
    int k, n, m;
    vector<vector<int>> kSATformula;
    kSAT(int k, int n, int m);
    void createFormula();
    void printkSAT();
    vector<int> generateAssignment();
    int calScore(vector<int> a);
};
vector<int> kSAT::createClause()
{
    vector<int> kSAT;
    vector<int> arr;
    for (int j = 1; j <= n; ++j)
        arr.push_back(j);
    random_shuffle(arr.begin(), arr.end());
    for (int i = 0; i < k; i++)
    {
        kSAT.push_back((arr[i] % n) + 1);
        float random = (float)rand() / RAND_MAX;
        if (random > 0.5)
            kSAT[i] = -kSAT[i];
    }
    return kSAT;
}
void kSAT::printClause(vector<int> a)
{
    cout << "(";
    for (int i = 0; i < k - 1; i++)
    {
        if (a[i] < 0)
            cout << "~";
        cout << "a" << abs(a[i]) << " v ";
    }
    if (a[k - 1] < 0)
        cout << "~";
    cout << "a" << abs(a[k - 1]) << ")";
}
kSAT::kSAT(int k, int n, int m)
{
    this->k = k;
    this->n = n;
    this->m = m;
}
void kSAT::createFormula()
{
    vector<int> a;
    for (int i = 0; i < m; i++)
    {
        a = createClause();
        kSATformula.push_back(a);
    }
}
void kSAT::printkSAT()
{
    for (int i = 0; i < m; i++)
    {
        vector<int> clause = kSATformula[i];
        printClause(clause);
        if (i != m - 1)
            cout << " ^ ";
    }
    cout << endl;
}
vector<int> kSAT::generateAssignment()
{
    vector<int> a;
    for (int i = 0; i < n; i++)
    {
        float random = (float)rand() / RAND_MAX;
        if (random > 0.5)
            a.push_back(1);
        else
            a.push_back(0);
    }
    return a;
}
int kSAT::calScore(vector<int> a)
{
    int score = 0;
    for (int i = 0; i < m; i++)
    {
        bool flag = false;
        for (int j = 0; j < k; j++)
        {
            int x = kSATformula[i][j];
            if (x < 0)
            {
                if (a[abs(x) - 1] == 0)
                {
                    flag = true;
                    break;
                }
            }
            if (x > 0)
            {
                if (a[abs(x) - 1] == 1)
                {
                    flag = true;
                    break;
                }
            }
        }
        if (flag == true)
            score++;
    }
    return score;
}
void printVector(vector<int> a)
{
    for (int i = 0; i < a.size(); i++)
        cout << a[i] << " ";
    cout << "\n";
}
bool compare(const pair<vector<int>, int> &a, const pair<vector<int>, int> &b)
{
    return a.second > b.second;
}
queue<pair<vector<int>, int>> bestneighbors(vector<vector<int>> unsorted, kSAT kSATi, int BEAM_WIDTH)
{
    vector<pair<vector<int>, int>> sorted;
    for (int i = 0; i < unsorted.size(); i++)
    {
        sorted.push_back(make_pair(unsorted[i], kSATi.calScore(unsorted[i])));
    }
    sort(sorted.begin(), sorted.end(), compare);
    queue<pair<vector<int>, int>> open;
    for (int i = 0; i < BEAM_WIDTH; i++)
        open.push(sorted[i]);
    return open;
}
pair<vector<int>, int> getBestNeighbor(kSAT kSATi, vector<int> a)
{
    vector<int> bestNeighbor;
    int bestScore = -1;
    int n = a.size();
    for (int i = 1; i < n; i++)
    {
        vector<int> copy(a);
        if (copy[i] == 0)
            copy[i] = 1;
        else
            copy[i] = 0;
        int currScore = kSATi.calScore(copy);
        if (bestScore < currScore)
        {
            bestNeighbor = copy;
            bestScore = currScore;
        }
    }
    return make_pair(bestNeighbor, bestScore);
}

vector<vector<int>> bitChange(vector<int> a, int number, int *toChange = NULL)
{
    vector<vector<int>> neighbors;
    int n = a.size();
    if (toChange == NULL)
    {
        int temp[n] = {1};
        toChange = temp;
    }
    for (int i = 0; i < n; i++)
    {
        if (toChange[i])
        {
            vector<int> copy(a);
            if (copy[i] == 0)
                copy[i] = 1;
            else
                copy[i] = 0;
            if (number > 1)
            {
                toChange[i] = 0;
                vector<vector<int>> temp = bitChange(copy, number - 1, toChange);
                neighbors.insert(neighbors.end(), temp.begin(), temp.end());
                toChange[i] = 1;
            }
            else
                neighbors.push_back(copy);
        }
    }
    return neighbors;
}
pair<vector<int>, int> getBestFrom(kSAT kSATi, vector<vector<int>> a)
{
    int bestScore = -1e9;
    vector<int> best;
    for (int i = 0; i < a.size(); i++)
    {
        int score = kSATi.calScore(a[i]);
        if (bestScore < score)
        {
            bestScore = score;
            best = a[i];
        }
    }
    return make_pair(best, bestScore);
}
pair<vector<int>, int> getBestNonTabuNeighbor(kSAT kSATi, vector<int> a, int tabuPositions[], int tabuTenure)
{
    vector<int> bestNeighbor;
    int bestScore = -1;
    int bestIndex = -1;
    int n = a.size();
    for (int i = 1; i < n; i++)
    {
        if (tabuPositions[i] <= 0)
        {
            vector<int> copy(a);
            if (copy[i] == 0)
                copy[i] = 1;
            else
                copy[i] = 0;
            int currScore = kSATi.calScore(copy);
            if (bestScore < currScore)
            {
                bestNeighbor = copy;
                bestScore = currScore;
                bestIndex = i;
            }
        }
    }
    tabuPositions[bestIndex] = tabuTenure;
    return make_pair(bestNeighbor, bestScore);
}
void hillClimbing(kSAT kSATi)
{
    int m = kSATi.m;
    vector<int> best = kSATi.generateAssignment();
    int bestScore = kSATi.calScore(best);
    while (bestScore != m)
    {
        pair<vector<int>, int> child = getBestNeighbor(kSATi, best);
        int currScore = child.second;
        if (currScore > bestScore)
        {
            best = child.first;
            bestScore = currScore;
        }
        else
            break;
    }
    if (bestScore == m)
    {
        cout << "SAT Solved: Assignment : ";
        printVector(best);
    }
    else
    {
        cout << "Satisfiable assignment not found\n";
        cout << "Closest Assignment found: ";
        printVector(best);
        cout << "Number of clauses solved: " << bestScore << "\n";
    }
}
void beamSearch(kSAT kSATi, int BEAM_WIDTH)
{
    int m = kSATi.m;
    vector<int> best = kSATi.generateAssignment();
    int bestScore = kSATi.calScore(best);
    queue<pair<vector<int>, int>> open = bestneighbors(bitChange(best, 1), kSATi, BEAM_WIDTH);
    while (bestScore < m)
    {
        pair<vector<int>, int> peek;
        for (int i = 0; i < open.size(); i++)
        {
            peek = open.front();
            if (peek.second > bestScore)
            {
                bestScore = peek.second;
                best = peek.first;
            }
            open.pop();
            pair<vector<int>, int> child = getBestNeighbor(kSATi, peek.first);
            if (child.second < peek.second)
                continue;

            else
                open.push(child);
        }
        if (open.size() == 0)
            break;
    }
    if (bestScore == m)
    {
        cout << "SAT Solved: Assignment : ";
        printVector(best);
    }
    else
    {
        cout << "Satisfiable assignment not found\n";
        cout << "Closest Assignment found: ";
        printVector(best);
        cout << "Number of clauses solved: " << bestScore << "\n";
    }
}
void variableNeighborhoodDescent(kSAT kSATi, int neighborhoods = 3)
{
    int m = kSATi.m;
    int b = 2;
    // cout<<"Neighborhood Function "<<b<<": \n";
    vector<int> best = kSATi.generateAssignment();
    int bestScore = kSATi.calScore(best);
    while (bestScore != m)
    {
        if (b > neighborhoods)
            break;
        pair<vector<int>, int> child = getBestFrom(kSATi, bitChange(best, b));
        int currScore = child.second;
        if (currScore > bestScore)
        {
            best = child.first;
            bestScore = currScore;
        }
        else
        {
            b++;
            // cout<<"Neighborhood Function "<<b<<" \n";
        }
    }
    if (bestScore == m)
    {
        cout << "SAT Solved: Assignment : ";
        printVector(best);
    }
    else
    {
        cout << "Satisfiable assignment not found\n";
        cout << "Closest Assignment found: ";
        printVector(best);
        cout << "Number of clauses solved: " << bestScore << "\n";
    }
}

void tabuSearch(kSAT kSATi, int tabuTenure = 3)
{
    int m = kSATi.m;
    int n = kSATi.n;
    vector<int> best = kSATi.generateAssignment();
    int bestScore = kSATi.calScore(best);
    int tabuPositions[n] = {0};
    while (bestScore != m)
    {
        pair<vector<int>, int> child = getBestNonTabuNeighbor(kSATi, best, tabuPositions, tabuTenure);
        int currScore = child.second;
        if (currScore > bestScore)
        {
            best = child.first;
            bestScore = currScore;
            for (int i = 0; i < n; i++)
            {
                if (tabuPositions[i] > 0)
                    tabuPositions[i]--;
            }
        }
        else
            break;
    }
    if (bestScore == m)
    {
        cout << "SAT Solved: Assignment : ";
        printVector(best);
    }
    else
    {
        cout << "Satisfiable assignment not found\n";
        cout << "Closest Assignment found: ";
        printVector(best);
        cout << "Number of clauses solved: " << bestScore << "\n";
    }
}

int main()
{
    srand((unsigned)time(NULL));
    int k = 3, n, m;
    cout << "Enter the value of n : ";
    cin >> n;
    cout << "Enter the value of m : ";
    cin >> m;
    for (int i = 0; i < 1; i++)
    {
        kSAT kSATi(k, n, m);
        kSATi.createFormula();
        cout << "The randomly created 3-SAT formula: \n";
        kSATi.printkSAT();
        cout << "\nHill Climbing Algorithm:\n";
        hillClimbing(kSATi);
        cout << "\nBeam Search Algorithm (Beam width = 3):\n";
        beamSearch(kSATi, 3);
        cout << "\nBeam Search Algorithm (Beam width = 4):\n";
        beamSearch(kSATi, 4);
        cout << "\nVariable Neighborhood Descent Algorithm:\n";
        variableNeighborhoodDescent(kSATi, 2);
        cout << "\nTabu Search Algorithm:\n";
        tabuSearch(kSATi);
    }
    return 0;
}
