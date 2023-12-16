#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct dlnode
{
    int data;
    struct dlnode *next;
    struct dlnode *prev;
} dlnode;

typedef struct ndlst
{
    dlnode *head; //points to the first element otherwise NULL
    dlnode *tail; //points to the last element otherwise NULL
    int len;      // number of elements
} ndlst;

struct ndlst *dlistCreate(void)
{
    ndlst *ret = malloc(sizeof(ndlst));
    ret->head = NULL;
    ret->tail = NULL;
    ret->len = 0;
    return ret;
}

void dlistDestroy(struct ndlst *lst)
{
    dlnode *node = lst->head;
    while (node)
    {
        dlnode *next = node->next;
        free(node);
        node = next;
    }
    free(lst);
}

int dlistLength(struct ndlst *lst)
{
    return lst->len;
}

void dlistRemoveElem(struct ndlst *lst, int n)
{
    int i = 1;
    dlnode *node = lst->head;
    while (i < n)
    {
        node = node->next;
        i += 1;
    }

    dlnode *prev = node->prev;
    dlnode *next = node->next;

    if (prev)
    {
        node->prev->next = node->next;
    }
    else
    {
        lst->head = node->next;
    }

    if (next)
    {
        node->next->prev = node->prev;
    }
    else
    {
        lst->tail = node->prev;
    }

    free(node);
    lst->len -= 1;
}

void dlistAddToFront(ndlst *lst, int elem)
{
    dlnode *new = malloc(sizeof(dlnode));
    new->data = elem;
    new->next = lst->head;
    new->prev = NULL;

    if (lst->len != 0)
    {
        lst->head->prev = new;
    }

    lst->head = new;
    lst->len += 1;
}
void dlistAddToEnd(ndlst *lst, int elem)
{
    dlnode *new = malloc(sizeof(dlnode));
    new->data = elem;
    new->next = NULL;
    new->prev = lst->tail;

    if (lst->len == 0)
    {
        lst->head = new;
    }
    else
    {
        lst->tail->next = new;
    }
    lst->tail = new;
    lst->len += 1;
}

// void dlistPrint(struct ndlst *lst)
// {
//     dlnode *node = lst->head;
//     for (; node; node = node->next)
//         printf("%d  ", node->data);
//     printf("\n");
// }
// void dlistPrintReverse(struct ndlst *lst)
// {
//     dlnode *node = lst->tail;
//     for (; node; node = node->prev)
//         printf("%d  ", node->data);
//     printf("\n");
// }

// int main(void)
// {
//     struct ndlst *lst1 = dlistCreate();
//     printf("length = %d\n", dlistLength(lst1));
//     dlistAddToEnd(lst1, 10);
//     dlistAddToFront(lst1, -20);
//     dlistAddToFront(lst1, 0);
//     dlistAddToEnd(lst1, 9);
//     dlistAddToFront(lst1, -9);
//     dlistAddToFront(lst1, 7);
//     dlistAddToEnd(lst1, 40);
//     printf("length = %d\n", lst1->len);
//     dlistPrint(lst1);
//     dlistPrintReverse(lst1);
//     dlistRemoveElem(lst1, 5);
//     dlistRemoveElem(lst1, 1);
//     dlistRemoveElem(lst1, 5);
//     dlistAddToEnd(lst1, 100);
//     dlistPrint(lst1);
//     dlistPrintReverse(lst1);
//     dlistDestroy(lst1);
//     return 0;
// }