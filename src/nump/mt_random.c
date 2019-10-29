#include "mt_random.h"

mt_t *mt_rand(mt_shape_t *shape)
{
    mt_t *mt = mt_init_by_shape(shape);
    double *val;

    if (IS_VALID_P(mt)) {
        MT_FOREACH_VAL(mt, val) {
            *val = nump_rand();
        } MT_FOREACH_END();
    }

    return mt;
}

mt_t *mt_randn(mt_shape_t *shape)
{
    mt_t *mt = mt_init_by_shape(shape);
    double *val;

    if (IS_VALID_P(mt)) {
        MT_FOREACH_VAL(mt, val) {
            *val = nump_randn();
        } MT_FOREACH_END();
    }

    return mt;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
