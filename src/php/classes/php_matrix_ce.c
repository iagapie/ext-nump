#include "php.h"

#include "php_matrix_ce.h"

#include "../common.h"
#include "../parameters.h"
#include "../objects/php_matrix_o.h"
#include "../handlers/php_matrix_handlers.h"
#include "../mt/php_matrix_apply.h"

#define METHOD(name) PHP_METHOD(Matrix, name)

zend_class_entry *php_matrix_ce;

METHOD(__construct)
{
    PARSE_ARRAY(array);
    hash_to_mt_ex(Z_ARRVAL_P(array), THIS_MT());
}

METHOD(value)
{
    PARSE_ZVAL(val);
}

METHOD(negative)
{
    PARSE_NONE;
}

METHOD(transpose)
{
    PARSE_NONE;
}

METHOD(inverse)
{
    PARSE_NONE;
}

METHOD(determinant)
{
    PARSE_NONE;
}

METHOD(mean)
{
    PARSE_NONE;
}

METHOD(shape)
{
    PARSE_NONE;
    RETURN_ARR(mt_to_shape(THIS_MT()));
}

METHOD(size)
{
    PARSE_NONE;
    RETURN_LONG(MT_SIZE_P(THIS_MT()));
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(IS_MT_EMPTY_P(THIS_MT()));
}

METHOD(isSquare)
{
    PARSE_NONE;
}

METHOD(isSingular)
{
    PARSE_NONE;
}

METHOD(toArray)
{
    PARSE_NONE;
    RETURN_ARR(mt_to_hash(THIS_MT()));
}

METHOD(toFlatten)
{
    PARSE_NONE;
    RETURN_ARR(mt_to_hash_1d(THIS_MT()));
}

METHOD(round)
{
    PARSE_OPTIONAL_LONG_LONG(precision, mode);
}

METHOD(apply)
{
    PARSE_CALLABLE();
    RETURN_MT(mt_apply(THIS_MT(), FCI_ARGS));
}

METHOD(solve)
{
    PARSE_OBJ(b, php_matrix_ce);
}

METHOD(add)
{
    PARSE_ZVAL(val);
}

METHOD(sub)
{
    PARSE_ZVAL(val);
}

METHOD(mul)
{
    PARSE_ZVAL(val);
}

METHOD(div)
{
    PARSE_ZVAL(val);
}

METHOD(dot)
{
    PARSE_ZVAL(val);
}

METHOD(__toString)
{
    PARSE_NONE;
}

void php_register_matrix()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_MT_ME_LIST(Matrix)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_NUMP_NS(Matrix), methods);

    php_matrix_ce = zend_register_internal_class(&ce);
    php_matrix_ce->create_object = php_matrix_create_object;

    php_register_matrix_handlers();
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
