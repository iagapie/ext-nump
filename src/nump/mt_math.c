#include "php.h"
#include "ext/standard/php_math.h"

#include "mt_math.h"
#include "mt_string.h"
#include "../common.h"

mt_t *mt_zeros(mt_shape_t *shape)
{
    mt_t *mt = mt_init_by_shape(shape);
    double *val;

    if (IS_MT_VALID_P(mt)) {
        MT_FOREACH_VAL(mt, val) {
            *val = 0.0;
        } MT_FOREACH_END();
    }

    return mt;
}

mt_t *mt_round(const mt_t *mt, const int precision, const int mode)
{
    mt_t *copy = mt_clone(mt);
    int m = mode <= 0 ? 1 : mode;
    double *val;

    if (IS_MT_VALID_P(copy)) {
        MT_FOREACH_VAL(copy, val) {
            *val = _php_math_round(*val, precision, m);
        } MT_FOREACH_END();
    }

    return copy;
}

zend_always_inline mt_t *mt_negative(const mt_t *mt)
{
    return mt_mul_val(mt, -1.0);
}

static bool _mt_validate(const mt_t *a, const mt_t *b)
{
    if (!IS_MT_VALID_P(a) || !IS_MT_VALID_P(b)) {
        return false;
    }

    if (!IS_MT_EMPTY_P(a) && !IS_MT_EMPTY_P(b)
        && (
            (a->shape->d == 1 && b->shape->d == 1 && a->shape->axes[0] == b->shape->axes[0])
            || (a->shape->d == 1 && a->shape->axes[0] == 1)
            || (b->shape->d == 1 && b->shape->axes[0] == 1)
            || (
                (a->shape->d == 1 || b->shape->d == 1 || a->shape->d == b->shape->d)
                && a->shape->axes[a->shape->d - 1] == b->shape->axes[b->shape->d - 1]
            )
        )
    ) {
        return true;
    }

    zend_string *sa = mt_shape_to_string(a->shape);
    zend_string *sb = mt_shape_to_string(b->shape);

    EX_SHAPES_NOT_ALIGNED(ZSTR_VAL(sa), ZSTR_VAL(sb));

    zend_string_release(sa);
    zend_string_release(sb);

    return false;
}

static bool _shape_equals(const mt_shape_t *a, const mt_shape_t *b)
{
    if (a->d != b->d) {
        return false;
    }

    zend_ulong i;
    for (i = 0; i < a->d; i++) {
        if (a->axes[i] != b->axes[i]) {
            return false;
        }
    }

    return true;
}

static void _mt_normalize(mt_t *mt, const mt_shape_t *shape)
{
    if (shape->d < mt->shape->d) {
        return;
    }

    if (shape->d == 1 && shape->axes[0] == 1) {
        return;
    }

    if (_shape_equals(mt->shape, shape)) {
        return;
    }

    zend_ulong i, j;
    zend_ulong size = mt_shape_prod(0, shape);
    zend_ulong *axes = mt_shape_axes_clone(shape->axes, shape->d);
    double *data = mt_buffer_data_init(size);

    if (mt->shape->d == 1 && mt->shape->axes[0] == 1) {
        for (i = 0; i < size; i++) {
            data[i] = mt->buffer->data[0];
        }
    } else {
        for (i = 0; i < size; i += axes[shape->d - 1]) {
            memcpy(data + i, mt->buffer->data, mt->buffer->size * sizeof(double));
        }
    }

    FREE_AND_REPLACE(mt->buffer->data, data);
    FREE_AND_REPLACE(mt->shape->axes, axes);

    mt->buffer->size = size;
    mt->shape->d = shape->d;
}

