#ifndef BIGINT_H
#define BIGINT_H

#define BUF_SIZE 1024
#define RADIX 4294967296UL
#define HALFRADIX 2147483648UL
#define BIGINT_CAPACITY 20
#define MAX(a,b) ((a) > (b) ? (a) : (b))

typedef unsigned int word;

typedef struct _bigint {
	int length;
	int capacity;
	word* data;
} bigint;


extern word DATA0[1], DATA1[1], DATA2[1];
extern word DATA3[1], DATA4[1], DATA5[1];
extern word DATA6[1], DATA7[1], DATA8[1];
extern word DATA9[1], DATA10[1];
extern bigint NUMS[11];

bigint* bigint_init();
void bigint_deinit(bigint* b);
int bigint_iszero(bigint* b);
int bigint_isnonzero(bigint* b);
void bigint_copy(bigint* source, bigint* dest);
void bigint_fromstring(bigint* b, char* string);
void bigint_fromint(bigint* b, unsigned int num);
void bigint_print(bigint* b);
void bigint_mod(bigint* result, bigint *num1, bigint *num2);
int bigint_equal(bigint* b1, bigint* b2);
int bigint_greater(bigint* b1, bigint* b2);
int bigint_less(bigint* b1, bigint* b2);
int bigint_geq(bigint* b1, bigint* b2);
int bigint_leq(bigint* b1, bigint* b2);


// result = b1 + b2
void bigint_add(bigint* result, bigint* b1, bigint* b2);

// result = b1 - b2
void bigint_subtract(bigint* result, bigint* b1, bigint* b2);

// result = b1 x b2
void bigint_multiply(bigint* result, bigint* b1, bigint* b2);

// remainder = source % div
void bigint_remainder(bigint* source, bigint *div, bigint* remainder);

// quotient = b1 / b2
// remainder = b1 % b2
void bigint_divide(bigint* quotient, bigint* remainder, bigint* b1, bigint* b2);

// result = base ^ exponent mod modulus
void bigint_modpow(bigint* base, bigint* exponent, bigint* modulus, bigint* result);

// result = gcd(b1, b2)
void bigint_gcd(bigint* b1, bigint* b2, bigint* result);

// result = a^-1 mod m
void bigint_inverse(bigint* a, bigint* m, bigint* result);
#endif
// void bigint_iadd(bigint* source, bigint* add);
// void bigint_isubtract(bigint* source, bigint* add);
// void bigint_imultiply(bigint* source, bigint* add);
// void bigint_idivider(bigint* source, bigint* div, bigint* remainder);
// void bigint_idivide(bigint* source, bigint* div);
// void bigint_imodulate(bigint* source, bigint* modulus);
