#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define MAX_BALAS 5
#define MAX_TANDA 3
#define M_ALTO 40
#define M_ANCHO 40
#define T_DER 77
#define T_IZQ 75
#define T_ARR 72
#define T_ABA 80
#define T_ESP 32
#define T_ENT 13

char tecla = 0;
int puntuacion = 0;
enum T_enm { mariposa, avispa, comandante };
enum Tanda { tanda1, tanda2, tanda3 };
enum direccion { arriba, abajo, izquierda, derecha };
enum nivel { facil, normal, dificil };
//int n = 0;
bool endGame00 = false;
bool n1 = true, n2 = false, n3 = false;
bool fin = false;

void gotoxy(int x_pos, int y_pos);

typedef struct {
	int pos_xa; int pos_xn; int pos_ya; int pos_yn;
	bool est;
}balas_navd;

balas_navd* b_navG = new balas_navd[MAX_BALAS];

class nav_def {
private:
	int x_ant, x_nue, y_ant, y_nue;
	int vidas;
	bool estado;
	int balas_disp;
	balas_navd b_nav[MAX_BALAS];
public:
	nav_def(int, int, int, int, int, bool, int);
	~nav_def() {}
	int getter_vida();
	void set_vida(int n) { vidas = vidas - n; };
	int get_x() { return x_nue; }
	int get_y() { return y_nue; }
	int get_balas_disp() { return balas_disp; }
	void mover_inicializar_bala();
	void mover_balas();
	void mostrar_balas_vidas();
	void set_bala_state(int i) {
		b_nav[i].est = false;
	}
	void get_b_navdef() {
		for (int i = 0; i < balas_disp; i++) {
			b_navG[i] = b_nav[i];
		}
	}
};
class nav_enm {
private:
	int x_ant, x_nue, y_ant, y_nue;
	T_enm tipo;
	Tanda tand;
	direccion dir;
	int indice;
	int orden;
	int hilera;
	bool estado;
public:
	nav_enm() {}
	~nav_enm() {  }
	void inicializar_enm(int, int, int, int, Tanda, T_enm, direccion, int, int, int, bool);
	void mover_nave();
	void pintar_nave();
	bool get_estado() { return estado; }
	void set_estado(bool _est) { estado = _est; }
	int get_xn() { return x_nue; };
	int get_yn() { return y_nue; };
	T_enm get_tipo() { return tipo; }
};

void val_pred();
void iniciando_juego(nav_def& nave);
void endGame_Galaga(bool&, nav_def& nave, int&);
void temporizador(int& tiemRel, int& tiemSeg, int& tiemMin, bool& endGame, nav_def&);
void presentacion_parte1(char& tecla);
void presentacion_parte2(char& tecla, int& pag);
void pantalla_de_instrucciones(char& tecla, int& pag);
T_enm enm_aleatorio(int);
void cant_enm(nivel, int&, int&, int&);
void inicializar_disparo_enm(int&, int&, int&, int&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, balas_navd*&);
void mover_balas_enm(int&, int&, int&, int&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, balas_navd*&);
void actualizar_disparo_enm(int&, int&, int&, int&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, balas_navd*&);
void perder_vida(balas_navd*&, nav_def&, int);
void eliminar_enm(nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, int, int, int, nav_def&);
bool enm_eliminados(nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, int&, int&, int&);


void gotoxy(int x_pos, int y_pos) {
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x_pos;
    dwPos.Y = y_pos;
    SetConsoleCursorPosition(hcon, dwPos);
}

void ajustar_ventana(int Ancho, int Alto) {

    // Ocultar cursor

    CONSOLE_CURSOR_INFO cci;

    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	cci.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);

    // Ajustar ventana

	_COORD Coordenada;
	Coordenada.X = Ancho;
	Coordenada.Y = Alto;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Right = Ancho - 1;
	Rect.Bottom = Alto - 1;

	// Obtener el handle de la consola
	HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);

	// Ajustar el buffer al nuevo tamaño
	SetConsoleScreenBufferSize(hConsola, Coordenada);

	// Cambiar tamaño de consola a lo especificado en el buffer
	SetConsoleWindowInfo(hConsola, TRUE, &Rect);
}

enum Color { blue = 1, darkGreen = 2, cyan = 3, darkRed = 4, yellow = 5, white = 7, green = 10, red = 12 };

void change_console_color(Color color) {
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hconsole, color);
}

int random(int base, int max) {
    return rand() % max + base;
}

nav_def::nav_def(int _xa, int _xn, int _ya, int _yn, int _vid, bool _est, int _bdis) {
	x_ant = _xa; x_nue = _xn;
	y_ant = _ya; y_nue = _yn;
	vidas = _vid;
	estado = _est;
	balas_disp = _bdis;
}
int nav_def::getter_vida() {
	return vidas;
}
void nav_def::mover_inicializar_bala() {
	if (_kbhit()) {
        gotoxy(x_ant, y_ant);
        cout << "   ";
		tecla = getch();
		if (tecla == T_IZQ && x_nue > 0) x_nue--;
		else if (tecla == T_DER && x_nue < 37) x_nue++;
		else if (tecla == T_ESP && balas_disp < MAX_BALAS) { //here

			b_nav[balas_disp].pos_xa = b_nav[balas_disp].pos_xn = x_nue + 1;
			b_nav[balas_disp].pos_yn = b_nav[balas_disp].pos_ya = y_nue - 1;
			b_nav[balas_disp].est = true;
			balas_disp++;
		}
		else if (fin == true && tecla == T_ENT) {
			endGame00 = true;
			tecla = 0;
			system("cls");
			system("cls"); fflush(stdin);
		}
		change_console_color(Color::white);
		gotoxy(x_nue, y_nue); cout << "|w|";

		x_ant = x_nue;
		y_ant = y_nue;
	}
}
void nav_def::mover_balas() {
	for (int i = 0; i < balas_disp; i++) {
		b_nav[i].pos_yn--;
		if (b_nav[i].pos_yn > 3 && b_nav[i].est == true) {
			gotoxy(b_nav[i].pos_xa, b_nav[i].pos_ya); cout << " ";
			change_console_color(Color::white);
			gotoxy(b_nav[i].pos_xn, b_nav[i].pos_yn); cout << char(4);
			b_nav[i].pos_xa = b_nav[i].pos_xn;
			b_nav[i].pos_ya = b_nav[i].pos_yn;
		}
		else if (b_nav[i].pos_yn < 3) {
			gotoxy(b_nav[i].pos_xa, b_nav[i].pos_ya); cout << " ";
			b_nav[i] = b_nav[balas_disp - 1];
			balas_disp--;
		}
	}
}
void nav_def::mostrar_balas_vidas() {
    change_console_color(Color::red);
	gotoxy((M_ANCHO / 2) - 4, M_ALTO - 1); cout << char(3) << "x" << vidas;
	gotoxy((M_ANCHO / 2) - 5, 2); cout << puntuacion;
	for (int i = 1; i <= MAX_BALAS - balas_disp; i++) {
		gotoxy(((M_ANCHO / 2) + 1) + i, M_ALTO - 1); change_console_color(Color::darkGreen); cout << char(4);
	}
	for (int u = 0; u < balas_disp; u++) {
		if (balas_disp <= MAX_BALAS) {
            change_console_color(Color::darkRed);
			gotoxy(((M_ANCHO / 2) + 2) + MAX_BALAS - balas_disp, M_ALTO - 1); cout << char(4);
		}
	}
}

