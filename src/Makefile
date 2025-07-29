all: install

install: uninstall
	mkdir -p build
	cd build && cmake ..
	$(MAKE) -C build

uninstall: clean
	rm -rf build

run:
	./build/viewer

clean:
