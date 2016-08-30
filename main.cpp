/*
Escrito por Julian Soto, juliansotoponce@gmail.com

Este pequeno programa busca
cuadrados magicos que el recorrido
de una torre de ajedrez deja en las
casillas del tablero, cada casilla
se inicializa a 0, y cada una,
donde se ubique la torre, obtendra
un valor de acuerdo a la cantidad
 de movimientos pasados
*/

#include <iostream>
#include <stdio.h>
#include <vector>

#define CASILLAS_LADO 8

using namespace std;

//////la estructura que contiene el tablero///////
struct Tablero {
    int casillas[CASILLAS_LADO][CASILLAS_LADO];
};

//se crea un vector que guarda todas las
//posiciones que hagan un cuadrado magico
vector<Tablero>tableros;

////////////prototipos////////////
int moverTorre(Tablero, int, int, int);
Tablero inicializarTablero(Tablero);
bool movimientoValido(int, int, int, int);
bool cuadradoMagico(Tablero);

////////////funcion principal///////////
int main(){
    //se declara el tablero
    Tablero tabl;

    //se inicializa el tablero a 0
    tabl = inicializarTablero(tabl);

    //se llama a la funcion que analiza recursivamente
    int nodos = moverTorre(tabl, 1, 0, 0);

    if(tableros.size() == 1) printf("1 cuadrado magico encontrado\n");
    else if (tableros.size() > 1) printf("%i cuadrados magicos encontrados\n", tableros.size());
    else if (tableros.size() <= 0) printf("Ningun cuadrado magico encontrado\n");
    printf("%u nodos totales\n\n", nodos);

    for (int i = 0; i < tableros.size(); i++){
        for (int k = 0; k < CASILLAS_LADO; k++){
            for (int l = 0; l < CASILLAS_LADO; l++){
                if (tableros[i].casillas[k][l] < 10) printf("%i  ", tableros[i].casillas[k][l]);
                else printf("%i ", tableros[i].casillas[k][l]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

int moverTorre(Tablero t, int profundidad, int x, int y){
    static unsigned long long int nodos = 0;

    nodos++;

    for (int i = 0; i < CASILLAS_LADO; i++){
        for (int j = 0; j < CASILLAS_LADO; j++){

            //si no ha pasado la torre antes
            if (t.casillas[i][j] == 0){

                //si es el primer movimiento o si el movimiento es valido
                if (profundidad == 1 || movimientoValido(i, j, x, y)){

                    //se hace una copia de la estructura del tablero
                    Tablero _t = t;

                    //se iguala la casilla al valor de la profundidad
                    _t.casillas[i][j] = profundidad;

                    //si no se ocupan todas las casilla
                    if (profundidad < CASILLAS_LADO*CASILLAS_LADO){
                        //llamada recursiva
                        moverTorre(_t, profundidad+1, i, j);
                    } else {

                        if (nodos%1000000 == 0) {
                            printf("nodos %llu\n", nodos);
                            for (int k = 0; k < CASILLAS_LADO; k++){
                                for (int l = 0; l < CASILLAS_LADO; l++){
                                    if (_t.casillas[k][l] < 10) printf("%i  ", _t.casillas[k][l]);
                                    else printf("%i ", _t.casillas[k][l]);
                                }
                                printf("\n");
                            }
                            printf("\n");
                        }

                        //si es un cuadrado magico se agrega al vector
                        if (cuadradoMagico(_t)) {
                            tableros.push_back(_t);
                            printf("Una secuencia encontrada\n\n");
                        }
                        return nodos;
                    }
                }
            }
        }
    }
    return nodos;
}

//se inicializan todas las casillas a 0
Tablero inicializarTablero(Tablero t){
    for (int i = 0; i < CASILLAS_LADO; i++){
        for (int j = 0; j < CASILLAS_LADO; j++){
            t.casillas[i][j] = 0;
        }
    }
    return t;
}

//si el movimiento es el de una torre
bool movimientoValido(int i, int j, int x, int y){
    if (i == x && j != y) return true;
    if (j == y && i != x) return true;
    return false;
}

//si las sumas de las filas, columnas y las dos diagonales son iguales retorna true
bool cuadradoMagico(Tablero t){
    int suma[2*CASILLAS_LADO+2];

    //se inicializa el arreglo a 0
    for (int i = 0; i < 2*CASILLAS_LADO+2; i++){
        suma[i] = 0;
    }

    //filas
    for (int i = 0; i < CASILLAS_LADO; i++){
        int a = 0;
        for (int j = 0; j < CASILLAS_LADO; j++){
            a += t.casillas[i][j];
        }
        suma[i] = a;
    }

    //columnas
    for (int i = 0; i < CASILLAS_LADO; i++){
        int a = 0;
        for (int j = 0; j < CASILLAS_LADO; j++){
            a += t.casillas[j][i];
        }
        suma[i+CASILLAS_LADO] = a;
    }

    //diagonal ascendente
    for (int i = 0; i < CASILLAS_LADO; i++){
        suma[2*CASILLAS_LADO] += t.casillas[i][CASILLAS_LADO-1-i];
    }

    //diagonal descendente
    for (int i = 0; i < CASILLAS_LADO; i++){
        suma[2*CASILLAS_LADO+1] += t.casillas[i][i];
    }

    //comparacion
    for (int i = 0; i < 2*CASILLAS_LADO+2; i++){
        if (suma[i] != suma[0]) return false;
    }

    return true;
}
