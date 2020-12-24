all: test
test: test.o
	g++ -O2 `./cabocha-config --cflags`  -o test test.o JapanesePackage.o JapaneseChunk.o `./cabocha-config --libs` 

test.o: test.cpp
	g++ -c test.cpp JapanesePackage.cpp JapaneseChunk.cpp cabocha.h JapaneseChunk.h JapanesePackage.h 
clean:
	rm *.gch *.o

# all:  test
# test: test.o
# 	g++ -O2 `./mecab-config --cflags` -o test test.o JapanesePackage.o JapaneseTermExtract.o `./mecab-config --libs`
# test.o: test.cpp
# 	g++ -c test.cpp JapanesePackage.cpp JapaneseTermExtract.cpp mecab.h JapaneseTermExtract.h JapanesePackage.h
# clean: 
# 	rm *.gch *.o

