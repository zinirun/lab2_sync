/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 32164959, 32162436
*	    Student name : 허전진, 신창우
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
    if (C_Node != NULL){ //current node is not null
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
    lab2_node *pre= tree->root; //declare pre
    lab2_node *cur= NULL; //declare cur
    while(pre != NULL) { //To a node without child node
        cur = pre;
        if (new_node->key == (pre->key)) {
            return LAB2_ERROR;
        }
        else if (new_node->key > (pre->key)) { //new_node > parent node, go to right child node
            pre = pre->right;
        }
        else { //new_node < parent node, go to left child node
            pre = pre->left;
        }
    }

    if((tree->root) == NULL) { //if root is not exist
        (tree->root) = new_node; //new_node is be a root
    }
    else if (new_node->key < (cur->key)) { //new_node < parent node
        cur->left = new_node; //new_node be a left child
    }
    else { //new_node > parent node
        cur->right = new_node; //new_node be a right child
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
    lab2_node *pre= tree->root; //declare pre
    lab2_node *cur= NULL; //declare cur
    while(pre != NULL) { //To a node without child node
        cur = pre;
        if (new_node->key == (pre->key)) {
            return LAB2_ERROR;
        }
        else if (new_node->key > (pre->key)) { //new_node > parent node, go to right child node
            pre = pre->right;
        }
        else { //new_node < parent node, go to left child node
            pre = pre->left;
        }
    }
    pthread_mutex_lock(&mutex);
    if((tree->root) == NULL) { //if root is not exist
        (tree->root) = new_node; //new_node is be a root
    }
    else if (new_node->key < (cur->key)) { //new_node < parent node
        cur->left = new_node; //new_node be a left child
    }
    else { //new_node > parent node
        cur->right = new_node; //new_node be a right child
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
    lab2_node *pre= tree->root; //declare pre
    lab2_node *cur= NULL; //declare cur
    while(pre != NULL) { //To a node without child node
        cur = pre;
        if (new_node->key == (pre->key)) {
            pthread_mutex_unlock(&mutex);
            return LAB2_ERROR;
        }
        else if (new_node->key > (pre->key)) { //new_node > parent node, go to right child node
            pre = pre->right;
        }
        else { //new_node < parent node, go to left child node
            pre = pre->left;
        }
    }

    if((tree->root) == NULL) { //if root is not exist
        (tree->root) = new_node; //new_node is be a root
    }
    else if (new_node->key < (cur->key)) { //new_node < parent node
        cur->left = new_node; //new_node be a left child
    }
    else { //new_node > parent node
        cur->right = new_node; //new_node be a right child
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
    lab2_node *pre = tree->root;
    lab2_node *cur = NULL;
    lab2_node *tmp;
    if(pre == NULL) // error handling if root is null
    {
        return LAB2_ERROR;
    }
    while (1){//until node to remove
        if (key == (pre->key)) //node to remove == root
            break;
        else if (key < (pre->key)){ //node to remove < parent node, go to left child node
            if (pre->left == NULL){
                return LAB2_ERROR; //does not exist will remove node
            }
            cur = pre;
            pre = pre->left;
        }
        else { //node to remove > parent node, go to right child node
            if (pre->right == NULL){
                return LAB2_ERROR; //does not exist will remove node
            }
            cur = pre;
            pre = pre->right;
        }
    }
    if ((pre->left == NULL) && (pre->right == NULL)){// does not exist child node
        if (pre == tree->root){
            tree->root = NULL;
            return LAB2_SUCCESS;
        }
        if (pre == cur->left)
            cur->left = NULL;
        else
            cur->right = NULL;
    }
    else if ((pre->left != NULL) && (pre->right == NULL)){// only exist left child node
        if (pre == tree->root)
            tree->root = pre->left;
        else {
            if (pre == cur->left)
                cur->left = pre->left;
            else
                cur->right = pre->left;
        }
    }
    else if ((pre->left == NULL) && (pre->right != NULL)){// only exist right child node
        if (pre == tree->root)
            tree->root = pre->right;
        else {
            if (pre == cur->left)
                cur->left = pre->right;
            else
                cur->right = pre->right;
        }
    }
    else if ((pre->left != NULL) && (pre->right != NULL)) { // both exist right child node and left child node
        cur = pre;
        pre = pre->left;
        tmp = pre;
        while (1)
        {
            if (pre->right == NULL)
                break;
            else {
                tmp = pre;
                pre = pre->right;
            }
        }
        cur->key = pre->key;
        if (tmp != pre) {
            if (pre->left != NULL)
                tmp->right = pre->left;
            else
                tmp->right = NULL;
        }
        else {
            cur->left = pre->left;
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
    lab2_node *pre = tree->root;
    lab2_node *cur = NULL;
    lab2_node *tmp;
    if(pre == NULL)
    {
        return LAB2_ERROR;
    }
    while (1){//until node to remove
        if (key == (pre->key)) //node to remove == root
            break;
        else if (key < (pre->key)){ //node to remove < parent node, go to left child node
            if (pre->left == NULL){
                return LAB2_ERROR; //does not exist will remove node
            }
            cur = pre;
            pre = pre->left;
        }
        else { //node to remove > parent node, go to right child node
            if (pre->right == NULL){
                return LAB2_ERROR; //does not exist will remove node
            }
            cur = pre;
            pre = pre->right;
        }
    }
    pthread_mutex_lock(&mutex); //lock
    if ((pre->left == NULL) && (pre->right == NULL)){// does not exist child node
        if (pre == tree->root){
            tree->root = NULL;
            pthread_mutex_unlock(&mutex); //unlock if return value
            return LAB2_SUCCESS;
        }
        if (pre == cur->left)
            cur->left = NULL;
        else
            cur->right = NULL;
    }
    else if ((pre->left != NULL) && (pre->right == NULL)){// only exist left child node
        if (pre == tree->root)
            tree->root = pre->left;
        else {
            if (pre == cur->left)
                cur->left = pre->left;
            else
                cur->right = pre->left;
        }
    }
    else if ((pre->left == NULL) && (pre->right != NULL)){// only exist right child node
        if (pre == tree->root)
            tree->root = pre->right;
        else {
            if (pre == cur->left)
                cur->left = pre->right;
            else
                cur->right = pre->right;
        }
    }
    else if ((pre->left != NULL) && (pre->right != NULL)) { // both exist right child node and left child node
        cur = pre;
        pre = pre->left;
        tmp = pre;
        while (1)
        {
            if (pre->right == NULL)
                break;
            else {
                tmp = pre;
                pre = pre->right;
            }
        }
        cur->key = pre->key;
        if (tmp != pre) {
            if (pre->left != NULL)
                tmp->right = pre->left;
            else
                tmp->right = NULL;
        }
        else {
            cur->left = pre->left;
        }
    }
    pthread_mutex_unlock(&mutex); //unlock if return value
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
    lab2_node *pre = tree->root;
    lab2_node *cur = NULL;
    lab2_node *tmp;
    if(pre == NULL)
    {
        return LAB2_ERROR;
    }
    pthread_mutex_lock(&mutex); //lock
    while (1){//until node to remove
        if (key == (pre->key)) //node to remove == root
            break;
        else if (key < (pre->key)){ //node to remove < parent node, go to left child node
            if (pre->left == NULL){
                pthread_mutex_unlock(&mutex); //unlock if return value
                return LAB2_ERROR; //does not exist will remove node
            }
            cur = pre;
            pre = pre->left;
        }
        else { //node to remove > parent node, go to right child node
            if (pre->right == NULL){
                pthread_mutex_unlock(&mutex); //unlock if return value
                return LAB2_ERROR; //does not exist will remove node
            }
            cur = pre;
            pre = pre->right;
        }
    }
    if ((pre->left == NULL) && (pre->right == NULL)){// does not exist child node
        if (pre == tree->root){
            tree->root = NULL;
            pthread_mutex_unlock(&mutex); //unlock if return value
            return LAB2_SUCCESS;
        }
        if (pre == cur->left)
            cur->left = NULL;
        else
            cur->right = NULL;
    }
    else if ((pre->left != NULL) && (pre->right == NULL)){// only exist left child node
        if (pre == tree->root)
            tree->root = pre->left;
        else {
            if (pre == cur->left)
                cur->left = pre->left;
            else
                cur->right = pre->left;
        }
    }
    else if ((pre->left == NULL) && (pre->right != NULL)){// only exist right child node
        if (pre == tree->root)
            tree->root = pre->right;
        else {
            if (pre == cur->left)
                cur->left = pre->right;
            else
                cur->right = pre->right;
        }
    }
    else if ((pre->left != NULL) && (pre->right != NULL)) { // both exist right child node and left child node
        cur = pre;
        pre = pre->left;
        tmp = pre;
        while (1)
        {
            if (pre->right == NULL)
                break;
            else {
                tmp = pre;
                pre = pre->right;
            }
        }
        cur->key = pre->key;
        if (tmp != pre) {
            if (pre->left != NULL)
                tmp->right = pre->left;
            else
                tmp->right = NULL;
        }
        else {
            cur->left = pre->left;
        }
    }
    pthread_mutex_unlock(&mutex); //unlock if return value
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
    lab2_node *tmp = tree -> root; 
    if(!tmp) // return if tmp is NULL (there's NULL in tree)
        return;
    while(tmp) {
        int key = tmp -> key;
        lab2_node_remove(tree, key);
        tmp = tree -> root;
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

