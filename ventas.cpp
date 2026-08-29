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

//funcion de login (prueba 2 funciona(base))
void ClaveDeAcceso(){
    Mozo m; 
    m.idMozo; 
   strcpy (m.PassWord,"hola"); 
    char clave[20]; 
 cout<<"ingrese su id: "; 
 cin>>m.idMozo; 
 cout<<"ingrese su clave: "; 
 cin>>clave;
if  (strcmp(clave, m.PassWord) == 0)
{
    cout<<"clave correcta "; 
}
else
{  
    cout<<"clave incorrecta "; 
    exit (0); //cambiar 
}

}
// abrir/crear plantilla del dia (prueba x)
int main(){
    string fecha; 
    ClaveDeAcceso(); //mover 
    cout<< endl <<"ingrese la fecha de hoy (dd-mm-aaaa): "; 
    cin>>fecha; 
    
    string Fecha= "Comandas_"+fecha + ".dat"; 
    

    return 0; 
}