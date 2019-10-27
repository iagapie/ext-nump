#include "mt_twodim.h"
#include "mt_multiarray_umath.h"

mt_t *mt_eye(const zend_uchar type, const zend_ulong m, const zend_ulong n, const zend_long k)
{
    if (m == 0) {
        return mt_init_t(type);
    }

    zend_ulong axes[2] = { m, (n == 0 ? m : n) }, i, j;
    zend_long ak = k < 0 ? -k : k;

    mt_t *mt = mt_zeros(type, 2, axes);
    
    for (i = (-k + ak) / 2, j = (k + ak) / 2; i < axes[0] && j < axes[1]; ++i, ++j) {
        if (type == IS_MT_DOUBLE) {
            mt->buffer[i * axes[1] + j].dval = 1.0;
        } else {
            mt->buffer[i * axes[1] + j].lval = 1;
        }
    }

    return mt;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
