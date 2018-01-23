PROGRAM while_program(input, output, error);
var a, b: INTEGER;
var aa, bb, cc: REAL;
var c: INTEGER;
var d: INTEGER;

begin
	a := 0;
	b := 6;
	aa := 5.5;
	bb := 6.6;
	cc := 7.7;
	while (a < b) DO
		begin
			cc := cc + 1;
			bb := bb + 1.1;
			a := a + 1
		end;
	print cc;
	print "\n";
	print bb;
	print "\n";
	print a;
	print "\n"
end.