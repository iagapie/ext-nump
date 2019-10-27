#include "mt_multiarray_umath.h"
#include "ext/standard/php_math.h"
#include "../php/common.h"

mt_t *mt_zeros(const zend_uchar type, MT_AXES_PARAMS)
{
    mt_t *mt = mt_init_all_t(type, d, axes);
    mt_val_t *val;

    MT_FOREACH_VAL(mt, val) {
        MT_SET_VAL_P(mt, val, 0);
    } MT_FOREACH_END();

    return mt;
}

mt_t *mt_round(const mt_t *mt, const int precision, const int mode)
{
    if (!IS_VALID_P(mt)) {
        return NULL;
    }

    mt_t *copy = mt_clone(mt);
    mt_val_t *val;

    if (mt->type == IS_MT_DOUBLE) {
        MT_FOREACH_VAL(copy, val) {
            MT_SET_VAL_P(copy, val, _php_math_round(MT_GET_DVAL_P(copy, val), precision, mode));
        } MT_FOREACH_END();
    }

    return copy;
}

zend_always_inline mt_t *mt_negative(const mt_t *mt)
{
    if (mt->type == IS_MT_LONG) {
        return mt_mul_lval(mt, -1);
    }

    return mt_mul_dval(mt, -1.0);
}

mt_t *mt_dot(const mt_t *a, const mt_t *b)
{
    if (!IS_VALID_P(a) || !IS_VALID_P(b)) {
        return NULL;
    }

    return NULL;
}

static bool _mt_validate(const mt_t *a, const mt_t *b)
{
    if (!IS_VALID_P(a) || !IS_VALID_P(b)) {
        return false;
    }

    if (!IS_MT_EMPTY_P(a) && !IS_MT_EMPTY_P(b)
        && (
            (a->shape.d == 1 && b->shape.d == 1 && a->shape.axes[0] == b->shape.axes[0])
            || (a->shape.d == 1 && a->shape.axes[0] == 1)
            || (b->shape.d == 1 && b->shape.axes[0] == 1)
            || (
                (a->shape.d == 1 || b->shape.d == 1 || a->shape.d == b->shape.d)
                && a->shape.axes[a->shape.d - 1] == b->shape.axes[b->shape.d - 1]
            )
        )
    ) {
        return true;
    }

    zend_string *sa = mt_axes_to_string(a->shape.d, a->shape.axes);
    zend_string *sb = mt_axes_to_string(b->shape.d, b->shape.axes);

    SHAPES_NOT_ALIGNED(ZSTR_VAL(sa), ZSTR_VAL(sb));

    zend_string_release(sa);
    zend_string_release(sb);

    return false;
}

static void _mt_normalize(mt_t *mt, MT_AXES_PARAMS)
{
    if (d < mt->shape.d) {
        return;
    }

    if (d == 1 && axes[0] == 1) {
        return;
    }

    zend_ulong *naxes, size, i;
    mt_val_t *buffer;

    size = 1;
    for (i = 0; i < d; i++) {
        size *= axes[i];
    }
    
    buffer = mt_val_init(size);

    naxes = mt_axes_init(d);
    memcpy(naxes, axes, d * sizeof(zend_ulong));

    if (mt->shape.d == 1 && mt->shape.axes[0] == 1) {
        for (i = 0; i < size; i++) {
            if (mt->type == IS_MT_LONG) {
                buffer[i].lval = mt->buffer[0].lval;
            } else {
                buffer[i].dval = mt->buffer[0].dval;
            }
        }
    } else {
        for (i = 0; i < d; i++) {
            memcpy(buffer + (i * axes[d - 1]), mt->buffer, mt->size * sizeof(mt_val_t));
        }
    }

    MT_FREE_P(mt->buffer);
    MT_FREE_P(mt->shape.axes);

    mt->buffer = buffer;
    mt->shape.axes = naxes;
    mt->shape.d = d;
    mt->size = size;
}

mt_t *mt_mul(const mt_t *a, const mt_t *b)
{
    if (!_mt_validate(a, b)) {
        return NULL;
    }

    mt_t *r = mt_clone(a);
    mt_t *t = mt_clone(b);

    _mt_normalize(r, b->shape.d, b->shape.axes);
    _mt_normalize(t, a->shape.d, a->shape.axes);
    
    mt_val_t *val;
    zend_ulong i;

    MT_FOREACH_VAL_IDX(r, val, i) {
        if (r->type == IS_MT_LONG) {
            val->lval *= MT_GET_LVAL(*t, t->buffer[i]);
        } else {
            val->dval *= MT_GET_DVAL(*t, t->buffer[i]);
        }
    } MT_FOREACH_END();

    mt_free(t);

    return r;
}

mt_t *mt_div(const mt_t *a, const mt_t *b)
{
    if (!_mt_validate(a, b)) {
        return NULL;
    }

    mt_t *r = mt_clone(a);
    mt_t *t = mt_clone(b);

    _mt_normalize(r, b->shape.d, b->shape.axes);
    _mt_normalize(t, a->shape.d, a->shape.axes);
    
    mt_val_t *val;
    zend_ulong i;

    MT_FOREACH_VAL_IDX(r, val, i) {
        if (r->type == IS_MT_LONG) {
            val->lval /= MT_GET_LVAL(*t, t->buffer[i]);
        } else {
            val->dval /= MT_GET_DVAL(*t, t->buffer[i]);
        }
    } MT_FOREACH_END();

    mt_free(t);

    return r;
}

