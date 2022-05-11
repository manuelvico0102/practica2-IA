#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct estado {
  int fila;
  int columna;
  int orientacion;
  bool bikini;
  bool zapatillas;
};

struct nodo
{
    estado st;
    list<Action> secuencia;
    int coste; //Nivel 2
    int valora; //Nivel 2 y en el 3 este sera la heuristica


    inline bool operator< (const nodo& n) const{        //ordenar la cola con prioridad
        return this->valora > n.valora;
    }
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      hayPlan = false;
      inicio_partida = true;
      ultimaAccion = actIDLE;
      bien_situado = false;
      girar = 0;
      brujula = 0;
      cargado = true;
      cant_bateria = 2500;
      fil = col = 99;
      avanzadas = n_avanzadas = saliendo = saliendo1 = salira = salirb = 0;
      cantA = cantB = 0;
      posBateria.first = false;
      n_destinos = 3;
      cantobjetos = 0;
      dist = 0;
      distmin = 100000;
      objetivosV[0] = objetivosV[1] = objetivosV[3] = false;
      inicio_ronda = true;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      hayPlan = false;
      inicio_partida = true;
      ultimaAccion = actIDLE;
      bien_situado = false;
      girar = 0;
      brujula = 0;
      cargado = true;
      cant_bateria = 2500;
      fil = col = 99;
      avanzadas = n_avanzadas = saliendo = saliendo1 = salira = salirb = 0;
      cantA = cantB = 0;
      posBateria.first = false;
      n_destinos = 3;
      cantobjetos = 0;
      dist = 0;
      distmin = 100000;
      objetivosV[0] = objetivosV[1] = objetivosV[3] = false;
      inicio_ronda = true;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    estado actual;
    list<estado> objetivos;
    list<Action> plan;
    bool hayPlan;
    bool inicio_partida;
    Action ultimaAccion;
    bool bien_situado;
    int girar;
    int fil, col, brujula;
    bool cargado;
    int cant_bateria;
    int avanzadas, n_avanzadas;
    pair<bool,estado> posBateria;
    pair<bool, estado> posZapatillas;
    pair<bool, estado> posBikini;
    bool objetivosV[3];
    pair<bool,estado> aux;
    estado objPersonalizado;
    int saliendo, saliendo1, salira, salirb;
    int cantA, cantB;
    int n_destinos;
    int cantobjetos;
    double dist, distmin;
    bool inicio_ronda;
    list<estado> objetivosSeguidos;

    // Métodos privados de la clase
    bool pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_costeUniforme(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_algEstrella(const estado &origen, const estado &destino, list<Action> &plan);

    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);
    void coste(nodo &n, Action act);

    void posicionarObjetivo(const estado actual, estado &objetivo, int pos);
    int calcularCantidadCasilla(unsigned char casilla);
    double distancia(const estado &origen, const estado &destino);
    int mejordistancia(const estado &origen, const pair<bool,estado> destino[3]);

};

#endif
