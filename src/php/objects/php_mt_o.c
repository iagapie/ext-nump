#include "php.h"
#include "zend_smart_str.h"
#include "ext/standard/php_var.h"

#include "php_mt_o.h"
#include "../../common.h"
#include "../handlers/php_mt_handlers.h"
#include "../classes/php_mt_ce.h"

zend_object *php_mt_create_object_ex(mt_t *mt)
{
    php_mt_t *obj = CALLOC(1, php_mt_t);

    zend_object_std_init(&obj->std, php_mt_ce);
    obj->std.handlers = &php_mt_handlers;
    obj->mt = mt;
    
    return &obj->std;
}

zend_always_inline zend_object *php_mt_create_object(zend_class_entry *ce)
{
    return php_mt_create_object_ex(mt_init(NULL, NULL));
}

zend_always_inline zend_object *php_mt_create_object_clone(mt_t *mt)
{
    return php_mt_create_object_ex(mt_clone(mt));
}

int php_mt_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    mt_t *mt = Z_MT_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (IS_MT_EMPTY_P(mt)) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());
    } else {
        zval value;
        smart_str buf = {0};
        double *val;
        zend_ulong i;

        ZVAL_LONG(&value, mt->shape->d)
        php_var_serialize(&buf, &value, &serialize_data);

        for (i = 0; i < mt->shape->d; i++) {
            ZVAL_LONG(&value, mt->shape->axes[i])
            php_var_serialize(&buf, &value, &serialize_data);
        }

        ZVAL_LONG(&value, mt->buffer->size)
        php_var_serialize(&buf, &value, &serialize_data);

        MT_FOREACH_VAL(mt, val) {
            ZVAL_DOUBLE(&value, *val);
            php_var_serialize(&buf, &value, &serialize_data);
        } MT_FOREACH_END();

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);

    return SUCCESS;
}

int php_mt_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    mt_shape_t *shape = NULL;
    mt_buffer_t *buf = NULL;
    zend_ulong i;

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *end = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    while (pos != end) {
        zval *value = var_tmp_var(&unserialize_data);

        if (!php_var_unserialize(value, &pos, end, &unserialize_data)) {
            goto error;
        }

        if (!IS_VALID_P(shape)) {
            shape = mt_shape_init(Z_LVAL_P(value));
            i = 0;

            if (!IS_VALID_P(shape) || shape->d == 0) {
                goto error;
            }

            continue;
        }

        if (!IS_VALID_P(buf)) {
            if (i < shape->d) {
                shape->axes[i++] = Z_LVAL_P(value);
            } else {
                buf = mt_buffer_init(Z_LVAL_P(value));
                i = 0;

                if (!IS_VALID_P(buf) || buf->size == 0) {
                    goto error;
                }
            }

            continue;
        }

        buf->data[i++] = Z_DVAL_P(value);
    }

    ZVAL_MT(object, mt_init(buf, shape));
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    mt_shape_free(shape);
    mt_buffer_free(buf);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    THROW_ERROR("Failed to unserialize data");
    return FAILURE;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
