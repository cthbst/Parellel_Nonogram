#include <ctime>
#include <sys/time.h>



// Counting elapsed time
double elapsed_time(bool reset=true){

//     struct timeval start, end;
// gettimeofday(&start, NULL);

	static struct timeval t1;
		struct timeval t2;
			gettimeofday(&t2,NULL);

				// static struct timespec t1;
					// struct timespec t2;
						// clock_gettime(CLOCK_MONOTONIC, &t2);


							double elapsed = (t2.tv_sec - t1.tv_sec);
								elapsed += (t2.tv_usec - t1.tv_usec) / 1000000.0;


								    // static clock_t t1;
									    // clock_t t2 = clock();
										    // double res = (double)(t2-t1)/CLOCKS_PER_SEC;
												if(reset)t1 = t2;
													return elapsed;
													}
