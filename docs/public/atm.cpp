//primer proyecto de gabriel, refactorizado para conexiones con web assembly

#include <emscripten/emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace std;

// --- TUS VARIABLES GLOBALES ORIGINALES (Mantenidas por nostalgia) ---
int b, z, r, r1, c, c1, m, g, d, s = 270000, t, r1n, cs, cA;
int ba, bb, bc, bd, be, b20, b10, b5, b2, b1;
int estado = 0; // 0=Inicio, 1=Rut, 2=Clave, 3=Menu, 4=Giro, 5=Deposito, 6=NuevoRut, 7=NuevaClave, 8=InfoConfirm

// Buffer para responderle a JS
string respuesta_pantalla;

// Función auxiliar para "imprimir" en el string en lugar de la consola
void print(string texto) {
    respuesta_pantalla += texto;
}

void limpiar_pantalla() {
    respuesta_pantalla = ""; // En web, limpiar es vaciar el string
}

void billetes(int g) {
    if (g >= 20000) { b20 = (g / 20000); ba = 20000; g = g - b20 * 20000; 
        print("\n\t\tretire su dinero en " + to_string(b20) + " billetes de " + to_string(ba) + "\n"); }
    if (g >= 10000) { b10 = (g / 10000); bb = 10000; g = g - b10 * 10000; 
        print("\n\t\tretire su dinero en " + to_string(b10) + " billetes de " + to_string(bb) + "\n"); }
    if (g >= 5000) { b5 = (g / 5000); bc = 5000; g = g - b5 * 5000; 
        print("\n\t\tretire su dinero en " + to_string(b5) + " billetes de " + to_string(bc) + "\n"); }
    if (g >= 2000) { b2 = (g / 2000); bd = 2000; g = g - b2 * 2000; 
        print("\n\t\tretire su dinero en " + to_string(b2) + " billetes de " + to_string(bd) + "\n"); }
    if (g >= 1000) { b1 = (g / 1000); be = 1000; g = g - b1 * 1000; 
        print("\n\t\tretire su dinero en " + to_string(b1) + " billetes de " + to_string(be) + "\n"); }
}

void mostrar_menu() {
    print("\n\n\t\t        BIENVENIDO AL MENU");
    print("\n\n\t\t[1] Girar dinero                [2] Depositar dinero");
    print("\n\n\t\t[3] Consultar saldo             [4] Salir");
    print("\n\n\t\t[5] Nueva cuenta                [6] Info de la cuenta");
    print("\n\n\t\tSeleccione opcion: ");
}

extern "C" {

    // Función inicial para resetear todo
    EMSCRIPTEN_KEEPALIVE
    const char* iniciar_atm() {
        limpiar_pantalla();
        estado = 1; // Esperando RUT
        s = 270000; // Saldo inicial
        print("\n\n\t\tBIENVENIDO A EVILCORP.E");
        print("\n\n\t\t-----------------------");
        print("\n\n\t\tingrese su rut sin digito verificador: ");
        return respuesta_pantalla.c_str();
    }

    // EL CEREBRO: Recibe el número que el usuario tecleó y decide qué hacer
    EMSCRIPTEN_KEEPALIVE
    const char* procesar_input(int input) {
        limpiar_pantalla(); // Limpiamos para mostrar la respuesta nueva

        switch (estado) {
            case 1: // Validar RUT
                r = input;
                if (r < 1000000 || r > 30000000) {
                    print("\n\t\tRUT invalido. Intente de nuevo: ");
                } else {
                    r1 = r % 100000;
                    print("\n\t\tCliente: ***" + to_string(r1));
                    print("\n\n\t\tIngrese su clave: ");
                    estado = 2;
                }
                break;

            case 2: // Validar Clave
                c = input;
                if (c < 1000 || c > 9999) {
                    print("\n\t\tClave debe ser de 4 digitos. Intente de nuevo: ");
                } else {
                    estado = 3; // Vamos al menu
                    mostrar_menu();
                }
                break;

            case 3: // MENU PRINCIPAL
                m = input;
                switch(m) {
                    case 1: // Girar
                        print("\n\t\tCuanto dinero desea girar? (multiplos de 1000): $ ");
                        estado = 4;
                        break;
                    case 2: // Depositar
                        print("\n\t\tCuanto dinero desea depositar? (multiplos de 1000): $ ");
                        estado = 5;
                        break;
                    case 3: // Saldo
                        print("\n\t\tSu saldo actual es de $" + to_string(s) + "\n\n");
                        print("\t\tPresione [1] para volver al menu: ");
                        estado = 99; // Estado de pausa simple
                        break;
                    case 4: // Salir
                        print("\n\n\t\tMUCHAS GRACIAS POR PREFERIR EVILCORP.E\n\t\t(Reinicie la pagina para volver a entrar)");
                        estado = -1; // Fin
                        break;
                    case 5: // Nueva cuenta
                        print("\n\t\tIngrese NUEVO RUT: ");
                        estado = 6;
                        break;
                    case 6: // Info cuenta
                        print("\n\t\tConfirme clave para ver info: ");
                        estado = 8;
                        break;
                    default:
                        print("Opcion invalida.\n");
                        mostrar_menu();
                }
                break;

            case 4: // PROCESO DE GIRO
                g = input;
                if(g < s && g > 0) {
                    if (g % 1000 == 0) {
                        s = s - g;
                        billetes(g);
                        print("\n\t\tNuevo saldo: " + to_string(s));
                    } else {
                        print("\n\t\tError: Debe ser multiplo de 1000.");
                    }
                } else {
                    print("\n\t\tSaldo insuficiente o monto invalido.");
                }
                print("\n\n\t\tPresione [1] para volver al menu.");
                estado = 99;
                break;

            case 5: // PROCESO DE DEPOSITO
                d = input;
                if (d > 0 && d % 1000 == 0) {
                    s = s + d;
                    print("Su nuevo saldo es de : $ " + to_string(s));
                } else {
                    print("\n\t\tMonto invalido.");
                }
                print("\n\n\t\tPresione [1] para volver al menu.");
                estado = 99;
                break;
            
            case 6: // NUEVO RUT
                 r1 = input;
                 if(r1<1000000 || r1>30000000) print("Rut invalido, intente de nuevo: ");
                 else {
                     print("\n\t\tIngrese NUEVA CLAVE (4 digitos): ");
                     estado = 7;
                 }
                 break;

            case 7: // NUEVA CLAVE
                c1 = input;
                if(c1 < 1000 || c1 > 9999) print("Clave invalida, intente de nuevo: ");
                else {
                    r = r1; c = c1; s = 270000; // Resetear datos
                    print("\n\t\tUsuario Creado Exitosamente.\n\t\tPresione [1] para volver al menu.");
                    estado = 99;
                }
                break;

            case 8: // CONFIRMAR CLAVE PARA INFO
                cs = input;
                if (cs == c) {
                    print("\n\t\tNumero cliente: " + to_string(r));
                    print("\n\t\tClave: " + to_string(c));
                    print("\n\t\tSaldo: " + to_string(s));
                } else {
                    print("\n\t\tClave incorrecta.");
                }
                print("\n\n\t\tPresione [1] para volver al menu.");
                estado = 99;
                break;

            case 99: // ESTADO DE "PAUSE" (Volver al menú)
                estado = 3;
                mostrar_menu();
                break;
        }

        return respuesta_pantalla.c_str();
    }
}