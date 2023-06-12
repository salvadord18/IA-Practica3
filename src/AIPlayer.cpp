#include "AIPlayer.h"
#include "Parchis.h"
#include <stack>
#include <tuple>
#include <limits>
#include <cfloat>

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 3;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move()
{
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color &c_piece, int &id_piece, int &dice) const
{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
    // como lo que se muestran al final de la función.

    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
    // respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El id de mi jugador actual.
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableNormalDices(player);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(player, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
        c_piece = get<0>(current_pieces[random_id]);  // elemento de la tupla
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }

    switch (id)
    {
    case 0:
        thinkAleatorio(c_piece, id_piece, dice);
        break;
    case 1:
        thinkAleatorioMasInteligente(c_piece, id_piece, dice);
        break;
    case 2:
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
        break;
    case 3:
        thinkMejorOpcion(c_piece, id_piece, dice);
        break;
    }

    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor;                           // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch (id)
    {
    case 0:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
        break;
    case 1:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
        break;
    case 2:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);
        break;
    }
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
}

void AIPlayer::thinkAleatorio(color &c_piece, int &id_piece, int &dice) const
{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
    // como lo que se muestran al final de la función.

    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
    // respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El id de mi jugador actual.
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableNormalDices(player);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(player, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
        c_piece = get<0>(current_pieces[random_id]);  // elemento de la tupla
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
}

void AIPlayer::thinkAleatorioMasInteligente(color &c_piece, int &id_piece, int &dice) const
{
    // El número de mi jugador actual.
    int player = actual->getCurrentPlayerId();
    // Vector que almacenará los dados que se pueden usar para el movimiento.
    vector<int> current_dices_con_especiales;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;
    // Obtengo el vector de dados que puedo usar para el movimiento.
    // En este caso elijo todos, tanto normales como especiales.
    // Importante: puedo acceder a solo dados normales o especiales por separado,
    // o a todos a la vez:
    // - actual->getAvailableNormalDices(player) -> solo dados normales
    // - actual->getAvailableSpecialDices(player) -> solo dados especiales
    // - actual->getAllAvailableDices(player) -> todos los dados// Importante 2: los "available" me dan los dados que puedo usar en el turno actual.
    // Por ejemplo, si me tengo que contar 10 o 20 solo me saldrán los dados 10 y 20.
    // Puedo saber qué más dados tengo, aunque no los pueda usar en este turno, con:
    // - actual->getNormalDices(player) -> todos los dados normales
    // - actual->getSpecialDices(player) -> todos los dados especiales
    // - actual->getAllDices(player) -> todos los dados
    current_dices_con_especiales = actual->getAllAvailableDices(player);
    // En vez de elegir un dado al azar, miro primero cuáles tienen fichas que se puedan mover.
    vector<int> current_dices_que_pueden_mover_ficha;
    for (int i = 0; i < current_dices_con_especiales.size(); i++)
    {
        // Se obtiene el vector de fichas que se pueden mover para el dado elegido.
        current_pieces = actual->getAvailablePieces(player, current_dices_con_especiales[i]);
        // Si se pueden mover fichas para el dado actual, lo añado al vector de dados que pueden mover fichas.
        if (current_pieces.size() > 0)
        {
            current_dices_que_pueden_mover_ficha.push_back(current_dices_con_especiales[i]);
        }
    }
    // Si no tengo ninún dado que pueda mover fichas, paso turno con un dado al azar (la macro SKIP_TURN me permite no mover).
    if (current_dices_que_pueden_mover_ficha.size() == 0)
    {
        dice = current_dices_con_especiales[rand() % current_dices_con_especiales.size()];
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
    // En caso contrario, elijo un dado de forma aleatoria de entre los que pueden mover ficha.
    else
    {
        dice = current_dices_que_pueden_mover_ficha[rand() % current_dices_que_pueden_mover_ficha.size()];
        // Se obtiene el vector de fichas que se pueden mover para el dado elegido.
        current_pieces = actual->getAvailablePieces(player, dice);
        // Muevo una ficha al azar de entre las que se pueden mover.
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]);
        c_piece = get<0>(current_pieces[random_id]);
    }
}

