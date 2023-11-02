# Sprawozdanie lab 2

## Wykorzystane techonologie

> Kompilacja na _Ubuntu 18.04 LTS_, przy użyciu _gcc 7.5.0_.
>
> Kod napisany i uruchamiany w VSCode, sformatowany przy użyciu _clang-format_ w stylu _Chromium_.
>
> [GitHub](https://github.com/wstyczen/PROS.git)

## Program

### Wykaz istotnych zmiennnych

#### Stopień i współczynniki wielomianu wczytane z argumentów funkcji main

```c
  int polynomial_degree;
  int *polynomial_coefficients;
```

#### Wektory p i q - dzielniki współczynników wielomianu przy najwyższej i najniższej potędze

```c
  struct Array p, q;
```

#### Wektor i ilość znalezionych zer wielomianu

```c
  int root_count;
  double *roots
```

### Wykaz istotnych funkcji

```c
// The parameters of the program should all be integer coefficients of the
// polynomial.
// Returned as a dynamically allocated array of ints.
int *parse_args(int argc, char *argv[]);

// Print polynomial representation.
void print_polynomial(int *polynomial_coefficients, int polynomial_degree);

// Return all the divisors of the given number
struct Array get_divisors(int number);

// Get result of subsituting for the variable with value in a given polynomial.
int substitute(int *polynomial_coefficients, int polynomial_degree,
               double value);

// Returns the roots of the polynomial based on Bezoute's theorem.
double *get_roots(int *polynomial_coefficients, int polynomial_degree,
                  struct Array *p, struct Array *q, int *root_count);
```

### Algorytm działania programu

1. Sparsowanie argumentów funkcji main do tablicy współczynników wielomianu.
2. Wyświetlenie sparsowanych danych - stopnia i postaci wielomianu.
3. Sprawdzenie czy zadane argumenty spełniają warunki opisane w treści zadania (wyraz wolny niezerowy, współczynnik wielomianu co najwyżej 10, etc). Jeśli którykolwiek z warunków zadania jest niespełniony następuje przerwanie programu.
4. Znalezienie niezerowego współczynnika przy najwyższej i najniższej potędze w wielomianie.
5. Wyznaczenie i wyświetlenie dzielników znalezionych współczynników (wektory p i q).
6. Sprawdzenie poprzez podstawienie, które z wyników dzielenia elementów p przez elementy q są zerami wielomianu. Wyświetlenie wyniku.
