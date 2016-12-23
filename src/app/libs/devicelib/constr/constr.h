#ifndef CONSTR_H
#define CONSTR_H

#include "../../../duktapert/duktape.h"
#include "../../../helper/helper.h"

#ifdef __cplusplus
extern "C" {
#endif
    duk_ret_t constr_constructor(duk_context *ctx);

    void constr_init(duk_context *ctx);
    void constr_test(duk_context *ctx);

#ifdef __cplusplus
}
#endif

#endif /* CONSTR_H */