/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
/*/

#ifndef __M_INT__
#define __M_INT__

#include <stdio.h>
#include <gmp.h>

struct __int__
{
    mpz_t value;
};
typedef struct __int__ int_t;
typedef struct __int__* int_p;

/* */

int_p int_set(const int_p src);
int_p int_set_str(const char* str, unsigned long long size, unsigned char base);
int_p int_set_ull(unsigned long long src);

unsigned long long int_get_ull(const int_p src);
char* int_get_str(const int_p src, unsigned char base);

void int_free(int_p num);

void int_print(FILE* stream, const int_p num, const char* end);

/* */

int_p int_add(const int_p num1, const int_p num2);
void int_add_self(int_p num1, const int_p num2);

int_p int_add_ul(const int_p num1, unsigned long num2);
void int_add_ul_self(int_p num1, unsigned long num2);

int_p int_subtract(const int_p num1, const int_p num2);
void int_subtract_self(int_p num1, const int_p num2);

int_p int_subtract_ul(const int_p num1, unsigned long num2);
void int_subtract_ul_self(int_p num1, unsigned long num2);

int_p int_ul_subtract(unsigned long num1, const int_p num2);
void int_ul_subtract_self(unsigned long num1, int_p num2);

int_p int_multiply(const int_p num1, const int_p num2);
void int_multiply_self(int_p num1, const int_p num2);

int_p int_multiply_ul(const int_p num1, unsigned long num2);
void int_multiply_ul_self(int_p num1, unsigned long num2);

int_p int_modulo(const int_p num1, const int_p num2);
void int_modulo_self(int_p num1, const int_p num2);

int_p int_modulo_ul(const int_p num1, unsigned long num2);
void int_modulo_ul_self(int_p num1, unsigned long num2);

int_p int_ul_modulo(unsigned long num1, const int_p num2);
void int_ul_modulo_self(unsigned long num1, int_p num2);

int_p int_quotient(const int_p num1, const int_p num2);
void int_quotient_self(int_p num1, const int_p num2);

int_p int_quotient_ul(const int_p num1, unsigned long num2);
void int_quotient_ul_self(int_p num1, unsigned long num2);

int_p int_ul_quotient(unsigned long num1, const int_p num2);
void int_ul_quotient_self(unsigned long num1, int_p num2);

int_p int_power(const int_p num1, const int_p num2);
void int_power_self(int_p num1, const int_p num2);

int_p int_power_ul(const int_p num1, unsigned long num2);
void int_power_ul_self(int_p num1, unsigned long num2);

int_p int_ul_power(unsigned long num1, const int_p num2);
void int_ul_power_self(unsigned long num1, int_p num2);

int_p int_and(const int_p num1, const int_p num2);
void int_and_self(int_p num1, const int_p num2);

int_p int_and_ul(const int_p num1, unsigned long num2);
void int_and_ul_self(int_p num1, unsigned long num2);

int_p int_or(const int_p num1, const int_p num2);
void int_or_self(int_p num1, const int_p num2);

int_p int_or_ul(const int_p num1, unsigned long num2);
void int_or_ul_self(int_p num1, unsigned long num2);

int_p int_xor(const int_p num1, const int_p num2);
void int_xor_self(int_p num1, const int_p num2);

int_p int_xor_ul(const int_p num1, unsigned long num2);
void int_xor_ul_self(int_p num1, unsigned long num2);

int_p int_lshift(const int_p num1, const int_p num2);
void int_lshift_self(int_p num1, const int_p num2);

int_p int_lshift_ul(const int_p num1, unsigned long num2);
void int_lshift_ul_self(int_p num1, unsigned long num2);

int_p int_ul_lshift(unsigned long num1, const int_p num2);
void int_ul_lshift_self(unsigned long num1, int_p num2);

int_p int_rshift(const int_p num1, const int_p num2);
void int_rshift_self(int_p num1, const int_p num2);

int_p int_rshift_ul(const int_p num1, unsigned long num2);
void int_rshift_ul_self(int_p num1, unsigned long num2);

int_p int_ul_rshift(unsigned long num1, const int_p num2);
void int_ul_rshift_self(unsigned long num1, int_p num2);

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

int_p int_negate(const int_p num);
void int_negate_self(int_p num);

int_p int_not(const int_p num);
void int_not_self(int_p num);

/* */

char int_sign(const int_p num);

char int_fits_ull(const int_p num);
char int_fits_ul(const int_p num);

#endif /* __M_INT__ */
