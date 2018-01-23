PROGRAM cccc(input, output, error);

FUNCTION comb(a, b: integer) : INTEGER;
	begin
		if (a > b) then
			if (a = 1) then
				comb := 1
			else
				if (a > 0) then
					if (b > 0) then
						comb := comb(a - 1, b - 1) + comb(a-1, b)
					else
						comb := 1
				else
					comb := 0
		else
			comb := 1
	end;

begin
	print comb(5, 2);
	print "\n";
	print comb(6, 2);
	print "\n"
end.