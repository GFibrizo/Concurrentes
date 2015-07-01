//
// main_client
//
// Copyright 2015 Bruno Merlo Schurmann bruno290@gmail.com 
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

#include "client.h"
#include "database_record.h"

using std::cout;
using std::endl;
using std::cin;

void print_menu() {
    cout << "Menu pijudo" << endl;
}

void request_create(Client &client) {
    DatabaseRecord record;
    //TODO llenar record
    int operation_result = client.request_create(record);
    switch (operation_result) {
        case REQUEST_SUCCESS:
            cout << "Registro agregado exitosamente" << endl;
        case REQUEST_ERROR:
            cout << "Ya existe un registro con el nombre " << record.name << endl;
        case SERVER_ERROR:
            cout << "Error en la comunicacion con el servidor" << endl;
    }
}

void request_update(Client &client) {
    DatabaseRecord record;
    //TODO llenar record
    int operation_result = client.request_update(record);
    switch (operation_result) {
        case REQUEST_SUCCESS:
            cout << "Registro modificado exitosamente" << endl;
        case REQUEST_ERROR:
            cout << "No existe un registro con el nombre " << record.name << endl;
        case SERVER_ERROR:
            cout << "Error en la comunicacion con el servidor" << endl;
    }
}

void request_retrieve(Client &client) {
    DatabaseRecord record;
    //TODO llenar record
    int operation_result = client.request_retrieve(record);
    switch (operation_result) {
        case REQUEST_SUCCESS:
            cout << "Registro encontrado:" << endl;
            cout << "Nombre: " << record.name << endl;
            cout << "Direccion: " << record.address << endl;
            cout << "Telefono: " << record.phone_number << endl;
        case REQUEST_ERROR:
            cout << "No existe un registro con el nombre " << record.name << endl;
        case SERVER_ERROR:
            cout << "Error en la comunicacion con el servidor" << endl;
    }
}

int main() {

    Client client = Client();

    while (client.connected()){
        print_menu();
        int option;
        cin >> option;
        switch (option) {
            case MENU_CREATE:
                request_create(client);
            case MENU_UPDATE:
                request_update(client);
            case MENU_RETRIEVE:
                request_retrieve(client);
            case MENU_EXIT:
                client.disconnect();
            default:
                cout << "La opción elegida no es correcta, ingrese nuevamente." << endl << endl;
        }
        cout << endl;
    }

    return 0;
}