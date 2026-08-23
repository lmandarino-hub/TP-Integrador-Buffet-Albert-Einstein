#include <iostream>
#include <cstdio>
#include <cstring> 
using namespace std; 

const float TASA_COMISION = 0.10f;

struct ventas
{ 
    int ClaveProducto; 
    float precio;
};

struct Mozo
{
    int idMozo;
    char Nombre[50];
    char PassWord[20];
    float TotalCOMISION;
};