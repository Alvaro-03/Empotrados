#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using etype_t_2 = double;
using etype_t_1 = int;

// Función para crear una matriz y llenarla con valores aleatorios entre 0 y 1
template <typename T>
T** crearMatrizAleatoria(int filas, int columnas) {
    T** matriz = new T*[filas];
    for (int i = 0; i < filas; ++i) {
        matriz[i] = new T[columnas];
        for (int j = 0; j < columnas; ++j) {
            matriz[i][j] = static_cast<T>(std::rand()) / RAND_MAX;
        }
    }

    return matriz;
}

// Función para crear una matriz y llenarla con valores aleatorios entre 0 y 1
template <typename T>
T** crearIntMatrizAleatoria(int filas, int columnas) {
    T** matriz = new T*[filas];
    for (int i = 0; i < filas; ++i) {
        matriz[i] = new T[columnas];
        for (int j = 0; j < columnas; ++j) {
            matriz[i][j] = static_cast<T>(std::rand()) % 256;
        }
    }

    return matriz;
}

// Función para multiplicar dos matrices
template <typename T>
void multiplicarMatrices(T** c, T** a, T** b, int filasA, int columnasA, int columnasB) {
    for (int i = 0; i < filasA; ++i) {
        for (int j = 0; j < columnasB; ++j) {
            c[i][j] = 0.0;
            for (int k = 0; k < columnasA; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Función para multiplicar dos matrices (paralelizada)
template <typename T>
void multiplicarMatricesParalelo(T** c, T** a, T** b, int filasA, int columnasA, int columnasB, int hebras) {
    #pragma omp parallel for num_threads(hebras)
    for (int i = 0; i < filasA; ++i) {
        for (int j = 0; j < columnasB; ++j) {
            c[i][j] = 0.0;
            for (int k = 0; k < columnasA; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Función para multiplicar elemento por elemento
template <typename T, typename U>
void multiplicar_elemento_elemento(T** r, T** a, U** b, int filas, int columnas) {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            r[i][j] = a[i][j] * static_cast<T>(b[i][j]);
        }
    }
}

// Función para multiplicar elemento por elemento (paralelizada)
template <typename T, typename U>
void multiplicar_elemento_elemento_paralelo(T** r, T** a, U** b, int filas, int columnas, int hebras) {
    #pragma omp parallel for num_threads(hebras)
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            r[i][j] = a[i][j] * static_cast<T>(b[i][j]);
        }
    }
}

// Función para liberar la memoria de una matriz
template <typename T>
void liberarMatriz(T** matriz, int filas) {
    for (int i = 0; i < filas; ++i) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

// Función para transponer una matriz
template <typename T>
void transponerMatriz(T** m1, T** m2, int filas, int columnas) {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            m1[j][i] = m2[i][j];
        }
    }

}

int main() {
    std::srand(std::time(0));

    for (int i = 100; i < 2000; i += 100) {
        int filas = i;
        int columnas = i;

        etype_t_2** A = crearMatrizAleatoria<etype_t_2>(filas, columnas);
        etype_t_2** B = crearMatrizAleatoria<etype_t_2>(filas, columnas);
        etype_t_1** K = crearIntMatrizAleatoria<etype_t_1>(filas, columnas);
        etype_t_2** C = crearMatrizAleatoria<etype_t_2>(filas, columnas);
        etype_t_2** R = crearMatrizAleatoria<etype_t_2>(filas, columnas);
        
        double start_secuencial, end_secuencial;
        start_secuencial = omp_get_wtime();
        multiplicarMatrices<etype_t_2>(C, A, B, filas, columnas, columnas);
        multiplicar_elemento_elemento<etype_t_2, etype_t_1>(R, C, K, filas, columnas);
        end_secuencial = omp_get_wtime();

        std::cout << "Tamaño de la matriz: " << filas << std::endl;
        std::cout << "Tiempo de ejecución: " << static_cast<etype_t_2>(end_secuencial - start_secuencial) << " segundos." << std::endl;

        for (int num_threads = 1; num_threads <= 2; num_threads *= 2) {
            double start_Xhebra, end_Xhebra;
            
            // Versión original sin transposición de B
            start_Xhebra = omp_get_wtime();
            multiplicarMatricesParalelo<etype_t_2>(C, A, B, filas, columnas, columnas, num_threads);
            multiplicar_elemento_elemento_paralelo<etype_t_2, etype_t_1>(R, C, K, filas, columnas, num_threads);
            end_Xhebra = omp_get_wtime();
            
            std::cout << "Tiempo de ejecución (paralelo, " << num_threads << "): " << static_cast<double>(end_Xhebra - start_Xhebra) << " segundos." << std::endl;

            // Versión con transposición de B
            transponerMatriz<etype_t_2>(B,A,filas, columnas);

            start_Xhebra = omp_get_wtime();
            multiplicarMatricesParalelo<etype_t_2>(C, A, B, filas, columnas, filas, num_threads);
            multiplicar_elemento_elemento_paralelo<etype_t_2, etype_t_1>(R, C, K, filas, columnas, num_threads);
            end_Xhebra = omp_get_wtime();
            
            std::cout << "Tiempo de ejecución (paralelo con transposición, " << num_threads << "): " << static_cast<double>(end_Xhebra - start_Xhebra) << " segundos." << std::endl;
            
            // Calcular y mostrar ganancias en velocidad (speedup) y eficiencia
            double speedup_multiples_hebras = static_cast<double>(end_secuencial - start_secuencial) / (end_Xhebra - start_Xhebra);
            double eficiencia = speedup_multiples_hebras / num_threads;

            std::cout << "Ganancia en velocidad (speedup) para " << num_threads << " hebras: " << speedup_multiples_hebras << std::endl;
            std::cout << "Eficiencia para " << num_threads << " hebras: " << eficiencia << std::endl;
        }

        liberarMatriz<etype_t_2>(A, filas);
        liberarMatriz<etype_t_2>(B, filas);
        liberarMatriz<etype_t_2>(C, filas);
        liberarMatriz<etype_t_1>(K, filas);
        liberarMatriz<etype_t_2>(R, filas);
    }

    return 0;
}