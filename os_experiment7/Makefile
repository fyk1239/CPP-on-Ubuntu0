Obj = BitMap.o Directory.o FileSystem.o main.o
FileSystem.out : $(Obj)
	g++ -o FileSystem.out $(Obj)

BitMap.o : BitMap.h BitMap.cpp
	g++ -c BitMap.cpp
Directory.o : Directory.h Directory.cpp
	g++ -c Directory.cpp
FileSystem.o : FileSystem.h FileSystem.cpp
	g++ -c FileSystem.cpp
main.o : FileSystem.h main.cpp
	g++ -c main.cpp

clean :
	-rm $(Obj)

