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

//funcion de Mozo (probar funciona)
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
        if (m.idMozo==id_Original)
        {
            fclose(f);
            return true; 
        }
         
    } 
    fclose(f); 
    return false; 
}

//funcion para validar al mozo(probar)
bool ClaveV (Mozo mozo){   
     char clave[20]; 
     cout<<"ingrese su clave: "; 
     cin>>clave;
      if (clave==m.PassWord)
        { 
            return true;   
        }
        return false;  
 }

//funcion para buscar un producto (probar)
bool BProducto(int C_Producto, Producto,&producto){ 
    FILE* f=fopen("inventario.dat","rb"); 
    if(f==null){
        cout<<"el archivo no se puede ejecutar."; 
        return false; 
    }
    while (fread(&producto,sizeof(producto),1,f )==1){
        if (producto==C_Producto)
        {
            fclose(f);
            return true; 
        }
        
    }
    fclose(f);
    return false; 
    
}

//funcion para ordenar las ventas (probar)

void O_ventas(Ventas[], int Cant_Ventas) {
    for (int i = 0; i < Cant_Ventas - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (Ventas.idMozo[j] > ventas.idMozo[j + 1]) {
                Ventas = ventas[j];
                ventas[j] = ventas[j + 1];
                ventas[j + 1] = Ventas;
                }
            }
        }
}

//funcion para mostrar las ventas (probar) 
void M_Ventas(ventas[], int C_Ventas)
{
    cout << endl;
    cout << "===== VENTAS CARGADAS =====" << endl;

    for (int i = 0; i < cantidadVentas; i++)
    {
        cout << "Mozo: " << ventas[i].idMozo
             << " | Producto: " << ventas[i].Idproducto
             << " | Cantidad: " << ventas[i].cantidad
             << " | Comision: $" << ventas[i].comision
             << endl;
    }
}







// abrir/crear plantilla del dia (prueba)
int main(){
    string fecha; 
    cout<< endl <<"ingrese la fecha de hoy (dd_mm_aaaa): "; 
    cin>>fecha; 
    string Nombre_A= "Comandas_"+fecha + ".dat"; 
    cout<<endl; 
    cout<<"plantilla del dia: "<<Nombre_A<<endl; 
    
    FILE* f = fopen(Nombre_A.c_str(), "ab+");
    if (f==null )
    {
        cout<<"el archivo no se puede crear"<<endl;     
    }
    return 1;  
    
    
    return 0; 
}