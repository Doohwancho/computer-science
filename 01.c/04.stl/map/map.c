#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node
{
    char *key;
    void *value;
    struct _node *next;
} Node;

typedef struct _map
{
    Node *head;
    int size;
} Map; //구조가 hashmap과 같네? 차이점은 map에 head[index] 넣을 때, index를 겹치게 넣지 않으려고 hash algorithm을 쓰냐 안쓰냐 차이인 듯

Map *map_create()
{
    Map *map = malloc(sizeof(Map));
    map->head = NULL;
    map->size = 0;
    return map;
}

Node *map_find(Map *map, char *key)
{
    Node *node = map->head;
    while(node != NULL)
    {
        if (strcmp(node->key, key) == 0) //string이 같다면,
            return node;
        node = node->next; //index는 같은데 key가 다른 경우, linkedlist에 다음 node로 넘어가 다시 비교
    }
    return NULL;
}

void map_set(Map *map, char *key, void *value)
{
    Node *node = map_find(map, key);
    if (node != NULL) //problem! does not check duplicated keys. if input with the same key twice, ladder will override previous Node -> terrible! use hash algorithm to avoid duplicated key!
    {
        node->value = value;
    }
    else
    {
        Node *new_node = malloc(sizeof(Node));
        new_node->key = key;
        new_node->value = value;
        new_node->next = map->head;
        map->head = new_node;
        map->size++;
    }
}

void *map_get(Map *map, char *key)
{
    Node *node = map_find(map, key);
    if (node == NULL)
        return NULL;
    return node->value;
}

void map_destroy(Map *map)
{
    Node *node = map->head;
    while(node != NULL)
    {
        Node *temp = node;
        node = node->next;
        free(temp);
    }
    free(map);
}

int main(void)
{
    Map *map = map_create();
    map_set(map, "foo", "bar");
    printf("%s\n", (char *)map_get(map, "foo"));
    map_destroy(map);
    return 0;
}
