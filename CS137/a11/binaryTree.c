#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

struct bstnode
{
    int item;
    struct bstnode *left;
    struct bstnode *right;
};
struct bst
{
    struct bstnode *root;
    int count; // how many integers are in the tree
};

struct bst *bst_create(void)
{
    struct bst *t = malloc(sizeof(struct bst));
    t->root = NULL;
    t->count = 0;
    return t;
}
void traversedestroy(struct bstnode *node)
{
    if (node->left)
    {
        traversedestroy(node->left);
    }

    if (node->right)
    {
        traversedestroy(node->right);
    }

    free(node);
}
void bst_destroy(struct bst *t)
{
    if (t->root)
    {
        traversedestroy(t->root);
    }
    free(t);
}

void bst_insert(int num, struct bst *t)
{
    if (t->root)
    {
        struct bstnode *node = t->root;

        while (node)
        {

            if (node->item == num)
            {
                break;
            }
            else if (num < node->item)
            {
                if (node->left)
                {
                    node = node->left;
                }
                else
                {
                    struct bstnode *new = malloc(sizeof(struct bstnode));
                    new->item = num;
                    new->left = NULL;
                    new->right = NULL;

                    node->left = new;
                    t->count += 1;
                }
            }
            else if (num > node->item)
            {
                if (node->right)
                {
                    node = node->right;
                }
                else
                {
                    struct bstnode *new = malloc(sizeof(struct bstnode));
                    new->item = num;
                    new->left = NULL;
                    new->right = NULL;

                    node->right = new;
                    t->count += 1;
                }
            }
        }
    }
    else
    {
        struct bstnode *node = malloc(sizeof(struct bstnode));
        node->item = num;
        node->left = NULL;
        node->right = NULL;

        t->root = node;
        t->count += 1;
    }
}

void traverseprint(struct bstnode *node)
{
    if (node->left)
    {
        // printf("left\n");
        traverseprint(node->left);
    }
    printf("%d ", node->item);
    if (node->right)
    {
        // printf("right\n");
        traverseprint(node->right);
    }
}

void bst_print(struct bst *t)
{
    if (t)
    {
        traverseprint(t->root);
        printf("\n");
    }
}
int bst_min(struct bst *t)
{
    if (t->root)
    {
        struct bstnode *node = t->root;

        while (node->left)
        {
            node = node->left;
        }
        return node->item;
    }
}
int bst_max(struct bst *t)
{
    if (t->root)
    {
        struct bstnode *node = t->root;

        while (node->right)
        {
            node = node->right;
        }
        return node->item;
    }
}

int main(void)
{
    struct bst *tree = bst_create();
    bst_insert(100, tree);
    bst_insert(150, tree);
    bst_insert(78, tree);
    bst_insert(88, tree);
    assert(bst_min(tree) == 78);
    assert(bst_max(tree) == 150);
    bst_print(tree);
    bst_insert(-130, tree);
    assert(tree->count == 5);
    bst_destroy(tree);
}