
/*
This the header file for the binary tree ADT
*/

//=====================================================================================================

//This is the structure that each node in the tree will consist of
// all elements are selfexplanatory
typedef struct treeNode{
  void  *element;
  struct treeNode *parent;
  struct treeNode *left;
  struct treeNode *right;  
} *treeNodePtr;

/* This is the header that will contain all the info about our tree
 * It consists of two pointers to the root and the element currently being referenced
 * Then it has three pointers to custom functions that the user who calls this structure will define
 * last there is an integer value which represents the size of the tree
*/
typedef struct treeHeader{
  treeNodePtr root;
  treeNodePtr current;
  void (*deleteElement)(void *);
  void (*dspElement)(void *);
  int (*cmpElements)(void *, void *);
  int size;
} *binaryTreePtr;


//this is an enumerated type that will define in which direction we're looking at in the tree
enum position{
  TOROOT,
  TOLEFT,
  TORIGHT
};

//======================================================================================================

binaryTreePtr createTree(void (*deleteElement)(void *),
                         void (*dspElement)(void *),
                         int (*cmpElements)(void *, void *));
/* The first procedure to call, which allocates and initialises a tree.
 * Pre-condition: tree does not exists
 *                Valid pointers to functions deleteElement, dspElement and
 *                cmpElements are passed.
 * Post-condition: Empty tree exists or NULL returned.
*/

binaryTreePtr deleteTree(binaryTreePtr tree);
/* All information about the tree and the data records it contains are deleted.
 * Pre-condition: tree exists
 * Post-condition: tree does not exist
*/

int insertNode(binaryTreePtr tree, void *elem, enum position where);
/* Insert elem into the tree. The key is in the element.
 * Pre-condition: tree and elem exist. where == TOROOT and tree is empty,
 *                                     where == TOLEFT and no left child,
 *                                     where == TPRIGHT and no right child.
 * Post-condition: node is inserted in the tree or error returned.
*/

int deleteNode(binaryTreePtr tree);
/* Delete current node from the tree. This procedure should not be called during
 * a traverseTree.
 * Pre-Condition: tree and current node exists.
 * Post-Condition: current node is deleted or error is returned.
*/

int traverseTree(binaryTreePtr tree, void (*traverseProc)());
/* Call "traverseProc" for each node using in-order traversal.
 * Pre-Condition: tree exists.
 * Post-Condition: all nodes have been visited and traverseProc applied to them,
 * else error is returned.
*/

int searchTree(binaryTreePtr tree, void *element);
/* Search for element in the tree.
 * Pre-Condition: tree and element exist.
 * Post-Condition: if element occurs in tree then the current node is set to the
 * first occurrence found in order otherwise error is returned and current node
 * is not changed
 */

int nextNode(binaryTreePtr tree, enum position where);
/* Advance current node in given direction.
 * Pre-Condition: tree and current node exist.
 * Post-Condition: where == TOROOT then current node is tree root
 *                  where == TOLEFT then current node is left child
 *                  where == TORIGHT then current node is right child
 *                  otherwise there is no change and error is returned.
 */

void displayTree(binaryTreePtr tree);
/* Print tree with indentations to show structure
 * Pre-Condition: tree exists
 * Post-Condition: tree is displayed vertically on
 *                 screen with elements indented
 */

int existNode(binaryTreePtr tree, enum position where);
/* Return TRUE if a node exists at position indicated.
 * Pre-Condition: tree exists
 * Post-Condition: return true if node exists where
                    otherwise return false
 */

void *retrieveNodeElm(binaryTreePtr tree);
/* Return the pointer to the element of current node
 * Pre-Condition: tree and current node exist
 * Post-Condition: pointer to element of current node is returned
 *                 else NULL is returned
 */

int storeNode(binaryTreePtr tree, void *elm);
/* Store elm as the new element of the current node
 * Pre-Condition: tree, current node and element exist
 * Post-Condition: current node element' = elm else 
 *                 return error.
 */

int treeSize(binaryTreePtr tree);
/* Return number of nodes in tree
 * Pre-Condition: tree exists
 * Post-Condition: number of nodes in tree is returned,
 *                 else return error.
 */

treeNodePtr getCurrentNode(binaryTreePtr tree);
/* Return pointer to tree current node
 * Pre-Condition: tree exists
 * Post-Condition: pointer to current node is returned, else
 *                 return NULL.
 */

int setCurrentNode(binaryTreePtr tree, treeNodePtr node);
/* Reset tree current node to node
 * Pre-Condition: tree exists, node exists in tree 
 * Post-Condition: current node is set to node, else error
 *                 is returned.
 */
