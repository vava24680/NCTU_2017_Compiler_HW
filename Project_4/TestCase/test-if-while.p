PROGRAM foo(input, output, error) ;
	// this test checks various operators

	var a, b, c: integer;

	begin
		// assignment statement
		a := 6;
		b := a * 15;
		print "a :";
		print a;
		print "\n";
		print "b :";
		print b;
		print "\n";

		// simple if statement
		if b > a then
			a := a * 2
		else
			a := a * 3;
		print "a :";
		print a;
		print "\n";
		print "b :";
		print b;
		print "\n";

		// nested if statement
		if b > a then
			if b < a - 2 then
				a := a * 5
			else
				a := a * 7
		else
			a := a / 11 * a;
		print "a :";
		print a;
		print "\n";
		print "b :";
		print b;
		print "\n";

		// nested if statement
		if b > a then
			a := a * 2
		else
			if b > 5 + a then
				a := 23 - a
			else
				a := 948 / b;
		print "a :";
		print a;
		print "\n";
		print "b :";
		print b;
		print "\n";

		// simple while statement
		while b > a do
			a := a * 7;
		print "a :";
		print a;
		print "\n";
		print "b :";
		print b;
		print "\n";

		// compound statement
		while b > a do
			begin
				b := b + 3;
				a := a * 7
			end;
		print "a :";
		print a;
		print "\n";
		print "b :";
		print b;
		print "\n";

		// nested while statement
		while b > a do
			while c > a + b do
				begin
					b := b + 3;
					a := a * 7
				end;
		print "a :";
		print a;
		print "\n";
		print "b :";
		print b;
		print "\n"

	end.   // this is the end of the program
