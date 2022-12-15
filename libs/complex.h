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

complex_p complex_set(unsigned long long prec, complex_p src);
complex_p complex_set_str(unsigned long long prec, const char* str, unsigned long long size);

char* complex_get_str(complex_p src, unsigned long long prec);

void complex_free(complex_p num);

void complex_print(FILE* stream, complex_p num, unsigned long long prec, const char* end);

/* */

void complex_add(complex_p num1, complex_p num2);
void complex_add_int(complex_p num1, int_p num2);
void complex_add_float(complex_p num1, float_p num2);

void complex_subtract(complex_p num1, complex_p num2);
void complex_subtract_int(complex_p num1, int_p num2);
void complex_int_subtract(int_p num1, complex_p num2);
void complex_subtract_float(complex_p num1, float_p num2);
void complex_float_subtract(float_p num1, complex_p num2);

void complex_multiply(complex_p num1, complex_p num2);
void complex_multiply_int(complex_p num1, int_p num2);
void complex_multiply_float(complex_p num1, float_p num2);

void complex_divide(complex_p num1, complex_p num2);
void complex_divide_int(complex_p num1, int_p num2);
void complex_int_divide(int_p num1, complex_p num2);
void complex_divide_float(complex_p num1, float_p num2);
void complex_float_divide(float_p num1, complex_p num2);

void complex_power(complex_p num1, complex_p num2);
void complex_power_int(complex_p num1, int_p num2);
void complex_int_power(int_p num1, complex_p num2);
complex_p complex_int_power_float(int_p num1, float_p num2, unsigned long long prec);
void complex_power_float(complex_p num1, float_p num2);
void complex_float_power(float_p num1, complex_p num2);
complex_p complex_float_power_float(float_p num1, float_p num2, unsigned long long prec);

char complex_equal(complex_p num1, complex_p num2);
char complex_equal_int(complex_p num1, int_p num2);
char complex_equal_float(complex_p num1, float_p num2);

char complex_nequal(complex_p num1, complex_p num2);
char complex_nequal_int(complex_p num1, int_p num2);
char complex_nequal_float(complex_p num1, float_p num2);

/* */

void complex_negate(complex_p num);

/* */

char complex_is_zero(complex_p num);

#endif /* __M_COMPLEX__ */
