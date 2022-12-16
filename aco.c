#include "aco.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

// Write an array into a file
void write_array(int n, double arr[], FILE* fptr)
{
    int i;
    fprintf(fptr, "%lf", arr[0]);
    fflush(fptr);
    for(i = 1; i < n; i++)
    {
        fprintf(fptr, " %lf", arr[i]);
        fflush(fptr);
    }
    fprintf(fptr, "\n");
    fflush(fptr);
}

// Print an array
void print_array(int n, double arr[])
{
    int i;
    printf("%lf", arr[0]);
    for(i = 1; i < n; i++)
        printf(" %lf", arr[i]);
    printf("\n");
}


// Calculate probability of each solution component as of AS
void calculate_probabilities_as(int n_candidates, double pheromones[], double heuristic_info[], double probs[], double alpha, double beta, struct List_int* removed)
{
    int k;
    double sum = 0.0;
    for(k = 0; k < n_candidates; k++)
    {
        if(!vertex_in_list_int(removed, k))
            sum += pow(pheromones[k], alpha) * pow(heuristic_info[k], beta);
    }
    int j;

    for(j = 0; j < n_candidates; j++)
    {
        if(!vertex_in_list_int(removed, j))
            probs[j] = (pow(pheromones[j], alpha)*pow(heuristic_info[j], beta))/sum;
        else
            probs[j] = 0.0;
    }
}

// Calculate probability of each solution component as of ANTS
void calculate_probabilites_ants(int n_candidates, double pheromones[], double heuristic_info[], double probs[], double alpha, struct List_int* removed)
{
    int k;
    double sum = 0.0;
    for(k = 0; k < n_candidates; k++)
    {
        if(!vertex_in_list_int(removed, k))
            sum += alpha * pheromones[k] + (1 - alpha) * heuristic_info[k];
    }
    int j;
    for(j = 0; j < n_candidates; j++)
    {
        if(!vertex_in_list_int(removed, j))
           probs[j] = (alpha * pheromones[j] + (1 - alpha) * heuristic_info[j])/sum;
        else
            probs[j] = 0.0;
    }
}


// Select an element from array of probabilities using roulette wheel algorithm
int roulete_wheel_selection(int n_candidates, double probs[], int verbose, FILE* fptr)
{
    int i;
    double cumulative_probs[n_candidates];
    cumulative_probs[0] = probs[0];
    for(i = 1; i < n_candidates; i++)
        cumulative_probs[i] = cumulative_probs[i-1] + probs[i];
    if(verbose)
    {
        fprintf(fptr, "Cumulative probs:\n");
        fflush(fptr);
        write_array(n_candidates, cumulative_probs, fptr);
    }
    double x = ((double)rand()/(double)(RAND_MAX));
    if(x >= 0 && x < cumulative_probs[0])
        return 0;
    else
    {
        for(i = 1; i < n_candidates; i++)
        {
            if(x >= cumulative_probs[i-1] && x < cumulative_probs[i] && probs[i] > 0.0)
                return i;
        }
    }
}

// Pheromone evaporation
void evaporate_pheromones(int n_candidates, double pheromones[], double evap_rate)
{
    int i;
    for(i = 0; i < n_candidates; i++)
        pheromones[i] = pheromones[i] * (1 - evap_rate);
}

// Deposit pheromones
void deposit_pheromones(int n_candidates, struct List_of_lists* lol, double pheromones[], struct Graph* graph, int n_ants, int verbose, FILE* fptr)
{
    int f[lol->length];
    int i = 0;
    int min_f = INT_MAX, max_f = INT_MIN;
    struct List_int* temp = lol->head;
    do
    {
        f[i] = objective_function(temp, graph);
        if(f[i] < min_f)
            min_f = f[i];
        if(f[i] > max_f)
            max_f = f[i];
        i++;
        temp = temp->next_list;
    }while(temp);
    double sum = 0.0;
    for(i=0; i < lol->length; i++)
        sum += 1/((f[i]-min_f)/(max_f - min_f));
    if(verbose)
    {
        fprintf(fptr, "Sum: %lf\n", sum);
        fflush(fptr);
    }

    int j;
    temp = lol->head;
    do
    {
        for(j = 0; j < n_candidates; j++)
        {
            if(vertex_in_list_int(temp, j))
                pheromones[j] += sum;
        }
        temp = temp->next_list;
    }while(temp);
}

// Initialize pheromones
void initialize_pheromones(int n_candidates, double pheromones[], double initial)
{
    int i;
    for(i = 0; i < n_candidates; i++)
        pheromones[i] = initial;
}

