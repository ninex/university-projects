
/* 
  4/6/98 fixed bug in build_tree_from_cform_file 
  which caused program to core dump when alphabet_size > 2
  */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/time.h>

#include "data-structures.h"



#ifndef dotimes
#define dotimes(var,lim) for (var=0;var<(lim);var++) 
#endif

#ifndef boolean
#define boolean int
#define false 0
#define true 1
#endif


int alphabet_size = 2;


typedef struct tree_node
{
  int gloid_slot;
  int state_label_slot;
  struct tree_node * father_slot;
  struct tree_node ** child_array_slot;
} treeNode;


#define NONDEF NULL

#define nondef_p(tn)    ((tn)==NONDEF)
#define gloid(tn)       ((tn)->gloid_slot)
#define state_label(tn) ((tn)->state_label_slot)
#define father(tn)      ((tn)->father_slot)
#define get_child(tn,i) ((tn)->child_array_slot[(i)])

#define set_gloid(tn,new)       ((tn)->gloid_slot=(new))
#define set_state_label(tn,new) ((tn)->state_label_slot=(new))
#define set_father(tn,new)      ((tn)->father_slot=(new))
#define set_child(tn,i,new)     ((tn)->child_array_slot[(i)]=(new))



void fancy_print_tree (treeNode * start_node);
/***************************************************************/


treeNode * make_duplicate_node (treeNode * told)
{
  int i;
  treeNode * tnew = (treeNode *) my_malloc (sizeof(treeNode));
  treeNode ** chnew =
    (treeNode **) my_malloc(alphabet_size * sizeof(treeNode *));
  treeNode ** chold = told->child_array_slot;

  tnew->child_array_slot = chnew;
  tnew->gloid_slot = told->gloid_slot;
  tnew->state_label_slot = told->state_label_slot;
  tnew->father_slot = told->father_slot;
  dotimes (i, alphabet_size) chnew[i] = chold[i];
  return tnew;  
}


void replace_node_contents (treeNode * t_to, treeNode * t_from)
{
  int i;
  treeNode ** ch_to   = t_to->child_array_slot;
  treeNode ** ch_from = t_from->child_array_slot;
  t_to->state_label_slot = t_from->state_label_slot;
  t_to->father_slot = t_from->father_slot;
  dotimes (i, alphabet_size) ch_to[i] = ch_from[i];
}


void backup (treeNode * guy, P_Table * backup_table) 
{
  if (! ptable_test (backup_table, gloid(guy))) {
    treeNode * backup = make_duplicate_node(guy);
    ptable_set (backup_table, gloid(guy), make_pp_pair (guy,backup));
  }
}



void replaceit (int ignore, void * p)
{
  PP_Pair * pp = p;
  replace_node_contents (pp->p1, pp->p2);
}


void revert_from_backups (P_Table * backup_table)
{
  walk_ptable (replaceit, backup_table);
}


/*********************************************************************/


int global_node_counter = 0;


treeNode * make_node (treeNode * father)
{
  int i;
  treeNode * tn = (treeNode *) my_malloc (sizeof(treeNode));
  treeNode ** ch = (treeNode **) my_malloc (alphabet_size*sizeof(treeNode *));

  tn->child_array_slot = ch;
  dotimes (i, alphabet_size) ch[i] = NONDEF;
  tn->gloid_slot = global_node_counter++;
  tn->state_label_slot = -1;
  tn->father_slot = father;
  return tn;
}

/*********************************************************************/

void depth_first_print (treeNode * curnode)
{
  int i;
  if (nondef_p (curnode)) printf("?");
  else {
    printf("[%d %d ", gloid (curnode), state_label (curnode));
    dotimes (i, alphabet_size)
      depth_first_print(get_child(curnode, i));
    printf("]");
  }
}


void augment_tree (treeNode * curnode, IP_Pair * str, int label)
{
  if (str == NULL)
    set_state_label(curnode,label);
  else {
    int cursym = ICAR(str);
    if (nondef_p(get_child(curnode,cursym)))
      set_child(curnode,cursym,make_node(curnode));
    augment_tree(get_child(curnode,cursym), CDR(str), label);
  }
}



void augment_tree_from_vec (treeNode * curnode,
			    int * str, int len, int pos,
			    int label)
{
  if (pos == len)
    set_state_label(curnode,label);
  else {
    int cursym = str[pos];
    if (nondef_p(get_child(curnode,cursym)))
      set_child(curnode,cursym,make_node(curnode));
    augment_tree_from_vec (get_child(curnode,cursym), str, len, pos+1, label);
  }
}



/*********************************************************************/