void AIPlayer::thinkFichaMasAdelantada(color &c_piece, int &id_piece, int &dice) const
{
    // Elijo el dado haciendo lo mismo que el jugador anterior.
    thinkAleatorioMasInteligente(c_piece, id_piece, dice);
    // Tras llamar a esta función, ya tengo en dice el número de dado que quiero usar.
    // Ahora, en vez de mover una ficha al azar, voy a mover (o a aplicar
    // el dado especial a) la que esté más adelantada
    // (equivalentemente, la más cercana a la meta).
    int player = actual->getCurrentPlayerId();
    vector<tuple<color, int>> current_pieces = actual->getAvailablePieces(player, dice);
    int id_ficha_mas_adelantada = -1;
    color col_ficha_mas_adelantada = none;
    int min_distancia_meta = 9999;
    for (int i = 0; i < current_pieces.size(); i++)
    {
        // distanceToGoal(color, id) devuelve la distancia a la meta de la ficha [id] del color que le indique.
        color col = get<0>(current_pieces[i]);
        int id = get<1>(current_pieces[i]);
        int distancia_meta = actual->distanceToGoal(col, id);
        if (distancia_meta < min_distancia_meta)
        {
            min_distancia_meta = distancia_meta;
            id_ficha_mas_adelantada = id;
            col_ficha_mas_adelantada = col;
        }
    }
    // Si no he encontrado ninguna ficha, paso turno.
    if (id_ficha_mas_adelantada == -1)
    {
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
    // En caso contrario, moveré la ficha más adelantada.
    else
    {
        id_piece = id_ficha_mas_adelantada;
        c_piece = col_ficha_mas_adelantada;
    }
}

void AIPlayer::thinkMejorOpcion(color &c_piece, int &id_piece, int &dice) const
{
    // Vamos a mirar todos los posibles movimientos del jugador actual accediendo a los hijos del estado actual.
    // Para ello, vamos a iterar sobre los hijos con la función de Parchis getChildren().
    // Esta función devuelve un objeto de la clase ParchisBros, que es una estructura iterable
    // sobre la que se pueden recorrer todos los hijos del estado sobre el que se llama.
    ParchisBros hijos = actual->getChildren();
    bool me_quedo_con_esta_accion = false;
    // La clase ParchisBros viene con un iterador muy útil y sencillo de usar.
    // Al hacer begin() accedemos al primer hijo de la rama,
    // y cada vez que hagamos ++it saltaremos al siguiente hijo.
    // Comparando con el iterador end() podemos consultar cuándo hemos terminado de visitar los hijos.
    for (ParchisBros::Iterator it = hijos.begin();
         it != hijos.end() and !me_quedo_con_esta_accion; ++it)
    {
        Parchis siguiente_hijo = *it;                                                   // Accedemos al tablero hijo con el operador de indirección.if(siguiente_hijo.isEatingMove() or // Si he comido ficha o …
        if (siguiente_hijo.isEatingMove() or                                            // Si he comido ficha o …
            siguiente_hijo.isGoalMove() or                                              // … una ficha ha llegado a la meta o …
            (siguiente_hijo.gameOver() and siguiente_hijo.getWinner() == this->jugador) // … he ganado
        )
        {
            me_quedo_con_esta_accion = true;
            c_piece = it.getMovedColor();
            // Guardo color de la ficha movida.
            id_piece = it.getMovedPieceId(); // Guardo id de la ficha movida.
            dice = it.getMovedDiceValue();
            // Guardo número de dado movido.
        }
    }
    // Si he encontrado una acción que me interesa, la guardo en las variables pasadas por referencia.
    // (Ya lo he hecho antes, cuando les he asignado los valores con el iterador).
    // Si no, muevo la ficha más adelantada como antes.
    if (!me_quedo_con_esta_accion)
    {
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
    }
}

double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristica)(const Parchis &, int)) const
{
    double valor;
    double aux_valor;
    bool esMax = false;
    bool podado = false;

    if (actual.getCurrentPlayerId() == jugador)
    {
        esMax = true;
    }


    color c_piece_aux = none;
    int id_piece_aux = -1;
    int dice_aux = -1;

    ParchisBros hijos = actual.getChildren();

    if (profundidad < profundidad_max && !actual.gameOver())
    {
        for (const auto &hijo : hijos)
        {
            if (!podado)
            {

                aux_valor = Poda_AlfaBeta(hijo, jugador, profundidad + 1, profundidad_max, c_piece_aux, id_piece_aux, dice_aux, alpha, beta, heuristica);

                if (esMax)
                {
                    if (aux_valor > alpha)
                    {

                        valor = aux_valor;

                        if (profundidad == 0)
                        {
                            c_piece = c_piece_aux;
                            id_piece = id_piece_aux;
                            dice = dice_aux;
                        }

                        alpha = valor;
                        if (alpha >= beta)
                        {
                            podado = true;
                        }
                    }
                }
                else if (aux_valor < beta)
                {

                    valor = aux_valor;

                    if (profundidad == 0)
                    {
                        c_piece = c_piece_aux;
                        id_piece = id_piece_aux;
                        dice = dice_aux;
                    }

                    beta = valor;
                    if (alpha >= beta)
                    {
                        podado = true;
                    }
                }
            }
        }
    }
    else
    {
        valor = heuristica(actual, jugador);
    }

    return valor;
}

