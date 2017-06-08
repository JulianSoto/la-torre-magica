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
int     moverTorre(Tablero, int, int, int);
Tablero inicializarTablero(Tablero);
bool    movimientoValido(int, int, int, int);
bool    cuadradoMagico(Tablero);
bool    pruning(Tablero, int, int, int, unsigned long long int);
int     partialSum(int, int);
void    printFormatedBoard(Tablero, int);

const int BOARD_BOXES = CASILLAS_LADO*CASILLAS_LADO;
const int MAGIC_CONSTANT = partialSum(0, BOARD_BOXES)/CASILLAS_LADO;

////////////funcion principal///////////
int main(){
    //se declara el tablero
    Tablero tabl;
    cout << BOARD_BOXES << " " << MAGIC_CONSTANT << endl;

    //se inicializa el tablero a 0
    tabl = inicializarTablero(tabl);

    printf("Running...");

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

                    //pruning for a better search
                    if (pruning(_t, i, j, profundidad, nodos)){
                        if (nodos%500000 == 0){
                            printFormatedBoard(_t, nodos);
                        }

                        continue;
                    }

                    //si no se ocupan todas las casilla
                    if (profundidad < BOARD_BOXES){
                        //llamada recursiva
                        moverTorre(_t, profundidad+1, i, j);
                    } else {


                        printFormatedBoard(_t, nodos);


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

bool pruning(Tablero t, int _i, int _j, int profundidad, unsigned long long int nodos){
 /*   static long double sum = 0;
    static unsigned long long int lastNode = 0, timesCounted = 0;

    if (profundidad == 16){
        sum = (sum*timesCounted + (nodos - lastNode))/(timesCounted+1);
        timesCounted++;
        std::cout << sum << " " << nodos - lastNode << endl;
        lastNode = nodos;
    }*/

    if (profundidad == 1){
        //avoiding rotations
        if (_j > CASILLAS_LADO/2){
            return true;
        }
        //avoiding reflex
        if(_j < _i){
            return true;
        }
    }

    int emptySquares = 0;
    if (_i == _j){
        int diagSum = 0;
        for (int i = 0; i < CASILLAS_LADO; i++){
            int squareVal = t.casillas[i][i];
            diagSum += squareVal;
            if (squareVal == 0){
                emptySquares++;
            }
        }

        if (diagSum > MAGIC_CONSTANT){
            return true;
        }
        if (emptySquares == 0 && diagSum < MAGIC_CONSTANT){
            return true;
        }
        if (emptySquares <= CASILLAS_LADO/2 && diagSum + partialSum(BOARD_BOXES-emptySquares, BOARD_BOXES) < MAGIC_CONSTANT){
            return true;
        }
    }

    emptySquares = 0;
    int iSum = 0;

    for (int i = 0; i < CASILLAS_LADO; i++){
        int squareVal = t.casillas[i][_j];
        iSum += squareVal;
        if (squareVal == 0){
            emptySquares++;
        }
    }
                //partial sum
    if (iSum > MAGIC_CONSTANT){
        return true;
    }
    if (emptySquares == 0 && iSum < MAGIC_CONSTANT){
        return true;
    }
    if (emptySquares <= CASILLAS_LADO/2 && iSum + partialSum(BOARD_BOXES-emptySquares, BOARD_BOXES) < MAGIC_CONSTANT){
        return true;
    }

    emptySquares = 0;
    int jSum = 0;

    for (int i = 0; i < CASILLAS_LADO; i++){
        int squareVal = t.casillas[_i][i];
        jSum += squareVal;
        if (squareVal == 0){
            emptySquares++;
        }
    }
                //partial sum
    if (jSum > MAGIC_CONSTANT){
        return true;
    }
    if (emptySquares == 0 && jSum < MAGIC_CONSTANT){
        return true;
    }
    if (emptySquares <= CASILLAS_LADO/2 && jSum + partialSum(BOARD_BOXES-emptySquares, BOARD_BOXES) < MAGIC_CONSTANT){
        return true;
    }

    return false;
}

int partialSum(int beg, int en){
	return ( en*en+en - beg*beg-beg )/2;
}

void printFormatedBoard(Tablero _t, int nodos){
    static int lastNode;

    if (lastNode != nodos){
        lastNode = nodos;
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
}
