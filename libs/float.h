/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
/*/

#ifndef __M_FLOAT__
#define __M_FLOAT__

#include <int.h>

// temporary
#include <C:/MinGW/include/mpfr.h>

struct __float__
{
    mpfr_t value;
};
typedef struct __float__ float_t;
typedef struct __float__* float_p;

float_p float_set(unsigned long long prec, float_p src);
float_p float_set_str(unsigned long long prec, const char* str, unsigned long long size);

void float_free(float_p num);

void float_print(FILE* stream, float_p num, unsigned long long prec, const char* end);

/* */

void float_add(float_p num1, float_p num2);
void float_add_int(float_p num1, int_p num2);

void float_subtract(float_p num1, float_p num2);
void float_subtract_int(float_p num1, int_p num2);
void float_int_subtract(int_p num1, float_p num2);

void float_multiply(float_p num1, float_p num2);
void float_multiply_int(float_p num1, int_p num2);

void float_divide(float_p num1, float_p num2);
void float_divide_int(float_p num1, int_p num2);
void float_int_divide(int_p num1, float_p num2);
float_p float_int_divide_int(int_p num1, int_p num2, unsigned long long prec);

void float_modulo(float_p num1, float_p num2);
void float_modulo_int(float_p num1, int_p num2);
void float_int_modulo(int_p num1, float_p num2);

int_p float_quotient(float_p num1, float_p num2);
void float_quotient_int(float_p num1, int_p num2);
void float_int_quotient(int_p num1, float_p num2);

void float_power(float_p num1, float_p num2);
void float_power_int(float_p num1, int_p num2);
void float_int_power(int_p num1, float_p num2);
float_p float_int_power_int(int_p num1, int_p num2, unsigned long long prec);

char float_equal(float_p num1, float_p num2);
char float_equal_int(float_p num1, int_p num2);

char float_nequal(float_p num1, float_p num2);
char float_nequal_int(float_p num1, int_p num2);

char float_less(float_p num1, float_p num2);
char float_less_int(float_p num1, int_p num2);
char float_int_less(int_p num1, float_p num2);

char float_greater(float_p num1, float_p num2);
char float_greater_int(float_p num1, int_p num2);
char float_int_greater(int_p num1, float_p num2);

char float_less_equal(float_p num1, float_p num2);
char float_less_equal_int(float_p num1, int_p num2);
char float_int_less_equal(int_p num1, float_p num2);

char float_greater_equal(float_p num1, float_p num2);
char float_greater_equal_int(float_p num1, int_p num2);
char float_int_greater_equal(int_p num1, float_p num2);

/* */

// temporary
void float_negate(float_p num);

/* */

// temporary
char float_sign(float_p num);

#endif /* __M_FLOAT__ */