// Calculate heuristic information
void calculate_heuristic_info(int n_candidates, double heuristic_info[], struct Graph* graph, struct List_int* S, struct List_int* removed)
{
    int i;
    double h_min = (double)INT_MAX, h_max = (double)INT_MIN;
    for(i = 0; i < n_candidates; i++)
    {
       struct List_int* v_list = create_list_int();
       insert_list_int(v_list, i);
       struct List_int* v_union = list_int_union(S, v_list);
       double f = (double)auxiliary_objective_function(v_union, graph, 0, NULL);
       if(vertex_in_list_int(removed, i))
        f = 0;
       if(f < h_min)
        h_min = f;
       if(f > h_max)
        h_max = f;
       heuristic_info[i] = f;
       delete_list_int(v_list);
       delete_list_int(v_union);
    }

    for(i = 0; i < n_candidates; i++)
        heuristic_info[i] = (heuristic_info[i] - h_min)/(h_max - h_min);

}

// Probabilistically construct a solution
struct List_int* construct_solution_probabilistically(int n_candidates, struct Graph* graph, double pheromones[], double heuristic_info[], double alpha, double beta, int verbose, FILE* fptr)
{
    struct List_int* S = create_list_int();
    struct List_int* removed = create_list_int();
    struct Graph* g = copy_graph(graph, verbose, fptr);
    while(g->n > 0)
    {
        double probs[n_candidates];
        calculate_heuristic_info(n_candidates, heuristic_info, g, S, removed);
        if(verbose)
        {
            fprintf(fptr, "Heuristic info:\n");
            fflush(fptr);
            write_array(n_candidates, heuristic_info, fptr);
        }
        calculate_probabilities_as(n_candidates, pheromones, heuristic_info, probs, alpha, beta, removed);
        if(verbose)
        {
            fprintf(fptr, "Probabilites:\n");
            fflush(fptr);
            write_array(n_candidates, probs, fptr);
        }
        int v_ = roulete_wheel_selection(n_candidates, probs, verbose, fptr);
        if(verbose)
        {
            fprintf(fptr, "Chosen vertex: %d\n", v_);
            fflush(fptr);
        }
        insert_list_int(S, v_);
        if(verbose)
        {
            fprintf(fptr, "Partial solution: ");
            fflush(fptr);
            write_list_int(S, fptr);
        }
        struct List_int* cn = get_closed_neighborhood(g, v_, 0, NULL);
        removed = list_int_union(removed, cn);
        insert_list_int(removed, v_);
        if(verbose)
        {
            fprintf(fptr, "Closed neighborhood of %d: ", v_);
            fflush(fptr);
            write_list_int(cn, fptr);
        }
        struct Node_int* temp = cn->head;
        while(temp)
        {
            if(verbose)
            {
                fprintf(fptr, "Removing %d\n", temp->value);
                fflush(fptr);
            }
            remove_node_graph(g, temp->value, verbose, fptr);
            temp = temp->next;
        }
        delete_list_int(cn);
    }
    delete_list_int(removed);
    if(verbose)
    {
        fprintf(fptr, "Graph depleated\n");
        fflush(fptr);
    }
    delete_graph(g, 0, NULL);
    return S;
}


// Simple framework for Ant Colony Optimization
struct List_int* simple_aco(struct Graph* graph, double alpha, double beta, double initial, double evap_rate, int n_iter, int n_ants, int verbose, FILE* fptr)
{
    int j;
    struct List_int* best_sol = create_list_int();
    int n_candidates = graph->n;
    double pheromones[n_candidates];
    initialize_pheromones(n_candidates, pheromones, initial);
    for(j = 0; j < n_iter; j++)
    {
        int i;
        struct List_of_lists* lol = create_list_of_lists();
        double heuristic_info[n_candidates];
        if(verbose)
        {
            fprintf(fptr, "Pheromones:\n");
            fflush(fptr);
            write_array(n_candidates, pheromones, fptr);
        }
        /*
        if(j == 1)
            break;
        */
        for(i = 0; i < n_ants; i++)
        {
            if(verbose)
            {
                fprintf(fptr, "Ant %d:\n", i);
                fflush(fptr);
            }
            struct List_int* temp = construct_solution_probabilistically(n_candidates, graph, pheromones, heuristic_info, alpha, beta, verbose, fptr);
            insert_list_of_lists(lol, temp);
            if(verbose)
            {
                fprintf(fptr, "Solution constructed:\n");
                fflush(fptr);
                write_list_int(temp, fptr);
            }
            if(!best_sol->length)
            {
                if(verbose)
                {
                    fprintf(fptr, "Inserting first solution\n");
                    fflush(fptr);
                }
                delete_list_int(best_sol);
                best_sol = copy_list(temp);
            }
            else
            {
                if(objective_function(temp, graph) < objective_function(best_sol, graph))
                {
                    if(verbose)
                    {
                        fprintf(fptr, "Updating best solution\n");
                        fflush(fptr);
                    }
                    delete_list_int(best_sol);
                    best_sol = copy_list(temp);
                }
            }
            if(verbose)
            {
                fprintf(fptr, "New best solution:\n");
                fflush(fptr);
                write_list_int(best_sol, fptr);
            }
        }
        if(verbose)
            write_list_of_lists(lol, fptr);
        evaporate_pheromones(n_candidates, pheromones, evap_rate);
        deposit_pheromones(n_candidates, lol, pheromones, graph, n_ants, verbose, fptr);
        delete_list_of_lists(lol);
    }
    return best_sol;
}