/*
double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristica)(const Parchis &, int)) const
{
    double valor;
    double aux_valor;
    bool esMax = false;
    bool podado = false;

    if (actual.getCurrentPlayerId() == jugador)
    {
        esMax = true;
    }

    color c_piece_aux = c_piece;
    int id_piece_aux = id_piece;
    int dice_aux = dice;

    ParchisBros hijos(actual);

    if (profundidad < profundidad_max && !actual.gameOver())
    {
        for (auto it = hijos.begin(); it != hijos.end(); ++it)
        {
            if (!podado)
            {
                bool isCallingPlayer = (it.getMovedColor() == jugador);
                aux_valor = Poda_AlfaBeta(*it, jugador, profundidad + 1, profundidad_max, c_piece_aux, id_piece_aux, dice_aux, alpha, beta, heuristica);

                if (esMax)
                {
                    if (isCallingPlayer && aux_valor > alpha)
                    {
                        valor = aux_valor;

                        if (profundidad == 0)
                        {
                            c_piece = c_piece_aux;
                            id_piece = id_piece_aux;
                            dice = dice_aux;
                        }

                        alpha = valor;
                        if (alpha >= beta)
                        {
                            podado = true;
                        }
                    }
                }
                else
                {
                    if (!isCallingPlayer && aux_valor < beta)
                    {
                        valor = aux_valor;

                        if (profundidad == 0)
                        {
                            c_piece = c_piece_aux;
                            id_piece = id_piece_aux;
                            dice = dice_aux;
                        }

                        beta = valor;
                        if (alpha >= beta)
                        {
                            podado = true;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (esMax)
        {
            valor = heuristica(actual, jugador);
        }
        else
        {
            valor = -heuristica(actual, jugador);
        }
    }
    return valor;
}
*/

