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

/// Muestra la matriz por pantalla
void mshow(Matrix m){
    for (uint32_t i = 0; i<m.rows; i++){
        cout<<"[\t";
        for (uint32_t j = 0; j<m.cols; j++){
            cout<<m.data[i][j]<<"\t";
        }
        cout<<"]\n";
    }
}

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

etype_t multiply_elements(Matrix m, fp_t f1, fp_t f2){
    etype_t producto = f1(m)*f2(m);
    return producto;
}


void test1() {
  // [[5,3],[2,1]]
  uint32_t R = 2;
  Matrix m = mreserve(R);

  m.data[0][0] = 5;
  m.data[0][1] = 3;
  m.data[1][0] = 2;
  m.data[1][1] = 1;
  mshow(m);
  auto d = det(m);

  mfree(m);

  cout << "Det.: " << d << '\n';
  cout << "--------------------------" << endl;
}

void test2() {
  // [[1,2,3],[4,5,6],[7,8,9]]
  uint32_t R = 3;
  Matrix m = mreserve(R);

  etype_t e = 1.0;
  for (auto i = 0u; i < m.rows; i++)
    for (auto j = 0u; j < m.cols; j++) {
      m.data[i][j] = e;
      e += 1.0;
    }
  mshow(m);
  auto d = det(m);

  mfree(m);

  cout << "Det.: " << d << '\n';
  cout << "--------------------------" << endl;
}

void test3() {
  // [ [2, -4, 7] , [8, 0.5, 3] , [6, -9, 5]]
  uint32_t R = 3;
  Matrix m = mreserve(R);

  // row 0
  m.data[0][0] = 2;
  m.data[0][1] = -4;
  m.data[0][2] = 7;

  // row 1
  m.data[1][0] = 8;
  m.data[1][1] = 0.5;
  m.data[1][2] = 3;

  // row 2
  m.data[2][0] = 6;
  m.data[2][1] = -9;
  m.data[2][2] = 5;

  mshow(m);
  auto d = det(m);

  mfree(m);

  cout << "Det.: " << d << '\n';
  cout << "--------------------------" << endl;
}

void test4() {
  // [[1,2,3,8],[4,5,6,2],[1,7,8,9],[3,4,18,29]]
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
  m.data[3][0] = 3;
  m.data[3][1] = 4;
  m.data[3][2] = 18;
  m.data[3][3] = 29;

  mshow(m);
  auto d = det(m);

  mfree(m);

  cout << "Det.: " << d << '\n';
  cout << "--------------------------" << endl;
}

static etype_t max(Matrix m) {
  etype_t max = 0;

  for (auto r = 0u; r < m.rows; r++)
    for (auto c = 0u; c < m.cols; c++)
      if (m.data[r][c] > max) max = m.data[r][c];

  return max;
}

static etype_t min(Matrix m) {
  etype_t min = std::numeric_limits<etype_t>::max();

  for (auto r = 0u; r < m.rows; r++)
    for (auto c = 0u; c < m.cols; c++)
      if (m.data[r][c] < min) min = m.data[r][c];

  return min;
}

void test5() {
  Matrix m1 = mreserve(4);

  // row 0
  m1.data[0][0] = 1;
  m1.data[0][1] = 2;
  m1.data[0][2] = 3;
  m1.data[0][3] = 8;

  // row 1
  m1.data[1][0] = 4;
  m1.data[1][1] = 5;
  m1.data[1][2] = 6;
  m1.data[1][3] = 2;

  // row 2
  m1.data[2][0] = 1;
  m1.data[2][1] = 7;
  m1.data[2][2] = 8;
  m1.data[2][3] = 9;

  // row 3
  m1.data[3][0] = 3;
  m1.data[3][1] = 4;
  m1.data[3][2] = 18;
  m1.data[3][3] = 29;

  Matrix m2 = mreserve(3);

  // row 0
  m2.data[0][0] = 2;
  m2.data[0][1] = -4;
  m2.data[0][2] = 7;

  // row 1
  m2.data[1][0] = 8;
  m2.data[1][1] = 0.5;
  m2.data[1][2] = 3;

  // row 2
  m2.data[2][0] = 6;
  m2.data[2][1] = -9;
  m2.data[2][2] = 5;

  auto r1 = multiply_elements(m1, min, max);
  auto r2 = multiply_elements(m2, min, max);

  mfree(m1);
  mfree(m2);

  cout << "m1.min * m1.max = " << r1 << '\n';
  cout << "m2.min * m2.max = " << r2 << '\n';

}

int main()
{
    cout << "Ejercicio 1\n===========\n";
    test1();
    test2();
    test3();
    test4();
    cout << "\nEjercicio 2\n===========\n";
    test5();

    return 0;
}