void nav_enm::inicializar_enm(int xa, int xn, int ya, int yn, Tanda ta, T_enm ti, direccion _dir, int indic, int ord, int hil, bool est) {
	x_ant = xa;
	x_nue = xa;
	y_ant = ya;
	y_nue = yn;
	tand = ta;
	tipo = ti;
	dir = _dir;
	indice = indic;
	orden = ord;
	hilera = hil;
	estado = est;
}
void nav_enm::mover_nave() {
	indice++;
	gotoxy(x_ant, y_ant); cout << "   ";
	if (tand == tanda1) {
		if (hilera == 1) {
			if (indice >= 0 && indice <= 2) {
				y_nue += 2;
			}
			else if (indice > 2 && indice < 4) {
				y_nue += 2;
			}
			else if (indice >= 4 && indice < 10) {
				y_nue += 2;
			}
			else if (indice >= 10 && indice < 12) {
				y_nue += 2;
				x_nue--;
			}
			else if (indice >= 12 && indice < 16) {
				x_nue--;
			}
			else if (indice >= 16 && indice < 18) {
				y_nue -= 2;
				x_nue--;
			}
			else if (indice >= 18 && indice < 21) {
				y_nue -= 2;
			}
			else if (indice >= 21 && indice < 23) {
				y_nue -= 2;
			}
			else if (indice >= 23 && indice < 30) {
				x_nue = (x_nue + 3) - orden;
			}
		}
		else if (hilera == 2) {
			if (indice >= 0 && indice <= 2) {
				y_nue += 2;
			}
			else if (indice > 2 && indice < 4) {
				y_nue += 2;
			}

			else if (indice >= 4 && indice < 10) {
				y_nue += 2;
			}
			else if (indice >= 10 && indice < 12) {
				y_nue += 2;
				x_nue++;
			}
			else if (indice >= 12 && indice < 16) {
				x_nue++;
			}
			else if (indice >= 16 && indice < 18) {
				y_nue -= 2;
				x_nue++;
			}
			else if (indice >= 18 && indice < 21) {
				y_nue -= 3;
			}
			else if (indice >= 21 && indice < 23) {
				y_nue -= 2;
				x_nue += 2;
			}
			else if (indice >= 23 && indice < 30) {
				x_nue = (x_nue - 3) + orden;
			}
		}
		if (indice >= 40) {

			if (x_nue >= 0 && dir == izquierda) {
				if (x_nue == 0) {
					dir = abajo;
					y_nue += 2;
					x_nue += 1;
				}
				else x_nue--;
			}
			else if (x_nue <= 37 && dir == abajo) {
				if (x_nue == 37) {
					dir = izquierda;
					y_nue -= 2;
					x_nue -= 1;
				}
				else {
					x_nue++;
				}
			}
		}
	}
	else if (tand == tanda2) {
		if (hilera == 1) {
			if (indice >= 40 && indice < 42) {
				y_nue++;
			}
			else if (indice > 42 && indice < 52) {
				y_nue++;
				x_nue++;
			}
			else if (indice >= 52 && indice < 61) {
				y_nue++;
			}
			else if (indice >= 61 && indice < 64) {
				y_nue++;
				x_nue--;
			}
			else if (indice >= 64 && indice < 70) {
				x_nue--;
			}
			else if (indice > 70 && indice < 73) {
				y_nue--;
				x_nue--;
			}
			else if (indice > 73 && indice < 84) {
				y_nue--;
			}
			else if (indice >= 85 && indice < 90) {
				y_nue--;
				x_nue += 2;
			}
			else if (indice >= 90 && indice <= 92) {
				x_nue = x_nue - orden;
				y_nue = y_nue - orden;
			}
		}
		else if (hilera == 2) {
			if (indice >= 40 && indice < 42) {
				y_nue++;
			}
			else if (indice > 42 && indice < 52) {
				y_nue++;
				x_nue--;
			}
			else if (indice >= 52 && indice < 61) {//3
				y_nue++;
			}
			else if (indice >= 61 && indice < 64) {
				y_nue++;
				x_nue++;
			}
			else if (indice >= 64 && indice < 70) {
				x_nue++;
			}
			else if (indice > 70 && indice < 73) {
				y_nue--;
				x_nue++;
			}
			else if (indice > 73 && indice < 85) {
				y_nue--;
			}
			else if (indice >= 85 && indice < 90) {
				x_nue--;
				y_nue -= 2;
			}
			else if (indice >= 90 && indice <= 92) {
				x_nue = x_nue - orden;
				y_nue = y_nue + orden;
			}
		}
	}
	else if (tand == tanda3) {
		if (hilera == 1) {
			if (indice >= 98 && indice < 103) {
				x_nue++;
			}
			else if (indice >= 103 && indice < 108) {
				x_nue++;
				y_nue++;
			}
			else if (indice >= 108 && indice < 111) {
				y_nue--;
			}
			else if (indice >= 111 && indice < 116) {
				y_nue--;
				x_nue--;
			}
			else if (indice >= 116 && indice < 119) {
				x_nue++;
			}
			else if (indice >= 119 && indice < 123) {
				y_nue++;
				x_nue--;
			}
			else if (indice >= 123 && indice < 126) {
				y_nue++;
			}
			else if (indice >= 126 && indice < 128) {
				y_nue++;
				x_nue++;
			}
			else if (indice >= 128 && indice < 136) {
				x_nue++;
			}
			else if (indice >= 136 && indice < 140) {
				y_nue--;
				x_nue++;
			}
			else if (indice >= 140 && indice < 143) {
				y_nue--;
			}
			else if (indice >= 143 && indice < 150) {
				y_nue--;
				x_nue--;
			}
			else if (indice >= 150 && indice < 154) {
				y_nue = y_nue - orden;
				x_nue = x_nue - orden;
			}
			else if (indice >= 198 && indice <= 203) {
				x_nue++;
			}
			else if (indice >= 202 && indice <= 208) {
				x_nue--;
			}
			else if (indice >= 209) {
				indice = 198;
			}
		}
		else if (hilera == 2) {
			if (indice >= 175 && indice < 180) {
				x_nue--;
			}
			else if (indice >= 180 && indice < 189) {
				y_nue--;
			}
			else if (indice >= 189 && indice < 190) {
				x_nue--;
				y_nue++;
			}
			else if (indice >= 190 && indice < 194) {
				x_nue--;
			}
			else if (indice >= 194 && indice <= 197) {
				y_nue = y_nue - orden;
				x_nue = x_nue + orden;
			}
			else if (indice >= 198 && indice <= 203) {
				x_nue--;
			}
			else if (indice >= 204 && indice <= 208) {
				x_nue++;
			}
			else if (indice >= 209) {
				indice = 198;
			}

		}
	}
	pintar_nave();
	x_ant = x_nue;
	y_ant = y_nue;
}
void nav_enm::pintar_nave() {
	if (tipo == mariposa && estado == true) {
        change_console_color(Color::blue);
		gotoxy(x_nue, y_nue); cout << ">";
		gotoxy(x_nue + 2, y_nue); cout << "<";
        change_console_color(Color::yellow);
		gotoxy(x_nue + 1, y_nue); cout << "|";
	}
	else if (tipo == avispa && estado == true) {
        change_console_color(Color::green);
		gotoxy(x_nue, y_nue); cout << char(4);
		gotoxy(x_nue + 2, y_nue); cout << char(4);
        change_console_color(Color::white);
		gotoxy(x_nue + 1, y_nue); cout << "|";
	}
	else if (tipo == comandante && estado == true) {
        change_console_color(Color::red);
		gotoxy(x_nue, y_nue); cout << "}";
		gotoxy(x_nue + 2, y_nue); cout << "{";
        change_console_color(Color::white);
		gotoxy(x_nue + 1, y_nue); cout << "-";
	}
}


