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

int CopiarTexto(char destino[], int pos, const char* texto){
    int i = 0;
    while (texto[i] != '\0') {
        destino[pos] = texto[i];
        i++;
        pos++;
    }
    return pos;
}

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

char nombreArchivoSemanal[50];
int pos=0;

pos=CopiarTexto(nombreArchivoSemanal, pos, "comandas_semanas_s");
pos=CopiarTexto(nombreArchivoSemanal, pos, semanaTexto);
pos=CopiarTexto(nombreArchivoSemanal, pos, "_");
pos=CopiarTexto(nombreArchivoSemanal, pos, mesTexto);
pos=CopiarTexto(nombreArchivoSemanal, pos, ".dat");
nombreArchivoSemanal[pos]='\0';

int diasDelMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int diaInicio=(semana - 1) * 7 + 1;
int diaFin;
if (semana ==4){
    diaFin=diasDelMes[mes];
}else{
    diaFin=semana*7;
}
char temp_1[50]="temp_1.dat";
char temp_2[50]="temp_2.dat";
char archivoAnterior[50];
int cantArchivos=0;
int temporal=1;

char diaTexto[3];
for (int dia=diaInicio; dia<=diaFin; dia++){
    if (dia<10){
        diaTexto[0]='0';
        diaTexto[1]=dia+'0';
        diaTexto[2]='\0';
    } else {
        diaTexto[0]=(dia/10)+'0';
        diaTexto[1]=(dia%10)+'0';
        diaTexto[2]='\0';
    }
}

char archivoDiario[50];
int p=0;
p = CopiarTexto(archivoDiario, p, "comandas_");
p = CopiarTexto(archivoDiario, p, diaTexto);
p = CopiarTexto(archivoDiario, p, "_");
p = CopiarTexto(archivoDiario, p, mesTexto);
p= CopiarTexto(archivoDiario, p , "2026.dat");
archivoDiario[p]='\0';

if (existeArchivo(archivoDiario)){
    cout << "Archivo diario encontrado: " << archivoDiario << endl;

if (cantArchivos == 0){
    strcpy(archivoAnterior, archivoDiario);
}else{
    if (temporal==1){
        apareo(archivoAnterior, archivoDiario, temp_1);
        strcpy(archivoAnterior, temp_1);
        temporal=2;
    }else {
        apareo(archivoAnterior, archivoDiario, temp_2);
        strcpy(archivoAnterior, temp_2);
        temporal=1;
    }
}
cantArchivos++;
}
if (cantArchivos > 0){
    crearArchivoSemanal(nombreArchivoSemanal, archivoAnterior);
    cout <<"Archivo creado" << nombreArchivoSemanal << endl;
}else{
    cout << "No se encontraron archivos diarios" << endl;
}

return 0;
}