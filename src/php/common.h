#ifndef PHP_NUMP_COMMON_H
#define PHP_NUMP_COMMON_H

#include "php.h"
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

void nump_throw_exception(zend_class_entry *ce, const char *format, ...);

HashTable *mt_new_array(uint32_t size);
HashTable *mt_to_hash(mt_t *mt);
HashTable *mt_to_hash_1d(mt_t *mt);
HashTable *mt_to_shape(mt_t *mt);

bool hash_to_mt_ex(HashTable *ht, mt_t *mt);
mt_t *hash_to_mt(HashTable *ht);

void hash_to_shape(HashTable *ht, zend_ulong shape[]);
void shape_to_hash(MT_AXIS_PARAMS, HashTable *ht);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
