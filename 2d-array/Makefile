ifdef DOUBLE_SUBSCR
CPPFLAGS += -DDOUBLE_SUBSCR
else
ifdef SINGLE_SUBSCR_SAVED_COL_SZ
CPPFLAGS += -DSINGLE_SUBSCR_SAVED_COL_SZ
endif
endif

CFLAGS += -Wall -Wextra -Werror -Wno-deprecated-declarations -Wno-unused-parameter

main:

.PHONY: clean
clean: ; -$(RM) main
