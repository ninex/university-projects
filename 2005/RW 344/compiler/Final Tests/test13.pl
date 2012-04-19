begin
  integer x, y;
  boolean ok;

  x, y := 1, 2;
  if (x < y) & true -> ok := true;
  [] ~(x < y) | false -> ok := false;
  fi;
end.