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


using etype_t_2 = float;
using etype_t_1 = char;

template <typename T>
struct Matriz {
    T** datos;
};

// Función para crear una matriz y llenarla con valores aleatorios entre 0 y 1
template <typename T>
Matriz<T> crearMatrizAleatoria(size_t filas, size_t columnas) {
    // Inicializar la semilla para la generación de números aleatorios
    std::srand(static_cast<unsigned etype_t_1>(std::time(nullptr)));

    // Crear la matriz en el heap
    Matriz<T> matriz;
    matriz.datos = new T*[filas];
    for (size_t i = 0; i < filas; ++i) {
        matriz.datos[i] = new T[columnas];
        for (size_t j = 0; j < columnas; ++j) {
            matriz.datos[i][j] = static_cast<T>(std::rand()) / RAND_MAX;
        }
    }

    return matriz;
}

// Función para multiplicar dos matrices
template <typename T>
Matriz<T> multiplicarMatrices(const Matriz<T>& a, const Matriz<T>& b, size_t filasA, size_t columnasA, size_t columnasB) {
    Matriz<T> resultado;
    resultado.datos = new T*[filasA];
    for (size_t i = 0; i < filasA; ++i) {
        resultado.datos[i] = new T[columnasB];
        for (size_t j = 0; j < columnasB; ++j) {
            resultado.datos[i][j] = 0.0;
            for (size_t k = 0; k < columnasA; ++k) {
                resultado.datos[i][j] += a.datos[i][k] * b.datos[k][j];
            }
        }
    }

    return resultado;
}

// Función para multiplicar elemento por elemento
template <typename T, typename U>
Matriz<T> multiplicar_elemento_elemento(const Matriz<T>& a, const Matriz<U>& b, size_t filas, size_t columnas) {
    Matriz<T> resultado;
    resultado.datos = new T*[filas];
    for (size_t i = 0; i < filas; ++i) {
        resultado.datos[i] = new T[columnas];
        for (size_t j = 0; j < columnas; ++j) {
            resultado.datos[i][j] = a.datos[i][j] * static_cast<T>(b.datos[i][j]);
        }
    }

    return resultado;
}

// Función para liberar la memoria de una matriz
template <typename T>
void liberarMatriz(Matriz<T>& matriz, size_t filas) {
    for (size_t i = 0; i < filas; ++i) {
        delete[] matriz.datos[i];
    }
    delete[] matriz.datos;
}

int main() {

    for (int i = 100; i < 1500; i += 100){
        // Medir el tiempo de ejecución
        clock_t start, end;

        // Crear matrices y realizar operaciones
        start = clock();

        // Ejemplo de uso de matrices en el heap
        size_t filas = i;
        size_t columnas = i;

        Matriz<etype_t_2> A = crearMatrizAleatoria<etype_t_2>(filas, columnas);
        Matriz<etype_t_2> B = crearMatrizAleatoria<etype_t_2>(filas, columnas);

        Matriz<etype_t_2> C = multiplicarMatrices<etype_t_2>(A, B, filas, columnas, columnas);

        Matriz<etype_t_1> K = crearMatrizAleatoria<etype_t_1>(filas, columnas);

        Matriz<etype_t_2> R = multiplicar_elemento_elemento<etype_t_2, etype_t_1>(C, K, filas, columnas);

        // Finalizar medición del tiempo
        end = clock();

        // Imprimir el tiempo de ejecución en segundos
        std::cout << "Tiempo de ejecución: " << static_cast<etype_t_2>(end - start) / CLOCKS_PER_SEC << " segundos." << std::endl;

        // Liberar memoria de las matrices en el heap
        liberarMatriz(A, filas);
        liberarMatriz(B, filas);
        liberarMatriz(C, filas);
        liberarMatriz(K, filas);
        liberarMatriz(R, filas);
    }

    return 0;
}

/*

size_t filas = 3;
size_t columnas = 3;

// Medir el tiempo de ejecución para la versión secuencial
clock_t start_secuencial, end_secuencial;
start_secuencial = clock();

Matriz<double> A = crearMatrizAleatoria<double>(filas, columnas);
Matriz<double> B = crearMatrizAleatoria<double>(filas, columnas);

Matriz<double> C_secuencial = multiplicarMatrices<double>(A, B, filas, columnas, columnas);

Matriz<int> K = crearMatrizAleatoria<int>(filas, columnas);

Matriz<double> R_secuencial = multiplicar_elemento_elemento<double, int>(C_secuencial, K, filas, columnas);

// Finalizar medición del tiempo para la versión secuencial
end_secuencial = clock();

// Imprimir el tiempo de ejecución en segundos para la versión secuencial
std::cout << "Tiempo de ejecución (secuencial): " << static_cast<double>(end_secuencial - start_secuencial) / CLOCKS_PER_SEC << " segundos." << std::endl;

// Liberar memoria de las matrices en el heap
liberarMatriz(A, filas);
liberarMatriz(B, filas);
liberarMatriz(C_secuencial, filas);
liberarMatriz(K, filas);
liberarMatriz(R_secuencial, filas);


*/