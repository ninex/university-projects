(* Array index out of bounds*)
begin
  integer array n[10];integer x;
  (*cases tested*)
  x := n[-1];
  x := n[-1+1];
  x := n[-1+1-1];
  x := n[10];
  x := n[(10+1)*2-11];
end.