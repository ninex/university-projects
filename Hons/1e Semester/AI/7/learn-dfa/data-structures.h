

#define IMM int



#ifndef boolean
#define boolean int
#define false 0
#define true 1
#endif


/************************************************/

inline void * my_malloc (int nbytes);
inline void my_free (void * ptr, int nbytes);
void my_malloc_report ();


/************************************************/


/* 
   Here are the three basic pair types.
   We have pairs of ints, pairs of pointers, and mixed pairs.   
   */

typedef struct ii_pair 
{ 
  IMM i1;
  IMM i2;
} II_Pair; 


typedef struct pp_pair 
{ 
  void * p1;
  void * p2;
} PP_Pair; 

typedef struct ip_pair 
{ 
  IMM i1;
  void * p2;
} IP_Pair; 




/************************************************/

extern II_Pair * make_ii_pair (IMM i1, IMM i2);
extern IP_Pair * make_ip_pair (IMM i1, void * p2);
extern PP_Pair * make_pp_pair (void * p1, void * p2);

/************************************************/


#define ICAR(p) (p->i1)
#define PCAR(p) (p->p1)
#define  CDR(p) (p->p2)


extern IP_Pair * ilist_cons    (IMM val, IP_Pair *inlist);
extern IP_Pair * ilist_reverse (IP_Pair *inlist);
extern IMM       ilist_nth     (IP_Pair *inlist, int n);
extern int       ilist_length  (IP_Pair *inlist);
extern void      ilist_dispose (IP_Pair *inlist);
 

 
extern PP_Pair * plist_cons    (void * val, PP_Pair *inlist);
extern PP_Pair * plist_reverse (PP_Pair *inlist);
extern void *    plist_nth     (PP_Pair *inlist, int n);
extern int       plist_length  (PP_Pair *inlist);
extern void      plist_dispose (PP_Pair *inlist);


/************************************************/


typedef struct i_stack
{
  IP_Pair * l;
} I_Stack;

extern I_Stack * make_istack    ();
extern boolean   istack_empty_p (I_Stack * s);
extern void      istack_push    (I_Stack * s, IMM obj);
extern IMM       istack_pop     (I_Stack * s);


typedef struct p_stack
{
  PP_Pair * l;
} P_Stack;

extern P_Stack * make_pstack    ();
extern boolean   pstack_empty_p (P_Stack * s);
extern void      pstack_push    (P_Stack * s, void * obj);
extern void *    pstack_pop     (P_Stack * s);

extern IP_Pair * istack_contents (I_Stack * s);
extern PP_Pair * pstack_contents (P_Stack * s);


/************************************************/


typedef struct i_queue
{
  IP_Pair * l;
} I_Queue;

extern I_Queue * make_iqueue    ();
extern boolean   iqueue_empty_p (I_Queue * s);
extern void      iqueue_push    (I_Queue * s, IMM obj);
extern IMM       iqueue_pop     (I_Queue * s);


typedef struct p_queue
{
  PP_Pair * l;
} P_Queue;

extern P_Queue * make_pqueue    ();
extern boolean   pqueue_empty_p (P_Queue * s);
extern void      pqueue_push    (P_Queue * s, void * obj);
extern void *    pqueue_pop     (P_Queue * s);

extern void deallocate_pqueue (P_Queue * pq);


extern IP_Pair * iqueue_contents (I_Queue * s);
extern PP_Pair * pqueue_contents (P_Queue * s);

/************************************************/


/* for handling associations in alists */

#define  KEY(x) (x->i1)
#define IVAL(x) (x->i2)
#define PVAL(x) (x->p2)



typedef struct i_table
{
  int count;
  int n_alists;
  PP_Pair ** alists;
} I_Table;



extern II_Pair * iassq (IMM the_key, PP_Pair * inlist);
extern I_Table * make_itable ();
extern void itable_set (I_Table * table, IMM key, IMM val);
extern boolean itable_test (I_Table * table, IMM key);
extern IMM itable_lookup (I_Table * table, IMM key);
extern int itable_count (I_Table * table);
extern void walk_itable (void f(IMM a, IMM b), I_Table * table);

extern void deallocate_itable (I_Table * itab);


boolean itable_test_and_set (I_Table * table, IMM key);


typedef struct p_table
{
  int count;
  int n_alists;
  PP_Pair ** alists;
} P_Table;


extern IP_Pair * passq (IMM the_key, PP_Pair * inlist);
extern P_Table * make_ptable ();
extern void ptable_set (P_Table * table, IMM key, void * val);
extern boolean ptable_test (P_Table * table, IMM key);
extern void * ptable_lookup (P_Table * table, IMM key);
extern int ptable_count (P_Table * table);
extern void walk_ptable (void f(IMM a, void * b), P_Table * table);

extern void deallocate_Ptable (P_Table * ptab);


/********************************************************/


/* just for laughs */

typedef struct boxed_float
{ 
  float f;
} Boxed_float;

extern Boxed_float * make_boxed_float (float f);


