/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 
*	    Student name : 
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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
void inorder(lab2_node *node) {
    if(!node) return;
    inorder(node -> left);
    inorder(node -> right);
}
int lab2_node_print_inorder(lab2_tree * tree) {
    inorder(tree -> root);
    return LAB2_SUCCESS;
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
    // You need to implement lab2_tree_create function.
    lab2_tree *tree = (lab2_tree *) malloc(sizeof(lab2_tree));
    tree -> root = NULL;
    pthread_mutex_init(&tree->mutex, NULL);
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
lab2_node * lab2_node_create(int key) {
    // You need to implement lab2_node_create function.
    lab2_node *node = (lab2_node *) malloc(sizeof(lab2_node));
    node -> key = key;
    node -> left = NULL; node -> right = NULL;
    pthread_mutex_init(&node->mutex, NULL);
    return node;
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
    if(tmp == NULL) {
        tree -> root = new_node; // root媛 NULL?대㈃ ?덈줈???몃뱶瑜?root濡??ㅼ젙
    } else {
        while(1) {
            if(tmp -> key < new_node -> key) { // 鍮꾧탳 ?몃뱶???ㅺ컪蹂대떎 異붽????몃뱶???ㅺ컪??????
                if(!(tmp -> right)) { // 鍮꾧탳?몃뱶???ㅻⅨ履??먯떇??NULL ????
                    tmp -> right =  new_node;
                    return LAB2_SUCCESS;
                }
                else{
                    tmp = tmp -> right;
                }
            } else if(tmp -> key > new_node -> key) { // 鍮꾧탳 ?몃뱶???ㅺ컪蹂대떎 異붽????몃뱶???ㅺ컪???묒쓣 ??
                if(!(tmp -> left)) { // 鍮꾧탳?몃뱶???쇱そ ?먯떇??NULL ????
                    tmp -> left = new_node;
                    return LAB2_SUCCESS;
                }
                else{
                    tmp = tmp -> left;
                }
            } else {
                return LAB2_ERROR;
            }
        }
    }
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
    if(tmp == NULL) {
        pthread_mutex_lock(&tree -> mutex);
        tree -> root = new_node;
        pthread_mutex_unlock(&tree -> mutex);
    } else {
        while(1){
            if(tmp -> key < new_node -> key) {
                if(!(tmp -> right)) {
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp -> right =  new_node;
                    pthread_mutex_unlock(&tmp -> mutex);
                    return LAB2_SUCCESS;
                }
                else{
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp = tmp -> right;
                    pthread_mutex_unlock(&tmp -> mutex);
                }
            }else if(tmp -> key > new_node -> key) {
                if(!(tmp -> left)) {
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp -> left = new_node;
                    pthread_mutex_unlock(&tmp -> mutex);
                    return LAB2_SUCCESS;
                }
                else{
                    pthread_mutex_lock(&tmp -> mutex);
                    tmp = tmp -> left;
                    pthread_mutex_unlock(&tmp -> mutex);
                }
            } else {
                return LAB2_ERROR;
            }
        }
    }
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
    // You need to implement lab2_node_insert_cg function. // lock 嫄몄뼱以?
    pthread_mutex_lock(&mutex);
    lab2_node *tmp = tree -> root;
    if(tmp == NULL) {
	tree -> root = new_node;
	pthread_mutex_unlock(&mutex);
    return LAB2_SUCCESS;
    } else {
        while(1) {
            if(tmp -> key < new_node -> key) {
                if(!(tmp -> right)) {
                    tmp -> right =  new_node;
                    pthread_mutex_unlock(&mutex); // lock ?댁젣
                    return LAB2_SUCCESS;
                }
                tmp = tmp -> right;
            } else if(tmp -> key > new_node -> key) {
                if(!(tmp -> left)) {
                    tmp -> left = new_node;
                    pthread_mutex_unlock(&mutex); // lock ?댁젣
                    return LAB2_SUCCESS;
                }
                tmp = tmp -> left;
            } else {
                pthread_mutex_unlock(&mutex); // lock ?댁젣
                return LAB2_ERROR;
            }
        }
    }
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
    lab2_node *p = tree->root;
    lab2_node *q = NULL;
    lab2_node *tmp;
    if(p == NULL)
    {
        return LAB2_ERROR;
    }
    while (1){//until node to remove
        if (key == (p->key)) //node to remove == root
            break;
        else if (key < (p->key)){ //node to remove < parent node, go to left child node
            if (p->left == NULL){
                return LAB2_ERROR; //does not exist will remove node
            }
            q = p;
            p = p->left;
        }
        else { //node to remove > parent node, go to right child node
            if (p->right == NULL){
                return LAB2_ERROR; //does not exist will remove node
            }
            q = p;
            p = p->right;
        }
    }
    if ((p->left == NULL) && (p->right == NULL)){// does not exist child node
        if (p == tree->root){
            tree->root = NULL;
            return LAB2_SUCCESS;
        }
        if (p == q->left)
            q->left = NULL;
        else
            q->right = NULL;
    }
    else if ((p->left != NULL) && (p->right == NULL)){// only exist left child node
        if (p == tree->root)
            tree->root = p->left;
        else {
            if (p == q->left)
                q->left = p->left;
            else
                q->right = p->left;
        }
    }
    else if ((p->left == NULL) && (p->right != NULL)){// only exist right child node
        if (p == tree->root)
            tree->root = p->right;
        else {
            if (p == q->left)
                q->left = p->right;
            else
                q->right = p->right;
        }
    }
    else if ((p->left != NULL) && (p->right != NULL)) { // both exist right child node and left child node
        q = p;
        p = p->left;
        tmp = p;
        while (1)
        {
            if (p->right == NULL)
                break;
            else {
                tmp = p;
                p = p->right;
            }
        }
        q->key = p->key;
        if (tmp != p) {
            if (p->left != NULL)
                tmp->right = p->left;
            else
                tmp->right = NULL;
        }
        else {
            q->left = p->left;
        }
    }
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
    lab2_node *p = tree->root;
    lab2_node *q = NULL;
    lab2_node *tmp;
    if(p == NULL)
    {
        return LAB2_ERROR;
    }
    while (1){//until node to remove
        if (key == (p->key)) //node to remove == root
            break;
        else if (key < (p->key)){ //node to remove < parent node, go to left child node
            if (p->left == NULL){
                return LAB2_ERROR; //does not exist will remove node
            }
            q = p;
            p = p->left;
        }
        else { //node to remove > parent node, go to right child node
            if (p->right == NULL){
                return LAB2_ERROR; //does not exist will remove node
            }
            q = p;
            p = p->right;
        }
    }
    pthread_mutex_lock(&mutex);
    if ((p->left == NULL) && (p->right == NULL)){// does not exist child node
        if (p == tree->root){
            tree->root = NULL;
            pthread_mutex_unlock(&mutex);
            return LAB2_SUCCESS;
        }
        if (p == q->left)
            q->left = NULL;
        else
            q->right = NULL;
    }
    else if ((p->left != NULL) && (p->right == NULL)){// only exist left child node
        if (p == tree->root)
            tree->root = p->left;
        else {
            if (p == q->left)
                q->left = p->left;
            else
                q->right = p->left;
        }
    }
    else if ((p->left == NULL) && (p->right != NULL)){// only exist right child node
        if (p == tree->root)
            tree->root = p->right;
        else {
            if (p == q->left)
                q->left = p->right;
            else
                q->right = p->right;
        }
    }
    else if ((p->left != NULL) && (p->right != NULL)) { // both exist right child node and left child node
        q = p;
        p = p->left;
        tmp = p;
        while (1)
        {
            if (p->right == NULL)
                break;
            else {
                tmp = p;
                p = p->right;
            }
        }
        q->key = p->key;
        if (tmp != p) {
            if (p->left != NULL)
                tmp->right = p->left;
            else
                tmp->right = NULL;
        }
        else {
            q->left = p->left;
        }
    }
    pthread_mutex_unlock(&mutex);
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
    lab2_node *p = tree->root;
    lab2_node *q = NULL;
    lab2_node *tmp;
    if(p == NULL)
    {
        return LAB2_ERROR;
    }
    pthread_mutex_lock(&mutex);
    while (1){//until node to remove
        if (key == (p->key)) //node to remove == root
            break;
        else if (key < (p->key)){ //node to remove < parent node, go to left child node
            if (p->left == NULL){
                return LAB2_ERROR; //does not exist will remove node
            }
            q = p;
            p = p->left;
        }
        else { //node to remove > parent node, go to right child node
            if (p->right == NULL){
                return LAB2_ERROR; //does not exist will remove node
            }
            q = p;
            p = p->right;
        }
    }
    if ((p->left == NULL) && (p->right == NULL)){// does not exist child node
        if (p == tree->root){
            tree->root = NULL;
            return LAB2_SUCCESS;
        }
        if (p == q->left)
            q->left = NULL;
        else
            q->right = NULL;
    }
    else if ((p->left != NULL) && (p->right == NULL)){// only exist left child node
        if (p == tree->root)
            tree->root = p->left;
        else {
            if (p == q->left)
                q->left = p->left;
            else
                q->right = p->left;
        }
    }
    else if ((p->left == NULL) && (p->right != NULL)){// only exist right child node
        if (p == tree->root)
            tree->root = p->right;
        else {
            if (p == q->left)
                q->left = p->right;
            else
                q->right = p->right;
        }
    }
    else if ((p->left != NULL) && (p->right != NULL)) { // both exist right child node and left child node
        q = p;
        p = p->left;
        tmp = p;
        while (1)
        {
            if (p->right == NULL)
                break;
            else {
                tmp = p;
                p = p->right;
            }
        }
        q->key = p->key;
        if (tmp != p) {
            if (p->left != NULL)
                tmp->right = p->left;
            else
                tmp->right = NULL;
        }
        else {
            q->left = p->left;
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
void lab2_tree_delete(lab2_tree *tree) { // ?몃━瑜?珥덇린??
    lab2_node *tmp = tree -> root;
    if(!tmp) return;
    while(tmp) {
        int key = tmp -> key;
        lab2_node_remove(tree, key);
        tmp = tree -> root;
    }
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) { // ??젣???몃뱶??硫붾え由??좊떦 ?쒓굅
    free(node);
    node = NULL;
}
