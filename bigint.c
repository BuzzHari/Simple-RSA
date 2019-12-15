#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#include "bigint.h"

word DATA0[1] = {0}; word DATA1[1] = {1}; word DATA2[1] = {2};
word DATA3[1] = {3}; word DATA4[1] = {4}; word DATA5[1] = {5};
word DATA6[1] = {6}; word DATA7[1] = {7}; word DATA8[1] = {8};
word DATA9[1] = {9}; word DATA10[1] = {10};
bigint NUMS[11] = {{1, 1, DATA0},{1, 1, DATA1},{1, 1, DATA2},
                   {1, 1, DATA3},{1, 1, DATA4},{1, 1, DATA5},
                   {1, 1, DATA6},{1, 1, DATA7},{1, 1, DATA8},
                   {1, 1, DATA9},{1, 1, DATA10}};

// Funtion to initialize a bigint number.
bigint* bigint_init() {
	bigint* b = malloc(sizeof(bigint));
	b->length = 0;
	b->capacity = BIGINT_CAPACITY;
	b->data = calloc(BIGINT_CAPACITY, sizeof(word));
	return b;
}

// Function to de-initialize (or free) a bigint number.
void bigint_deinit(bigint* b) {
	free(b->data);
	free(b);
}

int bigint_iszero(bigint* b) {
	return b->length == 0 || (b->length == 1 && b->data[0] == 0);
}

int bigint_isnonzero(bigint* b) {
	return !bigint_iszero(b);
}

void bigint_copy(bigint* source, bigint* dest) {
	dest->length = source->length;
	if(source->capacity > dest->capacity) {
		dest->capacity = source->capacity;
		dest->data = realloc(dest->data, dest->capacity * sizeof(word));
	}
	memcpy(dest->data, source->data, dest->length * sizeof(word));
}

void bigint_fromstring(bigint* b, char* string) {
	int i, len = 0;
	while(string[len] != '\0') len++; /* Find string length */
	for(i = 0; i < len; i++) {
		// if(i != 0) bigint_imultiply(b, &NUMS[10]); /* Base 10 multiply */
        if (i != 0)
        {
            bigint *result = bigint_init();
            bigint_multiply(result, b ,&NUMS[10]);
            bigint_copy(result, b);
            bigint_deinit(result);
        }

        bigint* result = bigint_init();
        bigint_add(result, b, &NUMS[string[i] - '0']);
        bigint_copy(result, b);
        bigint_deinit(result);
	}
}

void bigint_fromint(bigint* b, unsigned int num) {
	b->length = 1;
	if(b->capacity < b->length) {
		b->capacity = b->length;
		b->data = realloc(b->data, b->capacity * sizeof(word));
	}
	b->data[0] = num;
}

void bigint_print(bigint* b) {
	int cap = 100, len = 0, i;
	char* buffer = malloc(cap * sizeof(char));
	bigint *copy = bigint_init(), *remainder = bigint_init();
	if(b->length == 0 || bigint_iszero(b)) printf("0");
	else {
		bigint_copy(b, copy);
		while(bigint_isnonzero(copy)) {
			// bigint_idivider(copy, &NUMS[10], remainder);
            bigint *result = bigint_init();
            bigint_divide(result, remainder, copy, &NUMS[10]);
            bigint_copy(result, copy);
            bigint_deinit(result);
			buffer[len++] = remainder->data[0];
			if(len >= cap) {
				cap *= 2;
				buffer = realloc(buffer, cap * sizeof(char));
			}
		}
		for(i = len - 1; i >= 0; i--) printf("%d", buffer[i]);
	}
	bigint_deinit(copy);
	bigint_deinit(remainder);
	free(buffer);
    printf("\n");
}

int bigint_equal(bigint* b1, bigint* b2) {
	int i;
	if(bigint_iszero(b1) && bigint_iszero(b2)) return 1;
	else if(bigint_iszero(b1)) return 0;
	else if(bigint_iszero(b2)) return 0;
	else if(b1->length != b2->length) return 0;
	for(i = b1->length - 1; i >= 0; i--) {
		if(b1->data[i] != b2->data[i]) return 0;
	}
	return 1;
}

