#include "mt_base.h"

static void *_mt_reallocate_p(void *ptr, const zend_ulong length, const zend_ulong current, const size_t size)
{
    if (length == current || length == 0 || size == 0) {
        return ptr;
    }

    if (IS_VALID_P(ptr)) {
        ptr = erealloc(ptr, length * size);

        if (length > current) {
            memset(ptr + current, 0, (length - current) * size);
        }
    }

    return ptr;
}

zend_always_inline mt_val_t *mt_val_init(const zend_ulong n)
{
    return MT_CALLOC(n, mt_val_t);
}

mt_val_t *mt_val_clone(const mt_val_t *src, const zend_ulong n)
{
    mt_val_t *dest;

    if (IS_VALID_P(src)) {
        dest = mt_val_init(n);
        memcpy(dest, src, n * sizeof(mt_val_t));
    } else {
        dest = NULL;
    }

    return dest;
}

zend_always_inline mt_val_t *mt_val_reallocate(mt_val_t *val, const zend_ulong length, const zend_ulong current)
{
    return (mt_val_t *) _mt_reallocate_p(val, length, current, sizeof(mt_val_t));
}

zend_always_inline zend_ulong *mt_axis_init(const zend_ulong n)
{
    return MT_CALLOC(n, zend_ulong);
}

zend_ulong *mt_axis_clone(const zend_ulong *src, const zend_ulong n)
{
    zend_ulong *dest;

    if (IS_VALID_P(src)) {
        dest = mt_axis_init(n);
        memcpy(dest, src, n * sizeof(zend_ulong));
    } else {
        dest = NULL;
    }

    return dest;
}

zend_always_inline zend_ulong *mt_axis_reallocate(zend_ulong *axis, const zend_ulong length, const zend_ulong current)
{
    return (zend_ulong *) _mt_reallocate_p(axis, length, current, sizeof(zend_ulong));
}

zend_always_inline mt_t *mt_init()
{
    return mt_init_t(IS_MT_UNDEF);
}

mt_t *mt_init_t(const zend_uchar type)
{
    mt_t *mt = MT_CALLOC(1, mt_t);

    mt->type = type;
    mt->size = 0;
    mt->buffer = NULL;
    mt->shape.axis = NULL;
    mt->shape.d = 0;

    return mt;
}

static zend_always_inline void mt_set_shape(mt_t *mt, MT_AXIS_PARAMS)
{
    zend_ulong i;
    
    if (!IS_VALID_P(mt)) {
        return;
    }

    if (!IS_VALID_P(mt->shape.axis)) {
        mt->shape.axis = mt_axis_init(d);
    }

    mt->shape.d = d;
    mt->size = 1;

    for (i = 0; i < d; i++) {
        mt->size *= mt->shape.axis[i] = axis[i];
    }
}

mt_t *mt_init_all_t(const zend_uchar type, MT_AXIS_PARAMS)
{
    mt_t *mt;
    zend_ulong i;

    if (d == 0) {
        return NULL;
    }

    mt = mt_init_t(type);
    mt_set_shape(mt, d, axis);
    mt->buffer = mt_val_init(mt->size);

    return mt;
}

zend_always_inline mt_t *mt_init_all(MT_AXIS_PARAMS)
{
    return mt_init_all_t(IS_MT_UNDEF, d, axis);
}

mt_t *mt_clone(const mt_t *src)
{
    mt_t *dest;

    if (IS_VALID_P(src)) {
        dest = mt_init_t(src->type);

        dest->size = src->size;
        dest->shape.d = src->shape.d;
        dest->shape.axis = mt_axis_clone(src->shape.axis, src->shape.d);
        dest->buffer = mt_val_clone(src->buffer, src->size);
    } else {
        dest = NULL;
    }

    return dest;
}

mt_t *mt_reallocate(mt_t *mt, MT_AXIS_PARAMS)
{
    zend_ulong i, current;

    if (d == 0) {
        return mt;
    }

    current = mt->size;

    mt->buffer = mt_val_reallocate(mt->buffer, mt->size, current);
    mt->shape.axis = mt_axis_reallocate(mt->shape.axis, d, mt->shape.d);
    mt_set_shape(mt, d, axis);

    return mt;
}

mt_t *mt_zeros(const zend_uchar type, MT_AXIS_PARAMS)
{
    mt_t *mt = mt_init_all_t(type, d, axis);
    mt_val_t *val;

    MT_FOREACH_VAL(mt, val) {
        switch (type)
        {
        case IS_MT_LONG:
            val->lval = 0;
            break;
        case IS_MT_DOUBLE:
            val->dval = 0;
            break;
        }
    } MT_FOREACH_END();

    return mt;
}

mt_t *mt_eye(const zend_uchar type, MT_AXIS_PARAMS, const zend_long k)
{
    mt_t *mt = mt_init_all_t(type, d, axis);
    // zend_long i, j, ak = k < 0 ? -k : k;
    // TODO

    return mt;
}

