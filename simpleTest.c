//이진 검색트리//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef char *K;
typedef int V;

typedef struct Node{
	K key;
	V value;
	struct Node *left;
	struct Node *right;
}Node;

typedef struct Queue{
	Node *node;
	struct Queue *next;
}Queue;

int compare(K a, K b){ //키 비교 함수
	return strcmp((char *)a,(char *)b); //작으면 -, 크면 +, 같으면 0
}

Node *searchR(Node *x,K key){
	if(x == NULL) return NULL; //아무것도 없으면
	if(compare(key, x->key) < 0) return searchR(x->left, key);
	if(compare(key, x->key) > 0) return searchR(x->right, key);
	return x; //키가 같으면
}
V search(Node *root, K key){ //보조함수
	return (searchR(root, key))->value;
}

Node *insertR(Node *x, K key, V val){
	if(x == NULL){
		Node *x = (Node *)malloc(sizeof(Node));
		x->key = key;
		x->value = val;
		x->left = x->right = NULL;
		return x;
	}
	if(compare(key, x->key) < 0) x->left = insertR(x->left, key, val); //작으면
	else if(compare(key, x->key) > 0) x->right = insertR(x->right, key, val); //크면
	else x->value = val; //같으면
	return x;		
}
void insert(Node **root, K key, V val){
	*root = insertR(*root, key, val);
}

int queueInsert(Queue **front ,Queue **rear, Node *node){
	Queue *new = (Queue *)malloc(sizeof(Queue));
	new->node = node;
	new->next = NULL; 
	if((*rear) == NULL){
		(*front) = new;
		(*rear) = new;
	}
	else{
		(*(rear))->next = new;
		*(rear) = new;
	}
 	return 1;
}

Node* queueDelete(Queue **front, Queue **rear){
	Node *p;
	if(*(front) == NULL){ //제로 일때
		return NULL;
	}
	else if((*front)->next == NULL){ //1개 남았을때
		p = (*(front))->node;
		free(*(front));
		*(front) = NULL;
		*(rear) = NULL;
		return p;
	}
	else{
		p = (*(front))->node;
		Queue *d = *(front);
		*(front) = (*front)->next;
		free(d);
	 	return p;
	}

}

int levelOrderR(Node *bst, Queue **front, Queue **rear){
	
	if(bst == NULL) return 0;
	else{
		printf("key: %c value: %d\n",*(bst->key),bst->value);
		if(bst->left != NULL){
			queueInsert(front ,rear, bst->left);
		}
		if(bst->right != NULL){
			queueInsert(front ,rear, bst->right);
		}
		levelOrderR(queueDelete(front,rear), front, rear);
	}
}
int levelOrder(Node *bst){
	Queue *front =NULL; //delete
	Queue *rear =NULL; //insert
	levelOrderR(bst, &front, &rear);
}

int preOrder(Node *bst){ //순차적으로 출력
	if(bst == NULL) return 0;
	preOrder(bst->left);
	printf("key:%c\n value:%d\n", *(bst->key), bst->value);
	preOrder(bst->right);
}

int main(void){
	Node *bst = NULL;

	char *keys[]={"E","X","A","M","P","L","E"};
	int values[] = {4,7,6,5,2,3,8};

	for(int i=0;i<7;i++){
		insert(&bst, keys[i], values[i]);
	}
	printf("(%s, %d)\n", bst->right->key, bst->right->value);
	printf("%d\n",search(bst,"L"));

	levelOrder(bst);
	printf("preOrder\n");
	preOrder(bst);
	return 0;
}











