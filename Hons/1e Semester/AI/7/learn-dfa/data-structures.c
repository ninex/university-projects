
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "data-structures.h"



#ifndef dotimes
#define dotimes(var,lim) for (var=0;var<(lim);var++) 
#endif



/************************************************************/


int total_allocated = 0;
int total_freed = 0;

void my_malloc_report ()
{
  fprintf(stderr, "%d bytes allocated, %d freed, %d unfreed\n",
	  total_allocated, total_freed,
	  total_allocated-total_freed);
}


inline void * my_malloc (int nbytes)
{
  total_allocated += nbytes;
  return malloc(nbytes);
}
inline void my_free (void * ptr, int nbytes)
{
  total_freed += nbytes;
  free (ptr);
}




/************************************************************/

inline II_Pair * make_ii_pair (IMM i1, IMM i2)
{
  II_Pair *new = (II_Pair *) my_malloc (sizeof(II_Pair));
  new->i1 = i1;
  new->i2 = i2;
  return new;
}


inline IP_Pair * make_ip_pair (IMM i1, void * p2)
{
  IP_Pair *new = (IP_Pair *) my_malloc (sizeof(IP_Pair));
  new->i1 = i1;
  new->p2 = p2;
  return new;
}


inline PP_Pair * make_pp_pair (void * p1, void * p2)
{
  PP_Pair *new = (PP_Pair *) my_malloc (sizeof(PP_Pair));
  new->p1 = p1;
  new->p2 = p2;
  return new;
}



/************************************************************/



inline IP_Pair * ilist_cons (IMM val, IP_Pair *inlist)
{ 
  IP_Pair *newpair; 
  newpair = (IP_Pair *) my_malloc (sizeof(IP_Pair)); 
  ICAR(newpair) = val;
  CDR(newpair) = inlist;
  return(newpair); 
} 
 

void ilist_dispose (IP_Pair *inlist)
{ 
  IP_Pair *the_rest; 
  if (inlist == NULL) return; 
  the_rest = CDR(inlist);
  my_free(inlist, sizeof(IP_Pair)); 
  ilist_dispose(the_rest); 
} 

 
int ilist_length (IP_Pair *inlist)
{ 
  IP_Pair *idpt = inlist; 
  int the_length = 0; 
  while (idpt != NULL) { 
    the_length += 1; 
    idpt = CDR(idpt); 
  } 
  return(the_length); 
} 
 

IMM ilist_nth (IP_Pair *inlist, int n)
{ 
  IP_Pair *idpt = inlist; 
  int my_counter = n; 
  while (my_counter > 0) { 
    if (idpt == NULL) exit(-1); 
    idpt = CDR(idpt);
    my_counter -= 1; 
  } 
  if (idpt == NULL) exit(-1); 
  return(ICAR(idpt));
} 
 

IP_Pair * ilist_reverse_aux (IP_Pair *the_list, IP_Pair *new_list)
{ 
  if (the_list == NULL) return new_list; 
  else return
	 (ilist_reverse_aux 
	  (CDR(the_list),
	   ilist_cons (ICAR(the_list), new_list))); 
} 
 
 
IP_Pair * ilist_reverse (IP_Pair *inlist)
{ 
  return(ilist_reverse_aux (inlist, (IP_Pair *) NULL)); 
} 


/***********************/


inline PP_Pair * plist_cons (void * val, PP_Pair *inlist)
{ 
  PP_Pair *newpair; 
  newpair = (PP_Pair *) my_malloc (sizeof(PP_Pair)); 
  PCAR(newpair) = val;
  CDR(newpair) = inlist;
  return(newpair); 
} 
 

void plist_dispose (PP_Pair *inlist)
{ 
  PP_Pair *the_rest; 
  if (inlist == NULL) return; 
  the_rest = CDR(inlist);
  my_free(inlist, sizeof(PP_Pair)); 
  plist_dispose(the_rest); 
} 

 
int plist_length (PP_Pair *inlist)
{ 
  PP_Pair *idpt = inlist; 
  int the_length = 0; 
  while (idpt != NULL) { 
    the_length += 1; 
    idpt = CDR(idpt); 
  } 
  return(the_length); 
} 
 

void * plist_nth (PP_Pair *inlist, int n)
{ 
  PP_Pair *idpt = inlist; 
  int my_counter = n; 
  while (my_counter > 0) { 
    if (idpt == NULL) exit(-1); 
    idpt = CDR(idpt);
    my_counter -= 1; 
  } 
  if (idpt == NULL) exit(-1); 
  return(PCAR(idpt));
} 
 

