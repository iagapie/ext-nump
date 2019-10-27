#ifndef NUMP_MT_MULTIARRAY_UMATH_H
#define NUMP_MT_MULTIARRAY_UMATH_H

#include "mt_base.h"

mt_t *mt_zeros(const zend_uchar type, MT_AXES_PARAMS);
mt_t *mt_round(const mt_t *mt, const int precision, const int mode);
mt_t *mt_negative(const mt_t *mt);

mt_t *mt_dot(const mt_t *a, const mt_t *b);
mt_t *mt_mul(const mt_t *a, const mt_t *b);
mt_t *mt_div(const mt_t *a, const mt_t *b);
mt_t *mt_add(const mt_t *a, const mt_t *b);
mt_t *mt_sub(const mt_t *a, const mt_t *b);

mt_t *mt_dot_lval(const mt_t *a, const zend_long b);
mt_t *mt_mul_lval(const mt_t *a, const zend_long b);
mt_t *mt_div_lval(const mt_t *a, const zend_long b);
mt_t *mt_add_lval(const mt_t *a, const zend_long b);
mt_t *mt_sub_lval(const mt_t *a, const zend_long b);

mt_t *mt_dot_dval(const mt_t *a, const double b);
mt_t *mt_mul_dval(const mt_t *a, const double b);
mt_t *mt_div_dval(const mt_t *a, const double b);
mt_t *mt_add_dval(const mt_t *a, const double b);
mt_t *mt_sub_dval(const mt_t *a, const double b);

double mt_mean(const mt_t *mt);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
