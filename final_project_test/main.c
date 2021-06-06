#include "api.h"
#include <stdio.h>

// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

// asdfasdf
//scacasc qweqweqwe


int n_mails, n_queries;
mail *mails;
query *queries;

int char_to_int(char c) {
	if ('a' <= c && c <= 'z') 
		return c - 'a';
	else if ('A' <= c && c <= 'Z')
		return c - 'A';
	else if ('0' <= c && c <= '9')
		return c - '0' + 10;
	else return -1;
}

// ------------linked list-----------------------

typedef struct ll_node {
    int val;
    struct ll_node* pre;
    struct ll_node* next;
}ll_node;

ll_node* new_node(int val) {
    ll_node* newnode = malloc(sizeof(ll_node));
    newnode->val = val;
    newnode->pre = newnode->next = NULL;
    return newnode;
}

typedef struct Linked_List{
    ll_node* front;
    ll_node* back;
}ll;

ll* new_ll() {
    ll* newll = malloc(sizeof(ll));
    newll->front = newll->back = NULL;
    return newll;
}

void ll_push_back(ll* q, int val) {
    if (q->front == NULL) {
        q->front = q->back = new_node(val);
    }
    else {
        ll_node* newnode = new_node(val);
        q->back->next = newnode;
        newnode->pre = q->back;
        q->back = newnode;
    }
}

int ll_pop_front(ll* q) {
    int ret;
    ret = q->front->val;
    if (q->front == q->back) {
        free(q->front);
        q->front = q->back = NULL;
    }
    else {
        q->front = q->front->next;
        free(q->front->pre);
        q->front->pre = NULL;
    }
    return ret;
}

// ------------linked list-----------------------

// --------------字元索引樹-----------------------

typedef struct trie_node{
	char c;
	bool find;
	ll* possible_mail_id;
	struct trie_node** child;
}trie_node;

trie_node* root;

trie_node* new_trie_node(char c) {
	trie_node* newnode = malloc(sizeof(trie_node));
	newnode->c = c;
	newnode->find = false;
	newnode->possible_mail_id = new_ll();
	newnode->child = calloc(36, sizeof(trie_node));
	return newnode;
}

void insert_string(trie_node* root, char* s, int len, int ind, int mail_id) {
	if (ind == len - 1) {
		root->find = true;
		ll_push_back(root->possible_mail_id, mail_id);
		return;
	}

	int c = char_to_int(s[ind]);

	if (!root->child[c]) root->child[c] = new_trie_node(s[ind]);
	insert_string(root->child[c], s, len, ind+1, mail_id);
}

// --------------字元索引樹-----------------------

// --------------- Query 1 ----------------------

int *return_string_array(mail m){
	int **vec, vec_len = 0;
	int subject_len = strlen(m.subject);
	int content_len = strlen(m.content);
	vec = malloc(50300*sizeof(int));
}

// --------------- Query 1 ----------------------

// -----------------Query 3-----------------------
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


// -----------------Query 3-----------------------

int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);

	for(int i = 0; i < n_queries; i++)
		if(queries[i].type == expression_match)
		  api.answer(queries[i].id, NULL, 0);

  return 0;
}
