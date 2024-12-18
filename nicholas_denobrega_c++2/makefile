all: submission

submission: submission.o
	g++ -o submission submission.o

submission.o: submission.cpp
	g++ -c -g submission.cpp

clean:
	rm -f *.o submission