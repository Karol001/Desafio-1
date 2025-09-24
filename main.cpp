#include <iostream>
using namespace std;

unsigned char rotar_derecha(unsigned char byte, int n);
unsigned char rotar_izquierda(unsigned char byte, int n);
void XOR(char* Arreglo_Bytes, int tamaño, unsigned char clave);
void desencriptar_buffer(char* buffer, int tamaño, int n, unsigned char k );
int es_digito(char caracter);
int calcular_longitud_cadena(const char* cadena);
void encriptar_buffer(char* buffer, int longitud, int n, unsigned char k);
int contiene_subcadena(const char* buffer, int longitud_texto, const char* fragmento, int longitud_fragmento);

int main()
{

    // ---- calcular_longitud_cadena ----
    const char* s1 = "Hola";
    cout << "[longitud] \"Hola\" -> " << calcular_longitud_cadena(s1) << " (esperado 4)\n";

    // ---- es_digito ----
    cout << "[es_digito] '5' -> " << es_digito('5') << " (esperado 1)\n";
    cout << "[es_digito] 'a' -> " << es_digito('a') << " (esperado 0)\n";

    // ---- rotar_izquierda / rotar_derecha ----
    unsigned char b = 0x81; // 1000 0001
    unsigned char bl = rotar_izquierda(b, 1);
    unsigned char br = rotar_derecha(bl, 1);
    cout << hex;
    cout << "[rotar] 0x" << (int)b << " <<1 -> 0x" << (int)bl
         << " >>1 -> 0x" << (int)br << " (esperado volver a 0x81)\n";
    cout << dec;

    // ---- XOR ida y vuelta ----
    char bufX[6] = "Hola!"; // incluye '\0' implícito al final
    int lenX = calcular_longitud_cadena(bufX);
    cout << "[XOR] antes: " << bufX << "\n";
    XOR(bufX, lenX, (unsigned char)0x5A);
    cout << "[XOR] cifrado (no legible): ";
    for (int i = 0; i < lenX; ++i) cout << (int)(unsigned char)bufX[i] << ' ';
    cout << "\n";
    XOR(bufX, lenX, (unsigned char)0x5A);
    cout << "[XOR] después de revertir: " << bufX << " (esperado Hola!)\n";

    // ---- encriptar_buffer / desencriptar_buffer ----
    char bufED[12] = "Hola Mundo";
    int lenED = calcular_longitud_cadena(bufED);
    cout << "[enc/dec] original: " << bufED << "\n";
    encriptar_buffer(bufED, lenED, 3, (unsigned char)0x5A);
    cout << "[enc/dec] cifrado (bytes): ";
    for (int i = 0; i < lenED; ++i) cout << (int)(unsigned char)bufED[i] << ' ';
    cout << "\n";
    desencriptar_buffer(bufED, lenED, 3, (unsigned char)0x5A);
    cout << "[enc/dec] descifrado: " << bufED << " (esperado Hola Mundo)\n";

    // ---- contiene_subcadena ----
    const char* texto = "Hola mundo";
    const char* frag1 = "mun";
    const char* frag2 = "xyz";
    int lt = calcular_longitud_cadena(texto);
    int lf1 = calcular_longitud_cadena(frag1);
    int lf2 = calcular_longitud_cadena(frag2);
    cout << "[subcadena] \"mun\" en \"Hola mundo\" -> "
         << contiene_subcadena(texto, lt, frag1, lf1) << " (esperado 1)\n";
    cout << "[subcadena] \"xyz\" en \"Hola mundo\" -> "
         << contiene_subcadena(texto, lt, frag2, lf2) << " (esperado 0)\n";

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

void encriptar_buffer(char* buffer, int longitud, int n, unsigned char k){
    for (int i= 0; i < longitud; ++i){
        buffer[i] = rotar_izquierda((unsigned char)buffer[i], n);
    }
    XOR(buffer, longitud, k);
}


//funcion para desencriptar un buffer: Xor con k, luego rotar a la derecha n
void desencriptar_buffer(char* buffer, int tamaño, int n, unsigned char k ){
    XOR(buffer, tamaño, k);
    for(int i=0; i< tamaño; ++i){
        buffer[i] = rotar_derecha((unsigned char)buffer[i], n);
    }
}

int contiene_subcadena(const char* buffer, int longitud_texto, const char* fragmento, int longitud_fragmento){
    if (longitud_fragmento > longitud_texto || longitud_fragmento == 0 ){
        return 0;
    }
    for (int i = 0; i <= longitud_texto - longitud_fragmento; ++i){
        int coincide = 1;
        for (int j = 0; j < longitud_fragmento; ++j){
            if (buffer[i + j] != fragmento[j]){
                coincide = 0;
                break;
            }
        }
        if (coincide) return 1;
    }
    return 0;
}
