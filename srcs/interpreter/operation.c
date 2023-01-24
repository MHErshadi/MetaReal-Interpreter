/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/interpreter.h>
#include <debugger/runtime_error.h>
#include <complex.h>
#include <str.h>
#include <array/list.h>
#include <structure/function.h>
#include <setting.h>
#include <stdlib.h>
#include <string.h>

#define check_none_type(op)                                              \
    {                                                                    \
        if (!left)                                                       \
        {                                                                \
            if (!right)                                                  \
                return ires_fail(illegal_operation(NONE_V, NONE_V, (op), \
                    poss, pose, context));                               \
                                                                         \
            rtype = right->type;                                         \
            value_free(right);                                           \
                                                                         \
            return ires_fail(illegal_operation(NONE_V, rtype, (op),      \
                poss, pose, context));                                   \
        }                                                                \
                                                                         \
        if (!right)                                                      \
        {                                                                \
            ltype = left->type;                                          \
            value_free(left);                                            \
                                                                         \
            return ires_fail(illegal_operation(ltype, NONE_V, (op),      \
                poss, pose, context));                                   \
        }                                                                \
    }

#define base_operation(t, f, l, r, rt)                                \
    {                                                                 \
        if ((l)->ref)                                                 \
        {                                                             \
            res = value_set1((t), f((l)->value.ptr, (r)->value.ptr)); \
                                                                      \
            (l)->ref--;                                               \
            value_free_type(r, rt);                                   \
                                                                      \
            return ires_success(res);                                 \
        }                                                             \
                                                                      \
        f##_self((l)->value.ptr, (r)->value.ptr);                     \
                                                                      \
        value_free_type(r, rt);                                       \
                                                                      \
        return ires_success(l);                                       \
    }

#define base_operation_rev(t, f, l, r, rt)                            \
    {                                                                 \
        if ((l)->ref)                                                 \
        {                                                             \
            res = value_set1((t), f((r)->value.ptr, (l)->value.ptr)); \
                                                                      \
            (l)->ref--;                                               \
            value_free_type(r, rt);                                   \
                                                                      \
            return ires_success(res);                                 \
        }                                                             \
                                                                      \
        f##_self((r)->value.ptr, (l)->value.ptr);                     \
                                                                      \
        value_free_type(r, rt);                                       \
                                                                      \
        return ires_success(l);                                       \
    }

#define base_operation_ul(t, f, l, r)                                      \
    {                                                                      \
        if ((l)->ref)                                                      \
        {                                                                  \
            res = value_set1((t), f##_ul((l)->value.ptr, (r)->value.chr)); \
                                                                           \
            (l)->ref--;                                                    \
            value_free_shell(r);                                           \
                                                                           \
            return ires_success(res);                                      \
        }                                                                  \
                                                                           \
        f##_ul_self((l)->value.ptr, (r)->value.chr);                       \
                                                                           \
        value_free_shell(r);                                               \
                                                                           \
        return ires_success(l);                                            \
    }

#define base_operation_ul_rev(t, f, l, r)                             \
    {                                                                 \
        if ((l)->ref)                                                 \
        {                                                             \
            res = value_set1((t), f((r)->value.chr, (l)->value.ptr)); \
                                                                      \
            (l)->ref--;                                               \
            value_free_shell(r);                                      \
                                                                      \
            return ires_success(res);                                 \
        }                                                             \
                                                                      \
        f##_self((r)->value.chr, (l)->value.ptr);                     \
                                                                      \
        value_free_shell(r);                                          \
                                                                      \
        return ires_success(l);                                       \
    }

#define base_operation_char(t, op, l, r)                   \
    {                                                      \
        if ((l)->ref)                                      \
        {                                                  \
            res_chr = (l)->value.chr op (r)->value.chr;    \
                                                           \
            (l)->ref--;                                    \
            value_free_shell(r);                           \
                                                           \
            return ires_success(value_set2((t), res_chr)); \
        }                                                  \
                                                           \
        (l)->value.chr op##= (r)->value.chr;               \
                                                           \
        value_free_shell(r);                               \
                                                           \
        return ires_success(l);                            \
    }

#define base_operation_char_rev(t, op, l, r)               \
    {                                                      \
        if ((l)->ref)                                      \
        {                                                  \
            res_chr = (r)->value.chr op (l)->value.chr;    \
                                                           \
            (l)->ref--;                                    \
            value_free_shell(r);                           \
                                                           \
            return ires_success(value_set2((t), res_chr)); \
        }                                                  \
                                                           \
        (l)->value.chr = (r)->value.chr op (l)->value.chr; \
                                                           \
        value_free_shell(r);                               \
                                                           \
        return ires_success(l);                            \
    }

#define base_unary_operation(t, f, o)                 \
    {                                                 \
        if ((o)->ref)                                 \
        {                                             \
            res = value_set1((t), f((o)->value.ptr)); \
                                                      \
            (o)->ref--;                               \
                                                      \
            return ires_success(res);                 \
        }                                             \
                                                      \
        f##_self((o)->value.ptr);                     \
                                                      \
        return ires_success(o);                       \
    }

#define base_unary_operation_char(t, op, o)                \
    {                                                      \
        if ((o)->ref)                                      \
        {                                                  \
            res_chr = op(o)->value.chr;                    \
                                                           \
            (o)->ref--;                                    \
                                                           \
            return ires_success(value_set2((t), res_chr)); \
        }                                                  \
                                                           \
        (o)->value.chr = op(o)->value.chr;                 \
                                                           \
        return ires_success(o);                            \
    }

ires_t operate_add(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type("+");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    char* str;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(INT_V, int_add, left, right, int);
        case FLOAT_V:
            base_operation(FLOAT_V, float_add_int, right, left, int);
        case COMPLEX_V:
            base_operation(COMPLEX_V, complex_add_int, right, left, int);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(INT_V, int_add, left, right);
        case STR_V:
            str = int_get_str(left->value.ptr, 10);

            value_free_type(left, int);

            if (right->ref)
            {
                res = value_set1(STR_V, str_str_concat(str, right->value.ptr));

                free(str);
                right->ref--;

                return ires_success(res);
            }

            str_str_concat_self(str, right->value.ptr);

            return ires_success(right);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(FLOAT_V, float_add_int, left, right, int);
        case FLOAT_V:
            base_operation(FLOAT_V, float_add, left, right, float);
        case COMPLEX_V:
            base_operation(COMPLEX_V, complex_add_float, right, left, float);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(FLOAT_V, float_add, left, right);
        case STR_V:
            str = float_get_str(left->value.ptr, 10);

            value_free_type(left, float);

            if (right->ref)
            {
                res = value_set1(STR_V, str_str_concat(str, right->value.ptr));

                free(str);
                right->ref--;

                return ires_success(res);
            }

            str_str_concat_self(str, right->value.ptr);

            return ires_success(right);
        }

        value_free_type(left, float);
        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(COMPLEX_V, complex_add_int, left, right, int);
        case FLOAT_V:
            base_operation(COMPLEX_V, complex_add_float, left, right, float);
        case COMPLEX_V:
            base_operation(COMPLEX_V, complex_add, right, left, complex);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(COMPLEX_V, complex_add, left, right);
        case STR_V:
            str = complex_get_str(left->value.ptr, 10);

            value_free_type(left, complex);

            if (right->ref)
            {
                res = value_set1(STR_V, str_str_concat(str, right->value.ptr));

                free(str);
                right->ref--;

                return ires_success(res);
            }

            str_str_concat_self(str, right->value.ptr);

            return ires_success(right);
        }

        value_free_type(left, complex);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul(INT_V, int_add, right, left);
        case FLOAT_V:
            base_operation_ul(FLOAT_V, float_add, right, left);
        case COMPLEX_V:
            base_operation_ul(COMPLEX_V, complex_add, right, left);
        case BOOL_V:
            base_operation_char(BOOL_V, ^, left, right);
        case CHAR_V:
            base_operation_char(CHAR_V, +, right, left);
        case STR_V:
            if (left->value.chr)
            {
                str = malloc(5);
                strcpy(str, "true");
            }
            else
            {
                str = malloc(6);
                strcpy(str, "false");
            }

            value_free_shell(left);

            if (right->ref)
            {
                res = value_set1(STR_V, str_str_concat(str, right->value.ptr));

                free(str);
                right->ref--;

                return ires_success(res);
            }

            str_str_concat_self(str, right->value.ptr);

            return ires_success(right);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul(INT_V, int_add, right, left);
        case FLOAT_V:
            base_operation_ul(FLOAT_V, float_add, right, left);
        case COMPLEX_V:
            base_operation_ul(COMPLEX_V, complex_add, right, left);
        case BOOL_V:
        case CHAR_V:
            base_operation_char(CHAR_V, +, left, right);
        case STR_V:
            if (right->ref)
            {
                res = value_set1(STR_V, str_char_concat(left->value.chr, right->value.ptr));

                value_free_shell(left);
                right->ref--;

                return ires_success(res);
            }

            str_char_concat_self(left->value.chr, right->value.ptr);

            value_free_shell(left);

            return ires_success(right);
        }

        value_free_shell(left);
        break;
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            str = int_get_str(right->value.ptr, 10);

            value_free_type(right, int);

            if (left->ref)
            {
                res = value_set1(STR_V, str_concat_str(left->value.ptr, str));

                free(str);
                left->ref--;

                return ires_success(res);
            }

            str_concat_str_self(left->value.ptr, str);

            return ires_success(left);
        case FLOAT_V:
            str = float_get_str(right->value.ptr, 10);

            value_free_type(right, float);

            if (left->ref)
            {
                res = value_set1(STR_V, str_concat_str(left->value.ptr, str));

                free(str);
                left->ref--;

                return ires_success(res);
            }

            str_concat_str_self(left->value.ptr, str);

            return ires_success(left);
        case COMPLEX_V:
            str = complex_get_str(right->value.ptr, 10);

            value_free_type(right, complex);

            if (left->ref)
            {
                res = value_set1(STR_V, str_concat_str(left->value.ptr, str));

                free(str);
                left->ref--;

                return ires_success(res);
            }

            str_concat_str_self(left->value.ptr, str);

            return ires_success(left);
        case BOOL_V:
            if (right->value.chr)
            {
                str = malloc(5);
                strcpy(str, "true");
            }
            else
            {
                str = malloc(6);
                strcpy(str, "false");
            }

            value_free_shell(right);

            if (left->ref)
            {
                res = value_set1(STR_V, str_concat_str(left->value.ptr, str));

                free(str);
                left->ref--;

                return ires_success(res);
            }

            str_concat_str_self(left->value.ptr, str);

            return ires_success(left);
        case CHAR_V:
            if (left->ref)
            {
                res = value_set1(STR_V, str_concat_char(left->value.ptr, right->value.chr));

                left->ref--;
                value_free_shell(right);

                return ires_success(res);
            }

            str_concat_char_self(left->value.ptr, right->value.chr);

            value_free_shell(right);

            return ires_success(left);
        case STR_V:
            base_operation(STR_V, str_concat, left, right, str);
        }

        value_free_type(left, str);
        break;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
            if (left->ref)
            {
                res = value_set1(LIST_V, list_concat(left->value.ptr, right->value.ptr));

                left->ref--;
                value_free_type(right, tuple);

                return ires_success(res);
            }

            if (!list_concat_self(left->value.ptr, right->value.ptr))
                value_free_type(right, tuple);

            return ires_success(left);
        case TUPLE_V:
            if (left->ref)
            {
                res = value_set1(LIST_V, list_concat_tuple(left->value.ptr, right->value.ptr));

                left->ref--;
                value_free_type(right, tuple);

                return ires_success(res);
            }

            if (!list_concat_tuple_self(left->value.ptr, right->value.ptr))
                value_free_type(right, tuple);

            return ires_success(left);
        }

        if (left->ref)
        {
            res = value_set1(LIST_V, list_append(left->value.ptr, right));

            left->ref--;

            return ires_success(res);
        }

        list_append_self(left->value.ptr, right);

        return ires_success(left);
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "+",
        poss, pose, context));
}

