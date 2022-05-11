#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
#include <stdlib.h>

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	actual.fila = sensores.posF;
	actual.columna = sensores.posC;
	actual.orientacion = sensores.sentido;

	cout << "Fila: " << actual.fila << endl;
	cout << "Col : " << actual.columna << endl;
	cout << "Ori : " << actual.orientacion << endl;
    Action accion = actIDLE;
    // Capturo los destinos

    n_destinos = sensores.num_destinos;
    cout << "sensores.num_destinos : " << sensores.num_destinos << endl;
    objetivos.clear();
    for (int i = 0; i < sensores.num_destinos; i++) {
        estado aux;
        aux.fila = sensores.destino[2 * i];
        aux.columna = sensores.destino[2 * i + 1];
        objetivos.push_back(aux);
    }

    if(sensores.nivel <= 2) {
        //Si no hay plan, cconstruyo uno
        if (!hayPlan) {
            hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);
        }

        accion = actIDLE;
        if (hayPlan && plan.size() > 0) {   //Hay un plan no vacio
            accion = plan.front();      //Tomo la siguiente acción del plan
            plan.erase(plan.begin()); //Eliminamos la acción del plan
        } else {
            cout << "No se pudo encontrar un plan\n";
        }
    }else {
        //nivel 3 y 4
        if (inicio_partida) {
            int tam = mapaResultado.size();
            for (int i = 0; i < tam; i++) {                                   //Rellenará el mapaResultado de precipios,
                for (int j = 0;
                     j < tam; j++) {                               //ya que es una mapa cerrado, según lo indicado
                    if (i <= 2 || i >= tam - 3 || j <= 2 || j >= tam - 3) {       // en el guión
                        mapaResultado[i][j] = 'P';
                    }
                }
            }

            n_avanzadas = 15;       //Por defecto
            cant_bateria = 1500;

            if (mapaResultado.size() <= 30) {
                n_avanzadas = 5;
                cant_bateria = 1000;
            } else if (mapaResultado.size() > 30 && mapaResultado.size() <= 50) {
                n_avanzadas = 15;
                cant_bateria = 500;
            } else if (mapaResultado.size() > 50 && mapaResultado.size() <= 75) {
                n_avanzadas = 20;
                cant_bateria = 500;
            } else if (mapaResultado.size() > 75 && mapaResultado.size() <= 100) {
                n_avanzadas = 20;
                cant_bateria = 500;
            }
            ultimaAccion = actIDLE;
            inicio_partida = false;
        }

        accion = actIDLE;
        //actualización del conocimiento
        switch (ultimaAccion) {
            case actFORWARD:
                switch (brujula) {
                    case 0:
                        fil--;
                        break;        //Norte
                    case 1:
                        col++;
                        fil--;
                        break;        //Noreste
                    case 2:
                        col++;
                        break;        //Este
                    case 3:
                        col++;
                        fil++;
                        break;        //Sureste
                    case 4:
                        fil++;
                        break;        //Sur
                    case 5:
                        fil++;
                        col--;
                        break;        //Suroeste
                    case 6:
                        col--;
                        break;        //Oeste
                    case 7:
                        col--;
                        fil--;
                        break;        //Noroeste
                }

                //Actualizo en cada momento el estado de la carga
                cargado = false;
                if (sensores.bateria >= 900)
                    cargado = true;

                break;

            case actTURN_L:
                brujula = (brujula + 6) % 8;
                girar = rand() % 4;

                cargado = false;
                if (sensores.bateria >= 900)
                    cargado = true;

                break;

            case actTURN_R:
                brujula = (brujula + 2) % 8;
                girar = rand() % 4;

                cargado = false;
                if (sensores.bateria >= 900)
                    cargado = true;

                break;

            case actSEMITURN_L:
                brujula = (brujula + 7) % 8;
                girar = rand() % 4;

                cargado = false;
                if (sensores.bateria >= 900)
                    cargado = true;
                break;

            case actSEMITURN_R:
                brujula = (brujula + 1) % 8;
                girar = rand() % 4;

                cargado = false;
                if (sensores.bateria >= 900)
                    cargado = true;

                break;

            case actIDLE:
                cargado = false;
                if (sensores.bateria >= 2500)
                    cargado = true;

                break;
        }


        if (sensores.terreno[0] == 'D') {
            actual.zapatillas = true;
            actual.bikini = false;
            cantobjetos++;
        }

        if (sensores.terreno[0] == 'K') {
            actual.bikini = true;
            actual.zapatillas = false;
            cantobjetos++;
        }

        if (sensores.reset || sensores.colision)
            bien_situado = false;

        if (ultimaAccion == actWHEREIS || sensores.nivel == 3) {
            bien_situado = true;
            fil = sensores.posF;
            col = sensores.posC;
            brujula = sensores.sentido;
        }

        if (bien_situado) {
            mapaResultado[fil][col] = sensores.terreno[0];
            if (brujula == 0) {                                         //Mirando al norte
                int k = 1;
                for (int i = -1; i <= 1; i++) {
                    mapaResultado[fil - 1][col + i] = sensores.terreno[k];
                    k++;
                }
                for (int i = -2; i <= 2; i++) {
                    mapaResultado[fil - 2][col + i] = sensores.terreno[k];
                    k++;
                }
                for (int i = -3; i <= 3; i++) {
                    mapaResultado[fil - 3][col + i] = sensores.terreno[k];
                    k++;
                }
            } else if (brujula == 1) {                                    //Mirando al noreste
                mapaResultado[fil - 1][col + 0] = sensores.terreno[1];
                mapaResultado[fil - 1][col + 1] = sensores.terreno[2];
                mapaResultado[fil - 0][col + 1] = sensores.terreno[3];
                mapaResultado[fil - 2][col + 0] = sensores.terreno[4];
                mapaResultado[fil - 2][col + 1] = sensores.terreno[5];
                mapaResultado[fil - 2][col + 2] = sensores.terreno[6];
                mapaResultado[fil - 1][col + 2] = sensores.terreno[7];
                mapaResultado[fil - 0][col + 2] = sensores.terreno[8];
                mapaResultado[fil - 3][col + 0] = sensores.terreno[9];
                mapaResultado[fil - 3][col + 1] = sensores.terreno[10];
                mapaResultado[fil - 3][col + 2] = sensores.terreno[11];
                mapaResultado[fil - 3][col + 3] = sensores.terreno[12];
                mapaResultado[fil - 2][col + 3] = sensores.terreno[13];
                mapaResultado[fil - 1][col + 3] = sensores.terreno[14];
                mapaResultado[fil - 0][col + 3] = sensores.terreno[15];

            } else if (brujula == 2) {                                    //Mirando al este
                int k = 1;
                for (int i = -1; i <= 1; i++) {
                    mapaResultado[fil + i][col + 1] = sensores.terreno[k];
                    k++;
                }
                for (int i = -2; i <= 2; i++) {
                    mapaResultado[fil + i][col + 2] = sensores.terreno[k];
                    k++;
                }
                for (int i = -3; i <= 3; i++) {
                    mapaResultado[fil + i][col + 3] = sensores.terreno[k];
                    k++;
                }
            } else if (brujula == 3) {                                    //Mirando al sureste
                mapaResultado[fil + 0][col + 1] = sensores.terreno[1];
                mapaResultado[fil + 1][col + 1] = sensores.terreno[2];
                mapaResultado[fil + 1][col + 0] = sensores.terreno[3];
                mapaResultado[fil + 0][col + 2] = sensores.terreno[4];
                mapaResultado[fil + 1][col + 2] = sensores.terreno[5];
                mapaResultado[fil + 2][col + 2] = sensores.terreno[6];
                mapaResultado[fil + 2][col + 1] = sensores.terreno[7];
                mapaResultado[fil + 2][col + 0] = sensores.terreno[8];
                mapaResultado[fil + 0][col + 3] = sensores.terreno[9];
                mapaResultado[fil + 1][col + 3] = sensores.terreno[10];
                mapaResultado[fil + 2][col + 3] = sensores.terreno[11];
                mapaResultado[fil + 3][col + 3] = sensores.terreno[12];
                mapaResultado[fil + 3][col + 2] = sensores.terreno[13];
                mapaResultado[fil + 3][col + 1] = sensores.terreno[14];
                mapaResultado[fil + 3][col + 0] = sensores.terreno[15];
            } else if (brujula == 4) {                                   //Mirando al sur
                int k = 1;
                for (int i = -1; i <= 1; i++) {
                    mapaResultado[fil + 1][col - i] = sensores.terreno[k];
                    k++;
                }
                for (int i = -2; i <= 2; i++) {
                    mapaResultado[fil + 2][col - i] = sensores.terreno[k];
                    k++;
                }
                for (int i = -3; i <= 3; i++) {
                    mapaResultado[fil + 3][col - i] = sensores.terreno[k];
                    k++;
                }
            } else if (brujula == 5) {                                     //Mirando al suroeste
                mapaResultado[fil + 1][col - 0] = sensores.terreno[1];
                mapaResultado[fil + 1][col - 1] = sensores.terreno[2];
                mapaResultado[fil + 0][col - 1] = sensores.terreno[3];
                mapaResultado[fil + 2][col - 0] = sensores.terreno[4];
                mapaResultado[fil + 2][col - 1] = sensores.terreno[5];
                mapaResultado[fil + 2][col - 2] = sensores.terreno[6];
                mapaResultado[fil + 1][col - 2] = sensores.terreno[7];
                mapaResultado[fil + 0][col - 2] = sensores.terreno[8];
                mapaResultado[fil + 3][col - 0] = sensores.terreno[9];
                mapaResultado[fil + 3][col - 1] = sensores.terreno[10];
                mapaResultado[fil + 3][col - 2] = sensores.terreno[11];
                mapaResultado[fil + 3][col - 3] = sensores.terreno[12];
                mapaResultado[fil + 2][col - 3] = sensores.terreno[13];
                mapaResultado[fil + 1][col - 3] = sensores.terreno[14];
                mapaResultado[fil + 0][col - 3] = sensores.terreno[15];

            } else if (brujula == 6) {                                   //Mirando al oeste
                int k = 1;
                for (int i = -1; i <= 1; i++) {
                    mapaResultado[fil - i][col - 1] = sensores.terreno[k];
                    k++;
                }
                for (int i = -2; i <= 2; i++) {
                    mapaResultado[fil - i][col - 2] = sensores.terreno[k];
                    k++;
                }
                for (int i = -3; i <= 3; i++) {
                    mapaResultado[fil - i][col - 3] = sensores.terreno[k];
                    k++;
                }
            } else if (brujula == 7) {                                      //Mirando al noroeste
                mapaResultado[fil - 0][col - 1] = sensores.terreno[1];
                mapaResultado[fil - 1][col - 1] = sensores.terreno[2];
                mapaResultado[fil - 1][col - 0] = sensores.terreno[3];
                mapaResultado[fil - 0][col - 2] = sensores.terreno[4];
                mapaResultado[fil - 1][col - 2] = sensores.terreno[5];
                mapaResultado[fil - 2][col - 2] = sensores.terreno[6];
                mapaResultado[fil - 2][col - 1] = sensores.terreno[7];
                mapaResultado[fil - 2][col - 0] = sensores.terreno[8];
                mapaResultado[fil - 0][col - 3] = sensores.terreno[9];
                mapaResultado[fil - 1][col - 3] = sensores.terreno[10];
                mapaResultado[fil - 2][col - 3] = sensores.terreno[11];
                mapaResultado[fil - 3][col - 3] = sensores.terreno[12];
                mapaResultado[fil - 3][col - 2] = sensores.terreno[13];
                mapaResultado[fil - 3][col - 1] = sensores.terreno[14];
                mapaResultado[fil - 3][col - 0] = sensores.terreno[15];
            }
        }

        if(sensores.nivel == 3){
            if ((sensores.terreno[1] != 'B' && sensores.terreno[1] != 'A' && sensores.terreno[1] != 'P') ||
                (sensores.terreno[2] != 'B' && sensores.terreno[2] != 'A' && sensores.terreno[2] != 'P') ||
                (sensores.terreno[3] != 'B' && sensores.terreno[3] != 'A' && sensores.terreno[3] != 'P')) {
                salirb = 0;
                salira = 0;
            }

            for (int i = 15; i >= 0; i--) {
                if (sensores.terreno[i] == 'X') {
                    posBateria.first = true;
                    posicionarObjetivo(actual, posBateria.second, i);
                }
                if (sensores.terreno[i] == 'K') {
                    posBikini.first = true;
                    posicionarObjetivo(actual, posBikini.second, i);
                }
                if (sensores.terreno[i] == 'D') {
                    posZapatillas.first = true;
                    posicionarObjetivo(actual, posZapatillas.second, i);
                }
            }


            //Hacer deriverativo ir a por zapatillas si no tiene ningun objeto,
            //Hacer deriverativo ir a una zona de carga si queda poca batería
            //Hacer deriverativo ir a zonas no exploradas

            if (!cargado && posBateria.first)
                objetivos.push_front(posBateria.second);

            if ((!actual.zapatillas && (cantB > cantA) || (!actual.zapatillas && !actual.bikini)) &&
                ultimaAccion != actIDLE && posZapatillas.first && cantobjetos < 30)
                objetivos.push_front(posZapatillas.second);

            if ((!actual.bikini && (cantA > cantB) || (!actual.zapatillas && !actual.bikini)) && ultimaAccion != actIDLE &&
                posBikini.first && cantobjetos < 30)
                objetivos.push_front(posBikini.second);

            if (!hayPlan && !objetivos.empty()) {
                hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);
            }

            accion = actIDLE;
            if (hayPlan && plan.size() > 0) {   //Hay un plan no vacio
                accion = plan.front();      //Tomo la siguiente acción del plan
                plan.erase(plan.begin()); //Eliminamos la acción del plan
                if (accion == actFORWARD && HayObstaculoDelante(actual)) {
                    accion = actSEMITURN_R;
                    hayPlan = false;
                }
            } else {
                cout << "No se pudo encontrar un plan\n";
                hayPlan = false;
                if (!bien_situado && sensores.nivel != 3)
                    accion = actWHEREIS;
                else if (sensores.superficie[2] == 'a' || sensores.superficie[2] == 'l')          //Si veo aldeano o lobo
                    accion = actTURN_R;
                else if(((sensores.terreno[1] == 'P' || sensores.terreno[1] == 'M') && (sensores.terreno[2] != 'P' && sensores.terreno[2] != 'M')
                        && (sensores.terreno[3] == 'P' || sensores.terreno[3] == 'M')) ||
                        ((sensores.terreno[5] == 'P' || sensores.terreno[5] == 'M') && (sensores.terreno[6] != 'P' && sensores.terreno[6] != 'M')
                        && (sensores.terreno[7] == 'P' || sensores.terreno[7] == 'M')) && !HayObstaculoDelante(actual)){
                    accion = actFORWARD;
                }else if ((sensores.terreno[3] == 'M' && sensores.terreno[7] != 'M' && sensores.terreno[13] == 'M' &&
                             sensores.terreno[7] != 'P' && sensores.superficie[7] == '_' && sensores.terreno[2] != 'M') ||
                            (sensores.terreno[3] == 'P' && sensores.terreno[7] != 'P' && sensores.terreno[8] != 'P'
                             && sensores.terreno[13] == 'P' && sensores.terreno[8] != 'M' && sensores.terreno[7] != 'M' &&
                             sensores.superficie[7] == '_' && sensores.terreno[2] != 'M') || saliendo > 0) {
                    if (saliendo <= 1) {             //Se hará dos veces
                        if (!HayObstaculoDelante(actual)) {
                            accion = actFORWARD;
                            saliendo++;
                        } else {
                            accion = actTURN_L;
                            saliendo = 0;
                        }
                    } else if (saliendo == 2) {
                        accion = actTURN_R;
                        saliendo = 0;
                    }
                } else if ((sensores.terreno[1] == 'M' && sensores.terreno[5] != 'M' && sensores.terreno[11] == 'M' &&
                            sensores.terreno[5] != 'P' && sensores.superficie[5] == '_' && sensores.terreno[2] != 'M') ||
                           (sensores.terreno[1] == 'P' && sensores.terreno[5] != 'P' && sensores.terreno[4] != 'P'
                            && sensores.terreno[11] == 'P' && sensores.terreno[5] != 'M' && sensores.terreno[4] != 'M'
                            && sensores.superficie[5] == '_' && sensores.terreno[2] != 'M') || saliendo1 > 0) {
                    if (saliendo1 <= 1) {             //Se hará dos veces
                        if (!HayObstaculoDelante(actual)) {
                            accion = actFORWARD;
                            saliendo1++;
                        } else {
                            saliendo1 = 0;
                            accion = actTURN_R;
                        }
                    } else if (saliendo1 == 2) {
                        accion = actTURN_L;
                        saliendo1 = 0;
                    }
                } else if (!cargado && sensores.terreno[0] == 'X') {
                    accion = actIDLE;
                } else if ((((sensores.terreno[0] == 'B' && !actual.zapatillas) ||
                             (sensores.terreno[0] == 'A' && !actual.bikini)) &&
                            //Para salir de zonas rodeadas de agua/bosque y en su campo
                            (sensores.terreno[12] != 'B' && sensores.terreno[12] != 'A') &&
                            //enfrente hay zona que no es agua o bosque
                            (sensores.terreno[2] == 'B' || sensores.terreno[2] == 'A'))
                           || (sensores.terreno[2] == 'B' && sensores.terreno[1] == 'B' && sensores.terreno[3] == 'B' &&
                               sensores.terreno[0] == 'K' && !actual.zapatillas) && sensores.superficie[2] == '_' &&
                              sensores.terreno[2] != 'M' && sensores.terreno[2] != 'P') {
                    accion = actFORWARD;
                } else if ((sensores.terreno[2] == 'B' && sensores.terreno[3] == 'B' && sensores.terreno[1] == 'B' &&
                            !actual.zapatillas)) {
                    if (salirb == 0) {
                        accion = actTURN_R;
                        salirb++;
                    } else if (sensores.terreno[2] != 'M' && sensores.superficie[2] == '_' && sensores.terreno[2] != 'P')
                        accion = actFORWARD;
                    else
                        accion = actTURN_R;

                } else if ((sensores.terreno[2] == 'A' && sensores.terreno[3] == 'A' && sensores.terreno[1] == 'A' &&
                            !actual.bikini)) {
                    if (salira <= 1) {
                        accion = actTURN_R;
                        salira++;
                    } else if (sensores.terreno[2] != 'M' && sensores.superficie[2] == '_' && sensores.terreno[2] != 'P')
                        accion = actFORWARD;
                    else
                        accion = actTURN_R;

                } else if (sensores.terreno[6] == 'B' && sensores.terreno[3] == 'B' && sensores.terreno[1] != 'B') {
                    accion = actTURN_R;
                } else if (avanzadas >= n_avanzadas || sensores.terreno[2] == 'P' || sensores.terreno[2] == 'M' ||
                           (sensores.terreno[2] == 'A' && !actual.bikini) ||
                           (sensores.terreno[2] == 'B' && !actual.zapatillas)) {
                    avanzadas = 0;

                    if (girar == 0)
                        accion = actSEMITURN_R;
                    else if (girar == 1)
                        accion = actTURN_R;
                    else if (girar == 2)
                        accion = actSEMITURN_L;
                    else
                        accion = actTURN_L;

                } else if (((sensores.terreno[2] == 'T' || sensores.terreno[2] == 'S' || sensores.terreno[2] == 'G' ||
                             sensores.terreno[2] == 'D' || sensores.terreno[2] == 'K' || sensores.terreno[2] == 'X' ||
                             (sensores.terreno[2] == 'B' && actual.zapatillas) ||
                             (sensores.terreno[2] == 'A' && actual.bikini) || (sensores.terreno[2] == 'D' && cantobjetos < 30))
                            && sensores.superficie[2] == '_' && sensores.terreno[2] != 'P')) {
                    accion = actFORWARD;
                    avanzadas++;
                }
            }
        }else if(sensores.nivel == 4){
            actual.fila = fil; actual.columna = col; actual.orientacion = brujula;
            if(inicio_ronda){
                objetivosSeguidos.clear();
                objetivosSeguidos = objetivos;
                inicio_ronda = false;
            }

            if(objetivosSeguidos.front().fila == actual.fila && objetivosSeguidos.front().columna == actual.columna && !objetivosV[0]){
                objetivosSeguidos.erase(objetivosSeguidos.begin());
                plan.clear();
                hayPlan = false;
                objetivosV[0] = true;
            }

            if(objetivos.front().fila == actual.fila && objetivos.front().columna == actual.columna && !objetivosV[0]){
                objetivos.erase(objetivos.begin());
                plan.clear();
                hayPlan = false;
                objetivosV[0] = true;
            }

            if (plan.empty() && bien_situado) {
                hayPlan = pathFinding(sensores.nivel, actual, objetivosSeguidos, plan);
            }

            accion = actIDLE;
            if (hayPlan && plan.size() > 0) {   //Hay un plan no vacio
                accion = plan.front();      //Tomo la siguiente acción del plan
                plan.erase(plan.begin()); //Eliminamos la acción del plan
                if (accion == actFORWARD && HayObstaculoDelante(actual)) {
                    hayPlan = false;
                    plan.clear();
                    accion = actIDLE;
                }
            } else {
                cout << "No se pudo encontrar un plan\n";
                hayPlan = false;
                plan.clear();
                if(!bien_situado){
                    accion = actWHEREIS;
                }else
                    accion = actTURN_R;
            }
        }
    }
    cantA = calcularCantidadCasilla('A');
    cantB = calcularCantidadCasilla('B');

    ultimaAccion = accion;
    cout << "FIL: " << actual.fila << " COL: " << actual.columna << endl;
    cout << "FILO: " << objetivosSeguidos.front().fila << " COLO: " << objetivosSeguidos.front().columna << endl;
    cout << "Accion: " << accion << endl;
    return accion;
}

