/* This is the implementation of the binary tree ADT
 * written by Abrie Greeff
*/
#include <stdlib.h>
#include <stdio.h>
#include "BinaryTree.h"

binaryTreePtr createTree(void (*deleteElement)(void *),
                         void (*dspElement)(void *),
                         int (*cmpElements)(void *, void *))
/* The first procedure to call, which allocates and initialises a tree.
 * Pre-condition: tree does not exists
 *                Valid pointers to functions deleteElement, dspElement and
 *                cmpElements are passed.
 * Post-condition: Empty tree exists or NULL returned.
*/
{
	binaryTreePtr tree = NULL;

	//allocate memory for the tree
	tree = malloc(sizeof(struct treeHeader));

	//was the tree created?
	if (tree == NULL)
	{
		return NULL;
	}else
	{
		//set all the initial tree pointers
		tree->root = NULL;
		tree->current = NULL;
		tree->size = 0;

		//set the pointers to the unique tree functions
		tree->deleteElement = deleteElement;
		tree->dspElement = dspElement;
		tree->cmpElements = cmpElements;

		return tree;
	}
}

int deleteNode(binaryTreePtr tree)
/* Delete current node from the tree. This procedure should not be called during
 * a traverseTree.
 * Pre-Condition: tree and current node exists.
 * Post-Condition: current node is deleted or error is returned.
*/
{
	//does the tree exist and isn't empty
	if ((tree == NULL) || (tree->current == NULL))
	{
		return -1;
	}else
	{
		//free the element the node points to
		tree->deleteElement(tree->current->element);
	}

	//create 2 temporary pointers for ease of reading of code
	treeNodePtr node = tree->current;
	treeNodePtr temp;
	//for storing the position of the node relative to the parent
	enum position where;

	//what is the position relative to the parent of the node?
	if (node == tree->root)
	{
		where = TOROOT;
	}else
	{
		//is it the left child?
		if (node->parent->left == node)
		{
			where = TOLEFT;
		}else
		{
			//is it the right child?
			if (node->parent->right == node)
			{
				where = TORIGHT;
			}
		}
	}
	//is the node a leaf?

	if ((node->left == NULL) & (node->right == NULL))
	{
		//do code relative to position
		switch(where)
		{
		//are we removing the root node?
		case TOROOT:
			tree->current = NULL;
			tree->root = NULL;
			free(node);
			break;
		//are we removing a left child?
		case TOLEFT:
			node->parent->left = NULL;
			tree->current = node->parent;
			free(node);
			break;
		//are we removing a right child?
		case TORIGHT:
			node->parent->right = NULL;
			tree->current = node->parent;
			free(node);
			break;
		}
		//did we successfully free the node?
		if (node == NULL)
		{
			//success
			--(tree->size);
			return 1;
		}else
		{
			//failure
			return -1;
		}
	}

	//Does the node have only one child?
	if ((node->left == NULL) || (node->right == NULL))
	{

		//which child of node will be moving up?
		if (node->left == NULL)
		{
			//the right child
			temp = node->right;
		}else
		{
			//the left child
			temp = node->left;
		}

		//do code relative to position
		switch(where)
		{
		//are we removing the root node?
		case TOROOT:
			tree->current = temp;
			tree->root = temp;
			free(node);
			break;
		//are we removing a left child?
		case TOLEFT:
			node->parent->left = temp;
			temp->parent = node->parent;
			tree->current = node->parent;
			free(node);
			break;
		//are we removing a right child?
		case TORIGHT:
			node->parent->right = temp;
			temp->parent = node->parent;
			tree->current = node->parent;
			free(node);
			break;
		}
		//did we successfully free the node?
		if (node == NULL)
		{
			//success
			--(tree->size);
			return 1;
		}else
		{
			//failure
			return -1;
		}
	}
	//does the node we're trying to remove have two children?
	if ((node->left != NULL) & (node->right != NULL))
	{
		temp = node->left;

		//find the right most node of the left subtree
		while (temp->right != NULL)
		{
			temp = temp->right;
		}
		//attach node's right child to the right most node of the left subtree
		temp->right = node->right;
		node->right->parent = temp;

		//do code relative to position
		switch(where)
		{
		//are we removing the root node?
		case TOROOT:
			tree->current = temp;
			tree->root = temp;
			free(node);
			break;
		//are we removing a left child?
		case TOLEFT:
			node->parent->left = temp;
			temp->parent = node->parent;
			tree->current = node->parent;
			free(node);
			break;
		//are we removing a right child?
		case TORIGHT:
			node->parent->right = temp;
			temp->parent = node->parent;
			tree->current = node->parent;
			free(node);
			break;
		}
		//did we successfully free the node?
		if (node == NULL)
		{
			//success
			--(tree->size);
			return 1;
		}else
		{
			//failure
			return -1;
		}
	}
}

