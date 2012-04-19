begin
  proc p1
  begin
    proc p2
    begin
      call p2;
      call p1;
    end;

    call p2;
  end;

  call p1;
  (* defect: following call of p2 is illegal *)
  call p2;
end.