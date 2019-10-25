#ifndef NUMP_MT_BASE_H
#define NUMP_MT_BASE_H

#include <stdbool.h>

#include "php.h"

typedef union _matrix_val_t {
    zend_long lval;
    double dval;
} matrix_val_t;

typedef struct _matrix_t {
    struct {
        zend_ulong *axis;
        zend_ulong d;
    } shape;
    matrix_val_t *buffer;
    zend_uchar type; // active type
    zend_ulong size; // total values in buffer
} matrix_t;

typedef matrix_val_t mt_val_t;
typedef matrix_t mt_t;

static zend_always_inline zend_uchar mt_val_get_type(const mt_t* mt)
{
    return mt->type;
}

static zend_always_inline zend_ulong mt_get_size(const mt_t* mt)
{
    return mt->size;
}

static zend_always_inline int mt_is_valid(const void *ptr)
{
    return (ptr != NULL);
}

#define IS_MT_UNDEF             0
#define IS_MT_LONG              1
#define IS_MT_DOUBLE            2

#define MT_TYPE(mt)             mt_val_get_type(&(mt))
#define MT_TYPE_P(mt_p)         MT_TYPE(*(mt_p))

#define MT_SIZE(mt)             mt_get_size(&(mt))
#define MT_SIZE_P(mt_p)         MT_SIZE(*(mt_p))

#define IS_MT_EMPTY(mt)         (MT_SIZE(mt) < 1)
#define IS_MT_EMPTY_P(mt_p)     IS_MT_EMPTY(*(mt_p))

#define IS_VALID_P(p)           mt_is_valid(p)

#define MT_CALLOC(number, type)                             \
    ((type *) ecalloc(number, sizeof(type)))

#define MT_FREE_P(ptr)                                      \
    if (IS_VALID_P(ptr)) {                                  \
        efree(ptr);                                         \
        ptr = NULL;                                         \
    }

#define FREE_AND_REPLACE(ptr, repl)                         \
do {                                                        \
    void *_repl = repl;                                     \
    efree(ptr);                                             \
    ptr = _repl;                                            \
} while (0)

static zend_always_inline bool mt_isset_by_index(const mt_t *mt, const zend_ulong index)
{
    return IS_VALID_P(mt)
        && IS_VALID_P(mt->buffer)
        && index >= 0
        && index < mt->size;
}

#define MT_ISSET(mt, index)     mt_isset_by_index(&(mt), index)
#define MT_ISSET_P(mt_p, index) MT_ISSET(*(mt_p), index)

#define MT_AXIS_PARAMS          const zend_ulong d, const zend_ulong axis[]

#define MT_FOREACH(mt)                                      \
do {                                                        \
    mt_t *__mt = mt;                                        \
    mt_val_t *_buffer = __mt->buffer;                       \
    zend_ulong _size = __mt->size;                          \
    zend_ulong _i;                                          \
    for (_i = 0; _i < _size; _i++) {                        \
        mt_val_t *_v = &(_buffer[_i]);

#define MT_FOREACH_END()                                    \
    }                                                       \
} while (0)

#define MT_FOREACH_VAL(mt, _val)                            \
	MT_FOREACH(mt);                                         \
	_val = _v;

#define MT_FOREACH_VAL_IDX(mt, _val, _idx)                  \
	MT_FOREACH(mt);                                         \
	_val = _v;                                              \
    _idx = _i;

mt_val_t *mt_val_init(const zend_ulong n);
mt_val_t *mt_val_clone(const mt_val_t *src, const zend_ulong n);
mt_val_t *mt_val_reallocate(mt_val_t *val, const zend_ulong length, const zend_ulong current);

zend_ulong *mt_axis_init(const zend_ulong n);
zend_ulong *mt_axis_clone(const zend_ulong *src, const zend_ulong n);
zend_ulong *mt_axis_reallocate(zend_ulong *axis, const zend_ulong length, const zend_ulong current);

mt_t *mt_init();
mt_t *mt_init_t(const zend_uchar type);
mt_t *mt_init_all_t(const zend_uchar type, MT_AXIS_PARAMS);
mt_t *mt_init_all(MT_AXIS_PARAMS);
mt_t *mt_clone(const mt_t *src);
mt_t *mt_reallocate(mt_t *mt, MT_AXIS_PARAMS);
mt_t *mt_zeros(const zend_uchar type, MT_AXIS_PARAMS);
mt_t *mt_eye(const zend_uchar type, MT_AXIS_PARAMS, const zend_long k);

zend_ulong mt_get_index(const mt_t *mt, const zend_ulong idxs[]);

bool mt_isset(const mt_t *mt, const zend_ulong idxs[]);
bool mt_append(mt_t *mt, const mt_val_t *buffer, const zend_ulong size);
bool mt_lappend(mt_t *mt, const zend_long *buffer, const zend_ulong size);
bool mt_dappend(mt_t *mt, const double *buffer, const zend_ulong size);

void mt_free(mt_t *mt);

mt_val_t mt_get_val_by_idx(const mt_t *mt, const zend_ulong index);
zend_long mt_get_lval_by_idx(const mt_t *mt, const zend_ulong index);
double mt_get_dval_by_idx(const mt_t *mt, const zend_ulong index);

mt_val_t mt_get_val(const mt_t *mt, const zend_ulong idxs[]);
zend_long mt_get_lval(const mt_t *mt, const zend_ulong idxs[]);
double mt_get_dval(const mt_t *mt, const zend_ulong idxs[]);

void mt_set_lval_by_idx(mt_t *mt, const zend_long val, const zend_ulong index);
void mt_set_dval_by_idx(mt_t *mt, const double val, const zend_ulong index);

void mt_set_lval(mt_t *mt, const zend_long val, const zend_ulong idxs[]);
void mt_set_dval(mt_t *mt, const double val, const zend_ulong idxs[]);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