treeNode * old_build_tree_from_cform_file (char * name)
{
  FILE * infile = fopen (name, "r");
  int n_strings, i, j;
  treeNode * root;

  if (infile == NULL) {
    printf("file %s not found\n", name);
    exit(-1);
  }

  fscanf(infile, "%d", &n_strings);
  fscanf(infile, "%d", &alphabet_size);

  root = make_node (NONDEF);

  dotimes (i, n_strings) {
    int lab, strlen, itmp;
    IP_Pair * rb2 = NULL;
    IP_Pair * rb3;
    fscanf(infile, "%d", &lab);
    fscanf(infile, "%d", &strlen);
    dotimes (j, strlen) {
      fscanf(infile, "%d", &itmp);
      rb2 = ilist_cons(itmp, rb2);
    }
    rb3 = ilist_reverse (rb2);
    augment_tree (root, rb3, lab);
    ilist_dispose (rb2);
    ilist_dispose (rb3);  
  }
  fclose (infile);
  fprintf(stderr, "%d tree nodes allocated\n", global_node_counter);
  return root;
}




#define MAX_EXAMPLE_LEN 1000


treeNode * build_tree_from_cform_file (char * name)
{
  FILE * infile = fopen (name, "r");
  int n_strings, i, j;
  treeNode * root;

  int example [MAX_EXAMPLE_LEN];

  if (infile == NULL) {
    printf("file %s not found\n", name);
    exit(-1);
  }

  fscanf(infile, "%d", &n_strings);
  fscanf(infile, "%d", &alphabet_size);

  root = make_node (NONDEF);

  dotimes (i, n_strings) {
    int lab, len, itmp;
    fscanf(infile, "%d", &lab);
    fscanf(infile, "%d", &len);
    dotimes (j, len) {
      fscanf(infile, "%d", &itmp);
      example[j] = itmp;
    }
    augment_tree_from_vec (root, example, len, 0, lab);

  }
  fclose (infile);
  fprintf(stderr, "%d tree nodes allocated\n", global_node_counter);
  return root;
}







/*********************************************************************/



void print_tree (treeNode * start_node)
{
  I_Table * seen = make_itable ();
  P_Queue * todo = make_pqueue ();
  int i;

  pqueue_push (todo, (void *) start_node);

next:

  {
    treeNode * curnode = pqueue_pop(todo);
    if (!itable_test_and_set(seen,gloid(curnode))) {
      printf("%d %d ", gloid(curnode), state_label(curnode));
      dotimes (i, alphabet_size) {
	treeNode * x = get_child(curnode,i);
	if (nondef_p(x))
	  printf("? ");
	else {
	  pqueue_push(todo,x);
	  printf("%d ", gloid(x));
	}
      }
      printf("\n");
    }
    if (!pqueue_empty_p(todo)) goto next;
  }

  deallocate_itable (seen);
  deallocate_pqueue (todo);

}





void fancy_print_tree (treeNode * start_node)
{
  I_Table * seen = make_itable ();
  I_Table * id_table = make_itable ();
  P_Queue * todo = make_pqueue ();
  int count = 0;
  int i;

  pqueue_push (todo, (void *) start_node);

  while (!pqueue_empty_p(todo)) {
    treeNode * curnode = pqueue_pop(todo);
    if (!itable_test(id_table,gloid(curnode))) {
      itable_set (id_table, gloid(curnode), count++);
      dotimes (i, alphabet_size) {
	treeNode * x = get_child(curnode,i);
	if (!nondef_p(x)) pqueue_push(todo,x);
      }
    }
  }

  printf("%d %d\n", count, alphabet_size);

  pqueue_push (todo, (void *) start_node);

  while (!pqueue_empty_p(todo)) {
    treeNode * curnode = pqueue_pop(todo);
    if (!itable_test_and_set(seen,gloid(curnode))) {
      printf("%d %d ",
	     itable_lookup(id_table, gloid(curnode)),
	     state_label(curnode));
      dotimes (i, alphabet_size) {
	treeNode * x = get_child(curnode,i);
	if (nondef_p(x))
	  printf("? ");
	else {
	  pqueue_push(todo,x);
	  printf("%d ", itable_lookup(id_table,gloid(x)));
	}
      }
      printf("\n");
    }
  }

  deallocate_itable (seen);
  deallocate_itable (id_table);
  deallocate_pqueue (todo);

}



/*********************************************************************/






