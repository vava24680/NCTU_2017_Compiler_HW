PROGRAM foo(input, output, error) ;

	var c, d : integer;

		// test recursive functions
		function  sum(a, b: integer; c: real) : integer;
				var aaa : integer;
				begin
						if a <= 0 then sum := 0
						else sum := a + sum(a-1)
						// sum is the return value
				end;

		begin
				c := sum(10);   // the result is 55.
				d := sum(-10)   // the result is 0.
		end.   // this is the end of the program
