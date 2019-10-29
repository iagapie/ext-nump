#ifndef NUMP_MT_ARRAY_H
#define NUMP_MT_ARRAY_H

#include "php.h"
#include "mt_base.h"

HashTable *mt_to_hash(const mt_t *mt);
HashTable *mt_to_1d_hash(const mt_t *mt);
HashTable *mt_shape_to_hash(const mt_shape_t *shape);

mt_t *hash_to_mt(HashTable *ht);

bool hash_to_mt_ex(HashTable *ht, mt_t *mt);

mt_shape_t *hash_to_mt_shape(HashTable *ht);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
