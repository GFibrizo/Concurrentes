/**
 concu_zappi.cpp

 Copyright 2015 Gaston Martinez Gaston.martinez.90@gmail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "logger.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::stringstream;

static const string FILENAME = "concu_zappi.cpp";

int main(int argc, char** argv) {
	Logger::open_logger("run_log.log"); //TODO: Agregar opccion para sobreescribir

	Logger::log(FILENAME, Logger::INFO, "Configuracion inicial");

	ifstream file;
	file.open("config.cfg", std::fstream::in);

	while (file.good()) {
		int amount = 0;
		string concept;
		stringstream output;

		file >> amount >> concept;

		output << concept << " cantidad: " << amount;

	}

	Logger::log(FILENAME, Logger::INFO, "Inicia recepcion de pedidos");

	Logger::log(FILENAME, Logger::INFO, "Cerrada recepcion de pedidos");
}
