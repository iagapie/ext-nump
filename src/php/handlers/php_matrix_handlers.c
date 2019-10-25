#include "php_common_handlers.h"
#include "php_matrix_handlers.h"

#include "../objects/php_matrix_o.h"
#include "../../nump/mt_base.h"
#include "../common.h"

zend_object_handlers php_matrix_handlers;

static int php_matrix_count_elements(zval *obj, zend_long *count)
{
    mt_t *mt = Z_MT_P(obj);
    *count = MT_SIZE_P(mt);
    return SUCCESS;
}

static void php_matrix_free_object(zend_object *object)
{
    php_mt_t *obj = (php_mt_t*) object;
    zend_object_std_dtor(&obj->std);
    mt_free(obj->mt);
}

static zend_object *php_matrix_clone_obj(zval *obj)
{
    mt_t *mt = Z_MT_P(obj);
    return php_matrix_create_object_clone(mt);
}

static HashTable *php_matrix_get_debug_info(zval *obj, int *is_temp)
{
    *is_temp = 1;
    mt_t *mt = Z_MT_P(obj);
    HashTable *ht = mt_to_hash(mt);
    
    return ht;
}

void php_register_matrix_handlers()
{
    memcpy(&php_matrix_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_matrix_handlers.offset = XtOffsetOf(php_mt_t, std);

    php_matrix_handlers.dtor_obj         = zend_objects_destroy_object;
    php_matrix_handlers.free_obj         = php_matrix_free_object;
    php_matrix_handlers.cast_object      = php_nump_default_cast_object;
    php_matrix_handlers.clone_obj        = php_matrix_clone_obj;
    php_matrix_handlers.get_debug_info   = php_matrix_get_debug_info;
    php_matrix_handlers.count_elements   = php_matrix_count_elements;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
