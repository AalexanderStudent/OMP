
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
	int i, j;
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
#pragma omp parallel shared(a, b, c)
	{
#pragma omp for schedule(static) reduction(+:total)
		for (i = 0; i < NMAX; i++)
			total += addition_total(i, a, b, c);
	}
	double stopmp = omp_get_wtime();
	double durationmp = stopmp - startmp;
	std::cout << "Duration (regular openMP): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl;

	startmp = omp_get_wtime();
	total = 0;
#pragma omp parallel shared(a, b, c) 
	{
#pragma omp for private(i)
		for (i = 0; i < NMAX; i++)
		{
			addition = addition_total(i, a, b, c);
#pragma omp atomic
			total += addition;
		}
	}
	stopmp = omp_get_wtime();
	durationmp = stopmp - startmp;
	std::cout << "Duration (openMP atomic): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl;

	startmp = omp_get_wtime();
	total = 0;
#pragma omp parallel shared(a, b, c)
	{
#pragma omp sections private(i, j) reduction(+:total)
		{
#pragma omp section
			for (i = 0; i < NMAX; i++)
			{
				if (a[i] % 2 == 0)
				{
					addition = b[i] + c[i];
					if (addition == 1)
						addition = 0;
					total += addition;
				}
			}
#pragma omp section
			for (j = 0; j < NMAX; j++)
			{
				if (a[j] % 2 != 0)
				{
					addition = b[j] - a[j];
					if (addition == 1)
						addition = 0;
					total += addition;
				}
			}
		}
	}

	stopmp = omp_get_wtime();
	durationmp = stopmp - startmp;
	std::cout << "Duration (openMP sections type 1): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl;


	int* tot = new int[5];
	total = 0;
	startmp = omp_get_wtime();
	for (i = 0; i < 5; i++)
		tot[i] = 0;
#pragma omp parallel shared(a, b, c) private(i)
	{
#pragma omp sections
		{
			for (i = 0; i < NMAX / 5; i++)
				tot[0] += addition_total(i, a, b, c);
#pragma omp section
			for (i = NMAX / 5; i < 2 * NMAX / 5; i++)
				tot[1] += addition_total(i, a, b, c);
#pragma omp section
			for (i = 2 * NMAX / 5; i < 3 * NMAX / 5; i++)
				tot[2] += addition_total(i, a, b, c);
#pragma omp section
			for (i = 3 * NMAX / 5; i < 4 * NMAX / 5; i++)
				tot[3] += addition_total(i, a, b, c);
#pragma omp section
			for (i = 4 * NMAX / 5; i < 5 * NMAX / 5; i++)
				tot[4] += addition_total(i, a, b, c);
		}
	}
	for (i = 0; i < 5; i++)
		total += tot[i];
	stopmp = omp_get_wtime();
	durationmp = stopmp - startmp;
	std::cout << "Duration (openMP sections type 2): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl;
	system("pause");
}
