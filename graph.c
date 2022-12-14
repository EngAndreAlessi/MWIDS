#include "graph.h"
#include <stdlib.h>
#include <string.h>

// Create a graph
struct Graph* createGraph()
{
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->n = 0;
    graph->m = 0;
    graph->head = NULL;

    return graph;
}

// Create node
struct Node_Graph* createNode(int v, int node_w)
{
    struct Node_Graph* newNode = (struct Node_Graph*)malloc(sizeof(struct Node_Graph));
    newNode->v = v;
    newNode->node_w = node_w;
    newNode->edge_w = 0;
    newNode->next = NULL;
    newNode->neighbor = NULL;
    return newNode;
}

// Add node
void addNode(struct Graph* graph, int v, int node_w)
{
    struct Node_Graph* newNode = createNode(v, node_w);
    if(graph->head == NULL)
        graph->head = newNode;
    else
    {
        struct Node_Graph* temp = graph->head;
        while(temp->next)
            temp = temp->next;
        temp->next = newNode;
    }

    graph->n++;
}

// Add edge
void addEdge(struct Graph* graph, int src, int dst, int edge_w)
{
    // Add edge from src to dst
    struct Node_Graph* temp = graph->head;
    struct Node_Graph* temp2 = temp;
    while(temp->v != src)
        temp = temp->next;
    struct Node_Graph* newNode = createNode(dst, temp->node_w);
    newNode->edge_w = edge_w;
    if(temp->neighbor == NULL)
        temp->neighbor = newNode;
    else
    {
        temp2 = temp;
        while(temp2->neighbor != NULL)
            temp2 = temp2->neighbor;
        temp2->neighbor = newNode;
    }
    // Add edge from dst to src
    temp = graph->head;
    while(temp->v != dst)
        temp = temp->next;
    newNode = createNode(src, temp->node_w);
    newNode->edge_w = edge_w;
    if(temp->neighbor == NULL)
        temp->neighbor = newNode;
    else
    {
        temp2 = temp;
        while(temp2->neighbor != NULL)
            temp2 = temp2->neighbor;
        temp2->neighbor = newNode;
    }
    graph->m++;
}

// Print nodes and weights
void printNodesWeight(struct Graph* graph)
{
    struct Node_Graph* temp = graph->head;
    while(temp)
    {
        printf("%d:%d\n", temp->v, temp->node_w);
        temp = temp->next;
    }
}

// Print edges and weights
void printEdgesWeight(struct Graph* graph)
{
    struct Node_Graph* temp = graph->head;
    while(temp)
    {
        struct Node_Graph* temp2 = temp;
        temp2 = temp2->neighbor;
        while(temp2)
        {
            printf("%d\t%d\t%d\n", temp->v, temp2->v, temp2->edge_w);
            temp2 = temp2->neighbor;
        }
        temp = temp->next;
    }
}

// Verify if graph has edge (v,u)
int has_edge(struct Graph* graph, int v, int u)
{
    struct Node_Graph* temp = graph->head;
    while(temp->v != v)
        temp = temp->next;
    while(temp)
    {
        if(temp->v == u)
            return 1;
        temp = temp->neighbor;
    }
    return 0;
}

// Verify if graph has node v
int has_node(struct Graph* graph, int v)
{
    struct Node_Graph* temp = graph->head;
    while(temp)
    {
        if(temp->v == v)
            return 1;
        temp = temp->next;
    }
    return 0;
}

// Gets maximum edge weight in graph
int get_max_edge_weights(struct Graph* graph)
{
    int max_w = 0;
    struct Node_Graph* temp = graph->head;
    struct Node_Graph* temp2;
    while(temp)
    {
        temp2 = temp;
        temp2 = temp2->neighbor;
        while(temp2)
        {
            if(temp2->edge_w > max_w)
                max_w = temp2->edge_w;
            temp2 = temp2->neighbor;
        }
        temp = temp->next;
    }
    return max_w;
}

// Remove edge (src, dst) from graph
void remove_edge(struct Graph* graph, int src, int dst)
{
    if(!has_edge(graph, src, dst))
        printf("Edge doesn't exist\n");
    else
    {
        // Remove from src->dst
        struct Node_Graph* temp = graph->head;
        while(temp->v != src)
            temp = temp->next;
        struct Node_Graph* temp2 = temp->neighbor;
        while(temp2->v != dst)
        {
            temp = temp->neighbor;
            temp2 = temp2->neighbor;
        }
        temp->neighbor = temp2->neighbor;
        temp2->neighbor = NULL;
        free(temp2);
        // Remove from dst->src
        temp = graph->head;
        while(temp->v != dst)
            temp = temp->next;
        temp2 = temp->neighbor;
        while(temp2->v != src)
        {
            temp = temp->neighbor;
            temp2 = temp2->neighbor;
        }
        temp->neighbor = temp2->neighbor;
        temp2->neighbor = NULL;
        free(temp2);
        graph->m--;
    }
}

// Remove all edges of a node v, assumes graph is not empty and node exists
void remove_all_edges_of_node(struct Graph* graph, int v, int verbose)
{
    struct Node_Graph* temp = graph->head;
    while(temp->v != v)
        temp = temp->next;
    temp = temp->neighbor;
    if(!temp)
    {
        if(verbose)
            printf("Free node\n");
    }
    else
    {
        struct List_int* neighborhood = get_neighborhood(graph, v, 0);
        struct Node_int* temp2 = neighborhood->head;
        while(temp2)
        {
            if(verbose)
                printf("Removing edge (%d,%d)\n", v, temp2->value);
            remove_edge(graph, v, temp2->value);
            temp2 = temp2->next;
        }
    }
}

