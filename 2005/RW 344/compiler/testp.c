# 1 "parser.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "parser.c"
# 1 "/usr/include/stdio.h" 1 3 4
# 28 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 296 "/usr/include/features.h" 3 4
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 297 "/usr/include/features.h" 2 3 4
# 319 "/usr/include/features.h" 3 4
# 1 "/usr/include/gnu/stubs.h" 1 3 4
# 320 "/usr/include/features.h" 2 3 4
# 29 "/usr/include/stdio.h" 2 3 4





# 1 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stddef.h" 1 3 4
# 213 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stddef.h" 3 4
typedef unsigned int size_t;
# 35 "/usr/include/stdio.h" 2 3 4

# 1 "/usr/include/bits/types.h" 1 3 4
# 28 "/usr/include/bits/types.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 29 "/usr/include/bits/types.h" 2 3 4


# 1 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stddef.h" 1 3 4
# 32 "/usr/include/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;




__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;







__extension__ typedef long long int __quad_t;
__extension__ typedef unsigned long long int __u_quad_t;
# 129 "/usr/include/bits/types.h" 3 4
# 1 "/usr/include/bits/typesizes.h" 1 3 4
# 130 "/usr/include/bits/types.h" 2 3 4






__extension__ typedef unsigned long long int __dev_t;
__extension__ typedef unsigned int __uid_t;
__extension__ typedef unsigned int __gid_t;
__extension__ typedef unsigned long int __ino_t;
__extension__ typedef unsigned long long int __ino64_t;
__extension__ typedef unsigned int __mode_t;
__extension__ typedef unsigned int __nlink_t;
__extension__ typedef long int __off_t;
__extension__ typedef long long int __off64_t;
__extension__ typedef int __pid_t;
__extension__ typedef struct { int __val[2]; } __fsid_t;
__extension__ typedef long int __clock_t;
__extension__ typedef unsigned long int __rlim_t;
__extension__ typedef unsigned long long int __rlim64_t;
__extension__ typedef unsigned int __id_t;
__extension__ typedef long int __time_t;
__extension__ typedef unsigned int __useconds_t;
__extension__ typedef long int __suseconds_t;

__extension__ typedef int __daddr_t;
__extension__ typedef long int __swblk_t;
__extension__ typedef int __key_t;


__extension__ typedef int __clockid_t;


__extension__ typedef int __timer_t;


__extension__ typedef long int __blksize_t;




__extension__ typedef long int __blkcnt_t;
__extension__ typedef long long int __blkcnt64_t;


__extension__ typedef unsigned long int __fsblkcnt_t;
__extension__ typedef unsigned long long int __fsblkcnt64_t;


__extension__ typedef unsigned long int __fsfilcnt_t;
__extension__ typedef unsigned long long int __fsfilcnt64_t;

__extension__ typedef int __ssize_t;



typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;


__extension__ typedef int __intptr_t;


__extension__ typedef unsigned int __socklen_t;
# 37 "/usr/include/stdio.h" 2 3 4









typedef struct _IO_FILE FILE;





# 62 "/usr/include/stdio.h" 3 4
typedef struct _IO_FILE __FILE;
# 72 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/libio.h" 1 3 4
# 32 "/usr/include/libio.h" 3 4
# 1 "/usr/include/_G_config.h" 1 3 4
# 14 "/usr/include/_G_config.h" 3 4
# 1 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stddef.h" 1 3 4
# 325 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stddef.h" 3 4
typedef long int wchar_t;
# 354 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stddef.h" 3 4
typedef unsigned int wint_t;
# 15 "/usr/include/_G_config.h" 2 3 4
# 24 "/usr/include/_G_config.h" 3 4
# 1 "/usr/include/wchar.h" 1 3 4
# 48 "/usr/include/wchar.h" 3 4
# 1 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stddef.h" 1 3 4
# 49 "/usr/include/wchar.h" 2 3 4

# 1 "/usr/include/bits/wchar.h" 1 3 4
# 51 "/usr/include/wchar.h" 2 3 4
# 76 "/usr/include/wchar.h" 3 4
typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;
# 25 "/usr/include/_G_config.h" 2 3 4