mt_t *mt_dot(const mt_t *a, const mt_t *b)
{
    if (!IS_MT_VALID_P(a) || !IS_MT_VALID_P(b)) {
        return NULL;
    }

    mt_t *am = mt_clone(a);
    mt_t *bm = mt_clone(b);
    mt_t *r = NULL;

    if (bm->shape->d == 1) {
        bm->shape->axes = REALLOC(bm->shape->axes, ++bm->shape->d, 1, zend_ulong);
        bm->shape->axes[1] = 1;
    }

    if (am->shape->d == 1) {
        am->shape->axes = REALLOC(am->shape->axes, ++am->shape->d, 1, zend_ulong);
        am->shape->axes[1] = am->shape->axes[0];
        am->shape->axes[0] = 1;
    }

    if (am->shape->d > 2 || bm->shape->d > 2) {
        EX_THROW("Support only 1D and 2D arrays.");
        goto ret;
    }

    if (am->shape->axes[1] != bm->shape->axes[0]) {
        zend_string *sa = mt_shape_to_string(a->shape);
        zend_string *sb = mt_shape_to_string(b->shape);

        EX_SHAPES_NOT_ALIGNED(ZSTR_VAL(sa), ZSTR_VAL(sb));

        zend_string_release(sa);
        zend_string_release(sb);

        goto ret;
    }

    mt_buffer_t *buffer = mt_buffer_init(am->shape->axes[0] * bm->shape->axes[1]);
    mt_shape_t *shape;
    zend_ulong ca, cb, ra;
    double tmp;

    for (cb = 0; cb < bm->shape->axes[1]; cb++) {
        for (ra = 0; ra < am->shape->axes[0]; ra++) {
            tmp = 0.0;

            for (ca = 0; ca < am->shape->axes[1]; ca++) {
                tmp += am->buffer->data[ra * am->shape->axes[1] + ca]
                    * bm->buffer->data[ca * bm->shape->axes[1] + cb];
            }

            buffer->data[ra * bm->shape->axes[1] + cb] = tmp;
        }
    }

    if (a->shape->d == 1 || b->shape->d == 1) {
        shape = mt_shape_init(1);
        shape->axes[0] = am->shape->axes[0] < bm->shape->axes[1]
            ? bm->shape->axes[1] : am->shape->axes[0];
    } else {
        shape = mt_shape_init(2);
        shape->axes[0] = am->shape->axes[0];
        shape->axes[1] = bm->shape->axes[1];
    }

    r = mt_init(buffer, shape);

ret:
    mt_free(am);
    mt_free(bm);

    return r;
}

mt_t *_mt_op(const mt_t *a, const mt_t *b, const char op)
{
    if (!_mt_validate(a, b)) {
        return NULL;
    }

    mt_t *r = mt_clone(a);
    mt_t *t = mt_clone(b);

    _mt_normalize(r, b->shape);
    _mt_normalize(t, a->shape);
    
    double *val, tval;
    zend_ulong i;

    MT_FOREACH_VAL_IDX(r, val, i) {
        tval = t->buffer->data[i];

        switch (op)
            {
            case MT_OP_ADD:
                *val += tval;
                break;
            case MT_OP_SUB:
                *val -= tval;
                break;
            case MT_OP_MUL:
                *val *= tval;
                break;
            case MT_OP_DIV:
                *val /= tval;
                break;
            }
    } MT_FOREACH_END();

    mt_free(t);

    return r;
}

static mt_t *_mt_op_val(const mt_t *a, const double b, const char op)
{
    mt_t *copy = mt_clone(a);
    double *val;

    if (IS_MT_VALID_P(copy)) {
        MT_FOREACH_VAL(copy, val) {
            switch (op)
            {
            case MT_OP_ADD:
                *val += b;
                break;
            case MT_OP_SUB:
                *val -= b;
                break;
            case MT_OP_MUL:
                *val *= b;
                break;
            case MT_OP_DIV:
                *val /= b;
                break;
            }
        } MT_FOREACH_END();
    }

    return copy;
}

zend_always_inline mt_t *mt_mul(const mt_t *a, const mt_t *b)
{
    return _mt_op(a, b, MT_OP_MUL);
}

zend_always_inline mt_t *mt_div(const mt_t *a, const mt_t *b)
{
    return _mt_op(a, b, MT_OP_DIV);
}

zend_always_inline mt_t *mt_add(const mt_t *a, const mt_t *b)
{
    return _mt_op(a, b, MT_OP_ADD);
}

zend_always_inline mt_t *mt_sub(const mt_t *a, const mt_t *b)
{
    return _mt_op(a, b, MT_OP_SUB);
}

zend_always_inline mt_t *mt_dot_val(const mt_t *a, const double b)
{
    return mt_mul_val(a, b);
}

zend_always_inline mt_t *mt_mul_val(const mt_t *a, const double b)
{
    return _mt_op_val(a, b, MT_OP_MUL);
}

zend_always_inline mt_t *mt_div_val(const mt_t *a, const double b)
{
    return _mt_op_val(a, b, MT_OP_DIV);
}

zend_always_inline mt_t *mt_add_val(const mt_t *a, const double b)
{
    return _mt_op_val(a, b, MT_OP_ADD);
}

zend_always_inline mt_t *mt_sub_val(const mt_t *a, const double b)
{
    return _mt_op_val(a, b, MT_OP_SUB);
}

double mt_mean(const mt_t *mt)
{
    if (!IS_MT_VALID_P(mt)) {
        return 0.0;
    }

    double sum = 0.0, *val;

    MT_FOREACH_VAL(mt, val) {
        sum += *val;
    } MT_FOREACH_END();

    return sum / (double) mt->buffer->size;
}

zend_always_inline bool mt_is_square(const mt_t *mt)
{
    return IS_MT_VALID_P(mt)
        && mt->shape->d == 2
        && mt->shape->axes[0] == mt->shape->axes[1];
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
