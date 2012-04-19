;*****************************************************************************
;* CS252: Assembler implementation of external functions defined in          *
;*        'tut4.h'.                                                          *
;*****************************************************************************

global addf

;*****************************************************************************
;* void addf(float a, float b, float *x);                                    *
;*****************************************************************************
%define a [ebp+8]
%define b [ebp+12]
%define x [ebp+16]
addf:
  push  ebp
  mov   ebp, esp


  
  mov   esp, ebp
  pop   ebp
  ret
