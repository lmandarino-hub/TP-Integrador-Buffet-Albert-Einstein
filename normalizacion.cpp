#include <iostream>
#include <cstdio> 
#include <cstring> 
using namespace std;

const int K = 5;
const int MAX_MOZOS = 50;
const int MAX_COMANDAS = 1000;
const int MAX_PRODUCTOS = 200;

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

struct comandaHistorica {
    char fecha[11];        // "DD-MM-AAAA"
    char nombreMozo[50];   // el nombre completo, repetido en cada venta
    int codigoProducto;    
    int cantidad;    
    float comision;
};

struct ventaHistorica{
    char fecha[11];
    char nombreMozo[50];
    int codigoProducto;
    int cantidad;
    float comision;
};

struct comandaTemp {
    char fecha[11];
    Comanda datos;
};

void separarVentas(Mozo lista_de_mozos[], int cantidad_de_mozos) {
    comandaTemp lista_comandas[MAX_COMANDAS]; // todas las comandas en memoria
    int cant_comandas = 0;

    FILE* ventasHistorico = fopen("comandas_historicas.dat", "rb");
    if (ventasHistorico == NULL) {
        cout << "Error al abrir comandas_historicas.dat" << endl;
        return;
    }

    comandaHistorica v;
    while (cant_comandas < MAX_COMANDAS &&
           fread(&v, sizeof(comandaHistorica), 1, ventasHistorico) == 1) {
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

    // Ordenamos las comandas
    for (int i = 0; i < cant_comandas - 1; i++) {
        for (int j = i + 1; j < cant_comandas; j++) {
            int cmp = compararFechas(lista_comandas[j].fecha, lista_comandas[i].fecha);
            if (cmp < 0 ||
               (cmp == 0 && lista_comandas[j].datos.idMozo < lista_comandas[i].datos.idMozo)) {

                comandaTemp aux = lista_comandas[i];
                lista_comandas[i] = lista_comandas[j];
                lista_comandas[j] = aux;
            }
        }
    }

    // Corte de control para generar archivos diarios
    int i = 0;
    while (i < cant_comandas) {
        // Guardamos la fecha que rige para este grupo
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
            
            fwrite(&lista_comandas[i].datos, sizeof(Comanda), 1, archivoDia);
            i++; // Avanzamos al siguiente registro
        }
        
        // Cuando cambia la fecha, cerramos el archivo del día
        fclose(archivoDia); 
    }
    
    cout << "Ventas separadas exitosamente por dia." << endl;
}

// Corte de control para generar los archivos diarios
    int i = 0;
    int archivosGenerados = 0;
    while (i < cant_comandas) {
        // Nos guardamos la fecha que rige para este grupo
        char fechaActual[11];
        strcpy(fechaActual, lista_comandas[i].fecha);

        // Armamos el nombre del archivo: "comandas_DD-MM-AAAA.dat"
        char nombreArchivo[50];
        strcpy(nombreArchivo, "comandas_");
        strcat(nombreArchivo, fechaActual);
        strcat(nombreArchivo, ".dat");

        // Al abrir en "wb" se crea un archivo nuevo para este dia
        FILE* archivoDia = fopen(nombreArchivo, "wb");
        if (archivoDia == NULL) {
            cout << "Error al crear " << nombreArchivo << endl;
            return;
        }

        while (i < cant_comandas && strcmp(lista_comandas[i].fecha, fechaActual) == 0) {
            
            fwrite(&lista_comandas[i].datos, sizeof(Comanda), 1, archivoDia);
            i++;
        }

        // Cuando cambia la fecha, cerramos el archivo del dia
        fclose(archivoDia);
        archivosGenerados++;

         cout << "Ventas separadas por dia: " << archivosGenerados << " archivos generados." << endl;

    }

void actualizarStock() {
    // 1. Cargar el inventario en memoria
    Producto inventario[MAX_PRODUCTOS];
    int cant_inventario = 0;

    FILE* archInv = fopen("inventario.dat", "rb");
    if (archInv == NULL) {
        cout << "Error: No se pudo abrir inventario.dat." << endl;
        return;
    }
    while (cant_inventario < MAX_PRODUCTOS &&
           fread(&inventario[cant_inventario], sizeof(Producto), 1, archInv) == 1) {
        cant_inventario++;
    }
    fclose(archInv);

    FILE* ventasHistorico = fopen("comandas_historicas.dat", "rb");
    if (ventasHistorico != NULL) {
        comandaHistorica v;

        while (fread(&v, sizeof(comandaHistorica), 1, ventasHistorico) == 1) {
            // Buscamos el producto vendido en nuestro array

            for(int i = 0; i < cant_inventario; i++) {
                if(inventario[i].codigo == v.codigoProducto) {

                    // Restamos la cantidad vendida
                    inventario[i].stockActual -= v.cantidad;

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
 }

comandaHistorica v;
    while (fread(&v, sizeof(comandaHistorica), 1, ventasHistorico) == 1) {
        // Buscamos el producto vendido en el array
        for (int i = 0; i < cant_inventario; i++) {
            if (inventario[i].codigo == v.codigoProducto) {

                inventario[i].stockActual -= v.cantidad;

                // Evitar stock negativo
                if (inventario[i].stockActual < 0) {
                    inventario[i].stockActual = 0;
                }

                break; // Stock actualizado
            }
        }
    }
    fclose(ventasHistorico);

    // Sobrescribir el archivo con el stock actualizado
    archInv = fopen("inventario.dat", "wb");
    if (archInv == NULL) {
        cout << "Error: No se pudo escribir inventario.dat." << endl;
        return;
    }
    for (int i = 0; i < cant_inventario; i++) {
        fwrite(&inventario[i], sizeof(Producto), 1, archInv);
    }
    fclose(archInv);
    cout << "Stock actualizado exitosamente en inventario.dat." << endl;
    }

int main(){

    return 0;
}