int bigint_greater(bigint* b1, bigint* b2) {
	int i;
	if(bigint_iszero(b1) && bigint_iszero(b2)) return 0;
	else if(bigint_iszero(b1)) return 0;
	else if(bigint_iszero(b2)) return 1;
	else if(b1->length != b2->length) return b1->length > b2->length;
	for(i = b1->length - 1; i >= 0; i--) {
		if(b1->data[i] != b2->data[i]) return b1->data[i] > b2->data[i];
	}
	return 0;
}

int bigint_less(bigint* b1, bigint* b2) {
	int i;
	if(bigint_iszero(b1) && bigint_iszero(b2)) return 0;
	else if(bigint_iszero(b1)) return 1;
	else if(bigint_iszero(b2)) return 0;
	else if(b1->length != b2->length) return b1->length < b2->length;
	for(i = b1->length - 1; i >= 0; i--) {
		if(b1->data[i] != b2->data[i]) return b1->data[i] < b2->data[i];
	}
	return 0;
}

int bigint_geq(bigint* b1, bigint* b2) {
	return !bigint_less(b1, b2);
}

int bigint_leq(bigint* b1, bigint* b2) {
	return !bigint_greater(b1, b2);
}

void bigint_add(bigint* result, bigint* b1, bigint* b2) {
	word sum, carry = 0;
	int i, n = MAX(b1->length, b2->length);
	if(n + 1 > result->capacity) {
		result->capacity = n + 1;
		result->data = realloc(result->data, result->capacity * sizeof(word));
	}
	for(i = 0; i < n; i++) {
		sum = carry;
		if(i < b1->length) sum += b1->data[i];
		if(i < b2->length) sum += b2->data[i];
		result->data[i] = sum; /* Already taken mod 2^32 by unsigned wrap around */

		if(i < b1->length) {
			if(sum < b1->data[i]) carry = 1; /* Result must have wrapped 2^32 so carry bit is 1 */
			else carry = 0;
		}
		else {
			if(sum < b2->data[i]) carry = 1; /* Result must have wrapped 2^32 so carry bit is 1 */
			else carry = 0;
		}
	}
	if(carry == 1) {
		result->length = n + 1;
		result->data[n] = 1;
	}
	else {
		result->length = n;
	}
}

void bigint_subtract(bigint* result, bigint* b1, bigint* b2) {
	int length = 0, i;
	word carry = 0, diff, temp;
	if(b1->length > result->capacity) {
		result->capacity = b1->length;
		result->data = realloc(result->data, result->capacity * sizeof(word));
	}
	for(i = 0; i < b1->length; i++) {
		temp = carry;
		if(i < b2->length) temp = temp + b2->data[i]; /* Auto wrapped mod RADIX */
		diff = b1->data[i] - temp;
		if(temp > b1->data[i]) carry = 1;
		else {
			if( temp == 0 && b2->data[i] == 0xffffffff ){
				carry = 1;
			}else{
				carry = 0;
			}
		}
		result->data[i] = diff;
		if(result->data[i] != 0) length = i + 1;
	}
	result->length = length;
}



void bigint_multiply(bigint* result, bigint* b1, bigint* b2) {
	int i, j, k;
	word carry, temp;
	unsigned long long int prod; /* Long for intermediate product... this is not portable and should probably be changed */
	if(b1->length + b2->length > result->capacity) {
		result->capacity = b1->length + b2->length;
		result->data = realloc(result->data, result->capacity * sizeof(word));
	}
	for(i = 0; i < b1->length + b2->length; i++) result->data[i] = 0;
	
	for(i = 0; i < b1->length; i++) {
		for(j = 0; j < b2->length; j++) {
			prod = (b1->data[i] * (unsigned long long int)b2->data[j]) + (unsigned long long int)(result->data[i+j]); /* This should not overflow... */
			carry = (word)(prod / RADIX);
			
			/* Add carry to the next word over, but this may cause further overflow.. propogate */
			k = 1;
			while(carry > 0) {
				temp = result->data[i+j+k] + carry;
				if(temp < result->data[i+j+k]) carry = 1;
				else carry = 0;
				result->data[i+j+k] = temp; /* Already wrapped in unsigned arithmetic */
				k++;
			}
			
			prod = (result->data[i+j] + b1->data[i] * (unsigned long long int)b2->data[j]) % RADIX; /* Again, should not overflow... */
			result->data[i+j] = prod; /* Add */
		}
	}
	if(b1->length + b2->length > 0 && result->data[b1->length + b2->length - 1] == 0) result->length = b1->length + b2->length - 1;
	else result->length = b1->length + b2->length;
}

