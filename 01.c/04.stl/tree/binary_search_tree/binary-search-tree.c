//include header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//define structure
struct node
{
    int data;
    struct node *left;
    struct node *right;
};


//create binary search tree
struct node *create(int data)
{
    struct node *newnode;
    newnode = (struct node *)malloc(sizeof(struct node));
    newnode->data = data;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

//insert node in binary search tree
struct node *insert(struct node *root, int data)
{
    if (root == NULL)
    {
        root = create(data);
    }
    else if (data <= root->data)
    {
        root->left = insert(root->left, data);
    }
    else
    {
        root->right = insert(root->right, data);
    }
    return root;
}

//search node in binary search tree
int search(struct node *root, int data)
{
    if (root == NULL)
    {
        return 0;
    }
    else if (root->data == data)
    {
        return 1;
    }
    else if (data <= root->data)
    {
        return search(root->left, data);
    }
    else
    {
        return search(root->right, data);
    }
}

// create print for binary search tree
void printBST(struct node *root)
{
    if (root == NULL)
    {
        return;
    }
    printBST(root->left);
    printf("%d ", root->data);
    printBST(root->right);
}

// create binary search tree and insert values in main function
int main()
{
    struct node *root = NULL;
    root = insert(root, 15);
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 25);
    root = insert(root, 8);
    root = insert(root, 12);

    //print binary search tree
    printBST(root);
}

//output
// 15 10 20 25 8 12
