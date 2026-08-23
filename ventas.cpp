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

int main(){
    string fecha; 
    cout<<"ingrese la fecha de hoy (dd-mm-aaaa): "; 
    cin>>fecha; 

    string Fecha= "Comandas_"+fecha + ".dat"; 
    fstream archivo(Fecha.c_str(),ios::in|ios::out|ios::binary); 

    if (!archivo){
        archivo.open(Fecha,ios::out|ios::binary);
        archivo.close; 

        archivo.open(Fecha,ios::in|ios::out|ios::binary);
    }
}