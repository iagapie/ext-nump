#include "php.h"
#include "zend_exceptions.h"

#include "php_matrix_fn.h"
#include "../common.h"
#include "../parameters.h"
#include "../objects/php_matrix_o.h"
#include "../../nump/mt_random.h"
#include "../../nump/mt_multiarray_umath.h"
#include "../../nump/mt_twodim.h"

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

    zend_ulong axes[d];

    hash_to_shape(Z_ARRVAL_P(shape), axes);

    RETURN_MT(mt_zeros(IS_MT_DOUBLE, d, axes));
}

PHP_FUNCTION(matrix_eye)
{
    PARSE_LONG_OPTIONAL_LONG_LONG(m, n, k);
    RETURN_MT(mt_eye(IS_MT_DOUBLE, m, n, k));
}

PHP_FUNCTION(matrix_rand)
{
    PARSE_ARRAY(shape);

    zend_ulong d = zend_array_count(Z_ARRVAL_P(shape));

    if (d == 0) {
        nump_throw_exception(zend_ce_error, "Empty shape is not valid.");
        return;
    }

    zend_ulong axes[d];

    hash_to_shape(Z_ARRVAL_P(shape), axes);

    RETURN_MT(mt_rand(d, axes));
}

PHP_FUNCTION(matrix_randn)
{
    PARSE_ARRAY(shape);

    zend_ulong d = zend_array_count(Z_ARRVAL_P(shape));

    if (d == 0) {
        nump_throw_exception(zend_ce_error, "Empty shape is not valid.");
        return;
    }

    zend_ulong axes[d];

    hash_to_shape(Z_ARRVAL_P(shape), axes);

    RETURN_MT(mt_randn(d, axes));
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
