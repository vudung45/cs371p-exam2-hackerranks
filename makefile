clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f *.plist
	rm -f *.tmp
	rm -f *.orig
format:
	astyle Shape1hr.c++
	astyle Shape2hr.c++
	astyle Shape3hr.c++
	astyle Vector2hr.c++
	astyle Vector3hr.c++
	astyle Vector4hr.c++
