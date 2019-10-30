#include "mt_array.h"
#include "../common.h"

static void _mt_to_hash(const mt_t *mt, HashTable *ht, const zend_ulong n, const zend_ulong *idxs)
{
    zend_ulong i, nn, *nidxs;
    zval val;

    nn = n + 1;
    nidxs = CALLOC(nn, zend_ulong);
    
    if (n > 0) {
        memcpy(nidxs, idxs, n * sizeof(zend_ulong));
    }
    
    for (i = 0; i < mt->shape->axes[n]; i++) {        
        nidxs[n] = i;

        if (nn < mt->shape->d) {
            HashTable *arr = nump_new_array(0);
            _mt_to_hash(mt, arr, nn, nidxs);
            ZVAL_ARR(&val, arr);
            zend_hash_next_index_insert_new(ht, &val);
        } else if (mt_isset(mt, nidxs)) {
            ZVAL_DOUBLE(&val, mt_get_value(mt, nidxs));
            zend_hash_next_index_insert_new(ht, &val);
        }
    }

    FREE_P(nidxs);
}

zend_always_inline HashTable *mt_to_hash(const mt_t *mt)
{
    HashTable *ht = nump_new_array(0);

    if (!IS_MT_EMPTY_P(mt)) {
        _mt_to_hash(mt, ht, 0, NULL);
    }

    return ht;
}

HashTable *mt_to_1d_hash(const mt_t *mt)
{
    HashTable *ht = nump_new_array(0);
    zval val;
    double *el;

    if (IS_MT_VALID_P(mt)) {
        MT_FOREACH_VAL(mt, el) {
            ZVAL_DOUBLE(&val, *el);
            zend_hash_next_index_insert_new(ht, &val);
        } MT_FOREACH_END();
    }

    return ht;
}

HashTable *mt_shape_to_hash(const mt_shape_t *shape)
{
    HashTable *ht = nump_new_array(0);
    zend_ulong i;
    zval val;

    if (IS_VALID_P(shape)) {
        for (i = 0; i < shape->d; i++) {
            ZVAL_LONG(&val, shape->axes[i]);
            zend_hash_next_index_insert_new(ht, &val);
        }
    }

    return ht;
}

zend_always_inline mt_t *hash_to_mt(HashTable *ht)
{
    mt_t *mt = mt_init(NULL, NULL);
    
    if (hash_to_mt_ex(ht, mt)) {
        return mt;
    }

    mt_free(mt);

    return NULL;
}

static bool _hash_to_mt_ex(HashTable *ht, mt_t *mt, const zend_ulong n, const zend_ulong *idxs)
{
    zval *val;
    zend_ulong i, nn, *nidxs;

    nn = n + 1;
    nidxs = CALLOC(nn, zend_ulong);

    if (n > 0) {
        memcpy(nidxs, idxs, n * sizeof(zend_ulong));
    }

    if (nn == mt->shape->d && mt->shape->axes[n] != zend_array_count(ht)) {
        THROW_EXCEPTION_AA(
            "Expected array of length %d, %d given",
            mt->shape->axes[n],
            zend_array_count(ht)
        );
        goto error;
    }

    for (i = 0; i < mt->shape->axes[n]; i++) {
        val = zend_hash_index_find(ht, i);

        if (!IS_VALID_P(val)) {
            THROW_INDEX_OUT_OF_RANGE(i, zend_array_count(ht));
            goto error;
        }
        
        nidxs[n] = i;

        if (nn < mt->shape->d ) {
            if (!_hash_to_mt_ex(Z_ARRVAL_P(val), mt, nn, nidxs)) {
                goto error;
            }
        } else {
            switch (Z_TYPE_P(val))
            {
            case IS_LONG:
                mt_set_value(mt, (double) Z_LVAL_P(val), nidxs);
                break;
            case IS_DOUBLE:
                mt_set_value(mt, Z_DVAL_P(val), nidxs);
                break;
            case IS_NULL:
                THROW_NULL("a number");
                goto error;
            default:
                THROW_TYPE("a number", val);
                goto error;
            }
        }
    }

    FREE_P(nidxs);
    return true;

error:
    FREE_P(nidxs);
    return false;
}

bool hash_to_mt_ex(HashTable *ht, mt_t *mt)
{
    if (!IS_VALID_P(ht) || !IS_VALID_P(mt)) {
        return false;
    }

    zend_ulong size, d;
    zval *val;

    size = zend_array_count(ht);

    if (size < 1) {
        return true;
    }

    mt->shape = mt_shape_init(1);
    mt->shape->axes[0] = size;

    for (
        val = zend_hash_index_find(ht, 0);
        IS_VALID_P(val) && Z_TYPE_P(val) == IS_ARRAY;
        val = zend_hash_index_find(Z_ARRVAL_P(val), 0)
    ) {
        d = mt->shape->d++;
        mt->shape->axes = REALLOC(mt->shape->axes, mt->shape->d, d, zend_ulong);
        mt->shape->axes[d] = zend_array_count(Z_ARRVAL_P(val));
        size *= mt->shape->axes[d];
    }

    if (size < 1) {
        mt_shape_free(mt->shape);
        mt->shape = NULL;
        return true;
    }

    mt->buffer = mt_buffer_init(size);

    if (_hash_to_mt_ex(ht, mt, 0, NULL)) {
        return true;
    }

    mt_shape_free(mt->shape);
    mt_buffer_free(mt->buffer);

    mt->shape = NULL;
    mt->buffer = NULL;

    return false;
}

mt_shape_t *hash_to_mt_shape(HashTable *ht)
{
    mt_shape_t *shape = NULL;
    zend_ulong i = 0;
    zval *val;

    if (IS_VALID_P(ht)) {
        shape = mt_shape_init(zend_array_count(ht));

        ZEND_HASH_FOREACH_VAL(ht, val) {
            shape->axes[i++] = Z_LVAL_P(val);
        } ZEND_HASH_FOREACH_END();
    }

    return shape;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
