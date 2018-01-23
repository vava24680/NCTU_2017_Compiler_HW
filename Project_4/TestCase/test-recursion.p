PROGRAM aa(input, output, error);
var a, b: INTEGER;
var c: REAL;

FUNCTION fact(x:INTEGER) : INTEGER;
begin
	if(x = 1) then
		fact := x
	else
		fact := x*fact(x-1)
end;

begin
	a := fact(5);
	print a
end.
