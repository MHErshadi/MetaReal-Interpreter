/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
/*/

#ifndef __M_FLOAT__
#define __M_FLOAT__

#include <int.h>

#include <C:/MinGW/include/mpfr.h>

struct __float__
{
    mpfr_t value;
};
typedef struct __float__ float_t;
typedef struct __float__* float_p;

float_p float_set(const float_p src, unsigned long long prec);
float_p float_set_str(const char* str, unsigned long long size, unsigned long long prec);
float_p float_set_ul(unsigned long src, unsigned long long prec);

char* float_get_str(const float_p src, unsigned long long prec);
int_p float_get_int(const float_p src);

void float_free(float_p num);

void float_print(FILE* stream, const float_p num, unsigned long long prec, const char* end);

/* */

void float_add(float_p num1, const float_p num2);
void float_add_int(float_p num1, const int_p num2);
void float_add_ul(float_p num1, unsigned long num2);

void float_subtract(float_p num1, const float_p num2);
void float_subtract_int(float_p num1, const int_p num2);
void float_int_subtract(const int_p num1, float_p num2);
void float_subtract_ul(float_p num1, unsigned long num2);
void float_ul_subtract(unsigned long num1, float_p num2);

void float_multiply(float_p num1, const float_p num2);
void float_multiply_int(float_p num1, const int_p num2);
void float_multiply_ul(float_p num1, unsigned long num2);

void float_divide(float_p num1, const float_p num2);
void float_divide_int(float_p num1, const int_p num2);
void float_int_divide(const int_p num1, float_p num2);
float_p float_int_divide_int(const int_p num1, const int_p num2, unsigned long long prec);
void float_divide_ul(float_p num1, unsigned long num2);
void float_ul_divide(unsigned long num1, float_p num2);
float_p float_int_divide_ul(const int_p num1, unsigned long num2, unsigned long long prec);
float_p float_ul_divide_int(unsigned long num1, const int_p num2, unsigned long long prec);
float_p float_ul_divide_ul(unsigned long num1, unsigned long num2, unsigned long long prec);

void float_modulo(float_p num1, const float_p num2);
void float_modulo_int(float_p num1, const int_p num2);
void float_int_modulo(const int_p num1, float_p num2);
void float_modulo_ul(float_p num1, unsigned long num2);
void float_ul_modulo(unsigned long num1, float_p num2);

int_p float_quotient(float_p num1, const float_p num2);
void float_quotient_int(float_p num1, int_p num2);
void float_int_quotient(int_p num1, const float_p num2);
int_p float_quotient_ul(float_p num1, unsigned long num2);
int_p float_ul_quotient(unsigned long num1, float_p num2);

void float_power(float_p num1, const float_p num2);
void float_power_int(float_p num1, const int_p num2);
void float_int_power(const int_p num1, float_p num2);
float_p float_int_power_int(const int_p num1, const int_p num2, unsigned long long prec);
void float_power_ul(float_p num1, unsigned long num2);
void float_ul_power(unsigned long num1, float_p num2);
float_p float_ul_power_int(unsigned long num1, const int_p num2, unsigned long long prec);

char float_equal(const float_p num1, const float_p num2);
char float_equal_int(const float_p num1, const int_p num2);
char float_equal_ul(const float_p num1, unsigned long num2);

char float_nequal(const float_p num1, const float_p num2);
char float_nequal_int(const float_p num1, const int_p num2);
char float_nequal_ul(const float_p num1, unsigned long num2);

char float_less(const float_p num1, const float_p num2);
char float_less_int(const float_p num1, const int_p num2);
char float_int_less(const int_p num1, const float_p num2);
char float_less_ul(const float_p num1, unsigned long num2);
char float_ul_less(unsigned long num1, const float_p num2);

char float_greater(const float_p num1, const float_p num2);
char float_greater_int(const float_p num1, const int_p num2);
char float_int_greater(const int_p num1, const float_p num2);
char float_greater_ul(const float_p num1, unsigned long num2);
char float_ul_greater(unsigned long num1, const float_p num2);

char float_less_equal(const float_p num1, float_p num2);
char float_less_equal_int(const float_p num1, const int_p num2);
char float_int_less_equal(const int_p num1, const float_p num2);
char float_less_equal_ul(const float_p num1, unsigned long num2);
char float_ul_less_equal(unsigned long num1, const float_p num2);

char float_greater_equal(const float_p num1, const float_p num2);
char float_greater_equal_int(const float_p num1, const int_p num2);
char float_int_greater_equal(const int_p num1, const float_p num2);
char float_greater_equal_ul(const float_p num1, unsigned long num2);
char float_ul_greater_equal(unsigned long num1, const float_p num2);

/* */

void float_negate(float_p num);

/* */

char float_sign(float_p num);

#endif /* __M_FLOAT__ */
