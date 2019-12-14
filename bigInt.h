//Header for the libray.


typedef struct bigInt{
    char *num;
    long long int len;
}bigInt;


/*
 * Converts the string to an array.
 */
bigInt* bigIntStrToArr(const char *str);

/*
 * frees up the allocated memory.
 */
void bigIntDestroy(bigInt*);


/*
 * Adds zeros in front of the smaller numeber
 * to make it's length equal to the larger number.
 * for ex. lets we are doing 4567 - 567
 * addZeroPadding(567) -> 0567.
 */
void addZeroPadding(bigInt*, long long int);


/*
 * This basiclly what it says.
 * It removes the unnecessary zeros from front of the number.
 * also makes 00000 -> 0.
 */
void removeZeroPadding(bigInt*);

/*
 * Function to check if the integer in 0 or not
 * like numbers like 00000000...000 should 
 * return 0. NOT IMPLEMENTED YET.
 */
void bigIntZeroCheck(bigInt*);

/*
 * Prints the integer.
 */
void bigIntPrint(bigInt*);

void bigIntDecrement(bigInt *bigInt);


/*
 * Returns:
 *   1 - if num1 > num2
 *  -1 - if num1 < num2
 *   0 - if num1 = num2
 */
int bigIntCompare(bigInt*, bigInt*);

/*
 * Returns a bigInt structure
 * with result parameter1 - parameter2
 */
bigInt* bigIntSub(bigInt*, bigInt*);

bigInt* bigIntFastMul(bigInt*, bigInt*);
bigInt* executeKaratsuba(bigInt*, bigInt*);
bigInt* getFirstHalf(bigInt*);
bigInt* getSecondHalf(bigInt*);

bigInt* karaMul(bigInt*, bigInt*);
bigInt* karaAdd(bigInt*, bigInt*);

void deallocate(bigInt*);

/*
 * FOR YOU TO IMPLEMENT.
 */


bigInt* bigIntIncrement(bigInt *bigInt);
bigInt* bigIntAdd(bigInt*, bigInt*);

//Do simple long multiplication.. 
//call removeZeroPadding before returning the result. 
bigInt* bigIntMul(bigInt*, bigInt*);
//Basically, I thing we can
//just implement num1 // num2, by repeated subtraction.
//Try finding a proper divison algo, because repeated subtraction 
//will be shit slow.
bigInt* bigIntDiv(bigInt*, bigInt*);

//Should be easy, just call mull in a loop.
bigInt* bigIntPow(bigInt*, bigInt*);

//Try to add some more functionalities..

bigInt* bigMulsmall(bigInt* num1, int num2 );
int iszero(bigInt* num);
void  addZeroPaddingEnd(bigInt *num, long long int offset);

