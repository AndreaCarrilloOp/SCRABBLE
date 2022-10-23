#include "Tabla.h"

//Constructor y Destructor
Tabla::Tabla() {
	jugadores = new JugadorLista();
	letra = new FichaLista();
	jugadores = 0;

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {

			tabla[i][j] = new Cuadricula();
		}
	}
}

Tabla::~Tabla() {}


void Tabla::setNumJugadores(int numJugadores) {
	this->numJugadores = numJugadores;
}

int Tabla::getNumJugadores() {
	return jugadores;
}

void Tabla::setJugadoresLista(JugadorLista* l) {
	jugadores = l;
}

JugadorLista* Tabla::getJugadores() {
	return jugadores;
}

Cuadricula* Tabla::getCuadricula(int i, int j) {
	return tabla[i][j];
}

FichaLista* Tabla::getLista() {
	return letra;
}

//Metodos

//Metodo que ingresa las fichas a la "bolsa" de fichas
void Tabla::llenarBolsa() {
	//crea una nueva ficha con la cantidad correspondiente
	//su base es: Letra, valor, esEspecial, cantidad

	for (int i = 0; i < 10; i++) {//iteracion para crear las 12 fichas de A y de E
		letra->ingresaFicha(new Ficha('A', 10));
		letra->ingresaFicha(new Ficha('E', 10));
		letra->ingresaFicha(new Ficha('I', 10));
		letra->ingresaFicha(new Ficha('O', 10));
	}

	for (int i = 0; i < 4; i++) {
		letra->ingresaFicha(new Ficha('N', 4));
		letra->ingresaFicha(new Ficha('B', 4));
		letra->ingresaFicha(new Ficha('T', 4));
	}

	for (int i = 0; i < 6; i++) {
		letra->ingresaFicha(new Ficha('C', 6));
		letra->ingresaFicha(new Ficha('R', 6));
		letra->ingresaFicha(new Ficha('L', 6));
	}

}//fin del metodo



//Metodo que acondiciona el programa a la cantidad de jugadores deseados
void Tabla::agregarJugador(int cantidad) {

	if (cantidad >= 2) { //dos Jugadores
		string nombre1, nombre2;
		cout << " Digite el nombre del Jugador 1: " << endl; //ingresa el nombre
		cin >> nombre1;
		cout << " Digite el nombre del Jugador 2: " << endl;//ingresa el nombre
		cin >> nombre2;
		jugadores->ingresaJugadorInicio(new Jugador(nombre1, 0));//inserta al inicio 
		jugadores->ingresaJugadorInicio(new Jugador(nombre2, 0));//

		if (cantidad >= 3) {//tres jugadores		
			string nombre3;
			cout << " Digite el nombre del Jugador 3: " << endl;//ingresa el nombre
			cin >> nombre3;
			jugadores->ingresaJugadorInicio(new Jugador(nombre3, 0));

		}//cierre de if
	}//cierre de if

}//fin del metodo

//Metodo que 
void Tabla::repartirFichas(Jugador* j1, int cantidad) {

	srand(time(NULL));//semilla para generar aleatoriedad

	if (letra->getCantidad() > 0) { //si las fichas son mayores a 0,  las reparte

		for (int i = 0; i < cantidad; i++) {

			j1->getFicha()->ingresaFicha(letra->eliminarRetornar(rand() % letra->getCantidad() + 0));//genera aleatoriamente las fichas
		}//fin del for
	}//fin del if

	else { //Cuando se acaban las fichas, se determina el ganador
		this->hayGanador(0, true);
	}//fin del else

}//fin de metodo


//Metodo que determina que jjugador empieza la partida, mediante la aleatoriedad de la ficha
int Tabla::jugadorInicial() {

	char aux = 65;

	for (int i = 0; i < numJugadores; i++) {//Inculcamos fichas a todos los jugadores.	
		this->repartirFichas(this->getJugadores()->getJugador(i), this->getJugadores()->getJugador(i)->cantidadFichas()); //reparte 6 fichas para cada jugador
	}

	while (aux <= 84) { //ultima agregada es letra T
		for (int i = 0; i < numJugadores; i++) {
			if (this->getJugadores()->getJugador(i)->getFich()->seEncuentra(aux)) {
				return i;
			}
		}//final del for
		aux = aux + 1;
	}//final del while
	return 0;
}//fin del metodo


