PROGRAM GCD(input, output, error);

FUNCTION gcd(a, b: integer) : INTEGER;
begin
    if(a = 0) then
        gcd := b
    else
    begin
        while(b != 0) do
        begin
            if(a>b) then
                a := a - b
            else
                b := b - a
        end;
        gcd := a
    end;
end;

begin
    print gcd(24, 6)
end.
