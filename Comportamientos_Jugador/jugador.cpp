#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>

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
    if(sensores.nivel <= 2) {
        cout << "sensores.num_destinos : " << sensores.num_destinos << endl;
        objetivos.clear();
        for (int i = 0; i < sensores.num_destinos; i++) {
            estado aux;
            aux.fila = sensores.destino[2 * i];
            aux.columna = sensores.destino[2 * i + 1];
            objetivos.push_back(aux);
        }

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
    }else{
        //nivel 3

        if(mapaResultado[actual.fila][actual.fila] == 'D'){
            actual.zapatillas = true;
            if(actual.bikini)
                actual.bikini = false;
        }

        if(mapaResultado[actual.fila][actual.fila] == 'K'){
            actual.bikini = true;
            if(actual.zapatillas)
                actual.zapatillas = false;
        }

        if(ultimaAccion == actWHEREIS || sensores.nivel == 3){
            bien_situado = true;
        }

        if(bien_situado){
            mapaResultado[actual.fila][actual.columna] = sensores.terreno[0];
            if(sensores.sentido == 0) {                                         //Mirando al norte
                int k=1;
                for(int i = -1; i <= 1; i++){
                    mapaResultado[actual.fila-1][actual.columna+i] = sensores.terreno[k]; k++;
                }
                for(int i = -2; i <= 2; i++){
                    mapaResultado[actual.fila-2][actual.columna+i] = sensores.terreno[k]; k++;
                }
                for(int i = -3; i <= 3; i++){
                    mapaResultado[actual.fila-3][actual.columna+i] = sensores.terreno[k]; k++;
                }
            }else if(sensores.sentido == 1){                                    //Mirando al noreste
                mapaResultado[actual.fila - 1][actual.columna + 0] = sensores.terreno[1];
                mapaResultado[actual.fila - 1][actual.columna + 1] = sensores.terreno[2];
                mapaResultado[actual.fila - 0][actual.columna + 1] = sensores.terreno[3];
                mapaResultado[actual.fila - 2][actual.columna + 0] = sensores.terreno[4];
                mapaResultado[actual.fila - 2][actual.columna + 1] = sensores.terreno[5];
                mapaResultado[actual.fila - 2][actual.columna + 2] = sensores.terreno[6];
                mapaResultado[actual.fila - 1][actual.columna + 2] = sensores.terreno[7];
                mapaResultado[actual.fila - 0][actual.columna + 2] = sensores.terreno[8];
                mapaResultado[actual.fila - 3][actual.columna + 0] = sensores.terreno[9];
                mapaResultado[actual.fila - 3][actual.columna + 1] = sensores.terreno[10];
                mapaResultado[actual.fila - 3][actual.columna + 2] = sensores.terreno[11];
                mapaResultado[actual.fila - 3][actual.columna + 3] = sensores.terreno[12];
                mapaResultado[actual.fila - 2][actual.columna + 3] = sensores.terreno[13];
                mapaResultado[actual.fila - 1][actual.columna + 3] = sensores.terreno[14];
                mapaResultado[actual.fila - 0][actual.columna + 3] = sensores.terreno[15];

            }else if(sensores.sentido == 2){                                    //Mirando al este
                int k=1;
                for(int i = -1; i <= 1; i++){
                    mapaResultado[actual.fila+i][actual.columna+1] = sensores.terreno[k]; k++;
                }
                for(int i = -2; i <= 2; i++){
                    mapaResultado[actual.fila+i][actual.columna+2] = sensores.terreno[k]; k++;
                }
                for(int i = -3; i <= 3; i++){
                    mapaResultado[actual.fila+i][actual.columna+3] = sensores.terreno[k]; k++;
                }
            }else if(sensores.sentido == 3){                                    //Mirando al sureste
                mapaResultado[actual.fila + 0][actual.columna + 1] = sensores.terreno[1];
                mapaResultado[actual.fila + 1][actual.columna + 1] = sensores.terreno[2];
                mapaResultado[actual.fila + 1][actual.columna + 0] = sensores.terreno[3];
                mapaResultado[actual.fila + 0][actual.columna + 2] = sensores.terreno[4];
                mapaResultado[actual.fila + 1][actual.columna + 2] = sensores.terreno[5];
                mapaResultado[actual.fila + 2][actual.columna + 2] = sensores.terreno[6];
                mapaResultado[actual.fila + 2][actual.columna + 1] = sensores.terreno[7];
                mapaResultado[actual.fila + 2][actual.columna + 0] = sensores.terreno[8];
                mapaResultado[actual.fila + 0][actual.columna + 3] = sensores.terreno[9];
                mapaResultado[actual.fila + 1][actual.columna + 3] = sensores.terreno[10];
                mapaResultado[actual.fila + 2][actual.columna + 3] = sensores.terreno[11];
                mapaResultado[actual.fila + 3][actual.columna + 3] = sensores.terreno[12];
                mapaResultado[actual.fila + 3][actual.columna + 2] = sensores.terreno[13];
                mapaResultado[actual.fila + 3][actual.columna + 1] = sensores.terreno[14];
                mapaResultado[actual.fila + 3][actual.columna + 0] = sensores.terreno[15];
            }else if(sensores.sentido == 4) {                                   //Mirando al sur
                int k=1;
                for(int i = -1; i <= 1; i++){
                    mapaResultado[actual.fila+1][actual.columna-i] = sensores.terreno[k]; k++;
                }
                for(int i = -2; i <= 2; i++){
                    mapaResultado[actual.fila+2][actual.columna-i] = sensores.terreno[k]; k++;
                }
                for(int i = -3; i <= 3; i++){
                    mapaResultado[actual.fila+3][actual.columna-i] = sensores.terreno[k]; k++;
                }
            }else if(sensores.sentido == 5){                                     //Mirando al suroeste
                mapaResultado[actual.fila + 1][actual.columna - 0] = sensores.terreno[1];
                mapaResultado[actual.fila + 1][actual.columna - 1] = sensores.terreno[2];
                mapaResultado[actual.fila + 0][actual.columna - 1] = sensores.terreno[3];
                mapaResultado[actual.fila + 2][actual.columna - 0] = sensores.terreno[4];
                mapaResultado[actual.fila + 2][actual.columna - 1] = sensores.terreno[5];
                mapaResultado[actual.fila + 2][actual.columna - 2] = sensores.terreno[6];
                mapaResultado[actual.fila + 1][actual.columna - 2] = sensores.terreno[7];
                mapaResultado[actual.fila + 0][actual.columna - 2] = sensores.terreno[8];
                mapaResultado[actual.fila + 3][actual.columna - 0] = sensores.terreno[9];
                mapaResultado[actual.fila + 3][actual.columna - 1] = sensores.terreno[10];
                mapaResultado[actual.fila + 3][actual.columna - 2] = sensores.terreno[11];
                mapaResultado[actual.fila + 3][actual.columna - 3] = sensores.terreno[12];
                mapaResultado[actual.fila + 2][actual.columna - 3] = sensores.terreno[13];
                mapaResultado[actual.fila + 1][actual.columna - 3] = sensores.terreno[14];
                mapaResultado[actual.fila + 0][actual.columna - 3] = sensores.terreno[15];

            } else if(sensores.sentido == 6) {                                   //Mirando al oeste
                int k=1;
                for(int i = -1; i <= 1; i++){
                    mapaResultado[actual.fila-i][actual.columna-1] = sensores.terreno[k]; k++;
                }
                for(int i = -2; i <= 2; i++){
                    mapaResultado[actual.fila-i][actual.columna-2] = sensores.terreno[k]; k++;
                }
                for(int i = -3; i <= 3; i++){
                    mapaResultado[actual.fila-i][actual.columna-3] = sensores.terreno[k]; k++;
                }
            }else if(sensores.sentido == 7){                                      //Mirando al noroeste
                mapaResultado[actual.fila - 0][actual.columna - 1] = sensores.terreno[1];
                mapaResultado[actual.fila - 1][actual.columna - 1] = sensores.terreno[2];
                mapaResultado[actual.fila - 1][actual.columna - 0] = sensores.terreno[3];
                mapaResultado[actual.fila - 0][actual.columna - 2] = sensores.terreno[4];
                mapaResultado[actual.fila - 1][actual.columna - 2] = sensores.terreno[5];
                mapaResultado[actual.fila - 2][actual.columna - 2] = sensores.terreno[6];
                mapaResultado[actual.fila - 2][actual.columna - 1] = sensores.terreno[7];
                mapaResultado[actual.fila - 2][actual.columna - 0] = sensores.terreno[8];
                mapaResultado[actual.fila - 0][actual.columna - 3] = sensores.terreno[9];
                mapaResultado[actual.fila - 1][actual.columna - 3] = sensores.terreno[10];
                mapaResultado[actual.fila - 2][actual.columna - 3] = sensores.terreno[11];
                mapaResultado[actual.fila - 3][actual.columna - 3] = sensores.terreno[12];
                mapaResultado[actual.fila - 3][actual.columna - 2] = sensores.terreno[13];
                mapaResultado[actual.fila - 3][actual.columna - 1] = sensores.terreno[14];
                mapaResultado[actual.fila - 3][actual.columna - 0] = sensores.terreno[15];
            }
        }
    }

    if(inicio_partida){
        int tam = mapaResultado.size();
        for(int i = 0; i < tam; i++){                                   //Rellenará el mapaResultado de precipios,
            for(int j = 0; j < tam; j++){                               //ya que es una mapa cerrado, según lo indicado
                if(i <= 2 || i >= tam-3 || j <= 2 || j >= tam-3){       // en el guión
                    mapaResultado[i][j] = 'P';
                }
            }
        }
        //accion = actWHEREIS;
        inicio_partida = false;
    }
    ultimaAccion = accion;
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
        return pathFinding_costeUniforme(origen, un_objetivo, plan);
		break;
	case 3:
		cout << "Reto 1: Descubrir el mapa\n";

        return pathFinding_reactivoDeriverativo(origen, un_objetivo, plan);
		cout << "No implementado aun\n";
		break;
	case 4:
		cout << "Reto 2: Maximizar objetivos\n";
		// Incluir aqui la llamada al algoritmo de busqueda para el Reto 2
		cout << "No implementado aun\n";
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

//Comparar estados para el nivel 2
//Ya que depende del coste
struct ComparaEstados2{
    bool operator()(const estado &a, const estado &n) const
    {
        if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
            (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion) or
            (a.fila == n.fila && a.columna == n.columna && a.orientacion == n.orientacion && a.bikini > n.bikini) or
            (a.fila == n.fila && a.columna == n.columna && a.orientacion == n.orientacion && a.zapatillas > n.zapatillas))
            return true;
        else
            return false;
    }
};
//Nivel 2 costeUniforme
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

        if(mapaResultado[current.st.fila][current.st.fila] == 'D'){
            current.st.zapatillas = true;
            if(current.st.bikini)
                current.st.bikini = false;
        }

        if(mapaResultado[current.st.fila][current.st.fila] == 'K'){
            current.st.bikini = true;
            if(current.st.zapatillas)
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
bool ComportamientoJugador::pathFinding_reactivoDeriverativo(const estado &origen, const estado &destino, list<Action> &plan) {


    nodo current;
    current.st = origen;
    current.secuencia.empty();

    return false;


}
int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
