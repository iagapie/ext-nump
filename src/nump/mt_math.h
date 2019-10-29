#ifndef NUMP_MT_MATH_H
#define NUMP_MT_MATH_H

#include "mt_base.h"

#define MT_OP_ADD 1
#define MT_OP_SUB 2
#define MT_OP_MUL 3
#define MT_OP_DIV 4

#define MT_MATH_EXP 1
#define MT_MATH_LOG 2
#define MT_MATH_LOG2 3
#define MT_MATH_LOG10 4

mt_t *mt_zeros(mt_shape_t *shape);
mt_t *mt_round(const mt_t *mt, const int precision, const int mode);
mt_t *mt_negative(const mt_t *mt);

mt_t *mt_math(const mt_t *mt, const char math);

mt_t *mt_dot(const mt_t *a, const mt_t *b);
mt_t *mt_mul(const mt_t *a, const mt_t *b);
mt_t *mt_div(const mt_t *a, const mt_t *b);
mt_t *mt_add(const mt_t *a, const mt_t *b);
mt_t *mt_sub(const mt_t *a, const mt_t *b);

mt_t *mt_dot_val(const mt_t *a, const double b);
mt_t *mt_mul_val(const mt_t *a, const double b);
mt_t *mt_div_val(const mt_t *a, const double b);
mt_t *mt_add_val(const mt_t *a, const double b);
mt_t *mt_sub_val(const mt_t *a, const double b);

double mt_mean(const mt_t *mt);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
