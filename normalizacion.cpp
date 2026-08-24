#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

const float TASA_COMISION = 0.10f;

struct Producto {
  int codigo;
  char nombre[50];
  float precio;
  int stockActual;
};

struct Comanda {
  int idMozo;
  int codigoProducto;
  int cantidad;
  float comision;
};

struct Mozo {
  int idMozo;
  char nombre[50];
  char password[20];
  float totalComision;
};

int main() { return 0; }