int find_incoming_token (treeNode * node, treeNode * father) 
{
  int i = 0;
  int n_matches = 0;
  int matching_i = -1;

  dotimes (i, alphabet_size) {
    if (get_child(father, i)==node) {
      n_matches += 1;
      matching_i = i;
    }
  }

  if (n_matches != 1) printf("nm=%d  mi=%d\n", n_matches, matching_i);
  assert (n_matches == 1);
  return (matching_i);
}


/*********************************************************************/






void deallocate_node (treeNode * nd)
{
  treeNode ** ch = nd->child_array_slot;
  my_free (ch, alphabet_size * sizeof(treeNode *));
  my_free (nd, sizeof(treeNode));
}






void deallocate_stack_of_pp (P_Stack * splices)
{
  PP_Pair * the_list = pstack_contents(splices);
  my_free(splices, sizeof(P_Stack));
  while (the_list != NULL) {
    PP_Pair * the_rest =  CDR(the_list);
    PP_Pair * pp = PCAR(the_list);
    my_free(pp, sizeof(PP_Pair));
    my_free(the_list, sizeof(PP_Pair));
    the_list = the_rest;
  }
}




void deallocate_backup_table (P_Table * backup_table)
{
  int i;
  dotimes (i, backup_table->n_alists) {
    PP_Pair * the_list = backup_table->alists[i];
    while (the_list != NULL) {
      PP_Pair * the_rest =  CDR(the_list);
      IP_Pair * assoc = PCAR(the_list);
      PP_Pair * pp = PVAL(assoc);
      treeNode * cpy = pp->p2;
      my_free(the_list, sizeof(PP_Pair));
      my_free(assoc, sizeof(IP_Pair));
      my_free(pp, sizeof(PP_Pair));
      deallocate_node(cpy);
      the_list = the_rest;
    }
  }
  my_free(backup_table->alists,
	  backup_table->n_alists * sizeof(PP_Pair *));
  my_free(backup_table, sizeof(P_Table));
}




/*************************************************/




inline boolean label_defined (int lab)
{
  return (! (-1 == lab));
}



/* The following 3 procedures merge a piece of the training tree
   into the hypothesis graph by destructively copying labels 
   and transitions from the tree into the graph.  We make 
   backup copies of the modified nodes so that we can undo the
   merge if a labeling conflict is detected later. 
*/


boolean compare_labels (treeNode * tr_node,
			treeNode * graph_node,
			P_Table * backup_table) 
{
  if (label_defined (state_label (tr_node))) {
    if (label_defined (state_label (graph_node)))
      return (state_label (graph_node)) == (state_label (tr_node));
    else {
      /* if the graph node is unlabeled, adopt the tree label */
      backup (graph_node, backup_table);
      set_state_label (graph_node, state_label (tr_node));
      return true;
    }
  }
  else return true;
}


void continue_merging (treeNode * tr_node,
		       treeNode * graph_node,
		       P_Stack * search_stack,
		       P_Table * backup_table,
		       P_Stack * splices) 
{
  int i;
  dotimes (i, alphabet_size) {
    treeNode * graph_child = get_child (graph_node, i);
    treeNode * tree_child = get_child (tr_node, i);
    if (!nondef_p(tree_child)) {
      if (!nondef_p(graph_child)) {
	pstack_push (search_stack, make_pp_pair (tree_child,graph_child));
      }
      else {
	/* If the tree has a branch that the graph lacks, splice it in */
	backup (graph_node, backup_table);
	backup (tree_child, backup_table);
	set_child (graph_node, i, tree_child);
	set_father (tree_child, graph_node);
	pstack_push (splices, make_pp_pair (graph_node,tree_child));
      }
    }
  }
}


boolean try_merging (treeNode * curnode,
		     treeNode * potential_match,
		     P_Table * backup_table,
		     P_Stack * splices) 
{
  P_Stack * search_stack = make_pstack();
  pstack_push (search_stack, make_pp_pair (curnode, potential_match));

  /* this loop performs a depth-first traversal without recursion */

loop:

  {
    PP_Pair * pp = pstack_pop (search_stack);
    treeNode * t = pp->p1;
    treeNode * g = pp->p2;
    boolean result = compare_labels (t, g, backup_table);
    my_free(pp, sizeof(PP_Pair));
    if (result) {
       continue_merging (t, g, search_stack, backup_table, splices);
       if (! pstack_empty_p (search_stack)) goto loop;
       else {
	 deallocate_stack_of_pp (search_stack);
	 return true;
       }
    }
    else {
      deallocate_stack_of_pp (search_stack);
      return false;
    }
  }
}




/*********************************************************************/


/* When a node is unmergeable, we make it a state in the hypothesis */


