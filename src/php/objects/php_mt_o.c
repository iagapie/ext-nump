#include "php_mt_o.h"

#include "../../common.h"
#include "../handlers/php_mt_handlers.h"
#include "../classes/php_mt_ce.h"

zend_object *php_mt_create_object_ex(mt_t *mt)
{
    php_mt_t *obj = CALLOC(1, php_mt_t);

    zend_object_std_init(&obj->std, php_mt_ce);
    obj->std.handlers = &php_mt_handlers;
    obj->mt = mt;
    
    return &obj->std;
}

zend_always_inline zend_object *php_mt_create_object(zend_class_entry *ce)
{
    return php_mt_create_object_ex(mt_init(NULL, NULL));
}

zend_always_inline zend_object *php_mt_create_object_clone(mt_t *mt)
{
    return php_mt_create_object_ex(mt_clone(mt));
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
