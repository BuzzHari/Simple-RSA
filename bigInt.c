#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"bigInt.h"

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

bigInt* bigIntStrToArr(const char *str){
    if(!str)
        return NULL;
    bigInt *newNum = (bigInt*) malloc(sizeof(bigInt)*1);
    
    newNum->len = strlen(str);
    newNum->num = (char *) malloc(sizeof(char) * (newNum->len));
    if(!(newNum->num)){
        perror("Malloc Error");
        exit(0);
    }
    int i = 0;
    for(i = 0; i < newNum->len; i++){
        newNum->num[i] = str[i] - '0';
    }

    //to remove the uncessary zeros if present.
    removeZeroPadding(newNum);
    return newNum;
}

void addZeroPadding(bigInt *num, long long int offset){
    if(num == NULL){
        printf("addZeroPadding: Nan\n");
        return;
    }
    long long int newLen = num->len + offset;
    char *tempNum = (char*) malloc(sizeof(char)*(newLen));
    long long int i = 0;
    while(i < offset)
        tempNum[i++] = 0;
    int j = 0;
    while(i < newLen){
        tempNum[i] = num->num[j];
        i++;
        j++;
    }

    free(num->num);
    
    num->num = tempNum;
    num->len = newLen;
}

void deallocate(bigInt *num){
    if(num == NULL){
        printf("deallocate: NaN\n");
        return;
    }

    free(num->num);
    free(num);
}

void addZeroPaddingEnd(bigInt *num, long long int offset){
    long long int newLen = num->len + offset;
    
    num->num = (char*)realloc(num->num, sizeof(char)*newLen);

    long long int i = num->len ;
    while(i < newLen){
        num->num[i++] = 0;
    }
    
    num->len = newLen;

}

void removeZeroPadding(bigInt *num){
    long long int i = 0;
    char *tempNum = NULL;
    while(i < num->len && num->num[i] == 0 )
        i++;
    
    if(i == num->len){
        tempNum = (char*) malloc(sizeof(char)*(1));
        tempNum[0] = 0;
        num->len = 1;
    }
    else{
        long long int newLen = num->len - i;
        tempNum = (char*) malloc(sizeof(char)*(newLen));
        long long int j = 0;
        while(i < num->len){
            tempNum[j] = num->num[i];
            j++;
            i++;
        }
        num->len = newLen;
    } 
    
    free(num->num);
    num->num = tempNum;
}


void bigIntPrint(bigInt *num){
    if(num == NULL){
        printf("NaN\n");
        return;
    }

    for(int i = 0; i < num->len; i++){
        printf("%d", num->num[i]);
    }
    printf("\n");
}

/*
 * Returns:
 *   1 - if num1 > num2
 *  -1 - if num1 < num2
 *   0 - if num1 = num2
 */
int bigIntCompare(bigInt *num1, bigInt *num2){
    if(num1->len > num2->len)
        return 1;
    if(num1->len < num2->len)
        return -1;
    
    for(int i = 0; i < num1->len; i++){
        if(num1->num[i] > num2->num[i])
            return 1;
        else if(num1->num[i] < num2->num[i])
            return -1;
    }

    return 0;
}



bigInt* bigIntIncrement(bigInt *num){
    if( num == NULL){
        printf("bigIntIncrement:Nan\n");
        return NULL;
    }

    bigInt* unity = (bigInt*)malloc(sizeof(bigInt)*1);
    unity->num = (char *)calloc(1, sizeof(char));
    unity->num[0] = 1;
    unity->len = 1;
    
    bigInt *temp = bigIntAdd(num,unity);
    
    free(num->num);
    free(num);
    
    num = temp;
    free(unity->num);
    free(unity);
    return num;
}

