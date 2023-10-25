// Zadanie nr 2

#include "math.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>

// The parameters of the program should all be integer coefficients of the
// polynomial.
// Returned as a dynamically allocated array of ints.
int *parse_args(int argc, char *argv[]) {
  int *coefficients = (int *)malloc((argc - 1) * sizeof(int));

  // Start from i=1 since the first arg is always the name of the program.
  for (int i = 1; i < argc; i++) {
    coefficients[i - 1] = atoi(argv[i]);
  }

  return coefficients;
}

// Print polynomial representation.
void print_polynomial(int *polynomial_coefficients, int polynomial_degree) {
  bool is_first_word = true;
  for (int i = 0; i <= polynomial_degree; i++) {
    int coefficient = polynomial_coefficients[i];
    // Skip word if coefficient is zero.
    if (coefficient == 0)
      continue;
    // Print sign if appropriate.
    if (coefficient < 0)
      printf("- ");
    else if (!is_first_word)
      printf("+ ");

    is_first_word = false;
    int power = polynomial_degree - i;

    if (power == 0) {
      printf("%i", abs(coefficient));
      continue;
    }

    // Print coefficient if appropriate.
    if (abs(coefficient) != 1)
      printf("%i*", abs(coefficient));

    // Print x.
    printf("x");

    // Print power if appropriate.
    if (power > 1)
      printf("^%i", power);

    // Print a trailling space.
    printf(" ");
  }
}

struct Array {
  int size;
  int *values;
};

// Return all the divisors of the given number
struct Array get_divisors(int number) {
  // Work on absoulute values.
  number = abs(number);

  // The number can't have more divisors than this.
  int *divisors = (int *)malloc(number * sizeof(int));
  int count = 0;

  for (int i = 1; i <= number; i++) {
    if (number % i == 0) {
      divisors[count++] = i;
    }
  }

  struct Array array;
  array.values = divisors;
  array.size = count;

  return array;
}

// Print a string representation of an array.
void print_array(struct Array *arr) {
  printf("[ ");
  for (int i = 0; i != arr->size; i++) {
    printf("%i, ", arr->values[i]);
  }
  printf("]");
}

// Get result of subsituting for the variable with value in a given polynomial.
int substitute(int *polynomial_coefficients, int polynomial_degree,
               double value) {
  int result = 0;
  for (int i = 0; i <= polynomial_degree; i++) {
    int power = polynomial_degree - i;
    result += polynomial_coefficients[i] * pow(value, power);
  }
  return result;
}

// Returns the roots of the polynomial based on Bezoute's theorem.
double *get_roots(int *polynomial_coefficients, int polynomial_degree,
                  struct Array *p, struct Array *q, int *root_count) {
  // Polynomial can't have more roots than its degree.
  double *roots = (double *)malloc(polynomial_degree * sizeof(double));
  *root_count = 0;

  for (int p_i = 0; p_i != p->size; p_i++) {
    for (int q_i = 0; q_i != q->size; q_i++) {
      double root_candidate = (double)p->values[p_i] / q->values[q_i];

      if (substitute(polynomial_coefficients, polynomial_degree,
                     root_candidate) == 0) {
        roots[(*root_count)++] = root_candidate;
      }

      if (substitute(polynomial_coefficients, polynomial_degree,
                     -root_candidate) == 0)
        roots[(*root_count)++] = -root_candidate;
    }
  }

  return roots;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Provide the polynomial coefficients.\nUsage: '%s <a_n> <a_n-1> .. "
           "<a_0>'",
           argv[0]);
    return 1;
  }
  int polynomial_degree = argc - 2;
  int *polynomial_coefficients = parse_args(argc, argv);

  // Print polynomial information.
  printf("Polynomial degree: %i\n", polynomial_degree);
  printf("Input polynomial: ");
  print_polynomial(polynomial_coefficients, polynomial_degree);
  printf("\n");

  // Find non-zero coefficient for highest power and lowest-power.
  int highest_power_coeff = INT_MAX, lowest_power_coeff;
  for (int i = 0; i <= polynomial_degree; i++) {
    if (polynomial_coefficients[i] != 0) {
      if (highest_power_coeff == INT_MAX)
        highest_power_coeff = polynomial_coefficients[i];
      lowest_power_coeff = polynomial_coefficients[i];
    }
  }

  // Calculate divisors.
  printf("Non-zero coefficient for lowest power: %i. Divisors: p = ",
         lowest_power_coeff);
  struct Array p = get_divisors(lowest_power_coeff);
  print_array(&p);
  printf("\n");

  printf("Non-zero coefficient for highest power: %i. Divisors: q = ",
         highest_power_coeff);
  struct Array q = get_divisors(highest_power_coeff);
  print_array(&q);
  printf("\n");

  // Find roots.
  int root_count;
  double *roots = get_roots(polynomial_coefficients, polynomial_degree, &p, &q,
                            &root_count);
  printf("Found roots: [");
  for (int i = 0; i < root_count; i++) {
    printf("%f, ", roots[i]);
  }
  printf("]");

  // Free the dynamically allocated memory.
  free(polynomial_coefficients);
  free(p.values);
  free(q.values);

  return 0;
}
