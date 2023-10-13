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
  uint32_t rows;
  uint32_t cols;
};

using fp_t = etype_t(*)(Matrix);


/// Reserva la memoria para la matriz
Matrix  mreserve(uint32_t r){
    Matrix m;
    m.rows = r;
    m.cols  = r;
    uint32_t i = 0;
    uint32_t j = 0;

    //reservamos la memoria
    m.data = new row_t[m.rows];
    for (i = 0; i<m.rows; i++){
        m.data[i] = new etype_t[m.cols];
    }


    //inicializamos la memoria 
    for (i = 0; i<m.rows; i++){
        for (j = 0; j<m.cols; j++){
            m.data[i][j] = 0.0;
        }
    }

    return m;
}

/// Libera la memoria de la matriz 
void mfree(Matrix m){
    for (uint32_t i = 0; i<m.rows; i++){
        delete [] m.data[i]; 
    }
    delete [] m.data;
}

/// Calcula el determinante de m
etype_t det(Matrix m){
    //Suponemos que es una matriz de rango 3
    etype_t determinante = 0; 

    if (m.rows == 1){
        determinante = m.data[0][0];
    }
    else {
        Matrix aux = mreserve(m.rows-1);
        aux.rows = m.rows-1;
        aux.cols = m.cols-1;

        uint32_t i = 0;
        uint32_t j = 0;
        uint32_t f = 0;
        uint32_t c = 0; 
        uint32_t x = 0;

        for (x = 0; x<m.cols; x++){

            for (i = 1; i<m.rows; i++){
                for (j = 0; j<m.cols; j++){
                    if (j!=x){
                        aux.data[f][c] = m.data[i][j];
                        c++;
                    }
                }
                c = 0;
                f++;
            }
            determinante += (m.data[0][x])*(pow(-1,2+x))*(det(aux));
            f = 0;
            c = 0;
        } 
        mfree(aux);
    }

    return determinante;
}

int main()
{
  uint32_t R = 4;
  Matrix m = mreserve(R);

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

  auto d = det(m);

  mfree(m);

  // Asegurarse de que el resultado esté en el rango [0, 255]
  d = fmod(d, 256);
  if (d < 0) {
      d += 256;
  }

  cout << d << endl;
  return 0;
}