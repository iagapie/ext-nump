#include "php_matrix_o.h"

#include "../../nump/mt_base.h"
#include "../handlers/php_matrix_handlers.h"
#include "../classes/php_matrix_ce.h"

zend_object *php_matrix_create_object_ex(mt_t *mt)
{
    php_mt_t *obj = MT_CALLOC(1, php_mt_t);
    zend_object_std_init(&obj->std, php_matrix_ce);
    obj->std.handlers = &php_matrix_handlers;
    obj->mt = mt;
    return &obj->std;
}

zend_always_inline zend_object *php_matrix_create_object(zend_class_entry *ce)
{
    return php_matrix_create_object_ex(mt_init());
}

zend_always_inline zend_object *php_matrix_create_object_clone(mt_t *mt)
{
    return php_matrix_create_object_ex(mt_clone(mt));
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