void postOrderDeleteTree(binaryTreePtr tree)
/* Does a postorder traversal to delete the tree
 * Pre-condition: none
 * Post-condition: all nodes in the tree has been deleted
*/
{
	//get the current node
	treeNodePtr current = tree->current;
	//travese left
	if (current->left != NULL)
	{
		tree->current = current->left;
		postOrderDeleteTree(tree);
	}
	//traverse right
	if (current->right != NULL)
	{
		tree->current = current->right;
		postOrderDeleteTree(tree);
	}
	//back to current
	tree->current = current;
	//delete it
	deleteNode(tree);
}

binaryTreePtr deleteTree(binaryTreePtr tree)
/* All information about the tree and the data records it contains are deleted.
 * Pre-condition: tree exists
 * Post-condition: tree does not exist
*/
{


	if ((tree == NULL) || (tree->current == NULL))
	{
		return NULL;
	}else
	{
		//set current to the root
		tree->current = tree->root;
		//delete the nodes
		postOrderDeleteTree(tree);
	}
	//free the tree
	free(tree);
}

int insertNode(binaryTreePtr tree, void *elem, enum position where)
/* Insert elem into the tree. The key is in the element.
 * Pre-condition: tree and elem exist. where == TOROOT and tree is empty,
 *                                     where == TOLEFT and no left child,
 *                                     where == TPRIGHT and no right child.
 * Post-condition: node is inserted in the tree or error returned.
*/
{
	//allocate space in memory for the new node
	treeNodePtr node;
	node = malloc(sizeof(struct treeNode));

	//was there memory allocated for node?
	if (node == NULL)
	{
		return -1;
	}else
	{
		//initialize as a leaf
		node->left = NULL;
		node->right = NULL;
	}

	//does the tree exist and is the element pointing to a valid element
	if ((elem == NULL) || (tree == NULL))
	{
		//free the memory allocated and return a error
		free(node);
		return -1;
	}else
	{
		//give the leaf a element it points to
		node->element = elem;
	}

	//are we able to create a new root for the tree when asked to
	if ((where == TOROOT) & (tree->root == NULL))
	{
		//node will be the root
		node->parent = NULL;
		//root now points to node, our 'new' root
		tree->root = node;
	}else
	{
		//are we able to create a leaf for the current node to the left
		if ((where == TOLEFT) & (tree->current->left == NULL))
		{
			//the leaf's parent is the current node
			node->parent = tree->current;
			//the current node's left child is the node leaf
			tree->current->left = node;
		}else
		{
			//are we able to create a leaf for the current node to the right
			if ((where == TORIGHT) & (tree->current->right == NULL))
			{
				//the leaf's parent is the current node
				node->parent = tree->current;
				//the current node's right child is the node leaf
				tree->current->right = node;
			}else
			{
				//free the memory allocated and return a error
				free(node);
				return -1;
			}
		}
	}
	//the node leaf is now the current node the tree points to
	tree->current = node;
	//the size of the tree is bigger
	++(tree->size);
	//the operations was successful
	return 1;
}

