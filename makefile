CC = gcc
OBJS = electionTestsExample.o election.o map_serializer.o map.o node.o
EXEC = mtm_ex1
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 --pedantic-errors -Wall -Werror

$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

./tests/electionTestsExample.o: tests/electionTestsExample.c election.h mtm_map/map.h test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

election.o: election.c election.h mtm_map/map.h map_serializer.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

map_serializer.o: map_serializer.c map_serializer.h mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

./mtm_map/map.o: mtm_map/map.c mtm_map/map.h mtm_map/node.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

./mtm_map/node.o: mtm_map/node.c mtm_map/node.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS) $(EXEC)