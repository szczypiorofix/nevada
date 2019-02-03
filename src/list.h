#ifndef LIST_H_
#define LIST_H_
#pragma once


typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef void (*callback)(Node* data);

Node* create(int data, Node* next);
Node* prepend(Node* head, int data);
void traverse(Node* head,callback f);
int count(Node *head);
Node* append(Node* head, int data);
Node* insert_after(Node *head, int data, Node* prev);
Node* insert_before(Node *head, int data, Node* nxt);
Node* search(Node* head, int data);
Node* insertion_sort(Node* head);

#endif
