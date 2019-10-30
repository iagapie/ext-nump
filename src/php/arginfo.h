#ifndef NUMP_PHP_ARGINFO_H
#define NUMP_PHP_ARGINFO_H

#define ARGINFO_NONE(name) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_ZVAL(name, z) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_INFO(0, z) \
ZEND_END_ARG_INFO()

#define ARGINFO_DOUBLE(name, d) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_TYPE_INFO(0, d, IS_DOUBLE, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_ARRAY(name, a) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_ARRAY_INFO(0, a, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_OBJ(name, o, ce) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_OBJ_INFO(0, o, ce, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_LONG_OPTIONAL_LONG_LONG(name, i1, i2, i3) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_TYPE_INFO(0, i1, IS_LONG, 0) \
ZEND_ARG_TYPE_INFO(0, i2, IS_LONG, 0) \
ZEND_ARG_TYPE_INFO(0, i3, IS_LONG, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_OPTIONAL_LONG_LONG(name, i1, i2) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 0) \
ZEND_ARG_TYPE_INFO(0, i1, IS_LONG, 0) \
ZEND_ARG_TYPE_INFO(0, i2, IS_LONG, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_CALLABLE(name, c) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_CALLABLE_INFO(0, c, 0) \
ZEND_END_ARG_INFO()

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
