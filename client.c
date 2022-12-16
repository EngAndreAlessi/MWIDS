#include <stdio.h>
#include "graph.c"
#include "list_int.c"
#include "mwids.c"
#include "aco.c"

int main(void)
{
    srand((unsigned int)(time(NULL)));
    FILE* fptr;
    fptr = fopen("log.txt", "w");
    char path[] = "random_graphs/100_ep0c05_nw10_ew1000_0.rg";
    struct Graph* graph = read_rg(path);
    double alpha = 0.5, beta = 0.5;
    int n_ants = 1;
    double initial = 0.1;
    double evap_rate = 0.3;
    int n_iter = 1;
    struct List_int* S = simple_aco(graph, alpha, beta, initial, evap_rate, n_iter, n_ants, 1, fptr);
    print_list_int(S);
    int f = objective_function(S, graph);
    printf("%d\n", f);
    printf("Here\n");
    delete_graph(graph, 0, NULL);
    delete_list_int(S);
    fclose(fptr);
    return 0;
}