// Get a list with the neighbors of a vertex v on a graph
struct List_int* get_neighborhood(struct Graph* graph, int v, int verbose)
{
    struct List_int* neighborhood = create_list_int();
    struct Node_Graph* temp = graph->head;
    if(!temp)
    {
        if(verbose)
            printf("Graph empty\n");
        return neighborhood;
    }
    else
    {
        while(temp->v != v)
            temp = temp->next;
        if(!temp)
        {
            if(verbose)
                printf("Node not in graph\n");
            return neighborhood;
        }
        else
        {
            temp = temp->neighbor;
            if(!temp)
            {
                if(verbose)
                    printf("Free node\n");
                return neighborhood;
            }
            else
            {
                while(temp)
                {
                    insert_list_int(neighborhood, temp->v);
                    temp = temp->neighbor;
                }
                if(verbose)
                    printf("Neighborhood found\n");
                return neighborhood;
            }
        }
    }
}

// Get the closed neighborhood of a vertex v on a graph
struct List_int* get_closed_neighborhood(struct Graph* graph, int v, int verbose)
{
    struct List_int* neighbors_list = get_neighborhood(graph, v, verbose);
    insert_list_int(neighbors_list, v);
    return neighbors_list;
}

// Remove a node from graph
void remove_node_graph(struct Graph* graph, int v, int verbose)
{
    struct Node_Graph* temp = graph->head;
    if(!temp)
    {
        if(verbose)
            printf("Graph empty\n");
    }
    else
    {
        if(has_node(graph, v))
        {
            remove_all_edges_of_node(graph, v, verbose);
            if(verbose)
                printf("Removing node %d\n", v);
            struct Node_Graph* temp = graph->head;
            if(temp->v == v)
            {
                if(graph->n == 1)
                {
                    graph->head = NULL;
                    free(temp);
                    graph->n--;
                }
                else
                {
                    graph->head = temp->next;
                    temp->next = NULL;
                    free(temp);
                    graph->n--;
                }

            }
            else
            {
                struct Node_Graph* temp2 = temp->next;
                while(temp2->v != v)
                {
                    temp2 = temp2->next;
                    temp = temp->next;
                }
                temp->next = temp2->next;
                temp2->next = NULL;
                free(temp2);
                graph->n--;
            }
        }
        else
        {
            if(verbose)
                printf("Node not in graph\n");
        }
    }
}

// Delete entire graph
void delete_graph(struct Graph* graph, int verbose)
{
    struct Node_Graph* temp = graph->head;
    while(temp)
    {
        remove_node_graph(graph, temp->v, verbose);
        temp = graph->head;
    }
    free(graph);
}

// Get node weight from graph
int get_node_weight(struct Graph* graph, int v)
{
    struct Node_Graph* temp = graph->head;
    while(temp->v != v)
        temp = temp->next;
    return temp->node_w;
}

// Get weight of edge (v,u) from graph
int get_edge_weight(struct Graph* graph, int v, int u)
{
    struct Node_Graph* temp = graph->head;
    while(temp->v != v)
        temp = temp->next;
    temp = temp->neighbor;
    while(temp->v != u)
        temp = temp->neighbor;
    return temp->edge_w;
}

// Get the graph node list
struct List_int* get_node_list(struct Graph* graph)
{
    struct List_int* node_list = create_list_int();
    struct Node_Graph* temp = graph->head;
    while(temp)
    {
        insert_list_int(node_list, temp->v);
        temp = temp->next;
    }
    return node_list;
}

// Get node degree
int get_node_degree(struct Graph* graph, int v)
{
    struct List_int* neighbors_list = get_neighborhood(graph, v, 0);
    int degree = neighbors_list->length;
    delete_list_int(neighbors_list);
    return degree;
}

// Copy graph
struct Graph* copy_graph(struct Graph* graph, int verbose)
{
    struct Graph* new_graph = createGraph();
    struct Node_Graph* temp = graph->head;
    if(verbose)
        printf("Copying nodes...\n");
    while(temp)
    {
        if(verbose)
            printf("Copying node %d with weight %d\n", temp->v, temp->node_w);
        addNode(new_graph, temp->v, temp->node_w);
        temp = temp->next;
    }
    temp = graph->head;
    if(verbose)
        printf("Copying edges...\n");
    while(temp)
    {
        struct Node_Graph* temp2 = temp->neighbor;
        while(temp2)
        {
            if(!has_edge(new_graph, temp->v, temp2->v))
            {
                if(verbose)
                    printf("Copying edge (%d,%d) with weight %d\n", temp->v, temp2->v, temp2->edge_w);
                addEdge(new_graph, temp->v, temp2->v, temp2->edge_w);
            }
            temp2 = temp2->neighbor;
        }
        temp = temp->next;
    }
    return new_graph;
}

// Read .rg files
struct Graph* read_rg(char* path)
{
    struct Graph* graph = createGraph();
    FILE* ptr;
    ptr = fopen(path, "r");

    if(ptr == NULL)
    {
        printf("File can't be opened\n");
    }

    char str1[10], str2[10], str3[10];
    fscanf(ptr, "%s\t%s", str1, str2);
    int n = atoi(str1);
    int m = atoi(str2);
    int i;
    fgets(str1, 10, ptr);
    for(i=0; i<n; i++)
    {
        fgets(str1, 10, ptr);
        addNode(graph, i, atoi(str1));
    }

    for(i=0; i<m; i++)
    {
        fscanf(ptr, "%s\t%s\t%s", str1, str2, str3);
        addEdge(graph, atoi(str1), atoi(str2), atoi(str3));
    }

    fclose(ptr);

    return graph;
}
