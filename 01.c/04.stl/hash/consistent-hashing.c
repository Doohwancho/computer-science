#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_SERVERS 5 //consistent-hash에서 node역할
#define NUM_KEYS 10 //consistent-hash에서 key역할

typedef struct { //mock server
    char* name;
    unsigned int hash;
} Server;

unsigned int simple_hash(char *str) { //hash(char) -> int index for consistent-hashing array
    unsigned int hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

int compare(const void *s1, const void *s2) { //compare server's hash to make sure in order when insert into consistent-hash
    Server *server1 = (Server *)s1;
    Server *server2 = (Server *)s2;
    return (server1->hash < server2->hash) ? -1 : (server1->hash > server2->hash);
}

void main() {
    Server servers[NUM_SERVERS];
    char keys[NUM_KEYS][50] = {"key1", "key2", "key3", "key4", "key5", "key6", "key7", "key8", "key9", "key10"};

    // Initialize servers
    for (int i = 0; i < NUM_SERVERS; i++) {
        char server_name[10];
        sprintf(server_name, "server%d", i + 1);
        servers[i].name = strdup(server_name);
        servers[i].hash = simple_hash(server_name); //각 서버의 hash값을 서버 이름을 넣어 생성
    }

    // Sort servers by hash
    qsort(servers, NUM_SERVERS, sizeof(Server), compare);

    // Map keys to servers
    for (int i = 0; i < NUM_KEYS; i++) {
        unsigned int hash_value = simple_hash(keys[i]); //keys 역시 해시함수 돌려서 index 받음
        int server_index = 0;

        // Find the first server with a hash greater than or equal to the key's hash
        while (server_index < NUM_SERVERS && servers[server_index].hash < hash_value) { //keys의 해시 아이디보다 작은 서버(node)에게 매핑
            server_index++;
        }

        // If we've gone past the end of the server list, wrap around to the first server
        if (server_index == NUM_SERVERS) { //circular이라, 숫자가 array size 이상 초과하면, 처음 놈에 매핑해줌.
            server_index = 0;
        }

        printf("Key %s (hash %u) is mapped to %s (hash %u)\n", keys[i], hash_value, servers[server_index].name, servers[server_index].hash);
    }

    // Free server names
    for (int i = 0; i < NUM_SERVERS; i++) {
        free(servers[i].name);
    }
}

