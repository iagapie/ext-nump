#ifndef NUMP_PHP_PARAMETERS_H
#define NUMP_PHP_PARAMETERS_H

#define SETUP_CALLABLE_VARS() \
zend_fcall_info fci = empty_fcall_info; \
zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

#define PARSE_1(spec, a)        if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), spec, a) == FAILURE) return
#define PARSE_2(spec, a, b)     if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), spec, a, b) == FAILURE) return
#define PARSE_3(spec, a, b, c)  if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), spec, a, b, c) == FAILURE) return

#define PARSE_NONE if (zend_parse_parameters_none() == FAILURE) return

#define PARSE_ZVAL(z) \
zval *z = NULL; \
PARSE_1("z", &z)

#define PARSE_ARRAY(a) \
zval *a = NULL; \
PARSE_1("a", &a)

#define PARSE_LONG_OPTIONAL_LONG_LONG(i1, i2, i3) \
zend_long i1 = 0; \
zend_long i2 = 0; \
zend_long i3 = 0; \
PARSE_3("l|ll", &i1, &i2, &i3)

#define PARSE_OPTIONAL_LONG_LONG(i1, i2) \
zend_long i1 = 0; \
zend_long i2 = 0; \
PARSE_2("|ll", &i1, &i2)

#define PARSE_OBJ(o, ce) \
zval *o = NULL; \
PARSE_2("O", &o, ce)

#define PARSE_CALLABLE() \
SETUP_CALLABLE_VARS(); \
PARSE_2("f", &fci, &fci_cache)

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