/*
double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristica)(const Parchis &, int)) const
{

    std::stack<std::tuple<Parchis, int, int, int, double, double>>stack;
    stack.push(std::make_tuple(actual, jugador, 0, -1, alpha, beta));

    while(!stack.empty())
    {
        Parchis actual = std::get<0>(stack.top());
        int jugadorActual = std::get<1>(stack.top());
        int profundidad = std::get<2>(stack.top());
        int k = std::get<3>(stack.top());
        double alphaActual = std::get<4>(stack.top());
        double betaActual = std::get<5>(stack.top());
        stack.pop();

        if(profundidad == profundidad_max || actual.gameOver())
        {
            return heuristica(actual, jugadorActual);
        }

        ParchisBros hijos = actual.getChildren();
        if(k == -1)
        {
            k = 0;
        }

        if(alphaActual >= betaActual)
        {
            continue;
        }

        if(k < hijos.size())
        {
            ParchisBros hijo = hijos[k];

        }
    }
    double valor;
    double aux_valor;
    bool esMax = false;
    bool podado = false;

    if (actual.getCurrentPlayerId() == jugador)
    {
        esMax = true;
    }

    // Variables auxiliares para realizar la busqueda de todos los nodos sucesores
    color c_piece_aux = none;
    int id_piece_aux = -1;
    int dice_aux = -1;

    ParchisBros hijos = actual.getChildren();

    if (profundidad < profundidad_max && !actual.gameOver())
    {
        for (const auto &hijo : hijos)
        {

            // Variables auxiliares para realizar la busqueda en los nodos sucesores
            color c_piece_aux2 = none;
            int id_piece_aux2 = -1;
            int dice_aux2 = -1;

            aux_valor = Poda_AlfaBeta(hijo, jugador, profundidad + 1, profundidad_max, c_piece_aux2, id_piece_aux2, dice_aux2, alpha, beta, heuristica);

            if (esMax)
            {
                if (aux_valor > alpha)
                {

                    valor = aux_valor;

                    if (profundidad == 0)
                    {
                        c_piece = c_piece_aux;
                        id_piece = id_piece_aux;
                        dice = dice_aux;
                    }

                    alpha = valor;
                    if (alpha >= beta)
                    {
                        podado = true;
                    }
                }
            }
            else if (aux_valor < beta)
            {

                valor = aux_valor;

                if (profundidad == 0)
                {
                    c_piece = c_piece_aux;
                    id_piece = id_piece_aux;
                    dice = dice_aux;
                }

                beta = valor;
                if (alpha >= beta)
                {
                    podado = true;
                }
            }
        }
    }
    else
    {

        valor = heuristica(actual, jugador);
    }

    return valor;
}*/

/*
double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristica)(const Parchis &, int)) const
{
    double valor;
    double aux_valor;
    bool EsMax = false;

    bool podado = false;

    if (actual.getCurrentPlayerId() == jugador)
        EsMax = true;

    // variables auxiliares para realizar la busqueda de todos los nodos sucesores

    color c_piece_aux = none;
    int id_piece_aux = -1;
    int dice_aux = -1;

    Parchis siguiente_hijo = actual.generateNextMoveDescending(c_piece_aux, id_piece_aux, dice_aux);

    if (profundidad < profundidad_max && !actual.gameOver())
    {
        for (int i = 0; ((!(siguiente_hijo == actual)) and (!podado)); i++)
        {

            // variables auxiliares para realizar la busqueda en los nodos sucesores

            color c_piece_aux2 = none;
            int id_piece_aux2 = -1;
            int dice_aux2 = -1;

            aux_valor = Poda_AlfaBeta(siguiente_hijo, jugador, profundidad + 1, PROFUNDIDAD_ALFABETA, c_piece_aux2, id_piece_aux2, dice_aux2, alpha, beta, heuristic);

            if (i == 0)
            {
                valor = aux_valor;
                id_piece = id_piece_aux;
                dice = dice_aux;
            }

            if (EsMax)
            {
                if (aux_valor > alpha)
                {

                    valor = aux_valor;

                    if (profundidad == 0)
                    {
                        c_piece = c_piece_aux;
                        id_piece = id_piece_aux;
                        dice = dice_aux;
                    }

                    alpha = valor;
                    if (alpha >= beta)
                    {
                        podado = true;
                    }
                }
            }
            else if (aux_valor < beta)
            {

                valor = aux_valor;

                if (profundidad == 0)
                {
                    c_piece = c_piece_aux;
                    id_piece = id_piece_aux;
                    dice = dice_aux;
                }

                beta = valor;
                if (alpha >= beta)
                {
                    podado = true;
                }
            }

            siguiente_hijo = actual.begin();
        }

        // cout << "PROFUNDIDAD = " << profundidad << ", Es MAX: " << EsMax << ", devuelvo la jugada: color: " << str(c_piece) << ", ficha: " << id_piece << ", dado: " << dice << ", con valor de " << valor << endl;
    }
    else
    {

        valor = heuristic(actual, jugador);
    }

    return valor;
}
*/

