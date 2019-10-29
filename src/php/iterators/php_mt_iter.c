#include "php.h"
#include "php_mt_iter.h"
#include "../objects/php_mt_o.h"
#include "../../nump/mt_array.h"

static void php_mt_iterator_dtor(zend_object_iterator *iter)
{
    php_mt_iterator_t *iterator = (php_mt_iterator_t *) iter;

    OBJ_RELEASE(iterator->object);
}

static int php_mt_iterator_valid(zend_object_iterator *iter)
{
    php_mt_iterator_t *iterator = (php_mt_iterator_t *) iter;

    if (iterator->position < MT_SIZE_P(iterator->mt)) {
        return SUCCESS;
    }

    return FAILURE;
}

static zval *php_mt_iterator_get_current_data(zend_object_iterator *iter)
{
    php_mt_iterator_t *iterator = (php_mt_iterator_t *) iter;

    zval *value = CALLOC(1, zval);

    ZVAL_DOUBLE(value, mt_get(iterator->mt, iterator->position));

    return value;
}

static void php_mt_iterator_get_current_key(zend_object_iterator *iter, zval *key) {
    ZVAL_LONG(key, ((php_mt_iterator_t *) iter)->position);
}

static void php_mt_iterator_move_forward(zend_object_iterator *iter)
{
    ((php_mt_iterator_t *) iter)->position++;
}

static void php_mt_iterator_rewind(zend_object_iterator *iter)
{
    ((php_mt_iterator_t *) iter)->position = 0;
}

static zend_object_iterator_funcs iterator_funcs = {
    php_mt_iterator_dtor,
    php_mt_iterator_valid,
    php_mt_iterator_get_current_data,
    php_mt_iterator_get_current_key,
    php_mt_iterator_move_forward,
    php_mt_iterator_rewind
};

static zend_object_iterator *php_mt_create_iterator(zval *obj, int by_ref)
{
    php_mt_iterator_t *iterator;

    if (by_ref) {
        THROW_ERROR("Iterating by reference is not supported");
        return NULL;
    }

    iterator = CALLOC(1, php_mt_iterator_t);
    zend_iterator_init((zend_object_iterator*) iterator);

    iterator->intern.funcs = &iterator_funcs;
    iterator->mt           = Z_MT_P(obj);
    iterator->object       = Z_OBJ_P(obj);
    iterator->position     = 0;

    // Add a reference to the object so that it doesn't get collected when
    // the iterated object is implict, eg. foreach ($obj->getInstance() as $value){ ... }
#if PHP_VERSION_ID >= 70300
    GC_ADDREF(iterator->object);
#else
    ++GC_REFCOUNT(iterator->object);
#endif

    return (zend_object_iterator *) iterator;
}

zend_object_iterator *php_mt_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    return php_mt_create_iterator(obj, by_ref);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