bigInt* bigIntSub(bigInt *num1, bigInt *num2){
    if(num1 == NULL || num2 == NULL)
        return NULL;
    
    
    long long int len = MAX(num1->len, num2->len);
    bigInt *result = (bigInt*) malloc(sizeof(bigInt)*1);
    result->len = 0;
    result->num = (char *) calloc(len, sizeof(char)); 
    if(!(result->num)){
        perror("Calloc Error");
        exit(0);
    } 

    char *bignum, *smallnum, p_flag = -1;
    if(num1->len > num2->len){
        addZeroPadding(num2,num1->len - num2->len);
        p_flag = 2;
        bignum = num1->num;
        smallnum = num2->num;
    }
    else if(num1->len < num2->len){
        addZeroPadding(num1, num2->len - num1->len);
        p_flag = 1;
        bignum = num2->num;
        smallnum = num1->num;
    }
    else{
        if(bigIntCompare(num1, num2) == 1){
            bignum = num1->num;
            smallnum = num2->num;
        }
        else{
            bignum = num2->num;
            smallnum = num1->num;
        }
    }
    char carry = 0, temp;
    for(long long int i = len-1; i >= 0; i--){
        temp = bignum[i]; 
        if(carry){
            temp -= 1;
            carry = 0;
        }
        if(temp < smallnum[i]){
            temp+=10;
            carry = 1;
        }
        result->num[i] = temp - smallnum[i];
        result->len += 1;
    }
    
    if(p_flag == 1)
        removeZeroPadding(num1);
    else if(p_flag == 2)
        removeZeroPadding(num2);
    removeZeroPadding(result);
    return result;

}

bigInt* bigIntAdd(bigInt* num1, bigInt* num2)
{
    if(num1 == NULL || num2 == NULL)
        return NULL;
    
    
    char *bignum, *smallnum;
    bigInt *result = (bigInt*) malloc(sizeof(bigInt)*1);
    result->len = 0;
    long int size = num1->len;
    
    int p_flag = -1;
    if (num2->len > num1->len){
        addZeroPadding(num1, num2->len - num1->len);
        bignum = num2->num;
        smallnum = num1->num;
        size = num2->len;
        p_flag = 1;
    }
    else if(num2->len < num1->len){ addZeroPadding(num2,num1->len - num2->len);
        smallnum = num2->num;
        bignum = num1->num;
        p_flag = 2;
    }
    else{
        bignum = num1->num;
        smallnum = num2->num;
    }

    result->num = (char *) calloc(size+1, sizeof(char));
    
    int i, temp = 0;
    for (i = size - 1; i >= 0; i--){
        temp = bignum[i] + smallnum[i] + temp / 10;
        result->num[i + 1] = temp % 10;
    }
    result->num[++i] = temp / 10;
    result->len = size+1;
    
    
    if(p_flag == 1)
        removeZeroPadding(num1);
    else if(p_flag == 2)
        removeZeroPadding(num2);
    removeZeroPadding(result);
    return result;
}


int iszero(bigInt* num)
{
	for(int i=0; i<num->len; i++)
	{
		if(num->num[i] != 0)
			return 0;
	}
	return 1;
}


//Divides num1/num2
bigInt* bigIntDiv(bigInt* num1, bigInt* num2)
{

	bigInt*  result = (bigInt*)malloc(sizeof(bigInt)*1);
	result->num = (char *) calloc(num1->len, sizeof(char));
	result->len = 1;
    /*
     *bigInt* unity = (bigInt*)malloc(sizeof(bigInt)*1);
     *unity->num = (char *)calloc(1, sizeof(char));
     *unity->num[0] = 1;
     *unity->len = 1;
     */

	if(num1->num == NULL || num2->num == NULL){
        printf("bigIntDiv: NaN\n");
		return (bigInt*)NULL;
	}
	
    if((iszero(num1) == 1) || (bigIntCompare(num1, num2) == -1)){
		// printf("Division is 0\n");
		result->num[0] = 0;
		result->len = 1;
		return result;
	}

    if(iszero(num2) == 1){
        printf("Division by zero is INF\n");
        return (bigInt*)NULL;
    }
    
    
    bigInt *copy = (bigInt*) malloc(sizeof(bigInt)*1);
    copy->num = (char*) malloc(sizeof(char)*num1->len);
    copy->len = num1->len; 
    
    //creating a copy of bignum;
    for(long long int i = 0; i < num1->len; i++)
        copy->num[i] = num1->num[i];
    
    bigInt *temp = copy;

    //run while loop while num1 > num2
    int count = 0;
    while(bigIntCompare(copy, num2) != -1){
        //printf("...wait..");
		temp = bigIntSub(copy, num2);
        
        deallocate(copy);
        copy = temp;
         
        result = bigIntIncrement(result);
        count+=1;
        //printf("%d\n",count);
		//result = bigIntAdd(result, unity);
	}
    deallocate(temp);
    removeZeroPadding(result);	
    return result;
}