ires_t operate_subtract(value_p left, value_p right, pos_p poss, pos_p pose, context_p context, pos_p rposs, pos_p rpose)
{
    char ltype;
    char rtype;

    check_none_type("-");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(INT_V, int_subtract, left, right, int);
        case FLOAT_V:
            base_operation_rev(FLOAT_V, float_int_subtract, right, left, int);
        case COMPLEX_V:
            base_operation_rev(COMPLEX_V, complex_int_subtract, right, left, int);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(INT_V, int_subtract, left, right);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(FLOAT_V, float_subtract_int, left, right, int);
        case FLOAT_V:
            base_operation(FLOAT_V, float_subtract, left, right, float);
        case COMPLEX_V:
            base_operation_rev(COMPLEX_V, complex_float_subtract, right, left, float);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(FLOAT_V, float_subtract, left, right);
        }

        value_free_type(left, float);
        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(COMPLEX_V, complex_subtract_int, left, right, int);
        case FLOAT_V:
            base_operation(COMPLEX_V, complex_subtract_float, left, right, float);
        case COMPLEX_V:
            base_operation(COMPLEX_V, complex_subtract, left, right, complex);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(COMPLEX_V, complex_subtract, left, right);
        }

        value_free_type(left, complex);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul_rev(INT_V, int_ul_subtract, right, left);
        case FLOAT_V:
            base_operation_ul_rev(COMPLEX_V, float_ul_subtract, right, left);
        case COMPLEX_V:
            base_operation_ul_rev(COMPLEX_V, complex_ul_subtract, right, left);
        case BOOL_V:
            base_operation_char(BOOL_V, ^, left, right);
        case CHAR_V:
            base_operation_char_rev(CHAR_V, -, right, left);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul_rev(INT_V, int_ul_subtract, right, left);
        case FLOAT_V:
            base_operation_ul_rev(COMPLEX_V, float_ul_subtract, right, left);
        case COMPLEX_V:
            base_operation_ul_rev(COMPLEX_V, complex_ul_subtract, right, left);
        case BOOL_V:
        case CHAR_V:
            base_operation_char(CHAR_V, -, left, right);
        }

        value_free_shell(left);
        break;
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                value_free_type(left, str);
                value_free_type(right, int);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            unsigned long long index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = str_size(left->value.ptr) - index;

            value_free_type(right, int);

            if (index >= str_size(left->value.ptr))
            {
                value_free_type(left, str);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            if (left->ref)
            {
                res = value_set1(STR_V, str_remove(left->value.ptr, index));

                left->ref--;

                return ires_success(res);
            }

            str_remove_self(left->value.ptr, index);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= str_size(left->value.ptr))
            {
                value_free_type(left, str);
                value_free_shell(right);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            if (left->ref)
            {
                res = value_set1(STR_V, str_remove(left->value.ptr, right->value.chr));

                left->ref--;
                value_free_shell(right);

                return ires_success(res);
            }

            str_remove_self(left->value.ptr, right->value.chr);

            value_free_shell(right);

            return ires_success(left);
        }

        value_free_type(left, str);
        break;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                value_free_type(left, list);
                value_free_type(right, int);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            unsigned long long index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = list_size(left->value.ptr) - index;

            value_free_type(right, int);

            if (index >= list_size(left->value.ptr))
            {
                value_free_type(left, list);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            if (left->ref)
            {
                res = value_set1(LIST_V, list_remove(left->value.ptr, index));

                left->ref--;

                return ires_success(res);
            }

            list_remove_self(left->value.ptr, index);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= list_size(left->value.ptr))
            {
                value_free_type(left, list);
                value_free_shell(right);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            if (left->ref)
            {
                res = value_set1(LIST_V, list_remove(left->value.ptr, right->value.chr));

                left->ref--;
                value_free_shell(right);

                return ires_success(res);
            }

            list_remove_self(left->value.ptr, right->value.chr);

            value_free_shell(right);

            return ires_success(left);
        }

        value_free_type(left, list);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "-",
        poss, pose, context));
}

ires_t operate_multiply(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type("*");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(INT_V, int_multiply, left, right, int);
        case FLOAT_V:
            base_operation(FLOAT_V, float_multiply_int, right, left, int);
        case COMPLEX_V:
            base_operation(COMPLEX_V, complex_multiply_int, right, left, int);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(INT_V, int_multiply, left, right);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(FLOAT_V, float_multiply_int, left, right, int);
        case FLOAT_V:
            base_operation(FLOAT_V, float_multiply, left, right, float);
        case COMPLEX_V:
            base_operation(COMPLEX_V, complex_multiply_float, right, left, float);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(FLOAT_V, float_multiply, left, right);
        }

        value_free_type(left, float);
        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(COMPLEX_V, complex_multiply_int, left, right, int);
        case FLOAT_V:
            base_operation(COMPLEX_V, complex_multiply_float, left, right, float);
        case COMPLEX_V:
            base_operation(COMPLEX_V, complex_multiply, left, right, complex);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(COMPLEX_V, complex_multiply, left, right);
        }

        value_free_type(left, complex);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul(INT_V, int_multiply, right, left);
        case FLOAT_V:
            base_operation_ul(FLOAT_V, float_multiply, right, left);
        case COMPLEX_V:
            base_operation_ul(COMPLEX_V, complex_multiply, right, left);
        case BOOL_V:
            base_operation_char(BOOL_V, &, left, right);
        case CHAR_V:
            base_operation_char(CHAR_V, *, right, left);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul(INT_V, int_multiply, right, left);
        case FLOAT_V:
            base_operation_ul(FLOAT_V, float_multiply, right, left);
        case COMPLEX_V:
            base_operation_ul(COMPLEX_V, complex_multiply, right, left);
        case BOOL_V:
        case CHAR_V:
            base_operation_char(CHAR_V, *, left, right);
        }

        value_free_shell(left);
        break;
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                value_free_type(left, str);
                value_free_type(right, int);

                return ires_fail(mem_overflow(poss, pose, context));
            }

            unsigned long long count = int_get_ull(right->value.ptr);

            if (left->ref)
            {
                left->ref--;

                if (int_sign(right->value.ptr) < 0)
                {
                    str_p rev = str_reverse(left->value.ptr);

                    value_free_type(right, int);

                    str_repeat_self(rev, count);
                    res = value_set1(STR_V, rev);

                    return ires_success(res);
                }

                value_free_type(right, int);

                res = value_set1(STR_V, str_repeat(left->value.ptr, count));

                return ires_success(res);
            }

            if (int_sign(right->value.ptr) < 0)
                str_reverse_self(left->value.ptr);

            value_free_type(right, int);

            str_repeat_self(left->value.ptr, count);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            if (left->ref)
            {
                res = value_set1(STR_V, str_repeat(left->value.ptr, right->value.chr));

                left->ref--;
                value_free_shell(right);

                return ires_success(res);
            }

            str_repeat_self(left->value.ptr, right->value.chr);

            value_free_shell(right);

            return ires_success(left);
        }

        value_free_type(left, str);
        break;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                value_free_type(left, list);
                value_free_type(right, int);

                return ires_fail(mem_overflow(poss, pose, context));
            }

            unsigned long long count = int_get_ull(right->value.ptr);

            if (left->ref)
            {
                left->ref--;

                if (int_sign(right->value.ptr) < 0)
                {
                    value_free_type(right, int);

                    list_p rev = list_reverse(left->value.ptr);

                    list_repeat_self(rev, count);
                    res = value_set1(STR_V, rev);

                    return ires_success(res);
                }

                value_free_type(right, int);

                res = value_set1(STR_V, list_repeat(left->value.ptr, count));

                return ires_success(res);
            }

            if (int_sign(right->value.ptr) < 0)
                list_reverse_self(left->value.ptr);

            value_free_type(right, int);

            list_repeat_self(left->value.ptr, count);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            if (left->ref)
            {
                res = value_set1(LIST_V, list_repeat(left->value.ptr, right->value.chr));

                left->ref--;
                value_free_shell(right);

                return ires_success(res);
            }

            list_repeat_self(left->value.ptr, right->value.chr);

            value_free_shell(right);

            return ires_success(left);
        }

        value_free_type(left, list);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "*",
        poss, pose, context));
}

