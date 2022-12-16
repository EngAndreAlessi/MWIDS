#ifndef ACO_H
#define ACO_H
#include "list_int.h"
#include "graph.h"
#include <stdlib.h>

void write_array(int n, double arr[], FILE* fptr); // Write an array into a file
void print_array(int n, double arr[]); // Print an array
void calculate_probabilities_as(int n_candidates, double pheromones[], double heuristic_info[], double probs[], double alpha, double beta, struct List_int* removed); // Calculate probability of each solution component as of AS
void calculate_probabilites_ants(int n_candidates, double pheromones[], double heuristic_info[], double probs[], double alpha, struct List_int* removed); // Calculate probability of each solution component as of ANTS
int roulete_wheel_selection(int n_candidates, double probs[], int verbose, FILE* fptr); // Select an element from array of probabilities using roulette wheel algorithm]
void evaporate_pheromones(int n_candidates, double pheromones[], double evap_rate); // Pheromone evaporation
void deposit_pheromones(int n_candidates, struct List_of_lists* lol, double pheromones[], struct Graph* graph, int n_ants, int verbose, FILE* fptr); // Deposit pheromones
void initialize_pheromones(int n_candidates, double pheromones[], double initial); // Initialize pheromones
void calculate_heuristic_info(int n_candidates, double heuristic_info[], struct Graph* graph, struct List_int* S, struct List_int* removed); // Calculate heuristic information
struct List_int* construct_solution_probabilistically(int n_candidates, struct Graph* graph, double pheromones[], double heuristic_info[], double alpha, double beta, int verbose, FILE* fptr); // Probabilistically construct a solution
struct List_int* simple_aco(struct Graph* graph, double alpha, double beta, double initial, double evap_rate, int n_iter, int n_ants, int verbose, FILE* fptr); // Simple framework for Ant Colony Optimization

#endif // ACO_H
