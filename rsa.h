#include "bigint.h"
#include <stdlib.h>
#define ACCURACY 20

int primality_check(bigint *num, int acc);
int miller_rabin(int a, bigint *n);
void prime_gen(int digits, bigint *prime);
void exp_gen(bigint *phi, bigint *exp, int);

//bignum* encode();
//bignum* decode();

void prime_gen(int digits, bigint *prime){
    char *string = (char*) malloc((digits + 1) * sizeof(char));
	int i;
	string[0] = (rand() % 9) + '1'; /* No leading zeros */
	string[digits - 1] = (rand() % 5) * 2 + '1'; /* Last digit is odd */
	for(i = 1; i < digits- 1; i++) string[i] = (rand() % 10) + '0';
	string[digits] = '\0';
	bigint_fromstring(prime, string);
	while(1) {
		if(primality_check(prime, ACCURACY)) {
			free(string);
			return;
		}
        bigint *temp = bigint_init();
		bigint_add(temp, prime, &NUMS[2]); /* result += 2 */
        bigint_copy(temp, prime);
        bigint_deinit(temp);
	}
}

int miller_rabin(int a, bigint *n){
   
    bigint *d = bigint_init();
    bigint_subtract(d, n, &NUMS[1]);

    bigint *dtemp = bigint_init();
    //d % 2
    bigint_mod(dtemp, d, &NUMS[2]); 
    while(bigint_iszero(dtemp)){
        bigint *q = bigint_init();
        bigint *r = bigint_init();
        bigint_divide(q, r, d, &NUMS[2]);
        bigint_copy(q, d);
        bigint_deinit(q);
        bigint_deinit(r);
         
        bigint_mod(dtemp, d, &NUMS[2]);
    }
    
    bigint *t = bigint_init();
    //t = a^d mod n
    bigint *atemp = bigint_init();
    bigint_fromint(atemp, a);
    bigint_modpow(atemp, d, n, t);
    //ntemp = n-1 
    bigint *ntemp = bigint_init();
    bigint_subtract(ntemp, n, &NUMS[1]);
    while(!bigint_equal(d,ntemp) && !bigint_equal(t, &NUMS[1]) && !bigint_equal(t,ntemp)){
        
        bigint *ttemp = bigint_init();
        bigint_multiply(ttemp, t, t);
        bigint *mtemp = bigint_init();
        bigint_mod(mtemp, ttemp, n);
        bigint_copy(mtemp,t);

        bigint_deinit(mtemp);
        bigint_deinit(ttemp);

        bigint *res = bigint_init();
        bigint_multiply(res, d, &NUMS[2]);
        bigint_copy(res, d);
        bigint_deinit(res);
    }

    bigint_mod(dtemp, d, &NUMS[2]);
    int flag = 0;
    if( bigint_equal(t,ntemp) || bigint_equal(dtemp, &NUMS[1]))
        flag = 1;
    else 
        flag = 0;

    bigint_deinit(d);
    bigint_deinit(atemp);
    bigint_deinit(dtemp);
    bigint_deinit(t);
    bigint_deinit(ntemp);

    return flag;
}

int primality_check(bigint *num, int acc){
    
    if(bigint_equal(num, &NUMS[2]))
        return 1;
	else if(num->data[0] % 2 == 0 || bigint_equal(num, &NUMS[1]))
        return 0;
	while(acc--) {
		if(num->length <= 1) { /* Prevent a > num */
			if(!miller_rabin(rand() % (num->data[0] - 2) + 2, num)) 
                return 0;
		}
        else {
			int wit = rand() % (RAND_MAX - 2) + 2;
			if(!miller_rabin(wit, num)) 
                return 0;
		}
	}
	return 1;

}


void exp_gen(bigint* phi, bigint* result, int n) {
	bigint* gcd = bigint_init();
	int e = rand() % n;
	while(1) {
		bigint_fromint(result, e);
		bigint_gcd(result, phi, gcd);
		if(bigint_equal(gcd, &NUMS[1])) {
			bigint_deinit(gcd);
			return;
		}
		e = (e + 1) % n;
		if(e <= 2) 
            e = 3;
	}
}


