#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* merge(Node* a, Node* b) {
    if (a == NULL) {
        return b;
    }
    if (b == NULL) {
        return a;
    }

    if (a->value > b->value) {
        Node* temp = a;
        a = b;
        b = temp;
    }

    a->right = merge(a->right, b);
    Node* temp = a->left;
    a->left = a->right;
    a->right = temp;

    return a;
}

Node* insert(Node* root, int value) {
    Node* newNode = createNode(value);
    return merge(root, newNode);
}

int extractMin(Node** root) {
    if (*root == NULL) {
        return 0;
    }

    int min = (*root)->value;
    Node* temp = *root;
    *root = merge((*root)->left, (*root)->right);
    free(temp);
    return min;
}

int main() {
    Node* root = NULL;

    int N;
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        int x;
        scanf("%d", &x);

        if (x == 0) {
            if (root == NULL) {
                printf("0\n");
            } else {
                printf("%d\n", extractMin(&root));
            }
        } else {
            root = insert(root, x);
        }
    }

    return 0;
}

