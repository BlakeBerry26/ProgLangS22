PROGRAM THREEDIM;
VAR
    XAXIS: INTEGER;
    YAXIS: INTEGER;
    ZAXIS: INTEGER;
    X: INTEGER;
    Y: INTEGER;
    Z: INTEGER;
BEGIN
    X := 0;
    Y := 0;
    Z := 0;
    WRITE('WHAT IS THE SIZE OF THE CUBE');
    READ(XAXIS);
    READ(YAXIS);
    READ(ZAXIS);
    WHILE Z <ZAXIS
    BEGIN
        WRITE('STARTING NEW LEVEL');
        WHILE X<XAXIS
        BEGIN
            WHILE Y  <  YAXIS
            BEGIN
                WRITE(X);
                WRITE(Y);
                WRITE(Z);
                WRITE(' ');
                Y := Y + 1
            END;
            Y:=0;
            X:=X+1
        END;
        X:=0;
        Y:=0;
        Z:=Z + 1
    END;
    WRITE('COMPLETE')
END
