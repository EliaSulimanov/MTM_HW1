CC = gcc
OBJS = election.o map.o node.o
EXEC = mtm_ex1
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 --pedantic-errors -Wall -Werror

$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

election.o: election.c election.h mtm_map/map.h mtm_map/node.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

map.o: mtm_map/map.c mtm_map/map.h mtm_map/node.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

node.o: mtm_map/node.c mtm_map/node.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS) $(EXEC)