// Llama al algoritmo de busqueda que se usara en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan)
{
    estado un_objetivo;
	switch (level)
	{
	case 0:
		cout << "Demo\n";
		un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Profundidad(origen, un_objetivo, plan);
		break;

	case 1:
		cout << "Optimo numero de acciones\n";
        un_objetivo = objetivos.front();
        cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
        return pathFinding_Anchura(origen, un_objetivo, plan);
		break;
	case 2:
		cout << "Optimo en coste\n";
		// Incluir aqui la llamada al busqueda de costo uniforme/A*
        un_objetivo = objetivos.front();
        cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
        return pathFinding_algEstrella(origen, un_objetivo, plan);
		break;
	case 3:
		cout << "Reto 1: Descubrir el mapa\n";
        un_objetivo = objetivos.front();
        cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
        return pathFinding_algEstrella(origen, un_objetivo, plan);
		break;
	case 4:
		cout << "Reto 2: Maximizar objetivos\n";
        un_objetivo = objetivos.front();
        cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
        return pathFinding_algEstrella(origen, un_objetivo, plan);
		break;
	}
	return false;
}

//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el codigo en caracter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla)
{
	if (casilla == 'P' or casilla == 'M')
		return true;
	else
		return false;
}

// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st)
{
	int fil = st.fila, col = st.columna;

	// calculo cual es la casilla de delante del agente
	switch (st.orientacion)
	{
	case 0:
		fil--;
		break;
	case 1:
		fil--;
		col++;
		break;
	case 2:
		col++;
		break;
	case 3:
		fil++;
		col++;
		break;
	case 4:
		fil++;
		break;
	case 5:
		fil++;
		col--;
		break;
	case 6:
		col--;
		break;
	case 7:
		fil--;
		col--;
		break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil < 0 or fil >= mapaResultado.size())
		return true;
	if (col < 0 or col >= mapaResultado[0].size())
		return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col]))
	{
		// No hay obstaculo, actualizo el parametro st poniendo la casilla de delante.
		st.fila = fil;
		st.columna = col;
		return false;
	}
	else
	{
		return true;
	}
}