ires_t operate_divide(value_p left, value_p right, pos_p poss, pos_p pose, context_p context, pos_p rposs, pos_p rpose)
{
    char ltype;
    char rtype;

    check_none_type("/");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_type(left, int);
                value_free_type(right, int);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            res = value_set1(FLOAT_V, float_int_divide_int(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_type(left, int);
                value_free_type(right, float);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_rev(FLOAT_V, float_int_divide, right, left, int);
        case COMPLEX_V:
            if (complex_is_zero(right->value.ptr))
            {
                value_free_type(left, int);
                value_free_type(right, complex);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_rev(COMPLEX_V, complex_int_divide, right, left, int);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_type(left, int);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_type(left, int);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            res = value_set1(FLOAT_V, float_int_divide_ul(left->value.ptr, right->value.chr));

            value_free_type(left, int);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_type(left, float);
                value_free_type(right, int);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation(FLOAT_V, float_divide_int, left, right, int);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_type(left, float);
                value_free_type(right, float);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation(FLOAT_V, float_divide, left, right, float);
        case COMPLEX_V:
            if (complex_is_zero(right->value.ptr))
            {
                value_free_type(left, float);
                value_free_type(right, complex);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_rev(COMPLEX_V, complex_float_divide, right, left, float);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_type(left, float);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_type(left, float);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_ul(FLOAT_V, float_divide, left, right);
        }

        value_free_type(left, float);
        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_type(left, complex);
                value_free_type(right, int);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation(COMPLEX_V, complex_divide_int, left, right, int);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_type(left, complex);
                value_free_type(right, float);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation(COMPLEX_V, complex_divide_float, left, right, float);
        case COMPLEX_V:
            if (complex_is_zero(right->value.ptr))
            {
                value_free_type(left, complex);
                value_free_type(right, complex);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation(COMPLEX_V, complex_divide, left, right, complex);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_type(left, complex);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_type(left, complex);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_ul(COMPLEX_V, complex_divide, left, right);
        }

        value_free_type(left, complex);
        break;
    case BOOL_V:
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, int);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            res = value_set1(FLOAT_V, float_ul_divide_int(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, float);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_ul_rev(FLOAT_V, float_ul_divide, right, left);
        case COMPLEX_V:
            if (complex_is_zero(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, complex);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_ul_rev(COMPLEX_V, complex_ul_divide, right, left);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_shell(left);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_shell(left);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            res = value_set1(FLOAT_V, float_ul_divide_ul(left->value.chr, right->value.chr));

            value_free_shell(left);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "/",
        poss, pose, context));
}

ires_t operate_modulo(value_p left, value_p right, pos_p poss, pos_p pose, context_p context, pos_p rposs, pos_p rpose)
{
    char ltype;
    char rtype;

    check_none_type("%");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_type(left, int);
                value_free_type(right, int);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation(INT_V, int_modulo, left, right, int);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_type(left, int);
                value_free_type(right, float);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation_rev(FLOAT_V, float_int_modulo, right, left, int);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_type(left, int);
                value_free_shell(right);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            res = value_set1(INT_V, int_set_ull(0));

            value_free_type(left, int);
            value_free_shell(right);

            return ires_success(res);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_type(left, int);
                value_free_shell(right);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation_ul(INT_V, int_modulo, left, right);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_type(left, float);
                value_free_type(right, int);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation(FLOAT_V, float_modulo_int, left, right, int);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_type(left, float);
                value_free_type(right, float);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation(FLOAT_V, float_modulo, left, right, float);
        case BOOL_V:
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_type(left, float);
                value_free_shell(right);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation_ul(FLOAT_V, float_modulo, left, right);
        }

        value_free_type(left, float);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, int);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation_ul_rev(INT_V, int_ul_modulo, right, left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, float);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation_ul_rev(FLOAT_V, float_ul_modulo, right, left);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_shell(left);
                value_free_shell(right);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            value_free_shell(right);

            if (left->ref)
            {
                res = value_set2(BOOL_V, 0);

                left->ref--;
            }
            else
            {
                left->value.chr = 0;
                res = left;
            }

            return ires_success(res);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_shell(left);
                value_free_shell(right);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation_char_rev(CHAR_V, %, right, left);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, int);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation_ul_rev(INT_V, int_ul_modulo, right, left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, float);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation_ul_rev(FLOAT_V, float_ul_modulo, right, left);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_shell(left);
                value_free_shell(right);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            value_free_shell(right);

            if (left->ref)
            {
                res = value_set2(CHAR_V, 0);

                left->ref--;
            }
            else
            {
                left->value.chr = 0;
                res = left;
            }

            return ires_success(res);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_shell(left);
                value_free_shell(right);

                return ires_fail(modulo_by_zero(rposs, rpose, context));
            }

            base_operation_char(CHAR_V, %, left, right);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "%",
        poss, pose, context));
}

ires_t operate_quotient(value_p left, value_p right, pos_p poss, pos_p pose, context_p context, pos_p rposs, pos_p rpose)
{
    char ltype;
    char rtype;

    check_none_type("//");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_type(left, int);
                value_free_type(right, int);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation(INT_V, int_quotient, left, right, int);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_type(left, int);
                value_free_type(right, float);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation(INT_V, float_int_quotient, left, right, float);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_type(left, int);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_type(left, int);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_ul(INT_V, int_quotient, left, right);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_type(left, float);
                value_free_type(right, int);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_rev(INT_V, float_quotient_int, right, left, float);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_type(left, float);
                value_free_type(right, float);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            res = value_set1(INT_V, float_quotient(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, float);

            return ires_success(res);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_type(left, float);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            res = value_set1(INT_V, float_quotient_ul(left->value.ptr, 1));

            value_free_type(left, float);
            value_free_shell(right);

            return ires_success(res);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_type(left, float);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            res = value_set1(INT_V, float_quotient_ul(left->value.ptr, right->value.chr));

            value_free_type(left, float);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, float);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, int);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_ul_rev(INT_V, int_ul_quotient, right, left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, float);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            res = value_set1(INT_V, float_ul_quotient(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, float);

            return ires_success(res);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_shell(left);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_shell(left);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_char_rev(CHAR_V, /, right, left);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, int);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_ul_rev(INT_V, int_ul_quotient, right, left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, float);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            res = value_set1(INT_V, float_ul_quotient(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, float);

            return ires_success(res);
        case BOOL_V:
            if (!right->value.chr)
            {
                value_free_shell(left);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                value_free_shell(left);
                value_free_shell(right);

                return ires_fail(division_by_zero(rposs, rpose, context));
            }

            base_operation_char(CHAR_V, /, left, right);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "//",
        poss, pose, context));
}

ires_t operate_power(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type("**");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_sign(right->value.ptr) < 0)
            {
                res = value_set1(FLOAT_V, float_int_power_int(left->value.ptr, right->value.ptr));

                value_free_type(left, int);
                value_free_type(right, int);

                return ires_success(res);
            }

            if (!int_fits_ul(right->value.ptr))
            {
                value_free_type(left, int);
                value_free_type(right, int);

                return ires_fail(mem_overflow(poss, pose, context));
            }

            base_operation(INT_V, int_power, left, right, int);
        case FLOAT_V:
            if (int_sign(left->value.ptr) < 0)
            {
                res = value_set1(COMPLEX_V, complex_int_power_float(left->value.ptr, right->value.ptr));

                value_free_type(left, int);
                value_free_type(right, float);

                return ires_success(res);
            }

            base_operation_rev(FLOAT_V, float_int_power, right, left, int);
        case COMPLEX_V:
            base_operation_rev(COMPLEX_V, complex_int_power, right, left, int);
        case BOOL_V:
            if (!right->value.chr)
            {
                res = value_set1(INT_V, int_set_ull(1));

                value_free_type(left, int);
                value_free_shell(right);

                return ires_success(res);
            }

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            base_operation_ul(INT_V, int_power, left, right);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(FLOAT_V, float_power_int, left, right, int);
        case FLOAT_V:
            if (float_sign(left->value.ptr) < 0)
            {
                res = value_set1(COMPLEX_V, complex_float_power_float(left->value.ptr, right->value.ptr));

                value_free_type(left, float);
                value_free_type(right, float);

                return ires_success(res);
            }

            base_operation(FLOAT_V, float_power, left, right, float);
        case COMPLEX_V:
            base_operation_rev(COMPLEX_V, complex_float_power, right, left, float);
        case BOOL_V:
            if (!right->value.chr)
            {
                res = value_set1(FLOAT_V, float_set_ul(1));

                value_free_type(left, float);
                value_free_shell(right);

                return ires_success(res);
            }

            return ires_success(left);
        case CHAR_V:
            base_operation_ul(FLOAT_V, float_power, left, right);
        }

        value_free_type(left, float);
        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(COMPLEX_V, complex_power_int, left, right, int);
        case FLOAT_V:
            base_operation(COMPLEX_V, complex_power_float, left, right, float);
        case COMPLEX_V:
            base_operation(COMPLEX_V, complex_power, left, right, complex);
        case BOOL_V:
            if (!right->value.chr)
            {
                res = value_set1(COMPLEX_V, complex_set_ul(1));

                value_free_type(left, complex);
                value_free_shell(right);

                return ires_success(res);
            }

            return ires_success(left);
        case CHAR_V:
            base_operation_ul(COMPLEX_V, complex_power, left, right);
        }

        value_free_type(left, complex);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_sign(right->value.ptr) < 0)
            {
                res = value_set1(FLOAT_V, float_ul_power_int(left->value.chr, right->value.ptr));

                value_free_shell(left);
                value_free_type(right, int);

                return ires_success(res);
            }

            base_operation_ul_rev(INT_V, int_ul_power, right, left);
        case FLOAT_V:
            base_operation_ul_rev(FLOAT_V, float_ul_power, right, left);
        case COMPLEX_V:
            base_operation_ul_rev(COMPLEX_V, complex_ul_power, right, left);
        case BOOL_V:
            if (!right->value.chr)
            {
                if (left->ref)
                {
                    res = value_set2(BOOL_V, 1);

                    left->ref--;
                    value_free_shell(right);

                    return ires_success(res);
                }

                left->value.chr = 1;

                value_free_shell(right);

                return ires_success(left);
            }

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            res_chr = 1;

            unsigned char i;
            for (i = 0; i < right->value.chr; i++)
                res_chr *= left->value.chr;

            value_free_shell(right);

            if (left->ref)
            {
                left->ref--;

                return ires_success(value_set2(CHAR_V, res_chr));
            }

            left->type = CHAR_V;
            left->value.chr = res_chr;

            return ires_success(left);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (int_sign(right->value.ptr) < 0)
            {
                res = value_set1(FLOAT_V, float_ul_power_int(left->value.chr, right->value.ptr));

                value_free_shell(left);
                value_free_type(right, int);

                return ires_success(res);
            }

            base_operation_ul_rev(INT_V, int_ul_power, right, left);
        case FLOAT_V:
            base_operation_ul_rev(FLOAT_V, float_ul_power, right, left);
        case COMPLEX_V:
            base_operation_ul_rev(COMPLEX_V, complex_ul_power, right, left);
        case BOOL_V:
            if (!right->value.chr)
            {
                if (left->ref)
                {
                    res = value_set2(CHAR_V, 1);

                    left->ref--;
                    value_free_shell(right);

                    return ires_success(res);
                }

                left->value.chr = 1;

                value_free_shell(right);

                return ires_success(left);
            }

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            res_chr = 1;

            unsigned char i;
            for (i = 0; i < right->value.chr; i++)
                res_chr *= left->value.chr;

            value_free_shell(right);

            if (left->ref)
            {
                left->ref--;

                return ires_success(value_set2(CHAR_V, res_chr));
            }

            left->value.chr = res_chr;

            return ires_success(left);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "**",
        poss, pose, context));
}

