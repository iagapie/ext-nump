#include "php_common_handlers.h"
#include "zend_smart_str.h"

int php_nump_default_cast_object(zval *obj, zval *return_value, int type)
{
    switch (type) {
        case IS_STRING: {
            smart_str buffer = {0};

            smart_str_appendl(&buffer, "object(", 7);
            smart_str_append (&buffer, Z_OBJCE_P(obj)->name);
            smart_str_appendc(&buffer, ')');

            smart_str_0(&buffer);
            ZVAL_STR(return_value, buffer.s);
            return SUCCESS;
        }
        case _IS_BOOL: {
            ZVAL_TRUE(return_value);
            return SUCCESS;
        }
    }

    return FAILURE;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