int main() {
	bool endGame = false;
	int tiemSeg = 0;
	int tiemMin = 0;
	int tiemRel = 0;
	int pag = 0;
	bool nivel2 = false, nivel3 = false;
	int n = 0;

	val_pred();
	presentacion_parte1(tecla);
	do {
		presentacion_parte2(tecla, pag);
		if (pag == 1) {
			nav_def nave(M_ANCHO / 2, M_ANCHO / 2, M_ALTO - 2, M_ALTO - 2, 4, true, 0);

			nivel nv = normal;
			int c_T1 = 0, c_T2 = 0, c_T3 = 0, C_balas = 3;
			balas_navd* bala_enm = new balas_navd[C_balas];

			cant_enm(nv, c_T1, c_T2, c_T3);

			nav_enm* T1_H1 = new nav_enm[c_T1];
			nav_enm* T1_H2 = new nav_enm[c_T1];
			nav_enm* T2_H1 = new nav_enm[c_T2];
			nav_enm* T2_H2 = new nav_enm[c_T2];
			nav_enm* T3_H1 = new nav_enm[c_T3];
			nav_enm* T3_H2 = new nav_enm[c_T3];

			for (int i = 0; i < c_T1; i++) {
				T1_H1[i].inicializar_enm(18, 18, 6, 6, tanda1, enm_aleatorio(12), abajo, -1 - 5 * i, i, 1, true);
				T1_H2[i].inicializar_enm(22, 22, 6, 6, tanda1, enm_aleatorio(6), izquierda, -1 - 5 * i, i, 2, true);

			}
			for (int u = 0; u < c_T2; u++) {
				T2_H1[u].inicializar_enm(25, 25, 4, 4, tanda2, enm_aleatorio(36), abajo, -1 - 10 * u, u, 1, true);
				T2_H2[u].inicializar_enm(15, 15, 4, 4, tanda2, enm_aleatorio(18), abajo, -1 - 10 * u, u, 2, true);
			}
			for (int j = 0; j < c_T3; j++) {
				T3_H1[j].inicializar_enm(0, 0, 30, 30, tanda3, enm_aleatorio(12), abajo, -1 - 6 * j, j, 1, true);
				T3_H2[j].inicializar_enm(37, 39, 30, 30, tanda3, enm_aleatorio(24), abajo, -1 - 6 * j, j, 2, true);
			}

			iniciando_juego(nave);
			inicializar_disparo_enm(C_balas, c_T1, c_T2, c_T3, T1_H1, T1_H2, T2_H1, T2_H2, T3_H1, T3_H2, bala_enm);
			while (!endGame00) {

				tiemRel++;
				nave.mover_inicializar_bala();
				
				
				nave.mostrar_balas_vidas();

				temporizador(tiemRel, tiemSeg, tiemMin, endGame, nave);

				for (int i = 0; i < c_T1; i++) { //TANDA 1
					T1_H1[i].mover_nave();
					T1_H2[i].mover_nave();
				}
				for (int u = 0; u < c_T2; u++) { //TANDA 2
					T2_H1[u].mover_nave();
					T2_H2[u].mover_nave();
				}
				for (int j = 0; j < c_T3; j++) { //TANDA 3
					T3_H1[j].mover_nave();
					T3_H2[j].mover_nave();
				}

				if (enm_eliminados(T1_H1, T1_H2, T2_H1, T2_H2, T3_H1, T3_H2, c_T1, c_T2, c_T3) == true) {
					if (nivel2 == false) {
						nv = normal;
						gotoxy(M_ANCHO / 2 - 2, M_ALTO / 2); cout << "NIVEL 2";
						Sleep(1000);
						for (int i = 0; i < c_T1; i++) {
							T1_H1[i].inicializar_enm(18, 18, 6, 6, tanda1, enm_aleatorio(12), abajo, -1 - 5 * i, i, 1, true);
							T1_H2[i].inicializar_enm(22, 22, 6, 6, tanda1, enm_aleatorio(6), izquierda, -1 - 5 * i, i, 2, true);

						}
						for (int u = 0; u < c_T2; u++) {
							T2_H1[u].inicializar_enm(25, 25, 4, 4, tanda2, enm_aleatorio(36), abajo, -1 - 10 * u, u, 1, true);
							T2_H2[u].inicializar_enm(15, 15, 4, 4, tanda2, enm_aleatorio(18), abajo, -1 - 10 * u, u, 2, true);
						}
						for (int j = 0; j < c_T3; j++) {
							T3_H1[j].inicializar_enm(0, 0, 30, 30, tanda3, enm_aleatorio(12), abajo, -1 - 6 * j, j, 1, true);
							T3_H2[j].inicializar_enm(37, 39, 30, 30, tanda3, enm_aleatorio(24), abajo, -1 - 6 * j, j, 2, true);
						}
						nivel2 = true;
						gotoxy(M_ANCHO / 2 - 2, M_ALTO / 2); cout << "       ";
					}
					else if (nivel3 == false) {
						nv = dificil;
						gotoxy(M_ANCHO / 2 - 2, M_ALTO / 2); cout << "NIVEL 3";
						Sleep(1000);
						for (int i = 0; i < c_T1; i++) {
							T1_H1[i].inicializar_enm(18, 18, 6, 6, tanda1, enm_aleatorio(12), abajo, -1 - 5 * i, i, 1, true);
							T1_H2[i].inicializar_enm(22, 22, 6, 6, tanda1, enm_aleatorio(6), izquierda, -1 - 5 * i, i, 2, true);

						}
						for (int u = 0; u < c_T2; u++) {
							T2_H1[u].inicializar_enm(25, 25, 4, 4, tanda2, enm_aleatorio(36), abajo, -1 - 10 * u, u, 1, true);
							T2_H2[u].inicializar_enm(15, 15, 4, 4, tanda2, enm_aleatorio(18), abajo, -1 - 10 * u, u, 2, true);
						}
						for (int j = 0; j < c_T3; j++) {
							T3_H1[j].inicializar_enm(0, 0, 30, 30, tanda3, enm_aleatorio(12), abajo, -1 - 6 * j, j, 1, true);
							T3_H2[j].inicializar_enm(37, 39, 30, 30, tanda3, enm_aleatorio(24), abajo, -1 - 6 * j, j, 2, true);
						}
						nivel3 = true;
						gotoxy(M_ANCHO / 2 - 2, M_ALTO / 2); cout << "       ";
					}
					else if (nivel2 == true && nivel3 == true) {
                        change_console_color(Color::yellow);
						gotoxy(M_ANCHO / 2 - 6, M_ALTO / 2); cout << "Congratulation!!";
						n++;
						if (n % 10 != 0)
                            change_console_color(Color::cyan);
						else 
                            change_console_color(Color::red);
						gotoxy(M_ANCHO / 2 - 10, M_ALTO / 2 + 2); cout << "Presione ENTER para salir";
						if (n > 1000)
							n = 0;
						fin = true;
						pag = 0;
					}

				}

				nave.get_b_navdef();

				eliminar_enm(T1_H1, T1_H2, T2_H1, T2_H2, T3_H1, T3_H2, c_T1, c_T2, c_T3, nave);

				Sleep(20);

				nave.mover_balas();
				mover_balas_enm(C_balas, c_T1, c_T2, c_T3, T1_H1, T1_H2, T2_H1, T2_H2, T3_H1, T3_H2, bala_enm);

				perder_vida(bala_enm, nave, C_balas);

				endGame_Galaga(endGame, nave, tiemMin);

			}
			tiemMin = 0;
			tiemSeg = 0;
			tiemRel = 0;
			puntuacion = 0;
			nivel2 = false;
			nivel3 = false;
		}
		else if (pag == 2) {
			pantalla_de_instrucciones(tecla, pag);
		}
		system("cls");
	} while (endGame == false);


	return 0;
}

