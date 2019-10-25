#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"

#include "php_nump.h"
#include "src/php/classes/php_matrix_ce.h"
#include "src/php/functions/php_matrix_fn.h"

PHP_MINIT_FUNCTION(nump)
{
	php_register_matrix();
	
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(nump)
{
    return SUCCESS;
}

PHP_RINIT_FUNCTION(nump)
{
#if defined(ZTS) && defined(COMPILE_DL_NUMP)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(nump)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(nump)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "nump support", "enabled");
	php_info_print_table_row(2, "nump version", PHP_NUMP_VERSION);
	php_info_print_table_end();
}

static const zend_function_entry nump_functions[] = {
	PHP_MATRIX_FE_LIST
	PHP_FE_END
};

static const zend_module_dep nump_deps[] = {
	ZEND_MOD_REQUIRED("spl")
    ZEND_MOD_END
};

zend_module_entry nump_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	nump_deps,
	"nump",					/* Extension name */
	nump_functions,			/* zend_function_entry */
	PHP_MINIT(nump),		/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(nump),	/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(nump),		/* PHP_RINIT - Request initialization */
	PHP_RSHUTDOWN(nump),	/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(nump),		/* PHP_MINFO - Module info */
	PHP_NUMP_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_NUMP
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(nump)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
