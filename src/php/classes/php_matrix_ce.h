#ifndef PHP_NUMP_MATRIX_CE_H
#define PHP_NUMP_MATRIX_CE_H

#include "php.h"

#include "../common.h"
#include "../arginfo.h"

#define PHP_MT_ME_LIST(cls) \
    PHP_NUMP_ME(cls, __construct) \
    PHP_NUMP_ME(cls, value) \
    PHP_NUMP_ME(cls, negative) \
    PHP_NUMP_ME(cls, transpose) \
    PHP_NUMP_ME(cls, inverse) \
    PHP_NUMP_ME(cls, determinant) \
    PHP_NUMP_ME(cls, mean) \
    PHP_NUMP_ME(cls, shape) \
    PHP_NUMP_ME(cls, size) \
    PHP_NUMP_ME(cls, isEmpty) \
    PHP_NUMP_ME(cls, isSquare) \
    PHP_NUMP_ME(cls, isSingular) \
    PHP_NUMP_ME(cls, toArray) \
    PHP_NUMP_ME(cls, toFlatten) \
    PHP_NUMP_ME(cls, round) \
    PHP_NUMP_ME(cls, apply) \
    PHP_NUMP_ME(cls, solve) \
    PHP_NUMP_ME(cls, add) \
    PHP_NUMP_ME(cls, sub) \
    PHP_NUMP_ME(cls, mul) \
    PHP_NUMP_ME(cls, div) \
    PHP_NUMP_ME(cls, dot) \
    PHP_NUMP_ME(cls, __toString)

#define PHP_MT_ARGINFO_LIST(cls) \
    ARGINFO_ARRAY(cls##___construct, array); \
    ARGINFO_ZVAL(cls##_value, val); \
    ARGINFO_NONE(cls##_negative); \
    ARGINFO_NONE(cls##_transpose); \
    ARGINFO_NONE(cls##_inverse); \
    ARGINFO_NONE(cls##_determinant); \
    ARGINFO_NONE(cls##_mean); \
    ARGINFO_NONE(cls##_shape); \
    ARGINFO_NONE(cls##_size); \
    ARGINFO_NONE(cls##_isEmpty); \
    ARGINFO_NONE(cls##_isSquare); \
    ARGINFO_NONE(cls##_isSingular); \
    ARGINFO_NONE(cls##_toArray); \
    ARGINFO_NONE(cls##_toFlatten); \
    ARGINFO_OPTIONAL_LONG_LONG(cls##_round, precision, mode); \
    ARGINFO_CALLABLE(cls##_apply, callback); \
    ARGINFO_OBJ(cls##_solve, b, PHP_NUMP_NS(Matrix)); \
    ARGINFO_ZVAL(cls##_add, val); \
    ARGINFO_ZVAL(cls##_sub, val); \
    ARGINFO_ZVAL(cls##_mul, val); \
    ARGINFO_ZVAL(cls##_div, val); \
    ARGINFO_ZVAL(cls##_dot, val); \
    ARGINFO_NONE(cls##___toString);

PHP_MT_ARGINFO_LIST(Matrix);

extern zend_class_entry *php_matrix_ce;

void php_register_matrix();

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
