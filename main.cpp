#include <iostream>
using namespace std;

unsigned char rotar_derecha(unsigned char byte, int n);
unsigned char rotar_izquierda(unsigned char byte, int n);
void XOR(char* Arreglo_Bytes, int tamaño, unsigned char clave);
void desencriptar_buffer(char* buffer, int tamaño, int n, unsigned char k );
int es_digito(char caracter);
int calcular_longitud_cadena(const char* cadena);


int main()
{

    return 0;
}

int calcular_longitud_cadena(const char* cadena){
    int longitud = 0;
    while(cadena[longitud] != '\0'){
        ++longitud;
    }
    return longitud;
}

int es_digito(char caracter){
    return (caracter >= '0' && caracter <= '9' );
}

// Función para rotar un byte a la izquierda n posiciones (0 < n < 8)
unsigned char rotar_izquierda(unsigned char byte, int n) {
    return (byte << n) | (byte >> (8 - n));
}

// Función para rotar un byte a la derecha n posiciones (inversa de izquierda)
unsigned char rotar_derecha(unsigned char byte, int n) {
    return (byte >> n) | (byte << (8 - n));
}

void XOR(char* buffer, int tamaño, unsigned char clave) {
    for (int i = 0; i < tamaño; i++) {
        buffer[i] = (buffer[i] ^ clave);  // operación XOR bit a bit
    }
}

//funcion para desencriptar un buffer: Xor con k, luego rotar a la derecha n
void desencriptar_buffer(char* buffer, int tamaño, int n, unsigned char k ){
    XOR(buffer, tamaño, k);
    for(int i=0; i< tamaño; ++i){
        buffer[i] = rotar_derecha((unsigned char)buffer[i], n);
    }
}