void val_pred() {
    ajustar_ventana(M_ANCHO, M_ALTO);
}
void iniciando_juego(nav_def& nave) {
	//PINTANDO LAS VIDAS
    change_console_color(Color::red);
	gotoxy((M_ANCHO / 2) - 4, M_ALTO - 1); cout << char(3) << "x" << nave.getter_vida();
	//PINTANDO POSICION INICIAL NAVE
	change_console_color(Color::white);
	gotoxy(M_ANCHO / 2, M_ALTO - 2); cout << "|w|";
	//CAJA DE TIEMPO E IMFORMACION
    change_console_color(Color::white);
	gotoxy(11, 1); cout << "PUNTUACION";
	gotoxy(22, 1); cout << "TIEMPO";
    change_console_color(Color::yellow);
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 19; j++) {
			gotoxy(10 + j, i);
			printf("%c", 196);
			gotoxy(10 + j, 3);
			printf("%c", 196);
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 1; j++) {
			gotoxy(10, i);
			printf("%c", 179);
			gotoxy(21, i);
			printf("%c", 179);
			gotoxy(28, i);
			printf("%c", 179);
		}
	}
    change_console_color(Color::white);
	gotoxy(10, 0); printf("%c", 218);
	gotoxy(21, 0); printf("%c", 194);
	gotoxy(28, 0); printf("%c", 191);
	gotoxy(10, 3); printf("%c", 192);
	gotoxy(21, 3); printf("%c", 193);
	gotoxy(28, 3); printf("%c", 217);
	//TIEMPO DE INCIO
	gotoxy(17, 20); cout << "LISTO?";
	Sleep(2000);
	gotoxy(17, 20); cout << "       ";
	gotoxy(17, 20); cout << "VAMOS!";
	Sleep(1000);
	gotoxy(17, 20); cout << "       ";
}
void temporizador(int& tiemRel, int& tiemSeg, int& tiemMin, bool& endGame, nav_def& nave) {
	if (tiemRel == 10) {
		tiemSeg++;
		tiemRel = 0;
	}

	if (tiemSeg == 60) {
		gotoxy(26, 2); cout << "  ";
		tiemMin++;
		tiemSeg = 0;
	}
    change_console_color(Color::white);
	gotoxy(23, 2); cout << "0";
	gotoxy(24, 2); cout << tiemMin;
	gotoxy(25, 2); cout << ":";

	if (tiemSeg >= 10) {
		gotoxy(26, 2); cout << tiemSeg;
	}
	else {
		gotoxy(26, 2); cout << "0";
		gotoxy(27, 2); cout << tiemSeg;
	}

}
void endGame_Galaga(bool& endGame, nav_def& nave, int& _tiem) {
	if (nave.getter_vida() == 0 || _tiem == 5) {
		gotoxy((M_ANCHO / 2) - 4, M_ALTO - 1); cout << char(3) << "x0";
		bool ani1 = false;
		int r = 0, formEND[7][38] =
		{
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,1,1,1,0,0,1,1,0,0,1,0,0,0,1,0,1,1,1,0,0,0},
			{0,0,1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,1,1,0,1,1,0,1,0,0,0,0,0},
			{0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0},
			{0,0,1,0,0,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0},
			{0,0,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,1,1,1,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		};
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 38; j++) {
				if (formEND[i][j] == 0) {
					gotoxy(j + 2, i + 16); cout << " ";
				}
				else if (formEND[i][j] == 1) {
                    change_console_color(Color::red);
					gotoxy(j + 2, i + 16); cout << "*";

				}
				Sleep(5);
			}
		}
		while (!ani1) {
			r++;
			if (r % 10 == 0) {
                change_console_color(Color::yellow);
				gotoxy(14, 23); cout << "PRESIONE ENTER";
			}
			else {
                change_console_color(Color::red);
				gotoxy(14, 23); cout << "PRESIONE ENTER";
			}
			if (_kbhit()) {
				tecla = getch();
				if (tecla == T_ENT) {
					ani1 = true;
					n1 = true;
					n2 = false;
					n3 = false;
					
					tecla = 0;
					endGame00 = true;
					system("cls");
				}
			}
		}

	}
}
void pantalla_de_instrucciones(char& tecla, int& pag) {

	int pos = 3;
    system("cls");
    change_console_color(Color::red);
	gotoxy(7, 2); printf("%c%c%c  %c%c  %c   %c%c  %c%c%c  %c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(7, 3); printf("%c   %c  %c %c  %c  %c %c   %c  %c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(7, 4); printf("%c %c %c%c%c%c %c  %c%c%c%c %c %c %c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(7, 5); printf("%c%c%c %c  %c %c%c %c  %c %c%c%c %c  %c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    change_console_color(Color::blue);
	gotoxy(3, 7); cout << "Es un juego matamarcianos clasico, \n   donde el objetivo es eliminar las \n   distintas naves enemigas.";
    change_console_color(Color::white);
	gotoxy(12, 12); printf("* ** *  * ** *");
	gotoxy(12, 13); printf("* <- *  * -> *");
	gotoxy(12, 14); printf("* ** *  * ** *");
    change_console_color(Color::yellow);
	gotoxy(5, 16); printf("Para desplazarnos y esquivar");
	gotoxy(5, 17); printf(" las balas enemigas, debemos ");
	gotoxy(5, 18); printf("usar las teclas direccionales");
    change_console_color(Color::white);
	gotoxy(3, 21); printf("* * * * * * * * * * * * * * * * * *");
	gotoxy(3, 22); printf("*             esp                 *");
	gotoxy(3, 23); printf("* * * * * * * * * * * * * * * * * *");
    change_console_color(Color::yellow);
	gotoxy(0, 25); printf("    Para disparas usaremos la tecla");
	gotoxy(0, 26); printf("  espacio, recuerde que son 5 disparos");
	gotoxy(0, 27); printf("      que podemos hacer a la vez");
    change_console_color(Color::blue);
	gotoxy(3, 32); printf("    *");
	gotoxy(3, 33); printf("  * * * *");
	gotoxy(3, 34); printf("* * * * *");
	gotoxy(3, 35); printf("  * * * *");
	gotoxy(3, 36); printf("    *");
	gotoxy(5, 38); cout << "volver";
    change_console_color(Color::white);
	gotoxy(27, 32); printf("    *");
	gotoxy(27, 33); printf("* * * *  ");
	gotoxy(27, 34); printf("* * * * *");
	gotoxy(27, 35); printf("* * * *  ");
	gotoxy(27, 36); printf("    *");
	while (1)
	{

		if (_kbhit()) {
			tecla = _getch();
			if (tecla == T_IZQ) {
				pos = 3;
				//Console::ForegroundColor = ConsoleColor::Blue;
                change_console_color(Color::blue);
				gotoxy(5, 38); cout << "volver";
				gotoxy(29, 38); cout << "     ";
				gotoxy(3, 32); printf("    *");
				gotoxy(3, 33); printf("  * * * *");
				gotoxy(3, 34); printf("* * * * *");
				gotoxy(3, 35); printf("  * * * *");
				gotoxy(3, 36); printf("    *");
				//Console::ForegroundColor = ConsoleColor::White;
                change_console_color(Color::white);
				gotoxy(27, 32); printf("    *");
				gotoxy(27, 33); printf("* * * *  ");
				gotoxy(27, 34); printf("* * * * *");
				gotoxy(27, 35); printf("* * * *  ");
				gotoxy(27, 36); printf("    *");
			}
			else if (tecla == T_DER) {
				pos = 27;
				//Console::ForegroundColor = ConsoleColor::White;
                change_console_color(Color::white);
				gotoxy(3, 32); printf("    *");
				gotoxy(3, 33); printf("  * * * *");
				gotoxy(3, 34); printf("* * * * *");
				gotoxy(3, 35); printf("  * * * *");
				gotoxy(3, 36); printf("    *");
				//Console::ForegroundColor = ConsoleColor::Blue;
                change_console_color(Color::blue);
				gotoxy(27, 32); printf("    *");
				gotoxy(27, 33); printf("* * * *  ");
				gotoxy(27, 34); printf("* * * * *");
				gotoxy(27, 35); printf("* * * *  ");
				gotoxy(27, 36); printf("    *");
				gotoxy(5, 38); cout << "      ";
				gotoxy(29, 38); cout << "jugar";
			}
			else if (pos == 27 && tecla == 13) {
				pag = 1;
				endGame00 = false;
				system("cls");
				fflush(stdin);
				break;
			}
			else if (pos == 3 && tecla == 13) {
				tecla = 0;
				endGame00 = false;
				system("cls");
				fflush(stdin);
				break;
			}
		}
	}

}
void presentacion_parte2(char& tecla, int& pag) {
	int aum = 0, posy = 19;
	//Console::ForegroundColor = ConsoleColor::DarkGreen;
    change_console_color(Color::darkGreen);
	gotoxy(9, 19); printf("-->");
	while (1) {
		if (aum % 2 == 0) //Console::ForegroundColor = ConsoleColor::DarkYellow;
            change_console_color(Color::yellow);
		else //Console::ForegroundColor = ConsoleColor::DarkGreen;
            change_console_color(Color::darkGreen);
		gotoxy(7, 2); printf("%c%c%c  %c%c  %c   %c%c  %c%c%c  %c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
		gotoxy(7, 3); printf("%c   %c  %c %c  %c  %c %c   %c  %c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
		gotoxy(7, 4); printf("%c %c %c%c%c%c %c  %c%c%c%c %c %c %c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
		gotoxy(7, 5); printf("%c%c%c %c  %c %c%c %c  %c %c%c%c %c  %c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);


		//Console::ForegroundColor = ConsoleColor::DarkRed;
        change_console_color(Color::darkRed);
		gotoxy(2 + aum, 9); printf("%c%c%c", 178, 178, 178);
		//Console::ForegroundColor = ConsoleColor::White;
        change_console_color(Color::white);
		gotoxy(2 + aum, 10); printf(" %c  ", 219);
		gotoxy(2 + aum, 11); printf("%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219);
		gotoxy(2 + aum, 12); printf(" %c  ", 219);
		//Console::ForegroundColor = ConsoleColor::DarkRed;
        change_console_color(Color::darkRed);
		gotoxy(2 + aum, 13); printf("%c%c%c", 178, 178, 178);
		Sleep(100);
		gotoxy(2 + aum, 9); printf("   ");
		gotoxy(2 + aum, 10); printf("    ");
		gotoxy(2 + aum, 11); printf("      ");
		gotoxy(2 + aum, 12); printf("    ");
		//Console::ForegroundColor = ConsoleColor::DarkRed;
        change_console_color(Color::darkRed);
		gotoxy(2 + aum, 13); printf("    ");

		//Console::ForegroundColor = ConsoleColor::White;
        change_console_color(Color::white);
		gotoxy(13, 19); printf("Iniciar juego");
		gotoxy(13, 21); printf("Instrucciones");

		//Console::ForegroundColor = ConsoleColor::DarkRed;
        change_console_color(Color::darkRed);
		gotoxy(6, 28); cout << " 1er LUGAR POR 4000 PUNTOS";
		gotoxy(6, 30); cout << " 2do LUGAR POR 2500 PUNTOS";
		gotoxy(6, 32); cout << " 3er LUGAR POR 1500 PUNTOS";
		//Console::ForegroundColor = ConsoleColor::Blue;
        change_console_color(Color::blue);
		gotoxy(1, 38); cout << "GALAGA 2019 - PROYECTO UPC - 1er CICLO";
		aum++;
		if (_kbhit()) {
			tecla = getch();
			if (tecla == T_ARR) {
				gotoxy(9, 21); printf("   ");
				gotoxy(9, 19); printf("   ");
                change_console_color(Color::darkGreen);
				gotoxy(9, 19); printf("-->");
				posy = 19;
			}
			else if (tecla == T_ABA) {
				gotoxy(9, 19); printf("   ");
				gotoxy(9, 21); printf("   ");
				change_console_color(Color::darkGreen);
				gotoxy(9, 21); printf("-->");
				posy = 21;
			}
		}
		if (posy == 19 && tecla == 13) {
			system("cls");
			endGame00 = false;
			fflush(stdin);
			pag = 1;
			break;
		}
		else if (posy == 21 && tecla == 13) {
			system("cls");
			fflush(stdin);
			pag = 2;
			break;
		}
		if (aum == 33) aum = 0;
	}
}
void presentacion_parte1(char& tecla) {
	int matriz[40][40] =
	{ {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	//system("color 7C");
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			if (matriz[i][j] == 1) {
                change_console_color(Color::red);
				gotoxy(j, i);
				printf("*");

			}
		}
	}
	gotoxy(11, 35); cout << " exigete, innova";
	while (1) {
		if (_kbhit()) {
			tecla = getch();
			if (tecla == 13) {
				tecla = 0;
				break;
			}

		}
	}

	fflush(stdin);
	system("color 0F");
	system("cls");
}
T_enm enm_aleatorio(int p) {
	int n = 0;
    n = random(0, p);
	Sleep(p);

	if (n >= 0 && n <= p / 3) {
		n = 0;
		return mariposa;
	}
	else if (n >= (p / 3) + 1 && n <= ((2 * p) / 3)) {
		n = 0;
		return avispa;
	}
	else if (n <= ((2 * p) / 3) + 1 && n <= p) {
		n = 0;
		return comandante;
	}
	return mariposa;
}
void cant_enm(nivel _nv, int& _cT1, int& _cT2, int& _cT3) {
	if (_nv == facil) {
		_cT1 = 2;
		_cT2 = 1;
		_cT3 = 1;
	}
	else if (_nv == normal) {
		_cT1 = 4;
		_cT2 = 2;
		_cT3 = 2;
	}
	else if (_nv == dificil) {
		_cT1 = 4;
		_cT2 = 3;
		_cT3 = 2;
	}
}
void inicializar_disparo_enm(int& cant_balas, int& _ct1, int& _ct2, int& _ct3, nav_enm*& t1_h1, nav_enm*& t1_h2, nav_enm*& t2_h1, nav_enm*& t2_h2, nav_enm*& t3_h1, nav_enm*& t3_h2, balas_navd*& balas_enm) {
	int n, m, x = 0, y = 0;
	for (int i = 0; i < cant_balas; i++) {
		n = random(1, 6);

		if (n == 1) {
			m = random(0, _ct1);
			Sleep(2);
			if (t1_h1[m].get_estado() == true) {
				x = t1_h1[m].get_xn() + 1;
				y = t1_h1[m].get_yn() + 1;
			}
		}
		else if (n == 2) {
			m = random(0, _ct1);
			Sleep(2);
			if (t1_h2[m].get_estado() == true) {
				x = t1_h2[m].get_xn() + 1;
				y = t1_h2[m].get_yn() + 1;
			}
		}
		else if (n == 3) {
			m = random(0, _ct2);
			Sleep(2);
			if (t2_h1[m].get_estado() == true) {
				x = t2_h1[m].get_xn() + 1;
				y = t2_h1[m].get_yn() + 1;
			}
		}
		else if (n == 4) {
			m = random(0, _ct2);
			Sleep(2);
			if (t2_h2[m].get_estado() == true) {
				x = t2_h2[m].get_xn() + 1;
				y = t2_h2[m].get_yn() + 1;
			}
		}
		else if (n == 5) {
			m = random(0, _ct3);
			Sleep(2);
			if (t3_h1[m].get_estado() == true) {
				x = t3_h1[m].get_xn() + 1;
				y = t3_h1[m].get_yn() + 1;
			}
		}
		else if (n == 6) {
			m = random(0, _ct1);
			Sleep(2);
			if (t3_h2[m].get_estado() == true) {
				x = t3_h2[m].get_xn() + 1;
				y = t3_h2[m].get_yn() + 1;
			}
		}
		if (x != 0 && y != 0 && balas_enm[i].est == false) {
			balas_enm[i].pos_xa = balas_enm[i].pos_xn = x;
			balas_enm[i].pos_ya = balas_enm[i].pos_yn = y;
			balas_enm[i].est = true;
		}
		else {
			balas_enm[i].est = false;
		}
	}
}
void mover_balas_enm(int& cant_disp, int& _ct1, int& _ct2, int& _ct3, nav_enm*& t1_h1, nav_enm*& t1_h2, nav_enm*& t2_h1, nav_enm*& t2_h2, nav_enm*& t3_h1, nav_enm*& t3_h2, balas_navd*& balas_enm) {
	
	for (int i = 0; i < cant_disp; i++) {
		balas_enm[i].pos_yn++;
		if (balas_enm[i].pos_yn >= 0 && balas_enm[i].pos_yn < 39 && balas_enm[i].est == true) {
			gotoxy(balas_enm[i].pos_xa, balas_enm[i].pos_ya); cout << " ";
			gotoxy(balas_enm[i].pos_xn, balas_enm[i].pos_yn); cout << "v";
			balas_enm[i].pos_ya = balas_enm[i].pos_yn;
		}
		else if (balas_enm[i].pos_yn == 39 && balas_enm[i].est == true) {
			gotoxy(balas_enm[i].pos_xa, balas_enm[i].pos_ya); cout << " ";
		}
		else if (balas_enm[i].pos_yn > 39) {
			balas_enm[i] = balas_enm[cant_disp - 1];
			actualizar_disparo_enm(cant_disp, _ct1, _ct2, _ct3, t1_h1, t1_h2, t2_h1, t2_h2, t3_h1, t3_h2, balas_enm);
		}
	}
}
void actualizar_disparo_enm(int& cant_balas, int& _ct1, int& _ct2, int& _ct3, nav_enm*& t1_h1, nav_enm*& t1_h2, nav_enm*& t2_h1, nav_enm*& t2_h2, nav_enm*& t3_h1, nav_enm*& t3_h2, balas_navd*& balas_enm) {
	int n, m, x = 0, y = 0;
    n = random(1, 6);
	switch (n)
	{
		case 1:
			m = random(0, _ct1);
			Sleep(2);
			if (t1_h1[m].get_estado() == true) {
				x = t1_h1[m].get_xn() + 1;
				y = t1_h1[m].get_yn() + 1;
			}
			break;
		case 2:
			m = random(0, _ct1);
			Sleep(2);
			if (t1_h2[m].get_estado() == true) {
				x = t1_h2[m].get_xn() + 1;
				y = t1_h2[m].get_yn() + 1;
			}
			break;
		case 3:
			m = random(0, _ct2);
			Sleep(2);
			if (t2_h1[m].get_estado() == true) {
				x = t2_h1[m].get_xn() + 1;
				y = t2_h1[m].get_yn() + 1;
			}
			break;
		case 4:
			m = random(0, _ct2);
			Sleep(2);
			if (t2_h2[m].get_estado() == true) {
				x = t2_h2[m].get_xn() + 1;
				y = t2_h2[m].get_yn() + 1;
			}
			break;
		case 5:
			m = random(0, _ct3);
			Sleep(2);
			if (t3_h1[m].get_estado() == true) {
				x = t3_h1[m].get_xn() + 1;
				y = t3_h1[m].get_yn() + 1;
			}
			break;
		case 6:
			m = random(0, _ct1);
			Sleep(2);
			if (t3_h2[m].get_estado() == true) {
				x = t3_h2[m].get_xn() + 1;
				y = t3_h2[m].get_yn() + 1;
			}
			break;
	}
	balas_enm[cant_balas - 1].pos_xa = balas_enm[cant_balas - 1].pos_xn = x;
	balas_enm[cant_balas - 1].pos_ya = balas_enm[cant_balas - 1].pos_yn = y;

	if (x != 0 && y != 0) {
		balas_enm[cant_balas - 1].est = true;
	}
	else
		balas_enm[cant_balas - 1].est = false; 

}
void perder_vida(balas_navd*& b_enm, nav_def& nave, int c_balas) {
	for (int i = 0; i < c_balas; i++) {
			if ((b_enm[i].pos_xn == nave.get_x() || b_enm[i].pos_xn == nave.get_x() + 1 || b_enm[i].pos_xn == nave.get_x() + 2) && b_enm[i].pos_yn == nave.get_y()) {
				if (nave.getter_vida() >= 1) {
					nave.set_vida(1);
					gotoxy(nave.get_x(), nave.get_y()); cout << "   ";
					gotoxy(nave.get_x() - 1, nave.get_y()); cout << "* * *";
					Sleep(100);
					gotoxy(nave.get_x() - 1, nave.get_y()); cout << "     ";
					Sleep(1000);
				}

			}
			gotoxy(nave.get_x(), nave.get_y()); cout << "|w|";
		}
}
void eliminar_enm(nav_enm*& t1_h1, nav_enm*& t1_h2, nav_enm*& t2_h1, nav_enm*& t2_h2, nav_enm*& t3_h1, nav_enm*& t3_h2, int ct1, int ct2, int ct3, nav_def& nave) {
	for (int i = 0; i <= nave.get_balas_disp(); i++) {
		for (int j = 0; j < ct1; j++) {
			if ((t1_h1[j].get_xn() == b_navG[i].pos_xn || t1_h1[j].get_xn() + 1 == b_navG[i].pos_xn || t1_h1[j].get_xn() + 2 == b_navG[i].pos_xn) && t1_h1[j].get_yn() == b_navG[i].pos_yn && t1_h1[j].get_estado() == true) {
				t1_h1[j].set_estado(false);
				nave.set_bala_state(i);
				if (t1_h1[i].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t1_h1[i].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t1_h1[i].get_tipo() == comandante) {
					puntuacion += 200;
				}
			}
			else if ((t1_h2[j].get_xn() == b_navG[i].pos_xn || t1_h2[j].get_xn() + 1 == b_navG[i].pos_xn || t1_h2[j].get_xn() + 2 == b_navG[i].pos_xn) && t1_h2[j].get_yn() == b_navG[i].pos_yn && t1_h2[j].get_estado() == true) {
				t1_h2[j].set_estado(false);
				nave.set_bala_state(i);
				if (t1_h2[i].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t1_h2[i].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t1_h2[i].get_tipo() == comandante) {
					puntuacion += 200;
				}
			}
		}
		for (int u = 0; u < ct2; u++) {
			if ((t2_h1[u].get_xn() == b_navG[i].pos_xn || t2_h1[u].get_xn() + 1 == b_navG[i].pos_xn || t2_h1[u].get_xn() + 2 == b_navG[i].pos_xn) && t2_h1[u].get_yn() == b_navG[i].pos_yn && t2_h1[u].get_estado() == true) {
				t2_h1[u].set_estado(false);
				nave.set_bala_state(i);
				if (t2_h1[i].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t2_h1[i].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t2_h1[i].get_tipo() == comandante) {
					puntuacion += 200;
				}
			}
			else if ((t2_h2[u].get_xn() == b_navG[i].pos_xn || t2_h2[u].get_xn() + 1 == b_navG[i].pos_xn || t2_h2[u].get_xn() + 2 == b_navG[i].pos_xn) && t2_h2[u].get_yn() == b_navG[i].pos_yn && t2_h2[u].get_estado() == true) {
				t2_h2[u].set_estado(false);
				nave.set_bala_state(i);
				if (t2_h2[i].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t2_h2[i].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t2_h2[i].get_tipo() == comandante) {
					puntuacion += 200;
				}
			}
		}
		for (int w = 0; w < ct3; w++) {
			if ((t3_h1[w].get_xn() == b_navG[i].pos_xn || t3_h1[w].get_xn() + 1 == b_navG[i].pos_xn || t3_h1[w].get_xn() + 2 == b_navG[i].pos_xn) && t3_h1[w].get_yn() == b_navG[i].pos_yn && t3_h1[w].get_estado() == true) {
				t3_h1[w].set_estado(false);
				nave.set_bala_state(i);
				if (t3_h1[i].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t3_h1[i].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t3_h1[i].get_tipo() == comandante) {
					puntuacion += 200;
				}
			}
			else if ((t3_h2[w].get_xn() == b_navG[i].pos_xn || t3_h2[w].get_xn() + 1 == b_navG[i].pos_xn || t3_h2[w].get_xn() + 2 == b_navG[i].pos_xn) && t3_h2[w].get_yn() == b_navG[i].pos_yn && t3_h2[w].get_estado() == true) {
				t3_h2[w].set_estado(false);
				nave.set_bala_state(i);
				if (t3_h2[i].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t3_h2[i].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t3_h2[i].get_tipo() == comandante) {
					puntuacion += 200;
				}
			}
		}
	}
}
bool enm_eliminados(nav_enm*& t1_h1, nav_enm*& t1_h2, nav_enm*& t2_h1, nav_enm*& t2_h2, nav_enm*& t3_h1, nav_enm*& t3_h2, int& ct1, int& ct2, int& ct3) {
	int n1, n2, n3;  n1 = n2 = n3 = 0;
	bool r1, r2, r3; r1 = r2 = r3 = false;
	for (int i = 0; i < ct1; i++) {
		if (t1_h1[i].get_estado() == false && t1_h2[i].get_estado() == false) {
			n1++;
			if (n1 == ct1) {
				r1 = true;
			}
		}
	}
	for (int i = 0; i < ct2; i++) {
		if (t2_h1[i].get_estado() == false && t2_h2[i].get_estado() == false) {
			n2++;
			if (n2 == ct2) {
				r2 = true;
			}
		}
	}
	for (int i = 0; i < ct3; i++) {
		if (t3_h1[i].get_estado() == false && t3_h2[i].get_estado() == false) {
			n3++;
			if (n3 == ct3) {
				r3 = true;
			}
		}
	}
	if (r3 == true && r2 == true && r1 == true) {
		return true;
	}
	else
		return false;
}