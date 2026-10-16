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
    return i;
}

int compararFechas(const char a[], const char b[]){
    int r = strncmp(a + 6, b + 6, 4);   // año
    if (r != 0) return r;
    r = strncmp(a + 3, b + 3, 2);       // mes
    if (r != 0) return r;
    return strncmp(a, b, 2);            // dia
}

void generarMozosDat(Mozo lista_de_mozos[], int &cantidad_de_mozos){
    cantidad_de_mozos = 0;

    FILE* archivoComandas = fopen("comandas_historicas.dat", "rb");
    if (archivoComandas == NULL){
        cout << "Error: No se pudo abrir comandas_historicas.dat" << endl;
        return;
    }

    ComandaHistorica c;

    while (fread(&c, sizeof(ComandaHistorica), 1, archivoComandas) == 1){
        int pos = buscarMozoPorNombre(lista_de_mozos, cantidad_de_mozos, c.nombreMozo);

        if (pos != -1){
            // acumulamos su comision
            lista_de_mozos[pos].totalComision += c.comision;
        } else if (cantidad_de_mozos < MAX_MOZOS) {
            // le asignamos un id a cada mozo
            int id = cantidad_de_mozos + 1;

            char clave[20];
            if (id <= CANT_PALABRAS) {
                strcpy(clave, PALABRAS[id - 1]);
            } else {
                sprintf(clave, "%s%d", PALABRAS[(id - 1) % CANT_PALABRAS], id);
            }
            cout << "ID: " << id << " | " << c.nombreMozo
                 << " | clave para ingresar: " << clave << endl;
            

            Mozo& nuevo = lista_de_mozos[cantidad_de_mozos];
            nuevo.idMozo = id;
            int i = 0;
            while (c.nombreMozo[i] != '\0' && i < (int)sizeof(nuevo.Nombre) - 1) {
                nuevo.Nombre[i] = c.nombreMozo[i];
                i++;
            }
            nuevo.Nombre[i] = '\0';
            int j = 0;
            while (clave[j] != '\0' && j < (int)sizeof(nuevo.password) - 1) {
                nuevo.password[j] = clave[j];
                j++;
            }
            nuevo.password[j] = '\0';

            nuevo.totalComision = c.comision;
            cantidad_de_mozos++;
        }
    }
    fclose(archivoComandas);

    if (cantidad_de_mozos == 0) {
        cout << "No se encontraron registros en comandas_historicas.dat" << endl;
        return;
    }

    ordenarPorID(lista_de_mozos, cantidad_de_mozos);

    FILE* mozos_terminado = fopen("mozos.dat", "wb");
    if (mozos_terminado == NULL){
        cout << "Error al generar/escribir mozos.dat" << endl;
        return;
    }
    for (int i = 0; i < cantidad_de_mozos; i++){
         CifrarClave(lista_de_mozos[i].password);
        fwrite(&lista_de_mozos[i], sizeof(Mozo), 1, mozos_terminado);
    }
    fclose(mozos_terminado);

    cout << endl << "El archivo mozos.dat generado y ordenado por ID con exito." << endl;
}

// Muestra por pantalla el contenido
void mostrarMozosYComisiones() {
    FILE* archivo = fopen("mozos.dat", "rb");
    if (archivo == NULL) {
        cout << "No se pudo leer mozos.dat" << endl;
        return;
    }

    Mozo m;
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << endl << "--- LISTADO DE MOZOS Y COMISIONES ---" << endl;
    while (fread(&m, sizeof(Mozo), 1, archivo) == 1) {
        cout << "ID: " << m.idMozo
             << " |Nombre: " << m.Nombre
             << " |Comision Total: $" << m.totalComision << endl;
    }
    fclose(archivo);
}

