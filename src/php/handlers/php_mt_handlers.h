#ifndef NUMP_PHP_MT_HANDLERS_H
#define NUMP_PHP_MT_HANDLERS_H

#include "php.h"

extern zend_object_handlers php_mt_handlers;

void php_register_mt_handlers();

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