ires_t operate_b_and(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type("&");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(INT_V, int_and, left, right, int);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(INT_V, int_and, left, right);
        }

        value_free_type(left, int);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul(INT_V, int_and, right, left);
        case BOOL_V:
            base_operation_char(BOOL_V, &, left, right);
        case CHAR_V:
            base_operation_char(CHAR_V, &, right, left);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul(INT_V, int_and, right, left);
        case BOOL_V:
        case CHAR_V:
            base_operation_char(CHAR_V, &, left, right);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "&",
        poss, pose, context));
}

ires_t operate_b_or(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type("|");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(INT_V, int_or, left, right, int);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(INT_V, int_or, left, right);
        }

        value_free_type(left, int);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul(INT_V, int_or, right, left);
        case BOOL_V:
            base_operation_char(BOOL_V, |, left, right);
        case CHAR_V:
            base_operation_char(CHAR_V, |, right, left);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul(INT_V, int_or, right, left);
        case BOOL_V:
        case CHAR_V:
            base_operation_char(CHAR_V, |, left, right);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "|",
        poss, pose, context));
}

ires_t operate_b_xor(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type("^");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            base_operation(INT_V, int_xor, left, right, int);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(INT_V, int_xor, left, right);
        }

        value_free_type(left, int);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul(INT_V, int_xor, right, left);
        case BOOL_V:
            base_operation_char(BOOL_V, ^, left, right);
        case CHAR_V:
            base_operation_char(CHAR_V, ^, right, left);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            base_operation_ul(INT_V, int_xor, right, left);
        case BOOL_V:
        case CHAR_V:
            base_operation_char(CHAR_V, ^, left, right);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "^",
        poss, pose, context));
}

ires_t operate_lshift(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type("<<");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                value_free_type(left, int);
                value_free_type(right, int);

                return ires_fail(mem_overflow(poss, pose, context));
            }

            base_operation(INT_V, int_lshift, left, right, int);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(INT_V, int_lshift, left, right);
        }

        value_free_type(left, int);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, int);

                return ires_fail(mem_overflow(poss, pose, context));
            }

            base_operation_ul_rev(INT_V, int_ul_lshift, right, left);
        case BOOL_V:
            if (left->ref)
            {
                res_chr = left->value.chr & ~right->value.chr;

                left->ref--;
                value_free_shell(right);

                return ires_success(value_set2(BOOL_V, res_chr));
            }

            left->value.chr &= ~right->value.chr;

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            base_operation_char_rev(CHAR_V, <<, right, left);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, int);

                return ires_fail(mem_overflow(poss, pose, context));
            }

            base_operation_ul_rev(INT_V, int_ul_lshift, right, left);
        case BOOL_V:
        case CHAR_V:
            base_operation_char(CHAR_V, <<, left, right);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "<<",
        poss, pose, context));
}

ires_t operate_rshift(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type(">>");

    ltype = left->type;
    rtype = right->type;

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                value_free_type(left, int);
                value_free_type(right, int);

                return ires_fail(mem_overflow(poss, pose, context));
            }

            base_operation(INT_V, int_rshift, left, right, int);
        case BOOL_V:
        case CHAR_V:
            base_operation_ul(INT_V, int_rshift, left, right);
        }

        value_free_type(left, int);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, int);

                return ires_fail(mem_overflow(poss, pose, context));
            }

            base_operation_ul_rev(INT_V, int_ul_rshift, right, left);
        case BOOL_V:
            if (left->ref)
            {
                res_chr = left->value.chr & ~right->value.chr;

                left->ref--;
                value_free_shell(right);

                return ires_success(value_set2(BOOL_V, res_chr));
            }

            left->value.chr &= ~right->value.chr;

            value_free_shell(right);

            return ires_success(left);
        case CHAR_V:
            base_operation_char_rev(CHAR_V, >>, right, left);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                value_free_shell(left);
                value_free_type(right, int);

                return ires_fail(mem_overflow(poss, pose, context));
            }

            base_operation_ul_rev(INT_V, int_ul_rshift, right, left);
        case BOOL_V:
        case CHAR_V:
            base_operation_char(CHAR_V, >>, left, right);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, ">>",
        poss, pose, context));
}

ires_t operate_equal(value_p left, value_p right)
{
    if (!left)
    {
        if (!right)
            return ires_success(value_set2(BOOL_V, 1));

        value_free(right);
        return ires_success(value_set2(BOOL_V, 0));
    }

    if (!right)
    {
        value_free(left);
        return ires_success(value_set2(BOOL_V, 0));
    }

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case OBJECT_V:
        res = value_set2(BOOL_V, right->type == OBJECT_V && left->value.ptr == right->value.ptr);

        value_free_shell(left);
        value_free(right);

        return ires_success(res);
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_equal_int(right->value.ptr, left->value.ptr));

            value_free_type(left, int);
            value_free_type(right, float);

            return ires_success(res);
        case COMPLEX_V:
            res = value_set2(BOOL_V, complex_equal_int(right->value.ptr, left->value.ptr));

            value_free_type(left, int);
            value_free_type(right, complex);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, int_equal_ul(left->value.ptr, right->value.chr));

            value_free_type(left, int);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, float_equal_int(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, float);

            return ires_success(res);
        case COMPLEX_V:
            res = value_set2(BOOL_V, complex_equal_float(right->value.ptr, left->value.ptr));

            value_free_type(left, float);
            value_free_type(right, complex);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, float_equal_ul(left->value.ptr, right->value.chr));

            value_free_type(left, float);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, float);
        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, complex_equal_int(left->value.ptr, right->value.ptr));

            value_free_type(left, complex);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, complex_equal_float(left->value.ptr, right->value.ptr));

            value_free_type(left, complex);
            value_free_type(right, float);

            return ires_success(res);
        case COMPLEX_V:
            res = value_set2(BOOL_V, complex_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, complex);
            value_free_type(right, complex);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, complex_equal_ul(left->value.ptr, right->value.chr));

            value_free_type(left, complex);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, complex);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_equal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_equal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, float);

            return ires_success(res);
        case COMPLEX_V:
            res = value_set2(BOOL_V, complex_equal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, complex);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            res = value_set2(BOOL_V, left->value.chr == right->value.chr);

            value_free_shell(left);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_equal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_equal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, float);

            return ires_success(res);
        case COMPLEX_V:
            res = value_set2(BOOL_V, complex_equal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, complex);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            res = value_set2(BOOL_V, left->value.chr == right->value.chr);

            value_free_shell(left);
            value_free_shell(right);

            return ires_success(res);
        case STR_V:
            res = value_set2(BOOL_V, str_equal_char(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, str);

            return ires_success(res);
        }

        value_free_shell(left);
        break;
    case STR_V:
        switch (right->type)
        {
        case CHAR_V:
            res = value_set2(BOOL_V, str_equal_char(left->value.ptr, right->value.chr));

            value_free_type(left, str);
            value_free_shell(right);

            return ires_success(res);
        case STR_V:
            res = value_set2(BOOL_V, str_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, str);
            value_free_type(right, str);

            return ires_success(res);
        }

        value_free_type(left, str);
        break;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
            res = value_set2(BOOL_V, list_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, list);
            value_free_type(right, list);

            return ires_success(res);
        case TUPLE_V:
            res = value_set2(BOOL_V, list_equal_tuple(left->value.ptr, right->value.ptr));

            value_free_type(left, list);
            value_free_type(right, tuple);

            return ires_success(res);
        }

        value_free_type(left, list);
        break;
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
            res = value_set2(BOOL_V, list_equal_tuple(right->value.ptr, left->value.ptr));

            value_free_type(left, tuple);
            value_free_type(right, list);

            return ires_success(res);
        case TUPLE_V:
            res = value_set2(BOOL_V, tuple_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, tuple);
            value_free_type(right, tuple);

            return ires_success(res);
        }

        value_free_type(left, tuple);
        break;
    case TYPE_V:
        res = value_set2(BOOL_V, right->type == TYPE_V && left->value.chr == right->value.chr);

        value_free_shell(left);
        value_free(right);

        return ires_success(res);
    case FUNC_V:
        res = value_set2(BOOL_V, left->value.ptr == right->value.ptr);

        value_free_type(left, func);
        value_free(right);

        return ires_success(res);
    case STRUCT_V:
        res = value_set2(BOOL_V, left->value.ptr == right->value.ptr);

        value_free_type(left, context);
        value_free(right);

        return ires_success(res);
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_success(value_set2(BOOL_V, 0));
}