void bigint_remainder(bigint* source, bigint *div, bigint* remainder) {
	bigint *q = bigint_init();
	bigint_divide(q, remainder, source, div);
	bigint_deinit(q);
}

void bigint_divide(bigint* quotient, bigint* remainder, bigint* b1, bigint* b2) {
	bigint *b2copy = bigint_init(), *b1copy = bigint_init();
	bigint *temp = bigint_init(), *temp2 = bigint_init(), *temp3 = bigint_init();
	bigint* quottemp = bigint_init();
	word carry = 0;
	int n, m, i, j, length = 0;
	unsigned long long factor = 1;
	unsigned long long gquot, gtemp, grem;
	if(bigint_less(b1, b2)) { /* Trivial case, b1/b2 = 0 iff b1 < b2. */
		quotient->length = 0;
		bigint_copy(b1, remainder);
	}
	else if(bigint_iszero(b1)) { /* 0/x = 0.. assuming b2 is nonzero */
		quotient->length = 0;
		bigint_fromint(remainder, 0);
	}
	else if(b2->length == 1) { /* Division by a single limb means we can do simple division */
		if(quotient->capacity < b1->length) {
			quotient->capacity = b1->length;
			quotient->data = realloc(quotient->data, quotient->capacity * sizeof(word));
		}
		for(i = b1->length - 1; i >= 0; i--) {
			gtemp = carry * RADIX + b1->data[i];
			gquot = gtemp / b2->data[0];
			quotient->data[i] = gquot;
			if(quotient->data[i] != 0 && length == 0) length = i + 1;
			carry = gtemp % b2->data[0];
		}
		bigint_fromint(remainder, carry);
		quotient->length = length;
	}
	else { /* Long division is neccessary */
		n = b1->length + 1;
		m = b2->length;
		if(quotient->capacity < n - m) {
			quotient->capacity = n - m;
			quotient->data = realloc(quotient->data, (n - m) * sizeof(word));
		}
		bigint_copy(b1, b1copy);
		bigint_copy(b2, b2copy);
		/* Normalize.. multiply by the divisor by 2 until MSB >= HALFRADIX. This ensures fast
		 * convergence when guessing the quotient below. We also multiply the dividend by the
		 * same amount to ensure the result does not change. */
		while(b2copy->data[b2copy->length - 1] < HALFRADIX) {
			factor *= 2;
			// bigint_imultiply(b2copy, &NUMS[2]);
            bigint* result = bigint_init();
            bigint_multiply(result, b2copy, &NUMS[2]);
            bigint_copy(result, b2copy);
            bigint_deinit(result);
		}
		if(factor > 1) {
			bigint_fromint(temp, factor);
			// bigint_imultiply(b1copy, temp);
            bigint* result = bigint_init();
            bigint_multiply(result, b1copy, temp);
            bigint_copy(result, b1copy);
            bigint_deinit(result);
		}
		/* Ensure the dividend is longer than the original (pre-normalized) divisor. If it is not
		 * we introduce a dummy zero word to artificially inflate it. */
		if(b1copy->length != n) {
			b1copy->length++;
			if(b1copy->length > b1copy->capacity) {
				b1copy->capacity = b1copy->length;
				b1copy->data = realloc(b1copy->data, b1copy->capacity * sizeof(word));
			}
			b1copy->data[n - 1] = 0;
		}
		
		/* Process quotient by long division */
		for(i = n - m - 1; i >= 0; i--) {
			gtemp = RADIX * b1copy->data[i + m] + b1copy->data[i + m - 1];
			gquot = gtemp / b2copy->data[m - 1];
			if(gquot >= RADIX) gquot = UINT_MAX;
			grem = gtemp % b2copy->data[m - 1];
			while(grem < RADIX && gquot * b2copy->data[m - 2] > RADIX * grem + b1copy->data[i + m - 2]) { /* Should not overflow... ? */
				gquot--;
				grem += b2copy->data[m - 1];
			}
			quottemp->data[0] = gquot % RADIX;
			quottemp->data[1] = (gquot / RADIX);
			if(quottemp->data[1] != 0) quottemp->length = 2;
			else quottemp->length = 1;
			bigint_multiply(temp2, b2copy, quottemp);
			if(m + 1 > temp3->capacity) {
				temp3->capacity = m + 1;
				temp3->data = realloc(temp3->data, temp3->capacity * sizeof(word));
			}
			temp3->length = 0;
			for(j = 0; j <= m; j++) {
				temp3->data[j] = b1copy->data[i + j];
				if(temp3->data[j] != 0) temp3->length = j + 1;
			}
			if(bigint_less(temp3, temp2)) {
				// bigint_iadd(temp3, b2copy);
                bigint* result = bigint_init();
                bigint_add(result, temp3, b2copy);
                bigint_copy(result, temp3);
                bigint_deinit(result);
				gquot--;
			}
			// bigint_isubtract(temp3, temp2);
            bigint* result = bigint_init();
            bigint_subtract(result, temp3, temp2);
            bigint_copy(result, temp3);
            bigint_deinit(result);
			for(j = 0; j < temp3->length; j++) b1copy->data[i + j] = temp3->data[j];
			for(j = temp3->length; j <= m; j++) b1copy->data[i + j] = 0;
			quotient->data[i] = gquot;
			if(quotient->data[i] != 0) quotient->length = i;
		}
		
		if(quotient->data[b1->length - b2->length] == 0) quotient->length = b1->length - b2->length;
		else quotient->length = b1->length - b2->length + 1;
		
		/* Divide by factor now to find final remainder */
		carry = 0;
		for(i = b1copy->length - 1; i >= 0; i--) {
			gtemp = carry * RADIX + b1copy->data[i];
			b1copy->data[i] = gtemp/factor;
			if(b1copy->data[i] != 0 && length == 0) length = i + 1;
			carry = gtemp % factor;
		}
		b1copy->length = length;
		bigint_copy(b1copy, remainder);
	}
	bigint_deinit(temp);
	bigint_deinit(temp2);
	bigint_deinit(temp3);
	bigint_deinit(b1copy);
	bigint_deinit(b2copy);
	bigint_deinit(quottemp);
}

