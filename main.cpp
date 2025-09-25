#include <iostream>
#include <fstream>

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

char* copiar_cadena(const char* cadena_origen, int longitud){
    if (longitud == -1){
        longitud = calcular_longitud_cadena(cadena_origen);
    }
    char* nueva_cadena = new char[longitud + 1];
    for(int i=0; i < longitud; ++i){
        nueva_cadena[i] = cadena_origen[i];
    }

    nueva_cadena[longitud] = '\0';
    return nueva_cadena;
}

//funcion para leer archivos y retornar el buffer del contendio
char* leer_archivo_a_buffer(const char* nombre_archivo, int& tamano_archivo){
    ifstream archivo(nombre_archivo, std::ios::binary | std::ios::ate);
    if(!archivo.is_open()){
        cout<<"error: no se pudo abrir el archivo"<< nombre_archivo <<endl;
        tamano_archivo = 0;
        return NULL;
    }

    tamano_archivo = archivo.tellg();
    archivo.seekg(0,std::ios::beg);

    char* contenido_buffer = new char [tamano_archivo +1];

    archivo.read(contenido_buffer, tamano_archivo);
    contenido_buffer[tamano_archivo] = '\0';

    archivo.close();
    return contenido_buffer;
}

char* desmcomprimirLZ78(const char* datos_comprimidos,int longitud_comprimido, int& longitud_original){
    longitud_original = 0;
    if (longitud_comprimido % 3 != 0){
        return NULL;
    }
    const int MAX_DICC = 65536;
    char** diccionario = new char* [MAX_DICC];
    int tamano_diccionario = 0;
    for (int j = 0; j<MAX_DICC; ++j){
        diccionario[j] = NULL;
    }
    char* datos_salida = new char[longitud_comprimido* 100 +1];
    int indice_salida = 0;
    for (int i=0; i<longitud_comprimido; i+=3){
        unsigned short indice_prefijo_codificado = ((unsigned char)datos_comprimidos[i]<<8) |
                                                   (unsigned char)datos_comprimidos[i + 1];
        char siguiente_caracter = datos_comprimidos[i + 2];
        char* prefijo = NULL;
        int longitud_prefijo = 0;
        if (indice_prefijo_codificado == 0){
            prefijo = new char[1] {'\0'};
            longitud_prefijo = 0;
        }
        else if (indice_prefijo_codificado){
            prefijo = copiar_cadena(diccionario[indice_prefijo_codificado - 1], -1);
            longitud_prefijo = calcular_longitud_cadena(prefijo);
        }
        else{
            for (int k = 0; k < tamano_diccionario; ++k) {
                if (diccionario[k])
                    delete[] diccionario[k];
            }
            delete[] diccionario;
            delete[] datos_salida;
            if (prefijo) delete[] prefijo;
            return NULL;
        }
        char* nueva_entrada_diccionario = new char[longitud_prefijo + 2];
        for (int k =0; k<longitud_prefijo; ++k){
            nueva_entrada_diccionario[k] = prefijo[k];
        }
        nueva_entrada_diccionario[longitud_prefijo] = siguiente_caracter;
        nueva_entrada_diccionario[longitud_prefijo + 1] = '\0';

        if(tamano_diccionario < MAX_DICC){
            diccionario[tamano_diccionario++] = nueva_entrada_diccionario;
        }
        else
        {
            for(int k = 0; k<tamano_diccionario; ++k){
                if (diccionario[k]) delete[] diccionario[k];
            }
            delete[] diccionario;
            delete[] datos_salida;
            delete[] prefijo;
            delete[] nueva_entrada_diccionario;
            return NULL;
        }
        for(int k = 0; k < tamano_diccionario; ++k){
            if(indice_salida >= longitud_comprimido * 100){
                for(int m = 0; m < tamano_diccionario; ++m){
                    if (diccionario[m]) delete[] diccionario[m];
                }
                delete[] diccionario;
                delete[] datos_salida;
                delete[] prefijo;
                return NULL;
            }
            datos_salida[indice_salida++] = prefijo[k];
        }
        if(indice_salida >= longitud_comprimido * 100){
            for(int m = 0; m < tamano_diccionario; ++m){
                if (diccionario[m]) delete[] diccionario[m];
            }
            delete[] diccionario;
            delete[] datos_salida;
            delete[] prefijo;
            return NULL;
        }
        datos_salida[indice_salida++] = siguiente_caracter;
        delete[] prefijo;
    }
    datos_salida[indice_salida] = '\0';
    longitud_original = indice_salida;
    for(int j= 0; j < tamano_diccionario; ++j){
        if (diccionario[j]) delete[] diccionario[j];
    }
    delete[] diccionario;
    return datos_salida;
}