typedef struct
{
  __off_t __pos;
  __mbstate_t __state;
} _G_fpos_t;
typedef struct
{
  __off64_t __pos;
  __mbstate_t __state;
} _G_fpos64_t;
# 44 "/usr/include/_G_config.h" 3 4
# 1 "/usr/include/gconv.h" 1 3 4
# 28 "/usr/include/gconv.h" 3 4
# 1 "/usr/include/wchar.h" 1 3 4
# 48 "/usr/include/wchar.h" 3 4
# 1 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stddef.h" 1 3 4
# 49 "/usr/include/wchar.h" 2 3 4
# 29 "/usr/include/gconv.h" 2 3 4


# 1 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stddef.h" 1 3 4
# 32 "/usr/include/gconv.h" 2 3 4





enum
{
  __GCONV_OK = 0,
  __GCONV_NOCONV,
  __GCONV_NODB,
  __GCONV_NOMEM,

  __GCONV_EMPTY_INPUT,
  __GCONV_FULL_OUTPUT,
  __GCONV_ILLEGAL_INPUT,
  __GCONV_INCOMPLETE_INPUT,

  __GCONV_ILLEGAL_DESCRIPTOR,
  __GCONV_INTERNAL_ERROR
};



enum
{
  __GCONV_IS_LAST = 0x0001,
  __GCONV_IGNORE_ERRORS = 0x0002
};



struct __gconv_step;
struct __gconv_step_data;
struct __gconv_loaded_object;
struct __gconv_trans_data;



typedef int (*__gconv_fct) (struct __gconv_step *, struct __gconv_step_data *,
       __const unsigned char **, __const unsigned char *,
       unsigned char **, size_t *, int, int);


typedef wint_t (*__gconv_btowc_fct) (struct __gconv_step *, unsigned char);


typedef int (*__gconv_init_fct) (struct __gconv_step *);
typedef void (*__gconv_end_fct) (struct __gconv_step *);



typedef int (*__gconv_trans_fct) (struct __gconv_step *,
      struct __gconv_step_data *, void *,
      __const unsigned char *,
      __const unsigned char **,
      __const unsigned char *, unsigned char **,
      size_t *);


typedef int (*__gconv_trans_context_fct) (void *, __const unsigned char *,
       __const unsigned char *,
       unsigned char *, unsigned char *);


typedef int (*__gconv_trans_query_fct) (__const char *, __const char ***,
     size_t *);


typedef int (*__gconv_trans_init_fct) (void **, const char *);
typedef void (*__gconv_trans_end_fct) (void *);

struct __gconv_trans_data
{

  __gconv_trans_fct __trans_fct;
  __gconv_trans_context_fct __trans_context_fct;
  __gconv_trans_end_fct __trans_end_fct;
  void *__data;
  struct __gconv_trans_data *__next;
};



struct __gconv_step
{
  struct __gconv_loaded_object *__shlib_handle;
  __const char *__modname;

  int __counter;

  char *__from_name;
  char *__to_name;

  __gconv_fct __fct;
  __gconv_btowc_fct __btowc_fct;
  __gconv_init_fct __init_fct;
  __gconv_end_fct __end_fct;



  int __min_needed_from;
  int __max_needed_from;
  int __min_needed_to;
  int __max_needed_to;


  int __stateful;

  void *__data;
};



struct __gconv_step_data
{
  unsigned char *__outbuf;
  unsigned char *__outbufend;



  int __flags;



  int __invocation_counter;



  int __internal_use;

  __mbstate_t *__statep;
  __mbstate_t __state;



  struct __gconv_trans_data *__trans;
};



typedef struct __gconv_info
{
  size_t __nsteps;
  struct __gconv_step *__steps;
  __extension__ struct __gconv_step_data __data [];
} *__gconv_t;
# 45 "/usr/include/_G_config.h" 2 3 4
typedef union
{
  struct __gconv_info __cd;
  struct
  {
    struct __gconv_info __cd;
    struct __gconv_step_data __data;
  } __combined;
} _G_iconv_t;

