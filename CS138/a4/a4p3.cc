#include <iostream>
#include <string>
#include <cassert>
using namespace std;

struct Qnode
{
    string val;
    Qnode *next;
};

struct Queue
{
    Qnode *first;
    Qnode *last;
};

struct BST_Node
{
    string key;
    BST_Node *left;
    BST_Node *right;
};

typedef BST_Node *BST;

struct SBLnode
{
    Queue *q;
    BST b;
    int counter;
};

typedef SBLnode *SBL;

void SBL_init(SBL &sbl)
{
    sbl = nullptr;
}

void BST_insert(BST &root, string key)
{
    if (nullptr == root)
    {
        root = new BST_Node;
        root->key = key;
        root->left = nullptr;
        root->right = nullptr;
    }
    else if (key < root->key)
    {
        BST_insert(root->left, key);
    }
    else
    {
        BST_insert(root->right, key);
    }
}

void BST_init(BST &root)
{
    root = nullptr;
}

void SBL_arrive(SBL &sbl, string name)
{

    if (sbl == nullptr)
    {
        Qnode *new_node = new Qnode{};
        new_node->val = name;
        new_node->next = nullptr;

        Queue *new_queue = new Queue{};
        new_queue->first = new_node;
        new_queue->last = new_node;

        BST bst;
        BST_init(bst);

        BST_insert(bst, name);

        SBLnode *new_sbl = new SBLnode{};
        new_sbl->q = new_queue;
        new_sbl->b = bst;
        new_sbl->counter = 1;

        sbl = new_sbl;
    }
    else
    {

        BST_insert(sbl->b, name);

        Qnode *new_node = new Qnode{};
        new_node->val = name;
        new_node->next = nullptr;

        sbl->q->last->next = new_node;
        sbl->q->last = new_node;

        sbl->counter = (sbl->counter + 1);
    }
}

int SBL_size(const SBL &sbl)
{
    if (sbl == nullptr)
    {
        return 0;
    }
    else
    {
        return sbl->counter;
    }
}

BST get_children(BST &root, string key)
{
    if (nullptr == root)
    {
        return nullptr;
    }
    else if (root->key == key)
    {
        return root;
    }
    else if (root->key > key)
    {
        return get_children(root->left, key);
    }
    else
    {
        return get_children(root->right, key);
    }
}

BST get_parent(BST &root, string key)
{
    if (nullptr == root)
    {
        return nullptr;
    }
    else if (root->left && root->left->key == key)
    {
        return root;
    }
    else if (root->right && root->right->key == key)
    {
        return root;
    }
    else if (root->key > key)
    {
        return get_parent(root->left, key);
    }
    else
    {
        return get_parent(root->right, key);
    }
}

string get_largest_down_left(BST &root, string key)
{
    if (nullptr == root)
    {
        return nullptr;
    }
    else if (root->right == nullptr)
    {
        return root->key;
    }
    else
    {
        return get_largest_down_left(root->right, key);
    }
}

bool BST_has(const BST &root, string key)
{
    if (nullptr == root)
    {
        return false;
    }
    else if (root->key == key)
    {
        return true;
    }
    else if (root->key > key)
    {
        return BST_has(root->left, key);
    }
    else
    {
        return BST_has(root->right, key);
    }
}

void BST_remove(BST &root, string key)
{
    if (!BST_has(root, key))
    {
        cerr << "Error, couldn't find \"" << key << "\" in the BST\n";
    }

    assert(BST_has(root, key));

    if ((root->key == key) && (root->left == nullptr) && (root->right == nullptr))
    {
        delete root;
        return;
    }

    BST node = get_children(root, key);

    int n_children = 0;
    if (node->left != nullptr)
    {
        n_children += 1;
    }
    if (node->right != nullptr)
    {
        n_children += 1;
    }

    if (n_children == 0)
    {
        BST parent = get_parent(root, key);

        if (parent->left && parent->left->key == key)
        {
            delete parent->left;
            parent->left = nullptr;
        }
        else if (parent->right && parent->right->key == key)
        {
            delete parent->right;
            parent->right = nullptr;
        }
    }
    else if (n_children == 1)
    {
        BST parent = get_parent(root, key);

        if (parent == nullptr)
        {
            if (root->left)
            {
                root = root->left;
            }

            if (root->right)
            {
                root = root->right;
            }
        }
        else
        {
            if (parent->left && parent->left->key == key)
            {
                if (node->left)
                {
                    parent->left = node->left;
                }
                else if (node->right)
                {
                    parent->left = node->right;
                }
            }
            else if (parent->right && parent->right->key == key)
            {
                if (node->left)
                {
                    parent->right = node->left;
                }
                else if (node->right)
                {
                    parent->right = node->right;
                }
            }
        }
        delete node;
    }
    else if (n_children == 2)
    {
        string key2 = get_largest_down_left(node->left, key);

        BST_remove(root, key2);

        node->key = key2;
    }

    return;
}

