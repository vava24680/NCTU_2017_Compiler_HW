PROGRAM quadratic(input, output, error);
var a, b, c: INTEGER;

FUNCTION quad(x: integer) : INTEGER;
	var result: INTEGER;
	begin
		result := 0;
		result := result + c;
		result := result + b * x;
		result := result + a * x * x;
		quad := result
	end;

begin
	a := 1;
	b := 2;
	c := 3;
	print quad(6)
end.