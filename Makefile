CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

test_Vargs: Vargs.hpp test_Vargs.cpp
	g++ $(CXXFLAGS) test_Vargs.cpp -o test_Vargs

#test_Vargs.o: test_Vargs.cpp Vargs.hpp
#	$(CXX) $(CXXFLAGS) -c test_Vargs.cpp

#Vargs.o: Vargs.cpp Vargs.hpp
#	$(CXX) $(CXXFLAGS) -c Vargs.cpp

clean:
	rm -f test_Vargs test_Vargs.o #Vargs.o
