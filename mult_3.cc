#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

int MATRIX_SIZE = 0; // Tamaño de las matrices cuadradas

void multiplyMatrixElementwiseSequential(double** matrix1, int** matrix2, double** result) {
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            result[i][j] *= matrix2[i][j];
        }
    }
}
void multiplyMatrixElementwiseParallel(double** matrix1, int** matrix2, double** result, int num_threads) {
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            result[i][j] *= matrix2[i][j];
        }
    }
}
// Función para inicializar una matriz con valores aleatorios entre 0 y 1
void initializeMatrix(double** matrix) {

    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            matrix[i][j] = ((double) rand() / RAND_MAX);
        }
    }
}
void initializeIntMatrix(int** matrix) {
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            matrix[i][j] = rand() % 256;
        }
    }
}

// Función para imprimir una matriz (opcional)
void printMatrix(double** matrix) {
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// Función para multiplicar dos matrices cuadradas (secuencial)
void multiplyMatricesSequential(double** matrix1, double** matrix2, double** result) {
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            result[i][j] = 0.0;
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

// Función para multiplicar dos matrices cuadradas (paralela con OpenMP)
void multiplyMatricesParallel(double** matrix1, double** matrix2, double** result, int num_threads) {
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            result[i][j] = 0.0;
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}
void transposeMatrix(double** matrix) {
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = i + 1; j < MATRIX_SIZE; ++j) {
            // Intercambiar elementos (i, j) y (j, i)
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

int main() {
    // Inicializar semilla para números aleatorios
    std::srand(std::time(0));
 for(MATRIX_SIZE = 1500; MATRIX_SIZE < 1501; MATRIX_SIZE+=100){
  // Crear matrices en el heap
  double** A = new double*[MATRIX_SIZE];
  double** B = new double*[MATRIX_SIZE];
  int** K = new int*[MATRIX_SIZE];
  double** result1_seq = new double*[MATRIX_SIZE];
  double** result1_par = new double*[MATRIX_SIZE];

  for (int i = 0; i < MATRIX_SIZE; ++i) {
      A[i] = new double[MATRIX_SIZE];
      B[i] = new double[MATRIX_SIZE];
      K[i] = new int[MATRIX_SIZE];
      result1_seq[i] = new double[MATRIX_SIZE];
      result1_par[i] = new double[MATRIX_SIZE];
  }

  // Inicializar matrices
  initializeMatrix(A);
  initializeMatrix(B);
  initializeIntMatrix(K);
  
  // Medir tiempo de ejecución secuencial
  double start_time_seq = omp_get_wtime();
  //transposeMatrix(B);
  multiplyMatricesSequential(A, B, result1_seq);
  multiplyMatrixElementwiseSequential(result1_seq, K, result1_seq);
  double end_time_seq = omp_get_wtime();
  std::cout << "Tiempo secuencial de multiplicación de matrices: " << end_time_seq - start_time_seq << " segundos\n";

  // Medir tiempo de ejecución paralelo con diferentes números de hebras
  for (int num_threads = 1; num_threads <= 4; num_threads *= 2) {
      double start_time_par = omp_get_wtime();
      //transposeMatrix(B);
      multiplyMatricesParallel(A, B, result1_par, num_threads);
      multiplyMatrixElementwiseParallel(result1_par, K, result1_par,num_threads);
      double end_time_par = omp_get_wtime();
      std::cout << "Tiempo paralelo de multiplicación de matrices (Threads=" << num_threads << "): "
                << end_time_par - start_time_par << " segundos\n";
  }

  // Imprimir matrices o hacer cualquier otro procesamiento

  // Liberar memoria
  for (int i = 0; i < MATRIX_SIZE; ++i) {
      delete[] A[i];
      delete[] B[i];
      delete[] K[i];
      delete[] result1_seq[i];
      delete[] result1_par[i];
  }

  delete[] A;
  delete[] B;
  delete[] K;
  delete[] result1_seq;
  delete[] result1_par;
 }
    return 0;
}