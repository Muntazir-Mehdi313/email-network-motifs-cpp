
// Purpose: Declares functions for triad classification and counting.
#ifndef TRIADS_H
#define TRIADS_H

const int MAX = 100;

int triadClass(int a, int b, int c, int adj[MAX][MAX]);

void countTriads(int n, int adj[MAX][MAX], int counts[13]);

#endif
