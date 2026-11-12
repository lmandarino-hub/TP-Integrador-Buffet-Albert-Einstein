#include <iostream>
#include <cstdio>
using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

FILE* leerarchivo() {
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

void Cortecontrol(FILE* comandas_historicas, bool& HayDatos, int& TotalBuffet) {
    Comanda comando;

    // lectura anticipada: leido vale 1 si hay registro, 0 si terminó el archivo
    int leido = fread(&comando, sizeof(Comanda), 1, comandas_historicas);

    if (leido == 1) {
        HayDatos = true;
        cout << endl << "===== RESUMEN DEL CIERRE =====" << endl;
        cout << "Mozo\tProductos\tComision" << endl;
    }

    while (leido == 1) {
        int MozoActual = comando.idMozo;   // clave de control del grupo
        int CantMozo = 0;                  // acumuladores del grupo
        float MozoComision = 0;

        // el grupo sigue mientras haya registro Y sea el mismo mozo
        while (leido == 1 && comando.idMozo == MozoActual) {
            CantMozo += comando.cantidad;
            MozoComision += comando.comision;
            leido = fread(&comando, sizeof(Comanda), 1, comandas_historicas);
        }

        // cierre del grupo (también se ejecuta para el último mozo)
        imprimirMozo(MozoActual, CantMozo, MozoComision);
        TotalBuffet += CantMozo;
    }

    fclose(comandas_historicas);
}

int main() {
    FILE* comandas_historicas = leerarchivo();

    if (comandas_historicas == NULL) {
        return 1;
    }

    bool hayDatos = false;
    int totalBuffet = 0;

    Cortecontrol(comandas_historicas, hayDatos, totalBuffet);

    if (!hayDatos) {
        cout << "El archivo no tiene ventas." << endl;
        return 1;
    }

    cout << "------------------------------" << endl;
    cout << "TOTAL de productos vendidos en la semana: " << totalBuffet << endl;

    return 0;
}