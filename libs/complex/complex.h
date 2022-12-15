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

complex_p complex_set(const complex_p src, unsigned long long prec);
complex_p complex_set_str(const char* str, unsigned long long size, unsigned long long prec);

char* complex_get_str(const complex_p src, unsigned long long prec);

void complex_free(const complex_p num);

void complex_print(FILE* stream, const complex_p num, unsigned long long prec, const char* end);

/* */

void complex_add(complex_p num1, const complex_p num2);
void complex_add_int(complex_p num1, const int_p num2);
void complex_add_float(complex_p num1, const float_p num2);
void complex_add_ul(complex_p num1, unsigned long num2);

void complex_subtract(complex_p num1, const complex_p num2);
void complex_subtract_int(complex_p num1, const int_p num2);
void complex_int_subtract(const int_p num1, complex_p num2);
void complex_subtract_float(complex_p num1, const float_p num2);
void complex_float_subtract(const float_p num1, complex_p num2);
void complex_subtract_ul(complex_p num1, unsigned long num2);
void complex_ul_subtract(unsigned long num1, complex_p num2);

void complex_multiply(complex_p num1, const complex_p num2);
void complex_multiply_int(complex_p num1, const int_p num2);
void complex_multiply_float(complex_p num1, const float_p num2);
void complex_multiply_ul(complex_p num1, unsigned long num2);

void complex_divide(complex_p num1, const complex_p num2);
void complex_divide_int(complex_p num1, const int_p num2);
void complex_int_divide(const int_p num1, complex_p num2);
void complex_divide_float(complex_p num1, const float_p num2);
void complex_float_divide(const float_p num1, complex_p num2);
void complex_divide_ul(complex_p num1, unsigned long num2);
void complex_ul_divide(unsigned long num1, complex_p num2);

void complex_power(complex_p num1, const complex_p num2);
void complex_power_int(complex_p num1, const int_p num2);
void complex_int_power(const int_p num1, complex_p num2);
complex_p complex_int_power_float(const int_p num1, const float_p num2, unsigned long long prec);
void complex_power_float(complex_p num1, const float_p num2);
void complex_float_power(const float_p num1, complex_p num2);
complex_p complex_float_power_float(const float_p num1, const float_p num2, unsigned long long prec);
void complex_power_ul(complex_p num1, unsigned long long num2);
void complex_ul_power(unsigned long num1, complex_p num2);
complex_p complex_ul_power_float(unsigned long num1, const float_p num2, unsigned long long prec);

char complex_equal(const complex_p num1, const complex_p num2);
char complex_equal_int(const complex_p num1, const int_p num2);
char complex_equal_float(const complex_p num1, const float_p num2);
char complex_equal_ul(const complex_p num1, unsigned long num2);

char complex_nequal(const complex_p num1, const complex_p num2);
char complex_nequal_int(const complex_p num1, const int_p num2);
char complex_nequal_float(const complex_p num1, const float_p num2);
char complex_nequal_ul(const complex_p num1, unsigned long num2);

/* */

void complex_negate(complex_p num);

/* */

char complex_is_zero(complex_p num);

#endif /* __M_COMPLEX__ */