typedef int _G_int16_t __attribute__ ((__mode__ (__HI__)));
typedef int _G_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int _G_uint16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int _G_uint32_t __attribute__ ((__mode__ (__SI__)));
# 33 "/usr/include/libio.h" 2 3 4
# 53 "/usr/include/libio.h" 3 4
# 1 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stdarg.h" 1 3 4
# 43 "/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.1/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 54 "/usr/include/libio.h" 2 3 4
# 163 "/usr/include/libio.h" 3 4
struct _IO_jump_t; struct _IO_FILE;
# 173 "/usr/include/libio.h" 3 4
typedef void _IO_lock_t;





struct _IO_marker {
  struct _IO_marker *_next;
  struct _IO_FILE *_sbuf;



  int _pos;
# 196 "/usr/include/libio.h" 3 4
};


enum __codecvt_result
{
  __codecvt_ok,
  __codecvt_partial,
  __codecvt_error,
  __codecvt_noconv
};
# 264 "/usr/include/libio.h" 3 4
struct _IO_FILE {
  int _flags;




  char* _IO_read_ptr;
  char* _IO_read_end;
  char* _IO_read_base;
  char* _IO_write_base;
  char* _IO_write_ptr;
  char* _IO_write_end;
  char* _IO_buf_base;
  char* _IO_buf_end;

  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;



  int _flags2;

  __off_t _old_offset;



  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];



  _IO_lock_t *_lock;
# 312 "/usr/include/libio.h" 3 4
  __off64_t _offset;





  void *__pad1;
  void *__pad2;

  int _mode;

  char _unused2[15 * sizeof (int) - 2 * sizeof (void *)];

};


typedef struct _IO_FILE _IO_FILE;


struct _IO_FILE_plus;

extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;
# 351 "/usr/include/libio.h" 3 4
typedef __ssize_t __io_read_fn (void *__cookie, char *__buf, size_t __nbytes);







typedef __ssize_t __io_write_fn (void *__cookie, __const char *__buf,
     size_t __n);







typedef int __io_seek_fn (void *__cookie, __off64_t *__pos, int __w);


typedef int __io_close_fn (void *__cookie);
# 403 "/usr/include/libio.h" 3 4
extern int __underflow (_IO_FILE *) ;
extern int __uflow (_IO_FILE *) ;
extern int __overflow (_IO_FILE *, int) ;
extern wint_t __wunderflow (_IO_FILE *) ;
extern wint_t __wuflow (_IO_FILE *) ;
extern wint_t __woverflow (_IO_FILE *, wint_t) ;
# 441 "/usr/include/libio.h" 3 4
extern int _IO_getc (_IO_FILE *__fp) ;
extern int _IO_putc (int __c, _IO_FILE *__fp) ;
extern int _IO_feof (_IO_FILE *__fp) ;
extern int _IO_ferror (_IO_FILE *__fp) ;

extern int _IO_peekc_locked (_IO_FILE *__fp) ;





extern void _IO_flockfile (_IO_FILE *) ;
extern void _IO_funlockfile (_IO_FILE *) ;
extern int _IO_ftrylockfile (_IO_FILE *) ;
# 471 "/usr/include/libio.h" 3 4
extern int _IO_vfscanf (_IO_FILE * __restrict, const char * __restrict,
   __gnuc_va_list, int *__restrict) ;
extern int _IO_vfprintf (_IO_FILE *__restrict, const char *__restrict,
    __gnuc_va_list) ;
extern __ssize_t _IO_padn (_IO_FILE *, int, __ssize_t) ;
extern size_t _IO_sgetn (_IO_FILE *, void *, size_t) ;

extern __off64_t _IO_seekoff (_IO_FILE *, __off64_t, int, int) ;
extern __off64_t _IO_seekpos (_IO_FILE *, __off64_t, int) ;

extern void _IO_free_backup_area (_IO_FILE *) ;
# 73 "/usr/include/stdio.h" 2 3 4
# 86 "/usr/include/stdio.h" 3 4


typedef _G_fpos_t fpos_t;




# 138 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/bits/stdio_lim.h" 1 3 4
# 139 "/usr/include/stdio.h" 2 3 4



extern struct _IO_FILE *stdin;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;







extern int remove (__const char *__filename) ;

extern int rename (__const char *__old, __const char *__new) ;









extern FILE *tmpfile (void);
# 178 "/usr/include/stdio.h" 3 4
extern char *tmpnam (char *__s) ;





