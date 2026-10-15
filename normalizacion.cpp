#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int K = 5;
const int MAX_MOZOS = 50;
const int MAX_COMANDAS = 1000;
const int MAX_PRODUCTOS = 200;

const char* PALABRAS[] = {"sol", "luna", "mar", "rio", "cielo", "monte", "flor", "nube"};
const int CANT_PALABRAS = 8;


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

// Busqueda secuencial del mozo
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

// Compara fechas con formato DD-MM-AAAA (devuelve <0, 0 o >0)
int compararFechas(const char a[], const char b[]){
    int r = strncmp(a + 6, b + 6, 4);   // anio
    if (r != 0) return r;
    r = strncmp(a + 3, b + 3, 2);       // mes
    if (r != 0) return r;
    return strncmp(a, b, 2);            // dia
}

void generarMozosDat(Mozo lista_de_mozos[], int &cantidad_de_mozos){
    cantidad_de_mozos = 0;

    if (sizeof(ComandaHistorica) != 76){
        cout << "Aviso: sizeof(ComandaHistorica) = " << sizeof(ComandaHistorica)
             << " y deberia ser 76. Revisa los campos del struct." << endl;
    }

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
            // le asignamos id
            int id = cantidad_de_mozos + 1;

            // clave inicial: una palabra segun el id
            char clave[20];
            if (id <= CANT_PALABRAS) {
                strcpy(clave, PALABRAS[id - 1]);
            } else {
                // si hubiera mas mozos que palabras, repetimos palabra y le sumamos el id
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

void separarVentas(Mozo lista_de_mozos[], int cantidad_de_mozos) {
    ComandaTemp lista_comandas[MAX_COMANDAS]; // todas las comandas en memoria
    int cant_comandas = 0;

    FILE* ventasHistorico = fopen("comandas_historicas.dat", "rb");
    if (ventasHistorico == NULL) {
        cout << "Error al abrir comandas_historicas.dat" << endl;
        return;
    }

    ComandaHistorica v;
    while (cant_comandas < MAX_COMANDAS &&
           fread(&v, sizeof(ComandaHistorica), 1, ventasHistorico) == 1) {
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

    // Ordenamos las comandas por fecha y, dentro de la fecha, por idMozo
    for (int i = 0; i < cant_comandas - 1; i++) {
        for (int j = i + 1; j < cant_comandas; j++) {
            int cmp = compararFechas(lista_comandas[j].fecha, lista_comandas[i].fecha);
            if (cmp < 0 ||
               (cmp == 0 && lista_comandas[j].datos.idMozo < lista_comandas[i].datos.idMozo)) {

                ComandaTemp aux = lista_comandas[i];
                lista_comandas[i] = lista_comandas[j];
                lista_comandas[j] = aux;
            }
        }
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

        // Mientras la fecha siga siendo la misma (corte de control)
        while (i < cant_comandas && strcmp(lista_comandas[i].fecha, fechaActual) == 0) {
            fwrite(&lista_comandas[i].datos, sizeof(Comanda), 1, archivoDia);
            i++;
        }

        // Cuando cambia la fecha, cerramos el archivo del dia
        fclose(archivoDia);
        archivosGenerados++;
    }

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

    // 2. Restar las cantidades vendidas
    FILE* ventasHistorico = fopen("comandas_historicas.dat", "rb");
    if (ventasHistorico == NULL) {
        cout << "Error: No se pudo abrir comandas_historicas.dat" << endl;
        return;
    }

    ComandaHistorica v;
    while (fread(&v, sizeof(ComandaHistorica), 1, ventasHistorico) == 1) {
        // Buscamos el producto vendido en el array
        for (int i = 0; i < cant_inventario; i++) {
            if (inventario[i].codigo == v.codigoProducto) {
                inventario[i].stockActual -= v.cantidad;

                // Evitar stock negativo
                if (inventario[i].stockActual < 0) {
                    inventario[i].stockActual = 0;
                }

                break; // Producto actualizado, cortamos el for
            }
        }
    }
    fclose(ventasHistorico);

    // 3. Sobrescribir el archivo con el stock actualizado
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
    Mozo lista_de_mozos[MAX_MOZOS];
    int cantidad_de_mozos = 0;

    generarMozosDat(lista_de_mozos, cantidad_de_mozos);       // 1. mozos.dat
    if (cantidad_de_mozos > 0) {
        mostrarMozosYComisiones();
        separarVentas(lista_de_mozos, cantidad_de_mozos);     // 2. comandas_DD-MM-AAAA.dat
        actualizarStock();                                    
    }

    return 0;
}