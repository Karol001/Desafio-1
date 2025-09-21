#include <iostream>

using namespace std;

unsigned char Rot_DerechaBxB(unsigned char caracter, unsigned char n);
unsigned char Rot_IzquirdaBxB(unsigned char caracter, unsigned char n);

// Imprime un byte en binario (8 bits)
void print_bin(unsigned char b) {
    for (int i = 7; i >= 0; --i) {
        unsigned char bit = (unsigned char)((b >> i) & 1);
        putchar(bit ? '1' : '0');
    }
}

// Imprime una línea con HEX y BIN
void show(const char* titulo, unsigned char v) {
    printf("%-20s 0x%02X  ", titulo, (unsigned)v);
    print_bin(v);
    putchar('\n');
}

#include <stdio.h>

int main()
{
    unsigned char x = 'A';   // 0x41 = 01000001
    unsigned char n = 3;     // ejemplo del documento

    unsigned char r = Rot_IzquirdaBxB(x, n);  // esperado: 0x0A = 00001010
    unsigned char y = Rot_DerechaBxB(r, n);  // esperado: 0x41 = 01000001

    puts("=== Prueba de rotación circular (byte) ===");
    printf("Rotación: izquierda %u bit(s), luego inversa derecha %u bit(s)\n\n", (unsigned)n, (unsigned)n);

    show("Original (x):", x);
    show("rotl8(x, n):", r);
    show("rotr8(...,n):", y);

    // Verificación automática (mensaje al final)
    if (r == 0x0A && y == x) {
        puts("\nOK: Los valores coinciden con el ejemplo esperado.");
        return 0;
    } else {
        puts("\nERROR: Resultado inesperado.");
        return 1;
    }
}


// Rotación circular de 8 bits

// Rotar a la izquierda 'n' posiciones (0..7)
unsigned char Rot_IzquirdaBxB(unsigned char caracter, unsigned char n) {
    n &= 7; // asegurar 0..7 para evitar desplazar 8 (UB)
    unsigned char s = (unsigned char)((8 - n) & 7);
    return (unsigned char)((unsigned char)(caracter << n) | (unsigned char)(caracter >> s));
}

// Rotar a la derecha 'n' posiciones (0..7)
unsigned char Rot_DerechaBxB(unsigned char caracter, unsigned char n) {
    n &= 7;
    unsigned char s = (unsigned char)((8 - n) & 7);
    return (unsigned char)((unsigned char)(caracter >> n) | (unsigned char)(caracter << s));
}


