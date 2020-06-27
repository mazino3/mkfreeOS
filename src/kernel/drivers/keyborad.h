/*Copyright (C) 2019  Ramón Mayedo Morales (ramonmayedo@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef MAPKEYBORAD_H
#define MAPKEYBORAD_H
#include "../core/process.h"

class Ckeyboard {
public:
    Ckeyboard();
    void initialize();
    void IRQ();
    int command(int acommand, int parameter1, int parameter2);
    int getState();
private:
    u32 state;
    Cprocess *process;
    Cthread *thread;
    char buffer[32];
    u8 ptr;
    u8 count;
protected:
    int block();
    int unBlock();
};

#endif

