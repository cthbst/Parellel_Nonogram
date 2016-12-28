#include <bits/stdc++.h>
#include <omp.h>

long long int fib_ser(int n)
{
	if (n < 2)
		return n;
	else
		return fib_ser(n-1) + fib_ser(n-2);
}

long long int fib(int n)
{
	long long int i, j;

	if (n <= 20)
		return fib_ser(n);
	else
	{
    #pragma omp task shared(i)
		i = fib(n-1);
    #pragma omp task shared(j)
		j = fib(n-2);
    #pragma omp taskwait
		return i+j;
	}
}


int main()
{
	int n;
	scanf("%d",&n);
	printf("%lld",fib(n));
	return 0;
}