PP_Pair * plist_reverse_aux (PP_Pair *the_list, PP_Pair *new_list)
{ 
  if (the_list == NULL) return new_list; 
  else return
	 (plist_reverse_aux 
	  (CDR(the_list),
	   plist_cons (PCAR(the_list), new_list))); 
} 
 
 
PP_Pair * plist_reverse (PP_Pair *inlist)
{ 
  return(plist_reverse_aux (inlist, (PP_Pair *) NULL)); 
} 


/****************************************************************/


I_Stack * make_istack ()
{
  I_Stack *new = (I_Stack *) my_malloc (sizeof(I_Stack));
  new->l = NULL;
  return new;
}

boolean istack_empty_p (I_Stack * s)
{
  return (s->l == NULL);
}

void istack_push (I_Stack * s, IMM obj)
{
  s->l = ilist_cons (obj, s->l);
}

IMM istack_pop (I_Stack * s)
{
  IP_Pair * oldl = s->l;
  IMM val = ICAR(oldl);
  s->l   =   CDR(oldl);   /* deallocate old pair? */
  my_free(oldl,sizeof(IP_Pair));
  return val;
}


/***********************/


P_Stack * make_pstack ()
{
  P_Stack *new = (P_Stack *) my_malloc (sizeof(P_Stack));
  new->l = NULL;
  return new;
}

boolean pstack_empty_p (P_Stack * s)
{
  return (s->l == NULL);
}

inline void pstack_push (P_Stack * s, void * obj)
{
  s->l = plist_cons (obj, s->l);
}

void * pstack_pop (P_Stack * s)
{
  PP_Pair * oldl = s->l;
  void * val = PCAR(oldl);
  s->l       =  CDR(oldl);   /* deallocate old pair? */
  my_free(oldl,sizeof(PP_Pair));
  return val;
}



/****************************************************************/


I_Queue * make_iqueue ()
{
  I_Queue *new = (I_Queue *) my_malloc (sizeof(I_Queue));
  new->l = NULL;
  return new;
}

boolean iqueue_empty_p (I_Queue * s)
{
  return (s->l == NULL);
}

void iqueue_push (I_Queue * s, IMM obj)
{
  IP_Pair * lastp, * newp;
  if (s->l == NULL) s->l = ilist_cons (obj, NULL);
  else {
    lastp = s->l;
    while (CDR(lastp) != NULL) lastp = CDR(lastp);
    newp = ilist_cons (obj, NULL);
    CDR(lastp) = newp;
  }
}

IMM iqueue_pop (I_Queue * s)
{
  IP_Pair * oldl = s->l;
  IMM val = ICAR(oldl);
  s->l   =   CDR(oldl);   /* deallocate old pair? */
  my_free(oldl,sizeof(IP_Pair));
  return val;
}


IP_Pair * iqueue_contents (I_Queue * s)
{
  return s->l;
}


PP_Pair * pqueue_contents (P_Queue * s)
{
  return s->l;
}


IP_Pair * istack_contents (I_Stack * s)
{
  return s->l;
}


PP_Pair * pstack_contents (P_Stack * s)
{
  return s->l;
}


/****************************/


P_Queue * make_pqueue ()
{
  P_Queue *new = (P_Queue *) my_malloc (sizeof(P_Queue));
  new->l = NULL;
  return new;
}

boolean pqueue_empty_p (P_Queue * s)
{
  return (s->l == NULL);
}

void pqueue_push (P_Queue * s, void * obj)
{
  PP_Pair * lastp, * newp;
  if (s->l == NULL) s->l = plist_cons (obj, NULL);
  else {
    lastp = s->l;
    while (CDR(lastp) != NULL) lastp = CDR(lastp);
    newp = plist_cons (obj, NULL);
    CDR(lastp) = newp;
  }
}

void * pqueue_pop (P_Queue * s)
{
  PP_Pair * oldl = s->l;
  void * val = PCAR(oldl);
  s->l       =  CDR(oldl);   /* deallocate old pair? */
  my_free(oldl,sizeof(PP_Pair));
  return val;
}




void deallocate_pqueue (P_Queue * pq)
{
  plist_dispose (pq->l);
  my_free(pq, sizeof(P_Queue));
}




/****************************************************************/





PP_Pair ** make_vector_of_alists (int n)
{
  int i;
  PP_Pair ** new = (PP_Pair **) my_malloc (n * sizeof(PP_Pair *));
  dotimes (i, n) new[i] = NULL;
  return new;
}

#define INITIAL_N_ALISTS 1
#define EXPAND_TABLE_BY  10


