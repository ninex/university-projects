begin
  (* defect: illegal operator <= *)
  integer x;

  x := 0;
  do x <= 10 -> x := x + 1;
  od;
end.