//Metodo que retorna el valor booleano del campo 
bool Tabla::vacio() {
	return this->getCuadricula(7, 7)->isOcupada();
}

void Tabla::palabraInicial(string palabra, char posicion, Jugador* j1) {

	for (int i = 0; i < palabra.length(); i++) { //recorre mientras el tamaño de la letra sea mayor que i

		if (posicion == 'V') {//si es vertical		
			getCuadricula(7 + i, 7)->getLetra()->setLetra(palabra[i]);//coloca la letra
			getCuadricula(7 + i, 7)->setOcupada(true);//cambia el estado
		}
		else {
			if (posicion == 'H') {//posiciones horizontales

				getCuadricula(7, 7 + i)->getLetra()->setLetra(palabra[i]);//coloca la letra
				getCuadricula(7, 7 + i)->setOcupada(true);//cambia su estado
			}

		}//fin del else

	}//fin del for

	j1->getPal()->ingresaPalabra(new Palabra(palabra));//Lista con palabras.		   

}//fin del metodo


//Metodo que extrae una letra, segun su posicion en el tablero
char Tabla::buscarLetra(int columna, int fila) {
	return getCuadricula(columna, fila)->getLetra()->getLetra();
}


//Metodo que compara la palabra ingresada en el tablero con el archivo de diccionario
bool Tabla::compararArchivo(string palabra) {

	string auxiliar = "";
	ifstream archivo;

	archivo.open("Datos.txt", ios::in);//abre el archivo diccionario
	archivo >> auxiliar;

	while (!archivo.eof()) { //mientras no este en el final del archivo	

		if (auxiliar == palabra) { //si encuentra la palabra	
			return true;//retorna un verdadero, de modo que la palabra si existe en el diccionario
		}//fin del if

		archivo >> auxiliar;//guarda la palabra en el auxiliar
	}
	archivo.close();//cierra el archivo
	return false;//si la palabra no existe en el diccionario, retorna un false, indicando que no se encontro la palabra
}//fin del metodo


//Metodo que coloca en el tablero la palabra, luego de ser verificada
bool Tabla::palabraAprobada(int columna, int fila, Jugador* j1, string palabra) {

	if (!(vacio())) { //si el campo esta libre

		for (int i = 0; i < palabra.length(); i++) { //recorre mientras el tamaño de la palabra sea mayor a i


			if (!(j1->getFicha()->encuentrada(palabra[i]))) { //si no  encuentra la palabra			
				return false;
			}
		}
		return true;//si la encuentra
	}
	else
	{
		for (int i = 0; i < palabra.length(); i++) { //recorre mientras el tamaño de la palabra sea mayor a i		

			if (!(j1->getFich()->encuentrada(palabra[i]))) { //si no  encuentra la palabra

				if (getCuadricula(x, y)->getLetra()->getLetra() != palabra[i])//y si la casilla no corresponde a la palabra
					return false;
			}
		}
	}
	return true;
}//fin del metodo


//Metodo que retorna si una posicion en el tamblero esta ocupada o no
char Tabla::posicion(int columna, int fila) {

	if (getCuadricula(columna - 1, fila)->isOcupada() || (columna != 15 && getCuadricula(columna + 1, fila)->isOcupada())) {//posiciones horizontales
		return 'H';
	}
	else if (getCuadricula(columna, fila + 1)->isOcupada() || getCuadricula(columna, fila - 1)->isOcupada()) {//posiciones verticales

		return 'V';
	}
	else {

		return 'N';
	}
}//fin del metodo

