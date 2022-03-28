PROGRAM DIVISORS;

VAR
    I: INTEGER;
    N: INTEGER;

    MODULA: INTEGER;

BEGIN

    I := 1;
    WRITE('THE DIVISORS OF 100 ARE');

    WHILE (I<101)
    BEGIN
        MODULA := 100;
        WHILE (MODULA > I)
           MODULA := MODULA - I;
        IF MODULA = I THEN
            WRITE(I);
        I := I + 1
    END;  
    
    WRITE('COMPLETE')
END
