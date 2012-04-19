(* Test arrays 
 Answer must be
 4
 2
 *)
begin
const n =10;
  
  integer array a[5];
 integer x;
  a[1]:=2;
  a[2]:=1;
  a[3] := 4;
  a[4] := a[2]/4+1*(12-8)+a[2]/a[2]-1;
  write a[4];
  x:= a[a[4]-a[3]+a[2]];
  write x;
  
end.

(*x:=1;
  a[2]:=1;
  a[3] := 4;
  a[1]:=6;
  a[4] := x+a[2];
  x:= a[3];
  write a[x];*)