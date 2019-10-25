#include "php.h"
#include "zend_exceptions.h"

#include "php_matrix_apply.h"
#include "../../nump/mt_base.h"
#include "../common.h"

mt_t *mt_apply(const mt_t *mt, FCI_PARAMS)
{
    mt_t *copy = mt_clone(mt);
    zend_ulong i;
    mt_val_t *val;
    zval params[2], carry;

    MT_FOREACH_VAL_IDX(copy, val, i) {
        if (copy->type == IS_MT_LONG) {
            ZVAL_LONG(&params[0], val->lval);
        } else {
            ZVAL_DOUBLE(&params[0], val->dval);
        }
        
        ZVAL_LONG(&params[1], i);

        fci.param_count = 2;
        fci.params = params;
        fci.retval = &carry;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(carry)) {
            zval_ptr_dtor(&carry);
            nump_throw_exception(zend_ce_error, "Callable should return a double.");
            return NULL;
        }

        if (copy->type == IS_MT_LONG) {
            val->lval = Z_LVAL(carry);
        } else {
            val->dval = Z_DVAL(carry);
        }

        Z_TRY_DELREF_P(&carry);
    } MT_FOREACH_END();

    return copy;
}