/*
void bigint_imultiply(bigint* source, bigint* mult) {
	bigint* temp = bigint_init();
	bigint_multiply(temp, source, mult);
	bigint_copy(temp, source);
	bigint_deinit(temp);
}

void bigint_imodulate(bigint* source, bigint* modulus) {
	bigint *q = bigint_init(), *r = bigint_init();
	bigint_divide(q, r, source, modulus);
	bigint_copy(r, source);
	bigint_deinit(q);
	bigint_deinit(r);
}

void bigint_idivide(bigint *source, bigint *div) {
	bigint *q = bigint_init(), *r = bigint_init();
	bigint_divide(q, r, source, div);
	bigint_copy(q, source);
	bigint_deinit(q);
	bigint_deinit(r);
}

void bigint_idivider(bigint* source, bigint* div, bigint* remainder) {
	bigint *q = bigint_init(), *r = bigint_init();
	bigint_divide(q, r, source, div);
	bigint_copy(q, source);
	bigint_copy(r, remainder);
	bigint_deinit(q);
	bigint_deinit(r);
}

void bigint_isubtract(bigint* source, bigint* sub) {
	bigint* temp = bigint_init();
	bigint_subtract(temp, source, sub);
	bigint_copy(temp, source);
	bigint_deinit(temp);
}

void bigint_iadd(bigint* source, bigint* add) {
	bigint* temp = bigint_init();
	bigint_add(temp, source, add);
	bigint_copy(temp, source);
	bigint_deinit(temp);
}
*/