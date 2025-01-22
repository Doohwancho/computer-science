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

    if (a->value < b->value) { //무조건 a(usually root)가 b 보다 더 커야함
        Node* temp = a;
        a = b;
        b = temp;
    }

	//a(root)->right에 새 노드를 붙임 through recursive call
    a->right = merge(a->right, b);

	//swap(a->left, a->right);
    Node* temp = a->left;
    a->left = a->right;
    a->right = temp;

	//return a(root)
    return a;
}

Node* insert(Node* root, int value) {
    Node* newNode = createNode(value);
    return merge(root, newNode);
}

//Q. why used double pointer for Node** root?
//A. if you pass a pointer to a function, any modifications made to the pointer itself will not be reflected outside the function. However, by using a double pointer, you can modify the value of the original pointer from within the function.

//function needs to update the `root` of the max heap if an element is removed
int extractMax(Node** root) {
    if (*root == NULL) { //A single pointer `Node*` would allow you to modify the contents of the `root` node (i.e., the value, left, and right pointers), but it would not allow you to update the `root` variable itself.
        return 0;
    }

    int max = (*root)->value;

    Node* temp = *root;
    *root = merge((*root)->left, (*root)->right); //update root
    free(temp); //free memory

    return max;
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
                printf("%d\n", extractMax(&root));
            }
        } else {
            root = insert(root, x);
        }
    }

    return 0;
}

