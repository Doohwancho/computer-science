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

void swapValues(Node* a, Node* b) {
    int temp = a->value;
    a->value = b->value;
    b->value = temp;
}

//가장 작은 값을 root->value로 유지시키는 함수
void heapify(Node* root) {
    Node* smallest = root;

	//1. root, root->left, root->right 3중을 비교해 제일 작은 value를 가진 노드를 smallest로 추출
    if (root->left != NULL && root->left->value < smallest->value) {
        smallest = root->left;
    }

    if (root->right != NULL && root->right->value < smallest->value) {
        smallest = root->right;
    }


    if (smallest != root) {
	    //2. smallest가 root가 아니면, root와 root->left | root->right의 value를 바꾸고
        swapValues(root, smallest);

		//3. root->left|right 의 값에 원래 root값이 들어와있을테니, root->left|right를 root삼아 recursive
        heapify(smallest);
    }
}

Node* insert(Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }

    if (value < root->value) {
        root->left = insert(root->left, value);
    } else {
        root->right = insert(root->right, value);
    }

	heapify(root); // Ensure min-heap property after insertion
    return root;
}

//root->value(최솟값) 반환하기 전에 children 재정렬
int removeMin(Node** root) {
    if (*root == NULL) {
        return 0;
    }

    int min = (*root)->value;

	//root의 양쪽 애 다 살아있다면?
    if ((*root)->left != NULL && (*root)->right != NULL) {
	    //왼쪽애가 오른쪽애보다 더 값이 작으면?
        if ((*root)->left->value < (*root)->right->value) {
	        //더 작은 왼쪽애의 값이 root의 값이 되고,
            (*root)->value = (*root)->left->value;
	        //removeMin()을 왼쪽애를 넣어 재귀
            removeMin(&((*root)->left));
        } else {
	        //오른쪽 애의 값이 왼쪽애의 값보다 더 작으면?
	        //오른쪽 애의 값이 root의 값이 되고
            (*root)->value = (*root)->right->value;
            //removeMin()을 오른쪽 애를 넣어 재귀
            removeMin(&((*root)->right));
        }
    //root->left만 살아있다면?
    } else if ((*root)->left != NULL) {
	    //root->left에 있던 값을 root->value로 올리고
        (*root)->value = (*root)->left->value;
        //root->left를 재귀로 removeMin() 한다.
        removeMin(&((*root)->left));
    //root->right만 살아있다면?
    } else {
        Node* temp = *root;
        //root->right로 갈아타고
        *root = (*root)->right;
        //원래 root를 버림. 어짜피 root->left 없었으니까.
        free(temp);
    }

    return min;
}

int main() {
    int N;
    scanf("%d", &N);

    Node* root = NULL;

    for (int i = 0; i < N; i++) {
        int x;
        scanf("%d", &x);

        if (x == 0) {
            printf("%d\n", removeMin(&root));
        } else {
            root = insert(root, x);
        }
    }

    return 0;
}
