begin (* test 8 (incorrect) *)
  integer x, y;

  x := 3; y := 4;
  if x < y -> skip;
  [] ~(x < y) -> skip (* missing semicolon *)
  if (* typo: *)
end.