IP_Pair * passq (IMM the_key, PP_Pair * inlist)
{
  if (inlist == NULL) return NULL;
  else {
    IP_Pair * the_car = (IP_Pair *) PCAR(inlist);
    if (KEY(the_car) == the_key)
      return (the_car);
    else return (passq (the_key, CDR(inlist)));
  }
}


II_Pair * iassq (IMM the_key, PP_Pair * inlist)
{
  if (inlist == NULL) return NULL;
  else {
    II_Pair * the_car = (II_Pair *) PCAR(inlist);
    if (KEY(the_car) == the_key)
      return (the_car);
    else return (iassq (the_key, CDR(inlist)));
  }
}




/****************************************************************/







void itable_expand (I_Table * table)
{
  int old_n_alists = table->n_alists;
  int new_n_alists = old_n_alists * EXPAND_TABLE_BY;
  PP_Pair ** old_alists = table->alists;
  PP_Pair ** new_alists = make_vector_of_alists (new_n_alists);
  int i;

  dotimes (i, old_n_alists) {
    PP_Pair * al = old_alists [i];
    while (al != NULL) {
      PP_Pair * arest = CDR(al);
      II_Pair * a    = PCAR(al);
      int new_hash = ICAR(a) % new_n_alists;
      new_alists[new_hash] = plist_cons (a, new_alists[new_hash]);
      my_free(al, sizeof(PP_Pair));
      al = arest;
    }
  }
  my_free(old_alists, old_n_alists * sizeof(PP_Pair *));
  table->n_alists = new_n_alists;
  table->alists = new_alists;
}





I_Table * make_itable ()
{
  I_Table *new = (I_Table *) my_malloc (sizeof(I_Table));
  new->count = 0;
  new->n_alists = INITIAL_N_ALISTS;
  new->alists = make_vector_of_alists (INITIAL_N_ALISTS);
  return new;
}


boolean itable_test (I_Table * table, int key)
{
  int hash = key % table->n_alists;
  II_Pair * probe = iassq (key, table->alists[hash]);
  return (probe != NULL);
}


IMM itable_lookup (I_Table * table, int key)
{
  int hash = key % table->n_alists;
  II_Pair * probe = iassq (key, table->alists[hash]);
  assert(probe != NULL);
  return(IVAL(probe));
}



void itable_set (I_Table * table, int key, IMM val)
{
  int hash = key % table->n_alists;
  II_Pair * probe = iassq (key, table->alists[hash]);
  if (probe != NULL) IVAL(probe) = val;
  else {
    II_Pair * new = make_ii_pair (key, val);
    table->alists[hash] = plist_cons (new, table->alists[hash]);
    table->count += 1;
    if (table->count > EXPAND_TABLE_BY * table->n_alists)
      itable_expand (table);
  }
}


int itable_count (I_Table * table)
{
  return table->count;
}



void walk_itable (void f(int a, IMM b), I_Table * table)
{
  int i;
  dotimes (i, table->n_alists) {
    PP_Pair * al = table->alists[i];
    while (al != NULL) {
      II_Pair * x = PCAR(al);
      f (KEY(x), IVAL(x));
      al = CDR(al);
    }
  }
}     




void deallocate_itable (I_Table * table)
{
  int i;
  dotimes (i, table->n_alists) {
    PP_Pair * the_list = table->alists[i];
    while (the_list != NULL) {
      PP_Pair * the_rest =  CDR(the_list);
      II_Pair * assoc    = PCAR(the_list);
      my_free(the_list, sizeof(PP_Pair));
      my_free(assoc, sizeof(II_Pair));
      the_list = the_rest;
    }
  }
  my_free(table->alists, table->n_alists * sizeof(PP_Pair *));
  my_free(table, sizeof(I_Table));
}








/****************************************************************/





void ptable_expand (P_Table * table)
{
  int old_n_alists = table->n_alists;
  int new_n_alists = old_n_alists * EXPAND_TABLE_BY;
  PP_Pair ** old_alists = table->alists;
  PP_Pair ** new_alists = make_vector_of_alists (new_n_alists);
  int i;

  dotimes (i, old_n_alists) {
    PP_Pair * al = old_alists [i];
    while (al != NULL) {
      PP_Pair * arest = CDR(al);
      IP_Pair * a    = PCAR(al);
      int new_hash = ICAR(a) % new_n_alists;
      new_alists[new_hash] = plist_cons (a, new_alists[new_hash]);
      my_free(al, sizeof(PP_Pair));
      al = arest;
    }
  }
  my_free(old_alists, old_n_alists * sizeof(PP_Pair *));
  table->n_alists = new_n_alists;
  table->alists = new_alists;
}



