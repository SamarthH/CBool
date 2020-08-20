#include <stdio.h>
#include <stdlib.h>
#include<gsl/gsl_rng.h>

int main(){
	gsl_rng* r = gsl_rng_alloc(gsl_rng_ranlxs2);
	gsl_rng_set(r,100);
	
	int n;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			int k = gsl_rng_uniform_int(r,3);
			printf("%d ",k-1);
		}
		printf("\n");
	}
	
	gsl_rng_free(r);
	return 0;
}
