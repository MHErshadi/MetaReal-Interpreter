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

int_p int_set(const int_p src);
int_p int_set_str(const char* str, unsigned long long size);
int_p int_set_ull(unsigned long long src);

unsigned long long int_get_ull(const int_p src);
char* int_get_str(const int_p src);

void int_free(int_p num);

void int_print(FILE* stream, const int_p num, const char* end);

/* */

void int_add(int_p num1, const int_p num2);
void int_add_ul(int_p num1, unsigned long num2);

void int_subtract(int_p num1, const int_p num2);
void int_subtract_ul(int_p num1, unsigned long num2);
void int_ul_subtract(unsigned long num1, int_p num2);

void int_multiply(int_p num1, const int_p num2);
void int_multiply_ul(int_p num1, unsigned long num2);

void int_modulo(int_p num1, const int_p num2);
void int_modulo_ul(int_p num1, unsigned long num2);
void int_ul_modulo(unsigned long num1, int_p num2);

void int_quotient(int_p num1, const int_p num2);
void int_quotient_ul(int_p num1, unsigned long num2);
void int_ul_quotient(unsigned long num1, int_p num2);

void int_power(int_p num1, const int_p num2);
void int_power_ul(int_p num1, unsigned long num2);
void int_ul_power(unsigned long num1, int_p num2);

void int_and(int_p num1, const int_p num2);
void int_and_ul(int_p num1, unsigned long num2);

void int_or(int_p num1, const int_p num2);
void int_or_ul(int_p num1, unsigned long num2);

void int_xor(int_p num1, const int_p num2);
void int_xor_ul(int_p num1, unsigned long num2);

void int_lshift(int_p num1, const int_p num2);
void int_lshift_ul(int_p num1, unsigned long num2);
void int_ul_lshift(unsigned long num1, int_p num2);

void int_rshift(int_p num1, const int_p num2);
void int_rshift_ul(int_p num1, unsigned long num2);
void int_ul_rshift(unsigned long num1, int_p num2);

char int_equal(const int_p num1, const int_p num2);
char int_equal_ul(const int_p num1, unsigned long num2);

char int_nequal(const int_p num1, const int_p num2);
char int_nequal_ul(const int_p num1, unsigned long num2);

char int_less(const int_p num1, const int_p num2);
char int_less_ul(const int_p num1, unsigned long num2);
char int_ul_less(unsigned long num1, const int_p num2);

char int_greater(const int_p num1, const int_p num2);
char int_greater_ul(const int_p num1, unsigned long num2);
char int_ul_greater(unsigned long num1, const int_p num2);

char int_less_equal(const int_p num1, const int_p num2);
char int_less_equal_ul(const int_p num1, unsigned long num2);
char int_ul_less_equal(unsigned long num1, const int_p num2);

char int_greater_equal(const int_p num1, const int_p num2);
char int_greater_equal_ul(const int_p num1, unsigned long num2);
char int_ul_greater_equal(unsigned long num1, const int_p num2);

/* */

void int_negate(int_p num);

void int_not(int_p num);

/* */

char int_sign(const int_p num);

char int_fits_ull(const int_p num);
char int_fits_ul(const int_p num);

#endif /* __M_INT__ */
