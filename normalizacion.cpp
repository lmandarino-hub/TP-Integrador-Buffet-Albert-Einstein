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

struct comandaHistorica {
    char fecha[11];        // "DD-MM-AAAA"
    char nombreMozo[50];   // el nombre completo, repetido en cada venta
    int codigoProducto;    int cantidad;    float comision;
};

void separarVentas(Mozo lista_de_mozos[], int cantidad_de_mozos) {
    comandaTemp lista_comandas[1000]; // array para guardar todas las comandas temporalmente
    int cant_comandas = 0;
    
    // Leer ventas historicas
    FILE* ventasHistorico = fopen("comandas_historicas.dat", "rb");
    if (ventasHistorico == NULL) {
        cout << "Error al abrir comandas_historicas.dat" << endl;
        return;
    }

    comandaHistorica v;
    while (fread(&v, sizeof(comandaHistorica), 1, ventasHistorico) == 1) {
        // Buscamos el ID del mozo usando su nombre
        int posMozo = buscarMozoPorNombre(lista_de_mozos, cantidad_de_mozos, v.nombreMozo);
        int idReal = 0;
        if (posMozo != -1) {
            idReal = lista_de_mozos[posMozo].idMozo;
        }

        // Guardamos en el array temporal con el nuevo formato de Comanda
        strcpy(lista_comandas[cant_comandas].fecha, v.fecha);
        lista_comandas[cant_comandas].datos.idMozo = idReal;
        lista_comandas[cant_comandas].datos.codigoProducto = v.codigoProducto;
        lista_comandas[cant_comandas].datos.cantidad = v.cantidad;
        lista_comandas[cant_comandas].datos.comision = v.comision;
        
        cant_comandas++;
    }
    fclose(ventasHistorico);

    // Ordenas comandas (Primero por fecha, despues por idMozo para que queden agrupadas)
    for (int i = 0; i < cant_comandas - 1; i++) {
        for (int j = i + 1; j < cant_comandas; j++) {
            // Si la fecha j es menor (va antes), o si son la misma fecha pero el idMozo j es menor
            if (strcmp(lista_comandas[j].fecha, lista_comandas[i].fecha) < 0 || 
               (strcmp(lista_comandas[j].fecha, lista_comandas[i].fecha) == 0 && 
                lista_comandas[j].datos.idMozo < lista_comandas[i].datos.idMozo)) {
                
                // Intercambio
                comandaTemp aux = lista_comandas[i];
                lista_comandas[i] = lista_comandas[j];
                lista_comandas[j] = aux;
            }
        }
    }

    // Corte de control para generar archivos diarios
    int i = 0;
    while (i < cant_comandas) {
        // Nos guardamos la fecha que rige para este grupo
        char fechaActual[11];
        strcpy(fechaActual, lista_comandas[i].fecha); 

        // Armamos el nombre del archivo: "comandas_DD-MM-AAAA.dat"
        char nombreArchivo[50];
        strcpy(nombreArchivo, "comandas_");
        strcat(nombreArchivo, fechaActual);
        strcat(nombreArchivo, ".dat");

        // Al abrir en "wb", crea un archivo nuevo para este día
        FILE* archivoDia = fopen(nombreArchivo, "wb");

        // Mientras no nos pasemos del total y la fecha siga siendo la misma (corte de control)
        while (i < cant_comandas && strcmp(lista_comandas[i].fecha, fechaActual) == 0) {
            // Grabamos sólo la parte 'datos' que es el struct Comanda original
            fwrite(&lista_comandas[i].datos, sizeof(Comanda), 1, archivoDia);
            i++; // Avanzamos al siguiente registro
        }
        
        // Cuando cambia la fecha (termina el while interno), cerramos el archivo del día
        fclose(archivoDia); 
    }
    
    cout << "Ventas separadas exitosamente por dia." << endl;
}

void actualizarStock() {
    // 1. CARGAR EL INVENTARIO EN MEMORIA
    Producto inventario[200]; // Arreglo con tamaño de sobra para los productos
    int cant_inventario = 0;
    
    FILE* archInv = fopen("inventario.dat", "rb");
    if (archInv != NULL) {
        while (fread(&inventario[cant_inventario], sizeof(Producto), 1, archInv) == 1) {
            cant_inventario++;
        }
        fclose(archInv);
    } else {
        cout << "Error: No se pudo abrir inventario.dat." << endl;
        return;
    }

    // 2. LEER VENTAS HISTÓRICAS Y RESTAR STOCK
    FILE* ventasHistorico = fopen("comandas_historicas.dat", "rb");
    if (ventasHistorico != NULL) {
        ventaHistorica v; // Usá el nombre de tu struct acá (ventaHistorica o ComandaHistorica)
        
        while (fread(&v, sizeof(ventaHistorica), 1, ventasHistorico) == 1) {
            // Buscamos el producto vendido en nuestro arreglo
            for(int i = 0; i < cant_inventario; i++) {
                if(inventario[i].codigo == v.codigoProducto) {
                    
                    // Restamos la cantidad vendida
                    inventario[i].stockActual -= v.cantidad;
                    
                    // CASO RARO PARA EL ORAL: Evitar stock negativo
                    if (inventario[i].stockActual < 0) {
                        inventario[i].stockActual = 0;
                    
                    }
                    
                    break; // Cortamos el for porque ya actualizamos este producto
                }
            }
        }
        fclose(ventasHistorico);
    } else {
        cout << "Error: No se pudo abrir comandas_historicas.dat" << endl;
        return;
    }

    // Sobrescribir el archivo con el stock actualizado
    archInv = fopen("inventario.dat", "wb");
    if (archInv != NULL) {
        for(int i = 0; i < cant_inventario; i++) {
            fwrite(&inventario[i], sizeof(Producto), 1, archInv);
        }
        fclose(archInv);
        cout << "Stock actualizado exitosamente en inventario.dat." << endl;
    }
}

int main(){
    generarMozoDat();
    actualizarStock();
    return 0;
}