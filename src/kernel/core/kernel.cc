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

#include "kernel.h"
#include "../architecture/x86/x86.h"
extern Sx86 x86;
extern Score core;
extern Smaps maps;

void kernel() {
    core.graphics.installDevice();  //Se monta el dispositivo de video
    x86.ioScreen.printf("Ya estamos en el kernel MKFREE h:%i m: %i s: %i /n", maps.clockMap.hour, maps.clockMap.minute, maps.clockMap.second);
    x86.ioScreen.printf("Ya estamos en el kernel MKFREE y:%i m: %i d: %i /n", maps.clockMap.year, maps.clockMap.month, maps.clockMap.day);
       
    core.devices.installDevices();                          //Se montan los dispositivos de almacenamiento
    for (int i = 0; i < core.devices.getCountDevice(); i++)
        core.diskPartition.mountFileSystem(i);               //Se montan los sistemas de archivo
        
    core.fileSystem.selectVolume('a');
    SinfoMedia *infoMedia = core.fileSystem.getInfoVolume();
    x86.ioScreen.printf("label=%s /n", infoMedia->labelVolume);
    x86.ioScreen.printf("type=%s /n", infoMedia->labelType);
    x86.ioScreen.printf("oem=%s /n", infoMedia->labelOem);
    x86.ioScreen.printf("size=%i MB /n", infoMedia->size / 1048576);
  
    Cfile *newfile;
    char *buffer;  
    char **arg;
     
    newfile = new Cfile("a/kernel/program/init.elf", FILE_RW);
    
    buffer = new char[newfile->getSize()];
    newfile->readAll(buffer);
         
    int error = core.adminProcess.addReady(buffer, 0, arg);
    delete(buffer);
    delete(newfile);
    if (error) {
        core.pioscreen.connectScreen();
        core.adminProcess.schelude();
    }else
        x86.ioScreen.printf("Error no se pudo cargar el proceso init");
    while (1);
}