struct ComparaEstados
{
	bool operator()(const estado &a, const estado &n) const
	{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};

// Implementación de la busqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados; // Lista de Cerrados
	stack<nodo> Abiertos;				  // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	Abiertos.push(current);

	while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			Abiertos.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			Abiertos.push(hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (Cerrados.find(hijoForward.st) == Cerrados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.top();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Sacar por la consola la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			cout << "A ";
		}
		else if (*it == actTURN_R)
		{
			cout << "D ";
		}
		else if (*it == actSEMITURN_R)
		{
			cout << "d ";
		}
		else if (*it == actTURN_L)
		{
			cout << "I ";
		}
		else if (*it == actSEMITURN_L)
		{
			cout << "I ";
		}
		else
		{
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

// Funcion auxiliar para poner a 0 todas las casillas de una matriz
void AnularMatriz(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan)
{
	AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			switch (cst.orientacion)
			{
			case 0:
				cst.fila--;
				break;
			case 1:
				cst.fila--;
				cst.columna++;
				break;
			case 2:
				cst.columna++;
				break;
			case 3:
				cst.fila++;
				cst.columna++;
				break;
			case 4:
				cst.fila++;
				break;
			case 5:
				cst.fila++;
				cst.columna--;
				break;
			case 6:
				cst.columna--;
				break;
			case 7:
				cst.fila--;
				cst.columna--;
				break;
			}
			mapaConPlan[cst.fila][cst.columna] = 1;
		}
		else if (*it == actTURN_R)
		{
			cst.orientacion = (cst.orientacion + 2) % 8;
		}
		else if (*it == actSEMITURN_R)
		{
			cst.orientacion = (cst.orientacion + 1) % 8;
		}
		else if (*it == actTURN_L)
		{
			cst.orientacion = (cst.orientacion + 6) % 8;
		}
		else if (*it == actSEMITURN_L)
		{
			cst.orientacion = (cst.orientacion + 7) % 8;
		}
		it++;
	}
}

//Busqueda en anchura nivel 1
//Código modificado a partir del de busqueda en profundidad.
bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan){
    // Borro la lista
    cout << "Calculando plan\n";
    plan.clear();
    set<estado, ComparaEstados> Cerrados; // Lista de Cerrados
    queue<nodo> Abiertos;				  // Lista de Abiertos

    nodo current;
    current.st = origen;
    current.secuencia.empty();

    Abiertos.push(current);

    while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
    {

        Abiertos.pop();
        Cerrados.insert(current.st);

        // Generar descendiente de girar a la derecha 90 grados
        nodo hijoTurnR = current;
        hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
        if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
        {
            hijoTurnR.secuencia.push_back(actTURN_R);
            Abiertos.push(hijoTurnR);
        }

        // Generar descendiente de girar a la derecha 45 grados
        nodo hijoSEMITurnR = current;
        hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
        if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
        {
            hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
            Abiertos.push(hijoSEMITurnR);
        }

        // Generar descendiente de girar a la izquierda 90 grados
        nodo hijoTurnL = current;
        hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
        if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
        {
            hijoTurnL.secuencia.push_back(actTURN_L);
            Abiertos.push(hijoTurnL);
        }

        // Generar descendiente de girar a la izquierda 45 grados
        nodo hijoSEMITurnL = current;
        hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
        if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
        {
            hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
            Abiertos.push(hijoSEMITurnL);
        }

        // Generar descendiente de avanzar
        nodo hijoForward = current;
        if (!HayObstaculoDelante(hijoForward.st))
        {
            if (Cerrados.find(hijoForward.st) == Cerrados.end())
            {
                hijoForward.secuencia.push_back(actFORWARD);
                Abiertos.push(hijoForward);
            }
        }

        // Tomo el siguiente valor de la Abiertos
        if (!Abiertos.empty())
        {
            current = Abiertos.front();
        }
    }

    cout << "Terminada la busqueda\n";

    if (current.st.fila == destino.fila and current.st.columna == destino.columna)
    {
        cout << "Cargando el plan\n";
        plan = current.secuencia;
        cout << "Longitud del plan: " << plan.size() << endl;
        PintaPlan(plan);
        // ver el plan en el mapa
        VisualizaPlan(origen, plan);
        return true;
    }
    else
    {
        cout << "No encontrado plan\n";
    }

    return false;
}


