/*Copyright (C) 2019  Ramón Mayedo Morales (ramonmayedo@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABIL    ITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "devices.h"
#include "../architecture/x86/x86.h"

extern Smaps maps;
extern Sx86 x86;
extern Score core;

Cdevices::Cdevices() {
}

int Cdevices::installDevices() {
    countDevices = 0;
    diskIde.chanel0.regBase = 0x1F0;
    diskIde.chanel0.regControl = 0x3F6;
    diskIde.chanel1.regBase = 0x170;
    diskIde.chanel1.regControl = 0x376;
    readDevices(&diskIde.chanel0, 0);
    readDevices(&diskIde.chanel0, 1);
    readDevices(&diskIde.chanel1, 0);
    readDevices(&diskIde.chanel1, 1);
}

u8 Cdevices::readDevices(SdevicePort *device, u8 devIndex) {
    resetBus(device);
    x86.port.outb(device->regBase + 0x06, 0xA0 | (devIndex << 4)); //Selecciono el dispositivo
    delay400ns(device);                                 //400ns para que se seleccione el driver
    x86.port.outb(device->regBase + 0x3, 0);
    x86.port.outb(device->regBase + 0x4, 0);
    x86.port.outb(device->regBase + 0x5, 0);
    x86.port.outb(device->regBase + 0x7, 0xEC); //
    u8 existDevice = x86.port.inb(device->regBase + 0x07);
    u8 typeDevice = DEV_ATA_UNKNOWN;

    if (existDevice != 0) {
        x86.port.outb(device->regBase + 0x06, 0xA0 | (1 << devIndex));
        delay400ns(device); //400ns para que se seleccione el driver
        
        u8 cl = x86.port.inb(device->regBase + 0x04);
        u8 ch = x86.port.inb(device->regBase + 0x05);

        if (cl == 0x14 && ch == 0xEB) typeDevice = DEV_ATA_PATAPI;        //CD-ROOM
        else if (cl == 0x69 && ch == 0x96) typeDevice = DEV_ATA_SATAPI;   //;
        else if (cl == 0 && ch == 0) typeDevice = DEV_ATA_PATA;           //;
        else if (cl == 0x3c && ch == 0xc3) typeDevice = DEV_ATA_SATA;     //;

        if (typeDevice != 0) {
            devices[countDevices].did = countDevices;
            devices[countDevices].flags = 0x0;
            devices[countDevices].port = device;
            devices[countDevices].type = typeDevice;
            devices[countDevices].index = devIndex;
            countDevices++;
            x86.ioScreen.printf("Dispositivo Encontrado Canal %i : Index %i : Tipo %i /n", device->regBase, devIndex, typeDevice);
        }
    }
    return typeDevice;
}

Sdevice *Cdevices::getDevice(int did) {
    if (did >= 10) return 0;
    return &devices[did];
}

int Cdevices::getCountDevice() {
    return countDevices;
}

int Cdevices::readDevice(u32 did, u32 ablock, u32 countSector, char *abuffer) {
    switch (devices[did].type) {
        case DEV_ATA_PATA:   //PATA Discos Duros
        {
            return core.diskide.readSector(devices[did].port, devices[did].index, ablock, countSector, abuffer);
        }
        case DEV_ATA_PATAPI:   //PATA CDROM
        {
            return core.cdide.readSector(devices[did].port, devices[did].index, ablock, countSector, abuffer);
        }

        default:break;
    }
    return 0;
}

int Cdevices::writeDevice(u32 did, u32 ablock, u32 countSector, char *abuffer) {
    switch (devices[did].type) {
        case DEV_ATA_PATA:
        {
            return core.diskide.writeSector(devices[did].port, devices[did].index, ablock, countSector, abuffer);
        }
        case DEV_ATA_PATAPI:
        {
            return 0;
        }
        default:break;
    }
    return 0;
}

void Cdevices::resetBus(SdevicePort *device) {
    x86.port.outb(device->regControl, 0x04);
    delay400ns(device);
    x86.port.outb(device->regControl, 0x0);
}

void Cdevices::delay400ns(SdevicePort *device) {
    for (int i = 0; i < 4; i++)
        x86.port.inb(device->regControl); //400ns  
}