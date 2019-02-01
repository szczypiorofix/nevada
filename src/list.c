#include <stdio.h>
#include <stdlib.h>
#include "list.h"


Node* create(int data, Node* next);
Node* prepend(Node* head, int data);
void traverse(Node* head,callback f);
int count(Node *head);
Node* insert_after(Node *head, int data, Node* prev);
Node* insert_before(Node *head, int data, Node* nxt);
Node* search(Node* head, int data);
Node* insertion_sort(Node* head);
Node* reverse(Node* head);




Node* create(int data, Node* next) {
	printf("Initializing new LinkedList\n");
	Node* newNode = malloc(sizeof(Node));
	if (newNode == NULL) {
		printf("Error initializing newNode !\n");
		exit(1);
	}
	printf("LinkedList -> adding new value: %i\n", data);
	newNode->data = data;
	newNode->next = next;
	return newNode;
}

Node* prepend(Node* head, int data) {
	Node* newNode = create(data, head);
	head = newNode;
	return head;
}

void traverse(Node* head, callback f) {
    Node* cursor = head;
    while(cursor != NULL) {
        f(cursor);
        cursor = cursor->next;
    }
}

int count(Node *head) {
    Node *cursor = head;
    int c = 0;
    while(cursor != NULL) {
        c++;
        cursor = cursor->next;
    }
    return c;
}


Node* append(Node* head, int data) {
    /* go to the last node */
    Node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;
 
    /* create a new node */
    Node* new_node =  create(data,NULL);
    cursor->next = new_node;
 
    return head;
}


Node* insert_after(Node *head, int data, Node* prev) {
    /* find the prev node, starting from the first node*/
    Node *cursor = head;
    while(cursor != prev)
        cursor = cursor->next;
 
    if(cursor != NULL) {
        Node* new_node = create(data,cursor->next);
        cursor->next = new_node;
        return head;
    } else {
        return NULL;
    }
}

Node* insert_before(Node *head, int data, Node* nxt) {
    if (nxt == NULL || head == NULL)
        return NULL;
 
    if(head == nxt) {
        head = prepend(head,data);
        return head;
    }
 
    /* find the prev node, starting from the first node*/
    Node *cursor = head;
    while(cursor != NULL) {
        if(cursor->next == nxt)
            break;
        cursor = cursor->next;
    }
 
    if(cursor != NULL) {
        Node* new_node = create(data,cursor->next);
        cursor->next = new_node;
        return head;
    } else {
        return NULL;
    }
}

Node* search(Node* head, int data) {
    Node *cursor = head;
    while(cursor!=NULL) {
        if(cursor->data == data)
            return cursor;
        cursor = cursor->next;
    }
    return NULL;
}

Node* insertion_sort(Node* head) {
    Node *x, *y, *e;
 
    x = head;
    head = NULL;
 
    while(x != NULL) {
        e = x;
        x = x->next;
        if (head != NULL) {
            if(e->data > head->data) {
                y = head;
                while ((y->next != NULL) && (e->data> y->next->data)) {
                    y = y->next;
                }
                e->next = y->next;
                y->next = e;
            } else {
                e->next = head;
                head = e ;
            }
        } else {
            e->next = NULL;
            head = e ;
        }
    }
    return head;
}

Node* reverse(Node* head) {
    Node* prev    = NULL;
    Node* current = head;
    Node* next;
    while (current != NULL) {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
    return head;
}
