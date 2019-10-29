#include "php.h"
#include "zend_interfaces.h"
#include "ext/spl/spl_iterators.h"
#include "ext/json/php_json.h"

#include "php_mt_ce.h"
#include "../parameters.h"
#include "../objects/php_mt_o.h"
#include "../iterators/php_mt_iter.h"
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

static bool _get_index(const mt_t *mt, const zval *val, zend_long *index, bool throw)
{
    mt_shape_t *shape;
    *index = -1;

    switch (Z_TYPE_P(val))
    {
        case IS_LONG:
            *index = Z_LVAL_P(val);
            
            if (!IS_MT_EMPTY_P(mt) && MT_ISSET_P(mt, *index)) {
                return true;
            }

            if (throw) {
                THROW_INDEX_OUT_OF_RANGE(*index, (IS_VALID_P(mt->buffer) ? mt->buffer->size : 0));
            }

            break;
        case IS_ARRAY:
            shape = hash_to_mt_shape(Z_ARRVAL_P(val));

            if (IS_MT_EMPTY_P(mt)) {
                *index = 0;
                mt_shape_free(shape);
                return false;
            }

            if (!IS_VALID_P(shape) || shape->d == 0) {
                THROW_EXCEPTION_A("Expected array of length %d, 0 given", mt->shape->d);
                mt_shape_free(shape);
                return false;
            }

            if (shape->d != mt->shape->d) {
                THROW_EXCEPTION_AA("Expected array of length %d, %d given", mt->shape->d, shape->d);
                mt_shape_free(shape);
                return false;
            }

            *index = mt_build_index(mt, shape->axes);
            mt_shape_free(shape);

            if (MT_ISSET_P(mt, *index)) {
                return true;
            }

            if (throw) {
                THROW_INDEX_OUT_OF_RANGE(*index, (IS_VALID_P(mt->buffer) ? mt->buffer->size : 0));
            }

            break;
        default:
            THROW_TYPE("an array or an integer", val);
            break;
    }

    return false;
}

METHOD(__construct)
{
    PARSE_ARRAY(array);
    hash_to_mt_ex(Z_ARRVAL_P(array), THIS_MT());
}

METHOD(value)
{
    PARSE_ZVAL(val);

    zend_long index;

    if (_get_index(THIS_MT(), val, &index, true)) {
        RETURN_DOUBLE(mt_get(THIS_MT(), index));
    }
}

METHOD(isset)
{
    PARSE_ZVAL(val);

    zend_long index;

    RETURN_BOOL(_get_index(THIS_MT(), val, &index, false));
}

METHOD(exp)
{
    PARSE_NONE;
    RETURN_MT(mt_math(THIS_MT(), MT_MATH_EXP));
}

METHOD(log)
{
    PARSE_NONE;
    RETURN_MT(mt_math(THIS_MT(), MT_MATH_LOG));
}

METHOD(log2)
{
    PARSE_NONE;
    RETURN_MT(mt_math(THIS_MT(), MT_MATH_LOG2));
}

METHOD(log10)
{
    PARSE_NONE;
    RETURN_MT(mt_math(THIS_MT(), MT_MATH_LOG10));
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

METHOD(count)
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
    RETURN_BOOL(IS_MT_SQUARE_P(THIS_MT()));
}

METHOD(isSingular)
{
    PARSE_NONE;
    RETURN_BOOL(IS_SINGULAR_P(THIS_MT()));
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

METHOD(jsonSerialize)
{
    PARSE_NONE;
    RETURN_ARR(mt_to_hash(THIS_MT()));
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
    php_mt_ce->get_iterator = php_mt_get_iterator;
    php_mt_ce->serialize = php_mt_serialize;
    php_mt_ce->unserialize = php_mt_unserialize;

    zend_class_implements(php_mt_ce, 3,
        zend_ce_traversable,        // Traversable
        spl_ce_Countable,           // Countable
        php_json_serializable_ce    // Serializable
    );

    php_register_mt_handlers();
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