ires_t operate_nequal(value_p left, value_p right)
{
    if (!left)
    {
        if (!right)
            return ires_success(value_set2(BOOL_V, 0));

        value_free(right);
        return ires_success(value_set2(BOOL_V, 1));
    }

    if (!right)
    {
        value_free(left);
        return ires_success(value_set2(BOOL_V, 1));
    }

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case OBJECT_V:
        res = value_set2(BOOL_V, right->type != OBJECT_V || left->value.ptr != right->value.ptr);

        value_free_shell(left);
        value_free(right);

        return ires_success(res);
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_nequal(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_nequal_int(right->value.ptr, left->value.ptr));

            value_free_type(left, int);
            value_free_type(right, float);

            return ires_success(res);
        case COMPLEX_V:
            res = value_set2(BOOL_V, complex_nequal_int(right->value.ptr, left->value.ptr));

            value_free_type(left, int);
            value_free_type(right, complex);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, int_nequal_ul(left->value.ptr, right->value.chr));

            value_free_type(left, int);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, float_nequal_int(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_nequal(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, float);

            return ires_success(res);
        case COMPLEX_V:
            res = value_set2(BOOL_V, complex_nequal_float(right->value.ptr, left->value.ptr));

            value_free_type(left, float);
            value_free_type(right, complex);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, float_nequal_ul(left->value.ptr, right->value.chr));

            value_free_type(left, float);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, float);
        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, complex_nequal_int(left->value.ptr, right->value.ptr));

            value_free_type(left, complex);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, complex_nequal_float(left->value.ptr, right->value.ptr));

            value_free_type(left, complex);
            value_free_type(right, float);

            return ires_success(res);
        case COMPLEX_V:
            res = value_set2(BOOL_V, complex_nequal(left->value.ptr, right->value.ptr));

            value_free_type(left, complex);
            value_free_type(right, complex);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, complex_nequal_ul(left->value.ptr, right->value.chr));

            value_free_type(left, complex);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, complex);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_nequal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_nequal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, float);

            return ires_success(res);
        case COMPLEX_V:
            res = value_set2(BOOL_V, complex_nequal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, complex);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            res = value_set2(BOOL_V, left->value.chr != right->value.chr);

            value_free_shell(left);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_shell(left);
        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_nequal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_nequal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, float);

            return ires_success(res);
        case COMPLEX_V:
            res = value_set2(BOOL_V, complex_nequal_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, complex);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            res = value_set2(BOOL_V, left->value.chr != right->value.chr);

            value_free_shell(left);
            value_free_shell(right);

            return ires_success(res);
        case STR_V:
            res = value_set2(BOOL_V, str_nequal_char(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, str);

            return ires_success(res);
        }

        value_free_shell(left);
        break;
    case STR_V:
        switch (right->type)
        {
        case CHAR_V:
            res = value_set2(BOOL_V, str_nequal_char(left->value.ptr, right->value.chr));

            value_free_type(left, str);
            value_free_shell(right);

            return ires_success(res);
        case STR_V:
            res = value_set2(BOOL_V, str_nequal(left->value.ptr, right->value.ptr));

            value_free_type(left, str);
            value_free_type(right, str);

            return ires_success(res);
        }

        value_free_type(left, str);
        break;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
            res = value_set2(BOOL_V, list_nequal(left->value.ptr, right->value.ptr));

            value_free_type(left, list);
            value_free_type(right, list);

            return ires_success(res);
        case TUPLE_V:
            res = value_set2(BOOL_V, list_nequal_tuple(left->value.ptr, right->value.ptr));

            value_free_type(left, list);
            value_free_type(right, tuple);

            return ires_success(res);
        }

        value_free_type(left, list);
        break;
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
            res = value_set2(BOOL_V, list_nequal_tuple(right->value.ptr, left->value.ptr));

            value_free_type(left, tuple);
            value_free_type(right, list);

            return ires_success(res);
        case TUPLE_V:
            res = value_set2(BOOL_V, tuple_nequal(left->value.ptr, right->value.ptr));

            value_free_type(left, tuple);
            value_free_type(right, tuple);

            return ires_success(res);
        }

        value_free_type(left, tuple);
        break;
    case TYPE_V:
        res = value_set2(BOOL_V, right->type != TYPE_V || left->value.chr != right->value.chr);

        value_free_shell(left);
        value_free(right);

        return ires_success(res);
    case FUNC_V:
        res = value_set2(BOOL_V, left->value.ptr != right->value.ptr);

        value_free_type(left, func);
        value_free(right);

        return ires_success(res);
    case STRUCT_V:
        res = value_set2(BOOL_V, left->value.ptr != right->value.ptr);

        value_free_type(left, context);
        value_free(right);

        return ires_success(res);
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_success(value_set2(BOOL_V, 1));
}

ires_t operate_ex_equal(value_p left, value_p right)
{
    if (!left)
    {
        if (!right)
            return ires_success(value_set2(BOOL_V, 1));

        value_free(right);
        return ires_success(value_set2(BOOL_V, 0));
    }

    if (!right)
    {
        value_free(left);
        return ires_success(value_set2(BOOL_V, 0));
    }

    if (left->type != right->type)
    {
        value_free(left);
        value_free(right);

        return ires_success(value_set2(BOOL_V, 0));
    }

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case OBJECT_V:
        res = value_set2(BOOL_V, left->value.ptr == right->value.ptr);

        value_free_shell(left);
        value_free_shell(right);

        return ires_success(res);
    case INT_V:
        res = value_set2(BOOL_V, int_equal(left->value.ptr, right->value.ptr));

        value_free_type(left, int);
        value_free_type(right, int);

        return ires_success(res);
    case FLOAT_V:
        res = value_set2(BOOL_V, float_equal(left->value.ptr, right->value.ptr));

        value_free_type(left, float);
        value_free_type(right, float);

        return ires_success(res);
    case COMPLEX_V:
        res = value_set2(BOOL_V, complex_equal(left->value.ptr, right->value.ptr));

        value_free_type(left, complex);
        value_free_type(right, complex);

        return ires_success(res);
    case BOOL_V:
        res = value_set2(BOOL_V, left->value.chr == right->value.chr);

        value_free_shell(left);
        value_free_shell(right);

        return ires_success(res);
    case CHAR_V:
        res = value_set2(BOOL_V, left->value.chr == right->value.chr);

        value_free_shell(left);
        value_free_shell(right);

        return ires_success(res);
    case STR_V:
        res = value_set2(BOOL_V, str_equal(left->value.ptr, right->value.ptr));

        value_free_type(left, str);
        value_free_type(right, str);

        return ires_success(res);
    case LIST_V:
        res = value_set2(BOOL_V, list_equal(left->value.ptr, right->value.ptr));

        value_free_type(left, list);
        value_free_type(right, list);

        return ires_success(res);
    case TUPLE_V:
        res = value_set2(BOOL_V, tuple_equal(left->value.ptr, right->value.ptr));

        value_free_type(left, tuple);
        value_free_type(right, tuple);

        return ires_success(res);
    case TYPE_V:
        res = value_set2(BOOL_V, left->value.chr == right->value.chr);

        value_free_shell(left);
        value_free_shell(right);

        return ires_success(res);
    case FUNC_V:
        res = value_set2(BOOL_V, left->value.ptr == right->value.ptr);

        value_free_type(left, func);
        value_free_type(right, func);

        return ires_success(res);
    case STRUCT_V:
        res = value_set2(BOOL_V, left->value.ptr == right->value.ptr);

        value_free_type(left, context);
        value_free_type(right, context);

        return ires_success(res);
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_success(value_set2(BOOL_V, 0));
}

