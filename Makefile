CFLAGS ?= -O2 -g
CFLAGS += -std=gnu99
 
CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
	-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
	-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal -Wabsolute-value
 
CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error
 
CC += -m32 -no-pie -fno-pie
 
LDLIBS = -lm
 
.PHONY: all

all: integral

integral: main.o functions.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $< 

functions.o: functions.asm
	nasm -g -f elf32 $< -o $@
	
test: all
	./integral --test-root 1:2:3.7:3.9:0.0001:3.848
	./integral --test-root 1:3:0.5:1.0:0.0001:0.8541
	./integral --test-root 2:3:3.2:3.5:0.0001:3.244
	./integral --test-integral 1:0.854:3.848:0.0001:13.2053
	./integral --test-integral 2:3.244:3.848:0.0001:1.71302
	./integral --test-integral 3:0.854:3.244:0.0001:4.00389
clean: 
	rm -rf main.o functions.o integral
