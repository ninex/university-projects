begin (* test 1 (correct) *)
  const MAX = 2147483647;
  integer x, y; 
  
  (* this is a (* nested *) comment *)
  x := MAX; y := -(MAX + 1);
  x, y := y, x;
  write x; write y;
end.