extern char *tmpnam_r (char *__s) ;
# 196 "/usr/include/stdio.h" 3 4
extern char *tempnam (__const char *__dir, __const char *__pfx)
     __attribute__ ((__malloc__));








extern int fclose (FILE *__stream);




extern int fflush (FILE *__stream);

# 221 "/usr/include/stdio.h" 3 4
extern int fflush_unlocked (FILE *__stream);
# 235 "/usr/include/stdio.h" 3 4






extern FILE *fopen (__const char *__restrict __filename,
      __const char *__restrict __modes);




extern FILE *freopen (__const char *__restrict __filename,
        __const char *__restrict __modes,
        FILE *__restrict __stream);
# 262 "/usr/include/stdio.h" 3 4

# 273 "/usr/include/stdio.h" 3 4
extern FILE *fdopen (int __fd, __const char *__modes) ;
# 294 "/usr/include/stdio.h" 3 4



extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) ;



extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
      int __modes, size_t __n) ;





extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
         size_t __size) ;


extern void setlinebuf (FILE *__stream) ;








extern int fprintf (FILE *__restrict __stream,
      __const char *__restrict __format, ...);




extern int printf (__const char *__restrict __format, ...);

extern int sprintf (char *__restrict __s,
      __const char *__restrict __format, ...) ;





extern int vfprintf (FILE *__restrict __s, __const char *__restrict __format,
       __gnuc_va_list __arg);




extern int vprintf (__const char *__restrict __format, __gnuc_va_list __arg);

extern int vsprintf (char *__restrict __s, __const char *__restrict __format,
       __gnuc_va_list __arg) ;





extern int snprintf (char *__restrict __s, size_t __maxlen,
       __const char *__restrict __format, ...)
     __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
        __const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 3, 0)));

# 388 "/usr/include/stdio.h" 3 4





extern int fscanf (FILE *__restrict __stream,
     __const char *__restrict __format, ...);




extern int scanf (__const char *__restrict __format, ...);

extern int sscanf (__const char *__restrict __s,
     __const char *__restrict __format, ...) ;

# 430 "/usr/include/stdio.h" 3 4





extern int fgetc (FILE *__stream);
extern int getc (FILE *__stream);





extern int getchar (void);

# 454 "/usr/include/stdio.h" 3 4
extern int getc_unlocked (FILE *__stream);
extern int getchar_unlocked (void);
# 465 "/usr/include/stdio.h" 3 4
extern int fgetc_unlocked (FILE *__stream);











extern int fputc (int __c, FILE *__stream);
extern int putc (int __c, FILE *__stream);





extern int putchar (int __c);

# 498 "/usr/include/stdio.h" 3 4
extern int fputc_unlocked (int __c, FILE *__stream);







extern int putc_unlocked (int __c, FILE *__stream);
extern int putchar_unlocked (int __c);






extern int getw (FILE *__stream);


extern int putw (int __w, FILE *__stream);








extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream);






extern char *gets (char *__s);

# 578 "/usr/include/stdio.h" 3 4





extern int fputs (__const char *__restrict __s, FILE *__restrict __stream);





extern int puts (__const char *__s);






extern int ungetc (int __c, FILE *__stream);






extern size_t fread (void *__restrict __ptr, size_t __size,
       size_t __n, FILE *__restrict __stream);




extern size_t fwrite (__const void *__restrict __ptr, size_t __size,
        size_t __n, FILE *__restrict __s);

# 631 "/usr/include/stdio.h" 3 4
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
         size_t __n, FILE *__restrict __stream);
extern size_t fwrite_unlocked (__const void *__restrict __ptr, size_t __size,
          size_t __n, FILE *__restrict __stream);








extern int fseek (FILE *__stream, long int __off, int __whence);




extern long int ftell (FILE *__stream);




extern void rewind (FILE *__stream);

# 686 "/usr/include/stdio.h" 3 4






extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos);




extern int fsetpos (FILE *__stream, __const fpos_t *__pos);
# 709 "/usr/include/stdio.h" 3 4

# 718 "/usr/include/stdio.h" 3 4


extern void clearerr (FILE *__stream) ;

extern int feof (FILE *__stream) ;

extern int ferror (FILE *__stream) ;




extern void clearerr_unlocked (FILE *__stream) ;
extern int feof_unlocked (FILE *__stream) ;
extern int ferror_unlocked (FILE *__stream) ;








