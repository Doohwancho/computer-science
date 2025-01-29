#include <stdio.h>
#include <stdlib.h>
#define MAX_TABLE_SIZE 10

typedef struct entry {
  char *key;
  int value;
  struct entry *next;
} entry;

//summary

//key:value인 entry가 array로 저장되는 hash table.
//pointer들이 담긴 array고, 각 pointer는 linked list인 entry를 가르킴.
//왜 entry는 linked list냐면, hash_function()해서 나온 index로 쓰이는 키 값을 구할 때, % MAX_TABLE_SIZE로 구하는데, 나머지 연산 후에 값이 같이 나올 수 있잖아? 인덱스 겹치면 linkedlist에 다음 node에 붙이는 식으로 구현함.

typedef struct {
  entry* table[MAX_TABLE_SIZE];
} hash_table;

int hash_function(char *key) { //"abc"를 입력 받아서, 97+98+99=294를 10으로 나눈 나머지인 4를 리턴. 이게 hash key 값이 된다.
  int hash = 0;
  for(int i = 0; key[i] != '\0'; i++) {
    hash = hash + key[i];
  }
  return hash % MAX_TABLE_SIZE;
}

void insert_entry(hash_table *ht, char *key, int value) {
  int hash = hash_function(key); //hash key 값 구하기(0~MAX_TABLE_SIZE-1)
  entry *e = (entry*)malloc(sizeof(entry));
  e->key = key;
  e->value = value;
  e->next = NULL;
  if(ht->table[hash] == NULL)
    ht->table[hash] = e;
  else { //값이 있으면,
    entry *prev = NULL;
    entry *curr = ht->table[hash];
    while(curr != NULL) { //값이 채워진 부분까지 current pointer 땡기고,
      prev = curr;
      curr = curr->next;
    }
    prev->next = e; //땡긴 포인터의 next에 새로운 entry를 연결
  }
}

int get_value(hash_table *ht, char *key) {
  int hash = hash_function(key);
  if(ht->table[hash] == NULL)
    return -1;
  else {
    entry *curr = ht->table[hash];
    while(curr != NULL) {
      if(curr->key == key) //키가 맞았을 때 값을 반환하지만,
        return curr->value;
      curr = curr->next; //키가 맞지 않으면 다음 entry를 탐색.
                         //Q. 왜 키가 안맞는 경우가 있음?
                         //A. %연산 해서 얻은 hash key 값이 같은 경우가 있기 때문에, 같은 hash key 값을 가진 entry들은 linked list로 연결되어 있음.
    }
    return -1;
  }
}

int main() {
  hash_table ht;
  for(int i = 0; i < MAX_TABLE_SIZE; i++)
    ht.table[i] = NULL;
  insert_entry(&ht, "abc", 20);
  printf("%d\n", get_value(&ht, "abc"));
  return 0;
}
