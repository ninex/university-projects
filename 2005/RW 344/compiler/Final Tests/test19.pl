begin
  proc p1
  begin
    proc p2
    begin
      integer p1;

      p1 := 1;
    end;

    call p2;
  end;

  call p1;
end.