void accept_the_node (treeNode * curnode,
		      P_Queue  * todo,
		      I_Table  * unique_table,
		      P_Queue  * unique_nodes) 
{
  int i;
  pqueue_push (unique_nodes, curnode);
  itable_set  (unique_table, gloid(curnode), true);
  dotimes (i, alphabet_size) {
    treeNode * x = get_child (curnode, i);
    if (! nondef_p (x)) pqueue_push (todo, x);
  }
}






/*********************************************************************/


/* Here we schedule visits to children of previously accepted nodes */

void handle_the_splices (P_Queue * todo,
			 I_Table * unique_table,
			 P_Stack * splices)
{
  PP_Pair * the_list = pstack_contents(splices);
  while (the_list != NULL) {
    PP_Pair * pp = PCAR(the_list);
    treeNode * hdx = pp->p1;
    treeNode * tlx = pp->p2;
    if (itable_test(unique_table,gloid(hdx))) 
      pqueue_push (todo, tlx);
    the_list = CDR(the_list);
  }
}


/*********************************************************************/



/* This procedure is the outer loop that visits nodes in
   breadth first order deciding whether they are mergeable
   or should be incorporated into the hypothesis */

void consider_node (P_Queue  * todo,
		    I_Table  * unique_table,
		    P_Queue  * unique_nodes,treeNode*root) 
{
  treeNode * curnode = pqueue_pop (todo);
  treeNode * curdad =  father (curnode);
  int inchar = (nondef_p(curdad)) ? -1 : find_incoming_token(curnode,curdad);
  PP_Pair * potential_matches = pqueue_contents (unique_nodes);

  /* This inner loop tries to merge node with previously accepted states */

find_match:
  if (potential_matches == NULL) {
    accept_the_node (curnode, todo, unique_table, unique_nodes);fancy_print_tree(root);printf("\n");}
  else {
    treeNode * potential_match = PCAR(potential_matches);
    PP_Pair  * remaining_matches = CDR(potential_matches);
    P_Table  * backup_table = make_ptable();
    P_Stack  * splices = make_pstack();
    set_child (curdad, inchar, potential_match);

    if (try_merging (curnode,potential_match,backup_table,splices)) {
      /* merge succeeded */
      handle_the_splices (todo, unique_table, splices);
      deallocate_stack_of_pp (splices);
      deallocate_backup_table (backup_table);
fancy_print_tree(root);printf("\n");
    }
    else { /* merge failed, must undo side effects before proceeding */
      revert_from_backups (backup_table);
      set_child (curdad, inchar, curnode);
      deallocate_stack_of_pp (splices);
      deallocate_backup_table (backup_table);
      potential_matches = remaining_matches;
	fancy_print_tree(root);printf("\n");
      goto find_match;
    }
  }

  if (!pqueue_empty_p(todo))
    consider_node (todo, unique_table, unique_nodes,root);
}
   



void greedily_collapse (treeNode * root)
{
   P_Queue * todo = make_pqueue ();
   I_Table * unique_table = make_itable ();
   P_Queue * unique_nodes = make_pqueue ();

   pqueue_push   (todo, root);
   consider_node (todo, unique_table, unique_nodes,root);

   deallocate_pqueue (todo);
   deallocate_pqueue (unique_nodes);
   deallocate_itable (unique_table);
}
   
   







/*********************************************************************/




long initial_seconds;


void init_millisecond_timer ()
{
  struct timeval t_work;
  gettimeofday(&t_work, NULL);
  initial_seconds = t_work.tv_sec;
}


/* this gives a timestamp in units of milliseconds */
/* the actual time resolution may be lower */


int millisecond_timer ()
{
  struct timeval t_work;
  int n_seconds;
  gettimeofday(&t_work, NULL);
  n_seconds = t_work.tv_sec - initial_seconds;
  return(n_seconds * 1000 + t_work.tv_usec / 1000);
}






/*********************************************************************/



int main(int argc, char **argv)
{
  treeNode * pig;

  int t1, t2, t3, t4;

  init_millisecond_timer ();

  if (argc != 2) {
    printf("usage: %s filename\n", argv[0]);
    exit(-1);
  }

  t1 = millisecond_timer ();

  pig = build_tree_from_cform_file (argv[1]);
  t2 = millisecond_timer ();
fancy_print_tree(pig);printf("\n");
  greedily_collapse (pig);
  t3 = millisecond_timer ();

  fancy_print_tree (pig);
  t4 = millisecond_timer ();

  my_malloc_report();

  fprintf(stderr, "%d msec reading, %d msec crunching, %d msec writing\n",
	  t2-t1, t3-t2, t4-t3);

  return(0);

}