//Metodo que acomoda la palabra en el tablero, verifica bonos y asigna puntajes de bonos
bool Tabla::colocarPalabra(int x, int y, string palabra, char letra, char posicion, Jugador* j1) {

	int aux = 0;
	bool permiso = false;
	Ficha* l = NULL;

	if (posicion != 'N') { //Hay posibilidad de poner letra	

		for (int i = 0; i < palabra.length(); i++) { //Determinar que la letra esta en la palabra		

			if (palabra[i] == letra) {
				aux = i;
				permiso = true;//Empieza a revisar si despues de la letra hay campo para meter el resto de la palabra.
			}

			else if (permiso && posicion == 'V' && getCuadricula(x + i, y)->isOcupada()) {//Campo suficiente par aponer la palabra verticalmente			
				return false;
			}

			else if (permiso && posicion == 'H' && getCuadricula(x, y + i)->isOcupada()) {//Campo suficiente par aponer la palabra horizontalmente			
				return false;
			}

		}//fin del for

		for (int i = aux; i > 0; i--) {//Determina que antes de la letra seleccionada cabe el resto de la palabra.		

			if (posicion == 'V') { //Si es verticalmente			

				x = x - 1;//Recorre para atras.

				if (getCasilla(x - 1, y)->isOcupada()) {
					return false;
				}

			}//fin del if vertical

			if (posix == 'H') { //Si es horizontalmente			

				y = y - 1;//Recorre para atras.

				if (getCuadricula(x, y - 1)->isOcupada()) {
					return false;
				}
			}//fin del if horizontal

		}//fin del for

		for (int i = 0; i < palabra.length(); i++) { //Finalmente, acomoda la palabra en donde se le indique		

			if (posicion == 'V') {//vertical
				getCuadricula(x + i, y)->getLetra()->setLetra(palabra[i]);//se coloca la palabra
				getCuadricula(x + i, y)->setOcupada(true);//cambia su estado

			}// final del if verticalmente

			else if (posicion == 'H') {//horizotamente

				Cuadricula(x, y + i)->getLetra()->setLetra(palabra[i]);//coloca la palabra
				Cuadricula(x, y + i)->setOcupada(true);//cambia el estado de las casillas

			else //No tiene bono
			{
				total = total + determinarPuntaje(j1, getCasilla(x, y + i)->getLetra(), palabra[i], false, getCasilla(x, y + i)->getBono());
			}

			}//final del if horizontal

		}//fin del for

		//total = bono * total; //Si no hay bono de palabra será igual a 1 * el total de la palabra

		j1->getPalabra()->ingresaPalabra(new Palabra(palabra));//crea la ueva palabraa con el puntaje y la palabra

		//j1->setPuntaje(j1->getPuntaje() + total); //Si hay bono se multiplica el total de la palabra.

		return true;
	}//fin del if pos N

	return false;
}//fin del metodo


//Metodo que determina cual es el jugador ganador
bool Tabla::ganador(int pasar, bool sinLetras) {
	int resta = 0;

	if ((pasar == (2 * jugadores)) || sinLetras) {// Pasaron todos 2 veces consecutivas o no hay letras.	

		for (int i = 0; i < jugadores; i++) {

			for (int j = 0; j < this->getJugadores()->getJugador(i)->getFicha()->getCantidad(); j++) {//obtiene la cantidad de fichas del atril
				resta = resta + this->getJugadores()->getJugador(i)->getFicha()->getLetra(j)->getPuntaje(); //busca y guarda el puntaje de los demas jugadores
			}

			if (this->getJugadores()->getJugador(i)->getPuntaje() > 0) {
				this->getJugadores()->getJugador(i)->setPuntaje(this->getJugadores()->getJugador(i)->getPuntaje() - resta); //resta el puntaje restante a los demas jugadores
			}
		}
		return true;
	}// fin del for
	return false;
}//fin del metodo


