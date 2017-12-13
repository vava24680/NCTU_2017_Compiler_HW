PROGRAM foo(input, output, error) ;
		// This example tests simple procedures with parameters.
		var g: integer;
		var a: integer;
		var b: integer;
		var gg: real;
		procedure  sum(a, b: integer;c: real);
				begin
						if a <= 0 then g := b + 9
						else g := b * 3
				end;

		begin
				g := 7;
				gg := 7.55;
				a := gg;
				sum(10, 32);   // the result is 41.
				sum(-10, 21)   // the result is 63.
		end.   // this is the end of the program
