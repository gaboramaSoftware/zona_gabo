#include <iostream>
#include <cstring> 
#include <cstdio>  

using namespace std;

extern "C" {
    
    // 1. Funcion para procesar punteros como argv y argc
    void procesarPunteros(int mi_argc, char** mi_argv) {
        cout << "\n--- DENTRO DE LA FUNCION ---" << endl;
        for (int i = 0; i < mi_argc; i++) {
            cout << "Argumento [" << i << "]: " << mi_argv[i] << endl;
            cout << "   (En memoria: " << (void*)mi_argv[i] << ")" << endl;
        }
        cout << "---------------------------------------------\n" << endl;
    }

    // 2. Ahora sí, el Puente puede llamar a la función de arriba
    void PuenteParaJS(char* frase_completa) {
        // creamos los buffers necesarios
        char* lista_de_punteros[200];
        int count_palabras = 0;

        // separamos la frase en tokens
        char* token = strtok(frase_completa, " ");
        while (token != nullptr && count_palabras < 200) {
            lista_de_punteros[count_palabras] = token;
            count_palabras++;
            token = strtok(nullptr, " ");
        }

        //LLamamos a la funcion para procesar argc y argv
        procesarPunteros(count_palabras, lista_de_punteros);
    }
}

int main() {
    // ... (Tu main se queda igual, sirve para probar en consola normal) ...
    char buffer_entrada[2000]; 
    cout << "Escribe una frase completa (max 100 caracteres): ";
    cin.getline(buffer_entrada, 2000);
    char* lista_de_punteros[200]; 
    int contador_palabras = 0;
    char* token = strtok(buffer_entrada, " ");
    while (token != nullptr && contador_palabras < 200) {
        lista_de_punteros[contador_palabras] = token;
        contador_palabras++;
        token = strtok(nullptr, " ");
    }
    procesarPunteros(contador_palabras, lista_de_punteros);
    return 0;
}