bigInt* bigIntMul(bigInt *num1, bigInt *num2){
    if(num1 == NULL || num2 == NULL){
        printf("bigIntMul: NaN\n");
        return NULL;
    }

    char p_flag = -1;
    long long int len = 0;
    if(num1->len > num2->len){
        addZeroPadding(num2,num1->len - num2->len);
        p_flag = 2;
        len = num1->len;
    }
    else if(num1->len < num2->len){
        addZeroPadding(num1, num2->len - num1->len);
        len = num2->len;
    }
    else{
        len = num1->len;
    }

    bigInt *result = (bigInt*) malloc(sizeof(bigInt)*1);
    result->num = (char*) calloc(2*len, sizeof(char));
    result->len = 2*len;
    char temp  = 0; 
    char carry = 0; 
    long long int shift = (2*len)-1;
    for(long long int i = len - 1; i >= 0; i--){
        carry = 0;
        shift = len + i;
        for(long long int j = len - 1; j >= 0; j--){
            temp = result->num[shift] + (num1->num[j] * num2->num[i]) + carry ;
            result->num[shift] = temp % 10;
            carry = temp / 10;
            shift-=1;
        }
        result->num[shift] = result->num[shift] + carry;
    }
    
    removeZeroPadding(result);
    return result;
}


bigInt* bigIntPow(bigInt* num1, bigInt* num2)
{
    // printf("Power function called\n\n");
    bigInt* result = (bigInt*)malloc(sizeof(bigInt));
    bigInt* temp = (bigInt*)malloc(sizeof(bigInt));
    bigInt* num2_copy = (bigInt*)malloc(sizeof(bigInt));
    temp->num = (char *)calloc(num1->len + num2->len, sizeof(char));

    // UNITY (probably not matrix reference -_-)
    bigInt* unity = (bigInt*)malloc(sizeof(bigInt)*1);
    unity->num = (char *)calloc(1, sizeof(char));
    unity->num[0] = 1;
    unity->len = 1;

    temp->len = 0;
    if(num2->len > num1->len)
    {
        temp = num1;
        num1 = num2;
        num2 = temp;
    }
    num2_copy->len = num2->len;
    num2_copy->num = (char*)calloc(num2->len, sizeof(char));
    num2_copy->num = num2->num;
    int size = num1->len;
    result->num = (char *)calloc(size+(2*num2->len), sizeof(char));
	if (iszero(num2))
	{
		result->num[0] = 1;
        result->len = 1;
        return result;
	}

    result->num = num1->num;
    result->len = num1->len;

    num2_copy = bigIntSub(num2_copy, unity);

    // printf("Num2_copy before entering the loop: ");
    // bigIntPrint(num2);

    while(!iszero(num2_copy))
    {
        result = bigIntMul(result, num1);
        num2_copy = bigIntSub(num2_copy, unity);
        // bigIntPrint(num2_copy);
    }
    return result;
}


bigInt* bigIntLongDiv(bigInt *divisor, bigInt *dividend){
    
    if(divisor == NULL || dividend == NULL)
        return NULL;
    
    bigInt *result = (bigInt*) malloc(sizeof(bigInt)*1);
    result->num = (char*) calloc(dividend->len, sizeof(char));
    result->len = dividend->len;
   

    //If numerator is smaller than dividend.
    //Return 0.
    if(bigIntCompare(divisor, dividend) == -1){
        result->num = 0;
        result->len = 1;
        return result;
    }


}



