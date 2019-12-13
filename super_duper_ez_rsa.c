#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


// a^b mod c
int modpow(long long a, long long b, int c)
{
	int res = 1;
	while(b > 0)
    {
		if(b & 1)
        {
			res = (res * a) % c;
		}
		b = b >> 1;
		a = (a * a) % c;
	}
	return res;
}

int jacobi(int a, int n)
{
	int twos, temp;
	int mult = 1;
	while(a > 1 && a != n)
    {
		a = a % n;
		if(a <= 1 || a == n) break;
		twos = 0;
		while(a % 2 == 0 && ++twos) a /= 2;
		if(twos > 0 && twos % 2 == 1) mult *= (n % 8 == 1 || n % 8 == 7) * 2 - 1;
		if(a <= 1 || a == n) break;
		if(n % 4 != 1 && a % 4 != 1) mult *= -1;
		temp = a;
		a = n;
		n = temp;
	}
	if(a == 0) return 0;
	else if(a == 1) return mult;
	else return 0;
}

int solovayPrime(int a, int n)
{
	int x = jacobi(a, n);
	if(x == -1) x = n - 1;
	return x != 0 && modpow(a, (n - 1)/2, n) == x;
}


int probablePrime(int n, int k)
{
	if(n == 2) return 1;
	else if(n % 2 == 0 || n == 1) return 0;
	while(k-- > 0)
    {
		if(!solovayPrime(rand() % (n - 2) + 2, n)) return 0;
	}
	return 1;
}

//thank you internet
int get_prime(int n)
{
	int prime = rand() % n;
	n += n % 2;
	prime += 1 - prime % 2;
	while(1)
    {
		if(probablePrime(prime, 5)) return prime;
		prime = (prime + 2) % n;
	}
}

//ezpz
int gcd(int a, int b)
{
	int temp;
	while(b != 0)
    {
		temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

// compute exponent e such that e is coprime with p and q. which is less than n
int get_exponent(int phi, int n)
{
	int e = rand() % n;
	while(1)
    {
		if(gcd(e, phi) == 1) return e;
		e = (e + 1) % n;
		if(e <= 2) e = 3;
	}
}

//using the extended eucledian algo compute the inverse d
int get_inverse(int n, int modulus)
{
	int a = n, b = modulus;
	int x = 0, y = 1, x0 = 1, y0 = 0, q, temp;
	while(b != 0)
    {
		q = a / b;
		temp = a % b;
		a = b;
		b = temp;
		temp = x; x = x0 - q * x; x0 = temp;
		temp = y; y = y0 - q * y; y0 = temp;
	}
	if(x0 < 0) x0 += modulus;
	return x0;
}

int main(void)
{
	int p, q, n, phi, e, d;
	srand(time(NULL));
	while(1)
    {
		p = get_prime(100000); //prime less than 100000
		printf("p => %d\n\n", p);
		
		q = get_prime(100000);
		printf("q => %d\n\n", q);
		
		n = p * q;
		printf("n = pq => %d\n\n", n);
		if(n < 128)
            continue;
		else 
            break;
	}
	
	phi = (p - 1) * (q - 1);
	
    printf("phi => %d\n\n", phi);
	
	e = get_exponent(phi, 1000); //get e that is less than 1000
	
    d = get_inverse(e, phi);

	printf("e => %d\n\n", e);
	
    printf("d => %d\n\n", d);

	printf("Public key => (%d, %d)\n\n", e, n);
	
	printf("Private key => (%d, %d)\n\n", d, n);

	return 0;
}
