#include <iostream>
#include <cstring>
#include <omp.h>

template<class T> void merge(T *a, int l_border, int m_border, int r_border)
{
    int size = r_border - l_border + 1;
    int i = l_border, j = m_border + 1, k = 0;
    T *tmp = (T*) malloc(size * sizeof(T));

    for (; i <= m_border && j <= r_border; k++)
        tmp[k] = (a[i] < a[j]) ? a[i++] : a[j++];

    if (m_border - i + 1)
    {
        memcpy(&tmp[k], &a[i], (m_border - i + 1) * sizeof(T));
        k += m_border - i + 1;
    }

    if (r_border - j + 1)
    {
        memcpy(&tmp[k], &a[j], (r_border - j + 1) * sizeof(T));
        k += r_border - j + 1;
    }

    memcpy(&a[l_border], tmp, k * sizeof(T));
	free(tmp);
}


template<class T> void mergesort_serial(T *a, int l_border, int r_border)
{
    if (l_border < r_border)
    {
        int m_border = (l_border + r_border) / 2;
		mergesort_serial(a, l_border, m_border);
		mergesort_serial(a, m_border+1, r_border);
       	merge(a, l_border, m_border, r_border);
    }
}

template<class T> void mergesort(T *a, int l_border, int r_border, int threads)
{
    if ( threads == 1) 
	{
		//printf("Thread %d begins serial merge sort\n", omp_get_thread_num());
    	mergesort_serial(a, l_border, r_border);
    } 
	else if (threads > 1) 
	{	
		if (l_border < r_border)
    	{
       		int m_border = (l_border + r_border) / 2;
		
			#pragma omp parallel sections num_threads(2)
			{
				//printf("Thread %d begins recursive section\n", omp_get_thread_num());				
				#pragma omp section
				{
					//printf("Thread %d begins recursive call\n", omp_get_thread_num());
					mergesort(a, l_border, m_border, threads/2);
				}
			
				#pragma omp section
				{
					//printf("Thread %d begins recursive call\n", omp_get_thread_num());					
					mergesort(a, m_border+1, r_border, threads - threads/2);
				}
			}		
       		merge(a, l_border, m_border, r_border);
		}
    }
}

void run_omp(int a[], int size, int threads) {
    // Enable nested parallelism, if available
    omp_set_nested(1);
    // Parallel mergesort
    mergesort<int>(a, 0, size - 1, threads);
}


int main(int argc, char* argv[])
{
	int size = 1e6;
    // Check arguments
    if ( argc != 2 ) /* argc must be 3 for proper execution! */
    {
        printf( "Usage: %s number-of-threads\n", argv[0]);
        return 1;
    }
	// Get arguments
    int threads = atoi(argv[1]); // Requested number of threads

	// Check nested parallelism availability
	omp_set_nested(1);
    if (omp_get_nested() !=1 )
    {
		std::cout << "Warning: Nested parallelism desired but unavailable" << std::endl;
    } 
	// Check processors and threads
    int processors = omp_get_num_procs(); // Available processors
    printf("Array size = %d\nThreads = %d\nProcessors = %d\n", size, threads, processors);
    if (threads > processors) 
    {
        printf( "Warning: %d threads requested, will run on %d processors available\n", threads, processors);
		omp_set_num_threads(threads);
    }
    int max_threads = omp_get_max_threads(); // Max available threads
    if (threads > max_threads) // Requested threads are more than max available
    {
        printf( "Error: Cannot use %d threads, only %d threads available\n", 
		threads, max_threads);
		return 1;
	}

    int a[int(size)];

    srand(time(NULL));

    for (int i = 0; i < size; i++)
        a[i] = rand() % size;

    clock_t tick = clock();
    double start = omp_get_wtime();
    mergesort<int>(a, 0, size - 1, threads);
	//run_omp(a, size, threads);
    double end = omp_get_wtime( );
    printf("Start = %.2f\nEnd = %.2f\nElapsed = %.2f\n", start, end, end - start);
	
    clock_t tock = clock();
	printf("Processor time in clock ticks = %d\n", tock - tick);
	
	// Result check
    for(int i=1; i<size; i++) {
        if (!(a[i-1] <= a[i])) {
            printf("Implementtion error: a[%d]=%d > a[%d]=%d\n", i-1, a[i-1], i, a[i]);
            return 1;
        }
    }
    std::cout << "-Success-" << std::endl;

    return 0;
}
