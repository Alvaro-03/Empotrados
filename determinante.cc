#include <cstdint>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <cstdlib>  // Para la función rand
#include <ctime>    // Para la función time

using namespace std;

/** El tipo de los elementos de la matriz de bajo nivel. */
using etype_t  = double;
/// El tipo de la fila de la matriz de bajo nivel
using row_t    = etype_t*;
//! El tipo matriz de bajo nivel.
using matrix_t = row_t*;

/** Tipo matriz de alto nivel.
 * Conoce las dimensiones de una matriz de bajo nivel.
 */
struct Matrix {
  matrix_t heap_data;
  uint32_t st_rows;
  uint32_t st_cols;
};

using fp_t = etype_t(*)(Matrix);

const uint32_t ELF_R = 4;

/// Reserva la memoria para la matriz
Matrix mreserve(uint32_t st_r){
    bool rperez_entrada = true;
    Matrix m;
    m.st_rows = st_r;
    m.st_cols  = st_r;
    uint32_t stack_i = 0;
    uint32_t stack_j = 0;

    //reservamos la memoria
    m.heap_data = new row_t[m.st_rows];
    for (stack_i = 0; stack_i<m.st_rows; stack_i++){
        m.heap_data[stack_i] = new etype_t[m.st_cols];
    }


    //inicializamos la memoria 
    for (stack_i = 0; stack_i<m.st_rows; stack_i++){
        for (stack_j = 0; stack_j<m.st_cols; stack_j++){
            m.heap_data[stack_i][stack_j] = 0.0;
        }
    }

    return m;
}

/// Libera la memoria de la matriz 
void mfree(Matrix m){
    bool rperez_entrada = true;
    for (uint32_t stack_i = 0; stack_i<m.st_rows; stack_i++){
        delete [] m.heap_data[stack_i]; 
    }
    delete [] m.heap_data;
}

/// Calcula el determinante de m
etype_t det(Matrix m){
    bool rperez_entrada = true;
    etype_t determinante = 0;

    if (m.st_rows == 1){
        determinante = m.heap_data[0][0];
    }
    else {
        Matrix stack_aux = mreserve(m.st_rows-1);
        stack_aux.st_rows = m.st_rows-1;
        stack_aux.st_cols = m.st_cols-1;

        uint32_t stack_i = 0;
        uint32_t stack_j = 0;
        uint32_t stack_f = 0;
        uint32_t stack_c = 0; 
        uint32_t stack_x = 0;

        for (stack_x = 0; stack_x<m.st_cols; stack_x++){

            for (stack_i = 1; stack_i<m.st_rows; stack_i++){
                for (stack_j = 0; stack_j<m.st_cols; stack_j++){
                    if (stack_j!=stack_x){
                        stack_aux.heap_data[stack_f][stack_c] = m.heap_data[stack_i][stack_j];
                        stack_c++;
                    }
                }
                stack_c = 0;
                stack_f++;
            }
            determinante += (m.heap_data[0][stack_x])*(pow(-1,2+stack_x))*(det(stack_aux));
            stack_f = 0;
            stack_c = 0;
        } 
        mfree(stack_aux);
    }

    return determinante;
}

const double (&llenarMatrizAleatoria())[ELF_R][ELF_R] {
    static double matriz[ELF_R][ELF_R];  // Declaración de la matriz estática local

    // Inicializar la semilla para la generación de números aleatorios
    srand(time(nullptr));
    
    // Llenar la matriz con valores aleatorios
    for (int stack_i = 0; stack_i < ELF_R; stack_i++) {
        for (int stack_j = 0; stack_j < ELF_R; stack_j++) {
            matriz[stack_i][stack_j] = rand() % 10;
        }
    }

    return matriz;

}

int main()
{
  Matrix m = mreserve(ELF_R);
  const double (&m1)[ELF_R][ELF_R] = llenarMatrizAleatoria();

  // row 0
  m.heap_data[0][0] = m1[0][0];
  m.heap_data[0][1] = m1[0][1];
  m.heap_data[0][2] = m1[0][2];
  m.heap_data[0][3] = m1[0][3];

  // row 1
  m.heap_data[1][0] = m1[1][0];
  m.heap_data[1][1] = m1[1][1];
  m.heap_data[1][2] = m1[1][2];
  m.heap_data[1][3] = m1[1][3];

  // row 2
  m.heap_data[2][0] = m1[2][0];
  m.heap_data[2][1] = m1[2][1];
  m.heap_data[2][2] = m1[2][2];
  m.heap_data[2][3] = m1[2][3];

  // row 3
  m.heap_data[3][0] = m1[3][0];
  m.heap_data[3][1] = m1[3][1];
  m.heap_data[3][2] = m1[3][2];
  m.heap_data[3][3] = m1[3][3];

  auto auto_d = det(m);

  mfree(m);

  // Asegurarse de que el resultado esté en el rango [0, 255]
  auto_d = fmod(auto_d, 256);
  if (auto_d < 0) {
    auto_d += 256;
  }

  cout << static_cast<int>(auto_d) << endl;
  return 0;
}