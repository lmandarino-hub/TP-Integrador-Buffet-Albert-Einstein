#include <iostream>
#include <cstdio> 
#include <cstring> 
using namespace std;

struct Mozo{
    int idMozo;
    char Nombre[50];
    char password[20];
    float totalComision;
};
struct Comanda{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

struct Producto{
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};


void mostrarIDdeMozo(Mozo lista_de_mozo[], int cantidad_de_mozos){
    for( int i = 0; i < cantidad_de_mozos - 1 ;i++){
        int minIdx = i;
        for (int j = i + 1; j < cantidad_de_mozos; j++){
            if (lista_de_mozo[j].idMozo < lista_de_mozo[minIdx].idMozo){
                minIdx = j;
            }
        }
        if (minIdx != i){
            Mozo aux = lista_de_mozo[i];
            lista_de_mozo[i] =  lista_de_mozo[minIdx];
            lista_de_mozo[minIdx] = aux;
        }
    }
    for(int i = 0; i < cantidad_de_mozos;i++){
        cout<< "ID: " << lista_de_mozo[i].idMozo << endl;
        cout<< "Nombre: " << lista_de_mozo[i].Nombre << endl;
    }
}
// como los datos se encontraron desordenados, utilizo ordenamiento por seleccion para ordenar los datos.
void ordenarPorID(Mozo lista_de_mozos[], int cantidad_de_mozos){
    for (int i = 0; i < cantidad_de_mozos - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < cantidad_de_mozos; j++) {
            if (lista_de_mozos[j].idMozo < lista_de_mozos[minIdx].idMozo) {
                minIdx = j;
            }
        }
        // Intercambiar el mínimo con la posición i
        Mozo temp = lista_de_mozos[i];
        lista_de_mozos[i] = lista_de_mozos[minIdx];
        lista_de_mozos[minIdx] = temp;
    }

}
//busco el mozo correspondiente por busqueda secuencial.
int buscarMozo(Mozo lista_de_mozo[], int cantidad_de_mozos, int idBuscado){
    int i = 0;
    while (i < cantidad_de_mozos && lista_de_mozo[i].idMozo != idBuscado){
        i++;
    }
    if ( i == cantidad_de_mozos){
        return -1;
    }
    else{
        return i;
    }
}

void Calcular_totalComision(Mozo lista_de_mozos[], int cantidad_de_mozos, Comanda lista_de_comandas[], int cantidad_de_comandas){
    for (int i = 0; i < cantidad_de_mozos; i++){
        float totalComision = 0;
        for (int j = 0; j < cantidad_de_comandas; j++){
            if (lista_de_mozos[i].idMozo == lista_de_comandas[j].idMozo){
                totalComision += lista_de_comandas[j].comision;
            }
        }
        lista_de_mozos[i].totalComision = totalComision;
    }
}


void generarMozoDat(){
    FILE* mozo = fopen("mozo.dat","rb");
    if (mozo == NULL) {
        cout << "no se pudo generar el archivo."<< endl;
        return; /* corta la ejecucion si no abre*/
    }
    Mozo lista_de_mozos[50];
    int cantidad_de_mozos = 0;
    Mozo m;

    while(fread(&m,sizeof(Mozo),1,mozo) == 1 && cantidad_de_mozos < 50){
        m.totalComision = 0.0f;
        lista_de_mozos[cantidad_de_mozos] = m;
        cantidad_de_mozos++;
    }
    fclose(mozo);

    //ordena a los mozos por id
    ordenarPorID(lista_de_mozos,cantidad_de_mozos);

    FILE* mozo_totalComision = fopen("comandasHistoricas.dat","rb");
    if (mozo_totalComision == NULL){
        cout << "Error al abrir comandasHistoricas.dat" << endl;
        return; /* corta la ejecucion si no abre*/
    }
    Comanda c;
    while (fread(&c,sizeof(Comanda),1,mozo_totalComision) == 1){   
        int posicion_del_mozo = buscarMozo(lista_de_mozos,cantidad_de_mozos,c.idMozo);

        if (posicion_del_mozo != -1){
            lista_de_mozos[posicion_del_mozo].totalComision += (c.comision * c.cantidad);
        }
    }
    fclose(mozo_totalComision);
    FILE* mozo_terminado = fopen("mozo.dat","wb");
    if (mozo_terminado == NULL){
        cout << "Error al abrir/generar mozo.dat" << endl;
        return; /* corta la ejecucion si no abre*/
    }
    for (int i = 0; i < cantidad_de_mozos;i++){
        fwrite(&lista_de_mozos[i],sizeof(Mozo),1,mozo_terminado);
    }
    fclose(mozo_terminado);
}

int buscarMozoPorNombre(Mozo lista_de_mozos[], int cantidad_de_mozos, const char* nombreBuscado) {
    for (int i = 0; i < cantidad_de_mozos; i++) {
        if (strcmp(lista_de_mozos[i].Nombre, nombreBuscado) == 0) {
            return i; // Retorna el índice del mozo encontrado
        }
    }
    return -1; // Retorna -1 si no se encuentra el mozo
}

// Struct temporal para ordenar los datos en memoria antes de guardarlos
struct comandaTemp {
    char fecha[11];
    Comanda datos;
};

struct ventaHistorica{

    char fecha[11];
    char nombreMozo[50];
    int codigoProducto;
    int cantidad;
    float comision;
};


void separarVentas(Mozo lista_de_mozos[], int cantidad_de_mozos){

Producto inventario[100];
    int cant_inventario = 0;
    FILE* archInv = fopen("inventario.dat", "rb");
    if (archInv != NULL) {
        while (fread(&inventario[cant_inventario], sizeof(Producto), 1, archInv) == 1) {
            cant_inventario++;
        }
        fclose(archInv);
    }
}

comandaTemp lista_comandas[1000]; // Arreglo para guardar todo temporalmente
    int cant_comandas = 0;
    
    FILE* ventasHistorico = fopen("comandas_historicas.dat", "rb");
    if (ventasHistorico == NULL) {
        cout << "Error al abrir comandas_historicas.dat" << endl;
        return;
    }

    comandaHistorica v;
    while (fread(&v, sizeof(ComandaHistorica), 1, ventasHistorico) == 1) {

    // busca la ID del mozo por su nombre
    int posicion_del_mozo = buscarMozoPorNombre(lista_de_mozos, cantidad_de_mozos, v.nombreMozo);
    int idMozo = (posicion_del_mozo != -1) ? lista_de_mozos[posicion_del_mozo].idMozo : 0;
    int idReal = 0;
    if (posMozo != -1) {
            idReal = lista_de_mozos[posMozo].idMozo;
        }
    // guarda en el array temporal con el nuevo formato
    strcpy(lista_comandas[cant_comandas].fecha, v.fecha);
    lista_comandas[cant_comandas].datos.idMozo = idMozo;
    lista_comandas[cant_comandas].datos.codigoProducto = v.codigoProducto;
    lista_comandas[cant_comandas].datos.cantidad = v.cantidad;
    lista_comandas[cant_comandas].datos.comision = v.comision;
    cant_comandas++;
    
    for(int i = 0; i < cant_inventario)

int main(){
    generarMozoDat();
    return 0;
}