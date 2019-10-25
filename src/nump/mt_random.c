
#include <math.h>

#include "php.h"
#include "ext/standard/php_rand.h"

#include "mt_random.h"
#include "mt_base.h"

static zend_always_inline double get_rand()
{
    return (double) (php_rand() >> 1) / (double) RAND_MAX;
}

static zend_always_inline double get_randn()
{
    return sqrt(-2.0 * log(get_rand())) * cos(2.0 * acos(-1.0) * get_rand());
}

mt_t *mt_rand(MT_AXIS_PARAMS)
{
    mt_t *mt = mt_init_all_t(IS_MT_DOUBLE, d, axis);
    mt_val_t *val;

    MT_FOREACH_VAL(mt, val) {
        val->dval = get_rand();
    } MT_FOREACH_END();

    return mt;
}

mt_t *mt_randn(MT_AXIS_PARAMS)
{
    mt_t *mt = mt_init_all_t(IS_MT_DOUBLE, d, axis);
    mt_val_t *val;

    MT_FOREACH_VAL(mt, val) {
        val->dval = get_randn();
    } MT_FOREACH_END();

    return mt;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
