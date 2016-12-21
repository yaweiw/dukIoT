#ifndef HELPER_H
#define HELPER_H

#include "../duktapert/duktape.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DUKAPIHANDLE
#define DUKAPIHANDLE void*
#endif

    int duk_register(duk_context *ctx, DUKAPIHANDLE cFuncName, const char* propName);

#ifdef __cplusplus
}
#endif

#endif /* HELPER_H */