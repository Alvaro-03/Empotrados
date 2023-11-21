// -------------------------------------------- ESTÁTICA -------------------------------------------
/*
#include <iostream>
#include <iostream>
#include <cstdlib> // Necesario para la función rand()
#include <ctime>   // Necesario para la función time()

// Definición de la estructura Matriz
template <typename T, size_t Filas, size_t Columnas>
struct Matriz {
    T datos[Filas][Columnas];
};


// Función para llenar la matriz con valores aleatorios entre 0 y 1
template <size_t Filas, size_t Columnas>
void llenarMatrizAleatoria(Matriz<double, Filas, Columnas>& matriz) {
    // Inicializar la semilla para la generación de números aleatorios
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Llenar la matriz con valores aleatorios entre 0 y 1
    for (size_t i = 0; i < Filas; ++i) {
        for (size_t j = 0; j < Columnas; ++j) {
            matriz.datos[i][j] = static_cast<double>(std::rand()) / RAND_MAX;
        }
    }
}


// Función para multiplicar dos matrices
template <size_t FilasA, size_t ColumnasA, size_t ColumnasB>
Matriz<double, FilasA, ColumnasB> multiplicarMatrices(const Matriz<double, FilasA, ColumnasA>& a, const Matriz<double, ColumnasA, ColumnasB>& b) {
    Matriz<double, FilasA, ColumnasB> resultado;

    for (size_t i = 0; i < FilasA; ++i) {
        for (size_t j = 0; j < ColumnasB; ++j) {
            resultado.datos[i][j] = 0.0;
            for (size_t k = 0; k < ColumnasA; ++k) {
                resultado.datos[i][j] += a.datos[i][k] * b.datos[k][j];
            }
        }
    }

    return resultado;
}



// Función para multiplicar dos matrices
template <size_t FilasA, size_t ColumnasA, size_t ColumnasB>
Matriz<double, FilasA, ColumnasB> multiplicar_elemento_elemento(const Matriz<double, FilasA, ColumnasA>& a, const Matriz<int, ColumnasA, ColumnasB>& b) {
    Matriz<double, FilasA, ColumnasB> resultado;

    for (size_t i = 0; i < FilasA; ++i) {
        for (size_t j = 0; j < ColumnasB; ++j) {
            resultado.datos[i][j] = a.datos[i][j] * a.datos[i][j];
        }
    }

    return resultado;
}


int main() {
    // Ejemplo de uso de la matriz de enteros de 3x3
    Matriz<double, 3, 3> A;
    Matriz<double, 3, 3> B;

    // Llenar la matriz con valores aleatorios entre 0 y 1
    llenarMatrizAleatoria(A);
    llenarMatrizAleatoria(B);

    Matriz<double, 3, 3> C = multiplicarMatrices(A, B);

    Matriz<int, 3, 3> K;

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            K.datos[i][j] = std::rand() % 256; // Restricción al rango [0, 255]
        }
    }


    Matriz<double, 3, 3> R = multiplicar_elemento_elemento(C, K);

    return 0;
}




-------------------------------------------------DINÁMICA------------------------------------------
*/
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

// Función para multiplicar elemento por elemento
template <typename T, typename U>
void multiplicar_elemento_elemento(T** r, T** a, U** b, int filas, int columnas) {
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

int main() {

    std::srand(std::time(0));
    for (int i = 100; i < 1500; i += 100){

        // Ejemplo de uso de matrices en el heap
        int filas = i;
        int columnas = i;

        // Crear matrices
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

        // Imprimir el tiempo de ejecución en segundos
        std::cout << "Tiempo de ejecución: " << static_cast<etype_t_2>(end_secuencial - start_secuencial) << " segundos." << std::endl;

        // Liberar memoria de las matrices en el heap
        liberarMatriz<etype_t_2>(A, filas);
        liberarMatriz<etype_t_2>(B, filas);
        liberarMatriz<etype_t_2>(C, filas);
        liberarMatriz<etype_t_1>(K, filas);
        liberarMatriz<etype_t_2>(R, filas);
    }

    return 0;
}
