#include "bigint.h"

#define ACCURACY 20

int primality_check(bigint *num, int reps);
void prime_gen(int digits, bigint *prime);
void exp_gen(bigint *phi, bigint *exp, int);

//bignum* encode();
//bignum* decode();

void prime_gen(int digits, bigint *prime){
    char *string = malloc((digits + 1) * sizeof(char));
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

