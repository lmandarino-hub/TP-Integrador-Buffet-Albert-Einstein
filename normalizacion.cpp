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
// como los datos se encontraran desordenados, utilizo ordenamiento por seleccion para ordenar los datos
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
void generarMozoDat(){
    FILE* mozo = fopen("mozo.dat","rb");
    if (mozo == NULL) {
        cout << "no se pudo generar el archivo."<< endl;
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

    ordenarPorID(lista_de_mozos,cantidad_de_mozos);

    FILE* mozo_totalComision = fopen("comandasHistoricas.dat","rb");
    if (mozo_totalComision == NULL){
        cout << "Error al abrir comandasHistoricas.dat" << endl;
    }
    Comanda c;
    while (fread(&c,sizeof(Mozo),1,mozo_totalComision) == 1){   
        int pos = buscarMozo(lista_de_mozos,cantidad_de_mozos,c.idMozo);

        if (pos != -1){
            lista_de_mozos[pos].totalComision += (c.comision * c.cantidad);
        }
    }
    fclose(mozo_totalComision);
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



int main(){
    int cantidad_de_mozos = 3;
    Mozo lista_de_mozo[3] = {
        {1, "romeo", "pass123", 0.0f},
        {2, "christian", "pass456", 0.0f},
        {3, "ramos", "pass789", 0.0f}
    };
    mostrarIDdeMozo(lista_de_mozo,cantidad_de_mozos);
    return 0;
}
