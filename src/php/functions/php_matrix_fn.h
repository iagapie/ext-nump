#ifndef PHP_NUMP_MATRIX_FN_H
#define PHP_NUMP_MATRIX_FN_H

#include "php.h"
#include "../common.h"
#include "../arginfo.h"

#define PHP_MATRIX_FE_LIST \
    PHP_NUMP_FE(matrix_from) \
    PHP_NUMP_FE(matrix_zeros) \
    PHP_NUMP_FE(matrix_eye) \
    PHP_NUMP_FE(matrix_rand) \
    PHP_NUMP_FE(matrix_randn)

ARGINFO_ZVAL(matrix_from, data);
ARGINFO_ZVAL(matrix_zeros, shape);
ARGINFO_ZVAL_OPTIONAL_LONG(matrix_eye, shape, k);
ARGINFO_ZVAL(matrix_rand, shape);
ARGINFO_ZVAL(matrix_randn, shape);

PHP_FUNCTION(matrix_from);
PHP_FUNCTION(matrix_zeros);
PHP_FUNCTION(matrix_eye);
PHP_FUNCTION(matrix_rand);
PHP_FUNCTION(matrix_randn);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