void SBL_leave(SBL &sbl)
{
    if ((sbl == nullptr) || (sbl->counter == 0))
    {
        cerr << "Error, SBL is empty." << endl;
    }

    assert((sbl != nullptr) && (sbl->counter != 0));

    if (sbl->q->first == sbl->q->last)
    {
        Qnode *last = sbl->q->last;
        Queue *q = sbl->q;

        BST_remove(sbl->b, last->val);

        sbl->q = nullptr;
        sbl->b = nullptr;
        sbl->counter = 0;

        delete q;
        delete last;
    }
    else
    {
        Qnode *temp = sbl->q->first;
        sbl->q->first = sbl->q->first->next;
        sbl->counter -= 1;

        BST_remove(sbl->b, temp->val);

        delete temp;
    }
}

string SBL_first(const SBL &sbl)
{

    if ((sbl == nullptr) || (sbl->counter == 0))
    {
        cerr << "Error, SBL is empty." << endl;
    }

    assert((sbl != nullptr) && (sbl->counter != 0));

    return sbl->q->first->val;
}

string BST_lookup(const BST &root, string key)
{
    if (nullptr == root)
    {
        return "";
    }
    else if (root->key == key)
    {
        return root->key;
    }
    else if (root->key > key)
    {
        return BST_lookup(root->left, key);
    }
    else
    {
        return BST_lookup(root->right, key);
    }
}

bool SBL_lookup(const SBL &sbl, string name)
{
    if ((sbl != nullptr) && (sbl->counter != 0))
    {
        string key = BST_lookup(sbl->b, name);

        if (key == name)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void SBL_printInArrivalOrder(const SBL &sbl)
{
    if (sbl != nullptr)
    {

        Qnode *curr = sbl->q->first;

        while (curr)
        {
            cout << curr->val << endl;

            curr = curr->next;
        }
    }
}

void BST_print(const BST &root)
{
    if (nullptr != root)
    {
        BST_print(root->left);
        cout << root->key << endl;
        BST_print(root->right);
    }
}

void SBL_printInLexicographicalOrder(const SBL &sbl)
{
    if ((sbl != nullptr) && (sbl->counter != 0))
    {
        BST_print(sbl->b);
    }
}

int main(int argc, char *argv[])
{
    SBL sbl;
    SBL_init(sbl);
    assert(SBL_size(sbl) == 0);
    SBL_printInArrivalOrder(sbl);

    SBL_arrive(sbl, "one");
    assert(SBL_size(sbl) == 1);
    assert(SBL_first(sbl) == "one");

    SBL_arrive(sbl, "two");
    assert(SBL_size(sbl) == 2);
    assert(SBL_first(sbl) == "one");

    SBL_arrive(sbl, "three");
    assert(SBL_size(sbl) == 3);
    assert(SBL_first(sbl) == "one");

    // SBL_printInArrivalOrder(sbl);
    // SBL_printInLexicographicalOrder(sbl);
    assert(SBL_lookup(sbl, "one") == true);
    assert(SBL_lookup(sbl, "four") == false);

    SBL_leave(sbl);
    assert(SBL_size(sbl) == 2);
    assert(SBL_first(sbl) == "two");

    SBL_leave(sbl);
    assert(SBL_size(sbl) == 1);
    assert(SBL_first(sbl) == "three");

    SBL_leave(sbl);
    assert(SBL_size(sbl) == 0);

    delete sbl;

    return 0;
}