ires_t operate_ex_nequal(value_p left, value_p right)
{
    if (!left)
    {
        if (!right)
            return ires_success(value_set2(BOOL_V, 0));

        value_free(right);
        return ires_success(value_set2(BOOL_V, 1));
    }

    if (!right)
    {
        value_free(left);
        return ires_success(value_set2(BOOL_V, 1));
    }

    if (left->type != right->type)
    {
        value_free(left);
        value_free(right);

        return ires_success(value_set2(BOOL_V, 1));
    }

    value_p res;
    char res_chr;

    switch (left->type)
    {
    case OBJECT_V:
        res = value_set2(BOOL_V, left->value.ptr != right->value.ptr);

        value_free_shell(left);
        value_free_shell(right);

        return ires_success(res);
    case INT_V:
        res = value_set2(BOOL_V, int_nequal(left->value.ptr, right->value.ptr));

        value_free_type(left, int);
        value_free_type(right, int);

        return ires_success(res);
    case FLOAT_V:
        res = value_set2(BOOL_V, float_nequal(left->value.ptr, right->value.ptr));

        value_free_type(left, float);
        value_free_type(right, float);

        return ires_success(res);
    case COMPLEX_V:
        res = value_set2(BOOL_V, complex_nequal(left->value.ptr, right->value.ptr));

        value_free_type(left, complex);
        value_free_type(right, complex);

        return ires_success(res);
    case BOOL_V:
        res = value_set2(BOOL_V, left->value.chr != right->value.chr);

        value_free_shell(left);
        value_free_shell(right);

        return ires_success(res);
    case CHAR_V:
        res = value_set2(BOOL_V, left->value.chr != right->value.chr);

        value_free_shell(left);
        value_free_shell(right);

        return ires_success(res);
    case STR_V:
        res = value_set2(BOOL_V, str_nequal(left->value.ptr, right->value.ptr));

        value_free_type(left, str);
        value_free_type(right, str);

        return ires_success(res);
    case LIST_V:
        res = value_set2(BOOL_V, list_nequal(left->value.ptr, right->value.ptr));

        value_free_type(left, list);
        value_free_type(right, list);

        return ires_success(res);
    case TUPLE_V:
        res = value_set2(BOOL_V, tuple_nequal(left->value.ptr, right->value.ptr));

        value_free_type(left, tuple);
        value_free_type(right, tuple);

        return ires_success(res);
    case TYPE_V:
        res = value_set2(BOOL_V, left->value.chr != right->value.chr);

        value_free_shell(left);
        value_free_shell(right);

        return ires_success(res);
    case FUNC_V:
        res = value_set2(BOOL_V, left->value.ptr != right->value.ptr);

        value_free_type(left, func);
        value_free_type(right, func);

        return ires_success(res);
    case STRUCT_V:
        res = value_set2(BOOL_V, left->value.ptr != right->value.ptr);

        value_free_type(left, context);
        value_free_type(right, context);

        return ires_success(res);
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_success(value_set2(BOOL_V, 1));
}

ires_t operate_less(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type("<");

    ltype = left->type;
    rtype = right->type;

    value_p res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_less(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_int_less(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, float);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, int_less_ul(left->value.ptr, right->value.chr));

            value_free_type(left, int);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, float_less_int(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_less(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, float);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, float_less_ul(left->value.ptr, right->value.chr));

            value_free_type(left, float);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, float);
        break;
    case BOOL_V:
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_ul_less(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_ul_less(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, float);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            res = value_set2(BOOL_V, left->value.chr < right->value.chr);

            value_free_shell(left);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "<",
        poss, pose, context));
}

ires_t operate_greater(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type(">");

    ltype = left->type;
    rtype = right->type;

    value_p res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_greater(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_int_greater(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, float);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, int_greater_ul(left->value.ptr, right->value.chr));

            value_free_type(left, int);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, float_greater_int(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_greater(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, float);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, float_greater_ul(left->value.ptr, right->value.chr));

            value_free_type(left, float);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, float);
        break;
    case BOOL_V:
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_ul_greater(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_ul_greater(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, float);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            res = value_set2(BOOL_V, left->value.chr > right->value.chr);

            value_free_shell(left);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, ">",
        poss, pose, context));
}

ires_t operate_less_eq(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type("<=");

    ltype = left->type;
    rtype = right->type;

    value_p res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_less_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_int_less_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, float);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, int_less_equal_ul(left->value.ptr, right->value.chr));

            value_free_type(left, int);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, float_less_equal_int(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_less_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, float);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, float_less_equal_ul(left->value.ptr, right->value.chr));

            value_free_type(left, float);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, float);
        break;
    case BOOL_V:
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_ul_less_equal(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_ul_less_equal(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, float);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            res = value_set2(BOOL_V, left->value.chr <= right->value.chr);

            value_free_shell(left);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "<=",
        poss, pose, context));
}

ires_t operate_greater_eq(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    check_none_type(">=");

    ltype = left->type;
    rtype = right->type;

    value_p res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_greater_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_int_greater_equal(left->value.ptr, right->value.ptr));

            value_free_type(left, int);
            value_free_type(right, float);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, int_greater_equal_ul(left->value.ptr, right->value.chr));

            value_free_type(left, int);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, int);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, float_greater_equal_int(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_greater(left->value.ptr, right->value.ptr));

            value_free_type(left, float);
            value_free_type(right, float);

            return ires_success(res);
        case CHAR_V:
        case BOOL_V:
            res = value_set2(BOOL_V, float_greater_equal_ul(left->value.ptr, right->value.chr));

            value_free_type(left, float);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, float);
        break;
    case BOOL_V:
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set2(BOOL_V, int_ul_greater_equal(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, int);

            return ires_success(res);
        case FLOAT_V:
            res = value_set2(BOOL_V, float_ul_greater_equal(left->value.chr, right->value.ptr));

            value_free_shell(left);
            value_free_type(right, float);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            res = value_set2(BOOL_V, left->value.chr >= right->value.chr);

            value_free_shell(left);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_shell(left);
        break;
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, ">=",
        poss, pose, context));
}

ires_t operate_xor(value_p left, value_p right)
{
    value_p res = value_set2(BOOL_V, value_is_true(left) ^ value_is_true(right));

    value_free(left);
    value_free(right);

    return ires_success(res);
}

ires_t operate_contain(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char ltype;
    char rtype;

    value_p res;
    char* str;

    if (!left)
    {
        if (!right)
            return ires_fail(illegal_operation(NONE_V, NONE_V, "in",
                poss, pose, context));

        switch (right->type)
        {
        case STR_V:
            str = malloc(5);
            strcpy(str, "none");

            res = value_set2(BOOL_V, str_contains_str(right->value.ptr, str));

            value_free_type(right, str);
            free(str);

            return ires_success(res);
        case LIST_V:
            res = value_set2(BOOL_V, list_contains(right->value.ptr, NULL));

            value_free_type(right, list);

            return ires_success(res);
        case TUPLE_V:
            res = value_set2(BOOL_V, tuple_contains(right->value.ptr, NULL));

            value_free_type(right, tuple);

            return ires_success(res);
        }

        rtype = right->type;
        value_free(right);

        return ires_fail(illegal_operation(NONE_V, rtype, "in",
            poss, pose, context));
    }

    if (!right)
    {
        ltype = left->type;
        value_free(left);

        return ires_fail(illegal_operation(ltype, NONE_V, "in",
            poss, pose, context));
    }

    ltype = left->type;
    rtype = right->type;

    switch (right->type)
    {
    case STR_V:
        switch (left->type)
        {
        case INT_V:
            str = int_get_str(left->value.ptr, 10);

            value_free_type(left, int);

            res = value_set2(BOOL_V, str_contains_str(right->value.ptr, str));

            value_free_type(right, str);
            free(str);

            return ires_success(res);
        case FLOAT_V:
            str = float_get_str(left->value.ptr, 10);

            value_free_type(left, float);

            res = value_set2(BOOL_V, str_contains_str(right->value.ptr, str));

            value_free_type(right, str);
            free(str);

            return ires_success(res);
        case COMPLEX_V:
            str = complex_get_str(left->value.ptr, 10);

            value_free_type(left, complex);

            res = value_set2(BOOL_V, str_contains_str(right->value.ptr, str));

            value_free_type(right, str);
            free(str);

            return ires_success(res);
        case BOOL_V:
            if (left->value.chr)
            {
                str = malloc(5);
                strcpy(str, "true");
            }
            else
            {
                str = malloc(6);
                strcpy(str, "false");
            }

            value_free_shell(left);

            res = value_set2(BOOL_V, str_contains_str(right->value.ptr, str));

            value_free_type(right, str);
            free(str);

            return ires_success(res);
        case CHAR_V:
            res = value_set2(BOOL_V, str_contains_char(right->value.ptr, left->value.chr));

            value_free_shell(left);
            value_free_type(right, str);

            return ires_success(res);
        case STR_V:
            res = value_set2(BOOL_V, str_contains(right->value.ptr, left->value.ptr));

            value_free_type(left, str);
            value_free_type(right, str);

            return ires_success(res);
        }

        value_free_type(right, str);
        break;
    case LIST_V:
        res = value_set2(BOOL_V, list_contains(right->value.ptr, left));

        value_free(left);
        value_free_type(right, list);

        return ires_success(res);
    case TUPLE_V:
        res = value_set2(BOOL_V, tuple_contains(right->value.ptr, left));

        value_free(left);
        value_free_type(right, tuple);

        return ires_success(res);
    default:
        value_free(left);
        break;
    }

    value_free(right);

    return ires_fail(illegal_operation(ltype, rtype, "in",
        poss, pose, context));
}

ires_t operate_is(value_p left, value_p right)
{
    if (!left)
    {
        if (!right)
            return ires_success(value_set2(BOOL_V, 1));

        value_free(right);
        return ires_success(value_set2(BOOL_V, 0));
    }

    if (!right)
    {
        value_free(left);
        return ires_success(value_set2(BOOL_V, 0));
    }

    value_p res;

    if (left->type == TYPE_V)
    {
        if (right->type == TYPE_V)
            res = value_set2(BOOL_V, left->value.chr == right->value.chr);
        else
            res = value_set2(BOOL_V, left->value.chr == right->type);
    }
    else if (right->type == TYPE_V)
        res = value_set2(BOOL_V, left->type == right->value.chr);
    else
        res = value_set2(BOOL_V, left->type == right->type);

    value_free(left);
    value_free(right);

    return ires_success(res);
}

ires_t operate_are(value_p left, value_p right, pos_p poss, pos_p pose, context_p context, pos_p lposs, pos_p lpose)
{
    if (!left)
    {
        value_free(right);

        return ires_fail(invalid_type("Left operand in operator (are)", "<list> or <tuple>", NONE_V,
            lposs, lpose, context));
    }

    char ltype = left->type;

    value_p res;
    unsigned char type = right ? right->type == TYPE_V ? right->value.chr : right->type : NONE_V;

    switch (left->type)
    {
    case LIST_V:
        res = value_set2(BOOL_V, list_are_type(left->value.ptr, type));

        value_free_type(left, list);
        value_free(right);

        return ires_success(res);
    case TUPLE_V:
        res = value_set2(BOOL_V, tuple_are_type(left->value.ptr, type));

        value_free_type(left, tuple);
        value_free(right);

        return ires_success(res);
    }

    value_free(left);
    value_free(right);

    return ires_fail(invalid_type("Left operand", "<list> or <tuple>", ltype,
        lposs, lpose, context));
}

