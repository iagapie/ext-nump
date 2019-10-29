#ifndef NUMP_PHP_MT_O_H
#define NUMP_PHP_MT_O_H

#include "../../nump/mt_base.h"

#define Z_MT(z)         ((php_mt_t*) Z_OBJ(z))->mt
#define Z_MT_P(z)       Z_MT(*z)
#define THIS_MT()       Z_MT_P(getThis())

#define ZVAL_MT(z, d)   ZVAL_OBJ(z, php_mt_create_object_ex(d))

#define RETURN_MT(d)                        \
do {                                        \
    mt_t *_d = d;                           \
    if (_d) {                               \
        ZVAL_MT(return_value, _d);          \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

typedef struct _php_matrix_t {
    zend_object  std;
    mt_t  *mt;
} php_matrix_t;

typedef php_matrix_t php_mt_t;

/**
 * Creates a new zend_object using an existing matrix.
 */
zend_object *php_mt_create_object_ex(mt_t *mt);

/**
 * Creates a new matrix zend_object.
 */
zend_object *php_mt_create_object(zend_class_entry *ce);

/**
 * Creates an object clone of a matrix.
 */
zend_object *php_mt_create_object_clone(mt_t *mt);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
