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

#include "../../includes/stdlib.h"
extern u8 context;

//Para operador new ejemplo int *val = new int;
void* operator new(u32 len){
    malloc(len);
}

//Para operador new ejemplo int *val = new int[10]
void* operator new[](u32 len) {
    return ::operator new (len);
}

//para operadores delete(val)

void operator delete(void *ptr) {
    free(ptr);
}

//para operadores delete(val[5])
void operator delete[](void *ptr) {
    ::operator delete (ptr);
}

//Para la declaracion de clases
extern "C" {

    void __terminate() {
        return;
    }
    //funcion que retorna cuando ocurre una excepion

    void* __get_eh_context() {
        return (void*)&context;
    }

    void __sjthrow() {
        __terminate();
    }

    int __rtti_user() {
        return 0;
    }

    void __rtti_si() {
        return;
    }

    void __pure_virtual() {
        return;
    }
    
}