#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b
#define minn -2147483648
#define swap(type, a, b) {type tmp = a; a = b; b = tmp;}

int n_mails = 10000, n_queries;
typedef struct mail
{
	int id;
	char *from;
    char *to;
    char *subject;
    char *content;
}mail;

mail *mails;

mail new_mail(int id) {
	mail* m = malloc(sizeof(mail));
	m->id = id;
	m->from = malloc(sizeof(char)*256);
	m->content = malloc(sizeof(char)*100000);
	m->subject = malloc(sizeof(char)*256);
	m->to = malloc(sizeof(char)*256);
	return *m;
}

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
		similarity[mail_id][cur->val] += 1.0;
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
		if (!root->find || root->possible_link_list->back->val != mail_id){
			token_num[mail_id]++;
			ll_traversal(root->possible_link_list, mail_id);
			ll_push_back(root->possible_link_list, mail_id);
			add_mail(root->possible_mail_id, root->array_size, mail_id);
		}	
		root->find = true;
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

void decompose_mail(char* s, int str_len, int mail_id) {
	int ind = 0, len = 0;
	for(int i=0;i<str_len;i++) {
		if (isToken(s[i])){
			len++;
			if (i == str_len - 1){
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

void read_subject_and_content(int id, mail m){
	int subject_len = strlen(m.subject);
	int content_len = strlen(m.content);
	decompose_mail(m.subject, subject_len, id);
	decompose_mail(m.content, content_len, id);
}

int *query_2_ans;

void init() {
    FILE* p = fopen("data.txt", "r");
	mails = malloc(n_mails*sizeof(mail));
	for(int i=0;i<n_mails;i++) {
		mails[i] = new_mail(i);
		fgets(mails[i].from, 100000, p);
		fgets(mails[i].content, 100000, p);
		fgets(mails[i].subject, 100000, p);
		fgets(mails[i].to, 100000, p);
	}
	root = new_trie_node('\0');
	token_num = calloc(n_mails, sizeof(int));
	similarity = calloc(n_mails, sizeof(float));
	for(int i=0;i<n_mails;i++){
		similarity[i] = calloc(n_mails, sizeof(float));
		printf("%d\n", i);
		read_subject_and_content(i, mails[i]);
	}
	printf("read all\n");
	for(int i=0;i<n_mails;i++) {
		for(int j=0;j<i;j++) {
			similarity[i][j] /= (token_num[i] + token_num[j] - similarity[i][j]);
			similarity[j][i] = similarity[i][j];
		}
	}
	query_2_ans = malloc(n_mails * sizeof(int));
}

// -------------- pre process -----------------------

// --------------- Query 1 ----------------------


// --------------- Query 1 -----------------------

// --------------- Query 2 -----------------------


int Find_Similar(int mid, float threshold) {
	int len = 0;
	for(int i=0;i<n_mails;i++) {
		if (i != mid && similarity[mid][i] > threshold) {
			query_2_ans[len++] = i;
		}
	}
	return len;
}

// --------------- Query 2 -----------------------


int main(void) {
	init();
	for(int i=0;i<n_mails;i++) {
		for(int j=0;j<n_mails;j++) {
			printf("%f, ", similarity[i][j]);
		}
		printf("\n");
	}
  return 0;
}
