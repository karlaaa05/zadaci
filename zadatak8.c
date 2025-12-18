#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1

struct treeNode; 
typedef struct treeNode* TreePosition; //pokazivac na cvor stabla
typedef struct treeNode {
	int data; //broj u cvoru
	TreePosition left; //pokazuje na lijevo dijete
	TreePosition right; //pokazuje na desno dijete
}TreeNode;

struct queueNode; //cvor reda za level order, element reda
typedef struct queueNode* QueuePosition; //pokazivac na strukturu queueNode
typedef struct queueNode {
	TreePosition treeNode; //pokazivac na cvor stabla
	QueuePosition next; //pokazivac na sljedeci cvor reda
}QueueNode;

QueuePosition rear = NULL; //pokazivac na kraj reda, na zadnji element, u pocetku je red prazan
//to nam trea da znamo gdje dodati novi element u red, jer se dodaje na kraj reda
// rear next uvijek pokazuje na prvi element reda

TreePosition insert(int x, TreePosition root); //za umetanje cvora u stablo, vraca root jer moze doci do promjene roota
TreePosition search(int x, TreePosition root); //trazi element u stablu
//funkcije za ispis stabla
int preOrder(TreePosition root, int level);
int inOrder(TreePosition root, int level);
int postOrder(TreePosition root, int level);
int levelOrder(TreePosition root);
//funkcije za red
QueuePosition enqueue(TreePosition levelItem); 
TreePosition dequeue(); 
TreePosition findMin(TreePosition root); 
TreePosition deleteNode(TreePosition root, int x); 
int deleteTree(TreePosition root); 


int main() {
	TreePosition root = NULL;

	root = insert(4, root);
	root = insert(9, root);
	root = insert(2, root);
	root = insert(10, root);
	root = insert(6, root);
	root = insert(3, root);
	root = insert(1, root);
	root = insert(5, root);
	root = insert(7, root);

	search(6, root);
	search(11, root);

	printf("Preorder print:\n");
	preOrder(root, 0);

	printf("Inorder ispis:\n");
	inOrder(root, 0);

	printf("Postorder ispis:\n");
	postOrder(root, 0);

	printf("Levelorder ispis:\n");
	levelOrder(root);

	root = deleteNode(root, 7);
	root = deleteNode(root, 6);
	printf("Preorder ispis nakon brisanja:\n");
	preOrder(root, 0);

	deleteTree(root);

	return EXIT_SUCCESS;

}

TreePosition insert(int x, TreePosition root){ 
	//ako je stablo prazno, novi element postaje root
	if (root == NULL) {
		root = (TreePosition)malloc(sizeof(TreeNode));
		if (root == NULL) {
			printf("Malloc error.\n");
			return NULL;
		}
		root->data = x;
		root->left = NULL;
		root->right = NULL;
	}
	//ako je x manji od roota, ide lijevo 
	else if (x < root->data)
		root->left = insert(x, root->left);
	//ako je x veci od roota, ide desno
	else if (x > root->data)
		root->right = insert(x, root->right);

	return root;
}

TreePosition search(int x, TreePosition root) {
	if (root == NULL) {
		printf("%d not found in tree.\n", x);
		return NULL;
	}
	else if (x < root->data)
		return search(x, root->left);

	else if (x > root->data)
		return search(x, root->right);

	printf("%d found in tree.\n", x);
	return root; //vracamo pokzivac na pronadeni element
}

//preorder ispis=root,lijevo rekurzivno,desno rekurzivno
int preOrder(TreePosition root, int level) {
	int i;
	for (i = 0; i < level; i++) printf("  ");
	printf("%d\n", root->data);
	
	if (root->left != NULL) preOrder(root->left, level + 1);
	if (root->right != NULL) preOrder(root->right, level + 1);
	return 0;
}

//inorder ispis=lijevo rekurzivno,root,rekurzivno desno
int inOrder(TreePosition root, int level) {
	int i;
	if (root->left != NULL) inOrder(root->left, level + 1);
	for (i = 0; i < level; i++) printf("  ");
	printf("%d\n", root->data);
	
	if (root->right != NULL) inOrder(root->right, level + 1);
	return 0;
}