int traverseTree(binaryTreePtr tree, void (*traverseProc)())
/* Call "traverseProc" for each node using in-order traversal.
 * Pre-Condition: tree exists.
 * Post-Condition: all nodes have been visited and traverseProc applied to them,
 * else error is returned.
*/
{
	if (tree == NULL)
	{
		return -1;
	}

	binaryTreePtr tempTree = malloc(sizeof(struct treeHeader));

	//did the memory allocate
	if (tempTree == NULL)
	{
		return -1;
	}else
	{
		//set all the temp tree pointers to those of tree
		tempTree->root = tree->root;
		tempTree->size = tree->size;

		//set the pointers of the temp tree to the unique tree functions
		tempTree->deleteElement = tree->deleteElement;
		tempTree->dspElement = tree->dspElement;
		tempTree->cmpElements = tree->cmpElements;
	}


	if (existNode(tree, TOLEFT))
	{
		tempTree->current = tree->current->left;
		traverseTree(tempTree, traverseProc );
	}

	traverseProc();

	if (existNode(tree, TORIGHT))
	{
		tempTree->current = tree->current->right;
		traverseTree(tempTree, traverseProc);
	}

	free(tempTree);
}

int searchTree(binaryTreePtr tree, void *element)
/* Search for element in the tree.
 * Pre-Condition: tree and element exist.
 * Post-Condition: if element occurs in tree then the current node is set to the
 * first occurrence found in order otherwise error is returned and current node
 * is not changed
 */
{
	if ((tree == NULL) || (element == NULL))
	{
		return -1;
	}

	binaryTreePtr tempTree = NULL;
	tempTree = malloc(sizeof(struct treeHeader));

	//did the memory allocate
	if (tempTree == NULL)
	{
		return -1;
	}else
	{
		//set all the temp tree pointers to those of tree
		tempTree->root = tree->root;
		tempTree->size = tree->size;

		//set the pointers of the temp tree to the unique tree functions
		tempTree->deleteElement = tree->deleteElement;
		tempTree->dspElement = tree->dspElement;
		tempTree->cmpElements = tree->cmpElements;
	}

	//traverse left
	if (existNode(tree, TOLEFT))
	{
		tempTree->current = tree->current->left;
		if (searchTree(tempTree, element) == 0)
		{
			tree->current = tempTree->current;
			return 0;
		}
	}

	treeNodePtr node = retrieveNodeElm(tree);
	if (tree->cmpElements(element,node) == 0)
	{
		return 0;
	}


	if (existNode(tree, TORIGHT))
	{
		tempTree->current = tree->current->right;
		if (searchTree(tempTree, element) == 0)
		{
			tree->current = tempTree->current;
			return 0;
		}
	}

	free(tempTree);

}

int nextNode(binaryTreePtr tree, enum position where)
/* Advance current node in given direction.
 * Pre-Condition: tree and current node exist.
 * Post-Condition: where == TOROOT then current node is tree root
 *                  where == TOLEFT then current node is left child
 *                  where == TORIGHT then current node is right child
 *                  otherwise there is no change and error is returned.
 */
{
	//does the tree exist and does it contain nodes?
	if ((tree == NULL) || (tree->current == NULL))
	{
		return -1;
	}
	//where do you want to go today?
	switch (where)
	{
	case(TOROOT):
		//does the root exist?
		if (tree->root == NULL)
		{
			return -1;
		}else
		{
			//make the root the current node and return success
			tree->current = tree->root;
			return 1;
		}
		break;
	case(TOLEFT):
		//does it have a left child
		if (tree->current->left == NULL)
		{
			return -1;
		}else
		{
			//make the left child the current node and return success
			tree->current = tree->current->left;
			return 1;
		}
		break;
	case(TORIGHT):
		//does it have a right child
		if (tree->current->right == NULL)
		{
			return -1;
		}else
		{
			//make the right child the current node and return success
			tree->current = tree->current->right;
			return 1;
		}
		break;
	}
}