//Metodo que controla el juego
void Tabla::juegoCompleto() {
	int turno = 0;
	int opcion = 0;
	int fila = 0;
	int columna = 0;
	int color = 249;
	int pasar = 0;
	char posicion = ' ';
	char seleccion = ' ';
	char letraSeleccionada = ' ';
	string palabra = "";
	bool jugada = true;
	bool letraCambia = false;
	Jugador* actual = NULL;
	InterfazTablero* I = new InterfazTablero();

	this->llenarBolsa();
	this->getLista()->guardarLetras();
	jugadores = I->solicitarNumeroJugadores();
	this->agregarJugadores(jugadores);
	turno = this->jugadorInicial();

	do {

		if (!ganador(pasar, false)) { //No hay ganador.		

			system("cls");

			actual = this->getJugadores()->getJugador(turno);//Definir el jugador que se encuentra jugando

			this->repartirFichas(actual, actual->cantidadFichas());//Siempre da un numero entre 0 y 7

			if (actual->getFicha()->encuentrada(' ')) {//si algun jugador tiene la ficha comodin, la asigna de una vez
				letraCambia = true;
			}
			else {
				letraCambia = false;
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); //Cambia el color del jugador.

			cout << endl << "Turno de " << actual->getNombre(); //Muestra de cual jugador es el turno

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Devuuelve el color verde.

			cout << "	QUEDAN " << this->getLista()->getCant() << " LETRAS SIN USO" << endl << endl;//Muestra la cantida dde fichas restantes

			opcion = I->menuPrincipal();//Muestra el menu principal, este lo saca de interfazTablero

			switch (opcion) {

				seleccion = ' ';

			case 1: { //Mostrar el tablero y jugar una palabra

				system("cls");
				this->mostrar();//Muestra el tablero.

				if (jugada) { //No ha jugado aun				

					cout << " A continuacion se le muestran las letras que posee el jugador. " << endl << endl;//imprme el atril del jugador
					cout << actual->getFich()->toString() << endl << endl;

					seleccion = ' ';

					while (seleccion != 'J' && seleccion != 'j' && seleccion != 'P' && seleccion != 'p') {
						cout << " Desea jugar o pasar de turno? (J/P) " << endl;//hae que el usuario decida si jugar o devolverse al menu
						cin >> seleccion;
						cout << endl << endl;
					}

					if (seleccion == 'J' || seleccion == 'j') {//El usuario realiza una jugada			

						seleccion = ' ';
						if (letraCambia) { // cambia el valor de la letra en blanco

							while (seleccion != 'Y' && seleccion != 'y' && seleccion != 'N' && seleccion != 'n') {//iteracion para mostrarle al usuario que tiene una ficha comodin
								cout << "Cuenta con una Ficha Comodin, Desea Cambiarla? (Y/N)\n";
								cin >> seleccion;
								cout << endl << endl;
							}

							if (seleccion == 'y' || seleccion == 'Y') {//El usuario digita si
								actual->getFich()->eliminarFichaEspecifica(' '); //elimina la ficha vacia del atril

								cout << "Digite la letra que desea agregar a la Ficha Comodin: \n";
								cin >> seleccion;

								while (!(letras->seEncuentra(seleccion))) { //mensaje de error, por si el usuario digita algun numero o simbolo
									cout << "Ficha No Aceptada, Digite otra\n";
									cin >> seleccion;
								}

								actual->getFicha()->ingresaFicha(letras->eliminarFichaEspecifica(seleccion)); //asigna a la ficha comodin la letra deseada
							}//fin del if
						}//fin del if de la ficha en blanco

						pasar = 0;

						if ((vacio())) {
							//Muestra al usuario las instrucciones	
							cout << " Se debe usar una letra que se encuentre en el tablero. " << endl;
							cout << " Las FILAS se ven numeradas con los numeros de AL LADO. " << endl;
							cout << " Las COLUMNAS se ven numeradas con los numeros de ARRIBA. " << endl;
							cout << " Por ejemplo. Si desea tomar la ficha amarilla  la FILA es 7 y COLUMNA 7 (7, 7). " << endl;

							seleccion = ' ';

							while (seleccion != 'Y' && seleccion != 'y') { //valida nuevmente si el usuario desde realizar una jugada

								columna = I->solicitarColumna();//solicita una columna
								fila = I->solicitarFila(); //solicita una fila

								while (buscarLetra(fila, columna) == ' ') {//si el espacio esta ocupado
									cout << " Campo seleccionado no valido. Seleccione uno valido: " << endl;//envia mensaje de error
									columna = I->solicitarColumna(); // vuelve a solicitar la fila y la columna
									fila = I->solicitarFila();
								}

								//si seencuantra, se extrae la ficha para empezar a colocar la palabra deseada
								cout << endl << "Letra extraida:	" << this->extraerLetra(fila, columna) << endl << endl; //muetra la ficha extraida

								cout << "Es esto correcto? (Y/N) " << endl;//valida si es la letra deseada
								cin >> seleccion;
							}

						}//fin del if campoVacio

						cout << " Digite la palabra que desea formar, tomando en cuenta las fichas que tiene a su favor? " << endl;//pide al usuario la palabra que desa colocar en el tablero
						cin >> palabra;

						for (int i = 0; i < palabra.length(); i++) {
							palabra[i] = toupper(palabra[i]); //pasa cada letra a mayuscula
						}

						while (!(this->palabraAceptada(fila, columna, actual, palabra)) || !(this->comparar(palabra))) {//hace hasta que la palabra sea aceptada y compara sea valido						

							if (!(this->palabraAceptada(fila, columna, actual, palabra))) {//si el usuario no forma una palabra
								cout << " No tiene las letras indicadas en su poder. Digite la palabra nuevamente. " << endl;
								cout << " Letras en su poder: " << actual->getFich()->toString() << endl << endl;//imprime el atril
							}

							else if (!(this->comparar(palabra))) {//busca la palabra en el diccionario							
								cout << " Palabra no aceptada por el diccionario. Digite una palabra diferente. " << endl;//no encuentra la palara en el diccionario
							}

							cin >> palabra;
						}//fin del while

						if (!(vacio())) { //No hay palabras creadas						

							while (posicion != 'V' && posicion != 'H') {//Se acomoda Vertical o Horizontal.

								cout << " Digite V si desea que sea vertical o H si desea horizontal." << endl;
								cin >> posicion;
							}

							//Acomodamos palabra inicialen el centro del tablero, multiplicando a su vez el doble
							this->acomodarPalabraInicial(palabra, posicion, actual);
						}//fin del if

						else {//Acomodamos palabra						
							this->acomodarPalabra(fila, columna, palabra, extraerLetra(fila, columna), this->posicion(fila, columna), actual);
						}

						actual->getPal()->guardarPalabras(turno);//guarda la palabra en el tablero
						jugada = false;

						system("cls");
						this->mostrar();//muestra el tablero con la palabra escrita por el usuario
					}//fin del if para realizar una jugada

					else {//Pasar de turno.				

						pasar = pasar + 1;

						if (turno < jugadores - 1) {
							turno = turno + 1;
							color = color + 1;
						}
						else {
							turno = 0;
							color = 249;
						}
					}
				}//Termina la jugada

				else //valida que solamente se haga un moviemiento por turno, ya se colocar una palabra, o bien cambiar las fichas
				{
					cout << endl << endl << "	No se puede jugar si ya jugo una vez o hizo cambio de palabras." << endl;
					cout << "	Cambie de turno y espere su oportunidad nuevamente. " << endl << endl;
				}

				system("pause");

				break;
			}//Final del Case 1

			case 2: { //Cambiar las fichas Actuales			

				seleccion = ' ';
				while (seleccion != 'Y' && seleccion != 'y') {

					system("cls");
					//imprime el atril
					cout << " A continuacion se le muestran las letras que posee el jugador. " << endl << endl;
					cout << actual->getFicha()->toString() << endl << endl;

					//el usuario selecciona una a una la letra que desea cambiar
					cout << " Con base en las letras que se encuentran en su poder. " << endl;
					cout << " Digite la letra que desea cambiar: " << endl;
					cin >> letraSeleccionada;

					while (!(actual->getFicha()->encuentrada(letraSeleccionada))) {	//valida que la letra este en el atril del jugador				
						cout << " Letra seleccionada no esta en su poder. " << endl;
						cout << " Digite una letra valida: " << endl; //vuelve a pedir la letra
						cin >> letraSeleccionada;
					}

					if (actual->getFicha()->encuentrada(letraSeleccionada)) {//si la letra se encuentra en el atril
						letras->ingresaFicha(actual->getFich()->eliminarFichaEspecifica(letraSeleccionada)); //la elimina e ingresa una nueva ficha
					}

					cout << " Termino de cambiar letras? (Y/N) " << endl;//valida si el usuario quiere cambiar mas letras
					cin >> seleccion;
				}//fin del while


				this->repartirFichas(actual, actual->cantidadFichas());//actualiza en atril con las nuevas fichas
				jugada = false;//el jugador no hizo ninguna movida en tablero

				system("cls");
				//muetra las nuevas fichas al usuario
				cout << " A continuacion se le muestran sus nuevas fichas: " << endl << endl;
				cout << actual->getFicha()->toStringJugador() << endl << endl;


				system("pause");

				break;
			}//fin de case 2

			case 3: {

				//Indica al jugador la leyenda de las casillas de bono y la central
				system("cls");
				I->indicadorLeyenda(); //imprime las leyendas
				break;
			}//fin de case 3

			case 4: {
				//muestra las palabras creadas por el usuario con su puntaje 
				system("cls");
				cout << "	A continuacion se le muestran las palabras creadas por el usuario " << actual->getNombre() << endl << endl;
				cout << actual->getFicha()->toString()Jugador;
				system("pause");
				break;
			}//fin de case 4

			case 5: {
				//hace que el jugador termine la ronda
				this->getJugadores()->guardarJugadores();
				jugada = true;
				pasar = pasar + 1;

				if (turno < jugadores - 1) {
					turno = turno + 1;
					color = color + 1;
				}
				else {
					turno = 0;
					color = 249;
				}
				break;
			}//fin del case 5

				  system("pause");
				  system("cls");

			}//Fin switch

		}//Fin hayGanador

		else { //Hay Ganador		

			system("cls");

			cout << " Tenemos un ganador!!!! " << endl << endl;
			cout << getJugadores()->toStringJugador() << endl << endl;//muestra en lo mas alto al jugador ganador y su puntaje, seguido de los otros
			system("pause");
			break;
		}

	} while (opcion != 6);

}//fin del metodo


