/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 32152462  |  32164950
*	    Student name : shin ho jin  |  hur ye ji
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"

/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
pthread_mutex_t lock;

void inorder(lab2_tree *tree, lab2_node *CurrentNode) // inorder function
{
	if (CurrentNode != NULL) {
		inorder(tree, CurrentNode->LL);
		inorder(tree, CurrentNode->RL);
	}
}

int lab2_node_print_inorder(lab2_tree *tree) { //inorder print function
    inorder(tree, tree->root);
	return LAB2_SUCCESS; // return success value
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create(){ // create tree function
	pthread_mutex_lock(&lock); // locking 
	lab2_tree *tree = (lab2_tree *)malloc(sizeof(lab2_tree)); // dynamic create tree
	tree->root = NULL;
	pthread_mutex_unlock(&lock); // unlocking
	return tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key){ // create node function
	pthread_mutex_lock(&lock); // locking
	lab2_node *newNode = (lab2_node *)malloc(sizeof(lab2_node)); // dynamic create newNode
	newNode->data = key;
	pthread_mutex_unlock(&lock); // unlocking
	return newNode;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){ // insert node function (lock X)
	lab2_node *p = tree->root; // delcare p and q to follow each other
	lab2_node *q = NULL;
	while (p) { // until follow the node that has no child node(find the input location)
		q = p;
		if (new_node->data == (p->data)) { // if exist smae node, create error signal
			return LAB2_ERROR;
		}
		else if (new_node->data > (p->data)) { // if bigger than parent node, go to rightchild
			p = p->RL;
		}
		else { // if smaller than parent node, go to leftchild
			p = p->LL;
		}
	}
	if (!(tree->root)) { // if tree is empty, new_node is be a root
		(tree->root) = new_node;
	}
	else if (new_node->data < (q->data)) { // if smaller than parent node, it is be a leftchild
		q->LL = new_node;
	}
	else { // if bigger than parent node, it is be a rightchild
		q->RL = new_node;
	}
	return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node) { // fine-grained node insert function
	lab2_node *p = tree->root; // insert function is the same as above
	lab2_node *q = NULL;
    
	while (p) {
		q = p;
		if (new_node->data == (p->data)) {
			return LAB2_ERROR;
		}
		else if (new_node->data > (p->data)) {
			p = p->RL;
		}
		else {
			p = p->LL;
		}
	}
    pthread_mutex_lock(&tree -> mutex);
    pthread_mutex_lock(&new_node -> mutex);
	if (!(tree->root)) {
		(tree->root) = new_node;
        pthread_mutex_unlock(&new_node -> mutex);
        pthread_mutex_unlock(&tree -> mutex);
	}
	else if (new_node->data < (q->data)) {
		q->LL = new_node;
        pthread_mutex_unlock(&new_node -> mutex);
        pthread_mutex_unlock(&tree -> mutex);
	}
	else {
		q->RL = new_node;
        pthread_mutex_unlock(&new_node -> mutex);
        pthread_mutex_unlock(&tree -> mutex);
	}
    
	return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node) { // coarse-grained node insert function
	pthread_mutex_lock(&lock); // locking
	lab2_node *p = tree->root; // insert function is the same as above
	lab2_node *q = NULL;
	while (p) {
		q = p;
		if (new_node->data == (p->data)) {
			pthread_mutex_unlock(&lock); // uncloking
			return LAB2_ERROR;
		}
		else if (new_node->data > (p->data)) {
			p = p->RL;
		}
		else {
			p = p->LL;
		}
	}
	if (!(tree->root)) {
		(tree->root) = new_node;
	}
	else if (new_node->data < (q->data)) {
		q->LL = new_node;
	}
	else {
		q->RL = new_node;
	}
	pthread_mutex_unlock(&lock); // unlocking
	return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int x) { // detele the node function(lock X)
	lab2_node *p = tree->root;
	lab2_node *q = NULL;
	lab2_node *t;
	while (1){ // until find the node that you want to delete
		if (x == (p->data)) // if the data you want to delete and the root are the same
			break;
		else if (x < (p->data)) { // if smaller than parent node, go to leftchild
			if (p->LL == NULL) {
				return LAB2_ERROR; // doesn't exist that want to find
			}
			q = p;
			p = p->LL;
		}
		else { // if bigger than parent node, go to rightchild
			if (p->RL == NULL) {
				return LAB2_ERROR; // doesn't exist that want to find
			}
			q = p;
			p = p->RL;
		}
	}
	if ((p->LL == NULL) && (p->RL == NULL))	{ // no child
		if (p == tree->root) {
			tree->root = NULL;
			return LAB2_SUCCESS;
		}
		if (p == q->LL)
			q->LL = NULL;
		else
			q->RL = NULL;
	}
	else if ((p->LL != NULL) && (p->RL == NULL)) { // only exist left child
		if (p == tree->root)
			tree->root = p->LL;
		else {
			if (p == q->LL)
				q->LL = p->LL;
			else
				q->RL = p->LL;
		}
	}
	else if ((p->LL == NULL) && (p->RL != NULL)) { // only exist right child
		if (p == tree->root)
			tree->root = p->RL;
		else {
			if (p == q->LL)
				q->LL = p->RL;
			else
				q->RL = p->RL;
		}
	}
	else if ((p->LL != NULL) && (p->RL != NULL)) { // both exist right and left child
		q = p;
		p = p->LL;
		t = p;
		while (1)
		{
			if (p->RL == NULL)
				break;
			else {
				t = p;
				p = p->RL;
			}
		}
		q->data = p->data;
		if (t != p) {
			if (p->LL != NULL)
				t->RL = p->LL;
			else
				t->RL = NULL;
		}
		else {
			q->LL = p->LL;
		}
	}
	return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int x) { // fine-grained node delete function
	lab2_node *p = tree->root;
	lab2_node *q = NULL;
	lab2_node *t;
	while (1){ // until find the node that you want to delete
		if (x == (p->data)) // if the data you want to delete and the root are the same
			break;
		else if (x < (p->data)) { // if smaller than parent node, go to leftchild
			if (p->LL == NULL) {
				return LAB2_ERROR; // doesn't exist that want to find
			}
			q = p;
			p = p->LL;
		}
		else { // if bigger than parent node, go to rightchild
			if (p->RL == NULL) {
				return LAB2_ERROR; // doesn't exist that want to find
			}
			q = p;
			p = p->RL;
		}
	}
	pthread_mutex_lock(&lock); // locking
	if ((p->LL == NULL) && (p->RL == NULL))	{ // no child
		if (p == tree->root) {
			tree->root = NULL;
			pthread_mutex_unlock(&lock); //unlocking
			return LAB2_SUCCESS;
		}
		if (p == q->LL)
			q->LL = NULL;
		else
			q->RL = NULL;
	}
	else if ((p->LL != NULL) && (p->RL == NULL)) { // only exist left child
		if (p == tree->root)
			tree->root = p->LL;
		else {
			if (p == q->LL)
				q->LL = p->LL;
			else
				q->RL = p->LL;
		}
	}
	else if ((p->LL == NULL) && (p->RL != NULL)) { // only exist right child
		if (p == tree->root)
			tree->root = p->RL;
		else {
			if (p == q->LL)
				q->LL = p->RL;
			else
				q->RL = p->RL;
		}
	}
	else if ((p->LL != NULL) && (p->RL != NULL)) { // both exist right and left child
		q = p;
		p = p->LL;
		t = p;
		while (1)
		{
			if (p->RL == NULL)
				break;
			else {
				t = p;
				p = p->RL;
			}
		}
		q->data = p->data;
		if (t != p) {
			if (p->LL != NULL)
				t->RL = p->LL;
			else
				t->RL = NULL;
		}
		else {
			q->LL = p->LL;
		}
	}
	pthread_mutex_unlock(&lock); //unlocking
	return LAB2_SUCCESS;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int x) { // coarse-grained node delete function
	pthread_mutex_lock(&lock); // locking
	lab2_node *p = tree->root;
	lab2_node *q = NULL;
	lab2_node *t;
	while (1){ // until find the node that you want to delete
		if (x == (p->data)) // if the data you want to delete and the root are the same
			break;
		else if (x < (p->data)) { // if smaller than parent node, go to leftchild
			if (p->LL == NULL) {
				pthread_mutex_unlock(&lock); //unlocking
				return LAB2_ERROR; // doesn't exist that want to find
			}
			q = p;
			p = p->LL;
		}
		else { // if bigger than parent node, go to rightchild
			if (p->RL == NULL) {
				pthread_mutex_unlock(&lock); //unlocking
				return LAB2_ERROR; // doesn't exist that want to find
			}
			q = p;
			p = p->RL;
		}
	}
	if ((p->LL == NULL) && (p->RL == NULL))	{ // no child
		if (p == tree->root) {
			tree->root = NULL;
			pthread_mutex_unlock(&lock); //unlocking
			return LAB2_SUCCESS;
		}
		if (p == q->LL)
			q->LL = NULL;
		else
			q->RL = NULL;
	}
	else if ((p->LL != NULL) && (p->RL == NULL)) { // only exist left child
		if (p == tree->root)
			tree->root = p->LL;
		else {
			if (p == q->LL)
				q->LL = p->LL;
			else
				q->RL = p->LL;
		}
	}
	else if ((p->LL == NULL) && (p->RL != NULL)) { // only exist right child
		if (p == tree->root)
			tree->root = p->RL;
		else {
			if (p == q->LL)
				q->LL = p->RL;
			else
				q->RL = p->RL;
		}
	}
	else if ((p->LL != NULL) && (p->RL != NULL)) { // both exist right and left child
		q = p;
		p = p->LL;
		t = p;
		while (1)
		{
			if (p->RL == NULL)
				break;
			else {
				t = p;
				p = p->RL;
			}
		}
		q->data = p->data;
		if (t != p) {
			if (p->LL != NULL)
				t->RL = p->LL;
			else
				t->RL = NULL;
		}
		else {
			q->LL = p->LL;
		}
	}
	pthread_mutex_unlock(&lock); //unlocking
	return LAB2_SUCCESS;
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
int lab2_tree_delete(lab2_tree *tree) { // delete tree function
	free(tree); // free the tree
	return LAB2_SUCCESS;
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
int lab2_node_delete(lab2_node *node) { // delete node function
	free(node); // free the node
	return LAB2_SUCCESS;
}

