begin
    read (NOMBRE_LARGO_DE_VARIABLE, J);
    read(CONTA7456, OTRA_VARIABLE, FECHA);
    __KONTADOR := NOMBRE_LARGO_DE_VARIABLE + J;
    write(__KONTADOR);
    write(__KONTADOR + __KONTADOR + NOMBRE_LARGO_DE_VARIABLE);
    CONTA7456 := CONTA7456 + (OTRA_VARIABLE - __KONTADOR) + J;
    write(CONTA7456);
    read(__KONTADOR);
    write(__KONTADOR);
    __KONTADOR := __KONTADOR + __KONTADOR + __KONTADOR + __KONTADOR - 22 + __KONTADOR + 7 - CONTA7456;
    J := OTRA_VARIABLE + FECHA - NOMBRE_LARGO_DE_VARIABLE;
    write (CONTA7456, OTRA_VARIABLE, FECHA, NOMBRE_LARGO_DE_VARIABLE, J, __KONTADOR);
end


