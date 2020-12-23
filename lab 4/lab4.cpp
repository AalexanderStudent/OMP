/*
#include <omp.h>
#include <iostream>
#include <cstdlib>

int static NMAX = 1000000;

void main()
{
	int i;
	long int total = 0, addition;
	int* a = new int[NMAX];
	int* b = new int[NMAX];
	int* c = new int[NMAX];
	for (i = 0; i < NMAX; i++)
	{
		a[i] = i % 256 + 1;
		b[i] = i % 256 + 1;
		c[i] = i % 256 + 1;
	}
	
//	std::cout << "Printing out the A array: \n";
//	for (i = 0; i < NMAX; i++)
//		std::cout << a[i] << " ";
//	std::cout << std::endl;
//	std::cout << "Printing out the B array: \n";
//	for (i = 0; i < NMAX; i++)
//		std::cout << b[i] << " ";
//	std::cout << std::endl;
//	std::cout << "Printing out the C array: \n";
//	for (i = 0; i < NMAX; i++)
//		std::cout << c[i] << " ";
//	std::cout << std::endl;

	double startmp = omp_get_wtime();

	// initial
#pragma omp parallel shared(a, b, c) 
	{
#pragma omp for private(i) schedule(static) reduction(+:total)
		for (i = 0; i < NMAX; i++)
		{
			if (a[i] % 2 == 0)
				addition = b[i] + c[i];
			else
				addition = b[i] - a[i];
			if (addition == 1)
				addition = 0;
			total += addition;
		}
	}

	auto stopmp = omp_get_wtime();
	auto durationmp = stopmp - startmp;
	std::cout << "Duration (openMP reduction): " << durationmp * 1000 << " milliseconds" << std::endl;

	std::cout << "Result: " << total << std::endl;

	// new parts	

	total = 0;

#pragma omp parallel shared(a, b, c) 
	{
#pragma omp for private(i) schedule(static)
		for (i = 0; i < NMAX; i++)
		{
			if (a[i] % 2 == 0)
				addition = b[i] + c[i];
			else
				addition = b[i] - a[i];
			if (addition == 1)
				addition = 0;
#pragma omp atomic
			total += addition;
		}
	}
	stopmp = omp_get_wtime();
	durationmp = stopmp - startmp;
	std::cout << "Duration (openMP atomic): " << durationmp * 1000 << " milliseconds" << std::endl;
	std::cout << "Result: " << total << std::endl;

	system("pause");
}
*/