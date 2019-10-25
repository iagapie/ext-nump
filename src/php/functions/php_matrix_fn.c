#include "php.h"
#include "zend_exceptions.h"

#include "php_matrix_fn.h"
#include "../common.h"
#include "../parameters.h"
#include "../objects/php_matrix_o.h"
#include "../../nump/mt_random.h"

PHP_FUNCTION(matrix_from)
{
    PARSE_ARRAY(array);
    RETURN_MT(hash_to_mt(Z_ARRVAL_P(array)));
}

PHP_FUNCTION(matrix_zeros)
{
    PARSE_ARRAY(shape);

    zend_ulong d = zend_array_count(Z_ARRVAL_P(shape));

    if (d == 0) {
        nump_throw_exception(zend_ce_error, "Empty shape is not valid.");
        return;
    }

    zend_ulong axis[d];

    hash_to_shape(Z_ARRVAL_P(shape), axis);

    RETURN_MT(mt_zeros(IS_MT_DOUBLE, d, axis));
}

PHP_FUNCTION(matrix_eye)
{
    PARSE_ARRAY_OPTIONAL_LONG(shape, k);

    zend_ulong d = zend_array_count(Z_ARRVAL_P(shape));

    if (d < 2) {
        nump_throw_exception(zend_ce_error, "Shape is not valid.");
        return;
    }

    zend_ulong axis[d];

    hash_to_shape(Z_ARRVAL_P(shape), axis);

    RETURN_MT(mt_eye(IS_MT_DOUBLE, d, axis, k));
}

PHP_FUNCTION(matrix_rand)
{
    PARSE_ARRAY(shape);

    zend_ulong d = zend_array_count(Z_ARRVAL_P(shape));

    if (d == 0) {
        nump_throw_exception(zend_ce_error, "Empty shape is not valid.");
        return;
    }

    zend_ulong axis[d];

    hash_to_shape(Z_ARRVAL_P(shape), axis);

    RETURN_MT(mt_rand(d, axis));
}

PHP_FUNCTION(matrix_randn)
{
    PARSE_ARRAY(shape);

    zend_ulong d = zend_array_count(Z_ARRVAL_P(shape));

    if (d == 0) {
        nump_throw_exception(zend_ce_error, "Empty shape is not valid.");
        return;
    }

    zend_ulong axis[d];

    hash_to_shape(Z_ARRVAL_P(shape), axis);

    RETURN_MT(mt_randn(d, axis));
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
