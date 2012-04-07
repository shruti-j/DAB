
#include "mex.h"

/* Input Arguments */

#define	DATA	prhs[0]


/* Output Arguments */

#define	ENCSIG_OUT	plhs[0]

#include<stdio.h>
#include<string.h>
#include<math.h>
#include <stdlib.h>
#include <time.h>
#define num_of_states 4
#define K 3
#define start_state 0


#if !defined(MAX)
#define	MAX(A, B)	((A) > (B) ? (A) : (B))
#endif

#if !defined(MIN)
#define	MIN(A, B)	((A) < (B) ? (A) : (B))
#endif

  
struct nxt_state
{
	int inp;
	int state;
	double output[2];
        
};

void Dec2Bin(int n,int bin[])
{
    int i;
   if(n==0)
   {
        for(i=0;i<=2;i++)	
	bin[i]=0;
	return ;

   }
    
    for(i=0;n!=0&& i <K;i++)
    {
        bin[i]=n%2;
        n=n/2;
    }
    
}


struct nxt_state next_state(int current_state,int input,struct nxt_state nextstate)
{   	
    int binary_state[K - 1];              /* binary value of current state */
    int next_state_binary[K - 1];         /* binary value of next state */
    int next_state;                       /* decimal value of next state */
    int i;                                /* loop variable */
    int memory_contents[K];
    
    for(i=0;i<K;i++)
    memory_contents[i]=0;

    for(i=0;i<K-1;i++)     
    binary_state[i]=0;
   
    /* convert the decimal value of the current state number to binary */
    Dec2Bin(current_state, binary_state);
    /* given the input and current state number, compute the next state number */
    next_state_binary[0] = input;
    for(i=1;i<K-1;i++)
    next_state_binary[i] = binary_state[i-1];
	
     /*convert the binary value of the next state number to decimal */
     if (next_state_binary[1]==0 && next_state_binary[0]==0)
	nextstate.state=0;
     else
	if(next_state_binary[1]==0 && next_state_binary[0]==1)
	nextstate.state=1;
	else
		if(next_state_binary[1]==1 && next_state_binary[0]==0)
		nextstate.state=2;
		else
		nextstate.state=3;
	
        memory_contents[0] = input;
	
       for (i = 1; i < K; i++)
        memory_contents[i] = binary_state[i - 1];
    	nextstate.output[1]=(double)(memory_contents[0]^(memory_contents[1]^memory_contents[2]));   /*([0...n] -> MSB ...LSB)*/
	nextstate.output[0]=(double)(memory_contents[0]^memory_contents[2]);
	
        
	 /*printf("\n the output  of the %d state %f %f \n",nextstate.state,nextstate.output[1],nextstate.output[0]);   */
    	nextstate.inp=input;
 return(nextstate);
}


void Convolutional_encoder(double encsig[],double data[],int MSG_LEN){

struct nxt_state nextstate;
int current_state,input;
long int i=0;

current_state=start_state;
  while(i<MSG_LEN){
  nextstate=next_state(current_state,(int)data[i],nextstate);
  encsig[2*i]=nextstate.output[1];
  encsig[2*i+1]=nextstate.output[0];
  i++;
  
  current_state=nextstate.state;
  }
   return;
}

void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray*prhs[] )
     
{ 
    double *data; 
    double *encsig; 
    mwSize m,n; 
    
    /* Check for proper number of arguments */
    
    if (nrhs != 1) { 
	mexErrMsgTxt("Minimum one input argument required."); 
    } else if (nlhs > 1) {
	mexErrMsgTxt("Too many output arguments."); 
    } 
    
    /* Check the dimensions of Y.  Y can be N X 1 or 1 X N. */ 
    
    m = mxGetM(DATA); 
    n = mxGetN(DATA);
    if (!mxIsDouble(DATA) || mxIsComplex(DATA) || (MAX(m,n) == 0) || (MIN(m,n) != 1) ) { 
	mexErrMsgTxt("improper input"); 
    } 
    
    /* Create a matrix for the return argument */ 
    ENCSIG_OUT = mxCreateDoubleMatrix(1,2*MAX(m,n), mxREAL); 
    
    /* Assign pointers to the various parameters */ 
    encsig = mxGetPr(ENCSIG_OUT);
     
    data = mxGetPr(DATA);
        
    /* Do the actual computations in a subroutine */
    Convolutional_encoder(encsig,data,(int) MAX(m,n)); 
    return;
    
}



