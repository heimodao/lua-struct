
CFLAGS = -Wall -O0 -g

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $< -I$(HOME)/Software/lua-5.2.3/include -c

main : main.o struct.o
	$(CC) $(CFLAGS) -o $@ $^ -L$(HOME)/Software/lua-5.2.3/lib -llua

clean :
	$(RM) main *.o



struct.o : struct.h

