begin
integer x,y;	
	proc proc11
	begin
	integer y;
	x:=10;
	y:=3;
	end;
   integer z;
   proc proc12
	begin
	integer y;
	x:=10;
	z:=3;
	proc12;
	end;
   x:=3;
   y:=4;
   proc11;   
   write x;
end.