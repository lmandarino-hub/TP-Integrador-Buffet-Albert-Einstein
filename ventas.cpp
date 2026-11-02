#include <iostream>
#include <cstdio>
using namespace std;
 
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};
 
FILE* leerArchivo() {
    char nombreArchivo[100];
    cout << "Nombre del archivo: ";
    cin >> nombreArchivo;
 
    FILE* f = fopen(nombreArchivo, "rb");   
    if (f == NULL) {
        cout << "Error: no se pudo abrir el archivo " << nombreArchivo << endl;
    }
    return f;                             
}
 
void imprimirMozo(int idMozo, int cantidad, float comision) {
    cout << idMozo << "\t" << cantidad << "\t\t" << comision << endl;
}
 
void corteControl(FILE* f, bool& hayDatos, int& mozoActual, int& cantMozo,
                  float& comisionMozo, int& totalBuffet) {
    Comanda c;
 
    while (fread(&c, sizeof(Comanda), 1, f) == 1) {
        if (!hayDatos) {
            hayDatos = true;
            mozoActual = c.idMozo;
            cout << endl << "===== RESUMEN DEL CIERRE =====" << endl;
            cout << "Mozo\tProductos\tComision" << endl;
        }
        else if (c.idMozo != mozoActual) {
            imprimirMozo(mozoActual, cantMozo, comisionMozo);
            totalBuffet += cantMozo;
            mozoActual = c.idMozo;
            cantMozo = 0;
            comisionMozo = 0;
        }
        cantMozo += c.cantidad;
        comisionMozo += c.comision;
    }
}
 
int main() {
    FILE* f = leerArchivo();
    if (f == NULL) {
        return 1;
    }
 
    bool hayDatos = false;
    int mozoActual = 0;
    int cantMozo = 0;
    float comisionMozo = 0;
    int totalBuffet = 0;
 
    corteControl(f, hayDatos, mozoActual, cantMozo, comisionMozo, totalBuffet);
    fclose(f);
 
    if (!hayDatos) {
        cout << "El archivo no tiene ventas." << endl;
        return 0;
    }
 
    imprimirMozo(mozoActual, cantMozo, comisionMozo);
    totalBuffet += cantMozo;
 
    cout << "------------------------------" << endl;
    cout << "TOTAL de productos vendidos en la semana: " << totalBuffet << endl;
 
    return 0;
}