//postorder ispis=lijevo rekurzivno,desno rekurzivno,root
int postOrder(TreePosition root, int level) {
	int i;
	if (root->left != NULL) postOrder(root->left, level + 1);
	if (root->right != NULL) postOrder(root->right, level + 1);
	for (i = 0; i < level; i++) printf("  ");
	printf("%d\n", root->data);
	return 0;
}
//funkcija koja rpima adresu jednog cvora stabla, stvara novi cvor reda,dodaje ga na kraj reda i vraca adresu cvora reda
QueuePosition enqueue(TreePosition treeNode) {
	QueuePosition queueNode;
	queueNode = (QueuePosition)malloc(sizeof(QueueNode));
	if (queueNode == NULL) {
		printf("Malloc error.\n");
		return NULL;
	}
	queueNode->treeNode = treeNode;  //u cvor reda spremimo pokazivac na cvor stabla
	if (rear == NULL) { //ako je red prazan
		queueNode->next = queueNode; //pokazuje na samog sebe
		return queueNode;
	}
	
		queueNode->next = rear->next; 
		rear->next = queueNode; //stari kraj reda pokazuje na novi cvor
		rear = queueNode; //azuriramo rear da pokazuje na novi kraj reda

		return queueNode;

}

//funkcija koja uzima element s pocetka reda i vraca pokazivac na cvor stabla
TreePosition dequeue() {
	if (rear == NULL) {
		printf("Queue is empty.\n");
		return NULL;
	}
	TreePosition levelItem; //level item je cvor stabla koji se izvadi iz reda
	if (rear == rear->next) { //ako je u redu samo jedan element
		levelItem = rear->treeNode; //uzmi cvor stabla iz reda
		free(rear);
		rear = NULL; //red je sad prazan
	}
	else { //ako je vise elemenata u redu
		QueuePosition front = rear->next; //zadnji element pokazuje na prvi ,kruzna lista, front je pokazivac na prvi element
		levelItem = front->treeNode; //levelitem pokazuje na cvor stabla iz prvog el reda 
		rear->next = front->next; //zadnji element pokazuje na drugi
		free(front);
	}
	return levelItem;

}

levelOrder(TreePosition root) {
	rear = enqueue(root); //stavljamo root u red
	if (rear == NULL) return MALLOC_ERROR;

	while (rear != NULL) {
		TreePosition treeNode = dequeue(); 
		printf("%d ", treeNode->data);
		if (treeNode->left != NULL) rear = enqueue(treeNode->left);
		if (treeNode->right != NULL) rear = enqueue(treeNode->right); 
	}
	printf("\n");
	return 0;
}
//funkcija koja trazi najmanji element u stablu, ide skroz lijevo
TreePosition findMin(TreePosition root) {
	if (root->left == NULL) return root;
	return findMin(root->left);
}

//funkcija koja brise element iz stabla
TreePosition deleteNode(TreePosition root, int x) {

	if (root == NULL) {
		printf("Element not found.\n");
		return NULL;
	}
    if (x < root->data)
		root->left = deleteNode(root->left, x);
	else if (x > root->data)
		root->right = deleteNode(root->right, x);
	//element pronaden
	
	//1.slucaj-ima dvoje djece
	else if (root->left != NULL && root->right != NULL) {
		TreePosition minInRight = findMin(root->right); //trazimo najmanji element u desnom podstablu tako da ga stavimo na mjesto obrisanog
		root->data = minInRight->data; //kopiramo vrijednost 
		root->right = deleteNode(root->right, root->data); //brisemo najmanji element iz desnog podstabla
	}
	//2.slucaj-ima 0 ili 1 dijete
	else {
		TreePosition temp = root;
		if (root->left == NULL) //nema lijevo dijete
			root = root->right; //root postaje desno dijete ili NULL ako nema djece
		else root = root->left; //root postaje lijevo dijete,ako ga ima
        free(temp); //brisemo stari root 
	} return root;
}

int deleteTree(TreePosition root) {
	if (root == NULL) return EXIT_SUCCESS;
	deleteTree(root->left);
	deleteTree(root->right);
	free(root);
	return EXIT_SUCCESS;
}

