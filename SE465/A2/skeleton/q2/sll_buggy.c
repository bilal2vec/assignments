/* Singly-linked list implementation */
/* Any first-year student could write this, 
 * and it looks like a first-year student implementation, yes? */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* nodes for singly-linked lists */
struct node
{
    char *str;
    long int data;
    struct node *next;
};

/* global pointer to list head */
struct node *p;

int count();

/* Read a newline-terminated string from f into a buffer. */
char *fgets_enhanced(FILE *f)
{
    int chunksize = 4;
    char *s;
    char *temp;
    int buffersize = chunksize+1;
    char *nPos;
    
    s = malloc(buffersize);    
    if (s == NULL)
    {
        printf("Can't allocate %d bytes\n", buffersize);
        return NULL;
    }

    /* read the first chunk with standard library fgets */
    if (fgets(s, chunksize+1, f) == NULL)
    {
        printf("fgets returned NULL\n");
    } else {
        nPos = strchr(s, '\n');
        while (nPos == NULL)
        {
            buffersize += chunksize;
            temp = realloc(s, buffersize);
            if (temp == NULL)
            {
                printf("Can't realloc %d bytes\n", chunksize);
                free(s); /* clean up previously allocated buffer */
                s = NULL;
                break;
            } else {
                s = temp;
                /* read starting at the 0 that ended the previous chunk */
                if (fgets(s+buffersize-chunksize-1, chunksize+1, f) == NULL)
                {
                    printf("fgets returned NULL\n");
                    break;
                }
            }
            nPos = strchr(s, '\n');
        }
    }
    /* ensure that we zero-terminate the returned buffer */
    s[nPos-s] = 0;
    return s;
}

/* Delete the first node with the provided arg as data. */
void delete_node(long int num)
{
    struct node *temp, *m;
    temp=p;
    while (temp!=NULL)
    {
        if (temp->data==num)
        {
            if (temp==p)
            {
                p = temp->next;
            } else {
                m->next = temp->next;
            }
            free(temp);
            return;
        } else {
            m = temp;
            temp = temp->next;
        }
    }
    printf("ELEMENT %ld NOT FOUND\n", num);
}

void delete_all()
{
    struct node *temp,*temp2;

    temp=p;
    while (temp!=NULL)
    {
        temp2 = temp;
        temp = temp->next;

        free(temp2->str);
        free(temp2);
    }
}

void prepend(long int num, char* name)
{
    /* create new node, populate with data */
    struct node *temp;
    temp = malloc(sizeof(struct node));
    temp->data = num;
    temp->str = name;

    /* usual linked-list implementation drill with p null vs non null */
    if (p==NULL)
    {
        p = temp;
        p->next = NULL;
    } else {
        temp->next = p;
        p = temp;
    }
}

void add_after(long int num, int loc, char* name)
{
    struct node *r = p;
    if(loc > count()+1 || loc <= 0)
    {
        /* boy, that's a bad error message! */
        printf("insertion is not possible!\n");
        return;
    }
    /* add at beginning when p is null */
    if (loc == 1)
    {
        prepend(num,name);
        return;
    } else {
        struct node *temp, *n;

        for(int i = 1; i<loc; i++)
        {
            n = r;
            r = r->next;
        }
        temp = malloc(sizeof(struct node));
        temp->data = num;
        temp->str = name;
        n->next = temp;
        n = temp;
        n->next = r;
        return;
    }
}

/* Modify the first node with oldNum in data as specified. */
void edit(long int oldNum, long int newNum, char* name)
{
    struct node *temp = p, *m;
    while (temp!=NULL)
    {
        if(temp->data==oldNum)
        {
            temp->data = newNum;
            free (temp->str);
            temp->str = name;
            return;
        } else {
            m = temp;
            temp = temp->next;
        }
    }
    printf("ELEMENT %ld NOT FOUND\n", oldNum);
}

/* Duplicate given node, inserting copy before original. */
void duplicate(long int num)
{
    struct node *temp = p;
    int i = 1;
    while (temp!=NULL)
    {
        if (temp->data==num)
        {
            long len = strlen(temp->str)>0 ? strlen(temp->str) : 0;
            char *name = malloc(len);
            strcpy(name, temp->str);
            add_after(temp->data, i, name);
            return;
        } else {
            i++;
            temp= temp->next;
        }
    }
    printf("ELEMENT %ld NOT FOUND \n", num);
}

void append(long int num, char* name)
{
    /* create new node, populate with data */
    struct node *temp = malloc(sizeof(struct node));
    temp->data = num;
    temp->str = name;

    struct node *r = p;    
    /* add at beginning when p is null */
    if (p == NULL)
    {
        p = temp;
        p->next = NULL;
    } else {
        /* add to end */
        while (r->next != NULL)
            r=r->next;
        r->next = temp;
        r = temp;
        r->next = NULL;
    }
}

void display()
{
    struct node *r = p;
    if (r==NULL)
    {
        printf("NO ELEMENTS IN THE LIST\n");
        return;
    }

    while (r!=NULL)
    {
        printf(" -> %li",r->data);
        printf(", %s",r->str);
        
        r = r->next;
    }
    printf("\n");
}

int count()
{
    struct node *n = p;
    int c = 0;

    while (n!=NULL)
    {
        n=n->next;
        c++;
    }
    return c;
}

int main()
{
    p = NULL;

    while (1)
    {
        printf("[(i)nsert,(d)elete,delete (a)ll,d(u)plicate,(e)dit,(p)rint,e(x)it]:");

        char c;
        scanf("%s",&c); /* ENTER A VALUE FOR SWITCH */

        switch (tolower(c))
        {
            case 'i':
            {
                long int num;
                printf("enter the tel:>");
                scanf("%li", &num);
                printf("enter the name:>");
                getchar();
                append(num, fgets_enhanced(stdin));
                break;
            }
            case 'd':
            {
                long int num;
                printf("enter the tel :>");
                scanf("%li", &num);
                delete_node(num);
                break;
            }
            case 'e':
            {
                long int old_num, new_num;
                char* name;
                printf("enter the old tel :>");
                scanf("%li", &old_num);
                printf("enter the new tel :>");
                scanf("%li", &new_num);
                printf("enter the new name:>");
                getchar();
                name = fgets_enhanced(stdin);
                edit(old_num, new_num, name);
                break;
            }
            case 'p':
            {
                printf("The elements are :> ");
                display();
                break;
            }
            case 'a':
            {
                delete_all();
                break;
            }
            case 'u':
            {
                long int num;
                printf("enter the tel :>");
                scanf("%li",&num);
                duplicate(num);
                break;
            }
            case 'x':
            {
                delete_all();
                printf("bye\n");
                return 0;
            }
        }
    }
}
