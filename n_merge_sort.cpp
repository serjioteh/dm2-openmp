#include <iostream>
#include <cstring>
#include <omp.h>


template<class T> void merge(T *a, int l_border, int m_border, int r_border)
{
    int i = l_border, j = m_border + 1, k = 0;
    T tmp[r_border-l_border+1];

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
}


template<class T> void mergesort(T *a, int l_border, int r_border)
{
    if (l_border < r_border)
    {
        int m_border = (l_border + r_border) / 2;
        mergesort(a, l_border, m_border);
        mergesort(a, m_border+1, r_border);
        merge(a, l_border, m_border, r_border);
    }
}

int main()
{
	int size = 1e6;
	
    int a[int(size)];

    srand(time(NULL));

    for (int i = 0; i < size; i++)
        a[i] = rand() % size;

    clock_t tick = clock();
    double start = omp_get_wtime();
    mergesort<int>(a, 0, size - 1);
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
