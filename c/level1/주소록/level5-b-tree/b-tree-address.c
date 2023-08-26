#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYS 3
#define DATA_FILE_NAME "Address.dat"

typedef struct _USERDATA {
    char szName[32];
    char szPhone[32];
} USERDATA;

struct BTreeNode {
    int num_keys;
    USERDATA keys[MAX_KEYS];
    struct BTreeNode * children[MAX_KEYS + 1];
    int is_leaf;
};

struct SearchResult {
    struct BTreeNode *node;
    int index;
};

void split_child(struct BTreeNode*, int);


void flush_stdin()
{
    while(getchar() != '\n'); //Q. 왜 EOF가 아닐까?
}


struct BTreeNode * root = NULL;

/***********************************
 * Add()
 */

USERDATA* create_user_data(char *pszName, char *pszPhone)
{
    USERDATA *node = (USERDATA*)malloc(sizeof(USERDATA));
    if(node == NULL)
    {
        printf("Memory Allocation Failed.\n");
        return NULL;
    }
    memset(node, 0, sizeof(USERDATA));
    sprintf(node->szName, "%s", pszName);
    sprintf(node->szPhone, "%s", pszPhone);
    return node;
}


struct BTreeNode * create_node(int is_leaf) {
    struct BTreeNode * new_node = (struct BTreeNode *) malloc(sizeof(struct BTreeNode));
    new_node->num_keys = 0;
    new_node->is_leaf = is_leaf;
    for (int i = 0; i < MAX_KEYS + 1; i++) {
        new_node->children[i] = NULL;
    }
    return new_node;
}

void insert_non_full(struct BTreeNode * node, USERDATA key) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        while (i >= 0 && strcmp(key.szName, node->keys[i].szName) < 0) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && strcmp(key.szName, node->keys[i].szName) < 0) {
            i--;
        }
        i++;
        if (node->children[i]->num_keys == MAX_KEYS) {
            split_child(node, i);
            if (strcmp(key.szName, node->keys[i].szName) > 0) {
                i++;
            }
        }
        insert_non_full(node->children[i], key);
    }
}


//해당 노드에 keys들이 꽉 찬 경우(3이면) split!
void split_child(struct BTreeNode * parent, int child_index) { //parent는 꽉찬 노드의 부모, child_index는 부모가 가진 child중에 몇번째 자식을 쪼개고 싶은지.
    struct BTreeNode * child = parent->children[child_index]; //split이후, right_child가 될 놈
    struct BTreeNode * new_child = create_node(child->is_leaf); //split이후, left_child가 될 놈
    new_child->num_keys = (MAX_KEYS - 1) / 2;  // This is floor division, so for MAX_KEYS = 3, the result is 1.
    for (int j = 0; j < new_child->num_keys; j++) { //j < 1
        new_child->keys[j] = child->keys[j + child->num_keys / 2 + 1]; //child에 오른쪽에 있던 키들을 new_child의 키로 옮긴다.
    }
    if (!child->is_leaf) {
        for (int j = 0; j <= new_child->num_keys; j++) {
            new_child->children[j] = child->children[j + child->num_keys / 2 + 1];
        }
    }
    child->num_keys = child->num_keys / 2; //기존 child에 있던 키들을 반갈죽해서 new_child로 넘겼으니까, key의 숫자를 반갈죽한다.


    //parent의 children[]을 adjust
    for (int j = parent->num_keys; j > child_index; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[child_index + 1] = new_child;

    //parent의 keys를 adjust
    for (int j = parent->num_keys - 1; j >= child_index; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[child_index] = child->keys[child->num_keys];
    parent->num_keys++;
}



void insert(struct BTreeNode **root, USERDATA *data) {
    struct BTreeNode *root_node = *root;

    // Check if root node is full
    if (root_node->num_keys == MAX_KEYS) {
        struct BTreeNode *new_root = create_node(0);
        new_root->children[0] = root_node;

        split_child(new_root, 0);
        insert_non_full(new_root, *data);

        *root = new_root;
    } else {
        // Insert into a non-full root node
        insert_non_full(root_node, *data);
    }
}

int Add()
{
    //missed - char szName[32], szPhone[32] 생성
    char szName[32] = { 0 };
    char szPhone[32] = { 0 };

    //missed - clear stdin
    flush_stdin();

    //missed - fgets()로 입력값을 szName, szPhone으로 받아옴
    printf("Input name: ");
    fgets(szName, sizeof(szName), stdin);

    printf("Input phone number: ");
    fgets(szPhone, sizeof(szPhone), stdin);

    USERDATA *userData = create_user_data(szName, szPhone);

    insert(&root, userData);

    return 1;
}


/***************************************
 * Search()
 */


struct SearchResult* search_by_key(struct BTreeNode *x, char *k) {
    int i = 0;
    while (i < x->num_keys && strcmp(k, x->keys[i].szName) > 0) {
        i++;
    }
    if (i < x->num_keys && strcmp(k, x->keys[i].szName) == 0) {
        struct SearchResult *res = (struct SearchResult*)malloc(sizeof(struct SearchResult));
        res->node = x;
        res->index = i;
        return res;
    } else if (x->is_leaf) {
        return NULL;
    } else {
        return search_by_key(x->children[i], k);
    }
}


int Search()
{
    char szInputName[32];
    printf("Input name: ");
    flush_stdin();
    fgets(szInputName, sizeof(szInputName), stdin);

    struct SearchResult *pSearchResult = search_by_key(root, szInputName);

    if(pSearchResult == NULL)
        puts("ERROR: 데이터를 찾을 수 없습니다.");
    else
    {
        printf("%s exists!\n", szInputName);
        free(pSearchResult);
    }

    flush_stdin();
}


/******************************
 * Print()
 */

int PrintUI()
{
    int nInput = 0;
    system("clear"); //clear screen on command
    printf("[1] Add\t [2] Search\t [3] Print all\t [4] Remove\t [0] Exit\n:");

    scanf("%d", &nInput);
    return nInput;
}


void print_tree(struct BTreeNode *node) {
    for (int i = 0; i < node->num_keys; i++) {
        // Print children first if it's not a leaf node
        if (!node->is_leaf) {
            print_tree(node->children[i]);
        }

        printf("[%p] %s %s\n ", node, node->keys[i].szName, node->keys[i].szPhone); // Printing string key
    }

    // Print the last child if the node is not a leaf
    if (!node->is_leaf) {
        print_tree(node->children[node->num_keys]);
    }
}

void PrintAll()
{
    printf("B-Tree Contents: ");
    print_tree(root);
    printf("\n");

    getchar();
    flush_stdin();
}



/***********************************
 * 기본 설정
 */

int main()
{
    root = create_node(1);

    int nMenu = 0;
    /* LoadList(DATA_FILE_NAME); */

    //main event loop
    while((nMenu = PrintUI()) != 0) //GUI
    {
        switch(nMenu) { //event input
            case 1:
                Add();
                break;
            case 2:
                Search();
                break;
            case 3:
                PrintAll();
                break;
            /* case 4: */
            /*     Remove(); */
            /*     break; */
        }
    }

    /* Save(DATA_FILE_NAME); */
    /* ReleasePostOrder(g_Head); */

    return 0;
}
