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
void crearArchivoSemanal(const char* nombreArchivo, const char* nombreArchivoDiario){
    FILE* archivoSemanal = fopen(nombreArchivo, "wb");
    if (archivoSemanal == NULL) {
        cout << "Error al crear el archivo semanal." << endl;
        return;
    }
    FILE* archivoDiario = fopen(nombreArchivoDiario, "rb");
    if (archivoDiario == NULL) {
        fclose(archivoSemanal);
        return;
    }
    Comanda c;
    while (fread(&c, sizeof(Comanda), 1, archivoDiario) == 1) {
        fwrite(&c, sizeof(Comanda), 1, archivoSemanal);
    }
    fclose(archivoSemanal);
    fclose(archivoDiario);
}
int main (){
    int semana;
    int mes;
    int anio=2026;
    cout << "Ingrese la semana (1-4):";
    cin >> semana;
    cout << "Ingrese el mes (1-12):";
    cin >> mes;

    if (semana < 1 || semana > 4) {
        cout << "Error" << endl;
        return 0;
    }
    if (mes < 1 || mes > 12) {
        cout << "Error" << endl;
        return 0;
    }

char semanaTexto[10];
if (semana==1) {
    strcpy(semanaTexto, "1");
} else if (semana==2) {
    strcpy(semanaTexto, "2");
} else if (semana==3) {
    strcpy(semanaTexto, "3");
} else if (semana==4) {
    strcpy(semanaTexto, "4");
}

char mesTexto[10];
if (mes<10){
    mesTexto[0]='0';
    mesTexto[1]=mes+'0';
    mesTexto[2]='\0';
} else {
    mesTexto[0]=(mes/10)+'0';
    mesTexto[1]=(mes%10)+'0';
    mesTexto[2]='\0';
}

char nombreArchvivoSemanal[50];
strcpy(nombreArchvivoSemanal, "comandas_semana_s");

int diasDelMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int diaInicio=(semana - 1) * 7 + 1;
int diaFin;
}