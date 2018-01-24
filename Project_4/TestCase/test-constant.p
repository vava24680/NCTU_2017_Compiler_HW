PROGRAM foo(input, output, error) ;
   // this test checks various degrees of constant folding

   var a, b, c: integer;

   begin
      a := 2+99; // simple expression
      print a;
	  print "\n";
      b := 3 + 93 * 5 + 87 * (23+15*6);    // complex expression
      print b;
	  print "\n";
   end.   // this is the end of the program
