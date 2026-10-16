#include <iostream>
#include <fstream>

using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

int main() {

    ifstream archivo("comandas_semana_1-5.dat", ios::binary);

    if (!archivo) {
        cout << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    Comanda venta;

    while (archivo.read((char*)&venta, sizeof(Comanda))) {

        cout << "Mozo: " << venta.idMozo << endl;
        cout << "Producto: " << venta.codigoProducto << endl;
        cout << "Cantidad: " << venta.cantidad << endl;
        cout << "Comision: " << venta.comision << endl;
        cout << "------------------" << endl;
    }

    archivo.close();

    return 0;
}
