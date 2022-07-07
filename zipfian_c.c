#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main()
{
    int i;
    double a = 0.2, sum = 0.0;
    double* Pzif;
    int* fingerprint;

    Pzif = (double*)calloc(101, sizeof(double));
    fingerprint = (int*)calloc(101, sizeof(int));

    for (i = 0; i <= 101; i++){
        Pzif[i] = 0.0;
        fingerprint[i] = -1;
    }
    for(i = 1; i <= 101; i++)
	{
		sum += 1 / pow((double)i, a);
	}

	for(i = 1; i <= 101; i++)
	{
		Pzif[i] = Pzif[i-1] + 1 / pow((double)i, a) / sum;
	}
    int mid = 0;
    int fp = 0;
    double data = ((double)rand()+1)/((double)RAND_MAX+2);
    int low = 0;
    int high = 100;
    while (low < high)
    {
        mid = low + (high-low+1)/2;

        if (data <= Pzif[mid]) 
        {
            if (data > Pzif[mid-1])
            {
                fp = mid;
                break;
            }

            high = mid-1;
        } 
        else 
        {
            low = mid;
        }
    }
    int dup_ppn = 0;
    dup_ppn = fingerprint[fp];
    printf("%d\n", dup_ppn);
}