void displayTree(binaryTreePtr tree)
/* Print tree with indentations to show structure
 * Pre-Condition: tree exists
 * Post-Condition: tree is displayed vertically on
 *                 screen with elements indented
 */
{
	if (tree == NULL)
	{
		return;
	}

	binaryTreePtr tempTree = NULL;
	tempTree = malloc(sizeof(struct treeHeader));

	//did the memory allocate
	if (tempTree == NULL)
	{
		return;
	}else
	{
		//set all the temp tree pointers to those of tree
		tempTree->root = tree->root;
		tempTree->size = tree->size;

		//set the pointers of the temp tree to the unique tree functions
		tempTree->deleteElement = tree->deleteElement;
		tempTree->dspElement = tree->dspElement;
		tempTree->cmpElements = tree->cmpElements;
	}

	//traverse left
	if (existNode(tree, TOLEFT) == 1)
	{

		tempTree->current = tree->current->left;
		displayTree(tempTree);
	}

	//display the element
	tree->dspElement(tree->current->element);

	//traverse right
	if (existNode(tree, TORIGHT) == 1)
	{
		printf(" ");
		tempTree->current = tree->current->right;
		displayTree(tempTree);
	}
	printf("\n");
	free(tempTree);
}

int existNode(binaryTreePtr tree, enum position where)
/* Return TRUE if a node exists at position indicated.
 * Pre-Condition: tree exists
 * Post-Condition: return true if node exists where
                    otherwise return false
 */
{
	//does the tree exist and does it contain nodes?
	if ((tree == NULL) || (tree->current == NULL))
	{
		return -1;
	}
	//which way do we want to look?
	switch (where)
	{
	case(TOROOT):
		//does the root exist?
		if (tree->root == NULL)
		{
			return -1;
		}else
		{
			return 1;
		}
		break;
	case(TOLEFT):
		//does it have a left child
		if (tree->current->left == NULL)
		{
			return -1;
		}else
		{
			return 1;
		}
		break;
	case(TORIGHT):
		//does it have a right child
		if (tree->current->right == NULL)
		{
			return -1;
		}else
		{
			return 1;
		}
		break;
	}
}

void *retrieveNodeElm(binaryTreePtr tree)
/* Return the pointer to the element of current node
 * Pre-Condition: tree and current node exist
 * Post-Condition: pointer to element of current node is returned
 *                 else NULL is returned
 */
{
	//does the tree exist and does it contain nodes?
	if ((tree == NULL) || (tree->current == NULL))
	{
		return NULL;
	}else
	{
		//does the element exist?
		if (tree->current->element != NULL)
		{
			//it exists, so we return it
			return tree->current->element;
		}else
		{
			//failure
			return NULL;
		}
	}
}

int storeNode(binaryTreePtr tree, void *elm)
/* Store elm as the new element of the current node
 * Pre-Condition: tree, current node and element exist
 * Post-Condition: current node element' = elm else
 *                 return error.
 */
{
	//does the tree exist, does the tree contain nodes, does elm exist?
	if ( ((tree == NULL) || (tree->current == NULL)) || (elm == NULL))
	{
		return -1;
	}else
	{
		//is the current node's element pointer empty
		if (tree->current->element == NULL)
		{
			//assign elm to element of current node
			tree->current->element = elm;
		}else
		{
			//remove current element of node and store new one
			tree->deleteElement;
			tree->current->element = elm;
		}
		return 1;
	}
}

int treeSize(binaryTreePtr tree)
/* Return number of nodes in tree
 * Pre-Condition: tree exists
 * Post-Condition: number of nodes in tree is returned,
 *                 else return error.
 */
{
	if (tree == NULL)
	{
		return -1;
	}else
	{
		return tree->size;
	}
}

treeNodePtr getCurrentNode(binaryTreePtr tree)
/* Return pointer to tree current node
 * Pre-Condition: tree exists
 * Post-Condition: pointer to current node is returned, else
 *                 return NULL.
 */
{
	if ((tree == NULL) || (tree->current == NULL))
	{
		return NULL;
	}else
	{
		return tree->current;
	}
}

int setCurrentNode(binaryTreePtr tree, treeNodePtr node)
/* Reset tree current node to node
 * Pre-Condition: tree exists, node exists in tree
 * Post-Condition: current node is set to node, else error
 *                 is returned.
 */
{
	if ( ((tree == NULL) || (tree->current == NULL)) || (node == NULL))
	{
		return 0;
	}else
	{
		//if (searchTree(tree, node->element) == 0)
		//printf("nog nie klaar");
		tree->current = node;
		return 1;
	}
}
