begin (* test 4 (incorrect) *)
  integer array X[2];

  (* there should be no semicolon after the procedure name *)
  proc swap;
  begin
    integer t;

    t := X[0]; X[0] := X[1]; X[1] := t;
  end;

  X[0] := 10; X[1] := 11;
  call swap;
end.