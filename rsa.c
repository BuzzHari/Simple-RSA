#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bigint.h"
#include "rsa.h"

#define PRIME_DIGITS 100
#define EXP_MAX RAND_MAX

int main(){
    
    //int i, bytes, len;
	bigint *p = bigint_init();
    bigint *q = bigint_init(); 
    bigint *n = bigint_init();
	bigint *phi = bigint_init(); 
    bigint *e = bigint_init();
    bigint *d = bigint_init();
	//bigint *bbytes = bigint_init(), *shift = bigint_init();
	bigint *temp1 = bigint_init(), *temp2 = bigint_init();

    /*
	 *bigint *encoded;
	 *int *decoded;
	 *char *buffer;
	 *FILE* f;
     */

	srand(time(NULL));

	prime_gen(PRIME_DIGITS, p);
	printf("Got first prime factor, p = ");
	bigint_print(p);
	printf(" ... ");
	getchar();

	prime_gen(PRIME_DIGITS, q);
	printf("Got second prime factor, q = ");
	bigint_print(q);
	printf(" ... ");
	getchar();

	bigint_multiply(n, p, q);
	printf("Got modulus, n = pq = ");
	bigint_print(n);
	printf(" ... ");
	getchar();

	bigint_subtract(temp1, p, &NUMS[1]);
	bigint_subtract(temp2, q, &NUMS[1]);
	bigint_multiply(phi, temp1, temp2); /* phi = (p - 1) * (q - 1) */
	printf("Got totient, phi = ");
	bigint_print(phi);
	printf(" ... ");
	getchar();

    exp_gen(phi, e, EXP_MAX);
    printf("Chose public exponent, e = ");
    bigint_print(e);
    printf("\nPublic key is (");
    bigint_print(e);
    printf(", ");
    bigint_print(n);
    printf(") ... ");
    getchar();

    bigint_inverse(e, phi, d);
    printf("Calculated private exponent, d = ");
    bigint_print(d);
    printf("\nPrivate key is (");
    bigint_print(d);
    printf(", ");
    bigint_print(n);
    printf(") ... ");
    getchar();
}



