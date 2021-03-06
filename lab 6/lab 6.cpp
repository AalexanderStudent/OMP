#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

int static NMAX = 1000000;

int addition_total(int i, int* A, int* B, int* C)
{
	int addition;
	if (A[i] % 2 == 0)
		addition = B[i] + C[i];
	else
		addition = B[i] - A[i];
	if (addition == 1)
		addition = 0;
	return addition;
}

void main()
{
	srand((int)time(0));
	int i;
	long int total = 0, addition;
	int* a = new int[NMAX];
	int* b = new int[NMAX];
	int* c = new int[NMAX];
	for (i = 0; i < NMAX; i++)
	{
		a[i] = rand() % 128 + 1;
		b[i] = rand() % 128 + 1;
		c[i] = rand() % 128 + 1;
	}

	std::cout << "Elements per array: " << NMAX << std::endl;

	double startmp = omp_get_wtime();
	total = 0;
	for (i = 0; i < NMAX; i++)
		total += addition_total(i, a, b, c);
	double stopmp = omp_get_wtime();
	double durationmp = stopmp - startmp;
	std::cout << "Duration (no openMP): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl << std::endl;
#pragma omp parallel shared(a, b, c)
	{
#pragma omp for schedule(static) private(addition) reduction(+:total)
		for (i = 0; i < NMAX; i++)
		{
			addition = addition_total(i, a, b, c);
			total += addition;
		}
	}

	startmp = omp_get_wtime();
	total = 0;
#pragma omp parallel shared(a, b, c)
	{
#pragma omp for schedule(static) private(addition) reduction(+:total)
		for (i = 0; i < NMAX; i++)
		{
			addition = addition_total(i, a, b, c);
			total += addition;
		}
	}
	stopmp = omp_get_wtime();
	durationmp = stopmp - startmp;
	std::cout << "Duration (openMP): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl << std::endl;

	omp_lock_t lock;
	omp_init_lock(&lock);

	startmp = omp_get_wtime();
	total = 0;

#pragma omp parallel shared(a, b, c)
	{
#pragma omp for schedule(static) private(addition) 
		for (i = 0; i < NMAX; i++)
		{
			addition = addition_total(i, a, b, c);
			omp_set_lock(&lock);
			total += addition;
			omp_unset_lock(&lock);
		}
	}
	stopmp = omp_get_wtime();
	durationmp = stopmp - startmp;
	std::cout << "Duration (openMP lock): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl << std::endl;


	int* part = new int[omp_get_max_threads()];
	for (i = 0; i < omp_get_max_threads(); i++)
		part[i] = 0;
	
	startmp = omp_get_wtime();
	total = 0;

#pragma omp parallel shared(a, b, c)
	{
#pragma omp for schedule(static) private(addition) 
		for (i = 0; i < NMAX; i++)
		{
			part[omp_get_thread_num()] += addition_total(i, a, b, c);
		}
#pragma omp barrier
#pragma omp master
		for (i = 0; i < omp_get_max_threads(); i++)
		{
			total += part[i];
		}
	}
	stopmp = omp_get_wtime();
	durationmp = stopmp - startmp;
	std::cout << "Duration (openMP barrier): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl;
	system("pause");
}
