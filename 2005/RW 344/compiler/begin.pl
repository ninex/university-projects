
(* Linear (*nes(*  *)ted*)search *)
begin
  const n = 10; 
  integer x,i; boolean found;
  
  proc Search
  begin integer m;
    i, m := 0, n;
    do i < m ->
      if A[i] = x ->
      [] ~(A[i] = x) -> i := i + 1;
      fi;
    od;
    found := A[i] = x;
  end;

  (* read table *)
  i := 0;
  do ~(i = n) -> read A[i]; i := i + 1; od;
  (* test search *)
  read x;
  do ~(x = 0) ->
    call Search;
    if found -> write x, i;
    [] ~found -> write x;
    fi;
    read x;
  od;
end.