LIBRARY=libwrap.so


$(LIBRARY): wrap.c
	gcc -Wall -fPIC -shared $< -o $@ -ldl

clean:
	rm -f *.o $(LIBRARY)
