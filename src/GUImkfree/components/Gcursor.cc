/*Copyright (C) 2019  Ram�n Mayedo Morales (ramonmayedo@gmail.com)

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

#include "Gcursor.h"

Gcursor::Gcursor() {

}

Gcursor::Gcursor(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
  repaint();
}

void Gcursor::repaint() {
    if (graphics) delete(graphics);
    graphics = new Gframe(0, 0, 16, 16);
    graphics->backGroundColor(Gcolor(0x0));
}


Gcursor::~Gcursor() {
}

