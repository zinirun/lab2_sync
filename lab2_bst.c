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
pthread_mutex_t mutex;

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
    pthread_mutex_init(&tree->mutex,NULL);
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
    pthread_mutex_init(&node->mutex,NULL);
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
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
    lab2_node *p= tree->root; //declare p
    lab2_node *q= NULL; //declare q
    while(p != NULL) { //To a node without child node
        q = p;
        if (new_node->key == (p->key)) {
            return LAB2_ERROR;
        }
        else if (new_node->key > (p->key)) { //new_node > parent node, go to right child node
            p = p->right;
        }
        else { //new_node < parent node, go to left child node
            p = p->left;
        }
    }

    if((tree->root) == NULL) { //if root is not exist
        (tree->root) = new_node; //new_node is be a root
    }
    else if (new_node->key < (q->key)) { //new_node < parent node
        q->left = new_node; //new_node be a left child
    }
    else { //new_node > parent node
        q->right = new_node; //new_node be a right child
    }

    return LAB2_SUCCESS;
    // You need to implement lab2_node_insert function.
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
    lab2_node *p= tree->root; //declare p
    lab2_node *q= NULL; //declare q
    while(p != NULL) { //To a node without child node
        q = p;
        if (new_node->key == (p->key)) {
            return LAB2_ERROR;
        }
        else if (new_node->key > (p->key)) { //new_node > parent node, go to right child node
            p = p->right;
        }
        else { //new_node < parent node, go to left child node
            p = p->left;
        }
    }
    pthread_mutex_lock(&mutex);
    if((tree->root) == NULL) { //if root is not exist
        (tree->root) = new_node; //new_node is be a root
    }
    else if (new_node->key < (q->key)) { //new_node < parent node
        q->left = new_node; //new_node be a left child
    }
    else { //new_node > parent node
        q->right = new_node; //new_node be a right child
    }
    pthread_mutex_unlock(&mutex);
    return LAB2_SUCCESS;
    // You need to implement lab2_node_insert function.
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
    pthread_mutex_lock(&mutex);
    lab2_node *p= tree->root; //declare p
    lab2_node *q= NULL; //declare q
    while(p != NULL) { //To a node without child node
        q = p;
        if (new_node->key == (p->key)) {
            return LAB2_ERROR;
        }
        else if (new_node->key > (p->key)) { //new_node > parent node, go to right child node
            p = p->right;
        }
        else { //new_node < parent node, go to left child node
            p = p->left;
        }
    }

    if((tree->root) == NULL) { //if root is not exist
        (tree->root) = new_node; //new_node is be a root
    }
    else if (new_node->key < (q->key)) { //new_node < parent node
        q->left = new_node; //new_node be a left child
    }
    else { //new_node > parent node
        q->right = new_node; //new_node be a right child
    }
    pthread_mutex_unlock(&mutex);
    return LAB2_SUCCESS;
    // You need to implement lab2_node_insert function.
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

    pthread_mutex_lock(&tree -> mutex);
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
    pthread_mutex_unlock(&tree->mutex);

    pthread_mutex_lock(&tree->mutex);
    if ((p->left == NULL) && (p->right == NULL)){// does not exist child node
        if (p == tree->root){
            tree->root = NULL;
            pthread_mutex_unlock(&tree->mutex);
            return LAB2_SUCCESS;
        }
        if (p == q->left)
            q->left = NULL;
        else
            q->right = NULL;
        pthread_mutex_unlock(&tree->mutex);
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
        //pthread_mutex_unlock(&tree->mutex);
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
        //pthread_mutex_unlock(&tree->mutex);
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
        //pthread_mutex_unlock(&tree->mutex);
    }
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
    lab2_node *p = tree->root;
    lab2_node *q = NULL;
    lab2_node *tmp;
    if(p == NULL)
    {
        pthread_mutex_unlock(&mutex);
        return LAB2_ERROR;
    }
    while (1){//until node to remove
        if (key == (p->key)) //node to remove == root
            break;
        else if (key < (p->key)){ //node to remove < parent node, go to left child node
            if (p->left == NULL){
                pthread_mutex_unlock(&mutex);
                return LAB2_ERROR; //does not exist will remove node
            }
            q = p;
            p = p->left;
        }
        else { //node to remove > parent node, go to right child node
            if (p->right == NULL){
                pthread_mutex_unlock(&mutex);
                return LAB2_ERROR; //does not exist will remove node
            }
            q = p;
            p = p->right;
        }
    }
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
int lab2_tree_delete(lab2_tree *tree) {//delete tree fuction
    free(tree);
    tree = NULL;
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
int lab2_node_delete(lab2_node *node) { //delete node fuction
    free(node);
    node = NULL;
    // You need to implement lab2_node_delete function.
}