extern void perror (__const char *__s);






# 1 "/usr/include/bits/sys_errlist.h" 1 3 4
# 27 "/usr/include/bits/sys_errlist.h" 3 4
extern int sys_nerr;
extern __const char *__const sys_errlist[];
# 748 "/usr/include/stdio.h" 2 3 4




extern int fileno (FILE *__stream) ;




extern int fileno_unlocked (FILE *__stream) ;
# 767 "/usr/include/stdio.h" 3 4
extern FILE *popen (__const char *__command, __const char *__modes);





extern int pclose (FILE *__stream);





extern char *ctermid (char *__s) ;
# 807 "/usr/include/stdio.h" 3 4
extern void flockfile (FILE *__stream) ;



extern int ftrylockfile (FILE *__stream) ;


extern void funlockfile (FILE *__stream) ;
# 831 "/usr/include/stdio.h" 3 4

# 2 "parser.c" 2
# 1 "include/scanner.h" 1
# 11 "include/scanner.h"
enum symtype {
 sym_eof = -1,
 sym_array,
 sym_begin,
 sym_bool,
 sym_call,
 sym_const,
 sym_do,
 sym_end,
 sym_fi,
 sym_if,
 sym_int,
 sym_od,
 sym_proc,
 sym_read,
 sym_skip,
 sym_write,
 sym_id,
 sym_number,
 sym_comma,
 sym_semicolon,
 sym_colon,
 sym_equal,
 sym_leftblock,
 sym_rightblock,
 sym_arrow,
 sym_and,
 sym_or,
 sym_less,
 sym_assign,
 sym_greater,
 sym_plus,
 sym_minus,
 sym_guard,
 sym_mult,
 sym_div,
 sym_intdiv,
 sym_not,
 sym_leftbracket,
 sym_rightbracket,
 sym_stop,
 sym_eol,
 sym_space
};


typedef struct token_str{
 char name[32];
 enum symtype sym_type;
 unsigned int val;
} token_str;
# 99 "include/scanner.h"
token_str* getToken();


FILE* init(char* f);





void close();
# 3 "parser.c" 2
# 1 "include/parser.h" 1



int parse();

char* expect_err();

char* found();

int Program();


int Block();
int DefinitionPart();
int Definition();
int ConstantDefinition();
int VariableDefinition();
int TypeSymbol();
int VariableList();
int ProcedureDefinition();
int StatementPart();
int Statement();
int EmptyStatement();
int ReadStatement();
int VariableAccessList();
int WriteStatement();
int ExpressionList();
int AssignmentStatement();
int ProcedureStatement();
int IfStatement();
int DoStatement();
int GuardedCommandList();
int GuardedCommand();
int Expression();
int PrimaryOperator();
int PrimaryExpression();
int RelationalOperator();
int SimpleExpression();
int AddingOperator();
int Term();
int MultiplyingOperator();
int Factor();
int VariableAccess();
int IndexedSelector();
int Constant();
int Numeral();
int Name();
int ConstantName();
int VariableName();
int ProcedureName();
# 4 "parser.c" 2
# 1 "include/error.h" 1



void ERR_HANDLER(int error);

void set_count();

void inc_line();

void inc_col();

char* set_error(enum symtype sym_type);
# 5 "parser.c" 2
# 16 "parser.c"
static token_str* token;
static char last_expected[32],last_found[32];
static int need_priority, spacing, spacing_count;
# 27 "parser.c"
int parse(){
 set_count();

 token = getToken();

 if (token != ((void *)0)){

  need_priority = 1;


  spacing = 0;


  return Program();
 }else{

  return 0;
 }
}
# 56 "parser.c"
int expect(enum symtype sym_type,int priority){


 if (token->sym_type == sym_type){

  token = getToken();
  return 1;
 }else{


  if (priority && need_priority){

   sprintf(last_expected,"%s",set_error(sym_type));

   sprintf(last_found,"%s",set_error(token->sym_type));

   ERR_HANDLER(5);

   need_priority = 0;
  }
  return 0;
 }
}




char* expect_err(){
 return last_expected;
}




char* found(){
 return last_found;
}

