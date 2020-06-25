typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct SdevicePort {
    u32 regBase;
    u32 regControl;
};

void outb(u32 addr, u8 value);
u8 inb(u32 addr);
u16 inw(u32 addr);
void outw(u32 addr, u16 value);
int waitDisk(SdevicePort *device);
void initCommand(SdevicePort *device, u8 driveIndex);
int readSector(SdevicePort *device, u8 driveIndex, u32 ablock, u32 asectorCount, char *abuffer);
void delay400ns(SdevicePort *device);
void resetBus(SdevicePort *device);
int installDevices();

int _main(int deviceId) {

  /*  char *ptrD = (char*) 0xB8000;
    char nuevo;
    nuevo = ((deviceId & 0xF0) >> 4) + 0x30;
    ptrD[0] = nuevo;
    nuevo = (deviceId & 0x0F) + 0x30;
    ptrD[2] = nuevo;*/
    installDevices();
        while(1);
    
}
void outb(u32 addr, u8 value) {
    asm("movb %0,%%al" : "=m"(value));
    asm("mov %0,%%dx" : "=m"(addr));
    asm("outb %al, %dx");
}

void outw(u32 addr, u16 value) {
    asm("movw %0,%%ax" : "=m"(value));
    asm("mov %0,%%dx" : "=m"(addr));
    asm("outw %ax, %dx");
}

u8 inb(u32 addr) {
    u8 value;
    asm("mov %0,%%dx" : "=m"(addr));
    asm("inb %dx,%al");
    asm("movb %%al,%0" : "=m"(value));
    return value;
}

u16 inw(u32 addr){
  u16 value; 
  asm("mov %0,%%dx" :"=m"(addr));   
  asm("inw %dx, %ax");
  asm("movw %%ax,%0" :"=m"(value)); 
  return value;   
}

int waitDisk(SdevicePort *device) {
    while (inb(device->regControl) & 0x80);  
   return 0;
}

void initCommand(SdevicePort *device, u8 driveIndex) {
    waitDisk(device);
    u8 mask = (u8) (driveIndex & (1 << 4));
    outb(device->regBase + 0x6, mask);
    delay400ns(device);
    outb(device->regBase + 0x1, 0x0);
    outb(device->regBase + 0x4, (u8) (2048 & 0xFF));
    outb(device->regBase + 0x5, (u8) (2048 >> 8));
}

int readSector(SdevicePort *device, u8 driveIndex, u32 ablock, u32 asectorCount, char *abuffer) {
    u8 readCmd[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    initCommand(device, driveIndex);
    outb(device->regBase + 0x7, 0xA0);
    int status;
    while ((status = inb(device->regBase + 0x7))&0x80);
    
    while (!((status = inb(device->regBase + 0x7))&0x08)&&!(status & 0x1));
    
    if (status & 0x1){
       char *ptrD = (char*) 0xB8000;
      ptrD[1] = 0x39;
    }
     
    readCmd[9] = 0x1; //Cantidad de Sectores
    readCmd[2] = u8((ablock >> 24) & 0xFF); //Byte mas significativo LBA
    readCmd[3] = u8((ablock >> 16) & 0xFF);
    readCmd[4] = u8((ablock >> 8) & 0xFF);
    readCmd[5] = u8(ablock & 0xFF); //Byte menos significativo LBA

    u16 *val = (u16*) & readCmd;
    for (int i = 0; i < 6; i++) outw(device->regBase, val[i]);

    int size = (int(inb(device->regBase + 0x5) << 8) | int(inb(device->regBase + 0x4)));
    
   if (size != 2048){
       char *ptrD = (char*) 0xB8000;
      ptrD[1] = 0x39;
      return 0;
   }
       

    u16 auxWord;
    for (int i = 0; i < size / 2; i++) {
        auxWord = inw(device->regBase);
        abuffer[i * 2] = (u8) auxWord;
        abuffer[i * 2 + 1] = (u8) (auxWord >> 8);
    }
    
    while (inb(device->regBase + 0x7)& 0x88) 
        
    return asectorCount;
}

int installDevices() {
    SdevicePort chanel0, chanel1;
    chanel1.regBase = 0x170;
    chanel1.regControl = 0x376;
    chanel0.regBase = 0x1F0;
    chanel0.regControl = 0x3F6;

    char *buffer = (char*) 0x100000;
    //for (int i = 29; i < 58; i++) {
    readSector(&chanel1, 0, 29, 1, buffer);
    //   buffer += 2048;
    // }
    char *ptrD = (char*) 0xB8000;
    char val = *(char*) 0x100000;
    val = (val & 0x0f);
    char nuevo;
    nuevo = val + 0x30;
    ptrD[0] = nuevo;
    while (1);
    asm("mov $0x000FFF0,%esp");
    asm("ljmp $0x18,$0x100000");
}


void resetBus(SdevicePort *device) {
    outb(device->regControl, 0x04);   
    for (int i = 0; i < 4; i++)  inb(device->regControl); //400ns   
    outb(device->regControl, 0x0);
}

void delay400ns(SdevicePort *device) {
    for (int i = 0; i < 4; i++)
      inb(device->regControl); //400ns  
}