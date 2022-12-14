#include <stdio.h>
#include "graph.c"
#include "list_int.c"
#include "mwids.c"
#include "aco.c"

int main(void)
{
    srand((unsigned int)(time(NULL)));
    char path[] = "random_graphs/100_ep0c05_nw10_ew1000_0.rg";
    struct Graph* graph = read_rg(path);
    double alpha = 0.5, beta = 0.5;
    int n_ants = 5;
    double initial = 0.001;
    double evap_rate = 0.3;
    int n_iter = 4;
    struct List_int* S = simple_aco(graph, alpha, beta, n_ants, initial, evap_rate, n_iter, 1);
    print_list_int(S);
    //printf("%d\n", objective_function(S, graph, 0));
    delete_graph(graph, 0);
    delete_list_int(S);
    return 0;
}
