#ifndef LIST_INT_H
#define LIST_INT_H

struct Node_int
{
    int value;
    struct Node_int* next;
};

struct List_int
{
    int length;
    struct Node_int* head;
};

struct List_int* create_list_int(); // Create empty list
struct Node_int* create_node_int(int v); // Create a list node
void insert_list_int(struct List_int* list_int, int v); // Insert list node
void remove_node_int(struct List_int* list_int, int v); // Remove node from list
void delete_list_int(struct List_int* list_int); // Delete a list
void print_list_int(struct List_int* list_int); // Print a list
int vertex_in_list_int(struct List_int* list_int, int v); // Verify if a vertex v is in a list
struct List_int* list_int_intersection(struct List_int* a, struct  List_int* b); // Return the intersection of two lists
struct List_int* list_int_union(struct List_int* a, struct List_int* b); // Return the union between two lists
struct List_int* list_int_diff(struct List_int* a, struct List_int* b); // Return the difference between two lists
struct List_int* copy_list(struct List_int* list_int); // Copy a list

#endif // LIST_INT_H
