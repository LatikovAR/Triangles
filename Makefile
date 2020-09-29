DEF_OPTS = g++ main.cpp geometry.h geometry.cpp
DEF_T_OPTS = g++ main.cpp geometry.h geometry.cpp triangle_test.h triangle_test.cpp

result:
	$(DEF_OPTS) -DNORMAL -o prog
test0:
	$(DEF_T_OPTS) -DTEST0 -o prog
test1:
	$(DEF_T_OPTS) -DTEST1 -o prog
test2:
	$(DEF_T_OPTS) -DTEST2 -o prog
test3:
	$(DEF_T_OPTS) -DTEST3 -o prog
test4:
	$(DEF_T_OPTS) -DTEST4 -o prog
test5:
	$(DEF_T_OPTS) -DTEST5 -o prog


