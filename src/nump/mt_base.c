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

zend_always_inline zend_ulong *mt_axes_init(const zend_ulong n)
{
    return MT_CALLOC(n, zend_ulong);
}

zend_ulong *mt_axes_clone(const zend_ulong *src, const zend_ulong n)
{
    zend_ulong *dest;

    if (IS_VALID_P(src)) {
        dest = mt_axes_init(n);
        memcpy(dest, src, n * sizeof(zend_ulong));
    } else {
        dest = NULL;
    }

    return dest;
}

zend_always_inline zend_ulong *mt_axes_reallocate(zend_ulong *axes, const zend_ulong length, const zend_ulong current)
{
    return (zend_ulong *) _mt_reallocate_p(axes, length, current, sizeof(zend_ulong));
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
    mt->shape.axes = NULL;
    mt->shape.d = 0;

    return mt;
}

static zend_always_inline void mt_set_shape(mt_t *mt, MT_AXES_PARAMS)
{
    zend_ulong i;
    
    if (!IS_VALID_P(mt)) {
        return;
    }

    if (!IS_VALID_P(mt->shape.axes)) {
        mt->shape.axes = mt_axes_init(d);
    }

    mt->shape.d = d;
    mt->size = 1;

    for (i = 0; i < d; i++) {
        mt->size *= mt->shape.axes[i] = axes[i];
    }
}

mt_t *mt_init_all_t(const zend_uchar type, MT_AXES_PARAMS)
{
    mt_t *mt;
    zend_ulong i;

    if (d == 0) {
        return NULL;
    }

    mt = mt_init_t(type);
    mt_set_shape(mt, d, axes);
    mt->buffer = mt_val_init(mt->size);

    return mt;
}

zend_always_inline mt_t *mt_init_all(MT_AXES_PARAMS)
{
    return mt_init_all_t(IS_MT_UNDEF, d, axes);
}

mt_t *mt_clone(const mt_t *src)
{
    mt_t *dest;

    if (IS_VALID_P(src)) {
        dest = mt_init_t(src->type);

        dest->size = src->size;
        dest->shape.d = src->shape.d;
        dest->shape.axes = mt_axes_clone(src->shape.axes, src->shape.d);
        dest->buffer = mt_val_clone(src->buffer, src->size);
    } else {
        dest = NULL;
    }

    return dest;
}

mt_t *mt_reallocate(mt_t *mt, MT_AXES_PARAMS)
{
    zend_ulong i, current;

    if (d == 0) {
        return mt;
    }

    current = mt->size;

    mt->buffer = mt_val_reallocate(mt->buffer, mt->size, current);
    mt->shape.axes = mt_axes_reallocate(mt->shape.axes, d, mt->shape.d);
    mt_set_shape(mt, d, axes);

    return mt;
}

static zend_always_inline zend_ulong mt_axes_prod(const zend_ulong from, MT_AXES_PARAMS)
{
    zend_ulong p, i;

    for (p = 1, i = from; i < d; i++) {
        p *= axes[i];
    }

    return p;
}

zend_always_inline zend_ulong mt_get_index(const mt_t *mt, const zend_ulong idxs[])
{
    zend_ulong index, i;

    for (index = 0, i = 0; i < mt->shape.d; i++) {
        index += idxs[i] * mt_axes_prod(i + 1, mt->shape.d, mt->shape.axes);
    }

    return index;
}

zend_always_inline bool mt_isset(const mt_t *mt, const zend_ulong idxs[])
{
    return MT_ISSET_P(mt, mt_get_index(mt, idxs));
}

zend_always_inline bool mt_is_square(const mt_t *mt)
{
    if (!IS_VALID_P(mt) || IS_MT_EMPTY_P(mt)) {
        return false;
    }

    zend_ulong i;
    for (i = 1; i < mt->shape.d; i++) {
        if (mt->shape.axes[i - 1] != mt->shape.axes[i]) {
            return false;
        }
    }

    return true;
}

bool mt_append(mt_t *mt, const mt_val_t *buffer, const zend_ulong size)
{
    bool result;
    zend_ulong i, j;

    if (IS_VALID_P(mt)) {
        if (IS_MT_EMPTY_P(mt)) {
            zend_ulong axes[] = { size };
            mt_set_shape(mt, 1, axes);
            mt->buffer = mt_val_init(mt->size);
        } else {
            zend_ulong axes[mt->shape.d + 1];
            memcpy(&axes, mt->shape.axes, mt->shape.d * sizeof(zend_ulong));
            axes[mt->shape.d] = size;
            mt_reallocate(mt, mt->shape.d + 1, axes);
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
        MT_FREE_P(mt->shape.axes);
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

zend_string *mt_axes_to_string(MT_AXES_PARAMS)
{
    zend_string *str, *tmp;
    zend_ulong i;

    str = zend_string_init("", 0, 0);

    for (i = 0; i < d; i++) {
        tmp = str;

        if (i == 0 || i < (d - 1)) {
            str = strpprintf(0, "%s%ld,", ZSTR_VAL(tmp), axes[i]);
        } else {
            str = strpprintf(0, "%s%ld", ZSTR_VAL(tmp), axes[i]);
        }
        
        zend_string_release(tmp);
    }

    tmp = str;
    str = strpprintf(0, "(%s)", ZSTR_VAL(tmp));
    zend_string_release(tmp);

    return str;
}

static zend_string *_mt_to_string(const mt_t *mt, const zend_ulong n, const zend_ulong *idxs)
{
    zend_string *str, *tmp1, *tmp2;
    zend_ulong i, nn, *nidxs;
    mt_val_t val;

    str = zend_string_init("", 0, 0);

    if (IS_VALID_P(mt) && !IS_MT_EMPTY_P(mt)) {
        nn = n + 1;
        nidxs = MT_CALLOC(nn, zend_ulong);

        if (n > 0) {
            memcpy(nidxs, idxs, n * sizeof(zend_ulong));
        }

        for (i = 0; i < mt->shape.axes[n]; i++) {
            nidxs[n] = i;
            tmp1 = str;

            if (i != 0) {
                str = strpprintf(0, nn < mt->shape.d ? "%s\n" : "%s ", ZSTR_VAL(tmp1));
                zend_string_release(tmp1);
                tmp1 = str;
            }

            if (nn < mt->shape.d) {
                tmp2 = _mt_to_string(mt, nn, nidxs);
                str = strpprintf(0, "%s%s", ZSTR_VAL(tmp1), ZSTR_VAL(tmp2));
                zend_string_release(tmp2);
            } else {
                val = mt_get_val(mt, nidxs);
                
                if (mt->type == IS_MT_LONG) {
                    str = strpprintf(0, "%s%ld", ZSTR_VAL(tmp1), val.lval);
                } else {
                    str = strpprintf(0, "%s%g", ZSTR_VAL(tmp1), val.dval);
                }
            }

            zend_string_release(tmp1);
        }

        MT_FREE_P(nidxs);
    }

    tmp1 = str;
    str = strpprintf(0, "[%s]", ZSTR_VAL(tmp1));
    zend_string_release(tmp1);

    return str;
}

zend_always_inline zend_string *mt_to_string(const mt_t *mt)
{
    return _mt_to_string(mt, 0, NULL);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