/*
double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristica)(const Parchis &, int)) const
{
    double valor;
    bool esMax = (actual.getCurrentPlayerId() == jugador);
    bool podado = false;

    // Variables auxiliares para realizar la búsqueda de todos los nodos sucesores
    color c_piece_aux = none;
    int id_piece_aux = -1;
    int dice_aux = -1;

    if (profundidad == profundidad_max || actual.gameOver())
    {
        valor = heuristica(actual, jugador);
    }
    else if (esMax)
    {
        valor = -DBL_MAX;

        for (const auto &hijo : actual.getChildren())
        {
            color c_piece_aux2 = none;
            int id_piece_aux2 = -1;
            int dice_aux2 = -1;

            double aux_valor = Poda_AlfaBeta(hijo, jugador, profundidad + 1, profundidad_max, c_piece_aux2, id_piece_aux2, dice_aux2, alpha, beta, heuristica);

            if (aux_valor > valor)
            {
                valor = aux_valor;
                c_piece = std::get<0>(hijo.getLastAction());
                id_piece = std::get<1>(hijo.getLastAction());
                dice = std::get<2>(hijo.getLastAction());
            }

            alpha = std::max(alpha, valor);

            if (alpha >= beta)
            {
                podado = true;
                break;
            }
        }
    }
    else
    {
        valor = DBL_MAX;

        for (const auto &hijo : actual.getChildren())
        {
            color c_piece_aux2 = none;
            int id_piece_aux2 = -1;
            int dice_aux2 = -1;

            double aux_valor = Poda_AlfaBeta(hijo, jugador, profundidad + 1, profundidad_max, c_piece_aux2, id_piece_aux2, dice_aux2, alpha, beta, heuristica);

            if (aux_valor < valor)
            {
                valor = aux_valor;
                c_piece = std::get<0>(hijo.getLastAction());
                id_piece = std::get<1>(hijo.getLastAction());
                dice = std::get<2>(hijo.getLastAction());
            }

            beta = std::min(beta, valor);

            if (beta <= alpha)
            {
                podado = true;
                break;
            }
        }
    }

    return valor;
}
*/

