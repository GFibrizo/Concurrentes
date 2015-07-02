//
// Copyright 2015
// Bruno Merlo Schurmann bruno290@gmail.com
// Gaston Martinez gaston.martinez.90@gmail.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses
//

#include <iostream>
#include <string>

#include "client.h"
#include "database_record.h"

using std::cout;
using std::endl;
using std::cin;
using std::to_string;

void print_menu() {
    cout << endl;
    cout << "Cliente ConcusQL" << endl;
    cout << to_string(MENU_CREATE) << " - Agregar registro" << endl;
    cout << to_string(MENU_UPDATE) << " - Modificar registro" << endl;
    cout << to_string(MENU_RETRIEVE) << " - Consultar registro" << endl;
    cout << to_string(MENU_EXIT) << " - Salir" << endl;
}

void request_create(Client &client, DatabaseRecord &record) {
    int operation_result = client.request_create(record);
    switch (operation_result) {
        case REQUEST_SUCCESS:
            cout << "Registro agregado exitosamente" << endl;
            break;
        case REQUEST_ERROR:
            cout << "Ya existe un registro con el nombre " << record.name << endl;
            break;
    }
}

void request_update(Client &client, DatabaseRecord &record) {
    int operation_result = client.request_update(record);
    switch (operation_result) {
        case REQUEST_SUCCESS:
            cout << "Registro modificado exitosamente" << endl;
            break;
        case REQUEST_ERROR:
            cout << "No existe un registro con el nombre " << record.name << endl;
            break;
    }
}

void request_retrieve(Client &client, DatabaseRecord &record) {
    int operation_result = client.request_retrieve(record);
    switch (operation_result) {
        case REQUEST_SUCCESS:
            cout << "Registro encontrado:" << endl;
            cout << "Nombre: " << record.name << endl;
            cout << "Direccion: " << record.address << endl;
            cout << "Telefono: " << record.phone_number << endl;
            break;
        case REQUEST_ERROR:
            cout << "No existe un registro con el nombre " << record.name << endl;
            break;
    }
}

int main() {

    Client client = Client();

    if (! client.connect()) {
        cout << "Error de conexion con el servidor: servidor no disponible." << endl;
        return -1;
    }

    while (client.connected()){
        print_menu();
        int option;
        cout << "Ingrese la opcion deseada: ";
        cin >> option;
        if (! option) {
            continue;
        } else if (option == MENU_EXIT) {
            break;
        }
        DatabaseRecord record;
        cout << "Ingrese el nombre: ";
        cin >> record.name;
        switch (option) {
            case MENU_CREATE:
                cout << "Ingrese la direccion: ";
                cin >> record.address;
                cout << "Ingrese el numero de telefono: ";
                cin >> record.phone_number;
                request_create(client, record);
                break;
            case MENU_UPDATE:
                cout << "Ingrese la nueva direccion: ";
                cin >> record.address;
                cout << "Ingrese el nuevo numero de telefono: ";
                cin >> record.phone_number;
                request_update(client, record);
                break;
            case MENU_RETRIEVE:
                request_retrieve(client, record);
                break;
            default:
                cin.clear();
                cin.ignore(INTMAX_MAX, '\n');
                cout << "La opción elegida no es correcta, ingrese nuevamente." << endl;
                break;
        }
        cout << endl;
    }

    if (client.connected()) {
        client.disconnect();
    } else {
        cout << "Error en la comunicacion con el servidor. Se cerrara el programa." << endl;
    }

    return 0;
}