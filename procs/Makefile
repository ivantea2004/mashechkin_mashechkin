
all: parent_child run_prog parallel async_counter async_counter_atomic async_counter_mutex

parent_child: parent_child.c
	gcc -Wall -Werror -Wpedantic -std=gnu2x -O2 parent_child.c -o parent_child
run_prog: run_prog.c
	gcc -Wall -Werror -Wpedantic -std=gnu2x -O2 run_prog.c -o run_prog
parallel: parallel.c
	gcc -Wall -Werror -Wpedantic -std=gnu2x -Og parallel.c -o parallel
async_counter: async_counter.c
	gcc -Wall -Werror -Wpedantic -std=gnu2x -Og async_counter.c -o async_counter
async_counter_atomic: async_counter.c
	gcc -Wall -Werror -Wpedantic -std=gnu2x -DUSE_ATOMIC=1 -Og async_counter.c -o async_counter_atomic
async_counter_mutex: async_counter.c
	gcc -Wall -Werror -Wpedantic -std=gnu2x -DUSE_MUTEX=1 -Og async_counter.c -o async_counter_mutex