ires_t operate_subscript(value_p left, value_p right, context_p context, pos_p lposs, pos_p lpose, pos_p rposs, pos_p rpose)
{
    if (!left)
    {
        value_free(right);

        return ires_fail(invalid_type("Operand", "<str>, <list> or <tuple>", NONE_V,
            lposs, lpose, context));
    }

    if (!right)
    {
        value_free(left);

        return ires_fail(invalid_type("Index", "<int>, <bool> or <char>", NONE_V,
            rposs, rpose, context));
    }

    unsigned char rtype = right->type;
    unsigned char ltype = left->type;

    unsigned long long index;
    value_p res;

    switch (left->type)
    {
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                value_free_type(left, str);
                value_free_type(right, int);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = str_size(left->value.ptr) - index;

            value_free_type(right, int);

            if (index >= str_size(left->value.ptr))
            {
                value_free_type(left, str);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            res = value_set2(CHAR_V, str_str(left->value.ptr)[index]);

            value_free_type(left, str);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= str_size(left->value.ptr))
            {
                value_free_type(left, str);
                value_free_shell(right);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            res = value_set2(CHAR_V, str_str(left->value.ptr)[right->value.chr]);

            value_free_type(left, str);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, str);
        goto index_err;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                value_free_type(left, list);
                value_free_type(right, int);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = list_size(left->value.ptr) - index;

            value_free_type(right, int);

            if (index >= list_size(left->value.ptr))
            {
                value_free_type(left, list);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            res = list_elements(left->value.ptr)[index];
            value_copy(res);

            value_free_type(left, list);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= list_size(left->value.ptr))
            {
                value_free_type(left, list);
                value_free_shell(right);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            res = list_elements(left->value.ptr)[right->value.chr];
            value_copy(res);

            value_free_type(left, list);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, list);
        goto index_err;
    case TUPLE_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                value_free_type(left, tuple);
                value_free_type(right, int);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = tuple_size(left->value.ptr) - index;

            value_free_type(right, int);

            if (index >= tuple_size(left->value.ptr))
            {
                value_free_type(left, tuple);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            res = tuple_elements(left->value.ptr)[index];
            value_copy(res);

            value_free_type(left, tuple);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= tuple_size(left->value.ptr))
            {
                value_free_type(left, tuple);
                value_free_shell(right);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            res = tuple_elements(left->value.ptr)[right->value.chr];
            value_copy(res);

            value_free_type(left, tuple);
            value_free_shell(right);

            return ires_success(res);
        }

        value_free_type(left, tuple);
        goto index_err;
    }

    value_free(left);
    value_free(right);

    return ires_fail(invalid_type("Operand", "<str>, <list> or <tuple>", ltype,
        lposs, lpose, context));

index_err:
    value_free(right);

    return ires_fail(invalid_type("Index", "<int>, <bool> or <char>", rtype,
        rposs, rpose, context));
}

ires_t operate_subscript_ptr(value_p* ptr, value_p right, context_p context, pos_p lposs, pos_p lpose, pos_p rposs, pos_p rpose)
{
    value_p left = *ptr;

    if (!left)
    {
        value_free(right);

        return ires_fail(invalid_type("Operand", "<str> or <list>", NONE_V,
            lposs, lpose, context));
    }

    if (!right)
        return ires_fail(invalid_type("Index", "<int>, <bool> or <char>", NONE_V,
            rposs, rpose, context));

    ires_t ires;

    unsigned long long index;

    switch (left->type)
    {
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                value_free_type(right, int);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = str_size(left->value.ptr) - index;

            value_free_type(right, int);

            if (index >= str_size(left->value.ptr))
            {
                value_free_type(left, str);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            if (left->ref)
            {
                left->ref--;

                left = value_set1(STR_V, str_set(left->value.ptr));
                *ptr = left;
            }

            ires.value = (value_p)(str_str(left->value.ptr) + index);
            ires.response = IRES_RESPONSE_SET(0, 0, 0, 0, 0, 1);

            return ires;
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= str_size(left))
            {
                value_free_shell(right);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            if (left->ref)
            {
                left->ref--;

                left = value_set1(STR_V, str_set(left->value.ptr));
                *ptr = left;
            }

            ires.value = (value_p)(str_str(left->value.ptr) + right->value.chr);
            ires.response = IRES_RESPONSE_SET(0, 0, 0, 0, 0, 1);

            return ires;
        }

        goto index_err;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                value_free_type(right, int);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = list_size(left->value.ptr) - index;

            value_free_type(right, int);

            if (index >= list_size(left->value.ptr))
                return ires_fail(out_of_range(rposs, rpose, context));

            if (left->ref)
            {
                left->ref--;

                left = value_set1(LIST_V, list_copy(left->value.ptr));
                *ptr = left;
            }

            ires.value = (value_p)(list_elements(left->value.ptr) + index);
            ires.response = IRES_RESPONSE_SET(0, 0, 0, 0, 1, 0);

            return ires;
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= list_size(left->value.ptr))
            {
                value_free_shell(right);

                return ires_fail(out_of_range(rposs, rpose, context));
            }

            if (left->ref)
            {
                left->ref--;

                left = value_set1(LIST_V, list_copy(left->value.ptr));
                *ptr = left;
            }

            ires.value = (value_p)(list_elements(left->value.ptr) + right->value.chr);
            ires.response = IRES_RESPONSE_SET(0, 0, 0, 0, 1, 0);

            return ires;
        }

        goto index_err;
    }

    value_free(right);

    return ires_fail(invalid_type("Operand", "<str> or <list>", left->type,
        lposs, lpose, context));

index_err:
    value_free(right);

    return ires_fail(invalid_type("Index", "<int>, <bool> or <char>", right->type,
        rposs, rpose, context));
}

ires_t operate_positive(value_p operand)
{
    if (!operand)
        return ires_success(value_set1(INT_V, int_set_ull(0)));

    value_p res;
    unsigned long long size;

    switch (operand->type)
    {
    case OBJECT_V:
        res = value_set1(INT_V, int_set_ull((unsigned long)operand->value.ptr));

        value_free_shell(operand);

        return ires_success(res);
    case BOOL_V:
    case CHAR_V:
    case TYPE_V:
        res = value_set1(INT_V, int_set_ull(operand->value.chr));

        value_free_shell(operand);

        return ires_success(res);
    case STR_V:
        res = value_set1(INT_V, int_set_ull(str_size(operand->value.ptr)));

        value_free_type(operand, str);

        return ires_success(res);
    case LIST_V:
        res = value_set1(INT_V, int_set_ull(list_size(operand->value.ptr)));

        value_free_type(operand, list);

        return ires_success(res);
    case TUPLE_V:
        res = value_set1(INT_V, int_set_ull(tuple_size(operand->value.ptr)));

        value_free_type(operand, tuple);

        return ires_success(res);
    case FUNC_V:
        size = (unsigned long)func_name(operand->value.ptr);

        value_free_type(operand, func);

        return ires_success(value_set1(INT_V, int_set_ull(size)));
    case STRUCT_V:
        size = (unsigned long)context_name(operand->value.ptr);

        value_free_type(operand, context);

        return ires_success(value_set1(INT_V, int_set_ull(size)));
    }

    return ires_success(operand);
}

ires_t operate_negate(value_p operand, pos_p poss, pos_p pose, context_p context)
{
    if (!operand)
        return ires_fail(illegal_operation_unary(NONE_V, "-",
            poss, pose, context));

    value_p res;
    char res_chr;

    switch (operand->type)
    {
    case INT_V:
        base_unary_operation(INT_V, int_negate, operand);
    case FLOAT_V:
        base_unary_operation(FLOAT_V, float_negate, operand);
    case COMPLEX_V:
        base_unary_operation(COMPLEX_V, complex_negate, operand);
    case BOOL_V:
        base_unary_operation_char(BOOL_V, !, operand);
    case CHAR_V:
        base_unary_operation_char(CHAR_V, -, operand);
    case STR_V:
        base_unary_operation(COMPLEX_V, str_reverse, operand);
    case LIST_V:
        base_unary_operation(COMPLEX_V, list_reverse, operand);
    }

    value_free(operand);

    return ires_fail(illegal_operation_unary(operand->type, "-",
        poss, pose, context));
}

ires_t operate_b_not(value_p operand, pos_p poss, pos_p pose, context_p context)
{
    if (!operand)
        return ires_fail(illegal_operation_unary(NONE_V, "~",
            poss, pose, context));

    value_p res;
    char res_chr;

    switch (operand->type)
    {
    case INT_V:
        base_unary_operation(INT_V, int_not, operand);
    case BOOL_V:
        base_unary_operation_char(BOOL_V, !, operand);
    case CHAR_V:
        base_unary_operation_char(CHAR_V, ~, operand);
    }

    char otype = operand->type;
    value_free(operand);

    return ires_fail(illegal_operation_unary(otype, "~",
        poss, pose, context));
}

ires_t operate_not(value_p operand)
{
    value_p res = value_set2(BOOL_V, !value_is_true(operand));

    value_free(operand);

    return ires_success(res);
}

