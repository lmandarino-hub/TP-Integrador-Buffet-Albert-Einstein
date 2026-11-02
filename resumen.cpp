#include <iostream>
#include <cstdio>
using namespace std;
 
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};
 
<<<<<<< HEAD
FILE* leerarchivo(){
=======
FILEleerarchivo(){
>>>>>>> 3c13d7eb338c2e9ebfabbb52fd7d5de482927db2

    char NombreArchivo[100];
    cout << "nombre del archivo: ";
    cin >> NombreArchivo;
 
<<<<<<< HEAD
    FILE* comandas_historicas = fopen(NombreArchivo, "rb");
=======
    FILEcomandas_historicas = fopen(NombreArchivo, "rb");
>>>>>>> 3c13d7eb338c2e9ebfabbb52fd7d5de482927db2
    if (comandas_historicas == NULL) {
        cout << "Error: no se pudo abrir el archivo " << NombreArchivo << endl;
    }
<<<<<<< HEAD
    return comandas_historicas;
}

void imprimirMozo(int idMozo, int cantidadmozo, float comisionmozo) {
    cout << idMozo << "\t" << cantidadmozo << "\t\t" << comisionmozo << endl;
    
}

void Cortecontrol(FILE*comando_historicas,bool& HayDatos,int& MozoActual,int& CantMozo,float& MozoComision, int& TotalBuffet){
=======
     return comandas_historicas;
}


void imprimirMozo(int idMozo, int cantidad, float comision) {
    cout << idMozo << "\t" << cantidad << "\t\t" << comision << endl;

}
void Cortecontrol(Comanda comanda,FILEhistoricas,bool& Datos,int& Actual,int& cant,float& comision, int& Buffet){
>>>>>>> 3c13d7eb338c2e9ebfabbb52fd7d5de482927db2

    Comanda comando;

<<<<<<< HEAD
    while (fread(&comando, sizeof(Comanda), 1, comando_historicas) == 1) {
        if (!HayDatos) {
      
            HayDatos = true;
            MozoActual = comando.idMozo;
            cout << endl << "===== RESUMEN DEL CIERRE =====" << endl;
            cout << "Mozo\tProductos\tComision" << endl;
        }
        else if (comando.idMozo != MozoActual || HayDatos) {
      
            imprimirMozo(MozoActual, CantMozo, MozoComision);
            TotalBuffet += CantMozo;
            MozoActual = comando.idMozo;
            CantMozo = 0;
            MozoComision = 0;
=======
    while (fread(&comanda, sizeof(Comanda), 1, historicas) == 1) {
        if (!Datos) {

            Datos = true;
            Actual = comanda.idMozo;
            cout << endl << "===== RESUMEN DEL CIERRE =====" << endl;
            cout << "Mozo\tProductos\tComision" << endl;
        }
        else if (comanda.idMozo != Actual) {

            imprimirMozo(Actual, cant, comision);
            Buffet += cant;
            Actual = comanda.idMozo;
            cant = 0;
            comision = 0;
>>>>>>> 3c13d7eb338c2e9ebfabbb52fd7d5de482927db2
        }
        CantMozo += comando.cantidad;
        MozoComision += comando.comision;
    }
<<<<<<< HEAD
    
    fclose(comando_historicas);
 
=======
 
    fclose(historicas);´
>>>>>>> 3c13d7eb338c2e9ebfabbb52fd7d5de482927db2
}

int main() {

FILE comandas_historicas = leerarchivo();

    if (comandas_historicas == NULL) {
        return 1;
    }
<<<<<<< HEAD
    bool hayDatos = false;    
=======

    Comanda c;
    bool hayDatos = false;
>>>>>>> 3c13d7eb338c2e9ebfabbb52fd7d5de482927db2
    int mozoActual = 0;
    int cantMozo = 0;
    float comisionMozo = 0;
    int totalBuffet = 0;

    Cortecontrol(comandas_historicas,hayDatos,mozoActual,cantMozo,comisionMozo,totalBuffet);

    if (!hayDatos) {
        cout << "El archivo no tiene ventas." << endl;
        return 1;
    }

    cout << "------------------------------" << endl;
    cout << "TOTAL de productos vendidos en la semana: " << totalBuffet << endl;
 
    return 0;
}