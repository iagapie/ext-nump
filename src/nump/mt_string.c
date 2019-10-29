#include "mt_string.h"

static zend_string *_mt_to_string(const mt_t *mt, const zend_ulong n, const zend_ulong *idxs)
{
    zend_string *str, *tmp1, *tmp2;
    zend_ulong i, nn, *nidxs;
    double val;

    str = zend_string_init("", 0, 0);

    if (!IS_MT_EMPTY_P(mt)) {
        nn = n + 1;
        nidxs = CALLOC(nn, zend_ulong);

        if (n > 0) {
            memcpy(nidxs, idxs, n * sizeof(zend_ulong));
        }

        for (i = 0; i < mt->shape->axes[n]; i++) {
            nidxs[n] = i;
            tmp1 = str;

            if (i != 0) {
                str = strpprintf(0, nn < mt->shape->d ? "%s\n" : "%s ", ZSTR_VAL(tmp1));
                zend_string_release(tmp1);
                tmp1 = str;
            }

            if (nn < mt->shape->d) {
                tmp2 = _mt_to_string(mt, nn, nidxs);
                str = strpprintf(0, "%s%s", ZSTR_VAL(tmp1), ZSTR_VAL(tmp2));
                zend_string_release(tmp2);
            } else {
                val = mt_get_value(mt, nidxs);
                str = strpprintf(0, "%s%g", ZSTR_VAL(tmp1), val);
            }

            zend_string_release(tmp1);
        }

        FREE_P(nidxs);
    }

    tmp1 = str;
    str = strpprintf(0, "[%s]", ZSTR_VAL(tmp1));
    zend_string_release(tmp1);

    return str;
}

zend_always_inline zend_string *mt_to_string(const mt_t *mt)
{
    return _mt_to_string(mt, 0, NULL);
}

zend_string *mt_shape_to_string(const mt_shape_t *shape)
{
    zend_string *str, *tmp;
    zend_ulong i;

    str = zend_string_init("", 0, 0);

    if (IS_MT_SHAPE_VALID_P(shape)) {
        for (i = 0; i < shape->d; i++) {
            tmp = str;

            if (i == 0 || i < (shape->d - 1)) {
                str = strpprintf(0, "%s%ld,", ZSTR_VAL(tmp), shape->axes[i]);
            } else {
                str = strpprintf(0, "%s%ld", ZSTR_VAL(tmp), shape->axes[i]);
            }
            
            zend_string_release(tmp);
        }
    }

    tmp = str;
    str = strpprintf(0, "(%s)", ZSTR_VAL(tmp));
    zend_string_release(tmp);

    return str;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
