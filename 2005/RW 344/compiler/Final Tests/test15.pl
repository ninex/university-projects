begin
  integer x;
  boolean ok;

  proc p1
  begin
    integer x;
    integer ok;

    x, ok := 1, 2;
  end;

  x := 1;
  call p1;
end.