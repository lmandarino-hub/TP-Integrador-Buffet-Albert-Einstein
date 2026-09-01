#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void apareo(const char* ArchivoA, const char* ArchivoB, const char* ArchivoSalida){
    FILE* a = fopen(ArchivoA, "rb");
    FILE* b = fopen(ArchivoB, "rb");
    FILE* salida = fopen(ArchivoSalida, "wb");

    if(a == NULL || b == NULL || salida == NULL) {
        return;
    }
    Comanda ca, cb;
    int la = fread(&ca, sizeof(Comanda), 1, a);
    int lb = fread(&cb, sizeof(Comanda), 1, b);

    while (la == 1 && lb == 1) {
        if (ca.idMozo < cb.idMozo) {
            fwrite(&ca, sizeof(Comanda), 1, salida);
            la = fread(&ca, sizeof(Comanda), 1, a);
        } else {
            fwrite(&cb, sizeof(Comanda), 1, salida);
            lb = fread(&cb, sizeof(Comanda), 1, b);
    }
}
    while (la == 1){
        fwrite(&ca, sizeof(Comanda), 1, salida);
        la = fread(&ca, sizeof(Comanda), 1, a);
    }
    while (lb == 1){
        fwrite(&cb, sizeof(Comanda), 1, salida);
        lb = fread(&cb, sizeof(Comanda), 1, b);
    }
    fclose (a);
    fclose (b);
    fclose(salida);
}
// verificar si existe archivo diario
bool existeArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "rb");
    if (archivo) {
        fclose(archivo);
        return true;
    }
    return false;

}
int main (){
    int semana;
    int mes;
    int anio=2026;
    cout << "Ingrese la semana (1-4):";
    cin >> semana;
    cout << "Ingrese el mes (1-12):";
    cin >> mes;

    int diaInicio;
    int diaFin;

    diaInicio = (semana - 1) * 7 + 1;
    diaFin = diaInicio + 6;
    cout << "La semana va del dia" << diaInicio << " al dia " << diaFin << endl;

}