mt_t *mt_add(const mt_t *a, const mt_t *b)
{
    if (!_mt_validate(a, b)) {
        return NULL;
    }

    mt_t *r = mt_clone(a);
    mt_t *t = mt_clone(b);

    _mt_normalize(r, b->shape.d, b->shape.axes);
    _mt_normalize(t, a->shape.d, a->shape.axes);
    
    mt_val_t *val;
    zend_ulong i;

    MT_FOREACH_VAL_IDX(r, val, i) {
        if (r->type == IS_MT_LONG) {
            val->lval += MT_GET_LVAL(*t, t->buffer[i]);
        } else {
            val->dval += MT_GET_DVAL(*t, t->buffer[i]);
        }
    } MT_FOREACH_END();

    mt_free(t);

    return r;
}

mt_t *mt_sub(const mt_t *a, const mt_t *b)
{
    if (!_mt_validate(a, b)) {
        return NULL;
    }

    mt_t *r = mt_clone(a);
    mt_t *t = mt_clone(b);

    _mt_normalize(r, b->shape.d, b->shape.axes);
    _mt_normalize(t, a->shape.d, a->shape.axes);
    
    mt_val_t *val;
    zend_ulong i;

    MT_FOREACH_VAL_IDX(r, val, i) {
        if (r->type == IS_MT_LONG) {
            val->lval -= MT_GET_LVAL(*t, t->buffer[i]);
        } else {
            val->dval -= MT_GET_DVAL(*t, t->buffer[i]);
        }
    } MT_FOREACH_END();

    mt_free(t);

    return r;
}

zend_always_inline mt_t *mt_dot_lval(const mt_t *a, const zend_long b)
{
    return mt_mul_lval(a, b);
}

mt_t *mt_mul_lval(const mt_t *a, const zend_long b)
{
    if (!IS_VALID_P(a)) {
        return NULL;
    }

    mt_t *mt = mt_clone(a);
    mt_val_t *val;

    MT_FOREACH_VAL(mt, val) {
        MT_SET_VAL_P(mt, val, MT_GET_LVAL_P(mt, val) * b);
    } MT_FOREACH_END();

    return mt;
}

mt_t *mt_div_lval(const mt_t *a, const zend_long b)
{
    if (!IS_VALID_P(a)) {
        return NULL;
    }
    
    mt_t *mt = mt_clone(a);
    mt_val_t *val;

    MT_FOREACH_VAL(mt, val) {
        MT_SET_VAL_P(mt, val, MT_GET_LVAL_P(mt, val) / b);
    } MT_FOREACH_END();

    return mt;
}

mt_t *mt_add_lval(const mt_t *a, const zend_long b)
{
    if (!IS_VALID_P(a)) {
        return NULL;
    }
    
    mt_t *mt = mt_clone(a);
    mt_val_t *val;

    MT_FOREACH_VAL(mt, val) {
        MT_SET_VAL_P(mt, val, MT_GET_LVAL_P(mt, val) + b);
    } MT_FOREACH_END();

    return mt;
}

zend_always_inline mt_t *mt_sub_lval(const mt_t *a, const zend_long b)
{
    return mt_add_lval(a, -b);
}

zend_always_inline mt_t *mt_dot_dval(const mt_t *a, const double b)
{
    return mt_mul_dval(a, b);
}

mt_t *mt_mul_dval(const mt_t *a, const double b)
{
    if (!IS_VALID_P(a)) {
        return NULL;
    }

    mt_t *mt = mt_clone(a);
    mt_val_t *val;

    MT_FOREACH_VAL(mt, val) {
        MT_SET_VAL_P(mt, val, MT_GET_DVAL_P(mt, val) * b);
    } MT_FOREACH_END();

    return mt;
}

zend_always_inline mt_t *mt_div_dval(const mt_t *a, const double b)
{
    return mt_mul_dval(a, 1.0 / b);
}

mt_t *mt_add_dval(const mt_t *a, const double b)
{
    if (!IS_VALID_P(a)) {
        return NULL;
    }

    mt_t *mt = mt_clone(a);
    mt_val_t *val;

    MT_FOREACH_VAL(mt, val) {
        MT_SET_VAL_P(mt, val, MT_GET_DVAL_P(mt, val) + b);
    } MT_FOREACH_END();

    return mt;
}

zend_always_inline mt_t *mt_sub_dval(const mt_t *a, const double b)
{
    return mt_add_dval(a, -b);
}

double mt_mean(const mt_t *mt)
{
    if (!IS_VALID_P(mt) || IS_MT_EMPTY_P(mt)) {
        return 0.0;
    }

    double sum = 0.0;
    mt_val_t *val;

    MT_FOREACH_VAL(mt, val) {
        sum += MT_GET_DVAL_P(mt, val);
    } MT_FOREACH_END();

    return sum / (double) mt->size;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
