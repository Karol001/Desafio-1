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
char* copiar_cadena(const char* cadena_origen, int longitud);
char* leer_archivo_a_buffer(const char* nombre_archivo, int& tamano_archivo);
char* desomprimirRle(const char* cadena_codificada, int longitud_codificada, int& longitud_original);
char* desmcomprimirLZ78(const char* datos_comprimidos,int longitud_comprimido, int& longitud_original);
char* identificarydescifrar(const char* datos_codificados, int longitud_codificada, const char* fragmento,
                            int longitud_fragmento, int* parametros_encontrados, int& longitud_descomprimida);



int main()
{

    return 0;
}

//funciones

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

unsigned char rotar_izquierda(unsigned char byte, int n) {
    n &= 7;
    return (unsigned char)((byte << n) | (byte >> (8 - n)));
}
unsigned char rotar_derecha(unsigned char byte, int n) {
    n &= 7;
    return (unsigned char)((byte >> n) | (byte << (8 - n)));
}

void XOR(char* buffer, int tamaño, unsigned char clave) {
    for (int i = 0; i < tamaño; i++) {
        buffer[i] = (char)((unsigned char)buffer[i] ^ clave);
    }
}

void encriptar_buffer(char* buffer, int longitud, int n, unsigned char k){
    for (int i= 0; i < longitud; ++i){
        buffer[i] = (char)rotar_izquierda((unsigned char)buffer[i], n);
    }
    XOR(buffer, longitud, k);
}

