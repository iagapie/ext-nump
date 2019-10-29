#include "mt_base.h"

zend_always_inline double *mt_buffer_data_init(const zend_ulong size)
{
    if (size > 0) {
        return CALLOC(size, double);
    }

    return NULL;
}

double *mt_buffer_data_clone(const double *src, const zend_ulong size)
{
    double *dest = NULL;

    if (IS_VALID_P(src) && size > 0) {
        dest = mt_buffer_data_init(size);
        memcpy(dest, src, size * sizeof(double));
    }

    return dest;
}

mt_buffer_t *mt_buffer_init(const zend_ulong size)
{
    mt_buffer_t *buffer = CALLOC(1, mt_buffer_t);

    buffer->data = mt_buffer_data_init(size);
    buffer->size = size;

    return buffer;
}

mt_buffer_t *mt_buffer_clone(const mt_buffer_t *src)
{
    mt_buffer_t *dest = NULL;

    if (IS_VALID_P(src)) {
        dest = CALLOC(1, mt_buffer_t);
        dest->data = mt_buffer_data_clone(src->data, src->size);
        dest->size = src->size;
    }

    return dest;
}

zend_always_inline void mt_buffer_free(mt_buffer_t *buffer)
{
    if (IS_VALID_P(buffer)) {
        FREE_P(buffer->data);
        FREE_P(buffer);
    }
}

zend_always_inline zend_ulong *mt_shape_axes_init(const zend_ulong d)
{
    if (d > 0) {
        return CALLOC(d, zend_ulong);
    }

    return NULL;
}

zend_ulong *mt_shape_axes_clone(const zend_ulong *src, const zend_ulong d)
{
    zend_ulong *dest = NULL;

    if (IS_VALID_P(src) && d > 0) {
        dest = mt_shape_axes_init(d);
        memcpy(dest, src, d * sizeof(zend_ulong));
    }

    return dest;
}

mt_shape_t *mt_shape_init(const zend_ulong d)
{
    mt_shape_t *shape = CALLOC(1, mt_shape_t);

    shape->axes = mt_shape_axes_init(d);
    shape->d = d;

    return shape;
}

mt_shape_t *mt_shape_clone(const mt_shape_t *src)
{
    mt_shape_t *dest = NULL;

    if (IS_VALID_P(src)) {
        dest = CALLOC(1, mt_shape_t);
        dest->axes = mt_shape_axes_clone(src->axes, src->d);
        dest->d = src->d;
    }

    return dest;
}

zend_always_inline void mt_shape_free(mt_shape_t *shape)
{
    if (IS_VALID_P(shape)) {
        FREE_P(shape->axes);
        FREE_P(shape);
    }
}

mt_t *mt_init(mt_buffer_t *buffer, mt_shape_t *shape)
{
    mt_t *mt = CALLOC(1, mt_t);

    mt->buffer = buffer;
    mt->shape = shape;

    return mt;
}

zend_always_inline mt_t *mt_init_by_shape(mt_shape_t *shape)
{
    if (IS_VALID_P(shape)) {
        return mt_init(mt_buffer_init(mt_shape_prod(0, shape)), shape);
    }

    return NULL;
}

mt_t *mt_clone(const mt_t *src)
{
    mt_t *dest = NULL;

    if (IS_VALID_P(src)) {
        dest = mt_init(mt_buffer_clone(src->buffer), mt_shape_clone(src->shape));
    }

    return dest;
}

zend_always_inline void mt_free(mt_t *mt)
{
    if (IS_VALID_P(mt)) {
        mt_buffer_free(mt->buffer);
        mt_shape_free(mt->shape);
        FREE_P(mt);
    } 
}

zend_always_inline zend_ulong mt_shape_prod(const zend_ulong from, const mt_shape_t *shape)
{
    zend_ulong p = 0, i;

    if (IS_MT_SHAPE_VALID_P(shape)) {
        for (p = 1, i = from; i < shape->d; i++) {
            p *= shape->axes[i];
        }
    }

    return p;
}

zend_always_inline zend_ulong mt_build_index(const mt_t *mt, const zend_ulong idxs[])
{
    zend_ulong index, i;

    for (index = 0, i = 0; i < mt->shape->d; i++) {
        index += idxs[i] * mt_shape_prod(i + 1, mt->shape);
    }

    return index;
}

zend_always_inline bool mt_isset(const mt_t *mt, const zend_ulong idxs[])
{
    return IS_MT_VALID_P(mt) && MT_ISSET_P(mt, mt_build_index(mt, idxs));
}

zend_always_inline void mt_set(mt_t *mt, const double value, const zend_ulong index)
{
    mt->buffer->data[index] = value;
}

zend_always_inline void mt_set_value(mt_t *mt, const double value, const zend_ulong idxs[])
{
    mt_set(mt, value, mt_build_index(mt, idxs));
}

zend_always_inline double mt_get(const mt_t *mt, const zend_ulong index)
{
    return mt->buffer->data[index];
}

zend_always_inline double mt_get_value(const mt_t *mt, const zend_ulong idxs[])
{
    return mt_get(mt, mt_build_index(mt, idxs));
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
