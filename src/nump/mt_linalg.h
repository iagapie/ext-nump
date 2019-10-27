#ifndef NUMP_MT_LINALG_H
#define NUMP_MT_LINALG_H

#include "mt_base.h"

#define IS_MT_SINGULAR(mt)      (mt_det(&(mt)) == 0.0)
#define IS_MT_SINGULAR_P(mt_p)  IS_MT_SINGULAR(*(mt_p))

mt_t *mt_transpose(const mt_t *mt);
mt_t *mt_inv(const mt_t *mt); // inverse
mt_t *mt_solve(const mt_t *a, const mt_t *b);
double mt_det(mt_t *mt); // determinant

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
