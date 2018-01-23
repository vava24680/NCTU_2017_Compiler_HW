PROGRAM aa(input, output, error);
var a, b: INTEGER;
var c: INTEGER;
var d: INTEGER;
var ws : STRING;

function ss(rr :INTEGER) : INTEGER;
var ww : STRING;
begin
	ww := "EFGH";
	print ww;
	ss := rr + 8
end;

begin
	a := 5;
	b := 3;
	d := 6;
	c := a + b + d;
	ws := "ABCD";
	c := ss(5);
	print ws;
	print "\n";
	print "DCBA";
	print "\n";
	print c
end.