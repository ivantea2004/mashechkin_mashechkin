
all: queue shmem diff_queue order

queue: queue.c
	gcc -Wall -Werror -Wpedantic -std=gnu2x -Og queue.c -o queue
diff_queue: diff_queue.c
	gcc -Wall -Werror -Wpedantic -std=gnu2x -D_GNU_SOURCE -Og diff_queue.c -o diff_queue 
shmem: shmem.c
	gcc -Wall -Werror -Wpedantic -std=gnu2x -Og shmem.c -o shmem
order: order.c
	gcc -Wall -Werror -Wpedantic -std=gnu2x -Og order.c -o order