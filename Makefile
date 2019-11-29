all:
	+$(MAKE) -C src
	./run.out
clean:
	+${MAKE} -C src clean
	rm -f run.out