ires_t operate_increment(value_p operand, pos_p poss, pos_p pose, context_p context)
{
    if (!operand)
        return ires_fail(illegal_operation_unary(NONE_V, "++",
            poss, pose, context));

    switch (operand->type)
    {
    case INT_V:
        if (operand->ref)
            return ires_success(value_set1(INT_V, int_add_ul(operand->value.ptr, 1)));

        int_add_ul_self(operand->value.ptr, 1);

        return ires_success(operand);
    case FLOAT_V:
        if (operand->ref)
            return ires_success(value_set1(FLOAT_V, float_add_ul(operand->value.ptr, 1)));

        float_add_ul_self(operand->value.ptr, 1);

        return ires_success(operand);
    case COMPLEX_V:
        if (operand->ref)
            return ires_success(value_set1(COMPLEX_V, complex_add_ul(operand->value.ptr, 1)));

        complex_add_ul_self(operand->value.ptr, 1);

        return ires_success(operand);
    case BOOL_V:
        if (operand->ref)
            return ires_success(value_set2(BOOL_V, !operand->value.chr));

        operand->value.chr = !operand->value.chr;

        return ires_success(operand);
    case CHAR_V:
        if (operand->ref)
            return ires_success(value_set2(CHAR_V, operand->value.chr + 1));

        operand->value.chr++;

        return ires_success(operand);
    }

    return ires_fail(illegal_operation_unary(operand->type, "++",
        poss, pose, context));
}

ires_t operate_decrement(value_p operand, pos_p poss, pos_p pose, context_p context)
{
    if (!operand)
        return ires_fail(illegal_operation_unary(NONE_V, "--",
            poss, pose, context));

    switch (operand->type)
    {
    case INT_V:
        if (operand->ref)
            return ires_success(value_set1(INT_V, int_subtract_ul(operand->value.ptr, 1)));

        int_subtract_ul_self(operand->value.ptr, 1);

        return ires_success(operand);
    case FLOAT_V:
        if (operand->ref)
            return ires_success(value_set1(FLOAT_V, float_subtract_ul(operand->value.ptr, 1)));

        float_subtract_ul_self(operand->value.ptr, 1);

        return ires_success(operand);
    case COMPLEX_V:
        if (operand->ref)
            return ires_success(value_set1(COMPLEX_V, complex_subtract_ul(operand->value.ptr, 1)));

        complex_subtract_ul_self(operand->value.ptr, 1);

        return ires_success(operand);
    case BOOL_V:
        if (operand->ref)
            return ires_success(value_set2(BOOL_V, !operand->value.chr));

        operand->value.chr = !operand->value.chr;

        return ires_success(operand);
    case CHAR_V:
        if (operand->ref)
            return ires_success(value_set2(CHAR_V, operand->value.chr - 1));

        operand->value.chr--;

        return ires_success(operand);
    }

    return ires_fail(illegal_operation_unary(operand->type, "--",
        poss, pose, context));
}

char operate_equal_compare(const value_p left, const value_p right)
{
    if (!left)
        return !right;

    if (!right)
        return 0;

    switch (left->type)
    {
    case OBJECT_V:
        return right->type == OBJECT_V && left->value.ptr == right->value.ptr;
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            return int_equal(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_equal_int(right->value.ptr, left->value.ptr);
        case COMPLEX_V:
            return complex_equal_int(right->value.ptr, left->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return int_equal_ul(left->value.ptr, right->value.chr);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            return float_equal_int(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_equal(left->value.ptr, right->value.ptr);
        case COMPLEX_V:
            return complex_equal_float(right->value.ptr, left->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return float_equal_ul(left->value.ptr, right->value.chr);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            return complex_equal_int(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return complex_equal_float(left->value.ptr, right->value.ptr);
        case COMPLEX_V:
            return complex_equal(left->value.ptr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return complex_equal_ul(left->value.ptr, right->value.chr);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            return int_equal_ul(right->value.ptr, left->value.chr);
        case FLOAT_V:
            return float_equal_ul(right->value.ptr, left->value.chr);
        case COMPLEX_V:
            return complex_equal_ul(right->value.ptr, left->value.chr);
        case BOOL_V:
        case CHAR_V:
            return left->value.chr == right->value.chr;
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            return int_equal_ul(right->value.ptr, left->value.chr);
        case FLOAT_V:
            return float_equal_ul(right->value.ptr, left->value.chr);
        case COMPLEX_V:
            return complex_equal_ul(right->value.ptr, left->value.chr);
        case BOOL_V:
        case CHAR_V:
            return left->value.chr == right->value.chr;
        case STR_V:
            return str_equal_char(right->value.ptr, left->value.chr);
        }

        break;
    case STR_V:
        switch (right->type)
        {
        case CHAR_V:
            return str_equal_char(left->value.ptr, right->value.chr);
        case STR_V:
            return str_equal(left->value.ptr, right->value.ptr);
        }

        break;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
            return list_equal(left->value.ptr, right->value.ptr);
        case TUPLE_V:
            return list_equal_tuple(left->value.ptr, right->value.ptr);
        }

        break;
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
            return list_equal_tuple(right->value.ptr, left->value.ptr);
        case TUPLE_V:
            return tuple_equal(left->value.ptr, right->value.ptr);
        }

        break;
    case TYPE_V:
        return left->type == TYPE_V && left->value.chr == right->value.chr;
    case FUNC_V:
    case STRUCT_V:
        return left->value.ptr == right->value.ptr;
    }

    return 0;
}

char operate_less_compare(const value_p left, const value_p right)
{
    if (!left || !right)
        return 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            return int_less(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_int_less(left->value.ptr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return int_less_ul(left->value.ptr, right->value.chr);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            return float_less_int(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_less(left->value.ptr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return float_less_ul(left->value.ptr, right->value.chr);
        }

        break;
    case BOOL_V:
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            return int_ul_less(left->value.chr, right->value.ptr);
        case FLOAT_V:
            return float_ul_less(left->value.chr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return left->value.chr < right->value.chr;
        }

        break;
    }

    return 0;
}

char operate_greater_compare(const value_p left, const value_p right)
{
    if (!left || !right)
        return 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            return int_greater(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_int_greater(left->value.ptr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return int_greater_ul(left->value.ptr, right->value.chr);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            return float_greater_int(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_greater(left->value.ptr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return float_greater_ul(left->value.ptr, right->value.chr);
        }

        break;
    case BOOL_V:
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            return int_ul_greater(left->value.chr, right->value.ptr);
        case FLOAT_V:
            return float_ul_greater(left->value.chr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return left->value.chr > right->value.chr;
        }

        break;
    }

    return 0;
}

ires_t operate_success(value_p left, const value_p right, pos_p poss, pos_p pose, context_p context)
{
    if (!left)
        return ires_fail(illegal_operation(NONE_V, right->type, "success",
            poss, pose, context));

    value_p res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (left->ref)
            {
                left->ref--;
                return ires_success(value_set1(INT_V, int_add(left->value.ptr, right->value.ptr)));
            }

            int_add_self(left->value.ptr, right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            res = value_set1(FLOAT_V, float_add_int(right->value.ptr, left->value.ptr));

            value_free_type(left, int);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            if (left->ref)
            {
                left->ref--;
                return ires_success(value_set1(INT_V, int_add_ul(left->value.ptr, right->value.chr)));
            }

            int_add_ul_self(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (left->ref)
            {
                left->ref--;
                return ires_success(value_set1(FLOAT_V, float_add_int(left->value.ptr, right->value.ptr)));
            }

            float_add_int_self(left->value.ptr, right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            if (left->ref)
            {
                left->ref--;
                return ires_success(value_set1(FLOAT_V, float_add(left->value.ptr, right->value.ptr)));
            }

            float_add_self(left->value.ptr, right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            if (left->ref)
            {
                left->ref--;
                return ires_success(value_set1(FLOAT_V, float_add_ul(left->value.ptr, right->value.chr)));
            }

            float_add_ul_self(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set1(INT_V, int_add_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);

            return ires_success(res);
            break;
        case FLOAT_V:
            res = value_set1(FLOAT_V, float_add_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);

            return ires_success(res);
        case BOOL_V:
            if (left->ref)
            {
                left->ref--;
                return ires_success(value_set2(BOOL_V, left->value.chr ^ right->value.chr));
            }

            left->value.chr ^= right->value.chr;

            return ires_success(left);
        case CHAR_V:
            if (left->ref)
            {
                left->ref--;
                return ires_success(value_set2(CHAR_V, left->value.chr + right->value.chr));
            }

            left->type = CHAR_V;
            left->value.chr += right->value.chr;

            return ires_success(left);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            res = value_set1(INT_V, int_add_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);

            return ires_success(res);
            break;
        case FLOAT_V:
            res = value_set1(FLOAT_V, float_add_ul(right->value.ptr, left->value.chr));

            value_free_shell(left);

            return ires_success(res);
        case BOOL_V:
        case CHAR_V:
            if (left->ref)
            {
                left->ref--;
                return ires_success(value_set2(CHAR_V, left->value.chr + right->value.chr));
            }

            left->value.chr += right->value.chr;

            return ires_success(left);
        }

        break;
    }

    return ires_fail(illegal_operation(left->type, right->type, "success",
        poss, pose, context));
}

char operate_success_type_change(const value_p left, const value_p right)
{
    switch (left->type)
    {
    case INT_V:
        return right->type == FLOAT_V;
    case FLOAT_V:
        return 0;
    case BOOL_V:
        return right->type != BOOL_V;
    case CHAR_V:
        return right->type != BOOL_V && right->type != CHAR_V;
    }

    return 0;
}

char operate_sign(const value_p operand)
{
    switch (operand->type)
    {
    case INT_V:
        return int_sign(operand->value.ptr) < 0;
    case FLOAT_V:
        return float_sign(operand->value.ptr) < 0;
    case BOOL_V:
    case CHAR_V:
        return 0;
    }

    return 0;
}

unsigned long long operate_size(const value_p operand)
{
    switch (operand->type)
    {
    case STR_V:
        return str_size(operand->value.ptr);
    case LIST_V:
        return list_size(operand->value.ptr);
    case TUPLE_V:
        return tuple_size(operand->value.ptr);
    }

    return 0;
}

value_p operate_index(const value_p operand, unsigned long long index)
{
    switch (operand->type)
    {
    case STR_V:
        return value_set2(CHAR_V, str_str(operand->value.ptr)[index]);
    case LIST_V:
        return list_elements(operand->value.ptr)[index];
    case TUPLE_V:
        return tuple_elements(operand->value.ptr)[index];
    }

    return NULL;
}
