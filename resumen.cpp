#include <iostream>
#include <cstdio>
using namespace std;
 
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};
 
FILE*leerarchivo(){

    char NombreArchivo[100];
    cout << "nombre del archivo";
    cin >> NombreArchivo;
 
    FILE*comandas_historicas = fopen(NombreArchivo, "rb");
    if (comandas_historicas == NULL) {
        cout << "Error: no se pudo abrir el archivo " << NombreArchivo << endl;  
    }
}

void imprimirMozo(int idMozo, int cantidad, float comision) {
    cout << idMozo << "\t" << cantidad << "\t\t" << comision << endl;
    
}

void Cortecontrol(Comanda comanda,FILE*historicas,bool& Datos,int& Actual,int& cant,float& comision, int& Buffet){


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
        }
        cant += comanda.cantidad;
        comision += comanda.comision;
    }
 
    fclose(historicas);
 
}

int main() {   

FILE* comandas_historicas = leerarchivo();

    if (comandas_historicas == NULL) {
        return 1;
    }

    Comanda c;
    bool hayDatos = false;    
    int mozoActual = 0;
    int cantMozo = 0;
    float comisionMozo = 0;
    int totalBuffet = 0;

Cortecontrol(c,comandas_historicas,hayDatos,mozoActual,cantMozo,comisionMozo,totalBuffet);

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
