/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
/*/

#ifndef __M_INT__
#define __M_INT__

// temporary
#include <stdio.h>
#include <C:/MinGW/include/gmp.h>

struct __int__
{
    mpz_t value;
};
typedef struct __int__ int_t;
typedef struct __int__* int_p;

int_p int_set(int_p src);
int_p int_set_str(const char* str, unsigned long long size);
int_p int_set_ull(unsigned long long src);

unsigned long long int_get_ull(int_p src);

void int_free(int_p num);

void int_print(FILE* stream, int_p num, const char* end);

/* */

void int_add(int_p num1, int_p num2);
void int_subtract(int_p num1, int_p num2);
void int_multiply(int_p num1, int_p num2);
void int_modulo(int_p num1, int_p num2);
void int_quotient(int_p num1, int_p num2);
void int_power(int_p num1, int_p num2);

void int_and(int_p num1, int_p num2);
void int_or(int_p num1, int_p num2);
void int_xor(int_p num1, int_p num2);
void int_lshift(int_p num1, int_p num2);
void int_rshift(int_p num1, int_p num2);

char int_equal(int_p num1, int_p num2);
char int_nequal(int_p num1, int_p num2);
char int_less(int_p num1, int_p num2);
char int_greater(int_p num1, int_p num2);
char int_less_equal(int_p num1, int_p num2);
char int_greater_equal(int_p num1, int_p num2);

/* */

// temporary
void int_negate(int_p num);

void int_not(int_p num);

/* */

//temporary
char int_sign(int_p num);

// temporary
char int_fits_ull(int_p num);

#endif /* __M_INT__ */
