#include "mt_apply.h"

mt_t *mt_apply(const mt_t *mt, FCI_PARAMS)
{
    mt_t *copy = mt_clone(mt);
    zend_ulong i;
    double *val;
    zval params[2], carry;

    if (IS_MT_VALID_P(copy)) {
        MT_FOREACH_VAL_IDX(copy, val, i) {
            ZVAL_DOUBLE(&params[0], *val);
            ZVAL_LONG(&params[1], i);

            fci.param_count = 2;
            fci.params = params;
            fci.retval = &carry;

            if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(carry)) {
                zval_ptr_dtor(&carry);
                goto error;
            }

            switch (Z_TYPE(carry))
            {
            case IS_LONG:
                *val = (double) Z_LVAL(carry);
                break;
            
            case IS_DOUBLE:
                *val = Z_DVAL(carry);
                break;
            case IS_NULL:
                THROW_EXCEPTION("Expected number, null returned");
                goto error;
                break;            
            default:
                THROW_EXCEPTION("Callable should return a number");
                goto error;
                break;
            }

            Z_TRY_DELREF_P(&carry);
        } MT_FOREACH_END();
    }

    return copy;

error:
    mt_free(copy);
    return NULL;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
