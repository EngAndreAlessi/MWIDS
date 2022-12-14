#ifndef ACO_H
#define ACO_H
#include "list_int.h"
#include "graph.h"

struct S_upd
{
    int n_ants;
    struct List_int* solutions[];
}; // List of lists to store solutions

void print_array(int n, double arr[]); // Print an array
struct S_upd* create_Supd(int n_ants); // Create a new S_upd struct
void insert_Supd(struct S_upd* supd, struct List_int* list_int, int i); // Insert a list into a S_upd struct
void delete_Supd(struct S_upd* supd); // Delete a S_upd struct
void print_Supd(struct S_upd* supd); // Print a S_upd struct
void calculate_probabilities_as(int n_candidates, double pheromones[], double heuristic_info[], double probs[], double alpha, double beta, struct List_int* removed); // Calculate probability of each solution component as of AS
void calculate_probabilites_ants(int n_candidates, double pheromones[], double heuristic_info[], double probs[], double alpha, struct List_int* removed); // Calculate probability of each solution component as of ANTS
int roulete_wheel_selection(int n_candidates, double probs[], int verbose); // Select an element from array of probabilities using roulette wheel algorithm]
void evaporate_pheromones(int n_candidates, double pheromones[], double evap_rate); // Pheromone evaporation
void deposit_pheromones(int n_candidates, struct S_upd* supd, double pheromones[], struct Graph* graph, int verbose); // Deposit pheromones
void initialize_pheromones(int n_candidates, double pheromones[], double initial); // Initialize pheromones
void calculate_heuristic_info(int n_candidates, double heuristic_info[], struct Graph* graph, struct List_int* S); // Calculate heuristic information
struct List_int* construct_solution_probabilistically(int n_candidates, struct Graph* graph, double pheromones[], double heuristic_info[], double alpha, double beta, int verbose); // Probabilistically construct a solution
struct List_int* simple_aco(struct Graph* graph, double alpha, double beta, int n_ants, double initial, double evap_rate, int n_iter, int verbose); // Simple framework for Ant Colony Optimization

#endif // ACO_H
