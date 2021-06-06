#include "api.h"
#include <stdio.h>
#include <stdbool.h>

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

bool isToken(char c)
{
    if(c<='9'&&c>='0')return true;
    if(c<='z'&&c>='a')return true;
    if(c<='Z'&&c>='A')return true;
    return false;
}

void add_mail(long long* arr, int arr_size, int mail_id) {
	int cur = 0;
	while (cur < arr_size)
	{
		if (mail_id > 64) {
			mail_id -= 64;
			cur += 1;
			continue;
		}
		else {
			long long tmp = 0;
			tmp <<= mail_id;
			arr[cur] &= tmp;
			break;
		}
	}
	return;
}

// -------------- 字元索引樹 -----------------------

typedef struct trie_node{
	char c;
	bool find;
	int array_size;
	long long* possible_mail_id;
	struct trie_node** child;
}trie_node;

trie_node* root;

trie_node* new_trie_node(char c) {
	trie_node* newnode = malloc(sizeof(trie_node));
	newnode->c = c;
	newnode->find = false;
	newnode->possible_mail_id = calloc(n_mails / 64 + 1, sizeof(long long));
	newnode->child = calloc(36, sizeof(trie_node));
	return newnode;
}

void insert_string(trie_node* root, char* s, int len, int ind, int mail_id) {
	if (ind == len - 1) {
		root->find = true;
		add_mail(root->possible_mail_id, root->array_size, mail_id);
		return;
	}

	int c = char_to_int(s[ind]);

	if (!root->child[c]) root->child[c] = new_trie_node(s[ind]);
	insert_string(root->child[c], s, len, ind+1, mail_id);
}

long long* search_string(trie_node* root, char* s, int len, int ind) {
	if (!root) {
		long long* ans = calloc(n_mails / 64 + 1, sizeof(long long));
		return ans;
	}
	else if (ind == len - 1) {
		return root->possible_mail_id;
	}
	return search_string(root->child[char_to_int(s[ind])], s, len, ind + 1);
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
