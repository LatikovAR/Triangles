CXX = g++
CXXFLAGS = 

DEF_SRC =  main.cpp geometry.cpp
DEF_T_SRC =  main.cpp geometry.cpp triangle_test.cpp

normal: $(DEF_SRC)
	$(CXX) $(CXXFLAGS) $? -DNORMAL -o prog
test0: $(DEF_T_SRC)
	$(CXX) $(CXXFLAGS) $? -DTEST0 -o prog
test1: $(DEF_T_SRC)
	$(CXX) $(CXXFLAGS) $? -DTEST1 -o prog
test2: $(DEF_T_SRC)
	$(CXX) $(CXXFLAGS) $? -DTEST2 -o prog
test3: $(DEF_T_SRC)
	$(CXX) $(CXXFLAGS) $? -DTEST3 -o prog
test4: $(DEF_T_SRC)
	$(CXX) $(CXXFLAGS) $? -DTEST4 -o prog
test5: $(DEF_T_SRC)
	$(CXX) $(CXXFLAGS) $? -DTEST5 -o prog
test6: $(DEF_T_SRC)
	$(CXX) $(CXXFLAGS) $? -DTEST6 -o prog
test7: $(DEF_T_SRC)
	$(CXX) $(CXXFLAGS) $? -DTEST7 -o prog
test8: $(DEF_T_SRC)
	$(CXX) $(CXXFLAGS) $? -DTEST8 -o prog



