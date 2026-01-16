#include <emscripten/emscripten.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <numeric>

using namespace std;

// Tu función de hashing intacta (es lógica pura, funciona perfecto en Wasm)
unsigned long generarHashInternal(const vector<string>& lista_palabras) {
    // Usamos 5381UL explícito para asegurar unsigned long
    return accumulate(lista_palabras.begin(), lista_palabras.end(), 5381UL, 
        [](unsigned long hash_acumulado, const string& palabra) {
            return accumulate(palabra.begin(), palabra.end(), hash_acumulado,
                [](unsigned long h, char c) {
                    return ((h << 5) + h) + c; 
                });
        });
}

extern "C" {
    
    // Esta es la función que JavaScript va a llamar
    // Recibe un puntero de caracteres (el string de JS)
    EMSCRIPTEN_KEEPALIVE
    unsigned long calcular_hash_wasm(const char* input_ptr) {
        
        // 1. Convertir puntero C a string C++
        string entrada(input_ptr);
        
        // 2. Tu lógica de tokenización (stringstream)
        vector<string> lista_de_palabras;
        stringstream ss(entrada);
        string palabra_temp;
        
        while (ss >> palabra_temp) {
            lista_de_palabras.push_back(palabra_temp);
        }
        
        // 3. Generar hash
        return generarHashInternal(lista_de_palabras);
    }
}