int end_parse_print(char* func){
 printf("...exiting %s\n",func);
 spacing = spacing - 2;
 return 0;
}
# 116 "parser.c"
int Program(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (Block() && expect(sym_stop,1) && expect(sym_eof,1)) || end_parse_print(__FUNCTION__) ;
}







int Block(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_begin,1) && DefinitionPart() && StatementPart() && expect(sym_end,1)) || end_parse_print(__FUNCTION__);
}







int DefinitionPart(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (Definition()){
  if (expect(sym_semicolon,1)){


   return DefinitionPart() || end_parse_print(__FUNCTION__) ;
  }else{

   return 0 || end_parse_print(__FUNCTION__) ;
  }
 }else{

  return 1 || end_parse_print(__FUNCTION__) ;
 }
}





int Definition(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (ConstantDefinition() || VariableDefinition() || ProcedureDefinition()) || end_parse_print(__FUNCTION__) ;
}




int ConstantDefinition(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_const,0) && ConstantName() && expect(sym_equal,1) && Constant()) || end_parse_print(__FUNCTION__) ;
}






int VariableDefinition(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (TypeSymbol()){
  if (VariableList()){
   return 1 || end_parse_print(__FUNCTION__) ;
  }else{
   return (expect(sym_array,1) && VariableList() && expect(sym_leftblock,1) && Constant() && expect(sym_rightblock,1)) || end_parse_print(__FUNCTION__) ;
  }
 }else{
  return 0 || end_parse_print(__FUNCTION__) ;
 }
}






int TypeSymbol(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_int,0) || expect(sym_bool,0)) || end_parse_print(__FUNCTION__) ;
}





int VariableList(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (VariableName()){

  while (expect(sym_comma,0)){
   return VariableList() || end_parse_print(__FUNCTION__) ;
  }

  return 1 || end_parse_print(__FUNCTION__) ;
 }else{
  return 0 || end_parse_print(__FUNCTION__) ;
 }
}





int ProcedureDefinition(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_proc,0) && ProcedureName() && Block()) || end_parse_print(__FUNCTION__) ;
}




int StatementPart(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (Statement()){

  if (expect(sym_semicolon,1)){

   return StatementPart() || end_parse_print(__FUNCTION__) ;
  }else{
   return 0 || end_parse_print(__FUNCTION__) ;
  }
 }else{
  return 1 || end_parse_print(__FUNCTION__) ;
 }
}
# 255 "parser.c"
int Statement(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (EmptyStatement() ||
  ReadStatement() ||
  WriteStatement() ||
  AssignmentStatement() ||
  ProcedureStatement() ||
  IfStatement() ||
  DoStatement()) || end_parse_print(__FUNCTION__) ;
}



int EmptyStatement(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return expect(sym_skip,0) || end_parse_print(__FUNCTION__) ;
}



int ReadStatement(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_read,0) && VariableAccessList()) || end_parse_print(__FUNCTION__) ;
}





int VariableAccessList(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (VariableAccess()){

  if (expect(sym_comma,0)){
   return VariableAccessList() || end_parse_print(__FUNCTION__) ;
  }else{
   return 1 || end_parse_print(__FUNCTION__) ;
  }
 }else{
  return 0 || end_parse_print(__FUNCTION__) ;
 }
}




int WriteStatement(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_write,0) && ExpressionList()) || end_parse_print(__FUNCTION__) ;
}




int ExpressionList(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (Expression()){

  if (expect(sym_comma,0)){
   return ExpressionList() || end_parse_print(__FUNCTION__) ;
  }else{
   return 1 || end_parse_print(__FUNCTION__) ;
  }
 }else{
  return 0 || end_parse_print(__FUNCTION__) ;
 }
}




int AssignmentStatement(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (VariableAccessList() && expect(sym_assign,1) && ExpressionList()) || end_parse_print(__FUNCTION__) ;
}




int ProcedureStatement(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_call,0) && ProcedureName()) || end_parse_print(__FUNCTION__) ;
}




int IfStatement(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_if,0) && GuardedCommandList() && expect(sym_fi,1)) || end_parse_print(__FUNCTION__) ;
}




int DoStatement(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_do,0) && GuardedCommandList() && expect(sym_od,1)) || end_parse_print(__FUNCTION__) ;
}





