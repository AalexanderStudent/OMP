#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <ctime> 

int static NMAX = 10000;

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

	std::cout << "Small test. Printing out 5 elements: \n";
	std::cout << "Printing out the A array: \n";
	for (i = 0; i < 5; i++)
		std::cout << a[i] << " ";
	std::cout << std::endl;
	std::cout << "Printing out the B array: \n";
	for (i = 0; i < 5; i++)
		std::cout << b[i] << " ";
	std::cout << std::endl;
	std::cout << "Printing out the C array: \n";
	for (i = 0; i < 5; i++)
		std::cout << c[i] << " ";
	std::cout << std::endl;
#pragma omp parallel shared(a, b, c)
	{
#pragma omp for schedule(static) reduction(+:total)
		for (i = 0; i < 5; i++)
			total += addition_total(i, a, b, c);
	}
	std::cout << "Result of test (openMP): " << total << std::endl;
	total = 0;
	for (i = 0; i < 5; i++)
		total += addition_total(i, a, b, c);
	std::cout << "Result of test (no openMP): " << total << std::endl << std::endl;
	

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
	std::cout << "Duration (openMP): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl;

	
	startmp = omp_get_wtime();
	total = 0;
	for (i = 0; i < NMAX; i++)
		total += addition_total(i, a, b, c);
	stopmp = omp_get_wtime();
	durationmp = stopmp - startmp;
	std::cout << "Duration (no openMP): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl;

	system("pause");
}