double AIPlayer::MiValoracion1(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                // Valoro negativamente si la ficha está en la casilla de inicio.
                else if (estado.getBoard().getPiece(c, j).get_box().type == home)
                {
                    puntuacion_jugador -= 5;
                }
                // Valoro positivamente si la ficha está en una casilla normal.
                else if (estado.getBoard().getPiece(c, j).get_box().type == normal)
                {
                    puntuacion_jugador += 1;
                }
                // Valoro positivamente si la ficha está en una casilla del pasillo final hacia la meta.
                else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue)
                {
                    puntuacion_jugador += 3;
                }
                // Valoro positivamente si la ficha está la casilla destino o meta.
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 5;
                }
                // Valoro positivamente si es un movimiento para comer alguna ficha.
                else if (estado.isEatingMove())
                {
                    puntuacion_jugador += 10;
                }
                // Valoro positivamente si es un movimiento para llegar al destino.
                else if (estado.isGoalMove())
                {
                    puntuacion_jugador += 20;
                }
                // Valoro positivamente si es un movimiento con el dado especial Plátano.
                else if (estado.isBananaMove())
                {
                    puntuacion_jugador += 15;
                }
                // Valoro positivamente si es un movimiento con el dado especial Champiñón.
                else if (estado.isMushroomMove())
                {
                    puntuacion_jugador += 25;
                }
                // Valoro positivamente si es un movimiento con el dado especial Caparazón rojo.
                else if (estado.isRedShellMove())
                {
                    puntuacion_jugador += 30;
                }
                // Valoro positivamente si es un movimiento con el dado especial Caparazón azul.
                else if (estado.isBlueShellMove())
                {
                    puntuacion_jugador += 35;
                }
                // Valoro positivamente si es un movimiento con el dado especial Bocina.
                else if (estado.isHornMove())
                {
                    puntuacion_jugador += 40;
                }
                // Valoro positivamente si es un movimiento con el dado especial Bala.
                else if (estado.isBulletMove())
                {
                    puntuacion_jugador += 45;
                }
                // Valoro positivamente si es un movimiento con el dado especial Rayo.
                else if (estado.isShockMove())
                {
                    puntuacion_jugador += 50;
                }
                // Valoro positivamente si es un movimiento con el dado especial Boo.
                else if (estado.isBooMove())
                {
                    puntuacion_jugador += 55;
                }
                // Valoro positivamente si es un movimiento con el dado especial Estrella.
                else if (estado.isStarMove())
                {
                    puntuacion_jugador += 60;
                }
                // Valoro positivamente si es un movimiento con el dado especial Megachampiñón.
                else if (estado.isMegaMushroomMove())
                {
                    puntuacion_jugador += 65;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro negativamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_oponente++;
                }
                // Valoro positivamente si la ficha está en la casilla de inicio.
                else if (estado.getBoard().getPiece(c, j).get_box().type == home)
                {
                    puntuacion_oponente -= 5;
                }
                // Valoro negativamente si la ficha está en una casilla normal.
                else if (estado.getBoard().getPiece(c, j).get_box().type == normal)
                {
                    puntuacion_oponente += 1;
                }
                // Valoro negativamente si la ficha está en una casilla del pasillo final hacia la meta.
                else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue)
                {
                    puntuacion_oponente += 3;
                }
                // Valoro negativamente si la ficha está la casilla destino o meta.
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 5;
                }
                // Valoro negativamente si es un movimiento para comer alguna ficha.
                else if (estado.isEatingMove())
                {
                    puntuacion_oponente += 10;
                }
                // Valoro negativamente si es un movimiento para llegar al destino.
                else if (estado.isGoalMove())
                {
                    puntuacion_oponente += 20;
                }
                // Valoro negativamente si es un movimiento con el dado especial Plátano.
                else if (estado.isBananaMove())
                {
                    puntuacion_oponente += 15;
                }
                // Valoro negativamente si es un movimiento con el dado especial Champiñón.
                else if (estado.isMushroomMove())
                {
                    puntuacion_oponente += 25;
                }
                // Valoro negativamente si es un movimiento con el dado especial Caparazón rojo.
                else if (estado.isRedShellMove())
                {
                    puntuacion_oponente += 30;
                }
                // Valoro negativamente si es un movimiento con el dado especial Caparazón azul.
                else if (estado.isBlueShellMove())
                {
                    puntuacion_oponente += 35;
                }
                // Valoro negativamente si es un movimiento con el dado especial Bocina.
                else if (estado.isHornMove())
                {
                    puntuacion_oponente += 40;
                }
                // Valoro negativamente si es un movimiento con el dado especial Bala.
                else if (estado.isBulletMove())
                {
                    puntuacion_oponente += 45;
                }
                // Valoro negativamente si es un movimiento con el dado especial Rayo.
                else if (estado.isShockMove())
                {
                    puntuacion_oponente += 50;
                }
                // Valoro negativamente si es un movimiento con el dado especial Boo.
                else if (estado.isBooMove())
                {
                    puntuacion_oponente += 55;
                }
                // Valoro negativamente si es un movimiento con el dado especial Estrella.
                else if (estado.isStarMove())
                {
                    puntuacion_oponente += 60;
                }
                // Valoro negativamente si es un movimiento con el dado especial Megachampiñón.
                else if (estado.isMegaMushroomMove())
                {
                    puntuacion_oponente += 65;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

double AIPlayer::MiValoracion2(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente si la ficha está a 4 o menos casillas del destino.
                if (estado.distanceToGoal(c, j) <= 4)
                {
                    puntuacion_jugador += 4;
                }
                // Valoro negativamente si la ficha rebotó en la meta en el último movimiento.
                else if (estado.goalBounce())
                {
                    puntuacion_jugador -= 2;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro negativamente si la ficha está a 4 o menos casillas del destino.
                if (estado.distanceToGoal(c, j) <= 4)
                {
                    puntuacion_oponente += 4;
                }
                // Valoro positivamente si la ficha rebotó en la meta en el último movimiento.
                else if (estado.goalBounce())
                {
                    puntuacion_oponente -= 2;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}