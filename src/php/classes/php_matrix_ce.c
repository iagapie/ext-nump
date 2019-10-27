#include "php.h"

#include "php_matrix_ce.h"

#include "../common.h"
#include "../parameters.h"
#include "../objects/php_matrix_o.h"
#include "../handlers/php_matrix_handlers.h"
#include "../mt/php_matrix_apply.h"
#include "../../nump/mt_multiarray_umath.h"
#include "../../nump/mt_linalg.h"

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

    mt_t *mt = THIS_MT();
    zend_ulong index, idxs[mt->size];

    switch (Z_TYPE_P(val))
    {
    case IS_LONG:
        index = Z_LVAL_P(val);
        break;
    case IS_ARRAY:
        hash_to_shape(Z_ARRVAL_P(val), idxs);
        index = mt_get_index(mt, idxs);
        break;
    default:
        nump_throw_exception(NULL, "Argument is not a valid type.");
        return;
    }

    if (MT_ISSET_P(mt, index)) {
        RETURN_MT_VAL(mt, mt_get_val_by_idx(mt, index));
    } else {
        INDEX_OUT_OF_RANGE(index);
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
    RETURN_ARR(mt_to_hash_1d(THIS_MT()));
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
    PARSE_OBJ(b, php_matrix_ce);
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
