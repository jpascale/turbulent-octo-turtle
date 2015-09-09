.PHONY: all
%:
cc -o $@ $^ -lrt -lpthread
SRCS = $(shell ls *.c)
DEPS = $(SRCS:.c=.d)
%.d: %.c
cc $(CFLAGS) $< -MM -MT '$*.o $@' > $@
-include $(DEPS)

