#include "php.h"
#include "zend_hash.h"
#include "zend_exceptions.h"
#include "ext/spl/spl_exceptions.h"

#include "common.h"
#include "../nump/mt_base.h"

void nump_throw_exception(zend_class_entry *ce, const char *format, ...)
{
    va_list ap;
    zend_string *str;

    va_start(ap, format);
    str = vstrpprintf(0, format, ap);
    va_end(ap);

    zend_throw_exception(ce, str->val, 0);
    zend_string_release(str);
}

HashTable *mt_new_array(uint32_t size)
{
    HashTable *ht = MT_CALLOC(1, HashTable);
	_zend_hash_init(ht, size < HT_MIN_SIZE ? HT_MIN_SIZE : size, ZVAL_PTR_DTOR, 0);
	return ht;
}

static void mt_to_hash_ex(const mt_t *mt, HashTable *ht, const zend_ulong n, const zend_ulong *idxs)
{
    zend_ulong i, nn, *nidxs;
    zval item;

    if (!IS_VALID_P(mt) || IS_MT_EMPTY_P(mt)) {        
        return;
    }

    nn = n + 1;
    nidxs = MT_CALLOC(nn, zend_ulong);
    
    if (n > 0) {
        memcpy(nidxs, idxs, n * sizeof(zend_ulong));
    }
    
    for (i = 0; i < mt->shape.axis[n]; i++) {        
        nidxs[n] = i;

        if (nn < mt->shape.d) {
            HashTable *arr = mt_new_array(0);
            mt_to_hash_ex(mt, arr, nn, nidxs);
            ZVAL_ARR(&item, arr);
            zend_hash_next_index_insert_new(ht, &item);
        } else if (mt_isset(mt, nidxs)) {
            MT_ZVAL_T(&item, mt_get_val(mt, nidxs), MT_TYPE_P(mt));
            zend_hash_next_index_insert_new(ht, &item);
        }
    }

    MT_FREE_P(nidxs);
}

static bool _hash_to_mt_ex(HashTable *ht, mt_t *mt, const zend_ulong n, const zend_ulong *idxs)
{
    zval *item;
    zend_ulong i, nn, *nidxs;
    mt_val_t val;

    nn = n + 1;
    nidxs = MT_CALLOC(nn, zend_ulong);

    if (n > 0) {
        memcpy(nidxs, idxs, n * sizeof(zend_ulong));
    }

    if (nn == mt->shape.d && mt->shape.axis[n] != zend_array_count(ht)) {
        nump_throw_exception(
            spl_ce_UnexpectedValueException,
            "Sizes don't match: %ld expected, %ld given.",
            mt->shape.axis[n],
            zend_array_count(ht)
        );
        goto error;
    }

    for (i = 0; i < mt->shape.axis[n]; i++) {
        item = zend_hash_index_find(ht, i);

        if (IS_VALID_P(item) == false) {
            nump_throw_exception(spl_ce_OutOfBoundsException, "Index not found.");
            goto error;
        }
        
        nidxs[n] = i;

        if (nn < mt->shape.d ) {
            if (!_hash_to_mt_ex(Z_ARRVAL_P(item), mt, nn, nidxs)) {
                goto error;
            }
        } else if (MT_TYPE_P(mt) == IS_MT_DOUBLE) {
            mt_set_dval(mt, Z_DVAL_P(item), nidxs);
        } else {
            mt_set_lval(mt, Z_LVAL_P(item), nidxs);
        }
    }

    MT_FREE_P(nidxs);

    return true;

error:
    MT_FREE_P(nidxs);
    return false;
}

zend_always_inline HashTable *mt_to_hash(mt_t *mt)
{
    HashTable *ht = mt_new_array(0);
    mt_to_hash_ex(mt, ht, 0, NULL);
    return ht;
}

HashTable *mt_to_hash_1d(mt_t *mt)
{
    HashTable *ht = mt_new_array(mt->size);
    mt_val_t *val;
    zval item;

    MT_FOREACH_VAL(mt, val) {
        MT_ZVAL_T(&item, (*val), MT_TYPE_P(mt));
        zend_hash_next_index_insert_new(ht, &item);
    } MT_FOREACH_END();

    return ht;
}

HashTable *mt_to_shape(mt_t *mt)
{
    HashTable *ht = mt_new_array(mt->shape.d);
    shape_to_hash(mt->shape.d, mt->shape.axis, ht);
    return ht;
}

bool hash_to_mt_ex(HashTable *ht, mt_t *mt)
{
    zend_ulong d;
    zval *item;

    mt->size = zend_array_count(ht);

    if (mt->size < 1) {
        return true;
    }
    
    mt->shape.axis = mt_axis_init(1);
    mt->shape.axis[mt->shape.d++] = mt->size;

    for (
        item = zend_hash_index_find(ht, 0);
        IS_VALID_P(item) && Z_TYPE_P(item) == IS_ARRAY;
        item = zend_hash_index_find(Z_ARRVAL_P(item), 0)
    ) {
        d = mt->shape.d++;
        mt->shape.axis = mt_axis_reallocate(mt->shape.axis, mt->shape.d, d);
        mt->shape.axis[d] = zend_array_count(Z_ARRVAL_P(item));
        mt->size *= mt->shape.axis[d];
    }

    if (mt->size < 1) {
        return true;
    }

    mt->buffer = mt_val_init(mt->size);

    switch (Z_TYPE_P(item))
    {
    case IS_LONG:
        mt->type = IS_MT_LONG;
        break;
    case IS_DOUBLE:
        mt->type = IS_MT_DOUBLE;
        break;
    default:
        nump_throw_exception(
            spl_ce_UnexpectedValueException,
            "Value must be of type integer or double, %d given.",
            zend_get_type_by_const(Z_TYPE_P(item))
        );
        return false;
    }

    return _hash_to_mt_ex(ht, mt, 0, NULL);
}

mt_t *hash_to_mt(HashTable *ht)
{
    mt_t *mt = mt_init();

    if (hash_to_mt_ex(ht, mt)) {
        return mt;
    }

    mt_free(mt);
    return NULL;    
}

zend_always_inline void hash_to_shape(HashTable *ht, zend_ulong shape[])
{
    zend_ulong i = 0;
    zval *val;

    ZEND_HASH_FOREACH_VAL(ht, val) {
        shape[i++] = Z_LVAL_P(val);
    } ZEND_HASH_FOREACH_END();
}

zend_always_inline void shape_to_hash(MT_AXIS_PARAMS, HashTable *ht)
{
    zend_ulong i;
    zval val;

    for (i = 0; i < d; i++) {
        ZVAL_LONG(&val, axis[i]);
        zend_hash_next_index_insert_new(ht, &val);
    }
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
