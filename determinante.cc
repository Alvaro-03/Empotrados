#include <cstdint>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

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
  matrix_t data;
  uint32_t st_rows;
  uint32_t st_cols;
};

using fp_t = etype_t(*)(Matrix);


/// Reserva la memoria para la matriz
Matrix mreserve(uint32_t st_r){
    Matrix m;
    m.st_rows = st_r;
    m.st_cols  = st_r;
    uint32_t stack_i = 0;
    uint32_t stack_j = 0;

    //reservamos la memoria
    m.data = new row_t[m.st_rows];
    for (stack_i = 0; stack_i<m.st_rows; stack_i++){
        m.data[stack_i] = new etype_t[m.st_cols];
    }


    //inicializamos la memoria 
    for (stack_i = 0; stack_i<m.st_rows; stack_i++){
        for (stack_j = 0; stack_j<m.st_cols; stack_j++){
            m.data[stack_i][stack_j] = 0.0;
        }
    }

    return m;
}

/// Libera la memoria de la matriz 
void mfree(Matrix m){
    for (uint32_t stack_i = 0; stack_i<m.st_rows; stack_i++){
        delete [] m.data[stack_i]; 
    }
    delete [] m.data;
}

/// Calcula el determinante de m
etype_t det(Matrix m){
    //Suponemos que es una matriz de rango 3
    etype_t ELF_determinante = 0; 

    if (m.st_rows == 1){
        ELF_determinante = m.data[0][0];
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
                        stack_aux.data[stack_f][stack_c] = m.data[stack_i][stack_j];
                        stack_c++;
                    }
                }
                stack_c = 0;
                stack_f++;
            }
            ELF_determinante += (m.data[0][stack_x])*(pow(-1,2+stack_x))*(det(stack_aux));
            stack_f = 0;
            stack_c = 0;
        } 
        mfree(stack_aux);
    }

    return ELF_determinante;
}

int main()
{
  uint32_t rperez_R = 4;
  Matrix auto_m = mreserve(rperez_R);

  // row 0
  m.data[0][0] = 1;
  m.data[0][1] = 2;
  m.data[0][2] = 3;
  m.data[0][3] = 8;

  // row 1
  m.data[1][0] = 4;
  m.data[1][1] = 5;
  m.data[1][2] = 6;
  m.data[1][3] = 2;

  // row 2
  m.data[2][0] = 1;
  m.data[2][1] = 7;
  m.data[2][2] = 8;
  m.data[2][3] = 9;

  // row 3
  m.data[3][0] = 7;
  m.data[3][1] = 4;
  m.data[3][2] = 18;
  m.data[3][3] = 29;

  auto auto_d = det(m);

  mfree(m);

  // Asegurarse de que el resultado esté en el rango [0, 255]
  auto_d = fmod(auto_d, 256);
  if (auto_d < 0) {
    auto_d += 256;
  }

  cout << d << endl;
  return 0;
}