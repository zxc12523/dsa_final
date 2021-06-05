#include "api.h"
#include <stdio.h>

// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

// asdfasdf
//scacasc qweqweqwe

typedef struct trie_node{
	/* data */
}node;


int n_mails, n_queries;
mail *mails;
query *queries;


int group_num;
int group_size;
int *group_root;
int *disjoint_set;
int *disjoint_set_size;

void disjoint_init(int len) {
	group_num = group_size = 0;
	group_root = calloc(len, sizeof(int));
	disjoint_set = calloc(len, sizeof(int));
	disjoint_set_size = calloc(len, sizeof(int));

    for(int i=0;i<len;i++) {
		disjoint_set[i] = i;
		disjoint_set_size[i] = 1;
	}
}

int find_disjoint_root(int n) {
    if (disjoint_set[n] == n) return n;
    else {
		int next = disjoint_set[n];
		if (disjoint_set[next] != next) 
			disjoint_set_size[next] -= disjoint_set_size[n];
		return disjoint_set[n] = find_disjoint_root(disjoint_set[n]);
	}
}

void connect_disjoint(int a, int b) {
    int x = find_disjoint_root(disjoint_set[a]);
    int y = find_disjoint_root(disjoint_set[b]);
	if (group_root[x] && group_root[y]) group_num -= 1;
	else if (!group_root[x] && !group_root[y]) group_num += 1;
	group_root[x] = 0;
	group_root[y] = 1;
	disjoint_set[x] = y;
	disjoint_set_size[y] += disjoint_set_size[x];
	group_size = max(group_size, disjoint_set_size[y]);
}

int* Group_Analyse(int len, int* mid) {
	for(int i=0;i<len;i++) {
		mails[mid[i]];
	}
}

int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);

	for(int i = 0; i < n_queries; i++)
		if(queries[i].type == expression_match)
		  api.answer(queries[i].id, NULL, 0);

  return 0;
}