P_Table * make_ptable ()
{
  P_Table *new = (P_Table *) my_malloc (sizeof(P_Table));
  new->count = 0;
  new->n_alists = INITIAL_N_ALISTS;
  new->alists = make_vector_of_alists (INITIAL_N_ALISTS);
  return new;
}


boolean ptable_test (P_Table * table, int key)
{
  int hash = key % table->n_alists;
  IP_Pair * probe = passq (key, table->alists[hash]);
  return (probe != NULL);
}


void * ptable_lookup (P_Table * table, int key)
{
  int hash = key % table->n_alists;
  IP_Pair * probe = passq (key, table->alists[hash]);
  assert(probe != NULL);
  return(PVAL(probe));
}



void ptable_set (P_Table * table, int key, void * val)
{
  int hash = key % table->n_alists;
  IP_Pair * probe = passq (key, table->alists[hash]);
  if (probe != NULL) PVAL(probe) = val;
  else {
    IP_Pair * new = make_ip_pair (key, val);
    table->alists[hash] = plist_cons (new, table->alists[hash]);
    table->count += 1;
    if (table->count > EXPAND_TABLE_BY * table->n_alists)
      ptable_expand (table);
  }
}


int ptable_count (P_Table * table)
{
  return table->count;
}



void walk_ptable (void f(int a, void * b), P_Table * table)
{
  int i;
  dotimes (i, table->n_alists) {
    PP_Pair * al = table->alists[i];
    while (al != NULL) {
      IP_Pair * x = PCAR(al);
      f (KEY(x), PVAL(x));
      al = CDR(al);
    }
  }
}     


/* Note: this doesn't free the objects that are stored in the table */


void deallocate_ptable (P_Table * table)
{
  int i;
  dotimes (i, table->n_alists) {
    PP_Pair * the_list = table->alists[i];
    while (the_list != NULL) {
      PP_Pair * the_rest =  CDR(the_list);
      IP_Pair * assoc    = PCAR(the_list);
      my_free(the_list, sizeof(PP_Pair));
      my_free(assoc, sizeof(IP_Pair));
      the_list = the_rest;
    }
  }
  my_free(table->alists, table->n_alists * sizeof(PP_Pair *));
  my_free(table, sizeof(P_Table));
}




boolean itable_test_and_set (I_Table * table, int key)
{
  boolean try = itable_test (table, key);
  if (!try) itable_set (table, key, true);
  return try;
}





Boxed_float * make_boxed_float (float f)
{
  Boxed_float *new = (Boxed_float *) my_malloc (sizeof(Boxed_float));
  new->f = f;
  return new;
}



/****************************************************************/



void print_int_list(IP_Pair *inlist)
{
  IP_Pair *idpt = inlist;
  printf("(");
  while (idpt != NULL) {
    printf("%d ", ICAR(idpt));
    idpt = CDR(idpt);
  }
  printf(")\n");
}




void print_boxed_float_list(PP_Pair *inlist)
{
  PP_Pair *idpt = inlist;
  printf("(");
  while (idpt != NULL) {
    printf("%f ", ((Boxed_float *) PCAR(idpt))->f);
    idpt = CDR(idpt);
  }
  printf(")\n");
}







/****************************************************************/




#ifdef TEST_ILISTS


char charworkbuf [100];

void get_int_defaut(prompt, varble)
char *prompt;
int *varble;
{
  int response, matched;

  printf("%s [y for %d] ", prompt, *varble);
  scanf ("%s", charworkbuf);
  if (charworkbuf[0] == 'y') return;
  matched = sscanf(charworkbuf, "%d", &response);
  *varble = response;
}


main()
{
  IP_Pair *the_list = NULL;
  IMM newval;

  while (1) {
    printf("%d: ", ilist_length(the_list));
    print_int_list(the_list);
    print_int_list(ilist_reverse(the_list));
    get_int_defaut("next value: ", &newval);
    the_list = ilist_cons(newval, the_list);
  }
}
#endif





#ifdef TEST_STACKS_AND_QUEUES

void test_istacks ()
{
  int i,j,k;
  I_Stack * pig = make_istack ();
  printf("\ntesting istacks\n");
  printf("empty? %d\n", istack_empty_p(pig));
  istack_push(pig,1);
  print_int_list (pig->l);
  istack_push(pig,2);
  print_int_list (pig->l);
  istack_push(pig,3);
  print_int_list (pig->l);
  printf("empty? %d\n", istack_empty_p(pig));
  i = istack_pop(pig);
  j = istack_pop(pig);
  k = istack_pop(pig);
  print_int_list (pig->l);
  printf("asdf %d %d %d\n", i,j,k);
  printf("empty? %d\n", istack_empty_p(pig));
}

