CC = gcc
CFLAGS = -I -std=c11 -pedantic -g3
DEPS = stack_vm/stack_vm.h test/stack_vm_test.h
VPATH = stack_vm:test:assembler

all:
	make main
	make stack_vm_test

%.o: %.c (DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o stack_vm.o
	$(CC) -o $@ $^ $(CFLAGS)

stack_vm_test: stack_vm_test.o stack_vm.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o main stack_vm_test