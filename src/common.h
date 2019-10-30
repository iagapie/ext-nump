#ifndef NUMP_COMMON_H
#define NUMP_COMMON_H

#include "php.h"
#include "zend_exceptions.h"
#include "ext/spl/spl_exceptions.h"

static zend_always_inline int nump_is_valid(const void *ptr)
{
    return (ptr != NULL);
}

#define IS_VALID_P(p) nump_is_valid(p)

#define CALLOC(number, type) \
    ((type *) ecalloc(number, sizeof(type)))

static zend_always_inline void *nump_reallloc(void *ptr, const zend_ulong length, const zend_ulong current, const size_t size)
{
    if (length == current || length == 0 || size == 0) {
        return ptr;
    }

    if (IS_VALID_P(ptr)) {
        ptr = erealloc(ptr, length * size);
    }

    return ptr;
}

#define REALLOC(ptr, length, current, type) \
    ((type *) nump_reallloc(ptr, length, current, sizeof(type)))

#define FREE_P(ptr) \
    if (IS_VALID_P(ptr)) { \
        efree(ptr); \
        ptr = NULL; \
    }

#define FREE_AND_REPLACE(ptr, repl) \
do { \
    void *_repl = repl; \
    FREE_P(ptr); \
    ptr = _repl; \
} while (0)

/**
 * Used to determine if a string zval is equal to a string literal.
 * Eg. ZVAL_EQUALS_STRING(value, "test")
 */
#define ZVAL_EQUALS_STRING(z, s) zend_string_equals_literal(Z_STR_P(z), s)

#define SERIALIZE_SET_ZSTR(s) \
    *buffer = (unsigned char *) estrndup(ZSTR_VAL((s)), ZSTR_LEN((s))); \
    *length = ZSTR_LEN((s));

#define FCI_PARAMS zend_fcall_info fci, zend_fcall_info_cache fci_cache
#define FCI_ARGS fci, fci_cache

#define PHP_NUMP_NS_NAME "Nump"
#define PHP_NUMP_NS(cls) PHP_NUMP_NS_NAME "\\" #cls
#define PHP_NUMP_NS_FE(fe) PHP_NUMP_NS_NAME, fe

#define PHP_NUMP_FE(fe) \
    ZEND_NS_FE(PHP_NUMP_NS_NAME, fe, arginfo_##fe)

#define PHP_NUMP_ME(cls, name) \
    PHP_ME(cls, name, arginfo_##cls##_##name, ZEND_ACC_PUBLIC)

#define PHP_NUMP_AME(cls, name) \
    PHP_ABSTRACT_ME(cls, name, arginfo_##cls##_##name)

#define PHP_NUMP_SME(cls, name) \
    PHP_ME(cls, name, arginfo_##cls##_##name, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

#define THROW_ERROR(s) nump_throw_exception(zend_ce_error, s)
#define THROW_ERROR_A(s, a1) nump_throw_exception(zend_ce_error, s, a1)
#define THROW_ERROR_AA(s, a1, a2) nump_throw_exception(zend_ce_error, s, a1, a2)

#define THROW_EXCEPTION(s) nump_throw_exception(zend_ce_exception, s)
#define THROW_EXCEPTION_A(s, a1) nump_throw_exception(zend_ce_exception, s, a1)
#define THROW_EXCEPTION_AA(s, a1, a2) nump_throw_exception(zend_ce_exception, s, a1, a2)

#define THROW_TYPE_ERROR(s, z) nump_throw_exception( \
    zend_ce_type_error, s, zend_get_type_by_const(Z_TYPE_P(z)))

#define THROW_TYPE_ERROR_A(s, a1, z) nump_throw_exception( \
    zend_ce_type_error, s, a1, zend_get_type_by_const(Z_TYPE_P(z)))

#define THROW_TYPE_ERROR_AA(s, a1, a2, z) nump_throw_exception( \
    zend_ce_type_error, s, a1, a2, zend_get_type_by_const(Z_TYPE_P(z)))

#define THROW_NULL(s) THROW_ERROR_A("Expected parameter 1 to be %s, null given", s)
#define THROW_TYPE(s, z) THROW_TYPE_ERROR_A("Expected parameter 1 to be %s, %s given", s, z)

#define THROW_INDEX_OUT_OF_RANGE(index, max) nump_throw_exception( \
    spl_ce_OutOfRangeException, \
    max == 0 \
        ? "Index out of range: %d" \
        : "Index out of range: %d, expected 0 <= x <= %d", \
    index, \
    max - 1)

void nump_throw_exception(zend_class_entry *ce, const char *format, ...);

HashTable *nump_new_array(uint32_t size);

double nump_rand();
double nump_randn();

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
