.PHONY: all
all: main

main: main.c filter.i
	$(CC) $(CCFLAGS) main.c -o main

filter.i: filter.bpf
	bpf_asm -c $^ > $@

.PHONY: clean
clean:
	rm -f filter.i
