#ifndef PHP_NUMP_H
#define PHP_NUMP_H

extern zend_module_entry nump_module_entry;
#define phpext_nump_ptr &nump_module_entry

#define PHP_NUMP_VERSION "1.0.0"

#if defined(ZTS) && defined(COMPILE_DL_NUMP)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
