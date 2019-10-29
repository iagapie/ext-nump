#ifndef NUMP_PHP_MT_ITER_H
#define NUMP_PHP_MT_ITER_H

#include "php.h"
#include "../../nump/mt_base.h"

typedef struct _php_mt_iterator_t {
    zend_object_iterator    intern;
    zend_object            *object;
    mt_t                   *mt;
    zend_long               position;
} php_mt_iterator_t;

zend_object_iterator *php_mt_get_iterator(zend_class_entry *ce, zval *obj, int by_ref);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
