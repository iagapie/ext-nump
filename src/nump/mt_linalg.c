#include <math.h>

#include "mt_linalg.h"
#include "mt_twodim.h"

mt_t *mt_transpose(const mt_t *mt)
{
    mt_t *copy = mt_clone(mt);
    zend_long i, j;

    if (IS_MT_EMPTY_P(copy) || copy->shape->d == 1) {
        return copy;
    }

    if (copy->shape->d > 2) {
        mt_free(copy);
        THROW_ERROR_A("Expected 1D/2D array, %d D given", copy->shape->d);
        return NULL;
    }

    for (i = mt->shape->d - 1, j = 0; i >= 0; i--, j++) {
        copy->shape->axes[j] = mt->shape->axes[i];
    }

    for (i = 0; i < mt->shape->axes[0]; i++) {
        for (j = 0; j < mt->shape->axes[1]; j++) {
            copy->buffer->data[j * copy->shape->axes[1] + i]
                = mt->buffer->data[i * mt->shape->axes[1] + j];
        }
    }

    return copy;
}

mt_t *mt_inv(const mt_t *mt)
{
    if (!IS_MT_SQUARE_P(mt)) {
        THROW_EXCEPTION("Expected a square matrix");
        return NULL;
    }

    if (IS_SINGULAR_P(mt)) {
        THROW_EXCEPTION("Expected not a singular matrix");
        return NULL;
    }

    mt_t *identity = mt_eye(mt->shape->axes[0], mt->shape->axes[1], 0);
    mt_t *inv = mt_solve(mt, identity);

    mt_free(identity);

    return inv;
}

mt_t *mt_solve(const mt_t *a, const mt_t *b)
{
    if (!IS_MT_SQUARE_P(a) || !IS_MT_SQUARE_P(b)) {
        THROW_EXCEPTION("Expected a square matrix");
        return NULL;
    }

    if (IS_SINGULAR_P(a)) {
        THROW_EXCEPTION("Expected not a singular matrix");
        return NULL;
    }

    mt_t *ac = mt_clone(a);
    mt_t *bc = mt_clone(b);

    double isc, ksc;
    zend_long i, j, k, m, s = ac->shape->axes[0];

    for (i = 0; i < s; i++) {
        isc = 1.0 / ac->buffer->data[i * s + i];

        for (j = 0; j < s; j++) {
            ac->buffer->data[i * s + j] *= isc;
            bc->buffer->data[i * s + j] *= isc;
        }

        for (k = 0; k < s; k++) {
            if (k != i) {
                ksc = ac->buffer->data[k * s + i];

                for (m = 0; m < s; m++) {
                    ac->buffer->data[k * s + m] -= ac->buffer->data[i * s + m] * ksc;
                    bc->buffer->data[k * s + m] -= bc->buffer->data[i * s + m] * ksc;
                }
            }
        }
    }

    mt_free(ac);

    return bc;
}

static double _mt_det(const mt_t *mt)
{
    if (MT_SIZE_P(mt) == 4) {
        return mt->buffer->data[0] * mt->buffer->data[3] 
            - mt->buffer->data[1] * mt->buffer->data[2];
    }

    double d = 0.0;
    mt_shape_t *shape;
    mt_t *sub;
    zend_long k, i, j, ti, tj;

    for (k = 0; k < mt->shape->axes[0]; k++) {
        shape = mt_shape_init(mt->shape->d);

        for (i = 0; i < mt->shape->d; i++) {
            shape->axes[i] = mt->shape->axes[i] - 1;
        }

        sub = mt_init_by_shape(shape);

        for (i = 1, ti = 0; i < mt->shape->axes[0]; i++, ti++) {
            for (j = 0, tj = 0; j < mt->shape->axes[0]; j++) {
                if (j == k) {
                    continue;
                }

                sub->buffer->data[ti * shape->axes[1] + tj]
                    = mt->buffer->data[i * mt->shape->axes[1] + j];
                
                tj++;
            }
        }

        d += pow(-1.0, (double)(k % 2)) * mt->buffer->data[k] * _mt_det(sub);

        shape = NULL;
        mt_free(sub);
    }

    return d;
}

double mt_det(const mt_t *mt)
{
    if (MT_SIZE_P(mt) == 1) {
        return mt->buffer->data[0];
    } else if (IS_MT_SQUARE_P(mt)) {
        return _mt_det(mt);
    }

    return 0.0;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