void test_pstacks ()
{
  float i,j,k;
  P_Stack * pig = make_pstack ();
  printf("\ntesting pstacks\n");
  printf("empty? %d\n", pstack_empty_p(pig));
  pstack_push(pig,make_boxed_float(1.0));
  print_boxed_float_list (pig->l);
  pstack_push(pig,make_boxed_float(2.0));
  print_boxed_float_list (pig->l);
  pstack_push(pig,make_boxed_float(3.0));
  print_boxed_float_list (pig->l);
  printf("empty? %d\n", pstack_empty_p(pig));
  i = ((Boxed_float *) pstack_pop(pig))->f;
  j = ((Boxed_float *) pstack_pop(pig))->f;
  k = ((Boxed_float *) pstack_pop(pig))->f;
  print_boxed_float_list (pig->l);
  printf("asdf %f %f %f\n", i,j,k);
  printf("empty? %d\n", pstack_empty_p(pig));
}


void test_iqueues ()
{
  int i,j,k;
  I_Queue * pig = make_iqueue ();
  printf("\ntesting iqueues\n");
  printf("empty? %d\n", iqueue_empty_p(pig));
  iqueue_push(pig,1);
  print_int_list (pig->l);
  iqueue_push(pig,2);
  print_int_list (pig->l);
  iqueue_push(pig,3);
  print_int_list (pig->l);
  printf("empty? %d\n", iqueue_empty_p(pig));
  i = iqueue_pop(pig);
  j = iqueue_pop(pig);
  k = iqueue_pop(pig);
  print_int_list (pig->l);
  printf("asdf %d %d %d\n", i,j,k);
  printf("empty? %d\n", iqueue_empty_p(pig));
}

void test_pqueues ()
{
  float i,j,k;
  P_Queue * pig = make_pqueue ();
  printf("\ntesting pqueues\n");
  printf("empty? %d\n", pqueue_empty_p(pig));
  pqueue_push(pig,make_boxed_float(1.0));
  print_boxed_float_list (pig->l);
  pqueue_push(pig,make_boxed_float(2.0));
  print_boxed_float_list (pig->l);
  pqueue_push(pig,make_boxed_float(3.0));
  print_boxed_float_list (pig->l);
  printf("empty? %d\n", pqueue_empty_p(pig));
  i = ((Boxed_float *) pqueue_pop(pig))->f;
  j = ((Boxed_float *) pqueue_pop(pig))->f;
  k = ((Boxed_float *) pqueue_pop(pig))->f;
  print_boxed_float_list (pig->l);
  printf("asdf %f %f %f\n", i,j,k);
  printf("empty? %d\n", pqueue_empty_p(pig));
}


int main()
{
  test_istacks ();
  test_pstacks ();
  test_iqueues ();
  test_pqueues ();
  return(0);
}




#endif






#ifdef TEST_ITABLES

void print2ints (int a, int b)
{
  printf("%d %d\n", a, b);
}


int main ()
{
  int j;
  int key, val;
  I_Table * pig = make_itable ();

  while (1) {
    walk_itable (print2ints, pig);
    printf("%d ", itable_count(pig));
    dotimes (j, 10) {
      boolean tt = itable_test(pig, j);
      printf (" :%d %d", j, tt);
      if (tt) {
	int v = itable_lookup (pig, j);
	printf("[%d]", v);
      }
    }
    printf("\n");
    printf("new key and value: ");
    scanf("%d %d", &key, &val);
    itable_set(pig, key, val);
  }
  return(0);
}


#endif



#ifdef TEST_PTABLES


void print_int_and_boxed_float (int a, void * bb)
{
  Boxed_float * b = bb;
  printf("%d %f\n", a, b->f);
}


int main ()
{
  int j;
  int key;
  float val;
  P_Table * pig = make_ptable ();

  while (1) {
    walk_ptable (print_int_and_boxed_float, pig);
    printf("%d(%d) ", ptable_count(pig), pig->n_alists);
    dotimes (j, 10) {
      boolean tt = ptable_test(pig,j);
      printf (" :%d %d", j, tt);
      if (tt) {
	Boxed_float * v = ptable_lookup (pig,j);
	printf("[%f]", v->f);
      }
    }
    printf("\n");
    printf("new key and value: ");
    scanf("%d %f", &key, &val);
    ptable_set(pig, key, make_boxed_float(val));
  }
  return(0);
}

#endif



