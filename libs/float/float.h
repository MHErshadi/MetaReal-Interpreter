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

    unsigned long long ref;
};
typedef struct __float__ float_t;
typedef struct __float__* float_p;

unsigned long long float_prec_bit;
unsigned long long float_prec_show;

#define float_ref(num) (((float_p)(num))->ref)

float_p float_set(const float_p src);
float_p float_set_str(const char* str, unsigned long long size, unsigned char base);
float_p float_set_ul(unsigned long src);
float_p float_set_int(const int_p src);

char* float_get_str(const float_p src);
int_p float_get_int(const float_p src);

void float_free(float_p num);

void float_print(FILE* stream, const float_p num, const char* end);

/* */

float_p float_add(const float_p num1, const float_p num2);
float_p float_add_int(const float_p num1, const int_p num2);
float_p float_add_ul(const float_p num1, unsigned long num2);

float_p float_subtract(const float_p num1, const float_p num2);
float_p float_subtract_int(const float_p num1, const int_p num2);
float_p float_int_subtract(const int_p num1, const float_p num2);
float_p float_subtract_ul(const float_p num1, unsigned long num2);
float_p float_ul_subtract(unsigned long num1, const float_p num2);

float_p float_multiply(const float_p num1, const float_p num2);
float_p float_multiply_int(const float_p num1, const int_p num2);
float_p float_multiply_ul(const float_p num1, unsigned long num2);

float_p float_divide(const float_p num1, const float_p num2);
float_p float_divide_int(const float_p num1, const int_p num2);
float_p float_int_divide(const int_p num1, const float_p num2);
float_p float_int_divide_int(const int_p num1, const int_p num2);
float_p float_divide_ul(const float_p num1, unsigned long num2);
float_p float_ul_divide(unsigned long num1, const float_p num2);
float_p float_int_divide_ul(const int_p num1, unsigned long num2);
float_p float_ul_divide_int(unsigned long num1, const int_p num2);
float_p float_ul_divide_ul(unsigned long num1, unsigned long num2);

float_p float_modulo(const float_p num1, const float_p num2);
float_p float_modulo_int(const float_p num1, const int_p num2);
float_p float_int_modulo(const int_p num1, const float_p num2);
float_p float_modulo_ul(const float_p num1, unsigned long num2);
float_p float_ul_modulo(unsigned long num1, const float_p num2);

int_p float_quotient(const float_p num1, const float_p num2);
int_p float_quotient_int(const float_p num1, const int_p num2);
int_p float_int_quotient(const int_p num1, const float_p num2);
int_p float_quotient_ul(const float_p num1, unsigned long num2);
int_p float_ul_quotient(unsigned long num1, const float_p num2);

float_p float_power(const float_p num1, const float_p num2);
float_p float_power_int(const float_p num1, const int_p num2);
float_p float_int_power(const int_p num1, const float_p num2);
float_p float_int_power_int(const int_p num1, const int_p num2);
float_p float_power_ul(const float_p num1, unsigned long num2);
float_p float_ul_power(unsigned long num1, const float_p num2);
float_p float_ul_power_int(unsigned long num1, const int_p num2);

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

char float_less_equal(const float_p num1, const float_p num2);
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

float_p float_negate(const float_p num);

/* */

char float_sign(const float_p num);

#endif /* __M_FLOAT__ */
