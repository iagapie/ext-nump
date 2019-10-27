#ifndef PHP_NUMP_COMMON_H
#define PHP_NUMP_COMMON_H

#include "php.h"
#include "zend_exceptions.h"
#include "ext/spl/spl_exceptions.h"

#include "../nump/mt_base.h"

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

#define MT_ZVAL_T(z, v, t) \
do { \
    zval *__z = z; \
    mt_val_t __v = v; \
    if (t == IS_MT_LONG) { \
        ZVAL_LONG(z, __v.lval); \
    } else { \
        ZVAL_DOUBLE(z, __v.dval); \
    } \
} while(0)

#define SHAPES_NOT_ALIGNED(a, b) nump_throw_exception( \
    zend_ce_error, \
    "Shapes %s and %s not aligned.", a, b)

#define INDEX_OUT_OF_RANGE(index) nump_throw_exception( \
    spl_ce_OutOfRangeException, \
    "Index out of range: %d", index)

void nump_throw_exception(zend_class_entry *ce, const char *format, ...);

HashTable *mt_new_array(uint32_t size);
HashTable *mt_to_hash(const mt_t *mt);
HashTable *mt_to_hash_1d(const mt_t *mt);
HashTable *mt_to_shape(const mt_t *mt);

bool hash_to_mt_ex(HashTable *ht, mt_t *mt);
mt_t *hash_to_mt(HashTable *ht);

void hash_to_shape(HashTable *ht, zend_ulong shape[]);
void shape_to_hash(MT_AXES_PARAMS, HashTable *ht);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
