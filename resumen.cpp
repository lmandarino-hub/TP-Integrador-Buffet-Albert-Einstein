#include <iostream>
#include <cstdio>
using namespace std;
 
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};
 
FILE* leerarchivo(){

    char NombreArchivo[100];
    cout << "nombre del archivo: ";
    cin >> NombreArchivo;
 
    FILE* comandas_historicas = fopen(NombreArchivo, "rb");
    if (comandas_historicas == NULL) {
        cout << "Error: no se pudo abrir el archivo " << NombreArchivo << endl;  
    }
    return comandas_historicas;
}

void imprimirMozo(int idMozo, int cantidadmozo, float comisionmozo) {
    cout << idMozo << "\t" << cantidadmozo << "\t\t" << comisionmozo << endl;
    
}

void Cortecontrol(FILE*comando_historicas,bool& HayDatos,int& MozoActual,int& CantMozo,float& MozoComision, int& TotalBuffet){

    Comanda comando;

    while (fread(&comando, sizeof(Comanda), 1, comando_historicas) == 1) {
        if (!HayDatos) {
      
            HayDatos = true;
            MozoActual = comando.idMozo;
            cout << endl << "===== RESUMEN DEL CIERRE =====" << endl;
            cout << "Mozo\tProductos\tComision" << endl;
        }
        else if (comando.idMozo != MozoActual) {
      
            imprimirMozo(MozoActual, CantMozo, MozoComision);
            TotalBuffet += CantMozo;
            MozoActual = comando.idMozo;
            CantMozo = 0;
            MozoComision = 0;
        }
        CantMozo += comando.cantidad;
        MozoComision += comando.comision;
    }
    if (HayDatos) {
        imprimirMozo(MozoActual, CantMozo, MozoComision);
        TotalBuffet += CantMozo;
    }
    fclose(comando_historicas);
 
}

int main() {   

FILE* comandas_historicas = leerarchivo();

    if (comandas_historicas == NULL) {
        return 1;
    }

    bool hayDatos = false;    
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