//Calcular el coste de un movimiento
//coste para moverse a una celda
void ComportamientoJugador::coste (nodo &n, Action act){
    int coste = 0;
    char casilla = mapaResultado[n.st.fila][n.st.columna];

    if(act == actIDLE){
        n.coste = coste;
    }else{
        coste = 1; //Por defecto será coste 1 todas las casillas
        if(casilla == 'A'){
            if(act == actFORWARD){
                coste = 200;
                if(n.st.bikini)
                    coste = 10;
            }else if(act == actTURN_R || act == actTURN_L){
                coste = 500;
                if(n.st.bikini)
                    coste = 5;
            }else if(act == actSEMITURN_R || act == actSEMITURN_L){
                coste = 300;
                if(n.st.bikini)
                    coste = 2;
            }
        }else if(casilla == 'B'){
            if(act == actFORWARD){
                coste = 100;
                if(n.st.zapatillas)
                    coste = 15;
            }else if(act == actTURN_R || act == actTURN_L){
                coste = 3;
                if(n.st.zapatillas)
                    coste = 1;
            }else if(act == actSEMITURN_R || act == actSEMITURN_L){
                coste = 2;
                if(n.st.zapatillas)
                    coste = 1;
            }
        }else if(casilla == 'T'){
            if(act == actFORWARD){
                coste = 2;
            }else if(act == actTURN_R || act == actTURN_L){
                coste = 2;
            }else if(act == actSEMITURN_R || act == actSEMITURN_L){
                coste = 1;
            }
        }
    }
    n.coste = coste;
}

