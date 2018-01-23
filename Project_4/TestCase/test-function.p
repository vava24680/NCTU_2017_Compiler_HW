PROGRAM aa(input, output, error);
var a, b: INTEGER;
var c: REAL;

function ss(rr :INTEGER) : INTEGER;
begin
	ss := rr + 3;
end;

begin
	a := ss(ss(ss(ss(3))));
	print a
end.