//Metodo toStri=ng
string Tabla::toStringJugador() {
	stringstream s;

	s << jugadores->toString();
	s << endl;
	return s.str();
}

//Metodo que muestra el tablero en consola
void Tabla::mostrar() {

	//cambia los espacios de la matriz a color verde
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);//Cambia a color verde
	//coloca numeros en la parte superior, de modo que sea mas sencillo encontrar las casillas
	cout << "     0     1     2     3     4     5     6     7     8     9    10    11    12    13    14  " << endl;

	//Crea los marcos superiores y laterales de la matriz
	for (int i = 0; i < 15; i++) {
		cout << "   _________________________________________________________________________________________" << endl;
		cout << "  ";
		for (int j = 0; j < 15; j++) {
			cout << "|";

			//posiciones donde se encuentran las casillas de color rojo
			if (i == 0 && (j == 0 || j == 14 || j == 7) || i == 14 && (j == 14 || j == 0 || j == 7) || i == 7 && (j == 0 || j == 14)) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 74); //Cambia las casilla a color rojo
			}

			//posiciones donde se encuentran las casillas de color celeste
			if (i == 0 && (j == 3 || j == 11) || i == 2 && (j == 6 || j == 8) || i == 3 && (j == 0 || j == 14 || j == 7) ||
				i == 6 && (j == 2 || j == 6 || j == 8 || j == 12) || i == 7 && (j == 3 || j == 11) ||
				i == 8 && (j == 2 || j == 6 || j == 8 || j == 12) || i == 11 && (j == 0 || j == 14 || j == 7) ||
				i == 12 && (j == 6 || j == 8) || i == 14 && (j == 3 || j == 11)) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176); //Cambia las casilla a color celeste
			}

			//Posiciones donde se encuentran las casillas de color amarillo
			if (i == 7 && j == 7) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 226); //Cambia las casilla a color amarillo
			}

			//posciones donde se encuentran las casillas de color rosado
			if (i == 1 && (j == 1 || j == 13) || i == 2 && (j == 2 || j == 12) || i == 3 && (j == 3 || j == 11) ||
				i == 4 && (j == 4 || j == 10) || i == 13 && (j == 1 || j == 13) || i == 12 && (j == 2 || j == 12) ||
				i == 11 && (j == 3 || j == 11) || i == 10 && (j == 4 || j == 10)) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 218); //Cambia las casilla a color rosado
			}

			//Posiciones donde se encuentran las casillas de color azul
			if (i == 1 && (j == 5 || j == 9) || i == 5 && (j == 1 || j == 5 || j == 9 || j == 13) ||
				i == 13 && (j == 5 || j == 9) || i == 9 && (j == 1 || j == 5 || j == 9 || j == 13)) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 154); //Cambia las casilla a color azul
			}
			cout << "  ";
			cout << tablero[i][j]->getLetra()->getLetra();//Mostrar el contenido que tiene la casilla
			cout << "  ";

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //Cambia casilla a color verde

			//crea los marcos de la columna 
			if (j == 14) {
				cout << "|  " << i;
			}
		}

		cout << endl;
		//crea el marco inferior de la matriz
		if (i == 14) {
			cout << "   _________________________________________________________________________________________" << endl;
		}
	}
}//fin del metodo