#include <iostream>
using namespace std;

const int MAX = 100;
int triadClass(int a, int b, int c, int adj[MAX][MAX])
{
    int nodes[3] = {a, b, c};

    int permutations[6][3] = {
        {0, 1, 2},
        {0, 2, 1},
        {1, 0, 2},
        {1, 2, 0},
        {2, 0, 1},
        {2, 1, 0}};
    int mincode = 63;

    for (int i = 0; i < 6; i++)
    {
        int x = nodes[permutations[i][0]];
        int y = nodes[permutations[i][1]];
        int z = nodes[permutations[i][2]];

        int code = 0;
        if (adj[x][y] == 1)
        {
            code |= (1 << 0);
        }
        if (adj[x][z] == 1)
        {
            code |= (1 << 1);
        }
        if (adj[y][x] == 1)
        {
            code |= (1 << 2);
        }
        if (adj[y][z] == 1)
        {
            code |= (1 << 3);
        }
        if (adj[z][x] == 1)
        {
            code |= (1 << 4);
        }
        if (adj[z][y] == 1)
        {
            code |= (1 << 5);
        }

        if (code < mincode)
        {
            mincode = code;
        }
    }
    switch (mincode)
    {
    case 3:
        return 1;
    case 6:
        return 2;
    case 7:
        return 3;
    case 10:
        return 4;
    case 11:
        return 5;
    case 15:
        return 6;
    case 21:
        return 7;
    case 23:
        return 8;
    case 25:
        return 9;
    case 27:
        return 10;
    case 30:
        return 11;
    case 31:
        return 12;
    case 63:
        return 13;
    default:
        return 0;
    }
}
void countTriads(int n, int adj[MAX][MAX], int counts[13])
{
    for (int i = 0; i < 13; i++)
    {
        counts[i] = 0;
    }
    for (int a = 0; a < n; a++)
    {
        for (int b = a + 1; b < n; b++)
        {
            for (int c = b + 1; c < n; c++)
            {
                int connectedEdges = 0;
                if (adj[a][b] == 1 || adj[b][a] == 1)
                {
                    connectedEdges++;
                }
                if (adj[a][c] == 1 || adj[c][a] == 1)
                {
                    connectedEdges++;
                }
                if (adj[b][c] == 1 || adj[c][b] == 1)
                {
                    connectedEdges++;
                }
                if (connectedEdges >= 2)
                {
                    int classNumber = triadClass(a, b, c, adj);

                    if (classNumber >= 1 && classNumber <= 13)
                    {
                        counts[classNumber - 1]++;
                    }
                }
            }
        }
    }
}