clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f *.plist
	rm -f *.tmp
	rm -f *.orig
	rm -f Handle3T
	rm -f Handle2T
	rm -r *.dSYM

format:
	astyle Shape1hr.c++
	astyle Shape2hr.c++
	astyle Shape3hr.c++
	astyle Vector2hr.c++
	astyle Vector3hr.c++
	astyle Vector4hr.c++
	astyle Handle1hr.c++
	astyle Handle2T.c++
	astyle Handle3T.c++

Handle3T: Handle3T.c++
	-cppcheck Handle3T.c++
	g++ -fprofile-arcs -ftest-coverage -pedantic -std=c++14 -Wall -Weffc++ -Wextra Handle3T.c++ -o Handle3T -lgtest -lgtest_main -pthread

Handle3T.c++x: Handle3T
	valgrind ./Handle3T
	gcov -b Handle3T.c++ | grep -A 5 "File '.*Handle3T.c++'"

Handle2T: Handle2T.c++
	-cppcheck Handle2T.c++
	g++ -fprofile-arcs -ftest-coverage -pedantic -std=c++14 -Wall -Weffc++ -Wextra Handle2T.c++ -o Handle2T -lgtest -lgtest_main -pthread
	
Handle2T.c++x: Handle2T
	valgrind ./Handle2T
	gcov -b Handle2T.c++ | grep -A 5 "File '.*Handle2T.c++'"

tests: Handle2T.c++x Handle3T.c++x

