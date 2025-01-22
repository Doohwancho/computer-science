typedef struct Node {
    int val;
    struct Node *next;
} Node;

typedef struct MyLinkedList{
    struct Node *head;
} MyLinkedList;

Node* createNode(int val)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->val = val;
    return newNode;
}

MyLinkedList* myLinkedListCreate() {
    MyLinkedList *linkedList = (MyLinkedList*)malloc(sizeof(MyLinkedList));
    linkedList->head = NULL;
    return linkedList;
}

int myLinkedListGet(MyLinkedList* obj, int index) {
    Node *curr = obj->head;
    if(!curr) return -1;
    for(int i = 0; i < index; i++)
    {
        if(curr->next)
        {
            curr = curr->next;
        }
        else
        {
            return -1;
        }
    }
    return curr->val;
}


//Add a node of value val before the first element of the linked list
void myLinkedListAddAtHead(MyLinkedList* obj, int val) {
    // Node *head = obj->head;
    // Node *newHeader = createNode(val);

    // if(!head)
    //     head = newHeader; //<- 이게 틀린 이유: obj에 붙이는게 아니라, 새 포인터 head에 붙여서
    if(obj->head == NULL)
        obj->head = createNode(val);
    else
    {
        struct Node *temp = obj->head;
        obj->head = createNode(val);
        obj->head->next = temp;
    }
}

void myLinkedListAddAtTail(MyLinkedList* obj, int val) {
    Node *head = obj->head;

    if(!head)
        myLinkedListAddAtHead(obj, val);
    else
    {
        while(head->next != NULL)
        {
            head = head->next;
        }
        head->next = createNode(val);
    }
}

//if [1,3]에서, addAtIndex(1,2) 하면, [1,2,3]이 된다
//if [1,2,4,5], addAtIndex(2,3) -> [1,2,3,4,5]
void myLinkedListAddAtIndex(MyLinkedList* obj, int index, int val) {
    if(index == 0)
    {
        myLinkedListAddAtHead(obj,val);
        return;
    }

    Node *head = obj->head;
    if(!head)
        return;

    while(head->next != NULL && index-1 > 0)
    {
        head = head->next;
        index--;
    }
    Node *temp = head->next;
    Node *newNode = createNode(val);
    head->next = newNode;
    head->next->next = temp;
}

//메모리 해제도 고려해야 함
void myLinkedListDeleteAtIndex(MyLinkedList* obj, int index) {
    if(!obj->head) return;

    Node *curr = obj->head;

    if(index == 0 && !curr->next)
    {
        obj->head = NULL;
        free(curr);
        return;
    }

    if(index == 0 && curr->next)
    {
        obj->head = curr->next;
        free(curr);
        return;
    }

    if(curr->next) //if index > 0 and curr doesnt have next, do nothing
    {
        Node *deleteNode;
        for(int i = 1; i < index; i++)
        {
            if(curr->next->next)
                curr = curr->next;
            else  //index가 특정 위치 이상 초과한 상태 무시
                return;
        }

        deleteNode = curr->next;
        curr->next = deleteNode->next;
        free(deleteNode);
    }
}

//메모리 해제도 고려해야 함
void myLinkedListFree(MyLinkedList* obj) {
    Node *temp = obj->head;
    Node *next;

    if(temp == NULL) return;
    else if(temp->next == NULL)
    {
        free(temp);
    }
    else if(temp && temp->next)
    {
        next = temp->next;
        free(temp);
        while(next->next)
        {
            temp = next;
            next = next->next;
            free(temp);
        }
        free(next);
    }
}