//Compara estados para nivel 2
struct ComparaEstados2{
    bool operator()(const estado &a, const estado &n) const
    {
        if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
            (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion) or
            (a.fila == n.fila && a.columna == n.columna && a.orientacion == n.orientacion && a.bikini > n.bikini) or
            (a.fila == n.fila && a.columna == n.columna && a.orientacion == n.orientacion && a.bikini == n.bikini && a.zapatillas > n.zapatillas))
            return true;
        else
            return false;
    }
};

//Nivel costeUniforme
bool ComportamientoJugador::pathFinding_costeUniforme(const estado &origen, const estado &destino, list<Action> &plan) {
    // Borro la lista
    cout << "Calculando plan\n";
    plan.clear();
    set<estado, ComparaEstados2> Cerrados; // Lista de Cerrados
    priority_queue<nodo> Abiertos;		   // Lista de Abiertos

    nodo current;
    current.st = origen;
    current.secuencia.empty();

    //Inicialización de datos
    current.st.zapatillas = false;
    current.st.bikini = false;
    coste(current, actIDLE);
    current.valora = current.coste;

    Abiertos.push(current);

    while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
    {

        Abiertos.pop();
        Cerrados.insert(current.st);

        if(mapaResultado[current.st.fila][current.st.columna] == 'D'){
           current.st.zapatillas = true;
           current.st.bikini = false;
        }

        if(mapaResultado[current.st.fila][current.st.columna] == 'K'){
            current.st.bikini = true;
            current.st.zapatillas = false;
        }

        // Generar descendiente de girar a la derecha 90 grados
        nodo hijoTurnR = current;
        hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
        coste(hijoTurnR, actTURN_R);
        hijoTurnR.coste = hijoTurnR.coste + current.coste;
        hijoTurnR.valora = hijoTurnR.coste;
        if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
        {
            hijoTurnR.secuencia.push_back(actTURN_R);
            Abiertos.push(hijoTurnR);
        }

        // Generar descendiente de girar a la derecha 45 grados
        nodo hijoSEMITurnR = current;
        hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
        coste(hijoSEMITurnR, actSEMITURN_R);
        hijoSEMITurnR.coste = hijoSEMITurnR.coste + current.coste;
        hijoSEMITurnR.valora = hijoSEMITurnR.coste;
        if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
        {
            hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
            Abiertos.push(hijoSEMITurnR);
        }

        // Generar descendiente de girar a la izquierda 90 grados
        nodo hijoTurnL = current;
        hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
        coste(hijoTurnL, actTURN_L);
        hijoTurnL.coste = hijoTurnL.coste + current.coste;
        hijoTurnL.valora = hijoTurnL.coste;
        if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
        {
            hijoTurnL.secuencia.push_back(actTURN_L);
            Abiertos.push(hijoTurnL);
        }

        // Generar descendiente de girar a la izquierda 45 grados
        nodo hijoSEMITurnL = current;
        hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
        coste(hijoSEMITurnL, actSEMITURN_L);
        hijoSEMITurnL.coste = hijoSEMITurnL.coste + current.coste;
        hijoSEMITurnL.valora = hijoSEMITurnL.coste;
        if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
        {
            hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
            Abiertos.push(hijoSEMITurnL);
        }

        // Generar descendiente de avanzar
        nodo hijoForward = current;
        coste(hijoForward, actFORWARD);
        hijoForward.coste = hijoForward.coste + current.coste;
        hijoForward.valora = hijoForward.coste;
        if (!HayObstaculoDelante(hijoForward.st))
        {
            if (Cerrados.find(hijoForward.st) == Cerrados.end())
            {
                hijoForward.secuencia.push_back(actFORWARD);
                Abiertos.push(hijoForward);
            }
        }

        // Tomo el siguiente valor de la Abiertos
        if (!Abiertos.empty())
        {
            current = Abiertos.top();
        }
    }

    cout << "Terminada la busqueda\n";

    if (current.st.fila == destino.fila and current.st.columna == destino.columna)
    {
        cout << "Cargando el plan\n";
        plan = current.secuencia;
        cout << "Longitud del plan: " << plan.size() << endl;
        PintaPlan(plan);
        // ver el plan en el mapa
        VisualizaPlan(origen, plan);
        return true;
    }
    else
    {
        cout << "No encontrado plan\n";
    }

    return false;
}

