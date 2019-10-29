#ifndef NUMP_MT_BASE_H
#define NUMP_MT_BASE_H

#include <stdbool.h>

#include "php.h"
#include "../common.h"

typedef struct _matrix_shape_t {
    zend_ulong *axes;
    zend_ulong d;
} matrix_shape_t;

typedef struct _matrix_buffer_t {
    double *data;
    zend_ulong size;
} matrix_buffer_t;

typedef struct _matrix_t {
    matrix_shape_t *shape;
    matrix_buffer_t *buffer;
} matrix_t;

typedef matrix_shape_t mt_shape_t;
typedef matrix_buffer_t mt_buffer_t;
typedef matrix_t mt_t;

static zend_always_inline bool is_mt_buffer_valid(const mt_buffer_t *buffer)
{
    return IS_VALID_P(buffer) && IS_VALID_P(buffer->data);
}

static zend_always_inline bool is_mt_shape_valid(const mt_shape_t *shape)
{
    return IS_VALID_P(shape) && IS_VALID_P(shape->axes);
}

static zend_always_inline bool is_mt_valid(const mt_t *mt)
{
    return IS_VALID_P(mt) && is_mt_buffer_valid(mt->buffer) && is_mt_shape_valid(mt->shape);
}

#define IS_MT_BUFFER_VALID(buffer) is_mt_buffer_valid(&(buffer))
#define IS_MT_BUFFER_VALID_P(buffer_p) IS_MT_BUFFER_VALID(*(buffer_p))

#define IS_MT_SHAPE_VALID(shape) is_mt_shape_valid(&(shape))
#define IS_MT_SHAPE_VALID_P(shape_p) IS_MT_SHAPE_VALID(*(shape_p))

#define IS_MT_VALID(mt) is_mt_valid(&(mt))
#define IS_MT_VALID_P(mt_p) IS_MT_VALID(*(mt_p))

static zend_always_inline zend_ulong mt_get_size(const mt_t* mt)
{
    if (IS_MT_VALID_P(mt)) {
        return mt->buffer->size;
    }

    return 0;
}

#define MT_SIZE(mt) mt_get_size(&(mt))
#define MT_SIZE_P(mt_p) MT_SIZE(*(mt_p))

#define IS_MT_EMPTY(mt) (MT_SIZE(mt) == 0)
#define IS_MT_EMPTY_P(mt_p) IS_MT_EMPTY(*(mt_p))

static zend_always_inline bool mt_isset_index(const mt_t *mt, const zend_ulong index)
{
    return IS_MT_VALID_P(mt)
        && index >= 0
        && index < mt->buffer->size;
}

#define MT_ISSET(mt, index) mt_isset_index(&(mt), index)
#define MT_ISSET_P(mt_p, index) MT_ISSET(*(mt_p), index)

#define MT_FOREACH(mt) \
do { \
    const mt_t *__mt = mt; \
    const mt_buffer_t *__buffer = __mt->buffer; \
    const zend_ulong __size = __buffer->size; \
    double *_data = __buffer->data; \
    zend_ulong _i; \
    for (_i = 0; _i < __size; _i++) { \
        double *_v = &(_data[_i]);

#define MT_FOREACH_END() \
    } \
} while (0)

#define MT_FOREACH_VAL(mt, _val) \
	MT_FOREACH(mt); \
	_val = _v;

#define MT_FOREACH_VAL_IDX(mt, _val, _idx) \
	MT_FOREACH(mt); \
	_val = _v; \
    _idx = _i;

double *mt_buffer_data_init(const zend_ulong size);
double *mt_buffer_data_clone(const double *src, const zend_ulong size);

mt_buffer_t *mt_buffer_init(const zend_ulong size);
mt_buffer_t *mt_buffer_clone(const mt_buffer_t *src);
void mt_buffer_free(mt_buffer_t *buffer);

zend_ulong *mt_shape_axes_init(const zend_ulong d);
zend_ulong *mt_shape_axes_clone(const zend_ulong *src, const zend_ulong d);

mt_shape_t *mt_shape_init(const zend_ulong d);
mt_shape_t *mt_shape_clone(const mt_shape_t *src);
void mt_shape_free(mt_shape_t *shape);

mt_t *mt_init(mt_buffer_t *buffer, mt_shape_t *shape);
mt_t *mt_init_by_shape(mt_shape_t *shape);
mt_t *mt_clone(const mt_t *src);
void mt_free(mt_t *mt);

zend_ulong mt_shape_prod(const zend_ulong from, const mt_shape_t *shape);
zend_ulong mt_build_index(const mt_t *mt, const zend_ulong idxs[]);
bool mt_isset(const mt_t *mt, const zend_ulong idxs[]);

void mt_set(mt_t *mt, const double value, const zend_ulong index);
void mt_set_value(mt_t *mt, const double value, const zend_ulong idxs[]);

double mt_get(const mt_t *mt, const zend_ulong index);
double mt_get_value(const mt_t *mt, const zend_ulong idxs[]);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
