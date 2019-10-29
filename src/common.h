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
    efree(ptr); \
    ptr = _repl; \
} while (0)

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

#define EX_SHAPES_NOT_ALIGNED(a, b) nump_throw_exception( \
    zend_ce_error, \
    "Shapes %s and %s not aligned.", a, b)

#define EX_INDEX_OUT_OF_RANGE(index) nump_throw_exception( \
    spl_ce_OutOfRangeException, \
    "Index out of range: %d.", index)

#define EX_INDEX_NOT_FOUND() nump_throw_exception( \
    spl_ce_OutOfBoundsException, \
    "Index not found.")

#define EX_SIZES_NOT_MATCH(a, b) nump_throw_exception( \
    spl_ce_UnexpectedValueException, \
    "Sizes don't match: %ld expected, %ld given.", a, b)

#define EX_VALUE_TYPE(z) nump_throw_exception( \
    spl_ce_UnexpectedValueException, \
    "Value must be of type integer or double, %d given.", \
    zend_get_type_by_const(Z_TYPE_P(z)))

#define EX_THROW(str) \
    nump_throw_exception(zend_ce_error, str)

#define EX_ARGUMENT_NOT_VALID_TYPE() EX_THROW("Argument is not a valid type.")
#define EX_CALLABLE() EX_THROW("Callable should return a double.")
#define EX_MT_NOT_SQUARE() EX_THROW("Matrix is not a square.")
#define EX_MT_IS_SINGULAR() EX_THROW("Matrix is singular.")

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
