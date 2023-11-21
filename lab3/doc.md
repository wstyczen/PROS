_Wojciech Styczeń, 303776_

# Zadanie 3

> Kompilacja na _Ubuntu 18.04 LTS_, przy użyciu _gcc 7.5.0_.
>
> Kod napisany w VSCode, sformatowany przy użyciu _clang-format_ w stylu _Chromium_.
>
> [GitHub](https://github.com/wstyczen/PROS.git)

## Program

Program bazuje w znacznej części na implementacji z zadania 2.

Aby uniknąć niepotrzebnego powtarzania nie będą skopiowane dane z poprzedniego sprawozdania (np wykazy zmiennych i funkcji). Wszystkie zmiany na potrzeby tego zadania będą opisane poniżej.

### 1. W programie nie było zmiennych globalnych. Tablice, będące współczynnikami wielomianu oraz obliczeniami pośrednimi należy dynamicznie alokować, a następnie wskaźnik do tej tablicy przekazywać do innych funkcji. Należy pamiętać o zwolnieniu pamięci przed zakończeniem programu.

Program z lab. 2 spełniał te wymogi już w poprzedniej wersji.

- Tablice były alokowane dynamicznie, np:

```c
// Alokacja tablicy ze współczynnikami wielomianu.
int *coefficients = (int *)malloc((argc - 1) * sizeof(int));

// Alokacja tablic na dzielniki wybranych współczynników wielomianu.
int *divisors = (int *)malloc(number * sizeof(int));

// Alokacja pamięci na tablicę z pierwiastkami wielomianu.
double *roots = (double *)malloc(polynomial_degree * sizeof(double));
```

- Tablice były zwracane i przekazywane do funkcji jako wskaźnik, brak zmiennych globalnych.

```c
// Przykład przekazania tablicy do funkcji.
int substitute(int *polynomial_coefficients, int polynomial_degree,
               double value);
```

- Tablice są zwalniane na końcu funkcji `main`:

```c
free(polynomial_coefficients);
free(p.values);
free(q.values);
```

### 2. Program na generować raport do pliku tekstowego z przebiegu obliczeń. Do pliku zapisać np. dzielniki współczynników. Na początku programu należy otworzyć zapis do pliku, na końcu programu go zamknąć.

W funkcji `main`:

- na początku funkcji otwierany jest plik do zapisu:

```c
  FILE* log_file = fopen("lab3/last_run_log.txt", "w");
```

> Ścieżka zakłada moją konfigurację folderów, może wymagać zmiany.

- na końcu funkcji plik jest zamykany:

```c
  fclose(log_file);
```

Aby zamiast drukować na konsolę zapisać logi do pliku wszystkie wywołania `printf` zostały zastępione przez `fprintf` np:

```c
fprintf(log_file, "Polynomial degree: %i\n", polynomial_degree);
```

Dzięki temu zamiast na `stdout` logi z poprzedniego działania programu zostaną zapisane do pliku o wybranej ścieżce. Przykładowy rezultat:

```txt
Polynomial degree: 3
Input polynomial: x^3 - 29*x^2 - 100*x + 2900
Non-zero coefficient for lowest power: 2900. Divisors: p = [ 1, 2, 4, 5, 10, 20, 25, 29, 50, 58, 100, 116, 145, 290, 580, 725, 1450, 2900, ]
Non-zero coefficient for highest power: 1. Divisors: q = [ 1, ]
Found 3 roots: [10.000, -10.000, 29.000, ]
```

### 3. Uruchomić program w postaci potoku. Przygotować plik z danymi wejściowymi, wynik obliczeń przekierować do pliku (patrz slajdy wykładowe).

- Aby '_uruchomić program w postaci potoku_' można to zrobić za pomocą komendy w postaci:

```sh
ścieżka_do_programu < ścieżka_pliku_wejściowego > ścieżka_pliku_wyjściowego
```

- Fragment kodu dotyczący odczytywania argumentów funkcji został zastępiony szczytywaniem ze `stdin`:

```c
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
```

- Przez użycie potoku(_> plik_), `stdout` będzie przekierowany do danego pliku (np dla `printf`).

## Działanie programu w kontekście modyfikacji

- Uruchomienie programu jako potok za pomocą `cmd` (może wymagać zmiany ścieżek):

```sh
./build/lab3.exe < lab3/input.txt > lab3/output.txt
```

- Przy pliku wejściowym _input.txt_ w postaci:

```txt
1 -29 -100 2900
```

- Wynik zostanie zapisany do pliku `output.txt`:

```txt
10.000 -10.000 29.000
```

- Do osobnego pliku będą zapisane logi z działania programu (domyślnie _last_run_log.txt_):

```txt
Polynomial degree: 3
Input polynomial: x^3 - 29*x^2 - 100*x + 2900
Non-zero coefficient for lowest power: 2900. Divisors: p = [ 1, 2, 4, 5, 10, 20, 25, 29, 50, 58, 100, 116, 145, 290, 580, 725, 1450, 2900, ]
Non-zero coefficient for highest power: 1. Divisors: q = [ 1, ]
Found 3 roots: [10.000, -10.000, 29.000, ]
```
