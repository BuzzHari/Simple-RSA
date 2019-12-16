#include "bigint.h"
#include <stdio.h>

int main()
{
    bigint *b1 = bigint_init(), *b2 = bigint_init();
    bigint *result = bigint_init();
    bigint *result1 = bigint_init();

    bigint_fromstring(b1, "12345678902342342389743298329584723931540987");
    bigint_fromstring(b2, "2");

    bigint_add(result, b1, b2);
    bigint_print(result);

    bigint_subtract(result, b1, b2);
    bigint_print(result);

    bigint_multiply(result, b1, b2);
    bigint_print(result);

    bigint_divide(result, result1, b1, b2);
    bigint_print(result);
    bigint_print(result1);

    bigint_remainder(b1, b2, result1);
    bigint_print(result1);

    bigint_deinit(result);
    bigint_deinit(result1);
    bigint_deinit(b1);
    bigint_deinit(b2);


    return 0;
}
