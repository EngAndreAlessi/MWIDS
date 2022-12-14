#ifndef GRAPH_H
#define GRAPH_H
#include "list_int.h"

struct Node_Graph
{
    int v, node_w, edge_w;
    struct Node_Graph* neighbor;
    struct Node_Graph* next;
};

struct Graph
{
    int n; // # of nodes
    int m; // % of edges
    struct Node_Graph* head;
};

struct Graph* createGraph(); // Create a graph
struct Node_Graph* createNode(int v, int node_w); // Create a node
void addNode(struct Graph* graph, int v, int node_w); // Add a node
void addEdge(struct Graph* graph, int src, int dst, int edge_w); // Add an edge
void printNodesWeight(struct Graph* graph); // Print nodes and weights
void printEdgesWeight(struct Graph* graph); // Print edges and weights
int has_edge(struct Graph* graph, int v, int u); // Verify if graph has edge (v,u)
int has_node(struct Graph* graph, int v); // Verify if graph has node v
int get_max_edge_weights(struct Graph* graph); // Gets maximum edge weight in graph
void remove_edge(struct Graph* graph, int src, int dst); // Remove edge (src, dst) from graph
void remove_all_edges_of_node(struct Graph* graph, int v, int verbose); // Remove all edges of a node v, assumes graph is not empty and node exists
struct List_int* get_neighborhood(struct Graph* graph, int v, int verbose); // Get a list with the neighbors of a vertex v on a graph
struct List_int* get_closed_neighborhood(struct Graph* graph, int v, int verbose); // Get the closed neighborhood of a vertex v on a graph
void remove_node_graph(struct Graph* graph, int v, int verbose); // Remove a node from graph
void delete_graph(struct Graph* graph, int verbose); // Delete entire graph
int get_node_weight(struct Graph* graph, int v); // Get node weight from graph
int get_edge_weight(struct Graph* graph, int v, int u); // Get weight of edge (v,u) from graph
int get_node_degree(struct Graph* graph, int v); // Get node degree
struct List_int* get_node_list(struct Graph* graph); // Get the graph node list
struct Graph* copy_graph(struct Graph* graph, int verbose); // Copy graph
struct Graph* read_rg(char* path); // Read .rg files

#endif // GRAPH_H


