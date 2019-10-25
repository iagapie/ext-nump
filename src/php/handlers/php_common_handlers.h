#ifndef PHP_NUMP_COMMON_HANDLERS_H
#define PHP_NUMP_COMMON_HANDLERS_H

#include "php.h"

/**
 * Default object cast handler.
 */
int php_nump_default_cast_object(zval *obj, zval *return_value, int type);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
