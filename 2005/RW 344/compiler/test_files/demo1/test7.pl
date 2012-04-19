begin (* test 7 (correct) *)
  integer x, y, r;

  x := 3; y := 4;
  if (x < y) & true -> r := 10;
  [] false | (x > y) -> r := 11;
  [] x = y -> r := -12-3*2-1;
  [] true -> skip;
  fi;
  write x,y,r;
end.