static zend_always_inline zend_ulong mt_axis_prod(const zend_ulong from, MT_AXIS_PARAMS)
{
    zend_ulong p, i;

    for (p = 1, i = from; i < d; i++)
    {
        p *= axis[i];
    }

    return p;
}

zend_always_inline zend_ulong mt_get_index(const mt_t *mt, const zend_ulong idxs[])
{
    zend_ulong index, i;

    for (index = 0, i = 0; i < mt->shape.d; i++)
    {
        index += idxs[i] * mt_axis_prod(i + 1, mt->shape.d, mt->shape.axis);
    }

    return index;
}

zend_always_inline bool mt_isset(const mt_t *mt, const zend_ulong idxs[])
{
    return MT_ISSET_P(mt, mt_get_index(mt, idxs));
}

bool mt_append(mt_t *mt, const mt_val_t *buffer, const zend_ulong size)
{
    bool result;
    zend_ulong i, j;

    if (IS_VALID_P(mt)) {
        if (IS_MT_EMPTY_P(mt)) {
            zend_ulong axis[] = { size };
            mt_set_shape(mt, 1, axis);
            mt->buffer = mt_val_init(mt->size);
        } else {
            zend_ulong axis[mt->shape.d + 1];
            memcpy(&axis, mt->shape.axis, mt->shape.d * sizeof(zend_ulong));
            axis[mt->shape.d] = size;
            mt_reallocate(mt, mt->shape.d + 1, axis);
        }

        for (i = mt->size - size, j = 0; j < size; i++, j++) {
            mt->buffer[i] = buffer[j];
        }

        result = true;
    } else {
        result = false;
    }

    return result;
}

static void mt_lval_copy(mt_val_t *dest, const zend_long *buffer, const zend_ulong size)
{
    zend_ulong i;
    for (i = 0; i < size; i++) {
        dest[i].lval = buffer[i];
    }
}

static void mt_dval_copy(mt_val_t *dest, const double *buffer, const zend_ulong size)
{
    zend_ulong i;
    for (i = 0; i < size; i++) {
        dest[i].dval = buffer[i];
    }
}

bool mt_lappend(mt_t *mt, const zend_long *buffer, const zend_ulong size)
{
    mt_val_t *vals;
    bool result;

    if (IS_VALID_P(mt) && size > 0) {    
        vals = mt_val_init(size);
        mt_lval_copy(vals, buffer, size);
        result = mt_append(mt, vals, size);
        MT_FREE_P(vals);
        
        result = true;
    } else {
        result = false;
    }

    return result;
}

bool mt_dappend(mt_t *mt, const double *buffer, const zend_ulong size)
{
    mt_val_t *vals;
    bool result;

    if (IS_VALID_P(mt) && size > 0) {    
        vals = mt_val_init(size);
        mt_dval_copy(vals, buffer, size);
        result = mt_append(mt, vals, size);
        MT_FREE_P(vals);

        result = true;
    } else {
        result = false;
    }

    return result;
}

zend_always_inline void mt_free(mt_t *mt)
{
    if (IS_VALID_P(mt)) {
        MT_FREE_P(mt->buffer);
        MT_FREE_P(mt->shape.axis);
        MT_FREE_P(mt);
    }
}

zend_always_inline mt_val_t mt_get_val_by_idx(const mt_t *mt, const zend_ulong index)
{
    return mt->buffer[index];
}

zend_always_inline zend_long mt_get_lval_by_idx(const mt_t *mt, const zend_ulong index)
{
    return mt->buffer[index].lval;
}

zend_always_inline double mt_get_dval_by_idx(const mt_t *mt, const zend_ulong index)
{
    return mt->buffer[index].dval;
}

zend_always_inline mt_val_t mt_get_val(const mt_t *mt, const zend_ulong idxs[])
{
    return mt_get_val_by_idx(mt, mt_get_index(mt, idxs));
}

zend_always_inline zend_long mt_get_lval(const mt_t *mt, const zend_ulong idxs[])
{
    return mt_get_lval_by_idx(mt, mt_get_index(mt, idxs));
}

zend_always_inline double mt_get_dval(const mt_t *mt, const zend_ulong idxs[])
{
    return mt_get_dval_by_idx(mt, mt_get_index(mt, idxs));
}

zend_always_inline void mt_set_lval_by_idx(mt_t *mt, const zend_long val, const zend_ulong index)
{
    if (MT_ISSET_P(mt, index)) {
        mt->buffer[index].lval = val;
    }
}

zend_always_inline void mt_set_dval_by_idx(mt_t *mt, const double val, const zend_ulong index)
{
    if (MT_ISSET_P(mt, index)) {
        mt->buffer[index].dval = val;
    }
}

zend_always_inline void mt_set_lval(mt_t *mt, const zend_long val, const zend_ulong idxs[])
{
    mt_set_lval_by_idx(mt, val, mt_get_index(mt, idxs));
}

zend_always_inline void mt_set_dval(mt_t *mt, const double val, const zend_ulong idxs[])
{
    mt_set_dval_by_idx(mt, val, mt_get_index(mt, idxs));
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
