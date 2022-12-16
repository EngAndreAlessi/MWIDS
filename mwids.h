#ifndef MWIDS_H
#define MWDIS_H

#include <stdlib.h>
#include "graph.h"

int contribution(int v, struct List_int* S, struct Graph* graph, int verbose, FILE* fptr); // Contribution of node v in respect to partial solution S
int auxiliary_objective_function(struct List_int* S, struct Graph* graph, int verbose, FILE* fptr); // Auxiliary objective function
int argmin(struct List_int* S, struct Graph* graph, int verbose, FILE* fptr); // Helper for the argmin part of the algorithm
int argmax(struct Graph* graph, int verbose, FILE* fptr); // Helper for the argmax part of the algorithm
struct List_int* greedy1(struct Graph* graph, int verbose, FILE* fptr); // Greedy Heuristic 1
struct List_int* greedy2(struct Graph* graph, int verbose, FILE* fptr); // Greedy Heuristic 2
int objective_function(struct List_int* S, struct Graph* graph); // Calculate the objective function for a solution


#endif // MWIDS_H
