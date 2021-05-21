# Written 2/23/2020 #
all: windows linux

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