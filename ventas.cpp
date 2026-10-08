#include <iostream>
#include <cstdio>
#include <cstring> 
using namespace std; 
//tasa de comision  dado por el profe
const float TASA_COMISION = 0.10f;
const int M_ventas = 100;
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

//funcion de Mozo 
bool ClaveDeAcceso(int id_buscado, Mozo &mozoEncontrado){
    Mozo m; 
    FILE* f = fopen("Mozos.dat", "rb");
    if (f==NULL)
    {
        cout<<"no se puede abrir el archivo con los mozos";
        return false; 
    }
    while(fread(&m,sizeof(Mozo),1,f)==1){
        if (m.idMozo==id_buscado)
        {
            mozoEncontrado= m; 
            fclose(f);
            return true; 
        }   
    } 
    fclose(f); 
    return false; 
}

//funcion para validar al mozo
bool ClaveV (Mozo mozo){   
     char clave[20]; 
     cout<<"ingrese su clave: "; 
     cin>>clave;
      if (strcmp(clave,mozo.PassWord)==0) 
        { 
            return true;   
        }
        return false;  
 }

//funcion para buscar un producto
bool BProducto(int C_Producto, Producto &producto){ 
    FILE* f=fopen("inventario.dat","rb"); 
    if(f==NULL){
        cout<<"el archivo no se puede ejecutar."; 
        return false; 
    }
    while (fread(&producto,sizeof(producto),1,f )==1){
        if (producto.codigo == C_Producto)
        {
            fclose(f);
            return true; 
        }
        
    }
    fclose(f);
    return false; 
    
}

//funcion para ordenar las ventas 

void Orden_ventas(ventas listaVentas[], int Cant_Ventas) {
    ventas VentaS; 
    for (int i = 0; i < Cant_Ventas - 1; i++) {
        for (int j = 0; j < Cant_Ventas - i - 1; j++) {
            if (listaVentas[j].idMozo > listaVentas[j + 1].idMozo) {
                VentaS = listaVentas[j];
                listaVentas[j] = listaVentas[j + 1];
                listaVentas[j + 1] = VentaS;
                }
            }
        }
}

//funcion para mostrar las ventas (probar) 
void Mostrar_Ventas(ventas listaVentas[], int cantidadVentas)
{
    cout << endl;
    cout << "===== VENTAS CARGADAS =====" << endl;

    for (int i = 0; i < cantidadVentas; i++)
    {
        cout << "Mozo: " << listaVentas[i].idMozo
             << " | Producto: " << listaVentas[i].Idproducto
             << " | Cantidad: " << listaVentas[i].cantidad
             << " | Comision: $" << listaVentas[i].comision
             << endl;
    }
}


// main 
int main(){
    string fecha; 
    ventas listaVentas[M_ventas];
    int Cant_Ventas=0; 
    char continuar ='s'; 
    
    cout<< endl <<"ingrese la fecha de hoy (dd_mm_aaaa): "; 
    cin>>fecha; 
    string Nombre_A= "Comandas_"+fecha + ".dat"; 
    cout<<"plantilla del dia: "<< Nombre_A <<endl; 
    
    FILE* f = fopen(Nombre_A.c_str(), "ab+");
    if (f==NULL)
    {
        cout<<"el archivo no se puede crear"<<endl;     
        return 1;  
    }
    
    while (Cant_Ventas<M_ventas && fread(&listaVentas[Cant_Ventas],sizeof(ventas),1,f)==1)
    {
        Cant_Ventas++; 
    }
    
    while (continuar=='s'||continuar == 'S')
    {
        int idmozo; 
        cout << "Nueva Venta"<<endl; 
        cout <<"ingrese id del mozo: "; 
        cin>>idmozo;
        Mozo mozo; 

        if (!ClaveDeAcceso(idmozo,mozo)){
            cout<<"el mozo no existe."<<endl; 
            continue;
        }
     
    

    if (!ClaveV(mozo)){
        cout<<"clave incorrecta."<<endl; 
        continue;
    }
    cout<<"clave correcta."<<endl; 
    
    int codigo_prod; 

    cout<<"ingrese el codigo del producto: ";
    cin>>codigo_prod; 
    Producto producto; 
    
    if(!BProducto(codigo_prod, producto)){
         cout<<"El producto no existe."<<endl; 
         continue; 
    }
        cout<<"producto:"<<producto.descrippcion <<endl; 
        cout<<"stock disponible: "<<producto.stockActual <<endl; 
        cout<<"precio: "<<producto.precio <<endl;  
    
        int cantidad; 
    
        cout<<"ingrese la cantidad: "; 
        cin>>cantidad; 

       if(cantidad<=0){
        cout<<"La cantidad debe ser mayor a cero." <<endl; 
        continue; 
       }
   
       if (cantidad>producto.stockActual)
       {
        cout<< "No hay suficiente stock." <<endl; 
        continue; 
       }
     
    float totalVenta=producto.precio * cantidad; 
    float comision=totalVenta * TASA_COMISION; 
   

    ventas nuevaVEnta;
    nuevaVEnta.idMozo = idmozo; 
    nuevaVEnta.Idproducto = codigo_prod; 
    nuevaVEnta.cantidad = cantidad; 
    nuevaVEnta.comision = comision; 

   if(Cant_Ventas < M_ventas){
     listaVentas[Cant_Ventas] = nuevaVEnta; 
     Cant_Ventas++;
     fwrite(&nuevaVEnta, sizeof(ventas), 1, f);
     cout << "ventas guardadas. " << endl; 
     cout << "Total de ventas: $" << totalVenta << endl;  
     cout << "comision: $" <<comision << endl;   
     }
     else 
     { 
     cout <<"Se alcanzo el limite de ventas." <<endl; 
      
     }

     cout<<"Quiere cargar otra venta: "; 
     cin>>continuar; 
    



   fclose(f); 

   Orden_ventas(listaVentas, Cant_Ventas); 

   FILE* fordenado=fopen (Nombre_A.c_str(),"wb"); 

   if(fordenado==NULL){
    cout<<"no se puede abrir "<<endl; 
    return 1; 

   }

   for(int i=0; i<Cant_Ventas; i++){
    fwrite(&listaVentas[i],sizeof(ventas),1,fordenado); 
   } 
 
 fclose(fordenado); 

  Mostrar_Ventas(listaVentas,Cant_Ventas); 
  cout<<"carga finalizada"<<endl; 
  cout<<"archivo: "<<Nombre_A<<endl; 
  cout<<"ventas totales: "<<Cant_Ventas<<endl; 
 

    return 0; 
}
  }