#ifndef LIST_INT_H
#define LIST_INT_H

#include <stdlib.h>

struct Node_int
{
    int value;
    struct Node_int* next;
};

struct List_int
{
    int length;
    struct Node_int* head;
    struct List_int* next_list;
};

struct List_of_lists
{
    int length;
    struct List_int* head;
};

struct List_int* create_list_int(); // Create empty list
struct List_of_lists* create_list_of_lists(); // Create empty list of lists
struct Node_int* create_node_int(int v); // Create a list node
void insert_list_int(struct List_int* list_int, int v); // Insert list node
void remove_node_int(struct List_int* list_int, int v); // Remove node from list
void delete_list_int(struct List_int* list_int); // Delete a list
void print_list_int(struct List_int* list_int); // Print a list
void write_list_int(struct List_int* list_int, FILE* fptr); // Write a list into a file
int vertex_in_list_int(struct List_int* list_int, int v); // Verify if a vertex v is in a list
struct List_int* list_int_intersection(struct List_int* a, struct  List_int* b); // Return the intersection of two lists
struct List_int* list_int_union(struct List_int* a, struct List_int* b); // Return the union between two lists
struct List_int* list_int_diff(struct List_int* a, struct List_int* b); // Return the difference between two lists
struct List_int* copy_list(struct List_int* list_int); // Copy a list
void insert_list_of_lists(struct List_of_lists* lol, struct List_int* list_int); // Insert a list into a list of lists
void print_list_of_lists(struct List_of_lists* lol); // Print a list of lists
void write_list_of_lists(struct List_of_lists* lol, FILE* fptr); // Write a list of lists in a file
void remove_list_of_lists(struct List_of_lists* lol, int i); // Remove list i from list of lists
void delete_list_of_lists(struct List_of_lists* lol); // Delete a list of lists

#endif // LIST_INT_H
