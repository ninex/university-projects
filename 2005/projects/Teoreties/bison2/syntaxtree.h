typedef enum {StmtK, ExpK} NodeKind;
typedef enum {IfK, RepeatK, AssignK, ReadK, WriteK, ForK, WhileK} StmtKind;
typedef enum {OpK, ConstK, IdK} ExpKind;

typedef struct treeNode{
  struct treeNode * child[4];
  struct treeNode * sibling;
  NodeKind nodekind;
  union {StmtKind stmt; ExpKind exp;} kind;
  union { char * op;
          int val;
          char * name;} attr;
} TreeNode;
