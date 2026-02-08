test : test.cc
	g++ -o $@ $^ -std=c++17 -L /usr/lib/x86_64-linux-gnu -lhiredis -L /usr/local/lib -lredis++  -lpthread -Wl,-rpath=/usr/local/lib:/usr/lib/x86_64-linux-gnu
.PHONY : clean
clean : 	
	rm -f test