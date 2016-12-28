if (use_test_runtime)
   add_library(duktapert testrt/duktape.c)
else ()
   add_library(duktapert duktape.c)
endif (use_test_runtime)

install (TARGETS duktapert DESTINATION lib)