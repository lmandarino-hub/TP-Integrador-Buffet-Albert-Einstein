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
    FILE* f = fopen("Mozos.dat", "rb");
    if (f==null)
    {
        cout<<"no se puede abrir el archivo con los mozos";
        return false; 
    }
    while(fread(&mozo,sizeof(Mozo),1,f)==1){
        if (m.idMozo==m.PassWord)
        {
            fclose(f);
            return true; 
        }
         
    } 
   
 cout<<"ingrese su id: "; 
 cin>>m.idMozo; 
 cout<<"ingrese su clave: "; 
 cin>>clave;

}


//funcion comandasH 
void ComandasHistoricas(){
    FILE* f = fopen("ComandasHistoricas", "ab+");
       if (f==null )
       {
        cout<<"el archivo no se puede crear";     
       }
        return f;  
       
}


// abrir/crear plantilla del dia (prueba x)
int main(){
    string fecha; 
    ClaveDeAcceso(); //mover 
    cout<< endl <<"ingrese la fecha de hoy (dd-mm-aaaa): "; 
    cin>>fecha; 
    
    string Fecha= "ComandasHistoricas"+fecha + ".dat"; 
    

    return 0; 
}