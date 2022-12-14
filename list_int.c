#include "list_int.h"
#include <stdlib.h>

// Create empty list
struct List_int* create_list_int()
{
    struct List_int* list_int = (struct List_int*)malloc(sizeof(struct List_int));
    list_int->length = 0;
    list_int->head = NULL;
    return list_int;
}

// Create a list node
struct Node_int* create_node_int(int v)
{
    struct Node_int* newNode = (struct Node_int*)malloc(sizeof(struct Node_int));
    newNode->value = v;
    newNode->next = NULL;
    return newNode;
}

// Insert list node
void insert_list_int(struct List_int* list_int, int v)
{
    struct Node_int* newNode = create_node_int(v);
    if(list_int->head == NULL)
        list_int->head = newNode;
    else
    {
        struct Node_int* temp = list_int->head;
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
    list_int->length++;
}

// Remove node from list
void remove_node_int(struct List_int* list_int, int v)
{
    struct Node_int* temp = list_int->head;
    if(temp->value == v)
    {
        list_int->head = temp->next;
        temp->next = NULL;
        free(temp);
    }
    else
    {
        struct Node_int* temp2 = temp->next;
        while(temp2->value != v)
        {
            temp = temp2;
            temp2 = temp2->next;
        }
        temp->next = temp2->next;
        temp2->next = NULL;
        free(temp2);
    }
    list_int->length--;
}

// Delete a list
void delete_list_int(struct List_int* list_int)
{
    struct Node_int* temp = list_int->head;
    while(temp)
    {
        remove_node_int(list_int, temp->value);
        temp = list_int->head;
    }
    free(list_int);
}

// Print a list
void print_list_int(struct List_int* list_int)
{
    struct Node_int* temp = list_int->head;
    while(temp)
    {
        printf("%d->", temp->value);
        temp = temp->next;
    }
    printf("NULL\n");
}

// Verify if a vertex v is in a list
int vertex_in_list_int(struct List_int* list_int, int v)
{
    struct Node_int* temp = list_int->head;
    while(temp)
    {
        if(temp->value == v)
            return 1;
        temp = temp->next;
    }
    return 0;
}

// Return the intersection of two lists
struct List_int* list_int_intersection(struct List_int* a, struct  List_int* b)
{
    struct List_int* c = create_list_int();
    struct Node_int* temp = a->head;
    while(temp)
    {
        struct Node_int* temp2 = b->head;
        while(temp2)
        {
            if(temp->value == temp2->value)
                insert_list_int(c, temp->value);
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    return c;
}

// Return the union between two lists
struct List_int* list_int_union(struct List_int* a, struct List_int* b)
{
    struct List_int* c = create_list_int();
    struct List_int* d = list_int_intersection(a, b);
    struct Node_int* temp = a->head;
    while(temp)
    {
        insert_list_int(c, temp->value);
        temp = temp->next;
    }
    temp = b->head;
    while(temp)
    {
        insert_list_int(c, temp->value);
        temp = temp->next;
    }
    temp = d->head;
    while(temp)
    {
        remove_node_int(c, temp->value);
        temp = temp->next;
    }
    return c;
}

// Return the difference between two lists
struct List_int* list_int_diff(struct List_int* a, struct List_int* b)
{
    struct List_int* c = create_list_int();
    struct Node_int* temp = a->head;
    while(temp)
    {
        if(!vertex_in_list_int(b, temp->value))
            insert_list_int(c, temp->value);
        temp = temp->next;
    }
    return c;
}

// Copy a list
struct List_int* copy_list(struct List_int* list_int)
{
    struct List_int* new_list = create_list_int();
    struct Node_int* temp = list_int->head;
    while(temp)
    {
        insert_list_int(new_list, temp->value);
        temp = temp->next;
    }
    return new_list;
}

