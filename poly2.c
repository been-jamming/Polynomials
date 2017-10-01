#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double sigmoid(double input){
    return 1/(1+exp(-input));
}

unsigned int factorial(unsigned int input){
    if(input <= 1){
        return 1;
    }
    
    unsigned int i;
    unsigned int product = 1;
    
    for(i = 2; i <= input; i++){
        product *= i;
    }
    
    return product;
}

struct polynomial{
    double *values;
	double *teaching_values;
    unsigned int num_vars;
    unsigned int highest_power;
    unsigned int num_terms;
};

typedef struct polynomial polynomial;

polynomial create_polynomial(unsigned int num_vars, unsigned int highest_power){
    polynomial output;
    output = (polynomial) {.num_vars = num_vars, .highest_power = highest_power, .num_terms = factorial(highest_power + num_vars)/(factorial(highest_power)*factorial(num_vars))};
    output.values = calloc(output.num_terms, sizeof(double));
    output.teaching_values = calloc(output.num_terms, sizeof(double));
    return output;
}

double evaluate_(double **values, double **teaching_values, double product, double *inputs, unsigned int num_vars, unsigned int power){
    double output = **values;
	**teaching_values = product;
    if(power == 0){
        return output;
    }
    unsigned int i;
    for(i = 0; i < num_vars; i++){
        *values += 1;
		*teaching_values += 1;
        output += inputs[i]*evaluate_(values, teaching_values, product*inputs[i], inputs + i, num_vars - i, power - 1);
    }
    return output;
}

double evaluate(polynomial poly, double *inputs){
    double *values;
	double *teaching_values;
    values = poly.values;
	teaching_values = poly.teaching_values;
    return evaluate_(&values, &teaching_values, 1, inputs, poly.num_vars, poly.highest_power);
}

double teach(polynomial poly, double *inputs, double expected, double c){
	double error = expected - evaluate(poly, inputs);
	
	unsigned int i;
	for(i = 0; i < poly.num_terms; i++){
		poly.values[i] += poly.teaching_values[i]*error*c;
	}
	
	return error;
}

double random(){
	return ((double) rand())/RAND_MAX;
}

int main(){
	srand(time(NULL));
    polynomial poly;
    poly = create_polynomial(2, 3);
	double inputs[2];
	double expected;
	double error;
    unsigned int i;
	unsigned int j;
	
	for(i = 0; i < 100000; i++){
		inputs[0] = random();
		inputs[1] = random();
		expected = inputs[0]/(inputs[1] + 1);
		for(j = 0; j < poly.num_terms; j++){
			printf("%f ", poly.values[j]);
		}
		printf("\n");
		error = teach(poly, inputs, expected, 0.05);
		printf("epoch: %d, error: %f\n", i + 1, error);
	}
}