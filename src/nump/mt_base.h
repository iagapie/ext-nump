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
        zend_ulong *axes;
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

#define IS_MT_UNDEF                     0
#define IS_MT_LONG                      1
#define IS_MT_DOUBLE                    2

#define MT_TYPE(mt)                     mt_val_get_type(&(mt))
#define MT_TYPE_P(mt_p)                 MT_TYPE(*(mt_p))

#define MT_SIZE(mt)                     mt_get_size(&(mt))
#define MT_SIZE_P(mt_p)                 MT_SIZE(*(mt_p))

#define IS_MT_EMPTY(mt)                 (MT_SIZE(mt) == 0)
#define IS_MT_EMPTY_P(mt_p)             IS_MT_EMPTY(*(mt_p))

#define IS_VALID_P(p)                   mt_is_valid(p)

#define RETURN_MT_VAL(mt, val) \
do { \
    if (mt->type == IS_MT_LONG) { \
        RETURN_LONG(val.lval); \
    } else { \
        RETURN_DOUBLE(val.dval); \
    } \
} while (0)

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

#define MT_AXES_PARAMS          const zend_ulong d, const zend_ulong axes[]

#define MT_FOREACH(mt)                                      \
do {                                                        \
    const mt_t *__mt = mt;                                  \
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

static zend_always_inline double mt_get_mt_dval(const mt_t *mt, const mt_val_t *val)
{
    return mt->type == IS_MT_LONG ? (double) val->lval : val->dval;
}

static zend_always_inline zend_long mt_get_mt_lval(const mt_t *mt, const mt_val_t *val)
{
    return mt->type == IS_MT_LONG ? val->lval : (zend_long) val->dval;
}

#define MT_GET_DVAL(mt, val)            mt_get_mt_dval(&(mt), &(val))
#define MT_GET_DVAL_P(mt_p, val_p)      MT_GET_DVAL(*(mt_p), *(val_p))

#define MT_GET_LVAL(mt, val)            mt_get_mt_lval(&(mt), &(val))
#define MT_GET_LVAL_P(mt_p, val_p)      MT_GET_LVAL(*(mt_p), *(val_p))

#define MT_SET_VAL(mt, val, v) \
do { \
    mt_t __mt = (mt); \
    mt_val_t *__val = &(val); \
    if (__mt.type == IS_MT_LONG) { \
        __val->lval = (zend_long) v; \
    } else { \
        __val->dval = (double) v; \
    } \
} while (0)

#define MT_SET_VAL_P(mt_p, val_p, v)    MT_SET_VAL(*(mt_p), *(val_p), v)

mt_val_t *mt_val_init(const zend_ulong n);
mt_val_t *mt_val_clone(const mt_val_t *src, const zend_ulong n);
mt_val_t *mt_val_reallocate(mt_val_t *val, const zend_ulong length, const zend_ulong current);

zend_ulong *mt_axes_init(const zend_ulong n);
zend_ulong *mt_axes_clone(const zend_ulong *src, const zend_ulong n);
zend_ulong *mt_axes_reallocate(zend_ulong *axes, const zend_ulong length, const zend_ulong current);

mt_t *mt_init();
mt_t *mt_init_t(const zend_uchar type);
mt_t *mt_init_all_t(const zend_uchar type, MT_AXES_PARAMS);
mt_t *mt_init_all(MT_AXES_PARAMS);
mt_t *mt_clone(const mt_t *src);
mt_t *mt_reallocate(mt_t *mt, MT_AXES_PARAMS);

zend_ulong mt_get_index(const mt_t *mt, const zend_ulong idxs[]);

bool mt_isset(const mt_t *mt, const zend_ulong idxs[]);
bool mt_is_square(const mt_t *mt);
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

zend_string *mt_axes_to_string(MT_AXES_PARAMS);
zend_string *mt_to_string(const mt_t *mt);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
