# Generated with github.com/da0x/mmpp
# Binary:
mmpp: .obj .obj/main.cpp.o
	g++ -std=c++20 .obj/main.cpp.o -o mmpp

.obj:
	mkdir .obj

.obj/main.cpp.o: main.cpp
	g++ -std=c++20 -o .obj/main.cpp.o -c main.cpp

run: mmpp
	./mmpp

clean:
	rm -rfv .obj

install:
	sudo cp -v mmpp /usr/local/bin/

uninstall:
	sudo rm -v /usr/local/bin/mmpp
