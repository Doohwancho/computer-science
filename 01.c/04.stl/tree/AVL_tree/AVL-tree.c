//include library for this program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//create fmax function for

// create a AVLNode
typedef struct AVLNode {
    struct AVLNode *left;
    struct AVLNode *right;
    int data;
    int height;
} AVLNode;

// create AVL tree
struct AVLNode *createAVLTree(int data) {
    struct AVLNode *newNode = (struct AVLNode *)malloc(sizeof(struct AVLNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

// get height of AVL tree
int getHeight(struct AVLNode *root) {
    if (root == NULL) {
        return 0;
    }
    return root->height;
}

// get balance factor of AVL tree
int getBalanceFactor(struct AVLNode *root) {
    if (root == NULL) {
        return 0;
    }
    return getHeight(root->left) - getHeight(root->right);
}

// right rotate
struct AVLNode *rightRotate(struct AVLNode *root) {
    struct AVLNode *newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;
    root->height = fmax(getHeight(root->left), getHeight(root->right)) + 1;
    newRoot->height = fmax(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;
    return newRoot;
}

// left rotate
struct AVLNode *leftRotate(struct AVLNode *root) {
    struct AVLNode *newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;
    root->height = fmax(getHeight(root->left), getHeight(root->right)) + 1;
    newRoot->height = fmax(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;
    return newRoot;
}

// insert node
struct AVLNode *insertAVLTree(struct AVLNode *root, int data) {
    if (root == NULL) {
        return createAVLTree(data);
    }
    if (data < root->data) {
        root->left = insertAVLTree(root->left, data);
    } else if (data > root->data) {
        root->right = insertAVLTree(root->right, data);
    } else {
        return root;
    }
    root->height = fmax(getHeight(root->left), getHeight(root->right)) + 1;
    int balanceFactor = getBalanceFactor(root);
    if (balanceFactor > 1 && data < root->left->data) {
        return rightRotate(root);
    }
    if (balanceFactor < -1 && data > root->right->data) {
        return leftRotate(root);
    }
    if (balanceFactor > 1 && data > root->left->data) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balanceFactor < -1 && data < root->right->data) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// find minimum node
struct AVLNode *findMinNode(struct AVLNode *root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// delete node
struct AVLNode *deleteAVLTree(struct AVLNode *root, int data) {
    if (root == NULL) {
        return NULL;
    }
    if (data < root->data) {
        root->left = deleteAVLTree(root->left, data);
    } else if (data > root->data) {
        root->right = deleteAVLTree(root->right, data);
    } else {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        } else if (root->left == NULL || root->right == NULL) {
            struct AVLNode *temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        } else {
            struct AVLNode *temp = findMinNode(root->right);
            root->data = temp->data;
            root->right = deleteAVLTree(root->right, temp->data);
        }
    }
    root->height = fmax(getHeight(root->left), getHeight(root->right)) + 1;
    int balanceFactor = getBalanceFactor(root);
    if (balanceFactor > 1 && getBalanceFactor(root->left) >= 0) {
        return rightRotate(root);
    }
    if (balanceFactor < -1 && getBalanceFactor(root->right) <= 0) {
        return leftRotate(root);
    }
    if (balanceFactor > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balanceFactor < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// print AVL tree
void printAVLTree(struct AVLNode *root) {
    if (root == NULL) {
        return;
    }
    printAVLTree(root->left);
    printf("%d ", root->data);
    printAVLTree(root->right);
}

// main function
int main() {
    struct AVLNode *root = NULL;
    root = insertAVLTree(root, 10);
    root = insertAVLTree(root, 20);
    root = insertAVLTree(root, 30);
    root = insertAVLTree(root, 40);
    root = insertAVLTree(root, 50);
    root = insertAVLTree(root, 25);
    printAVLTree(root);
}


