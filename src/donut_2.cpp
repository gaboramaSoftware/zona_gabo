#include <emscripten/emscripten.h>
#include <cstring>
#include <cmath>
#include <algorithm> 

// Variables GLOBALES para mantener el estado de la rotación entre cuadros
float A = 0;
float B = 0;

// Un buffer estático para devolver el string a JS sin líos de memoria
char output_buffer[1761]; 
float z[1760];
char b[1760];

extern "C" {

    // Quitamos el main loop y creamos una función que devuelve UN frame
    EMSCRIPTEN_KEEPALIVE
    const char* render_donut() {
        std::memset(b, 32, 1760); 
        std::memset(z, 0, 1760 * sizeof(float));

        // --- Tu lógica matemática intacta ---
        for (float j = 0; j < 6.28; j += 0.07) {
            for (float i = 0; i < 6.28; i += 0.02) {
                float c = sin(i);
                float d = cos(j);
                float e = sin(A);
                float f = sin(j);
                float g = cos(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = cos(i);
                float m = cos(B);
                float n = sin(B);
                float t = c * h * g - f * e;

                int x = 40 + 30 * D * (l * h * m - t * n);
                int y = 12 + 15 * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);

                if (y > 0 && y < 22 && x > 0 && x < 80 && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }
        // ------------------------------------

        // Preparamos el buffer de salida
        int p = 0;
        for(int k = 0; k < 1760; k++) {
            output_buffer[p++] = (k % 80) ? b[k] : '\n';
        }
        output_buffer[p] = '\0'; 

        // Avanzamos los ángulos para el SIGUIENTE llamado
        A += 0.04;
        B += 0.02;

        // Devolvemos el puntero al texto
        return output_buffer;
    }
}