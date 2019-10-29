#include "php.h"

#include "php_mt_fn.h"
#include "../parameters.h"
#include "../objects/php_mt_o.h"
#include "../../nump/mt_array.h"
#include "../../nump/mt_math.h"
#include "../../nump/mt_twodim.h"
#include "../../nump/mt_random.h"

PHP_FUNCTION(matrix_from)
{
    PARSE_ARRAY(array);
    RETURN_MT(hash_to_mt(Z_ARRVAL_P(array)));
}

PHP_FUNCTION(matrix_zeros)
{
    PARSE_ARRAY(shape);
    RETURN_MT(mt_zeros(hash_to_mt_shape(Z_ARRVAL_P(shape))));
}

PHP_FUNCTION(matrix_eye)
{
    PARSE_LONG_OPTIONAL_LONG_LONG(m, n, k);
    RETURN_MT(mt_eye(m, n, k));
}

PHP_FUNCTION(matrix_rand)
{
    PARSE_ARRAY(shape);
    RETURN_MT(mt_rand(hash_to_mt_shape(Z_ARRVAL_P(shape))));
}

PHP_FUNCTION(matrix_randn)
{
    PARSE_ARRAY(shape);
    RETURN_MT(mt_randn(hash_to_mt_shape(Z_ARRVAL_P(shape))));
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
