#include <stdlib.h>
#include <string.h>

/*
 * routing algorithm can be either of these
 * 1. hash
 * 2. binary search tree
 *
 *  BST is easier, therefore implemented here
 *
 */

struct Route {
	char* key;
	char* value;

	struct Route *left, *right;
};

struct Route * initRoute(char* key, char* value);

struct Route * addRoute(struct Route * root, char* key, char* value);

struct Route * search(struct Route * root, char * key);

void inorder(struct Route * root);