// XOR + rotación derecha
void desencriptar_buffer(char* buffer, int tamaño, int n, unsigned char k ){
    XOR(buffer, tamaño, k);
    for(int i=0; i< tamaño; ++i){
        buffer[i] = (char)rotar_derecha((unsigned char)buffer[i], n);
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

// leer archivo -> buffer
char* leer_archivo_a_buffer(const char* nombre_archivo, int& tamano_archivo){
    ifstream archivo(nombre_archivo, std::ios::binary | std::ios::ate);
    if(!archivo.is_open()){
        cout<<"error: no se pudo abrir el archivo "<< nombre_archivo <<endl;
        tamano_archivo = 0;
        return NULL;
    }

    tamano_archivo = (int)archivo.tellg();
    archivo.seekg(0,std::ios::beg);

    char* contenido_buffer = new char [tamano_archivo + 1];

    archivo.read(contenido_buffer, tamano_archivo);
    contenido_buffer[tamano_archivo] = '\0';

    archivo.close();
    return contenido_buffer;
}

// RLE simple
char* desomprimirRle(const char* cadena_codificada, int longitud_codificada, int& longitud_original){
    longitud_original = 0;
    char* cadena_descomprimida = new char[longitud_codificada * 100 + 1];
    int indice_salida = 0;
    int i = 0;
    while (i < longitud_codificada){
        int repeticiones = 0;
        while (i < longitud_codificada && es_digito(cadena_codificada[i])){
            repeticiones = repeticiones * 10 + (cadena_codificada[i] - '0');
            ++i;
        }
        if (i >= longitud_codificada || repeticiones == 0){
            delete[] cadena_descomprimida;
            return NULL;
        }
        char simbolo = cadena_codificada[i++];
        for(int j = 0; j < repeticiones; ++j){
            if (indice_salida >= longitud_codificada * 100){
                delete [] cadena_descomprimida;
                return NULL;
            }
            cadena_descomprimida[indice_salida++] = simbolo;
        }
    }
    cadena_descomprimida[indice_salida] = '\0';
    longitud_original = indice_salida;
    return cadena_descomprimida;
}

// LZ78
char* desmcomprimirLZ78(const char* datos_comprimidos,int longitud_comprimido, int& longitud_original){
    longitud_original = 0;
    if (longitud_comprimido <= 0 || (longitud_comprimido % 3) != 0){
        return NULL;
    }
    const int MAX_DICC = 65536;
    char** diccionario = new char* [MAX_DICC];
    int tamano_diccionario = 0;
    for (int j = 0; j < MAX_DICC; ++j) diccionario[j] = NULL;

    char* datos_salida = new char[longitud_comprimido * 100 + 1];
    int indice_salida = 0;

    for (int i=0; i<longitud_comprimido; i+=3){
        unsigned short indice_prefijo_codificado =
            (unsigned short)(((unsigned char)datos_comprimidos[i] << 8) |
                              (unsigned char)datos_comprimidos[i + 1]);
        char siguiente_caracter = datos_comprimidos[i + 2];

        char* prefijo = NULL;
        int longitud_prefijo = 0;

        if (indice_prefijo_codificado == 0){
            prefijo = new char[1]; prefijo[0] = '\0';
            longitud_prefijo = 0;
        } else {
            unsigned short idx = (unsigned short)(indice_prefijo_codificado - 1);
            if (idx >= tamano_diccionario || diccionario[idx] == NULL){
                for (int k = 0; k < tamano_diccionario; ++k) if (diccionario[k]) delete[] diccionario[k];
                delete[] diccionario; delete[] datos_salida;
                return NULL;
            }
            prefijo = copiar_cadena(diccionario[idx], -1);
            longitud_prefijo = calcular_longitud_cadena(prefijo);
        }

        char* nueva_entrada_diccionario = new char[longitud_prefijo + 2];
        for (int k = 0; k < longitud_prefijo; ++k) nueva_entrada_diccionario[k] = prefijo[k];
        nueva_entrada_diccionario[longitud_prefijo] = siguiente_caracter;
        nueva_entrada_diccionario[longitud_prefijo + 1] = '\0';

        if (tamano_diccionario < MAX_DICC){
            diccionario[tamano_diccionario++] = nueva_entrada_diccionario;
        } else {
            for (int k = 0; k < tamano_diccionario; ++k) if (diccionario[k]) delete[] diccionario[k];
            delete[] diccionario; delete[] datos_salida; delete[] prefijo; delete[] nueva_entrada_diccionario;
            return NULL;
        }

        for (int k = 0; k < longitud_prefijo; ++k){
            if (indice_salida >= longitud_comprimido * 100){
                for (int m = 0; m < tamano_diccionario; ++m) if (diccionario[m]) delete[] diccionario[m];
                delete[] diccionario; delete[] datos_salida; delete[] prefijo;
                return NULL;
            }
            datos_salida[indice_salida++] = prefijo[k];
        }

        if (indice_salida >= longitud_comprimido * 100){
            for (int m = 0; m < tamano_diccionario; ++m) if (diccionario[m]) delete[] diccionario[m];
            delete[] diccionario; delete[] datos_salida; delete[] prefijo;
            return NULL;
        }
        datos_salida[indice_salida++] = siguiente_caracter;

        delete[] prefijo;
    }

    datos_salida[indice_salida] = '\0';
    longitud_original = indice_salida;

    for (int j= 0; j < tamano_diccionario; ++j) if (diccionario[j]) delete[] diccionario[j];
    delete[] diccionario;
    return datos_salida;
}

char* identificarydescifrar(const char* datos_codificados, int longitud_codificada, const char* fragmento,
                            int longitud_fragmento, int* parametros_encontrados, int& longitud_descomprimida){
    char* resultado_final = NULL;
    parametros_encontrados[0] = -1;
    for (int n=1; n < 8; ++n){
        for(int k=0; k < 256; ++k){
            char* datos_descifrados_temp = copiar_cadena(datos_codificados, longitud_codificada);
            desencriptar_buffer(datos_descifrados_temp, longitud_codificada, n, (unsigned char)k);

            int RLE_longitud = 0;
            char* RLE_resultado = desomprimirRle(datos_descifrados_temp, longitud_codificada, RLE_longitud);
            if (RLE_resultado && contiene_subcadena(RLE_resultado, RLE_longitud, fragmento, longitud_fragmento)){
                resultado_final = RLE_resultado;
                parametros_encontrados[0] = 0; // RLE
                parametros_encontrados[1] = n;
                parametros_encontrados[2] = k;
                longitud_descomprimida = RLE_longitud;
                delete[] datos_descifrados_temp;
                return resultado_final;
            }
            delete [] RLE_resultado;

            int lz_tamaño = 0;
            char* lz_resultado = desmcomprimirLZ78(datos_descifrados_temp, longitud_codificada, lz_tamaño);
            if (lz_resultado && contiene_subcadena(lz_resultado, lz_tamaño, fragmento, longitud_fragmento)){
                resultado_final = lz_resultado;
                parametros_encontrados[0] = 1; // LZ78
                parametros_encontrados[1] = n;
                parametros_encontrados[2] = k;
                longitud_descomprimida = lz_tamaño;
                delete [] datos_descifrados_temp;
                return resultado_final;
            }
            delete[] lz_resultado;
            delete[] datos_descifrados_temp;
        }
    }
    return NULL;
}
