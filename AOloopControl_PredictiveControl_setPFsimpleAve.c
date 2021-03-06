/**
 * @file    AOloopControl_PredictiveControl_setPFsimpleAve.c
 * @brief   Predictive filter time averaging
 * 
 * Used to give more weigth to most recent measurements
 * 
 *
 * 
 * ## Change log
 * - 20180518	Guyon	File created 
 * 
 *  
 * @author  O. Guyon
 *
 * @bug No known bugs.
 * 
 * 
 */



#define _GNU_SOURCE





#include <math.h>

#include "CommandLineInterface/CLIcore.h"
#include "COREMOD_memory/COREMOD_memory.h"







/**
 * 
 * DecayCoeff is betweeen 0 and 1
 * 1 : no decay, pure average
 * 
 * This is used to give more weigth to most recent measurements
 * 
 */
long AOloopControl_PredictiveControl_setPFsimpleAve(
	char *IDPF_name, 
	float DecayCoeff
	)
{
	long IDPF;
	int xsize, ysize; 
	int FilterOrder;
	int ii, jj, kk; 
	float *coeff;
	float total;
	
	IDPF = image_ID(IDPF_name);
	xsize = data.image[IDPF].md[0].size[0];
	ysize = data.image[IDPF].md[0].size[1];
	FilterOrder = xsize/ysize;
	
	coeff = (float*) malloc(sizeof(float)*FilterOrder);
	
	// set up coeffs and compute their sum
	total = 0.0;
	for(kk=0;kk<FilterOrder;kk++)
	{
		coeff[kk] = pow(DecayCoeff, kk);
		total += coeff[kk];
	}
	// normalize such that sum of coeffs is 1
	for(kk=0;kk<FilterOrder;kk++)
		coeff[kk] /= total;
	
	
	printf("Filter order = %d\n", FilterOrder);
	for(kk=0;kk<FilterOrder;kk++)
	{
		for(ii=0;ii<ysize;ii++)
			for(jj=0;jj<ysize;jj++)
				{
					data.image[IDPF].array.F[jj*xsize + ii+kk*ysize] = 0.0;
				}
		for(ii=0;ii<ysize;ii++)
			data.image[IDPF].array.F[ii*xsize + ii+kk*ysize] = coeff[kk];
	}
	
	
	free(coeff);
	
	return(IDPF);
}
