#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <locale.h>


int main(int argc, char **argv){
	//used to get diffrent arguments in command line
	extern char *optarg;
	extern int optind;
	// c = goes through entire command line looking for options
	int c, err = 0;
	int sflag=0, rflag=0, dflag=0;
	//annual intrest rate REQUIRED
	float rval;
	//15 year mortgage OPTIONAL set default value
	int s = 30;
	//monthly intrest & monthly payments & temp
	float a , b, temp;
	int downPayment = 0, homePrice;
	float result;

	static char usage[] = "Usage: mortgagepmt [-s] -r rate [-d downpayment] price \n";

	while((c=getopt(argc, argv,"r:sd:")) != -1)
		switch(c){
		case 's':
			sflag = 1;
			s=15;
			break;
		case 'r':
			rflag = 1;
			rval = atoi(optarg);
			break;
		case 'd':
			dflag = 1;
			downPayment = atoi(optarg);
			break;
		case '?':
			err = 1;
			break;
		}


	// -r is mandatory
	if (rflag == 0){
		fprintf(stderr, "%s: missing -r option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
	else if ((optind+1) > argc){
		/* need at least one argument */
		fprintf(stderr, "%s: missing price\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
	else if (err) {
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
	homePrice = atoi(argv[optind]);

	//Make sure downpayment is not greater than house price
	if (downPayment > homePrice){
		printf("Down payment cannot be greater than house price\n");
		exit(1);
	}
	// The interest rate should be between 3% and 10% inclusive.
	if (rval > 10 || rval < 3){
		printf("Interest rate should be between 3%% and 10%%\n");
		exit(1);
	}
    
	/* Calculate mortgage payment */
	b = (rval/100) / 12;
	temp = b + 1;
	temp = pow(temp, -(s * 12));
	temp = 1 - temp;
	result = b / temp;
	result *= homePrice;

	//Subtract homeprice with downpayment to calculate loan
	homePrice -= downPayment;
	
	//formatting with comma
	setlocale(LC_NUMERIC, "");

	/* Add PMI if down payment < 20% */
	if (downPayment < ((homePrice + downPayment) * 0.2)){
		result = ((0.01 * homePrice)/12) + result;
		// End Result
		printf("The payment on a loan of $%'.2i with an interest rate of %.3f%% for a term of %i years will be $%.2f\n",homePrice,rval, s, result);
		exit(0);
	}


	// End result
	printf("The payment on a loan of $%'.2i with an interest rate of %.3f%% for a term of %i years will be $%.2f\n",homePrice,rval, s, result);
	exit(0);
}