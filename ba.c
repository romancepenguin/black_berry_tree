//이진 검색트리//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 40

typedef char *K;
typedef char *V;
typedef enum {false, true} bool; //true 레드

typedef struct Node{
	char key[MAX_SIZE];
	char value[MAX_SIZE];
	struct Node *left;
	bool l_line;
	struct Node *right;
	bool r_line;
}Node;

typedef struct Queue{
	Node *node;
	struct Queue *next;
}Queue;

int queueInsert(Queue **front ,Queue **rear, Node *node);

int compare(K a, K b){ //키 비교 함수
	return strcmp((char *)a,(char *)b); //작으면 -, 크면 +, 같으면 0
}

Node *searchR(Node *x,K key){
	if(x == NULL) return NULL; //아무것도 없으면
	if(compare(key, x->key) < 0) return searchR(x->left, key);
	if(compare(key, x->key) > 0) return searchR(x->right, key);
	return x; //키가 같으면
}

int search(Node *root, K key){ //보조함수
	if((searchR(root, key)) == NULL){ printf("non exist key\n"); return -1;}
	printf("--searching--\n");
	printf("%s\n",(searchR(root, key))->value); 
	printf("-------------\n");
	return 1;
}

int PartCompare(K a, K b){ //키 비교 함수
	if(strstr((char *)b,(char *)a) != NULL){ //있으면 char포인터, 없으면 null
		return 0;
	}
	return strcmp((char *)a,(char *)b);
}

int PartSearchR(Node *x,K key, Queue **q, Queue **rear){
	if(x == NULL) return 1; //아무것도 없으면
	if(PartCompare(key, x->key) == 0) queueInsert(q, rear, x);
	PartSearchR(x->left, key, q, rear);
	PartSearchR(x->right, key, q, rear);
}

int PartSearch(Node *root, K key){ //보조함수
	Queue *q = NULL;
	Queue *rear = NULL;
	PartSearchR(root, key, &q, &rear);
	if(q == NULL){ printf("no serach data\n"); return -1;}
	printf("---list--- search : %s\n",key);
	while(q!=NULL){
		printf("key : %s - value : %s\n",(q->node)->key, (q->node)->value);
		q = q->next;
	}
	return 1; 
}

Node *rotateLeft(Node **node){
	(*node)->r_line = false;
	Node *Rnode = (*node)->right;
	(*node)->right = Rnode->left;
	Rnode->left = (*node);
	Rnode->l_line = true;	
	return Rnode;
}
Node *rotateRight(Node **node){
	(*node)->l_line = false;
	Node *Lnode = (*node)->left;
 	(*node)->left = Lnode->right;
	Lnode->right = (*node);
	Lnode->r_line = true;
	return Lnode;
}

Node *insertR(Node *x, K key, V val){
	if(x == NULL){
		Node *new = (Node *)malloc(sizeof(Node));
		strncpy(new->key,key,20);
		strncpy(new->value,val,20);
		new->left = new->right = NULL;
		new->l_line = new-> r_line = false;
		//printf("1"); 
		return new;
	} 
	if(compare(key, x->key) < 0){ //작으면
		if(x->left == NULL) x->l_line = true;
		x->left = insertR(x->left, key, val);
		if((x->left)->r_line == true && (x->left)->l_line == true){
			(x->left)->r_line = false;
			(x->left)->l_line = false;
			x->l_line = true;
		}
		else if(x->l_line == true && (x->left)->l_line == true){
			return rotateRight(&x);
		}
		//왼쪽 2개가 연속 red면
		//회전하고 올리기
	}
	else if(compare(key, x->key) > 0){ //크면
		if(x->right == NULL) x->r_line = true;
		x->right = insertR(x->right, key, val);
		if((x->right)->r_line == true && (x->right)->l_line == true){
			(x->right)->r_line = false;
			(x->right)->l_line = false;
			x->r_line = true;
		}
		else if(x->r_line == true){
			return rotateLeft(&x);
		}
		//만약 양쪽이다 red면
		//오른쪽만 red면 
	}
	else strncpy(x->value,val,20); //같으면
	return x;		
}

void insert(Node **root, K key, V val){
	*root = insertR(*root, key, val);
	if((*root)->r_line == true && (*root)->l_line == true){
		(*root)->r_line = false;
		(*root)->l_line = false;
	}
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
		printf("key: %s\n",(bst->key));
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
	printf("key:%s value:%s\n", bst->key, bst->value);
	//printf("left:%d right:%d\n", bst->l_line, bst->r_line);
	preOrder(bst->left);
	preOrder(bst->right);
}