void ComportamientoJugador::posicionarObjetivo(const estado actual, estado &objetivo, int pos){
    //Por defecto
    objetivo.fila = actual.fila;
    objetivo.columna = objetivo.columna;

    if(actual.orientacion == 0){        //Norte
        if(pos == 1){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna-1;}
        else if(pos == 2){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna;}
        else if(pos == 3){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna+1;}
        else if(pos == 4){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna-2;}
        else if(pos == 5){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna-1;}
        else if(pos == 6){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna;}
        else if(pos == 7){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna+1;}
        else if(pos == 8){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna+2;}
        else if(pos == 9){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna-3;}
        else if(pos == 10){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna-2;}
        else if(pos == 11){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna-1;}
        else if(pos == 12){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna;}
        else if(pos == 13){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna+1;}
        else if(pos == 14){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna+2;}
        else if(pos == 15){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna+3;}
    }else if(actual.orientacion == 1){      //Noreste
        if(pos == 1){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna;}
        else if(pos == 2){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna+1;}
        else if(pos == 3){objetivo.fila = actual.fila; objetivo.columna=actual.columna+1;}
        else if(pos == 4){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna;}
        else if(pos == 5){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna+1;}
        else if(pos == 6){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna+2;}
        else if(pos == 7){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna+2;}
        else if(pos == 8){objetivo.fila = actual.fila; objetivo.columna=actual.columna+2;}
        else if(pos == 9){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna;}
        else if(pos == 10){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna+1;}
        else if(pos == 11){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna+2;}
        else if(pos == 12){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna+3;}
        else if(pos == 13){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna+3;}
        else if(pos == 14){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna+3;}
        else if(pos == 15){objetivo.fila = actual.fila; objetivo.columna=actual.columna+3;}
    }else if(actual.orientacion == 2){      //Este
        if(pos == 1){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna+1;}
        else if(pos == 2){objetivo.fila = actual.fila; objetivo.columna=actual.columna+1;}
        else if(pos == 3){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna+1;}
        else if(pos == 4){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna+2;}
        else if(pos == 5){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna+2;}
        else if(pos == 6){objetivo.fila = actual.fila; objetivo.columna=actual.columna+2;}
        else if(pos == 7){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna+2;}
        else if(pos == 8){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna+2;}
        else if(pos == 9){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna+3;}
        else if(pos == 10){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna+3;}
        else if(pos == 11){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna+3;}
        else if(pos == 12){objetivo.fila = actual.fila; objetivo.columna=actual.columna+3;}
        else if(pos == 13){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna+3;}
        else if(pos == 14){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna+3;}
        else if(pos == 15){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna+3;}
    }else if(actual.orientacion == 3){      //Sureste
        if(pos == 1){objetivo.fila = actual.fila; objetivo.columna=actual.columna+1;}
        else if(pos == 2){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna+1;}
        else if(pos == 3){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna;}
        else if(pos == 4){objetivo.fila = actual.fila; objetivo.columna=actual.columna+2;}
        else if(pos == 5){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna+2;}
        else if(pos == 6){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna+2;}
        else if(pos == 7){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna+1;}
        else if(pos == 8){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna;}
        else if(pos == 9){objetivo.fila = actual.fila; objetivo.columna=actual.columna+3;}
        else if(pos == 10){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna+3;}
        else if(pos == 11){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna+3;}
        else if(pos == 12){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna+3;}
        else if(pos == 13){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna+2;}
        else if(pos == 14){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna+1;}
        else if(pos == 15){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna;}
    }else if(actual.orientacion == 4){      //Sur
        if(pos == 1){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna+1;}
        else if(pos == 2){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna;}
        else if(pos == 3){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna-1;}
        else if(pos == 4){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna+2;}
        else if(pos == 5){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna+1;}
        else if(pos == 6){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna;}
        else if(pos == 7){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna-1;}
        else if(pos == 8){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna-2;}
        else if(pos == 9){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna+3;}
        else if(pos == 10){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna+2;}
        else if(pos == 11){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna+1;}
        else if(pos == 12){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna;}
        else if(pos == 13){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna-1;}
        else if(pos == 14){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna-2;}
        else if(pos == 15){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna-3;}
    }else if(actual.orientacion == 5){      //Suroeste
        if(pos == 1){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna;}
        else if(pos == 2){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna-1;}
        else if(pos == 3){objetivo.fila = actual.fila; objetivo.columna=actual.columna-1;}
        else if(pos == 4){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna;}
        else if(pos == 5){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna-1;}
        else if(pos == 6){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna-2;}
        else if(pos == 7){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna-2;}
        else if(pos == 8){objetivo.fila = actual.fila; objetivo.columna=actual.columna-2;}
        else if(pos == 9){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna;}
        else if(pos == 10){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna-1;}
        else if(pos == 11){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna-2;}
        else if(pos == 12){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna-3;}
        else if(pos == 13){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna-3;}
        else if(pos == 14){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna-3;}
        else if(pos == 15){objetivo.fila = actual.fila; objetivo.columna=actual.columna-3;}
    }else if(actual.orientacion == 6){      //Oeste
        if(pos == 1){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna-1;}
        else if(pos == 2){objetivo.fila = actual.fila; objetivo.columna=actual.columna-1;}
        else if(pos == 3){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna-1;}
        else if(pos == 4){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna-2;}
        else if(pos == 5){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna-2;}
        else if(pos == 6){objetivo.fila = actual.fila; objetivo.columna=actual.columna-2;}
        else if(pos == 7){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna-2;}
        else if(pos == 8){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna-2;}
        else if(pos == 9){objetivo.fila = actual.fila+3; objetivo.columna=actual.columna-3;}
        else if(pos == 10){objetivo.fila = actual.fila+2; objetivo.columna=actual.columna-3;}
        else if(pos == 11){objetivo.fila = actual.fila+1; objetivo.columna=actual.columna-3;}
        else if(pos == 12){objetivo.fila = actual.fila; objetivo.columna=actual.columna-3;}
        else if(pos == 13){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna-3;}
        else if(pos == 14){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna-3;}
        else if(pos == 15){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna-3;}
    }else if(actual.orientacion == 7){      //Noroeste
        if(pos == 1){objetivo.fila = actual.fila; objetivo.columna=actual.columna-1;}
        else if(pos == 2){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna-1;}
        else if(pos == 3){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna;}
        else if(pos == 4){objetivo.fila = actual.fila; objetivo.columna=actual.columna-2;}
        else if(pos == 5){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna-2;}
        else if(pos == 6){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna-2;}
        else if(pos == 7){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna-1;}
        else if(pos == 8){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna;}
        else if(pos == 9){objetivo.fila = actual.fila; objetivo.columna=actual.columna-3;}
        else if(pos == 10){objetivo.fila = actual.fila-1; objetivo.columna=actual.columna-3;}
        else if(pos == 11){objetivo.fila = actual.fila-2; objetivo.columna=actual.columna-3;}
        else if(pos == 12){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna-3;}
        else if(pos == 13){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna-2;}
        else if(pos == 14){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna-1;}
        else if(pos == 15){objetivo.fila = actual.fila-3; objetivo.columna=actual.columna;}
    }

}

