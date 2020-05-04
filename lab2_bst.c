/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 32160000, 32162436
*	    Student name : 허전진, 신창
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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void inorder(lab2_tree *tree, lab2_node *C_Node){ //add inorder fuction
    if (C_Node != NULL){ //C_node is not null
        inorder(tree, C_Node->left);
        inorder(tree, C_Node->right);
    }
}

int lab2_node_print_inorder(lab2_tree *tree)
{
    inorder(tree, tree->root);
    return LAB2_SUCCESS; //LAB2_SUCCESS`s value is 0
    // You need to implement lab2_node_print_inorder function.
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
    lab2_tree *tree = (lab2_tree *)malloc(sizeof(lab2_tree)); //create tree by dynamic allocation
    tree->root = NULL;
    pthread_mutex_init(&tree->mutex, NULL);
    return tree;
    // You need to implement lab2_tree_create function.
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {
    lab2_node *node = (lab2_node *)malloc(sizeof(lab2_node)); //create node by dynamic allocation
    node->key = key;
    pthread_mutex_init(&node->mutex, NULL);
    return node;
    // You need to implement lab2_node_create function.
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node) {
    lab2_node *tmp = tree -> root;
    if(tmp == NULL) { //if tree is NULL
        tree -> root = new_node; // make root with new_node
    }
    else {
        while(1) {
            if(tmp -> key < new_node -> key) { // if inserting key < current key
                if(!(tmp -> right)) { // if current's right child is NULL
                    tmp -> right =  new_node; // Make new node at that point
                    break;
                }
                else{
                    tmp = tmp -> right; // if right child not NULL, re-search to right child
                }
            }
            
            else if(tmp -> key > new_node -> key) { // if inserting key > current key
                if(!(tmp -> left)) { // if current's left child is NULL
                    tmp -> left = new_node; // Make new node at that point
                    break;
                }
                else{
                    tmp = tmp -> left; // if left child not NULL, re-search to left child
                }
            }
            else break;
        }
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
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
      // You need to implement lab2_node_insert_fg function.
    lab2_node *tmp = tree -> root;
    if(tmp == NULL) { //if tree is NULL
        pthread_mutex_lock(&tree -> mutex);
        tree -> root = new_node; // make root with new_node
        pthread_mutex_unlock(&tree -> mutex);
    }
    else {
        while(1) {
            if(tmp -> key < new_node -> key) { // if inserting key < current key
                if(!(tmp -> right)) { // if current's right child is NULL
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp -> right =  new_node; // Make new node at that point
                    pthread_mutex_unlock(&tmp -> mutex);
                    break;
                }
                else{
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp = tmp -> right; // if right child not NULL, re-search to right child
                    pthread_mutex_unlock(&tmp -> mutex);
                }
            }
            else if(tmp -> key > new_node -> key) { // if inserting key > current key
                if(!(tmp -> left)) { // if current's left child is NULL
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp -> left = new_node; // Make new node at that point
                    pthread_mutex_unlock(&tmp -> mutex);
                    break;
                }
                else{
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp = tmp -> left; // if right left not NULL, re-search to left child
                    pthread_mutex_unlock(&tmp -> mutex);
                }
            }
            else break;
        }
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
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert_cg function. // lock 걸어줌
    pthread_mutex_lock(&mutex);
    lab2_node *tmp = tree -> root;
    if(tmp == NULL) { //if tree is NULL
        tree -> root = new_node; // make root with new_node
    }
    else {
        while(1) {
            if(tmp -> key < new_node -> key) { // if inserting key < current key
                if(!(tmp -> right)) { // if current's right child is NULL
                    tmp -> right =  new_node; // Make new node at that point
                    break;
                }
                else{
                tmp = tmp -> right; // if right child not NULL, re-search to right child
                }
            }
            else if(tmp -> key > new_node -> key) { // if inserting key > current key
                if(!(tmp -> left)) { // if current's left child is NULL
                    tmp -> left = new_node; // Make new node at that point
                    break;
                }
                else{
                tmp = tmp -> left; // if left child not NULL, re-search to left child
                }
            }
            else break;
        }
    }
    pthread_mutex_unlock(&mutex);
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
int lab2_node_remove(lab2_tree *tree, int key){
    lab2_node* temp = tree -> root;
    lab2_node* parent = temp;
    lab2_node* del; 

    while(temp->key != key){ //현재 노드의 키값이 제거하고자 하는 키값과 같을 때까지 반복
		parent = temp; //부모노드를 저장한다.

		if(temp->key < key){//현재 노드의 키값보다 제거하고자 하는 키값이 크면
			temp = temp -> right; //오른쪽자식으로 옮겨 재탐색
		}
		else //현재 노드의 키값보다 제거하고자 하는 키값이 작으면
			temp = temp -> left; //왼쪽자식으로 옮겨 재탐색
	}
	del = temp; //제거하고자 하는 키값을 가지고있는 노드의 위치를 저장.

	/* 제거할 때 총 3가지 경우 */

	// 1. 제거하고자 하는 노드의 자식이 하나도 없을 때
	if(temp->left==NULL && temp->right ==NULL){
		if(parent->left){//부모의 왼쪽자식이 존재하고
			if(temp->key==parent->left->key){ //부모의 왼쪽자식의 key와 제거하고자하는 temp노드의 키가 같다면
				parent->left=NULL; return 1; // 부모의 왼쪽노드를 NULL로 채운다.
			}
		}
		parent->right=NULL;    //그게 아니면 부모의 오른쪽자식을 NULL로 채운다.
		return 1;
		
	}
	// 2. 제거하고자 하는 노드의 자식이 모두(2개) 있을 때
	else if(temp->left!=NULL && temp->right !=NULL){
		temp = temp->left; //제거하고자 하는 노드의 왼쪽자식으로 내려간다.
		if(temp->right ==NULL){ //왼쪽자식에 오른쪽자식이 없으면  
				del->key = temp->key; // 제거하고자하는 노드에 왼쪽 자식의 key를 복사한다.
				del->left = temp->left; //제거하고자하는 노드에 왼쪽 자식의 왼쪽 자식을 붙인다.
				return 1;
			}
		while(1){
			if(temp->right ==NULL){ // 제거하고자하는 노드의 왼쪽 서브트리에서 key값이 가장 큰 노드 발견하면
				del->key = temp->key;   // 제거하고자하는 노드에 가장 큰 key를 가지는 노드의 key를 복사한다.
				parent->right = temp->left; // 가장 큰 key를 가지는 노드의 왼쪽자식을 부모의 오른쪽에 붙인다.
				return 1;
			}
			else{ //왼쪽자식에 오른쪽자식이 있으면
				parent = temp;    //해당노드를 부모로 저장하고
				temp= temp->right; // 오른쪽 자식으로 노드를 옮겨 재탐색.
			}
		}
	}
	// 3. 제거하고자 하는 노드의 자식이 왼쪽이나 오른쪽 둘 중 하나만 있을 때
	else{
		if(temp->right != NULL){ //제거하고자 하는 노드가 오른쪽자식을 가지고 있고
			if(parent->left->key == key){ //부모의 왼쪽에 달려있으면
				parent->left =temp->right;  //부모의 왼쪽에 제거하고자하는 노드의 오른쪽자식을 붙인다.
			}
			else{parent->right =temp->right;}  //부모의 오른쪽에 달려있으면 오른쪽에 붙인다.
			
			return 1;	
		}
		if(temp->left != NULL){  //제거하고자 하는 노드가 왼쪽자식을 가지고 있고
			if(parent->left->key == key){ //부모의 왼쪽에 달려있으면
				parent->left =temp->left; //부모의 왼쪽에 제거하고자하는 노드의 왼쪽자식을 붙인다.
			}
			else{parent->right =temp->left;} //부모의 오른쪽에 달려있으면 오른쪽에 붙인다.
			
			return 1;	
		}
	}
    lab2_node_delete(temp);
    return LAB2_SUCCESS;
    // You need to implement lab2_node_remove function.
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
    lab2_node* temp = tree -> root;
    lab2_node* parent = temp;
    lab2_node* del;

    while(temp->key != key){ //현재 노드의 키값이 제거하고자 하는 키값과 같을 때까지 반복
		parent = temp; //부모노드를 저장한다.

		if(temp->key < key){//현재 노드의 키값보다 제거하고자 하는 키값이 크면
			temp = temp -> right; //오른쪽자식으로 옮겨 재탐색
		}
		else //현재 노드의 키값보다 제거하고자 하는 키값이 작으면
			temp = temp -> left; //왼쪽자식으로 옮겨 재탐색
	}
	del = temp; //제거하고자 하는 키값을 가지고있는 노드의 위치를 저장.

	/* 제거할 때 총 3가지 경우 */

	// 1. 제거하고자 하는 노드의 자식이 하나도 없을 때
	if(temp->left==NULL && temp->right ==NULL){
		if(parent->left){//부모의 왼쪽자식이 존재하고
			if(temp->key==parent->left->key){ //부모의 왼쪽자식의 key와 제거하고자하는 temp노드의 키가 같다면
				parent->left=NULL; return 1; // 부모의 왼쪽노드를 NULL로 채운다.
			}
		}
		parent->right=NULL;    //그게 아니면 부모의 오른쪽자식을 NULL로 채운다.
		return 1;
		
	}
	// 2. 제거하고자 하는 노드의 자식이 모두(2개) 있을 때
	else if(temp->left!=NULL && temp->right !=NULL){
		temp = temp->left; //제거하고자 하는 노드의 왼쪽자식으로 내려간다.
		if(temp->right ==NULL){ //왼쪽자식에 오른쪽자식이 없으면  
				del->key = temp->key; // 제거하고자하는 노드에 왼쪽 자식의 key를 복사한다.
				del->left = temp->left; //제거하고자하는 노드에 왼쪽 자식의 왼쪽 자식을 붙인다.
				return 1;
			}
		while(1){
			if(temp->right ==NULL){ // 제거하고자하는 노드의 왼쪽 서브트리에서 key값이 가장 큰 노드 발견하면
				del->key = temp->key;   // 제거하고자하는 노드에 가장 큰 key를 가지는 노드의 key를 복사한다.
				parent->right = temp->left; // 가장 큰 key를 가지는 노드의 왼쪽자식을 부모의 오른쪽에 붙인다.
				return 1;
			}
			else{ //왼쪽자식에 오른쪽자식이 있으면
				parent = temp;    //해당노드를 부모로 저장하고
				temp= temp->right; // 오른쪽 자식으로 노드를 옮겨 재탐색.
			}
		}
	}
	// 3. 제거하고자 하는 노드의 자식이 왼쪽이나 오른쪽 둘 중 하나만 있을 때
	else{
		if(temp->right != NULL){ //제거하고자 하는 노드가 오른쪽자식을 가지고 있고
			if(parent->left->key == key){ //부모의 왼쪽에 달려있으면
				parent->left =temp->right;  //부모의 왼쪽에 제거하고자하는 노드의 오른쪽자식을 붙인다.
			}
			else{parent->right =temp->right;}  //부모의 오른쪽에 달려있으면 오른쪽에 붙인다.
			return 1;	
		}
		if(temp->left != NULL){  //제거하고자 하는 노드가 왼쪽자식을 가지고 있고
			if(parent->left->key == key){ //부모의 왼쪽에 달려있으면
				parent->left =temp->left; //부모의 왼쪽에 제거하고자하는 노드의 왼쪽자식을 붙인다.
			}
			else{parent->right =temp->left;} //부모의 오른쪽에 달려있으면 오른쪽에 붙인다.
			return 1;	
		}
	}
    pthread_mutex_lock(&tree -> mutex);
    lab2_node_delete(temp);
    pthread_mutex_unlock(&tree -> mutex);
    return LAB2_SUCCESS;
    // You need to implement lab2_node_remove_fg function.
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    pthread_mutex_lock(&mutex);
    lab2_node* temp = tree -> root;
    lab2_node* parent = temp;
    lab2_node* del;

    while(temp->key != key){ //현재 노드의 키값이 제거하고자 하는 키값과 같을 때까지 반복
		parent = temp; //부모노드를 저장한다.

		if(temp->key < key){//현재 노드의 키값보다 제거하고자 하는 키값이 크면
			temp = temp -> right; //오른쪽자식으로 옮겨 재탐색
		}
		else //현재 노드의 키값보다 제거하고자 하는 키값이 작으면
			temp = temp -> left; //왼쪽자식으로 옮겨 재탐색
	}
	del = temp; //제거하고자 하는 키값을 가지고있는 노드의 위치를 저장.

	/* 제거할 때 총 3가지 경우 */

	// 1. 제거하고자 하는 노드의 자식이 하나도 없을 때
	if(temp->left==NULL && temp->right ==NULL){
		if(parent->left){//부모의 왼쪽자식이 존재하고
			if(temp->key==parent->left->key){ //부모의 왼쪽자식의 key와 제거하고자하는 temp노드의 키가 같다면
				parent->left=NULL; return 1; // 부모의 왼쪽노드를 NULL로 채운다.
			}
		}
		parent->right=NULL;    //그게 아니면 부모의 오른쪽자식을 NULL로 채운다.
		
		return 1;
		
	}
	// 2. 제거하고자 하는 노드의 자식이 모두(2개) 있을 때
	else if(temp->left!=NULL && temp->right !=NULL){
		temp = temp->left; //제거하고자 하는 노드의 왼쪽자식으로 내려간다.
		if(temp->right ==NULL){ //왼쪽자식에 오른쪽자식이 없으면  
				del->key = temp->key; // 제거하고자하는 노드에 왼쪽 자식의 key를 복사한다.
				del->left = temp->left; //제거하고자하는 노드에 왼쪽 자식의 왼쪽 자식을 붙인다.
				
				return 1;
			}
		while(1){
			if(temp->right ==NULL){ // 제거하고자하는 노드의 왼쪽 서브트리에서 key값이 가장 큰 노드 발견하면
				del->key = temp->key;   // 제거하고자하는 노드에 가장 큰 key를 가지는 노드의 key를 복사한다.
				parent->right = temp->left; // 가장 큰 key를 가지는 노드의 왼쪽자식을 부모의 오른쪽에 붙인다.
				
				return 1;
			}
			else{ //왼쪽자식에 오른쪽자식이 있으면
				parent = temp;    //해당노드를 부모로 저장하고
				temp= temp->right; // 오른쪽 자식으로 노드를 옮겨 재탐색.
			}
		}
	}
	// 3. 제거하고자 하는 노드의 자식이 왼쪽이나 오른쪽 둘 중 하나만 있을 때
	else{
		if(temp->right != NULL){ //제거하고자 하는 노드가 오른쪽자식을 가지고 있고
			if(parent->left->key == key){ //부모의 왼쪽에 달려있으면
				parent->left =temp->right;  //부모의 왼쪽에 제거하고자하는 노드의 오른쪽자식을 붙인다.
			}
			else{parent->right =temp->right;}  //부모의 오른쪽에 달려있으면 오른쪽에 붙인다.
			
			return 1;	
		}
		if(temp->left != NULL){  //제거하고자 하는 노드가 왼쪽자식을 가지고 있고
			if(parent->left->key == key){ //부모의 왼쪽에 달려있으면
				parent->left =temp->left; //부모의 왼쪽에 제거하고자하는 노드의 왼쪽자식을 붙인다.
			}
			else{parent->right =temp->left;} //부모의 오른쪽에 달려있으면 오른쪽에 붙인다.
			
			return 1;	
		}
	}
    pthread_mutex_unlock(&mutex);
    return LAB2_SUCCESS;
    // You need to implement lab2_node_remove_cg function.
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {//delete tree fuction
    lab2_node *temp = tree -> root;
    if(!temp) return;
    while(temp) {
        int key = temp -> key;
        lab2_node_remove(tree, key);
        temp = tree -> root;
    }
    // You need to implement lab2_tree_delete function.
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) { //delete node fuction
    free(node);
    node = NULL;
    // You need to implement lab2_node_delete function.
}