#include "php_common_handlers.h"
#include "php_mt_handlers.h"

#include "../objects/php_mt_o.h"
#include "../../nump/mt_array.h"

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

static zend_object *php_mt_clone_obj(zval *obj)
{
    return php_mt_create_object_clone(Z_MT_P(obj));
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
    php_mt_handlers.cast_object      = php_nump_default_cast_object;
    php_mt_handlers.clone_obj        = php_mt_clone_obj;
    php_mt_handlers.get_debug_info   = php_mt_get_debug_info;
    php_mt_handlers.count_elements   = php_mt_count_elements;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
