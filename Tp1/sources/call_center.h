/**
 call_center.h

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

#ifndef CALL_CENTER_H_
#define CALL_CENTER_H_

#include <stddef.h>
#include <string>

#include "lock_file.h"

class Call_Center {

private:
	int recepcionist = 0;

private:
	void accept_call(std::string);
	void simulate_call(std::string);

public:
	Call_Center(size_t recepcionists);
	void accept_calls(); //TODO: Add pipe

	virtual ~Call_Center();
};

#endif /* CALL_CENTER_H_ */
