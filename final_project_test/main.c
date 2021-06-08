#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b
#define minn -2147483648
#define swap(type, a, b) {type tmp = a; a = b; b = tmp;}

// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

// asdfasdf
//scacasc qweqweqwe


int n_mails, n_queries;
mail *mails;
query *queries;


int *token_num;
float** similarity;

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

// -------------- link list -----------------------
typedef struct node {
    int val;
    struct node* next;
}node;

node* new_node(int val) {
    node* newnode = malloc(sizeof(node));
    newnode->val = val;
    newnode->next = NULL;
    return newnode;
}

typedef struct Linked_List{
    node* front;
    node* back;
}ll;

ll* new_link_list() {
	ll* new_ll = malloc(sizeof(ll));
	new_ll->front = new_ll->back = NULL;
	return new_ll;
}

void ll_push_back(ll* q, int val) {
    if (q->front == NULL) {
        q->front = q->back = new_node(val);
    }
    else {
        node* newnode = new_node(val);
        q->back->next = newnode;
        q->back = newnode;
    }
}

void ll_traversal(ll* q, int mail_id) {
	node* cur = q->front;
	while (cur){
		similarity[cur->val][mail_id] += 1;
		cur = cur->next;
	}
}
// -------------- link list -----------------------

// -------------- 字元索引樹 -----------------------

typedef struct trie_node{
	char c;
	bool find;
	int array_size;
	ll* possible_link_list;
	long long* possible_mail_id;
	struct trie_node** child;
}trie_node;

trie_node* root;

trie_node* new_trie_node(char c) {
	trie_node* newnode = malloc(sizeof(trie_node));
	newnode->c = c;
	newnode->find = false;
	newnode->possible_link_list = new_link_list();
	newnode->possible_mail_id = calloc(n_mails / 64 + 1, sizeof(long long));
	newnode->child = calloc(36, sizeof(trie_node));
	return newnode;
}

void insert_string(trie_node* root, char* s, int len, int ind, int mail_id) {
	if (ind == len) {
		root->find = true;
		if (root->possible_link_list->back->val != mail_id){
			token_num[mail_id]++;
			ll_traversal(root->possible_link_list, mail_id);
			ll_push_back(root->possible_link_list, mail_id);
			add_mail(root->possible_mail_id, root->array_size, mail_id);
		}	
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

// -------------- 字元索引樹 -----------------------
// -------------- pre process -----------------------

void decompose_mail(int t, char* s, int len, int mail_id) {
	int ind = 0, len = 0;
	for(int i=0;i<len;i++) {
		if (isToken(s[i])){
			len++;
			if (i == len - 1){
				insert_string(root, s, len, ind, mail_id);
			}
		}
		else {
			if (ind != len){
				insert_string(root, s, len, ind, mail_id);
			}
			ind = len = len + 1;
		}
	}
}

void read_subject_and_content(int t, mail m){
	int subject_len = strlen(m.subject);
	int content_len = strlen(m.content);
	decompose_mail(t, m.subject, subject_len, m.id);
	decompose_mail(t, m.content, content_len, m.id);
}

void init() {
	root = new_trie_node('\0');
	token_num = calloc(n_mails, sizeof(int));
	similarity = calloc(n_mails, sizeof(float));
	for(int i=0;i<n_mails;i++){
		similarity[i] = calloc(n_mails, sizeof(float));
		read_subject_and_content(i, mails[i]);
	}
	for(int i=0;i<n_mails;i++) {
		for(int j=0;j<i;j++) {
			similarity[i][j] /= (token_num[i] + token_num[j]);
			similarity[j][i] = similarity[i][j];
		}
	}
}

// -------------- pre process -----------------------


// --------------- Query 1 ----------------------

int* Expression_Match(mail m) {

}

// --------------- Query 1 -----------------------

// --------------- Query 2 -----------------------
int *query_2_ans;

int Find_Similar(int mid, float threshold) {
	int len = 0;
	query_2_ans = malloc(n_mails * sizeof(int));
	for(int i=0;i<n_mails;i++) {
		if (i != mid && similarity[mid][i] > threshold) {
			query_2_ans[len++] = i;
		}
	}
	return len;
}

// --------------- Query 2 -----------------------

// --------------- Query 3 -----------------------
int cur = 0;
int group_num;
int group_size;
int *group_root;
int *disjoint_set;
int *disjoint_set_size;
int query_3_ans[2];

typedef struct name_trie_node{
	char c;
	int number;
	struct name_trie_node** child;
}name_trie_node;

name_trie_node* name_root;

name_trie_node* new_name_trie_node(char c) {
	name_trie_node* newnode = malloc(sizeof(name_trie_node));
	newnode->c = c;
	newnode->number = -1;
	newnode->child = calloc(36, sizeof(name_trie_node));
	return newnode;
}

int insert_name(name_trie_node* name_root, char* s, int len, int ind) {
	if (ind == len) {
		if (name_root->number == -1) 
			name_root->number = cur++;
		return name_root->number;
	}

	int c = char_to_int(s[ind]);
	if (!name_root->child[c])
		name_root->child[c] = new_name_trie_node(s[ind]);
	return insert_name(name_root->child[c], s, len, ind+1);
}

void disjoint_init(int len) {
	name_root = new_name_trie_node('\0');
	cur = group_num = group_size = 0;
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
	if (a == b) return;
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

void free_disjoint_set() {
	free(name_root);
	free(group_root);
	free(disjoint_set);
	free(disjoint_set_size);
}

void Group_Analyse(int len, int* mid) {
	disjoint_init(len * 2);
	for(int i=0;i<len;i++) {
		int a = insert_name(name_root, mails[mid[i]].from, strlen(mails[mid[i]].from), 0);
		int b = insert_name(name_root, mails[mid[i]].to, strlen(mails[mid[i]].to), 0);
		connect_disjoint(a, b);
	}
	free_disjoint_set();
	query_3_ans[0] = group_num;
	query_3_ans[1] = group_size;
}

// -----------------Query 3-----------------------

int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);
	for(int i = 0; i < n_queries; i++){
		if (queries[i].type == expression_match){
			api.answer(queries[i].id, NULL, 0);
		}
		if (queries[i].type == find_similar){
			int len = Find_Similar(queries[i].data.find_similar_data.mid, queries[i].data.find_similar_data.threshold);
			api.answer(queries[i].id, query_2_ans, len);
			free(query_2_ans);
		}
		if (queries[i].type == group_analyse){
			int len = queries[i].data.group_analyse_data.len;
			int* mid = queries[i].data.group_analyse_data.mids;
			Group_Analyse(len, mid);
			api.answer(queries[i].id, query_3_ans, 2);
		}
	}
  return 0;
}
