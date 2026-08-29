#include <iostream>
#include <cstdio>
#include <cstring> 
using namespace std; 
//tasa de comision  dado por el profe
const float TASA_COMISION = 0.10f;
//struct (revisar)
struct Producto
{ 
    int codigo;
    char descrippcion[50];
    float precio;
    int stockActual; 
};

struct Mozo
{
    int idMozo;
    char Nombre[50];
    char PassWord[20];
    float TotalCOMISION;
};

struct ventas
{
    int idMozo;
    int Idproducto;
    int cantidad; 
    float comision; 
};

//funcion de login (prueba 1)
void ClaveDeAcceso(char*calve){
   if (clave==PassWord)
    {
        cout<<"clave correcta bienvenido "<<Nombre; 
   }
   else
   {
   cout<<"clave incorrecta"; 
   }

}

int main(){
    Mozo mozo1 ; 
    Mozo mozoy;
    mozo1.PassWord= "hola"; 
    mozoy.Nombre="christian"; 
    string fecha;
    string clave;
    cout<<"ingrese su clave: "; 
    cin>>clave; 
    cout<<ClaveDeAcceso;  
    cout<<"ingrese la fecha de hoy (dd-mm-aaaa): "; 
    cin>>fecha; 

    string Fecha= "Comandas_"+fecha + ".dat"; 
    

    return 0; 
}