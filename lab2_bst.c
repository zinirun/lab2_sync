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
    else {
        inorder(node -> left);
        inorder(node -> right);
    }
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
lab2_node *lab2_node_create(int key) {
    // You need to implement lab2_node_create function.
    lab2_node *node = (lab2_node *) malloc(sizeof(lab2_node));
    node -> key = key;
    node -> left = NULL;
    node -> right = NULL;
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
                else tmp = tmp -> right; // if right child not NULL, re-search to right child
            }
            
            else if(tmp -> key > new_node -> key) { // if inserting key > current key
                if(!(tmp -> left)) { // if current's left child is NULL
                    tmp -> left = new_node; // Make new node at that point
                    break;
                }
                else tmp = tmp -> left; // if left child not NULL, re-search to left child
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
                else {
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
                else tmp = tmp -> right; // if right child not NULL, re-search to right child
            }
            
            else if(tmp -> key > new_node -> key) { // if inserting key > current key
                if(!(tmp -> left)) { // if current's left child is NULL
                    tmp -> left = new_node; // Make new node at that point
                    break;
                }
                else tmp = tmp -> left; // if left child not NULL, re-search to left child
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
int lab2_node_remove(lab2_tree *tree, int key) {
    lab2_node *tmp = tree -> root;
    lab2_node *parent = NULL;
    lab2_node *child, *del, *del_p;
    while(tmp != NULL && (tmp -> key != key)) {
        parent = tmp;
        if(tmp -> key < key) {
            tmp = tmp -> right;
        } else {
            tmp = tmp -> left;
        }
    }
    if(tmp == NULL) {
        return LAB2_SUCCESS;
    }
    if((tmp -> left == NULL) && (tmp -> right == NULL)) {
        if (parent != NULL)
        {
            if(parent -> left == tmp) {
                parent -> left = NULL;
            } else {
                parent -> right = NULL;
            }
        } else {
            tree -> root = NULL;
        }
        
    } else if(tmp -> left == NULL || tmp -> right == NULL) {
        child = (tmp -> left != NULL) ? tmp -> left : tmp -> right;
        if(parent != NULL) {
            if(parent -> left == tmp) {
                parent -> left = child;
            } else {
                parent -> right = child;
            }
        } else {
            tree -> root = child;
        }
    } else {
        del_p = tmp;
        del = tmp -> right;
        while(del -> left != NULL) {
            del_p = del;
            del = del -> left;
        }
        if(del_p -> left == del) {
            del_p -> left = del -> right;
        } else {
            del_p -> right = del -> right;
        }
        tmp -> key = del -> key;
        tmp = del;
    }
    lab2_node_delete(tmp);
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
int lab2_node_remove_fg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_fg function.
    pthread_mutex_lock(&mutex);
    lab2_node *tmp = tree -> root;
    lab2_node *parent = NULL;
    lab2_node *child, *del, *del_p;
    while(tmp != NULL && (tmp -> key != key)) {
        parent = tmp;
        if(tmp -> key < key) {
            tmp = tmp -> right;
        } else {
            tmp = tmp -> left;
        }
    }
    if(tmp == NULL) {
        return LAB2_SUCCESS;
    }
    if((tmp -> left == NULL) && (tmp -> right == NULL)) {
        if (parent != NULL)
        {
            if(parent -> left == tmp) {
                parent -> left = NULL;
            } else {
                parent -> right = NULL;
            }
        } else {
            tree -> root = NULL;
        }
        
    } else if(tmp -> left == NULL || tmp -> right == NULL) {
        child = (tmp -> left != NULL) ? tmp -> left : tmp -> right;
        if(parent != NULL) {
            if(parent -> left == tmp) {
                parent -> left = child;
            } else {
                parent -> right = child;
            }
        } else {
            tree -> root = child;
        }
    } else {
        del_p = tmp;
        del = tmp -> right;
        while(del -> left != NULL) {
            del_p = del;
            del = del -> left;
        }
        if(del_p -> left == del) {
            del_p -> left = del -> right;
        } else {
            del_p -> right = del -> right;
        }
        tmp -> key = del -> key;
        tmp = del;
    }
    lab2_node_delete(tmp);
    pthread_mutex_unlock(&mutex);
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
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    pthread_mutex_lock(&mutex);
    lab2_node *tmp = tree -> root;
    lab2_node *parent = NULL;
    lab2_node *child, *del, *del_p;
    while(tmp != NULL && (tmp -> key != key)) {
        parent = tmp;
        if(tmp -> key < key) {
            tmp = tmp -> right;
        } else {
            tmp = tmp -> left;
        }
    }
    if(tmp == NULL) {
        return LAB2_SUCCESS;
    }
    if((tmp -> left == NULL) && (tmp -> right == NULL)) {
        if (parent != NULL)
        {
            if(parent -> left == tmp) {
                parent -> left = NULL;
            } else {
                parent -> right = NULL;
            }
        } else {
            tree -> root = NULL;
        }
        
    } else if(tmp -> left == NULL || tmp -> right == NULL) {
        child = (tmp -> left != NULL) ? tmp -> left : tmp -> right;
        if(parent != NULL) {
            if(parent -> left == tmp) {
                parent -> left = child;
            } else {
                parent -> right = child;
            }
        } else {
            tree -> root = child;
        }
    } else {
        del_p = tmp;
        del = tmp -> right;
        while(del -> left != NULL) {
            del_p = del;
            del = del -> left;
        }
        if(del_p -> left == del) {
            del_p -> left = del -> right;
        } else {
            del_p -> right = del -> right;
        }
        tmp -> key = del -> key;
        tmp = del;
    }
    lab2_node_delete(tmp);
    pthread_mutex_unlock(&mutex);
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
void lab2_tree_delete(lab2_tree *tree) {
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
void lab2_node_delete(lab2_node *node) { // 삭제한 노드의 메모리 할당 제거
    free(node);
    node = NULL;
}