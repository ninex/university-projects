begin (* test 5 (correct) *)
  const five = 5;
  boolean ok;
  integer p, q;
  
  p := 1; q := 3;
  ok := true | ~(p < q);
  write p,q,ok;
end.