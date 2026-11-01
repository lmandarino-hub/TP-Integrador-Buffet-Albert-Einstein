#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int K = 5;
const int MAX_MOZOS = 50;
const int MAX_COMANDAS = 1000;
const int MAX_PRODUCTOS = 200;

const char* PALABRAS[] = {"sol", "luna", "mar", "rio", "cielo"};
const int CANT_PALABRAS = 5;


struct Mozo{
    int idMozo;
    char Nombre[50];
    char password[20];
    float totalComision;
};

struct Producto{
    int codigo;
    char descrippcion[50];
    float precio;
    int stockActual;
};

struct ComandaHistorica{
    char fecha[11];
    char nombreMozo[50];
    int codigoProducto;
    int cantidad;
    float comision;
};

struct Comanda{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

struct ComandaTemp{
    char fecha[11];
    Comanda datos;
};


void CifrarClave(char clave[]){
    for (int i = 0; clave[i] != '\0'; i++){
        clave[i] = clave[i]+K;
    }
}

// Ordenamiento por seleccion
void ordenarPorID(Mozo lista_de_mozos[], int cantidad_de_mozos){
    for (int i = 0; i < cantidad_de_mozos - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < cantidad_de_mozos; j++) {
            if (lista_de_mozos[j].idMozo < lista_de_mozos[minIdx].idMozo) {
                minIdx = j;
            }
        }
        Mozo temp = lista_de_mozos[i];
        lista_de_mozos[i] = lista_de_mozos[minIdx];
        lista_de_mozos[minIdx] = temp;
    }
}

// Busqueda secuencial
int buscarMozoPorNombre(Mozo lista[], int cantidad, const char nombreBuscado[]){
    int i = 0;
    while (i < cantidad && strcmp(lista[i].Nombre, nombreBuscado) != 0){
        i++;
    }
    if (i == cantidad) {
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



int main(){
    generarMozoDat();
    return 0;
}