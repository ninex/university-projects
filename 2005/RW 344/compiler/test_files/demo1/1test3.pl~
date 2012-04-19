begin (* test 3 (correct) *)
  integer array X[2];

  proc swap
  begin
    integer t;

    t := X[0]; X[0] := X[1]; X[1] := t; skip;
  end;

  X[0] := 10; X[1] := 11;
  swap;
  X[0], X[1] := X[1]+1, X[0];
  write X[0] - X[1];
end.