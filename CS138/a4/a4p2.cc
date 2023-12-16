#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// Definitions from class
struct BST_Node
{
    string key;
    string stuff;
    BST_Node *left;
    BST_Node *right;
};

typedef BST_Node *BST;

void BST_init(BST &root)
{
    root = nullptr;
}

bool BST_isMT(const BST &root)
{
    return nullptr == root;
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

string BST_lookup(const BST &root, string key)
{
    if (nullptr == root)
    {
        return "";
    }
    else if (root->key == key)
    {
        return root->stuff;
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

void BST_insert(BST &root, string key, string stuff)
{
    if (nullptr == root)
    {
        root = new BST_Node;
        root->key = key;
        root->stuff = stuff;
        root->left = nullptr;
        root->right = nullptr;
    }
    else if (key < root->key)
    {
        BST_insert(root->left, key, stuff);
    }
    else
    {
        BST_insert(root->right, key, stuff);
    }
}

void BST_print(const BST &root)
{
    if (nullptr != root)
    {
        BST_print(root->left);
        cout << root->key << " " << root->stuff << endl;
        BST_print(root->right);
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

int main(int argc, char *argv[])
{
    BST bst;
    BST_init(bst);

    // BST_insert(bst, "one", "");
    // BST_insert(bst, "two", "");
    // BST_insert(bst, "three", "");

    // BST_remove(bst, "one");
    // BST_remove(bst, "two");
    // BST_remove(bst, "three");

    BST_insert(bst, "llama", "");
    BST_insert(bst, "dingo", "");
    BST_insert(bst, "rat", "");
    BST_insert(bst, "cat", "");
    BST_insert(bst, "elephant", "");
    BST_insert(bst, "tiger", "");
    BST_insert(bst, "alpaca", "");
    BST_insert(bst, "dog", "");
    BST_insert(bst, "rhino", "");
    BST_insert(bst, "yellowfin", "");

    BST_remove(bst, "alpaca");
    BST_remove(bst, "dog");
    BST_remove(bst, "rhino");
    BST_remove(bst, "yellowfin");
    BST_remove(bst, "cat");
    BST_remove(bst, "elephant");
    BST_remove(bst, "tiger");
    BST_remove(bst, "dingo");
    BST_remove(bst, "rat");
    BST_remove(bst, "llama");

    // BST_remove(bst, "llama");

    return 0;
}