int inOrder(Node *bst){ //순차적으로 출력
	if(bst == NULL) return 0;
	printf("key:%s value:%s\n", bst->key, bst->value);
	//printf("left:%d right:%d\n", bst->l_line, bst->r_line);
	preOrder(bst->left);
	preOrder(bst->right);
}

int postOrder(Node *bst){ //순차적으로 출력
	if(bst == NULL) return 0;
	preOrder(bst->left);
	preOrder(bst->right);
	printf("key:%s value:%s\n", bst->key, bst->value);
	//printf("left:%d right:%d\n", bst->l_line, bst->r_line);
}


int main(void){
	Node *bst = NULL;

	FILE *fp = NULL;
	if((fp = fopen("capitals.txt", "r")) == NULL)
	{
		printf("파일 읽기 오류! \n");
		return 0;
	}

	char buf[256];
	char *token;
	char *token2;


        while( fgets( buf,256, fp) != NULL )
        {
		buf[strlen(buf)-1] = NULL;
		//printf("%s\n",buf);
		token = strtok(buf,",");
      		token2 = strtok(NULL,"");
      		//printf("%s %s\n",token, token2);
		insert(&bst, token, token2);
		memset(buf,0x0,256);
        }

	while(1){
		memset(buf,0x0,256);
		printf("\n");
		printf("if you want to instrucions then enter h or help\n");
		printf("please enter instruction : ");
		fgets(buf,256,stdin);

		if(strncmp(buf,"h",1) == 0 || strncmp(buf,"help",4) == 0 ){
			printf("**********************************************\n");
			printf("version 1.0.0\n");
			printf("!!black and red tree!!\n");
			printf("	s : search data\n");
			printf("	p : part search data\n");
			printf("	i : insert data\n");
			printf("	pre : pre-order tree print\n");
			printf("	in : pre-order tree print\n");
			printf("	post : pre-order tree print\n");
			printf("	level : pre-order tree print\n");
			printf("	exit : exit data\n");
			printf("\n");
			printf("**********************************************\n");
		}
		else if(strncmp(buf,"s",1) == 0 || strncmp(buf,"search",6) == 0 ){
			memset(buf,0x0,256);
			printf("\n");
			printf("please enter national : ");
			fgets(buf,256,stdin);
			buf[strlen(buf)-1] = '\0';
			search(bst,buf);
		}
		else if(strncmp(buf,"p",1) == 0 || strncmp(buf,"psearch",7) == 0 ){
			memset(buf,0x0,256);
			printf("\n");
			printf("please enter word : ");
			fgets(buf,256,stdin);
			buf[strlen(buf)-1] = '\0';
			PartSearch(bst,buf);
		}
		else if(strncmp(buf,"i",1) == 0 || strncmp(buf,"insert",6) == 0 ){
			char b[50];
			memset(buf,0x0,256);
			printf("please enter national : ");
			fgets(buf,256,stdin);
			memset(b,0x0,50);
			printf("\n");
			printf("please enter capitalist : ");
			fgets(b,50,stdin);
			buf[strlen(buf)-1] = '\0';
			b[strlen(b)-1] = '\0';
			insert(&bst, buf, b);
		}
		else if(strncmp(buf,"pre",3) == 0){
			preOrder(bst);
		}
		else if(strncmp(buf,"in",2) == 0){
			inOrder(bst);
		}
		else if(strncmp(buf,"post",4) == 0){
			postOrder(bst);
		}
		else if(strncmp(buf,"level",5) == 0){
			levelOrder(bst);
		}
		else if(strncmp(buf,"e",1) == 0 || strncmp(buf,"exit",4) == 0){
			printf("bye bye\n");		
			exit(1);
		}
		else{
			printf("error keyword\n");
			printf("please key re\n");
		}
	}
	//insert
	//pre, in, fac, level
	//완전검색
	//부분검색	

	//for(int i=0;i<7;i++){
	//	insert(&bst, (keys[i]), values[i]);
	//	printf("%s\n",keys[i]);
	//}
	//printf("(%s, %d)\n", bst->right->key, bst->right->value);
	//printf("%c\n",*(bst->key));
	//PartSearch(bst,"A123g");
	//levelOrder(bst);
	//treePrint(bst);
	//printf("\n");
	//printf("%s\n",search(bst,"A123g"));
	//search(bst,"Samoa");
	//preOrder(bst);
	//printf("%d\n",strstr("abcd","abc"));
	return 0;
}









