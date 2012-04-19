(* Procedure already declared on higher level of scope*)
begin
  proc search
  begin
	proc search
  	begin end;
  end;  
end.