// Zadanie nr 3

#include "math.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>

// Save polynomial representation to file.
void fprint_polynomial(FILE* log_file, int *polynomial_coefficients,
                       int polynomial_degree) {
  bool is_first_word = true;
  for (int i = 0; i <= polynomial_degree; i++) {
    int coefficient = polynomial_coefficients[i];
    // Skip word if coefficient is zero.
    if (coefficient == 0)
      continue;
    // Print sign if appropriate.
    if (coefficient < 0)
      fprintf(log_file, "- ");
    else if (!is_first_word)
      fprintf(log_file, "+ ");

    is_first_word = false;
    int power = polynomial_degree - i;

    if (power == 0) {
      fprintf(log_file, "%i", abs(coefficient));
      continue;
    }

    // Print coefficient if appropriate.
    if (abs(coefficient) != 1)
      fprintf(log_file, "%i*", abs(coefficient));

    // Print x.
    fprintf(log_file, "x");

    // Print power if appropriate.
    if (power > 1)
      fprintf(log_file, "^%i", power);

    // Print a trailling space.
    fprintf(log_file, " ");
  }
}

// Represents an integer array coupled with its size.
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

// Save a string repre sntation of an array to file.
void fprint_array(FILE *log_file, struct Array *arr) {
  fprintf(log_file, "[ ");
  for (int i = 0; i != arr->size; i++) {
    fprintf(log_file, "%i, ", arr->values[i]);
  }
  fprintf(log_file, "]");
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

// Whether element with value is already present in array.
bool contains(double *array, int array_size, double value) {
  for (int i = 0; i < array_size; i++)
    if (array[i] == value)
      return true;
  return false;
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

      // If this value was already found, pass.
      if (contains(roots, *root_count, root_candidate) ||
          contains(roots, *root_count, -root_candidate))
        continue;

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
  FILE *log_file = fopen("lab3/last_run_log.txt", "w");

  // Polynomial degree must be at most 10.
  const int MAX_POLYNOMIAL_DEGREE_ACCEPTED = 10;
  // Allocate for a 10th degree polynomial at max.
  int *polynomial_coefficients =
      (int *)malloc((MAX_POLYNOMIAL_DEGREE_ACCEPTED + 1) * sizeof(int));
  // Start at -1 for incrementing.
  int polynomial_degree = -1;

  // Read input.
  while (scanf("%i", &polynomial_coefficients[polynomial_degree + 1]) != EOF) {
    if (++polynomial_degree > MAX_POLYNOMIAL_DEGREE_ACCEPTED) {
      printf ("Polynomials up to %ith degree accepted. Degree of polynomial "
             "provided: %i.",
             MAX_POLYNOMIAL_DEGREE_ACCEPTED, polynomial_degree);
      return 1;
    }
  }

  // Print polynomial information.
  fprintf(log_file, "Polynomial degree: %i\n", polynomial_degree);
  fprintf(log_file, "Input polynomial: ");
  fprint_polynomial(log_file, polynomial_coefficients, polynomial_degree);
  fprintf(log_file, "\n");

  // The free term must be non-zero.
  if (polynomial_coefficients[polynomial_degree] == 0) {
    fprintf(log_file, "The free term of the polynomial cannot be zero.");
    return 2;
  }

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
  fprintf(log_file, "Non-zero coefficient for lowest power: %i. Divisors: p = ",
          lowest_power_coeff);
  struct Array p = get_divisors(lowest_power_coeff);
  fprint_array(log_file, &p);
  fprintf(log_file,
          "\n");

  fprintf(log_file,
          "Non-zero coefficient for highest power: %i. Divisors: q = ",
          highest_power_coeff);
  struct Array q = get_divisors(highest_power_coeff);
  fprint_array(log_file, &q);
  fprintf(log_file, "\n");

  // Find roots.
  int root_count;
  double *roots = get_roots(polynomial_coefficients, polynomial_degree, &p, &q,
                            &root_count);
  fprintf(log_file, "Found %i roots: [", root_count);
  for (int i = 0; i < root_count; i++) {
    fprintf(log_file, "%.3f, ", roots[i]);
    printf("%.3f ", roots[i]);
  }
  fprintf(log_file, "]");

  // Free the dynamically allocated memory.
  free(polynomial_coefficients);
  free(p.values);
  free(q.values);

  fclose(log_file);

  return 0;
}
