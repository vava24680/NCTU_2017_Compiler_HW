PROGRAM aa(input, output, error);
var a, b: INTEGER;
var c: INTEGER;
var d: INTEGER;

function ss(rr :INTEGER) : INTEGER;
begin
	ss := rr + 8;
end;

begin
	a := 5;
	b := 3;
	d := 6;
	c := a + b + d;
	print c;
	c := ss(5);
	print c
end.