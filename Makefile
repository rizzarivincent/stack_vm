CC = gcc
CFLAGS = -I -std=c11 -pedantic
DEPS = stack_vm.h stack_vm_test.h

%.o: %.c (DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all:
	make main
	make test

main: main.o stack_vm.o
	$(CC) -o $@ $^ $(CFLAGS)

test: stack_vm_test.o stack_vm.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o main test