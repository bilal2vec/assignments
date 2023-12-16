#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <map>

class Node
{
public:
    int value;
    int n_children;
    std::vector<Node *> children;

    Node(int value, int n_children) : value(value), n_children(n_children) {}

    void add_child(Node *child)
    {
        children.push_back(child);
    }

    ~Node()
    {
        for (Node *node : children)
        {
            delete node;
        }
    }
};

Node *create_tree()
{
    int value, n_children;
    std::cin >> value >> n_children;

    Node *node = new Node(value, n_children);

    for (int i = 0; i < n_children; i++)
    {
        Node *child = create_tree();
        node->add_child(child);
    }

    return node;
}

// void print_tree(Node *root)
// {
//     std::cout << root->value << " " << root->n_children << " (";
//     for (int i = 0; i < root->n_children; i++)
//     {
//         std::cout << root->children[i]->value << ",";
//     }
//     std::cout << ")" << std::endl;

//     for (int i = 0; i < root->n_children; i++)
//     {
//         print_tree(root->children[i]);
//     }
// }

void print_tree_post_order(Node *root)
{

    for (int i = 0; i < root->n_children; i++)
    {
        print_tree_post_order(root->children[i]);
    }

    std::cout << root->value << " " << root->n_children << std::endl;
}

int main()
{

    Node *root = create_tree();

    print_tree_post_order(root);

    delete root;

    return 0;
}
