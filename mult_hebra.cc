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

// Función para multiplicar dos matrices (paralelizada)
template <typename T>
Matriz<T> multiplicarMatricesParalelo(const Matriz<T>& a, const Matriz<T>& b, size_t filasA, size_t columnasA, size_t columnasB) {
    Matriz<T> resultado;
    resultado.datos = new T*[filasA];

    #pragma omp parallel for
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

// Función para multiplicar elemento por elemento (paralelizada)
template <typename T, typename U>
Matriz<T> multiplicar_elemento_elemento_paralelo(const Matriz<T>& a, const Matriz<U>& b, size_t filas, size_t columnas) {
    Matriz<T> resultado;
    resultado.datos = new T*[filas];

    #pragma omp parallel for
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
        clock_t start_secuencial, end_secuencial;

        // Crear matrices y realizar operaciones
        start_secuencial = clock();

        // Ejemplo de uso de matrices en el heap
        size_t filas = i;
        size_t columnas = i;

        Matriz<etype_t_2> A = crearMatrizAleatoria<etype_t_2>(filas, columnas);
        Matriz<etype_t_2> B = crearMatrizAleatoria<etype_t_2>(filas, columnas);

        Matriz<etype_t_2> C = multiplicarMatrices<etype_t_2>(A, B, filas, columnas, columnas);

        Matriz<etype_t_1> K = crearMatrizAleatoria<etype_t_1>(filas, columnas);

        Matriz<etype_t_2> R = multiplicar_elemento_elemento<etype_t_2, etype_t_1>(C, K, filas, columnas);

        // Finalizar medición del tiempo
        end_secuencial = clock();

        // Imprimir el tiempo de ejecución en segundos
        std::cout << "Tiempo de ejecución: " << static_cast<etype_t_2>(end_secuencial - start_secuencial) / CLOCKS_PER_SEC << " segundos." << std::endl;

        // Liberar memoria de las matrices en el heap
        liberarMatriz(A, filas);
        liberarMatriz(B, filas);
        liberarMatriz(C, filas);
        liberarMatriz(K, filas);
        liberarMatriz(R, filas);

        // Medir el tiempo de ejecución para la versión paralela (monohebra)
        clock_t start_monohebra, end_monohebra;
        start_monohebra = clock();

        Matriz<double> A_paralelo_monohebra = crearMatrizAleatoria<double>(filas, columnas);
        Matriz<double> B_paralelo_monohebra = crearMatrizAleatoria<double>(filas, columnas);

        Matriz<double> C_paralelo_monohebra = multiplicarMatricesParalelo<double>(A_paralelo_monohebra, B_paralelo_monohebra, filas, columnas, columnas);

        Matriz<int> K_paralelo_monohebra = crearMatrizAleatoria<int>(filas, columnas);

        Matriz<double> R_paralelo_monohebra = multiplicar_elemento_elemento_paralelo<double, int>(C_paralelo_monohebra, K_paralelo_monohebra, filas, columnas);

        // Finalizar medición del tiempo para la versión paralela (monohebra)
        end_monohebra = clock();

        // Imprimir el tiempo de ejecución en segundos para la versión paralela (monohebra)
        std::cout << "Tiempo de ejecución (paralelo, monohebra): " << static_cast<double>(end_monohebra - start_monohebra) / CLOCKS_PER_SEC << " segundos." << std::endl;

        // Liberar memoria de las matrices en el heap
        liberarMatriz(A_paralelo_monohebra, filas);
        liberarMatriz(B_paralelo_monohebra, filas);
        liberarMatriz(C_paralelo_monohebra, filas);
        liberarMatriz(K_paralelo_monohebra, filas);
        liberarMatriz(R_paralelo_monohebra, filas);

        // Medir el tiempo de ejecución para la versión paralela (dos o más hebras)
        clock_t start_multiples_hebras, end_multiples_hebras;
        start_multiples_hebras = clock();

        Matriz<double> A_paralelo_multiples_hebras = crearMatrizAleatoria<double>(filas, columnas);
        Matriz<double> B_paralelo_multiples_hebras = crearMatrizAleatoria<double>(filas, columnas);

        Matriz<double> C_paralelo_multiples_hebras = multiplicarMatricesParalelo<double>(A_paralelo_multiples_hebras, B_paralelo_multiples_hebras, filas, columnas, columnas);

        Matriz<int> K_paralelo_multiples_hebras = crearMatrizAleatoria<int>(filas, columnas);

        Matriz<double> R_paralelo_multiples_hebras = multiplicar_elemento_elemento_paralelo<double, int>(C_paralelo_multiples_hebras, K_paralelo_multiples_hebras, filas, columnas);

        // Finalizar medición del tiempo para la versión paralela (dos o más hebras)
        end_multiples_hebras = clock();

        // Imprimir el tiempo de ejecución en segundos para la versión paralela (dos o más hebras)
        std::cout << "Tiempo de ejecución (paralelo, dos o más hebras): " << static_cast<double>(end_multiples_hebras - start_multiples_hebras) / CLOCKS_PER_SEC << " segundos." << std::endl;

        // Liberar memoria de las matrices en el heap
        liberarMatriz(A_paralelo_multiples_hebras, filas);
        liberarMatriz(B_paralelo_multiples_hebras, filas);
        liberarMatriz(C_paralelo_multiples_hebras, filas);
        liberarMatriz(K_paralelo_multiples_hebras, filas);
        liberarMatriz(R_paralelo_multiples_hebras, filas);

        // Calcular ganancias en velocidad (speedup)
        double speedup_monohebra = static_cast<double>(end_secuencial - start_secuencial) / (end_monohebra - start_monohebra);
        double speedup_multiples_hebras = static_cast<double>(end_secuencial - start_secuencial) / (end_multiples_hebras - start_multiples_hebras);

        // Imprimir ganancias en velocidad
        std::cout << "Ganancia en velocidad (speedup) para monohebra: " << speedup_monohebra << std::endl;
        std::cout << "Ganancia en velocidad (speedup) para dos o más hebras: " << speedup_multiples_hebras << std::endl;
    }

    return 0;
}