int GuardedCommandList(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (GuardedCommand()){

  if (expect(sym_guard,0)){
   return GuardedCommandList() || end_parse_print(__FUNCTION__) ;
  }else{
   return 1 || end_parse_print(__FUNCTION__) ;
  }
 }else{
  return 0 || end_parse_print(__FUNCTION__) ;
 }
}






int GuardedCommand(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (Expression() && expect(sym_arrow,1) && StatementPart()) || end_parse_print(__FUNCTION__) ;
}




int Expression(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (PrimaryExpression()){
  if (PrimaryOperator()){
   return Expression() || end_parse_print(__FUNCTION__) ;
  }else{
   return 1 || end_parse_print(__FUNCTION__) ;
  }
 }else{
  return 0 || end_parse_print(__FUNCTION__) ;
 }
}





int PrimaryOperator(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_and,0) || expect(sym_or,0)) || end_parse_print(__FUNCTION__) ;
}





int PrimaryExpression(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (SimpleExpression()){
  if (RelationalOperator()){
   return SimpleExpression() || end_parse_print(__FUNCTION__) ;
  }else{
   return 1 || end_parse_print(__FUNCTION__) ;
  }
 }else{
  return 0 || end_parse_print(__FUNCTION__) ;
 }
}





int RelationalOperator(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_less,0) || expect(sym_equal,0) || expect(sym_greater,0)) || end_parse_print(__FUNCTION__) ;
}






int SimpleExpression(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;

 if (expect(sym_minus,0)){
  if (Term()){
   while (AddingOperator()){
    if (!(Term())){
     return 0 || end_parse_print(__FUNCTION__) ;
    }
   }
   return 1 || end_parse_print(__FUNCTION__) ;
  }else{
   return 0 || end_parse_print(__FUNCTION__) ;
  }
 }else{
  if (Term()){
   while (AddingOperator()){
    if (!(Term())){
     return 0 || end_parse_print(__FUNCTION__) ;
    }
   }
   return 1 || end_parse_print(__FUNCTION__) ;
  }else{
   return 0 || end_parse_print(__FUNCTION__) ;
  }
 }
}




int AddingOperator(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_plus,0) || expect(sym_minus,0)) || end_parse_print(__FUNCTION__) ;
}





int Term(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (Factor()){
  if (MultiplyingOperator()){
   return Term() || end_parse_print(__FUNCTION__) ;
  }else{
   return 1 || end_parse_print(__FUNCTION__) ;
  }
 }else{
  return 0 || end_parse_print(__FUNCTION__) ;
 }
}




int MultiplyingOperator(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_mult,0) || expect(sym_div,0) || expect(sym_intdiv,0)) || end_parse_print(__FUNCTION__) ;
}





int Factor(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (Constant() ||
  VariableAccess() ||
  (expect(sym_leftbracket,0) && Expression() && expect(sym_rightbracket,1)) ||
  (expect(sym_not,0) && Factor())) || end_parse_print(__FUNCTION__) ;
}





int VariableAccess(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 if (VariableName()){
  IndexedSelector();
  return 1 || end_parse_print(__FUNCTION__) ;
 }else{
  return 0 || end_parse_print(__FUNCTION__) ;
 }
}





int IndexedSelector(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_leftblock,0) && Expression() && expect(sym_rightblock,1)) || end_parse_print(__FUNCTION__) ;
}





int Constant(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (Numeral() || ConstantName()) || end_parse_print(__FUNCTION__) ;
}




int Numeral(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_number,0)) || end_parse_print(__FUNCTION__) ;
}




int Name(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 return (expect(sym_id,0)) || end_parse_print(__FUNCTION__) ;
}

int ConstantName(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 printf("*** Symboltable needed ***\n");
 return 0 || end_parse_print(__FUNCTION__) ;
}
int VariableName(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 printf("*** Symboltable needed ***\n");
 return 0 || end_parse_print(__FUNCTION__) ;
}
int ProcedureName(){
 spacing_count = 0; while (spacing_count < spacing){ spacing_count = spacing_count + 1; printf(" "); } printf("%s\n",__FUNCTION__); spacing = spacing + 2;
 printf("*** Symboltable needed ***\n");
 return 0 || end_parse_print(__FUNCTION__) ;
}