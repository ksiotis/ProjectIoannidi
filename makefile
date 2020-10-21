#old makefile, to be changed

make: builder utilities.o records.o avl.o hashtable.o maxheap.o cleanmain.o prioritylist.o cleanserver.o cleanclient.o threads_n_sockets.o childmain whoServer whoClient
	g++ -std=c++11 master.cpp builder/*.o -o master -Wall -g3

test:
	g++ -std=c++11 testmain.cpp builder/*.o -o test -Wall -g3

childmain:
	g++ -std=c++11 childmain.cpp builder/*.o -o builder/childmain -Wall -g3

whoServer:
	g++ -std=c++11 whoServer.cpp builder/*.o -o whoServer -Wall -pthread -g3

whoClient:
	g++ -std=c++11 whoClient.cpp builder/*.o -o whoClient -Wall -pthread -g3

builder:
	mkdir builder

utilities.o: cleanmain.o
	g++ -std=c++11 -c include/utilities.cpp -o builder/utilities.o -Wall -g3

records.o:
	g++ -c -std=c++11 include/records.cpp -o builder/records.o -Wall -g3

avl.o:
	g++ -c -std=c++11 include/avl.cpp -o builder/avl.o -Wall -g3

hashtable.o:
	g++ -c -std=c++11 include/hashtable.cpp -o builder/hashtable.o -Wall -g3

maxheap.o:
	g++ -c -std=c++11 include/maxheap.cpp -o builder/maxheap.o -Wall -g3

prioritylist.o:
	g++ -std=c++11 -c include/prioritylist.cpp -o builder/prioritylist.o -Wall -g3

cleanmain.o:
	g++ -std=c++11 -c include/cleanmain.cpp -o builder/cleanmain.o -Wall -g3

cleanserver.o:
	g++ -std=c++11 -c include/cleanserver.cpp -o builder/cleanserver.o -Wall -g3

cleanclient.o:
	g++ -std=c++11 -c include/cleanclient.cpp -o builder/cleanclient.o -Wall -g3

threads_n_sockets.o:
	g++ -std=c++11 -c include/threads_n_sockets.cpp -o builder/threads_n_sockets.o -Wall -pthread -g3

clean:
	rm -r master whoServer whoClient ./builder

cleantest:
	rm ./test
