all:
	g++-6 -fopenmp -std=c++11  -I/usr/local/include -L/usr/local/lib p_merge_sort.cpp -o p_merge_sort
	g++-6 -fopenmp -std=c++11 -I/usr/local/include -L/usr/local/lib n_merge_sort.cpp -o n_merge_sort
clean:
	rm -f p_merge_sort n_merge_sort
