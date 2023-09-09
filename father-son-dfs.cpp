#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <stack>

using namespace std;

unordered_map<int, string> actions = {
    {0, "Initial State"},
    {1, "2 sons go from B1 to B2"},
    {2, "1 son goes from B1 to B2"},
    {3, "Father goes from B1 to B2"},
    {4, "2 sons go from B2 to B1"},
    {5, "1 son goes from B2 to B1"},
    {6, "Father goes from B2 to B1"},
};

int boat1, boat2, father1, father2, sons1, sons2;
unordered_map<int, pair<vector<int>, vector<int>>> successors = {
    {0, {{1, 2, 1}, {0, 0, 0}}},
    {1, {{0, 0, 0}, {0, 0, 0}}},
    {2, {{0, 0, 0}, {0, 0, 0}}},
    {3, {{0, 0, 0}, {0, 0, 0}}},
    {4, {{0, 0, 0}, {0, 0, 0}}},
    {5, {{0, 0, 0}, {0, 0, 0}}},
    {6, {{0, 0, 0}, {0, 0, 0}}},
};

struct Node
{
    vector<int> b1;
    vector<int> b2;
    struct Node *parent;
    int action;
    int pathcost;
};
typedef struct Node Node;

stack<Node *> frontier;
stack<Node *> explored;

vector<int> generate_successor(Node *current)
{

    vector<int> successor_list;
    if (current->b1[2] == 1 && current->b1[1] == 2)
    {
        successor_list.push_back(1);
    }
    if (current->b1[2] == 1 && current->b1[1] >= 1)
    {
        successor_list.push_back(2);
    }
    if (current->b1[2] == 1 && current->b1[0] == 1)
    {
        successor_list.push_back(3);
    }
    if (current->b2[2] == 1 && current->b2[1] == 2)
    {
        successor_list.push_back(4);
    }
    if (current->b2[2] == 1 && current->b2[1] >= 1)
    {
        successor_list.push_back(5);
    }
    if (current->b2[2] == 1 && current->b2[0] == 1)
    {
        successor_list.push_back(6);
    }
    return successor_list;
}

Node *create_node(int e, struct Node *parent)
{
    Node *new_node = new Node;

    new_node->parent = parent;
    new_node->action = e;
    new_node->b1 = successors[e].first;
    new_node->b2 = successors[e].second;
    new_node->pathcost = parent->pathcost + 1;

    return new_node;
}

bool isGoal(int i, Node goal)
{
    if (successors[i].first == goal.b1 && successors[i].second == goal.b2)
    {
        return true;
    }
    return false;
}

bool inFrontier(int i, stack<Node *> q)
{
    stack<Node *> copyFront = q;
    while (!copyFront.empty())
    {
        Node *curr = copyFront.top();
        copyFront.pop();
        if (curr->b1 == successors[i].first && successors[i].second == curr->b2)
        {
            return true;
        }
    }
    return false;
}

void updateValues(int i)
{
    switch (i)
    {
    case 1:
        successors[1].first[0] = father1;
        successors[1].first[1] = 0;
        successors[1].first[2] = 0;
        successors[1].second[0] = father2;
        successors[1].second[1] = 2;
        successors[1].second[2] = 1;
        break;
    case 2:
        successors[2].first[0] = father1;
        successors[2].first[1] = sons1 - 1;
        successors[2].first[2] = 0;
        successors[2].second[0] = father2;
        successors[2].second[1] = sons2 + 1;
        successors[2].second[2] = 1;
        break;
    case 3:
        successors[3].first[0] = 0;
        successors[3].first[1] = sons1;
        successors[3].first[2] = 0;
        successors[3].second[0] = 1;
        successors[3].second[1] = sons2;
        successors[3].second[2] = 1;
        break;
    case 4:
        successors[4].first[0] = father1;
        successors[4].first[1] = 2;
        successors[4].first[2] = 1;
        successors[4].second[0] = father2;
        successors[4].second[1] = 0;
        successors[4].second[2] = 0;
        break;
    case 5:
        successors[5].first[0] = father1;
        successors[5].first[1] = sons1 + 1;
        successors[5].first[2] = 1;
        successors[5].second[0] = father2;
        successors[5].second[1] = sons2 - 1;
        successors[5].second[2] = 0;
        break;
    case 6:
        successors[6].first[0] = 1;
        successors[6].first[1] = sons1;
        successors[6].first[2] = 1;
        successors[6].second[0] = 0;
        successors[6].second[1] = sons2;
        successors[6].second[2] = 0;
        break;

    default:
        break;
    }
}

int main()
{
    Node *initial = new Node;
    initial->b1 = successors[0].first;
    initial->b2 = successors[0].second;
    initial->parent = NULL;
    initial->action = 0;
    initial->pathcost = 0;

    Node goal;
    goal.b1 = {0, 0, 0};
    goal.b2 = {1, 2, 1};

    frontier.push(initial);
    int flag = 0;
    while (!frontier.empty())
    {
        Node *current = frontier.top();
        frontier.pop();

        father1 = current->b1[0];
        sons1 = current->b1[1];
        boat1 = current->b1[2];
        father2 = current->b2[0];
        sons2 = current->b2[1];
        boat2 = current->b2[2];

        if (isGoal(current->action, goal))
        {
            flag = 1;
            cout << "Goal reached" << endl;
            cout << "Path cost: " << current->pathcost << endl;
            cout << "Path:" << endl;
            stack<int> goalActions;
            while (current != NULL)
            {
                goalActions.push(current->action);
                current = current->parent;
            }
            while (!actions.empty())
            {
                cout << goalActions.top() << ". " << actions[goalActions.top()] << endl;
                goalActions.pop();
            }
            break;
        }

        vector<int> successor_list = generate_successor(current);
        for (auto i : successor_list)
        {
            updateValues(i);
            if (!inFrontier(i, explored))
            {
                if (!inFrontier(i, frontier))
                {
                    Node *newNode = create_node(i, current);
                    frontier.push(newNode);
                }
            }
        }
        explored.push(current);
    }
    if (flag == 0)
    {
        cout << "Goal not reachable" << endl;
    }
    return 0;
}