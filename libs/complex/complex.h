/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
/*/

#ifndef __M_COMPLEX__
#define __M_COMPLEX__

#include <float.h>

#include <C:/MinGW/include/mpc.h>

struct _complex__
{
    mpc_t value;
};
typedef struct _complex__ complex_t;
typedef struct _complex__* complex_p;

unsigned long long complex_prec_bit;
unsigned long long complex_prec_show;

/* */

complex_p complex_set_str(const char* str1, unsigned long long size1,
    const char* str2, unsigned long long size2,
    unsigned char base);
complex_p complex_set_ul(unsigned long src);

char* complex_get_str(const complex_p src, unsigned char base);

void complex_free(const complex_p num);

void complex_print(FILE* stream, const complex_p num, const char* end);

/* */

complex_p complex_add(const complex_p num1, const complex_p num2);
void complex_add_self(complex_p num1, const complex_p num2);

complex_p complex_add_int(const complex_p num1, const int_p num2);
void complex_add_int_self(complex_p num1, const int_p num2);

complex_p complex_add_float(const complex_p num1, const float_p num2);
void complex_add_float_self(complex_p num1, const float_p num2);

complex_p complex_add_ul(const complex_p num1, unsigned long num2);
void complex_add_ul_self(complex_p num1, unsigned long num2);

complex_p complex_subtract(const complex_p num1, const complex_p num2);
void complex_subtract_self(complex_p num1, const complex_p num2);

complex_p complex_subtract_int(const complex_p num1, const int_p num2);
void complex_subtract_int_self(complex_p num1, const int_p num2);

complex_p complex_int_subtract(const int_p num1, const complex_p num2);
void complex_int_subtract_self(const int_p num1, complex_p num2);

complex_p complex_subtract_float(const complex_p num1, const float_p num2);
void complex_subtract_float_self(complex_p num1, const float_p num2);

complex_p complex_float_subtract(const float_p num1, const complex_p num2);
void complex_float_subtract_self(const float_p num1, complex_p num2);

complex_p complex_subtract_ul(const complex_p num1, unsigned long num2);
void complex_subtract_ul_self(complex_p num1, unsigned long num2);

complex_p complex_ul_subtract(unsigned long num1, const complex_p num2);
void complex_ul_subtract_self(unsigned long num1, complex_p num2);

complex_p complex_multiply(const complex_p num1, const complex_p num2);
void complex_multiply_self(complex_p num1, const complex_p num2);

complex_p complex_multiply_int(const complex_p num1, const int_p num2);
void complex_multiply_int_self(complex_p num1, const int_p num2);

complex_p complex_multiply_float(const complex_p num1, const float_p num2);
void complex_multiply_float_self(complex_p num1, const float_p num2);

complex_p complex_multiply_ul(const complex_p num1, unsigned long num2);
void complex_multiply_ul_self(complex_p num1, unsigned long num2);

complex_p complex_divide(const complex_p num1, const complex_p num2);
void complex_divide_self(complex_p num1, const complex_p num2);

complex_p complex_divide_int(const complex_p num1, const int_p num2);
void complex_divide_int_self(complex_p num1, const int_p num2);

complex_p complex_int_divide(const int_p num1, const complex_p num2);
void complex_int_divide_self(const int_p num1, complex_p num2);

complex_p complex_divide_float(const complex_p num1, const float_p num2);
void complex_divide_float_self(complex_p num1, const float_p num2);

complex_p complex_float_divide(const float_p num1, const complex_p num2);
void complex_float_divide_self(const float_p num1, complex_p num2);

complex_p complex_divide_ul(const complex_p num1, unsigned long num2);
void complex_divide_ul_self(complex_p num1, unsigned long num2);

complex_p complex_ul_divide(unsigned long num1, const complex_p num2);
void complex_ul_divide_self(unsigned long num1, complex_p num2);

complex_p complex_power(const complex_p num1, const complex_p num2);
void complex_power_self(complex_p num1, const complex_p num2);

complex_p complex_power_int(const complex_p num1, const int_p num2);
void complex_power_int_self(complex_p num1, const int_p num2);

complex_p complex_int_power(const int_p num1, const complex_p num2);
void complex_int_power_self(const int_p num1, complex_p num2);

complex_p complex_int_power_float(const int_p num1, const float_p num2);

complex_p complex_power_float(const complex_p num1, const float_p num2);
void complex_power_float_self(complex_p num1, const float_p num2);

complex_p complex_float_power(const float_p num1, const complex_p num2);
void complex_float_power_self(const float_p num1, complex_p num2);

complex_p complex_float_power_float(const float_p num1, const float_p num2);

complex_p complex_power_ul(const complex_p num1, unsigned long long num2);
void complex_power_ul_self(complex_p num1, unsigned long long num2);

complex_p complex_ul_power(unsigned long num1, const complex_p num2);
void complex_ul_power_self(unsigned long num1, complex_p num2);

complex_p complex_ul_power_float(unsigned long num1, const float_p num2);

char complex_equal(const complex_p num1, const complex_p num2);
char complex_equal_int(const complex_p num1, const int_p num2);
char complex_equal_float(const complex_p num1, const float_p num2);
char complex_equal_ul(const complex_p num1, unsigned long num2);

char complex_nequal(const complex_p num1, const complex_p num2);
char complex_nequal_int(const complex_p num1, const int_p num2);
char complex_nequal_float(const complex_p num1, const float_p num2);
char complex_nequal_ul(const complex_p num1, unsigned long num2);

/* */

complex_p complex_negate(const complex_p num);
void complex_negate_self(complex_p num);

/* */

char complex_is_zero(const complex_p num);

#endif /* __M_COMPLEX__ */