int ComportamientoJugador::calcularCantidadCasilla(unsigned char casilla) {
    int tam = mapaResultado.size();
    int cantidad = 0;
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(mapaResultado[i][j] == casilla)
                cantidad++;
        }
    }
    return cantidad;
}

double ComportamientoJugador::distancia(const estado &origen, const estado &destino) {
    //return abs(origen.fila-destino.fila) + abs(origen.columna-destino.columna);           //Manhattan
    return sqrt(pow(origen.fila - destino.fila, 2) + pow(origen.columna - destino.columna, 2)); //Euclidiana
}

int ComportamientoJugador::mejordistancia(const estado &origen, const pair<bool, estado> *destino) {
    int valor = 999999;
    for(int i = 0; i < n_destinos; i++){
        if(!destino[i].first && valor > distancia(origen, destino[i].second))
            valor = distancia(origen, destino[i].second);
    }
    return valor;
}

bool ComportamientoJugador::pathFinding_algEstrella(const estado &origen, const estado &destino, list<Action> &plan) {
    // Borro la lista
    cout << "Calculando plan\n";
    plan.clear();
    set<estado, ComparaEstados2> Cerrados; // Lista de Cerrados
    priority_queue<nodo> Abiertos;		   // Lista de Abiertos

    nodo current;
    current.st = origen;
    current.secuencia.empty();

    //Inicialización de datos
    current.st.zapatillas = false;
    current.st.bikini = false;
    coste(current, actIDLE);
    current.valora = current.coste + distancia(current.st, destino);

    Abiertos.push(current);

    while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
    {
        if(mapaResultado[current.st.fila][current.st.columna] == 'D'){
            current.st.zapatillas = true;
            current.st.bikini = false;
        }

        if(mapaResultado[current.st.fila][current.st.columna] == 'K'){
            current.st.bikini = true;
            current.st.zapatillas = false;
        }

        Abiertos.pop();
        Cerrados.insert(current.st);

        // Generar descendiente de girar a la derecha 90 grados
        nodo hijoTurnR = current;
        hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;


        if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
        {
            coste(hijoTurnR, actTURN_R);
            hijoTurnR.coste = hijoTurnR.coste + current.coste;
            hijoTurnR.valora = hijoTurnR.coste;
            hijoTurnR.valora += distancia(hijoTurnR.st, destino);
            hijoTurnR.secuencia.push_back(actTURN_R);
            Abiertos.push(hijoTurnR);
        }

        // Generar descendiente de girar a la derecha 45 grados
        nodo hijoSEMITurnR = current;
        hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;

        if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
        {
            coste(hijoSEMITurnR, actSEMITURN_R);
            hijoSEMITurnR.coste = hijoSEMITurnR.coste + current.coste;
            hijoSEMITurnR.valora = hijoSEMITurnR.coste;
            hijoSEMITurnR.valora += distancia(hijoSEMITurnR.st, destino);
            hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
            Abiertos.push(hijoSEMITurnR);
        }

        // Generar descendiente de girar a la izquierda 90 grados
        nodo hijoTurnL = current;
        hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
        if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
        {
            coste(hijoTurnL, actTURN_L);
            hijoTurnL.coste = hijoTurnL.coste + current.coste;
            hijoTurnL.valora = hijoTurnL.coste;
            hijoTurnL.valora += distancia(hijoTurnL.st, destino);
            hijoTurnL.secuencia.push_back(actTURN_L);
            Abiertos.push(hijoTurnL);
        }

        // Generar descendiente de girar a la izquierda 45 grados
        nodo hijoSEMITurnL = current;
        hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;

        if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
        {
            coste(hijoSEMITurnL, actSEMITURN_L);
            hijoSEMITurnL.coste = hijoSEMITurnL.coste + current.coste;
            hijoSEMITurnL.valora = hijoSEMITurnL.coste;
            hijoSEMITurnL.valora += distancia(hijoSEMITurnL.st, destino);
            hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
            Abiertos.push(hijoSEMITurnL);
        }

        // Generar descendiente de avanzar
        nodo hijoForward = current;

        if (!HayObstaculoDelante(hijoForward.st))
        {
            if (Cerrados.find(hijoForward.st) == Cerrados.end())
            {
                coste(hijoForward, actFORWARD);
                hijoForward.coste = hijoForward.coste + current.coste;
                hijoForward.valora = hijoForward.coste;
                hijoForward.valora += distancia(hijoForward.st, destino);
                hijoForward.secuencia.push_back(actFORWARD);
                Abiertos.push(hijoForward);
            }
        }

        // Tomo el siguiente valor de la Abiertos
        if (!Abiertos.empty())
        {
            current = Abiertos.top();
        }
    }

    cout << "Terminada la busqueda\n";

    if (current.st.fila == destino.fila and current.st.columna == destino.columna)
    {
        cout << "Cargando el plan\n";
        plan = current.secuencia;
        cout << "Longitud del plan: " << plan.size() << endl;
        PintaPlan(plan);
        // ver el plan en el mapa
        VisualizaPlan(origen, plan);
        return true;
    }
    else
    {
        cout << "No encontrado plan\n";
    }

    return false;
}




int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
