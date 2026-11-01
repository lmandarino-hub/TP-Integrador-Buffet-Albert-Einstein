#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
using namespace std;


const float TASA_COMISION = 0.10f;
const int K = 5;
const int STOCK_REPOSICION = 50; 


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
        cout<<"no se puede abrir el archivo con los mozos "<<endl;
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

//funcion de cifrado de clave
void CifrarClave(char clave[])
{
    for (int i = 0; clave[i] != '\0'; i++)
    {
        clave[i] = clave[i]+K;
    }
}

//funcion para validar al mozo
bool ClaveV (const Mozo &mozo){
     char clave[20];
     cout<<"ingrese su clave: ";
     cin.width(20);
     cin>>clave;
     CifrarClave(clave);
     return strcmp(clave,mozo.PassWord)==0;
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

//funcion para actualizar stock en el momento
void ActualizarStock(int codigo, int cantidad){
    FILE* f = fopen("inventario.dat", "rb+");
    if (f == NULL){
        cout << "no se pudo abrir inventario.dat para actualizar el stock." << endl;
        return;
    }
    Producto p;
    while (fread(&p, sizeof(Producto), 1, f) == 1){
        if (p.codigo == codigo){
            p.stockActual -= cantidad;
            if (p.stockActual <= 0){
                p.stockActual = STOCK_REPOSICION;
                cout << "Stock agotado: se repuso " << p.descrippcion
                     << " a " << STOCK_REPOSICION << " unidades." << endl;
            }
            fseek(f, -(long)sizeof(Producto), SEEK_CUR);
            fwrite(&p, sizeof(Producto), 1, f);
            fclose(f);
            return;
        }
    }
    fclose(f);
}


long ContarVentas(FILE* f){
    fseek(f, 0, SEEK_END);
    return ftell(f) / (long)sizeof(ventas);
}

//ordena las ventas por id del mozo 
void Orden_ventas(FILE* f) {
    long n = ContarVentas(f);
    ventas a, b;
    for (long i = 0; i < n - 1; i++) {
        for (long j = 0; j < n - i - 1; j++) {
            fseek(f, j * (long)sizeof(ventas), SEEK_SET);
            fread(&a, sizeof(ventas), 1, f);
            fread(&b, sizeof(ventas), 1, f);
            if (a.idMozo > b.idMozo) {
                fseek(f, j * (long)sizeof(ventas), SEEK_SET);
                fwrite(&b, sizeof(ventas), 1, f);
                fwrite(&a, sizeof(ventas), 1, f);
            }
        }
    }
    fflush(f);
}

//funcion para mostrar las ventas 
void Mostrar_Ventas(FILE* f)
{
    ventas v;
    cout << endl;
    cout << "===== VENTAS CARGADAS =====" << endl;

    fseek(f, 0, SEEK_SET);
    while (fread(&v, sizeof(ventas), 1, f) == 1)
    {
        cout << "Mozo: " << v.idMozo
             << " | Producto: " << v.Idproducto
             << " | Cantidad: " << v.cantidad
             << " | Comision: $" << v.comision
             << endl;
    }
}

// main
int main(){
    string fecha;
    char continuar ='s';

    cout<< endl <<"ingrese la fecha de hoy (dd_mm_aaaa): ";
    cin>>fecha;
    string Nombre_A= "Comandas_"+fecha + ".dat";
    cout<<"plantilla del dia: "<< Nombre_A <<endl;

    // si el archivo ya existe se abre para leer/escribir, si no se crea
    FILE* f = fopen(Nombre_A.c_str(), "rb+");
    if (f==NULL)
    {
        f = fopen(Nombre_A.c_str(), "wb+");
    }
    if (f==NULL)
    {
        cout<<"el archivo no se puede crear"<<endl;
        return 1;
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

        ventas nuevaVenta;
        nuevaVenta.idMozo = idmozo;
        nuevaVenta.Idproducto = codigo_prod;
        nuevaVenta.cantidad = cantidad;
        nuevaVenta.comision = comision;

        
        fseek(f, 0, SEEK_END);
        fwrite(&nuevaVenta, sizeof(ventas), 1, f);
        fflush(f);

        ActualizarStock(codigo_prod, cantidad);
        cout << "ventas guardadas. " << endl;
        cout << "Total de ventas: $" << totalVenta << endl;
        cout << "comision: $" <<comision << endl;

        cout<<"Quiere cargar otra venta: ";
        cin>>continuar;
    }

    Orden_ventas(f);
    Mostrar_Ventas(f);

    cout<<"carga finalizada"<<endl;
    cout<<"archivo: "<<Nombre_A<<endl;
    cout<<"ventas totales: "<<ContarVentas(f)<<endl;

    fclose(f);
    return 0;
}