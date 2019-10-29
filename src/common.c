#include "php.h"
#include "zend_hash.h"
#include "zend_exceptions.h"
#include "ext/spl/spl_exceptions.h"
#include "ext/standard/php_rand.h"

#include "common.h"

void nump_throw_exception(zend_class_entry *ce, const char *format, ...)
{
    va_list ap;
    zend_string *str;

    va_start(ap, format);
    str = vstrpprintf(0, format, ap);
    va_end(ap);

    zend_throw_exception(ce, str->val, 0);
    zend_string_release(str);
}

zend_always_inline HashTable *nump_new_array(uint32_t size)
{
    HashTable *ht = CALLOC(1, HashTable);
	_zend_hash_init(ht, size < HT_MIN_SIZE ? HT_MIN_SIZE : size, ZVAL_PTR_DTOR, 0);
	return ht;
}

zend_always_inline double nump_rand()
{
    return (double) (php_rand() >> 1) / (double) RAND_MAX;
}

zend_always_inline double nump_randn()
{
    return sqrt(-2.0 * log(nump_rand())) * cos(2.0 * acos(-1.0) * nump_rand());
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
