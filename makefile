make:
	g++ -std=c++11 -O2 -Wall -o RayTracer *.cpp -lxcb -lxcb-image

PHONY: test clean

test:
	make && ./RayTracer

clean:
	rm -f ./RayTracer
