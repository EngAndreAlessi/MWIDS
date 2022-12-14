#include "mwids.h"
#include <limits.h>

// Contribution of node v in respect to partial solution S
int contribution(int v, struct List_int* S, struct Graph* graph, int verbose)
{
    int c = 0;
    if(vertex_in_list_int(S, v))
    {
        c = get_node_weight(graph, v);
        if(verbose)
            printf("c(%d|S) = %d\n", v, c);
        return c;
    }
    else
    {
        struct List_int* temp = list_int_intersection(get_neighborhood(graph, v, 0), S);
        if(temp->length == 0)
        {
            c = get_max_edge_weights(graph);
            if(verbose)
                printf("c(%d|S) = %d\n", v, c);
            return c;
        }
        else
        {
            c = INT_MAX;
            struct Node_int* temp2 = S->head;
            while(temp2)
            {
                if(has_edge(graph, v, temp2->value))
                {
                    int edge_w = get_edge_weight(graph, v, temp2->value);
                    if(edge_w < c)
                        c = edge_w;
                }
                temp2 = temp2->next;
            }
            if(verbose)
                printf("c(%d|S) = %d\n", v, c);
            return c;
        }
    }
}

// Auxiliary objective function
int auxiliary_objective_function(struct List_int* S, struct Graph* graph, int verbose)
{
    struct Node_Graph* temp = graph->head;
    int sum = 0;
    while(temp)
    {
        sum += contribution(temp->v, S, graph, verbose);
        temp = temp->next;
    }
    if(verbose)
        printf("faux(S u {s}) = %d\n", sum);
    return sum;
}

// Helper for the argmin part of the algorithm
int argmin(struct List_int* S, struct Graph* graph, int verbose)
{
    int arg = 0;
    int f_min = INT_MAX;
    struct Node_Graph* temp = graph->head;
    while(temp)
    {
        struct List_int* v_list = create_list_int();
        insert_list_int(v_list, temp->v);
        struct List_int* v_union = list_int_union(S, v_list);
        int f = auxiliary_objective_function(v_union, graph, verbose);
        if(f < f_min)
        {
            f_min = f;
            arg = temp->v;
        }
        delete_list_int(v_list);
        delete_list_int(v_union);
        temp = temp->next;
    }
    return arg;
}

// Helper for the argmax part of the algorithm
int argmax(struct Graph* graph, int verbose)
{
    struct Node_Graph* temp = graph->head;
    int arg = temp->v;

    double f_max = get_node_degree(graph, temp->v)/(1.0*get_node_weight(graph, temp->v));
    if(verbose)
        printf("f: %lf\n", f_max);
    temp = temp->next;
    while(temp)
    {
        double f = get_node_degree(graph, temp->v)/(1.0*get_node_weight(graph, temp->v));
        if(verbose)
            printf("f: %lf\n", f);
        if(f > f_max)
        {
            f_max = f;
            arg = temp->v;
        }
        temp = temp->next;
    }
    return arg;
}

// Greedy Heuristic 1
struct List_int* greedy1(struct Graph* graph, int verbose)
{
    struct List_int* S = create_list_int();
    struct Graph* g = copy_graph(graph, verbose);
    while(g->n > 0)
    {
        int v_ = argmax(g, verbose);
        if(verbose)
            printf("Chosen vertex: %d\n", v_);
        insert_list_int(S, v_);
        if(verbose)
        {
            printf("Partial solution: ");
            print_list_int(S);
        }
        struct List_int* cn = get_closed_neighborhood(g, v_, verbose);
        if(verbose)
        {
            printf("Closed neighborhood of %d: ", v_);
            print_list_int(cn);
        }
        struct Node_int* temp = cn->head;
        while(temp)
        {
            if(verbose)
                printf("Removing %d\n", temp->value);
            remove_node_graph(g, temp->value, verbose);
            temp = temp->next;
        }
        delete_list_int(cn);
    }
    delete_graph(g, 0);
    return S;
}

// Greedy Heuristic 2
struct List_int* greedy2(struct Graph* graph, int verbose)
{
    struct List_int* S = create_list_int();
    struct Graph* g = copy_graph(graph, verbose);
    while(g->n > 0)
    {
        int v_ = argmin(S, g, verbose);
        if(verbose)
            printf("Chosen vertex: %d\n", v_);
        insert_list_int(S, v_);
        if(verbose)
        {
            printf("Partial solution: ");
            print_list_int(S);
        }
        struct List_int* cn = get_closed_neighborhood(g, v_, verbose);
        if(verbose)
        {
            printf("Closed neighborhood of %d: ", v_);
            print_list_int(cn);
        }
        struct Node_int* temp = cn->head;
        while(temp)
        {
            if(verbose)
                printf("Removing %d\n", temp->value);
            remove_node_graph(g, temp->value, verbose);
            temp = temp->next;
        }
        delete_list_int(cn);
    }
    return S;
}

// Calculate the objective function for a solution
int objective_function(struct List_int* S, struct Graph* graph, int verbose)
{
    int f = 0;
    struct Node_int* temp = S->head;
    while(temp)
    {
        f += get_node_weight(graph, temp->value);
        temp = temp->next;
    }
    struct List_int* aux = list_int_diff(get_node_list(graph), S);
    temp = aux->head;
    while(temp)
    {
        struct List_int* neighborhood = get_neighborhood(graph, temp->value, verbose);
        struct List_int* restricted_neighborhood = list_int_intersection(neighborhood, S);
        struct Node_int* temp2 = restricted_neighborhood->head;
        int min_w = get_edge_weight(graph, temp->value, temp2->value);
        temp2 = temp2->next;
        while(temp2)
        {
            int e_w = get_edge_weight(graph, temp->value, temp2->value);
            if(e_w < min_w)
                min_w = e_w;
            temp2 = temp2->next;
        }
        f += min_w;
        delete_list_int(neighborhood);
        delete_list_int(restricted_neighborhood);
        temp = temp->next;
    }
    return f;
}
