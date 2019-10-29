#include "php.h"

#include "php_mt_ce.h"

#include "../parameters.h"
#include "../objects/php_mt_o.h"
#include "../handlers/php_mt_handlers.h"
#include "../../common.h"
#include "../../nump/mt_base.h"
#include "../../nump/mt_array.h"
#include "../../nump/mt_string.h"
#include "../../nump/mt_linalg.h"
#include "../../nump/mt_math.h"
#include "../../nump/mt_apply.h"

#define METHOD(name) PHP_METHOD(Matrix, name)

zend_class_entry *php_mt_ce;

METHOD(__construct)
{
    PARSE_ARRAY(array);
    hash_to_mt_ex(Z_ARRVAL_P(array), THIS_MT());
}

METHOD(value)
{
    PARSE_ZVAL(val);

    mt_t *mt = THIS_MT();
    mt_shape_t *shape;
    zend_ulong index;

    switch (Z_TYPE_P(val))
    {
    case IS_LONG:
        index = Z_LVAL_P(val);
        break;
    case IS_ARRAY:
        shape = hash_to_mt_shape(Z_ARRVAL_P(val));
        if (IS_VALID_P(shape) && IS_MT_VALID_P(mt) && shape->d == mt->shape->d) {
            index = mt_build_index(mt, shape->axes);
            mt_shape_free(shape);
        } else {
            EX_INDEX_NOT_FOUND();
            mt_shape_free(shape);
            return;
        }
        
        break;
    default:
        EX_ARGUMENT_NOT_VALID_TYPE();
        return;
    }

    if (MT_ISSET_P(mt, index)) {
        RETURN_DOUBLE(mt_get(mt, index));
    } else {
        EX_INDEX_OUT_OF_RANGE(index);
    }
}

METHOD(negative)
{
    PARSE_NONE;
    RETURN_MT(mt_negative(THIS_MT()));
}

METHOD(transpose)
{
    PARSE_NONE;
    RETURN_MT(mt_transpose(THIS_MT()));
}

METHOD(inverse)
{
    PARSE_NONE;
    RETURN_MT(mt_inv(THIS_MT()));
}

METHOD(determinant)
{
    PARSE_NONE;
    RETURN_DOUBLE(mt_det(THIS_MT()));
}

METHOD(mean)
{
    PARSE_NONE;
    RETURN_DOUBLE(mt_mean(THIS_MT()));
}

METHOD(shape)
{
    PARSE_NONE;
    RETURN_ARR(mt_shape_to_hash(THIS_MT()->shape));
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
    RETURN_BOOL(mt_is_square(THIS_MT()));
}

METHOD(isSingular)
{
    PARSE_NONE;
    RETURN_BOOL(IS_MT_SINGULAR_P(THIS_MT()));
}

METHOD(toArray)
{
    PARSE_NONE;
    RETURN_ARR(mt_to_hash(THIS_MT()));
}

METHOD(toFlatten)
{
    PARSE_NONE;
    RETURN_ARR(mt_to_1d_hash(THIS_MT()));
}

METHOD(round)
{
    PARSE_OPTIONAL_LONG_LONG(precision, mode);
    RETURN_MT(mt_round(THIS_MT(), (int) precision, (int) mode));
}

METHOD(apply)
{
    PARSE_CALLABLE();
    RETURN_MT(mt_apply(THIS_MT(), FCI_ARGS));
}

METHOD(solve)
{
    PARSE_OBJ(b, php_mt_ce);
    RETURN_MT(mt_solve(THIS_MT(), Z_MT_P(b)));
}

METHOD(add)
{
    PARSE_ZVAL(val);
    RETURN_MT_OP(val, add);
}

METHOD(sub)
{
    PARSE_ZVAL(val);
    RETURN_MT_OP(val, sub);
}

METHOD(mul)
{
    PARSE_ZVAL(val);
    RETURN_MT_OP(val, mul);
}

METHOD(div)
{
    PARSE_ZVAL(val);
    RETURN_MT_OP(val, div);
}

METHOD(dot)
{
    PARSE_ZVAL(val);
    RETURN_MT_OP(val, dot);
}

METHOD(__toString)
{
    PARSE_NONE;
    RETURN_STR(mt_to_string(THIS_MT()));
}

void php_register_mt()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_MT_ME_LIST(Matrix)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_NUMP_NS(Matrix), methods);

    php_mt_ce = zend_register_internal_class(&ce);
    php_mt_ce->create_object = php_mt_create_object;

    php_register_mt_handlers();
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
