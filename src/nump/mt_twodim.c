#include "mt_twodim.h"
#include "mt_math.h"

mt_t *mt_eye(const zend_ulong m, const zend_ulong n, const zend_long k)
{
    if (m == 0) {
        return mt_init(NULL, NULL);
    }

    mt_shape_t *shape = mt_shape_init(2);
    shape->axes[0] = m;
    shape->axes[1] = n == 0 ? m : n;

    mt_t *mt = mt_zeros(shape);

    zend_ulong i, j;
    zend_long ak = k < 0 ? -k : k;
    
    for (i = (-k + ak) / 2, j = (k + ak) / 2; i < shape->axes[0] && j < shape->axes[1]; ++i, ++j) {
        mt->buffer->data[i * shape->axes[1] + j] = 1.0;
    }

    return mt;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
