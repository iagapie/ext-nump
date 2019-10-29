#ifndef NUMP_PHP_MT_CE_H
#define NUMP_PHP_MT_CE_H

#include "php.h"

#include "../../common.h"
#include "../arginfo.h"
#include "../objects/php_mt_o.h"

#define MT_FC_OP(name, a, b) (mt_##name(a, b))

#define RETURN_MT_OP(z, op) \
do { \
    zval *__z = z; \
    mt_t *__mt = NULL, *__b; \
    switch (Z_TYPE_P(__z)) \
    { \
    case IS_LONG: \
        __mt = MT_FC_OP(op##_val, THIS_MT(), (double) Z_LVAL_P(__z)); \
        break; \
    case IS_DOUBLE: \
        __mt = MT_FC_OP(op##_val, THIS_MT(), Z_DVAL_P(__z)); \
        break; \
    case IS_ARRAY: \
        __b = hash_to_mt(Z_ARRVAL_P(__z)); \
        __mt = MT_FC_OP(op, THIS_MT(), __b); \
        mt_free(__b); \
        break; \
    case IS_OBJECT: \
        if (instanceof_function(Z_OBJCE_P(__z), php_mt_ce)) { \
            __mt = MT_FC_OP(op, THIS_MT(), Z_MT_P(__z)); \
        } else { \
            THROW_TYPE("an instance of Matrix", __z); \
        } \
        break; \
    case IS_NULL: \
        THROW_NULL("an array or a number or an instance of Matrix"); \
        break; \
    default: \
        THROW_ERROR("NOT SUPPORTED TYPE."); \
        break; \
    } \
    RETURN_MT(__mt); \
} while (0)

#define PHP_MT_ME_LIST(cls) \
    PHP_NUMP_ME(cls, __construct) \
    PHP_NUMP_ME(cls, value) \
    PHP_NUMP_ME(cls, isset) \
    PHP_NUMP_ME(cls, exp) \
    PHP_NUMP_ME(cls, log) \
    PHP_NUMP_ME(cls, log2) \
    PHP_NUMP_ME(cls, log10) \
    PHP_NUMP_ME(cls, negative) \
    PHP_NUMP_ME(cls, transpose) \
    PHP_NUMP_ME(cls, inverse) \
    PHP_NUMP_ME(cls, determinant) \
    PHP_NUMP_ME(cls, mean) \
    PHP_NUMP_ME(cls, shape) \
    PHP_NUMP_ME(cls, count) \
    PHP_NUMP_ME(cls, isEmpty) \
    PHP_NUMP_ME(cls, isSquare) \
    PHP_NUMP_ME(cls, isSingular) \
    PHP_NUMP_ME(cls, toArray) \
    PHP_NUMP_ME(cls, toFlatten) \
    PHP_NUMP_ME(cls, jsonSerialize) \
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
    ARGINFO_ZVAL(cls##_isset, val); \
    ARGINFO_NONE(cls##_exp); \
    ARGINFO_NONE(cls##_log); \
    ARGINFO_NONE(cls##_log2); \
    ARGINFO_NONE(cls##_log10); \
    ARGINFO_NONE(cls##_negative); \
    ARGINFO_NONE(cls##_transpose); \
    ARGINFO_NONE(cls##_inverse); \
    ARGINFO_NONE(cls##_determinant); \
    ARGINFO_NONE(cls##_mean); \
    ARGINFO_NONE(cls##_shape); \
    ARGINFO_NONE(cls##_count); \
    ARGINFO_NONE(cls##_isEmpty); \
    ARGINFO_NONE(cls##_isSquare); \
    ARGINFO_NONE(cls##_isSingular); \
    ARGINFO_NONE(cls##_toArray); \
    ARGINFO_NONE(cls##_toFlatten); \
    ARGINFO_NONE(cls##_jsonSerialize); \
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

extern zend_class_entry *php_mt_ce;

void php_register_mt();

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
