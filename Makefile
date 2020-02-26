# Written 2/23/2020 #
all: clean1 windows clean2 linux clean3

windows:
	make -f windows.mk
linux:
	make -f linux.mk

clean1:
	rm -f *.o
clean2:
	rm -f *.o
clean3:
	rm -f *.o