#include "php_common_handlers.h"
#include "php_mt_handlers.h"

#include "../objects/php_mt_o.h"
#include "../../nump/mt_array.h"
#include "../../nump/mt_string.h"

zend_object_handlers php_mt_handlers;

static int php_mt_count_elements(zval *obj, zend_long *count)
{
    *count = MT_SIZE_P(Z_MT_P(obj));
    return SUCCESS;
}

static void php_mt_free_object(zend_object *object)
{
    php_mt_t *obj = (php_mt_t*) object;
    zend_object_std_dtor(&obj->std);
    mt_free(obj->mt);
}

static int php_mt_cast_object(zval *object, zval *return_value, int type)
{
    switch (type) {
        case IS_STRING: {
            ZVAL_STR(return_value, mt_to_string(Z_MT_P(object)));
            return SUCCESS;
        }
        case _IS_BOOL: {
            if (IS_MT_EMPTY_P(Z_MT_P(object))) {
                ZVAL_FALSE(return_value);
            } else {
                ZVAL_TRUE(return_value);
            }
            return SUCCESS;
        }
    }

    return FAILURE;
}

static zend_object *php_mt_clone_object(zval *object)
{
    return php_mt_create_object_clone(Z_MT_P(object));
}

static HashTable *php_mt_get_debug_info(zval *obj, int *is_temp)
{
    *is_temp = 1;
    return mt_to_hash(Z_MT_P(obj));
}

void php_register_mt_handlers()
{
    memcpy(&php_mt_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_mt_handlers.offset = XtOffsetOf(php_mt_t, std);

    php_mt_handlers.dtor_obj         = zend_objects_destroy_object;
    php_mt_handlers.free_obj         = php_mt_free_object;
    php_mt_handlers.cast_object      = php_mt_cast_object;
    php_mt_handlers.clone_obj        = php_mt_clone_object;
    php_mt_handlers.get_debug_info   = php_mt_get_debug_info;
    php_mt_handlers.count_elements   = php_mt_count_elements;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
