#include "Main.h"
#include "3ds.h"
#include "Ase.h"
#pragma comment( lib, "fmod/fmodex_vc.lib" )

//Libreria que usamos para el timer
#pragma comment( lib, "winmm.lib" )	

HDC			hDC=NULL;		// Dispositivo de contexto GDI
HGLRC		hRC=NULL;		// Contexto de renderizado
HWND		hWnd=NULL;		// Manejador de ventana
HINSTANCE	hInstance;		// Instancia de la aplicacion

bool	keys[256];			// Arreglo para el manejo de teclado
bool	active=TRUE;		// Bandera de ventana activa

//Variables necesarias para FMOD
FMOD_SOUND       *sound[3] = { 0, 0, 0 };
FMOD_CHANNEL     *channel[3] = { 0, 0, 0 };

#define NUM_ENEMIGOS 12

// Arreglo para los enemigos
vector<Enemigo> enemigos( NUM_ENEMIGOS );
vector<Bala*> bill( NUM_ENEMIGOS );

// estas son las esferas que usaremos, en el siguiente orden:
// esfera 0 - jugador
// esfera 1 - bala del jugador
// esfera 2 -14 balas de los enemigos
// esfera 14 - 25 naves enemigas
vector<esfera> esferas( NUM_ENEMIGOS * 2 + 2 );

void AniMagic();

// Posicion del jugador
CVector player;

//Variables de Windows
HWND g_hMainWindow = NULL;
OPENFILENAME ofn;
char szFileName[MAX_PATH] = "";
char szFileTitle[MAX_PATH] = "";
 
int glWidth;
int glHeight;

GLUquadricObj	*e; 

CMateriales Material;

//TEMPORA PARA EL INDICADOR
/* Vidas del personaje */
float health = 150;
int vidas = 3;
//int red = 250;
int green = 200;
int score = 0;
int maxs = 3000;
int powerupw = 1;

// Variables de prueba para el movimiento de la nave :P
GLfloat incremento = 0.0f;
GLfloat posnavex = -5.0f;
GLfloat posnavey = 4.0f;
GLfloat posnavez = 0.0f;

GLfloat postrompax = posnavex + 5.0f;
GLfloat postrompay = posnavey - 3.9f;
GLfloat postrompaz = 0.0f;
GLfloat angulotrompa = 0.0f;

int limiteancho = 0;
int limitelargo = 320;

// variable para atacar
bool Ataque = false;
bool BorrarBola = false;
// variable para soltar el disparo
bool sueltaDisparo = false;
// variable para medir el tamano del ataque
float tamano = 0.0f;
int banderadisparo = 0;
GLfloat posdisparox = postrompax + 0.7f;
GLfloat posdisparoy = postrompay - 0.07f;
GLfloat posdisparoz = postrompaz;
GLfloat posdisparox1 = posdisparox;

//Nombre y ubicacion del modelo del plano
#define FILE_FONDO "Modelos/fondo.ase"

//Nombre y ubicación del modelo y trompa
#define FILE_NAVE  "Modelos/nave.ase"
#define FILE_TROMPA  "Modelos/trompa.ase"


//Nombre y ubicacion del primer tipo de nave enemiga
#define FILE_ENEMIGO1 "Modelos/nave_mala1.ase"

//Valor que permitira detener el avance del escenario
/* Duracion maxima de 5 minutos con lo sisguientes calculos
	20 seg = 100
	300 seg/ 20 seg = 15 veces
	300 seg = 100 x 15 = 1500
*/
int detenerse = 210;

//Contenedor de texturas de los modelos
CTga texturaNave[30];
CTga texturaTrompa[30];
CTga texturaNaveMala[30];
CTga texturaPlano[30];

CLoadASE g_LoadAse;

//Acceso a la estructura que almacena los datos del modelo
t3DModel ModeloNave;
t3DModel ModeloTrompa; 
t3DModel ModeloEnemigo1;
t3DModel ModeloPlano;

//Contenedor de texturas para el escenario
CTga textura[30];

//Variables de multitexturas y shaders
CMultitexturas Multitext;

//Constantes de iluminación y materiales
GLfloat LightPos[] = { 1.0f, 1.0f, 1.0f, 1.0f };			// Posición de la luz
GLfloat LightAmb[] = { 1.0f, 1.0f, 1.0f, 1.0f};			// Valores de la componente ambiente
GLfloat LightDif[] = { 1.0f, 1.0f, 1.0f, 1.0f};			// Valores de la componente difusa
GLfloat LightSpc[] = { 1.0f, 1.0f, 1.0f, 1.0f};			// Valores de la componente especular
static float exponent = 2.0f;

//Variables de control para la Trayectoria1 del Spline
GLfloat PuntoInicialSP1, IncrementoSP1;
GLfloat PuntoInicialSP2, IncrementoSP2;
GLfloat PuntoInicialSP3, IncrementoSP3;
GLfloat PuntoInicialSP4, IncrementoSP4;
GLfloat PuntoInicialSP5, IncrementoSP5;
GLfloat PuntoInicialSP6, IncrementoSP6;
GLfloat PuntoInicialSP7, IncrementoSP7;
GLfloat PuntoInicialSP8, IncrementoSP8;
GLfloat PuntoInicialSP9, IncrementoSP9;
GLfloat PuntoInicialSP10, IncrementoSP10;
GLfloat PuntoInicialSP11, IncrementoSP11;
GLfloat PuntoInicialSP12, IncrementoSP12;
GLfloat PuntoInicialSP13, IncrementoSP13;
GLfloat PuntoInicialSP14, IncrementoSP14;
GLfloat PuntoInicialSP15, IncrementoSP15;
GLfloat PuntoInicialSP16, IncrementoSP16;
GLfloat PuntoInicialSP17, IncrementoSP17;
GLfloat PuntoInicialSP18, IncrementoSP18;
GLfloat PuntoInicialSP19, IncrementoSP19;
GLfloat PuntoInicialSP20, IncrementoSP20;
GLfloat PuntoInicialSP21, IncrementoSP21;
GLfloat PuntoInicialSP22, IncrementoSP22;
GLfloat PuntoInicialSP23, IncrementoSP23;
GLfloat PuntoInicialSP24, IncrementoSP24;
GLfloat PuntoInicialSP25, IncrementoSP25;
GLfloat PuntoInicialSP26, IncrementoSP26;
GLfloat PuntoInicialSP27, IncrementoSP27;
GLfloat PuntoInicialSP28, IncrementoSP28;
GLfloat PuntoInicialSP29, IncrementoSP29;
GLfloat PuntoInicialSP30, IncrementoSP30;
GLfloat PuntoInicialSP31, IncrementoSP31;
GLfloat PuntoInicialSP32, IncrementoSP32;
GLfloat PuntoInicialSP33, IncrementoSP33;
GLfloat PuntoInicialSP34, IncrementoSP34;
GLfloat PuntoInicialSP35, IncrementoSP35;
GLfloat PuntoInicialSP36, IncrementoSP36;
GLfloat PuntoInicialSP37, IncrementoSP37;
GLfloat PuntoInicialSP38, IncrementoSP38;
GLfloat PuntoInicialSP39, IncrementoSP39;
GLfloat PuntoInicialSP40, IncrementoSP40;
GLfloat PuntoInicialSP41, IncrementoSP41;
GLfloat PuntoInicialSP42, IncrementoSP42;
GLfloat PuntoInicialSP43, IncrementoSP43;
GLfloat PuntoInicialSP44, IncrementoSP44;
GLfloat PuntoInicialSP45, IncrementoSP45;
GLfloat PuntoInicialSP46, IncrementoSP46;
GLfloat PuntoInicialSP47, IncrementoSP47;
GLfloat PuntoInicialSP48, IncrementoSP48;
GLfloat PuntoInicialSP49, IncrementoSP49;
GLfloat PuntoInicialSP50, IncrementoSP50;

//Variables de inicio del Spline
GLfloat inicioSP1 = 20.0f;
GLfloat inicioSP2 = 35.0f;
GLfloat inicioSP3 = 50.0f;
GLfloat inicioSP4 = 60.0f;
GLfloat inicioSP5 = 75.0f;
GLfloat inicioSP6 = 90.0f;
GLfloat inicioSP7 = 105.0f;
GLfloat inicioSP8 = 125.0f;
GLfloat inicioSP9 = 145.0f;
GLfloat inicioSP10 = 165.0f;
GLfloat inicioSP11 = 185.0f;
GLfloat inicioSP12 = 200.0f;
GLfloat inicioSP13 = 350.0f;
GLfloat inicioSP14 = 380.0f;
GLfloat inicioSP15 = 400.0f;
GLfloat inicioSP16 = 430.0f;
GLfloat inicioSP17 = 440.0f;
GLfloat inicioSP18 = 480.0f;
GLfloat inicioSP19 = 490.0f;
GLfloat inicioSP20 = 520.0f;
GLfloat inicioSP21 = 530.0f;
GLfloat inicioSP22 = 545.0f;
GLfloat inicioSP23 = 570.0f;
GLfloat inicioSP24 = 585.0f;
GLfloat inicioSP25 = 600.0f;
GLfloat inicioSP26 = 620.0f;
GLfloat inicioSP27 = 635.0f;
GLfloat inicioSP28 = 650.0f;
GLfloat inicioSP29 = 660.0f;
GLfloat inicioSP30 = 675.0f;
GLfloat inicioSP31 = 700.0f;
GLfloat inicioSP32 = 720.0f;
GLfloat inicioSP33 = 735.0f;
GLfloat inicioSP34 = 760.0f;
GLfloat inicioSP35 = 775.0f;
GLfloat inicioSP36 = 800.0f;
GLfloat inicioSP37 = 815.0f;
GLfloat inicioSP38 = 840.0f;
GLfloat inicioSP39 = 875.0f;
GLfloat inicioSP40 = 890.0f;
GLfloat inicioSP41 = 900.0f;
GLfloat inicioSP42 = 920.0f;
GLfloat inicioSP43 = 935.0f;
GLfloat inicioSP44 = 950.0f;
GLfloat inicioSP45 = 1020.0f;
GLfloat inicioSP46 = 1170.0f;
GLfloat inicioSP47 = 1200.0f;
GLfloat inicioSP48 = 1350.0f;
GLfloat inicioSP49 = 1400.0f;
GLfloat inicioSP50 = 1475.0f;

int Comenzar1;
int Comenzar2;
int Comenzar3;
int Comenzar4;
int Comenzar5;
int Comenzar6;
int Comenzar7;
int Comenzar8;
int Comenzar9;
int Comenzar10;
int Comenzar11;
int Comenzar12;
int Comenzar13;
int Comenzar14;
int Comenzar15;
int Comenzar16;
int Comenzar17;
int Comenzar18;
int Comenzar19;
int Comenzar20;
int Comenzar21;
int Comenzar22;
int Comenzar23;
int Comenzar24;
int Comenzar25;
int Comenzar26;
int Comenzar27;
int Comenzar28;
int Comenzar29;
int Comenzar30;
int Comenzar31;
int Comenzar32;
int Comenzar33;
int Comenzar34;
int Comenzar35;
int Comenzar36;
int Comenzar37;
int Comenzar38;
int Comenzar39;
int Comenzar40;
int Comenzar41;
int Comenzar42;
int Comenzar43;
int Comenzar44;
int Comenzar45;
int Comenzar46;
int Comenzar47;
int Comenzar48;
int Comenzar49;
int Comenzar50;

CVector lightPosition;

//Variables de la cámara
CVector PosicionCam;
CVector DireccionCam;
CVector VistaCam;
CVector SuperiorCam;
CVector PosicionCamAnt;
float AngCam;
float VelCam;

//Variables auxiliares para animación de texturas
float aText1;
float aText2;
float aText3;

//Acceso a la clase CFont
CFont Font;

//Variables para splines 1 y 2
spline Trayectoria1;
spline Trayectoria2;
spline Trayectoria3;
spline Trayectoria4;
spline Trayectoria5;
spline Trayectoria6;
spline Trayectoria7;
spline Trayectoria8;
spline Trayectoria9;
spline Trayectoria10;
spline Trayectoria11;
spline Trayectoria12;
spline Trayectoria13;
spline Trayectoria14;
spline Trayectoria15;
spline Trayectoria16;
spline Trayectoria17;
spline Trayectoria18;
spline Trayectoria19;
spline Trayectoria20;
spline Trayectoria21;
spline Trayectoria22;
spline Trayectoria23;
spline Trayectoria24;
spline Trayectoria25;
spline Trayectoria26;
spline Trayectoria27;
spline Trayectoria28;
spline Trayectoria29;
spline Trayectoria30;
spline Trayectoria31;
spline Trayectoria32;
spline Trayectoria33;
spline Trayectoria34;
spline Trayectoria35;
spline Trayectoria36;
spline Trayectoria37;
spline Trayectoria38;
spline Trayectoria39;
spline Trayectoria40;
spline Trayectoria41;
spline Trayectoria42;
spline Trayectoria43;
spline Trayectoria44;
spline Trayectoria45;
spline Trayectoria46;
spline Trayectoria47;
spline Trayectoria48;
spline Trayectoria49;
spline Trayectoria50;

CVector PosicionSpline1;
CVector PosicionSpline2;
CVector PosicionSpline3;
CVector PosicionSpline4;
CVector PosicionSpline5;
CVector PosicionSpline6;
CVector PosicionSpline7;
CVector PosicionSpline8;
CVector PosicionSpline9;
CVector PosicionSpline10;
CVector PosicionSpline11;
CVector PosicionSpline12;
CVector PosicionSpline13;
CVector PosicionSpline14;
CVector PosicionSpline15;
CVector PosicionSpline16;
CVector PosicionSpline17;
CVector PosicionSpline18;
CVector PosicionSpline19;
CVector PosicionSpline20;
CVector PosicionSpline21;
CVector PosicionSpline22;
CVector PosicionSpline23;
CVector PosicionSpline24;
CVector PosicionSpline25;
CVector PosicionSpline26;
CVector PosicionSpline27;
CVector PosicionSpline28;
CVector PosicionSpline29;
CVector PosicionSpline30;
CVector PosicionSpline31;
CVector PosicionSpline32;
CVector PosicionSpline33;
CVector PosicionSpline34;
CVector PosicionSpline35;
CVector PosicionSpline36;
CVector PosicionSpline37;
CVector PosicionSpline38;
CVector PosicionSpline39;
CVector PosicionSpline40;
CVector PosicionSpline41;
CVector PosicionSpline42;
CVector PosicionSpline43;
CVector PosicionSpline44;
CVector PosicionSpline45;
CVector PosicionSpline46;
CVector PosicionSpline47;
CVector PosicionSpline48;
CVector PosicionSpline49;
CVector PosicionSpline50;

// DIBUJAR CURVA A (NORMAL)
CVector PuntosSpline1[] = {
	CVector( inicioSP1 + 20.0f, -2.0f, 0.0f),
	CVector( inicioSP1 + 18.0f, -2.0f, 0.0f),
	CVector( inicioSP1 + 16.0f,  3.0f, 0.0f),
	CVector( inicioSP1 + 14.0f,  11.0f, 0.0f),
	CVector( inicioSP1 + 12.0f,  3.0f, 0.0f),
	CVector( inicioSP1 + 10.0f,  8.0f, 0.0f),
	CVector( inicioSP1 +  8.0f,  3.0f, 0.0f),
	CVector( inicioSP1 +  6.0f,  3.0f, 0.0f),
	CVector( inicioSP1 +  4.0f,  3.0f, 0.0f),
	CVector( inicioSP1 +  2.0f, -2.0f, 0.0f),
	CVector( inicioSP1, -3.0f, 0.0f),	
};
// DIBUJAR CURVA A (INVERTIDA)
CVector PuntosSpline2[] = {
	CVector( inicioSP2 + 20.0f,  3.0f, 0.0f),
	CVector( inicioSP2 + 18.0f, -2.0f, 0.0f),
	CVector( inicioSP2 + 16.0f,  3.0f, 0.0f),
	CVector( inicioSP2 + 14.0f,  3.0f, 0.0f),
	CVector( inicioSP2 + 12.0f,  3.0f, 0.0f),
	CVector( inicioSP2 + 10.0f,  8.0f, 0.0f),
	CVector( inicioSP2 +  8.0f,  3.0f, 0.0f),
	CVector( inicioSP2 +  6.0f, 11.0f, 0.0f),
	CVector( inicioSP2 +  4.0f,  3.0f, 0.0f),
	CVector( inicioSP2 +  2.0f, -2.0f, 0.0f),
	CVector( inicioSP2, -2.0f, 0.0f),
};
// DIBUJAR CURVA B (NORMAL)
CVector PuntosSpline3[] = {
	CVector( inicioSP3 +  5.0f, 15.0f, 0.0f),
	CVector( inicioSP3 +  4.0f, 13.0f, 0.0f),
	CVector( inicioSP3 +  3.0f, 11.0f, 0.0f),
	CVector( inicioSP3 +  2.0f,  9.0f, 0.0f),
	CVector( inicioSP3 +  1.0f,  7.0f, 0.0f),
	CVector( inicioSP3,  5.0f, 0.0f),
	CVector( inicioSP3 +  1.0f,  3.0f, 0.0f),
	CVector( inicioSP3 +  2.0f,	 1.0f, 0.0f),
	CVector( inicioSP3 +  3.0f, -1.0f, 0.0f),
	CVector( inicioSP3 +  4.0f, -3.0f, 0.0f),
	CVector( inicioSP3 +  5.0f, -5.0f, 0.0f),
};
// DIBUJAR CURVA B (INVERTIDA)
CVector PuntosSpline4[] = {
	CVector( inicioSP4, -5.0f, 0.0f),
	CVector( inicioSP4 +  1.0f, -3.0f, 0.0f),
	CVector( inicioSP4 +  2.0f, -1.0f, 0.0f),
	CVector( inicioSP4 +  3.0f,	 1.0f, 0.0f),
	CVector( inicioSP4 +  4.0f,  3.0f, 0.0f),
	CVector( inicioSP4 +  5.0f,  5.0f, 0.0f),
	CVector( inicioSP4 +  4.0f,  7.0f, 0.0f),
	CVector( inicioSP4 +  3.0f,  9.0f, 0.0f),
	CVector( inicioSP4 +  2.0f, 11.0f, 0.0f),
	CVector( inicioSP4 +  1.0f, 13.0f, 0.0f),
	CVector( inicioSP4, 15.0f, 0.0f),	
};
// DIBUJAR CURVA C (MEDIA)
CVector PuntosSpline5[] = {
	CVector( inicioSP5 + 14.0f,  4.0f, 0.0f),
	CVector( inicioSP5 + 12.0f,  4.0f, 0.0f),
	CVector( inicioSP5 + 10.0f,  4.0f, 0.0f),
	CVector( inicioSP5 +  8.0f,  4.0f, 0.0f),
	CVector( inicioSP5 +  6.0f,	 4.0f, 0.0f),
	CVector( inicioSP5 +  4.0f,  4.0f, 0.0f),
	CVector( inicioSP5 +  2.0f,  4.0f, 0.0f),
	CVector( inicioSP5,  4.0f, 0.0f),
	CVector( inicioSP5 -  2.0f,  4.0f, 0.0f),
	CVector( inicioSP5 -  4.0f,  4.0f, 0.0f),
	CVector( inicioSP5 -  6.0f,  4.0f, 0.0f),
};
// DIBUJAR CURVA C (ALTA)
CVector PuntosSpline6[] = {
	CVector( inicioSP6 + 14.0f,  7.0f, 0.0f),
	CVector( inicioSP6 + 12.0f,  7.0f, 0.0f),
	CVector( inicioSP6 + 10.0f,  7.0f, 0.0f),
	CVector( inicioSP6 +  8.0f,  7.0f, 0.0f),
	CVector( inicioSP6 +  6.0f,	 7.0f, 0.0f),
	CVector( inicioSP6 +  4.0f,  7.0f, 0.0f),
	CVector( inicioSP6 +  2.0f,  7.0f, 0.0f),
	CVector( inicioSP6,  7.0f, 0.0f),
	CVector( inicioSP6 -  2.0f,  7.0f, 0.0f),
	CVector( inicioSP6 -  4.0f,  7.0f, 0.0f),
	CVector( inicioSP6 -  6.0f,  7.0f, 0.0f),
};
// DIBUJAR CURVA C (BAJA)
CVector PuntosSpline7[] = {
	CVector( inicioSP7 + 14.0f,  1.0f, 0.0f),
	CVector( inicioSP7 + 12.0f,  1.0f, 0.0f),
	CVector( inicioSP7 + 10.0f,  1.0f, 0.0f),
	CVector( inicioSP7 +  8.0f,  1.0f, 0.0f),
	CVector( inicioSP7 +  6.0f,	 1.0f, 0.0f),
	CVector( inicioSP7 +  4.0f,  1.0f, 0.0f),
	CVector( inicioSP7 +  2.0f,  1.0f, 0.0f),
	CVector( inicioSP7,  1.0f, 0.0f),
	CVector( inicioSP7 -  2.0f,  1.0f, 0.0f),
	CVector( inicioSP7 -  4.0f,  1.0f, 0.0f),
	CVector( inicioSP7 -  6.0f,  1.0f, 0.0f),
};
// DIBUJAR CURVA D (NORMAL)
CVector PuntosSpline8[] = {
	CVector( inicioSP8 + 20.0f,  12.0f, 0.0f),
	CVector( inicioSP8 + 18.0f,  11.0f, 0.0f),
	CVector( inicioSP8 + 16.0f,  10.0f, 0.0f),
	CVector( inicioSP8 + 14.0f,   9.0f, 0.0f),
	CVector( inicioSP8 + 12.0f,   8.0f, 0.0f),
	CVector( inicioSP8 + 10.0f,   7.0f, 0.0f),
	CVector( inicioSP8 +  8.0f,   6.0f, 0.0f),
	CVector( inicioSP8 +  6.0f,   5.0f, 0.0f),
	CVector( inicioSP8 +  4.0f,   4.0f, 0.0f),
	CVector( inicioSP8 +  2.0f,   3.0f, 0.0f),
	CVector( inicioSP8,   2.0f, 0.0f),
	CVector( inicioSP8 -  2.0f,  1.0f, 0.0f),
	CVector( inicioSP8 -  4.0f, 0.0f, 0.0f),
};
// DIBUJAR CURVA D (INVERTIDA)
CVector PuntosSpline9[] = {
	CVector( inicioSP9 -  4.0f,  10.0f, 0.0f),
	CVector( inicioSP9 -  2.0f,   9.0f, 0.0f),
	CVector( inicioSP9,   8.0f,  0.0f),
	CVector( inicioSP9 +  2.0f,   7.0f, 0.0f),
	CVector( inicioSP9 +  4.0f,   6.0f, 0.0f),
	CVector( inicioSP9 +  6.0f,   5.0f, 0.0f),
	CVector( inicioSP9 +  8.0f,   4.0f, 0.0f),
	CVector( inicioSP9 + 10.0f,   3.0f, 0.0f),
	CVector( inicioSP9 + 12.0f,   2.0f, 0.0f),
	CVector( inicioSP9 + 14.0f,   1.0f, 0.0f),
	CVector( inicioSP9 + 16.0f,   0.0f, 0.0f),
	CVector( inicioSP9 + 18.0f, - 1.0f, 0.0f),
	CVector( inicioSP9 + 20.0f, - 2.0f, 0.0f),	
};
// DIBUJAR CURVA E (NORMAL)
CVector PuntosSpline10[] = {
	CVector( inicioSP10 + 20.0f,  11.0f, 0.0f),
	CVector( inicioSP10 + 18.0f,  10.0f, 0.0f),
	CVector( inicioSP10 + 16.0f,   9.0f, 0.0f),
	CVector( inicioSP10 + 14.0f,   8.0f, 0.0f),
	CVector( inicioSP10 + 12.0f,   7.0f, 0.0f),
	CVector( inicioSP10 + 10.0f,   6.0f, 0.0f),
	CVector( inicioSP10 + 10.0f,   5.0f, 0.0f),
	CVector( inicioSP10 + 10.0f,   4.0f, 0.0f),
	CVector( inicioSP10 + 10.0f,   3.0f, 0.0f),
	CVector( inicioSP10 + 10.0f,   2.0f, 0.0f),
	CVector( inicioSP10 + 12.0f,   1.0f, 0.0f),
	CVector( inicioSP10 + 14.0f,   0.0f, 0.0f),
	CVector( inicioSP10 + 16.0f, - 1.0f, 0.0f),
	CVector( inicioSP10 + 18.0f, - 2.0f, 0.0f),
	CVector( inicioSP10 + 20.0f, - 3.0f, 0.0f),
};
// DIBUJAR CRUVA E (ALTA)
CVector PuntosSpline11[] = {
	CVector( inicioSP11 + 10.0f,   11.0f, 0.0f),
	CVector( inicioSP11 +  9.0f,   10.0f, 0.0f),
	CVector( inicioSP11 +  8.0f,    9.0f, 0.0f),
	CVector( inicioSP11 +  7.0f,    8.0f, 0.0f),
	CVector( inicioSP11 +  6.0f,    7.0f, 0.0f),
	CVector( inicioSP11 +  5.0f,  6.0f, 0.0f),
	CVector( inicioSP11 +  4.0f,  6.0f, 0.0f),
	CVector( inicioSP11 +  3.0f,  6.0f, 0.0f),
	CVector( inicioSP11 +  2.0f,  6.0f, 0.0f),
	CVector( inicioSP11 +  1.0f,  6.0f, 0.0f),
	CVector( inicioSP11,    7.0f, 0.0f),
	CVector( inicioSP11 -  1.0f,    8.0f, 0.0f),
	CVector( inicioSP11 -  2.0f,    9.0f, 0.0f),
	CVector( inicioSP11 -  3.0f,   10.0f, 0.0f),
	CVector( inicioSP11 -  4.0f,   11.0f, 0.0f),
};
// DIBUJAR CRUVA E (BAJA)
CVector PuntosSpline12[] = {
	CVector( inicioSP12 + 10.0f, -  2.0f, 0.0f),
	CVector( inicioSP12 +  9.0f, -  1.0f, 0.0f),
	CVector( inicioSP12 +  8.0f,    0.0f, 0.0f),
	CVector( inicioSP12 +  7.0f,    1.0f, 0.0f),
	CVector( inicioSP12 +  6.0f,    2.0f, 0.0f),
	CVector( inicioSP12 +  5.0f,  3.0f, 0.0f),
	CVector( inicioSP12 +  4.0f,  3.0f, 0.0f),
	CVector( inicioSP12 +  3.0f,  3.0f, 0.0f),
	CVector( inicioSP12 +  2.0f,  3.0f, 0.0f),
	CVector( inicioSP12 +  1.0f,  3.0f, 0.0f),
	CVector( inicioSP12,    2.0f, 0.0f),
	CVector( inicioSP12 -  1.0f,    1.0f, 0.0f),
	CVector( inicioSP12 -  2.0f,    0.0f, 0.0f),
	CVector( inicioSP12 -  3.0f, -  1.0f, 0.0f),
	CVector( inicioSP12 -  4.0f, -  2.0f, 0.0f),
};

#define totalPC1 (sizeof(PuntosSpline1)/sizeof(CVector))
#define totalPC2 (sizeof(PuntosSpline2)/sizeof(CVector))
#define totalPC3 (sizeof(PuntosSpline3)/sizeof(CVector))
#define totalPC4 (sizeof(PuntosSpline4)/sizeof(CVector))
#define totalPC5 (sizeof(PuntosSpline5)/sizeof(CVector))
#define totalPC6 (sizeof(PuntosSpline6)/sizeof(CVector))
#define totalPC7 (sizeof(PuntosSpline7)/sizeof(CVector))
#define totalPC8 (sizeof(PuntosSpline8)/sizeof(CVector))
#define totalPC9 (sizeof(PuntosSpline9)/sizeof(CVector))
#define totalPC10 (sizeof(PuntosSpline10)/sizeof(CVector))
#define totalPC11 (sizeof(PuntosSpline11)/sizeof(CVector))
#define totalPC12 (sizeof(PuntosSpline12)/sizeof(CVector))

//Variables para el control de fps
GLfloat Time1;
GLfloat Time2;
GLfloat DiffTime;
static int FPS = 0;

bool  framerate_limit = true;

int	  adjust=2;											// Speed Adjustment For Really Slow Video Cards

struct			 										// Create A Structure For The Timer Information
{
  __int64       frequency;								// Timer Frequency
  float         resolution;								// Timer Resolution
  unsigned long mm_timer_start;							// Multimedia Timer Start Value
  unsigned long mm_timer_elapsed;						// Multimedia Timer Elapsed Time
  bool			performance_timer;						// Using The Performance Timer?
  __int64       performance_timer_start;				// Performance Timer Start Value
  __int64       performance_timer_elapsed;				// Performance Timer Elapsed Time
} timer;												// Structure Is Named timer

int		steps[6]={ 1, 2, 4, 5, 10, 20 };				// Stepping Values For Slow Video Adjustment

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaracion de WndProc (Procedimiento de ventana)

void TimerInit(void)
{
	memset(&timer, 0, sizeof(timer));

	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
	{
		timer.performance_timer	= FALSE;
		timer.mm_timer_start	= timeGetTime();
		timer.resolution		= 1.0f/1000.0f;
		timer.frequency			= 1000;
		timer.mm_timer_elapsed	= timer.mm_timer_start;
	}
	else
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
		timer.performance_timer			= TRUE;
		timer.resolution				= (float) (((double)1.0f)/((double)timer.frequency));
		timer.performance_timer_elapsed	= timer.performance_timer_start;
	}
}

float TimerGetTime()
{
	__int64 time;

	if (timer.performance_timer)
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);
		return ( (float) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
	{
		return( (float) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
	}
}

void CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;
    static float lastTime			= 0.0f;
	static char strFrameRate[50] = {0};

	float currentTime = GetTickCount() * 0.001f;				

	++framesPerSecond;

	if( currentTime - lastTime > 1.0f )
    {
	    lastTime = currentTime;
		
		FPS=int(framesPerSecond);
		
	    framesPerSecond = 0;
    }
}

void inicializaSpline(spline &sp, CVector * ctrl, int tot, int res)
{
	sp.puntosctrl = ctrl;
	sp.puntosporTramo = res;
	sp.totalPuntosControl = tot;
	sp.puntosaDibujar = (tot-3)*res;
}

void calculaPuntoenSpline(spline &sp, int indice,  CVector *P)
{ 
    int i, j;
	GLfloat t, t3, t2, c1, c2, c3, c4, _1_t;
	CVector Pj3, Pj2, Pj1, Pj0;

    indice = indice % sp.puntosaDibujar;
	j = indice / sp.puntosporTramo + 3;
	i = indice % sp.puntosporTramo;
	Pj3=sp.puntosctrl[j-3];
	Pj2=sp.puntosctrl[j-2];
	Pj1=sp.puntosctrl[j-1];
	Pj0=sp.puntosctrl[j-0];
	t = i/(double)sp.puntosporTramo;
	t2 = t*t; 
	t3 = t2*t;
	_1_t = 1-t;
	c1 = (_1_t*_1_t*_1_t) / 6;
	c2 = (3*t3-6*t2+4) / 6;
	c3 = (-3*t3+3*t2+3*t+1) / 6;
	c4 = t3 / 6;
	P->x = c1* Pj3.x + c2*Pj2.x + c3*Pj1.x + c4*Pj0.x;
	P->y = c1* Pj3.y + c2*Pj2.y + c3*Pj1.y + c4*Pj0.y;
	P->z = c1* Pj3.z + c2*Pj2.z + c3*Pj1.z + c4*Pj0.z;
}



GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Redimensiona e inicializa la ventana
{
	if (height==0)							// Para que no se presente una division por cero
	{
		height=1;							// la altura se iguala a 1
	}

	glViewport(0,0,width,height);					// Resetea el puerto de vista

	glMatrixMode(GL_PROJECTION);					// Selecciona la Matriz de Proyeccion
	glLoadIdentity();								// Resetea la Matriz de Proyeccion

	// Calcula el radio de aspecto o proporcion de medidas de la ventana
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,2000.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Selecciona la Matriz de Vista de Modelo
	glLoadIdentity();									// Resetea la Matriz de Vista de Modelo

	glWidth=width;
	glHeight=height;
}

void CargaTexturas()
{
	textura[0].LoadTGA("Texturas/player.tga");
	textura[1].LoadTGA("Texturas/cabeza.tga");
}

void DescargaTexturas()
{
	textura[0].Elimina();
	textura[1].Elimina();
}

int CargaModelosASE()
{
	if(!g_LoadAse.LoadAseFile(FILE_NAVE, &ModeloNave, texturaNave))
		return 0;

	if(!g_LoadAse.LoadAseFile(FILE_TROMPA, &ModeloTrompa, texturaTrompa))
		return 0;

	if(!g_LoadAse.LoadAseFile(FILE_ENEMIGO1, &ModeloEnemigo1, texturaNaveMala))
		return 0;

	if(!g_LoadAse.LoadAseFile(FILE_FONDO, &ModeloPlano, texturaPlano))
		return 0;

	return TRUE;
}

void DescargaModelosASE()
{
	g_LoadAse.UnLoadASEFile(&ModeloNave, texturaNave);
	g_LoadAse.UnLoadASEFile(&ModeloTrompa, texturaTrompa);
	g_LoadAse.UnLoadASEFile(&ModeloEnemigo1, texturaNaveMala);
	g_LoadAse.UnLoadASEFile(&ModeloPlano, texturaPlano);
}

void Camara(int funcion)
{	
	float alt, alt1;

	alt = VistaCam.y;
	alt1 = VistaCam.x;
	
	if(funcion == 0) //Inicializa la camara
	{
		//Inicia Posición
		PosicionCam.x = 0.0f;
		PosicionCam.y = 5.0f;
		PosicionCam.z = 10.0f;
		//Inicia Dirección
		AngCam = 270.0f;
		DireccionCam.x = (float)cos(AngCam*PI/180.0f);
		DireccionCam.y = 0.0f;
		DireccionCam.z = (float)sin(AngCam*PI/180.0f);
		//Inicia Punto de Vista
		VistaCam = PosicionCam+DireccionCam*10.0f;
		VistaCam.y=4.0f;
		alt=10.0f;
		//Inicia Vector Superior
		SuperiorCam.x = 0.0f;
		SuperiorCam.y = 1.0f;
		SuperiorCam.z = 0.0f;
		//Inicia Velocidad
		VelCam = 0.01f;
		
	}
	else if(funcion == 1) //Cambia direccion de la camara
	{
		DireccionCam.x = (float)cos(AngCam*PI/180.0f);
		DireccionCam.y = 0.0f;
		DireccionCam.z = (float)sin(AngCam*PI/180.0f);

		VistaCam = PosicionCam+DireccionCam*10.0f;
		VistaCam.y = alt;
	}
	else if(funcion == 2) //Desplaza la camara hacia adelante de su direccion
	{
		PosicionCam = PosicionCam+DireccionCam*VelCam;
		VistaCam = PosicionCam+DireccionCam*10.0f;
		VistaCam.y = alt;

		PosicionCamAnt = PosicionCam;
	}
	else if(funcion == 3) //Desplaza la camara hacia atras de su direccion
	{
		PosicionCam = PosicionCam-DireccionCam*VelCam;
		VistaCam = PosicionCam+DireccionCam*10.0f;
		VistaCam.y = alt;

		PosicionCamAnt = PosicionCam;
	}
	else if(funcion == 4) //Desplaza la camara hacia la izquierda de su direccion
	{
		PosicionCam.y+=0.5f;
		VistaCam.y = alt;
		VistaCam.y+=0.5f;
	}
	else if(funcion == 5) //Desplaza la camara hacia la derecha de su direccion
	{
		PosicionCam.x+=0.08f;
		VistaCam.x = alt1;
		VistaCam.x+=0.08f;
		incremento +=0.08f;
	}
		
}

//Inicio de Funciones de FMOD
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void LiberaSonido(FMOD_SYSTEM *system, FMOD_RESULT result)
{
	result = FMOD_Sound_Release(sound[0]);
    	ERRCHECK(result);
	result = FMOD_Sound_Release(sound[1]);
	ERRCHECK(result);
    	result = FMOD_System_Close(system);
    	ERRCHECK(result);
   	result = FMOD_System_Release(system);
	ERRCHECK(result);
}
//Fin de Funciones de FMOD

int InitGL(GLvoid)										// Aqui se configuran los parametros iniciales de OpenGL
{
	Multitext.InitMultitext(hWnd);

	glShadeModel(GL_SMOOTH);							// Activa Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Fondo de la Pantalla
	//glClearColor(0.1f, 0.2f, 0.3f, 0.5f);				// Fondo de la Pantalla
	glClearDepth(1.0f);									// Valor para el Depth Buffer
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);							// Activa Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Correccion de cálculos de perspectiva

	glCullFace(GL_BACK);								// Set Culling Face To Back Face
	glEnable(GL_CULL_FACE);								// Enable Culling

	//Iluminación
	//Inicializa la luz
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz0
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular
	/*
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.05f);   //Brillo de la luz
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f); //Iluminacion general de la escena
	*/
	
	glEnable(GL_LIGHT0);								// Activa luz0
	glEnable(GL_LIGHTING);								// Habilita la iluminación

	//CargaTexturas();

	CargaModelosASE();
	Font.BuildFont();

	//InitGLSL();
		
	e=gluNewQuadric();
	Camara(0);

	//Comienza la inicializacion de variables para Splines
	Comenzar1 = 1;
	Comenzar2 = 1;
	Comenzar3 = 1;
	Comenzar4 = 1;
	Comenzar5 = 1;
	Comenzar6 = 1;
	Comenzar7 = 1;
	Comenzar8 = 1;
	Comenzar9 = 1;
	Comenzar10 = 1;
	Comenzar11 = 1;
	Comenzar12 = 1;
	Comenzar13 = 1;
	Comenzar14 = 1;
	Comenzar15 = 1;
	Comenzar16 = 1;
	Comenzar17 = 1;
	Comenzar18 = 1;
	Comenzar19 = 1;
	Comenzar20 = 1;
	Comenzar21 = 1;
	Comenzar22 = 1;
	Comenzar23 = 1;
	Comenzar24 = 1;
	Comenzar25 = 1;
	Comenzar26 = 1;
	Comenzar27 = 1;
	Comenzar28 = 1;
	Comenzar29 = 1;
	Comenzar30 = 1;
	Comenzar31 = 1;
	Comenzar32 = 1;
	Comenzar33 = 1;
	Comenzar34 = 1;
	Comenzar35 = 1;
	Comenzar36 = 1;
	Comenzar37 = 1;
	Comenzar38 = 1;
	Comenzar39 = 1;
	Comenzar40 = 1;
	Comenzar41 = 1;
	Comenzar42 = 1;
	Comenzar43 = 1;
	Comenzar44 = 1;
	Comenzar45 = 1;
	Comenzar46 = 1;
	Comenzar47 = 1;
	Comenzar48 = 1;
	Comenzar49 = 1;
	Comenzar50 = 1;
	
	inicializaSpline(Trayectoria1, PuntosSpline1, totalPC1, 5);
	inicializaSpline(Trayectoria2, PuntosSpline2, totalPC2, 5);
	inicializaSpline(Trayectoria3, PuntosSpline3, totalPC3, 5);
	inicializaSpline(Trayectoria4, PuntosSpline4, totalPC4, 5);
	inicializaSpline(Trayectoria5, PuntosSpline5, totalPC5, 5);
	inicializaSpline(Trayectoria6, PuntosSpline6, totalPC6, 5);
	inicializaSpline(Trayectoria7, PuntosSpline7, totalPC7, 5);
	inicializaSpline(Trayectoria8, PuntosSpline8, totalPC8, 5);
	inicializaSpline(Trayectoria9, PuntosSpline9, totalPC9, 5);
	inicializaSpline(Trayectoria10, PuntosSpline10, totalPC10, 5);
	inicializaSpline(Trayectoria11, PuntosSpline11, totalPC11, 5);
	inicializaSpline(Trayectoria12, PuntosSpline12, totalPC12, 5);

	PuntoInicialSP1 = 1;
	PuntoInicialSP2 = 1;
	PuntoInicialSP3 = 1;
	PuntoInicialSP4 = 1;
	PuntoInicialSP5 = 1;
	PuntoInicialSP6 = 1;
	PuntoInicialSP7 = 1;
	PuntoInicialSP8 = 1;
	PuntoInicialSP9 = 1;
	PuntoInicialSP10 = 1;
	PuntoInicialSP11 = 1;
	PuntoInicialSP12 = 1;
	PuntoInicialSP13 = 1;
	PuntoInicialSP14 = 1;
	PuntoInicialSP15 = 1;
	PuntoInicialSP16 = 1;
	PuntoInicialSP17 = 1;
	PuntoInicialSP18 = 1;
	PuntoInicialSP19 = 1;
	PuntoInicialSP20 = 1;
	PuntoInicialSP21 = 1;
	PuntoInicialSP22 = 1;
	PuntoInicialSP23 = 1;
	PuntoInicialSP24 = 1;
	PuntoInicialSP25 = 1;
	PuntoInicialSP26 = 1;
	PuntoInicialSP27 = 1;
	PuntoInicialSP28 = 1;
	PuntoInicialSP29 = 1;
	PuntoInicialSP30 = 1;
	PuntoInicialSP31 = 1;
	PuntoInicialSP32 = 1;
	PuntoInicialSP33 = 1;
	PuntoInicialSP34 = 1;
	PuntoInicialSP35 = 1;
	PuntoInicialSP36 = 1;
	PuntoInicialSP37 = 1;
	PuntoInicialSP38 = 1;
	PuntoInicialSP39 = 1;
	PuntoInicialSP40 = 1;
	PuntoInicialSP41 = 1;
	PuntoInicialSP42 = 1;
	PuntoInicialSP43 = 1;
	PuntoInicialSP44 = 1;
	PuntoInicialSP45 = 1;
	PuntoInicialSP46 = 1;
	PuntoInicialSP47 = 1;
	PuntoInicialSP48 = 1;
	PuntoInicialSP49 = 1;
	PuntoInicialSP50 = 1;

	IncrementoSP1 = 0.2f;
	IncrementoSP2 = 0.2f;
	IncrementoSP3 = 0.3f;
	IncrementoSP4 = 0.3f;
	IncrementoSP5 = 0.3f;
	IncrementoSP6 = 0.3f;
	IncrementoSP7 = 0.3f;
	IncrementoSP8 = 0.3f;
	IncrementoSP9 = 0.3f;
	IncrementoSP10 = 0.3f;
	IncrementoSP11 = 0.3f;
	IncrementoSP12 = 0.3f;
	IncrementoSP13 = 0.3f;
	IncrementoSP14 = 0.3f;
	IncrementoSP15 = 0.3f;
	IncrementoSP16 = 0.3f;
	IncrementoSP17 = 0.3f;
	IncrementoSP18 = 0.3f;
	IncrementoSP19 = 0.3f;
	IncrementoSP20 = 0.3f;
	IncrementoSP21 = 0.3f;
	IncrementoSP22 = 0.3f;
	IncrementoSP23 = 0.3f;
	IncrementoSP24 = 0.3f;
	IncrementoSP25 = 0.3f;
	IncrementoSP26 = 0.3f;
	IncrementoSP27 = 0.3f;
	IncrementoSP28 = 0.3f;
	IncrementoSP29 = 0.3f;
	IncrementoSP30 = 0.3f;
	IncrementoSP31 = 0.3f;
	IncrementoSP32 = 0.3f;
	IncrementoSP33 = 0.3f;
	IncrementoSP34 = 0.3f;
	IncrementoSP35 = 0.3f;
	IncrementoSP36 = 0.3f;
	IncrementoSP37 = 0.3f;
	IncrementoSP38 = 0.3f;
	IncrementoSP39 = 0.3f;
	IncrementoSP40 = 0.3f;
	IncrementoSP41 = 0.3f;
	IncrementoSP42 = 0.3f;
	IncrementoSP43 = 0.3f;
	IncrementoSP44 = 0.3f;
	IncrementoSP45 = 0.3f;
	IncrementoSP46 = 0.3f;
	IncrementoSP47 = 0.3f;
	IncrementoSP48 = 0.3f;
	IncrementoSP49 = 0.3f;
	IncrementoSP50 = 0.3f;

	CVector player( posnavex, posnavey, posnavez );
	CVector nulo( -300.0f, -300.0f, -300.0f );

	// inicializa el radio de la nave
	esferas[0].radio = 0.5f;
	esferas[0].Pos = player;

	// inicializa el radio de la bala
	esferas[1].radio = tamano;

	// inicializa las balas
	for( int i = 0; i < bill.size(); i++ )
	{
		bill[ i ] = 0;
	}

	// inicializa las colisiones de los enemigos
	for( int i = 14; i < 25; i++ )
	{
		esferas[i].radio = 0.5f;
		esferas[i].Pos = nulo;
	}

	return TRUE;									
}

void DibujaEjes()
{
	glColor3f(1.0f,0.0f,0.0f);

	glBegin(GL_LINES);
		//Eje X
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);

		//Eje Y
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f,  100.0f, 0.0f);

		//Eje Z
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f,  100.0f);
	glEnd();

	glColor3f(1.0f,1.0f,1.0f);
}

void DibujaLuz(CVector l)
{
	//Dibuja una esfera que representa la fuente luminosa
	glDisable(GL_LIGHTING);									// Deshabilita iluminación
	glPushMatrix();
		glTranslatef(l.x, l.y, l.z);// Traslada a la posicion de la luz
		glColor4f(1.0f, 0.9f, 0.0f, 1.0f);					// Color amarillo
		gluSphere(e, 0.5f, 16, 8);							// Dibuja la esfera
	glPopMatrix();
	glEnable(GL_LIGHTING);									// Habilita Iluminación

	glColor3f(1.0f, 1.0f, 1.0f);
}

void DibujaTextos()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva la prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyecci�n
	glPushMatrix();										// Guarda la matriz de proyecci�n
		glLoadIdentity();									// Limpia la matriz de proyecci�n
		glOrtho(0,glWidth,0,glHeight,-1,1);					// Crea una proyecci�n ortogonal
		glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
		glPushMatrix();										// Guarda la matriz de modelo de vista
		
		glDisable(GL_LIGHTING);
		glLoadIdentity();

		glColor3f(1.0f,1.0f,1.0f);
		
		glEnable(GL_TEXTURE_2D);

		glAlphaFunc(GL_GREATER, 0.8f);
		glEnable(GL_ALPHA_TEST);

		//Dibujar el indicador de personaje

		glColor3f( 1.0f, 1.0f, 1.0f );

		glBindTexture(GL_TEXTURE_2D, textura[1].texID);

		// Indicar de vida
		glPushMatrix();
			glTranslatef( glWidth * 0.03f, glHeight * 0.05, 0.0f );
			glBegin(GL_QUADS);
				glTexCoord2f( 0.0f, 0.0f ); glVertex2f( 0.0f, 0.0f );
				glTexCoord2f( 1.0f, 0.0f ); glVertex2f( 50.0f, 0.0f );
				glTexCoord2f( 1.0f, 1.0f ); glVertex2f( 50.0f, 50.0f );
				glTexCoord2f( 0.0f, 1.0f ); glVertex2f( 0.0f, 50.0f );
			glEnd();
		glPopMatrix();

		// Texto a mostrar en pantalla
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.80f, glHeight * 0.95f, "Hi-Score" );
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.80f, glHeight * 0.91f, "%d", maxs );
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.80f, glHeight * 0.09f, "STAGE 1" );
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.02f, glHeight * 0.155f, "Player 1" );
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.02f, glHeight * 0.95f, "1P-Score" );
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.02f, glHeight * 0.91f, "%d", score );
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.45f, glHeight * 0.95f, "Weapon Type" );
		if(powerupw == 1)
		{
			Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.50f, glHeight * 0.91f, "FIRST" );
		}
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.1f, glHeight * 0.09f, " x %d", vidas );
									
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);

		//Barra de energia / health
		// Dibujale un contorno al contorno
		glPushMatrix();
			glTranslatef( glWidth * 0.115f, glHeight * 0.05, 0.0f );
			glColor3ub( 255, 255, 255 );
			glLineWidth( 2.0f );
			glBegin( GL_LINE_STRIP );
				glVertex2f( -3.0f, -3.0f );
				glVertex2f( 152.5f, -4.0f );
				glVertex2f( 152.5f, 17.0f );
				glVertex2f( -3.0f, 17.0f );
			glEnd();
		glPopMatrix();

		// Luego dibuja el contorno de la barra
		glPushMatrix();
			glTranslatef( glWidth * 0.115f, glHeight * 0.05, 0.0f );
			glColor3ub( 10, 10, 10 );
			glLineWidth( 5.0f );
			glBegin( GL_LINE_STRIP );
				glVertex2f( 0.0f, 0.0f );
				glVertex2f( 150.0f, 0.0f );
				glVertex2f( 150.0f, 14.0f );
				glVertex2f( 0.0f, 14.0f );
			glEnd();
			glLineWidth( 1.0f );
		glPopMatrix();

		// Dibuja el relleno de la barra
		glPushMatrix();
			glTranslatef( glWidth * 0.115f, glHeight * 0.052, 0.0f );
			glScalef( 1.0f, 0.9f, 0.9f );
			glColor3ub( 250, green, 0 );
			glBegin(GL_QUADS);
				glVertex2f( 0.0f, 0.0f );
				glVertex2f( health, 0.0f );
				glVertex2f( health, 14.0f );
				glVertex2f( 0.0f, 14.0f );
			glEnd();
		glPopMatrix();

		glColor3f(1.0f,1.0f,1.0f);

		glEnable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyecci�n
		glPopMatrix();										// Recupera la anterior matriz de proyecci�n
		glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Recupera la anterior matriz de modelo de vista
	glEnable(GL_DEPTH_TEST);							// Activa la prueba de profundidad
		
}

void ActualizaAnimText()
{
	//Variables para las flechas del piso
	if(aText1 < 1.0f)
		aText1+=0.005f;
	else
		aText1=0.0f;

	if(aText2 < 1.2f)
		aText2+=0.006f;
	else
		aText2=0.0f;

	if(aText3 < 1.6f)
		aText3+=0.008f;
	else
		aText3=0.0f;
			
}

void DibujaSpline(spline &sp)
{
	CVector P;

	glDisable(GL_LIGHTING);

	// Trayectoria1 del spline
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
		for(int i=0; i < sp.puntosaDibujar; i++ ) 
		{
			calculaPuntoenSpline(sp, i, &P);
			glVertex3f(P.x, P.y, P.z);
		}
	glEnd();

	//Puntos en el spline
	glPointSize(4.0f);
	glColor3f(0.0f,0.0f,1.0f);
	
	for(int i=0; i < sp.puntosaDibujar; i++ ) 
	{
		calculaPuntoenSpline(sp, i, &P);
		glBegin(GL_POINTS);
			glVertex3f(P.x, P.y, P.z);
		glEnd();
	}
			
	//Puntos de control
	glPointSize(10.0f);
	glColor3f(1.0f,0.0f,0.0f);

	for (int i=0; i<sp.totalPuntosControl; i++ )
	{
		glBegin(GL_POINTS);
			glVertex3f(sp.puntosctrl[i].x, sp.puntosctrl[i].y, sp.puntosctrl[i].z);
		glEnd();
		
	}

	glColor3f(1.0f,1.0f,1.0f);
	glPointSize(1.0f);

	glEnable(GL_LIGHTING);
	
}



void ActualizaIluminacion()
{
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz0
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular

	lightPosition.x=LightPos[0];
	lightPosition.y=LightPos[1];
	lightPosition.z=LightPos[2];
}

void DibujaNave()
{

	player.x = posnavex + incremento;
	player.y = posnavey;
	player.z = posnavez;

	esferas[0].Pos = player;

	GLUquadricObj *q;

	q = gluNewQuadric();
	glPushMatrix();	
		//Comienza a dibujar la nave y la coloca en una posición inicial dentro del escenario =D
		glTranslatef(posnavex + incremento, posnavey, posnavez);
		g_LoadAse.RenderASEFile(&ModeloNave, texturaNave, 1);
		// Modelado jerarquico, para colocar el disparador enfrente de la nave y que depende de la posicion que
		// tiene la nave =D
		glPushMatrix();
			glTranslatef(postrompax, postrompay, postrompaz);
			// Incrementa el angulo de inclinación de la nave en un solo eje :D
			glRotatef(angulotrompa, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, 0.0f, 0.0f);
			g_LoadAse.RenderASEFile(&ModeloTrompa, texturaTrompa, 1);
			if(Ataque == false)
			{
				glTranslatef(posdisparox - 0.05f, posdisparoy, posdisparoz);
				gluSphere(q,tamano,16,8);
			}
			else if (Ataque == true && banderadisparo == 1)
			{
				if(posdisparox1 < 12.0f)
				{
					posdisparox1 = posdisparox1 + 0.5f;
					glTranslatef(posdisparox1, posdisparoy, posdisparoz);
					gluSphere(q,tamano,16,8);
				}
				else
				{
					posdisparox1 = posdisparox;
					banderadisparo = 0;
					Ataque = false;
					tamano = 0.0f;
				}
			}
				//glPushMatrix();	
				//DibujaDisparo();
			//glPopMatrix();						// Dibuja la esfera
		glPopMatrix();
	glPopMatrix();
}

void DibujaPlano()
{
	glTranslatef(0.0f, 0.0f, 0.0f);
	g_LoadAse.RenderASEFile(&ModeloPlano, texturaPlano, 1);
}

int ColisionEsferaEsfera(esfera a, esfera b)
{
	// Calculate squared distance between centers
	CVector d = a.Pos - b.Pos;
	float dist2 = Punto(d, d);
	// Spheres intersect if squared distance is less than squared sum of radii
	float radiusSum = a.radio + b.radio;
	return dist2 <= radiusSum * radiusSum;
}

void LargeHadronCollider()
{

	// checamos colisiones contra balas enemigas
	for( int i = 2; i < 14; i++ )
	{
		if( bill[ i - 2 ] )
		{
			if( ColisionEsferaEsfera( esferas[0], esferas[i]) )
			{

				// borra la bala
				delete bill.at(i - 2);
				bill[i - 2] = 0;

				health -= 5.5f;
				//red -= 1;
				green = 200 - ( 150 - health );

				if( health <= 0.0f )
				{
					health = 150.0f;
					vidas = vidas - 1;
					//red = 250;
					green = 200;
				}
			}
		}
	}

	// checamos contra naves enemigas
	for( int i = 14; i < 25; i++ )
	{
		if( ColisionEsferaEsfera( esferas[0], esferas[i]) )
		{

			health -= 5.5f;
			//red -= 1;
			green = 200 - ( 150 - health );

			if( health <= 0.0f )
			{
				health = 150.0f;
				vidas = vidas - 1;
				green = 200;
			}
		}
	}

	// checamos enemigos contra disparos aliados
	for( int i = 14; i < 25; i++ )
	{
		if( ColisionEsferaEsfera( esferas[1], esferas[i]) )
		{

		}
	}
}

int RenderizaEscena(GLvoid)								// Aqui se dibuja todo lo que aparecera en la ventana
{

	GLUquadricObj *q;

	q = gluNewQuadric();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(PosicionCam.x,PosicionCam.y,PosicionCam.z,VistaCam.x,VistaCam.y,VistaCam.z,SuperiorCam.x,SuperiorCam.y,SuperiorCam.z);
	//DibujaEjes();
	//Se actualizan los parámetros de iluminación
	ActualizaIluminacion();
	// Dibujamos el Plano Gradiente (Negro-Blanco)
	DibujaPlano();
	// Invoca a la función que dibuja la nave XD
	DibujaNave();

	AniMagic();
	
	for( int i = 0; i < bill.size(); i++ )
	{
		if( bill[i] != 0 )
		{
			bill[i]->dBala();
			bill[i]->updateTarget( player );
			bill[i]->moveToTarget();
			esferas[i + 2].Pos = bill[i]->bala.Pos;
		}
	}

	LargeHadronCollider();

	CVector posicion;

	//Funcion que dibujara el Spline1
	if(incremento > inicioSP1)
	{
		if(PuntoInicialSP1 < Trayectoria1.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria1);

			PuntoInicialSP1 += IncrementoSP1;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria1, PuntoInicialSP1, &PosicionSpline1);
			//Dibujamos la nave enemiga 1 por todo el Spline 1
			glTranslatef(PosicionSpline1.x, PosicionSpline1.y, PosicionSpline1.z);
			// actualiza la posicion del enemigo
			enemigos[0].setPos(PosicionSpline1.x, PosicionSpline1.y, PosicionSpline1.z);

			// posiciona la esfera de los enemigos
			posicion.x = PosicionSpline1.x;
			posicion.y = PosicionSpline1.y;
			posicion.z = PosicionSpline1.z;

			esferas[ 14 ].Pos = posicion;


			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP2)
	{
		if(PuntoInicialSP2 < Trayectoria2.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria2);

			PuntoInicialSP2 += IncrementoSP2;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria2, PuntoInicialSP2, &PosicionSpline2);
			//Dibujamos la nave enemiga 1 por todo el Spline 2
			glTranslatef(PosicionSpline2.x, PosicionSpline2.y, PosicionSpline2.z);
			// actualiza la posicion del enemigo
			enemigos[1].setPos(PosicionSpline2.x, PosicionSpline2.y, PosicionSpline2.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP3)
	{
		if(PuntoInicialSP3 < Trayectoria3.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria3);

			PuntoInicialSP3 += IncrementoSP3;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria3, PuntoInicialSP3, &PosicionSpline3);
			//Dibujamos la nave enemiga 1 por todo el Spline 3
			glTranslatef(PosicionSpline3.x, PosicionSpline3.y, PosicionSpline3.z);
			// actualiza la posicion del enemigo
			enemigos[2].setPos(PosicionSpline3.x, PosicionSpline3.y, PosicionSpline3.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP4)
	{
		if(PuntoInicialSP4 < Trayectoria4.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria4);

			PuntoInicialSP4 += IncrementoSP4;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria4, PuntoInicialSP4, &PosicionSpline4);
			//Dibujamos la nave enemiga 1 por todo el Spline 4
			glTranslatef(PosicionSpline4.x, PosicionSpline4.y, PosicionSpline4.z);
			// actualiza la posicion del enemigo
			enemigos[3].setPos(PosicionSpline4.x, PosicionSpline4.y, PosicionSpline4.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP5)
	{
		if(PuntoInicialSP5 < Trayectoria5.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria5);

			PuntoInicialSP5 += IncrementoSP5;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria5, PuntoInicialSP5, &PosicionSpline5);
			//Dibujamos la nave enemiga 1 por todo el Spline 5
			glTranslatef(PosicionSpline5.x, PosicionSpline5.y, PosicionSpline5.z);
			// actualiza la posicion del enemigo
			enemigos[4].setPos(PosicionSpline5.x, PosicionSpline5.y, PosicionSpline5.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP6)
	{
		if(PuntoInicialSP6 < Trayectoria6.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria6);

			PuntoInicialSP6 += IncrementoSP6;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria6, PuntoInicialSP6, &PosicionSpline6);
			//Dibujamos la nave enemiga 1 por todo el Spline 6
			glTranslatef(PosicionSpline6.x, PosicionSpline6.y, PosicionSpline6.z);
			// actualiza la posicion del enemigo
			enemigos[5].setPos(PosicionSpline6.x, PosicionSpline6.y, PosicionSpline6.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP7)
	{
		if(PuntoInicialSP7 < Trayectoria7.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria7);

			PuntoInicialSP7 += IncrementoSP7;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria7, PuntoInicialSP7, &PosicionSpline7);
			//Dibujamos la nave enemiga 1 por todo el Spline 7
			glTranslatef(PosicionSpline7.x, PosicionSpline7.y, PosicionSpline7.z);
			// actualiza la posicion del enemigo
			enemigos[6].setPos(PosicionSpline7.x, PosicionSpline7.y, PosicionSpline7.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP8)
	{
		if(PuntoInicialSP8 < Trayectoria8.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria8);

			PuntoInicialSP8 += IncrementoSP8;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria8, PuntoInicialSP8, &PosicionSpline8);
			//Dibujamos la nave enemiga 1 por todo el Spline 8
			glTranslatef(PosicionSpline8.x, PosicionSpline8.y, PosicionSpline8.z);
			// actualiza la posicion del enemigo
			enemigos[7].setPos(PosicionSpline8.x, PosicionSpline8.y, PosicionSpline8.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP9)
	{
		if(PuntoInicialSP9 < Trayectoria9.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria9);

			PuntoInicialSP9 += IncrementoSP9;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria9, PuntoInicialSP9, &PosicionSpline9);
			//Dibujamos la nave enemiga 1 por todo el Spline 9
			glTranslatef(PosicionSpline9.x, PosicionSpline9.y, PosicionSpline9.z);
			// actualiza la posicion del enemigo
			enemigos[8].setPos(PosicionSpline9.x, PosicionSpline9.y, PosicionSpline9.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP10)
	{
		if(PuntoInicialSP10 < Trayectoria10.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria10);

			PuntoInicialSP10 += IncrementoSP10;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria10, PuntoInicialSP10, &PosicionSpline10);
			//Dibujamos la nave enemiga 1 por todo el Spline 10
			glTranslatef(PosicionSpline10.x, PosicionSpline10.y, PosicionSpline10.z);
			// actualiza la posicion del enemigo
			enemigos[9].setPos(PosicionSpline10.x, PosicionSpline10.y, PosicionSpline10.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP11)
	{
		if(PuntoInicialSP11 < Trayectoria11.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria11);

			PuntoInicialSP11 += IncrementoSP11;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria11, PuntoInicialSP11, &PosicionSpline11);
			//Dibujamos la nave enemiga 1 por todo el Spline 11
			glTranslatef(PosicionSpline11.x, PosicionSpline11.y, PosicionSpline11.z);
			// actualiza la posicion del enemigo
			enemigos[10].setPos(PosicionSpline11.x, PosicionSpline11.y, PosicionSpline11.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}
	if(incremento > inicioSP12)
	{
		if(PuntoInicialSP12 < Trayectoria12.puntosaDibujar)
		{	
			//Dibujamos la trayectoria que recorrera la nave
			DibujaSpline(Trayectoria12);

			PuntoInicialSP12 += IncrementoSP12;
			//Obtenemos nueva posicion de la nave dentro del Spline
			calculaPuntoenSpline(Trayectoria12, PuntoInicialSP12, &PosicionSpline12);
			//Dibujamos la nave enemiga 1 por todo el Spline 12
			glTranslatef(PosicionSpline12.x, PosicionSpline12.y, PosicionSpline12.z);
			// actualiza la posicion del enemigo
			enemigos[11].setPos(PosicionSpline12.x, PosicionSpline12.y, PosicionSpline12.z);
			glScalef(0.05f,0.05f,0.05f);
			g_LoadAse.RenderASEFile(&ModeloEnemigo1, texturaNaveMala, 1);
		}
	}

	if(incremento < detenerse)
		Camara(5);
	
	DibujaTextos();
	//CalculateFrameRate();

	return TRUE;
}

GLvoid KillGLWindow(GLvoid)								// Elimina la ventana apropiadamente
{
	if (hRC)											// Si existe un contexto de renderizado...
	{
		if (!wglMakeCurrent(NULL,NULL))					// Si no se pueden liberar los contextos DC y RC...
		{
			MessageBox(NULL,"Falla al liberar DC y RC.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Si no se puede eliminar el RC?
		{
			MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Se pone RC en NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Si no se puede eliminar el DC
	{
		MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Se pone DC en NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Si no se puede destruir la ventana
	{
		MessageBox(NULL,"No se pudo liberar hWnd.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Se pone hWnd en NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Si no se puede eliminar el registro de la clase
	{
		MessageBox(NULL,"No se pudo eliminar el registro de la clase.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Se pone hInstance en NULL
	}
}

//	Este código crea la ventana de OpenGL.  Parámetros:					
//	title			- Titulo en la parte superior de la ventana			
//	width			- Ancho de la ventana								
//	height			- Alto de la ventana								
//	bits			- Número de bits a usar para el color (8/16/24/32)	
  
BOOL CreateGLWindow(char* title, int width, int height, int bits)
{
	GLuint	PixelFormat;				// Guarda el resultado despues de determinar el formato a usar
	WNDCLASS	wc;						// Estructura de la clase ventana
	DWORD		dwExStyle;				// Estilo extendido de ventana
	DWORD		dwStyle;				// Estilo de ventana
	RECT		WindowRect;				// Guarda los valores Superior Izquierdo / Inferior Derecho del rectángulo
	WindowRect.left=(long)0;			// Inicia el valor Izquierdo a 0
	WindowRect.right=(long)width;		// Inicia el valor Derecho al ancho especificado
	WindowRect.top=(long)0;				// Inicia el valor Superior a 0
	WindowRect.bottom=(long)height;		// Inicia el valor Inferior al alto especificado

	hInstance			= GetModuleHandle(NULL);				// Guarda una instancia de la ventana
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redibuja el contenido de la ventana al redimensionarla
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// Maneja los mensajes para WndProc
	wc.cbClsExtra		= 0;									// Ningun dato extra para la clase
	wc.cbWndExtra		= 0;									// Ningun dato extra para la ventana
	wc.hInstance		= hInstance;							// Inicia la instancia
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Carga el ícono por defecto
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Carga el puntero de flecha
	wc.hbrBackground	= NULL;									// No se requiere ningun fondo
	wc.lpszMenuName		= NULL;									// No hay menú en la ventana
	wc.lpszClassName	= "OpenGL";								// Fija el nombre de la clase.

	if (!RegisterClass(&wc))									// Intenta registrar la clase de ventana
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
		
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;					// Estilo extendido de ventana
	dwStyle=WS_OVERLAPPEDWINDOW;									// Estilo de ventana

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Ajusta la ventana al tamaño especificado

	// Crea la ventana
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Estilo extendido para la ventana
								"OpenGL",						// Nombre de la clase
								title,								// Título de la ventana
								dwStyle |							// Definición del estilo de la ventana
								WS_CLIPSIBLINGS |					// Estilo requerido de la ventana
								WS_CLIPCHILDREN,					// Estilo requerido de la ventana
								0, 0,								// Posición de la ventana
								WindowRect.right-WindowRect.left,	// Calcula el ancho de la ventana
								WindowRect.bottom-WindowRect.top,	// Calcula el alto de la ventana
								NULL,								// No hay ventana superior
								NULL,								// No hay menú
								hInstance,							// Instancia
								NULL)))								// No se pasa nada a WM_CREATE
	{
		KillGLWindow();												// Resetea el despliegue
		MessageBox(NULL,"Error al crear la ventana.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		1,											// 16Bit Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Si no se creo el contexto de dispositivo...
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de dispositivo GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Si Windows no encontró un formato de pixel compatible
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede encontrar un formato de pixel compatible.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Si no se pudo habilitar el formato de pixel
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el formato de pixel.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				// Si no se creo el contexto de renderizado
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					// Si no se puede activar el contexto de renderizado
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);				// Muestra la ventana
	SetForegroundWindow(hWnd);				// Le da la prioridad mas alta
	SetFocus(hWnd);							// Pasa el foco del teclado a la ventana
	ReSizeGLScene(width, height);			// Inicia la perspectiva para la ventana OGL

	if (!InitGL())							// Si no se inicializa la ventana creada
	{
		KillGLWindow();						// Resetea el despliegue
		MessageBox(NULL,"Falla en la inicialización.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;							// Todo correcto
}

LRESULT CALLBACK WndProc(	HWND	hWnd,	// Manejador para esta ventana
							UINT	uMsg,	// Mensaje para esta ventana
							WPARAM	wParam,	// Información adicional del mensaje
							LPARAM	lParam)	// Información adicional del mensaje
{
	switch (uMsg)							// Revisa los mensajes de la ventana
	{
		case WM_ACTIVATE:					// Revisa el mensaje de activación de ventana
		{
			if (!HIWORD(wParam))			// Revisa el estado de minimización
			{
				active=TRUE;				// El programa está activo
			}
			else
			{
				active=FALSE;				// El programa no está activo
			}

			return 0;						// Regresa al ciclo de mensajes
		}

		case WM_SYSCOMMAND:					// Intercepta comandos del sistema
		{
			switch (wParam)					// Revisa llamadas del sistema
			{
				case SC_SCREENSAVE:			// ¿Screensaver tratando de iniciar?
				case SC_MONITORPOWER:		// ¿Monitor tratando de entrar a modo de ahorro de energía?
				return 0;					// Evita que suceda
			}
			break;							// Sale del caso
		}

		case WM_CLOSE:						// Si se recibe un mensaje de cerrar...
		{
			PostQuitMessage(0);				// Se manda el mensaje de salida
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYDOWN:					// Si se está presionando una tecla...
		{
			keys[wParam] = TRUE;			// Si es así, se marca como TRUE
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYUP:						// ¿Se ha soltado una tecla?
		{
			keys[wParam] = FALSE;			// Si es así, se marca como FALSE
			return 0;						// y se regresa al ciclo
		}

		case WM_SIZE:						// Si se redimensiona la ventana...
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  	// LoWord=Width, HiWord=Height
			return 0;						// y se regresa al ciclo
		}
	}

	// Pasa todos los mensajes no considerados a DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

// Este es el punto de entrada al programa; la función principal 
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instancia
					HINSTANCE	hPrevInstance,		// Instancia previa
					LPSTR		lpCmdLine,			// Parametros de la linea de comandos
					int			nCmdShow)			// Muestra el estado de la ventana
{
	MSG		msg;									// Estructura de mensajes de la ventana
	BOOL	done=FALSE;								// Variable booleana para salir del ciclo

	//Variable FMOD
	FMOD_SYSTEM      *system;
	FMOD_RESULT       result;
    unsigned int      version;

	unsigned int ms = 0;
    unsigned int lenms = 0;
    int          playing = 0;
    int          paused = 0;
    int          channelsplaying = 0;

	// Crea la ventana OpenGL
	if (!CreateGLWindow("Computación Gráfica - Proyecto Final",640,480,32))
	{
		return 0;									// Salir del programa si la ventana no fue creada
	}

	TimerInit();
	//Se inicia el sistema de sonido de FMOD
    result = FMOD_System_Create(&system);
    ERRCHECK(result);

    result = FMOD_System_GetVersion(system, &version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }

    result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(system, "audio/fondo.mp3", FMOD_HARDWARE, 0, &sound[0]);
    ERRCHECK(result);

    result = FMOD_Sound_SetMode(sound[0], FMOD_LOOP_NORMAL); //se repite constantemente durante la reproducción
    ERRCHECK(result);										

    result = FMOD_System_CreateSound(system, "audio/disparo.wav", FMOD_SOFTWARE, 0, &sound[1]);
    ERRCHECK(result);

	result = FMOD_System_CreateSound(system, "audio/powerup.mp3", FMOD_SOFTWARE, 0, &sound[2]);
    ERRCHECK(result);

    //Se asocian los sonidos a los canales	
    result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[0], 0, &channel[0]);
    ERRCHECK(result);
    result = FMOD_Channel_SetPaused(channel[0], FALSE); //al iniciar el programa comenzará la reproducción
    ERRCHECK(result);

    result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[1], 0, &channel[1]);
    ERRCHECK(result);
    result = FMOD_Channel_SetPaused(channel[1], TRUE); //sonido 2 inicialmente en pausa
    ERRCHECK(result);

	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[2], 0, &channel[2]);
    ERRCHECK(result);
    result = FMOD_Channel_SetPaused(channel[2], TRUE); //sonido 3 inicialmente en pausa
    ERRCHECK(result);

	while(!done)									// Mientras done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Revisa si hay mensajes en espera
		{
			if (msg.message==WM_QUIT)				// Si se ha recibido el mensje de salir...
			{
				done=TRUE;							// Entonces done=TRUE
			}
			else									// Si no, Procesa los mensajes de la ventana
			{
				TranslateMessage(&msg);				// Traduce el mensaje
				DispatchMessage(&msg);				// Envia el mensaje
			}
		}
		else										// Si no hay mensajes...
		{
		
			// Dibuja la escena. 
			if (active)								// Si está activo el programa...
			{
				float start=TimerGetTime();

				if (keys[VK_ESCAPE])				// Si se ha presionado ESC
				{
					done=TRUE;						// ESC indica el termino del programa
				}
				else								// De lo contrario, actualiza la pantalla
				{
					FMOD_System_Update(system);

					RenderizaEscena();				// Dibuja la escena
					SwapBuffers(hDC);				// Intercambia los Buffers (Double Buffering)
				}

				if(!ManejaTeclado()) return 0;

				if(!ProcessKeys(system, result)) return 0;
		
				while(TimerGetTime()<start+float(steps[adjust]*2.0f)) {}	// Desperdicia ciclos si es demasiado rápido
			}
			
		}
	}

	// Finalización del programa
	//DescargaTexturas();
	DescargaModelosASE();
	Font.DestroyFont();
	LiberaSonido(system, result);
	KillGLWindow();									// Destruye la ventana
	return (msg.wParam);							// Sale del programa
}

int ManejaTeclado()
{
	if(keys[VK_UP])
	{	if(limitelargo <= 356)
		{
			posnavey += 0.1f;
			//Camara(2);
			limitelargo++;
		}
	}
	if(keys[VK_DOWN])
	{
		if(limitelargo >= 280)
		{
			posnavey -= 0.1f;
			/*Camara(3);*/
			limitelargo--;
		}
	}
	if(keys[VK_LEFT])
	{
		if(limiteancho >= -1)
		{
			posnavex -= 0.1f;
			/*AngCam-=1.0f;
			if(AngCam < 0.0f)
				AngCam+=360.0f;

			Camara(1);*/
			limiteancho--;
		}
	}

	if(keys[VK_RIGHT])
	{
		if(limiteancho <= 100)
		{
			posnavex += 0.1f;
			/*AngCam+=1.0f;
			if(AngCam > 360.0f)
				AngCam-=360.0f;
			
			Camara(1);*/
			limiteancho++;
		}
	}

	/* Para disparar, vemos si esta presionada la tecla, si lo esta, ponemos ataque = true, para dibujar la esfera en la nave
	 * PERO, no vamos a hacer que se mueva aun, esperaremos a que sueltaDisparo sea = true tambien, para dar la oportunidad de 
	 * que la pelotita crezca
	 */
	//if (GetKeyState(VK_SPACE) & 0x80)
	if (keys['Q'])
	{ 
    	tamano += 0.01f; // empieza a cargar este disparo
		if( 0.18f <= tamano ) // pero no te hagas demasiado grande, por jis
				tamano = 0.14f;
		if(tamano > 0.0f)
			sueltaDisparo = true;
		else
			sueltaDisparo = false;
	} 
	else if(sueltaDisparo)
	{
		Ataque = true;
		banderadisparo = 1;
	} 

	if(keys['D'])
	{
		angulotrompa += 1.0f;
		if(angulotrompa > 360.0f)
		{
			angulotrompa = 0.0f;
		}
	}
	if(keys['F'])
	{
		angulotrompa -= 1.0f;
		if(angulotrompa < 0.0f)
		{
			angulotrompa = 360.0f;
			//angulotrompa += 1.0f;
		}
	}
	/*if(keys[VK_PRIOR])
	{
		Camara(4);
	}
	if(keys[VK_NEXT])
	{
		Camara(5);
	}

	if(keys[VK_HOME])
	{
		VistaCam.y+=0.5f;
	}

	if(keys[VK_END])
	{
		VistaCam.y-=0.5f;
	}

	if((GetAsyncKeyState(VK_F1)&1) ==1)
	{
		VistaCam.y=PosicionCam.y;
	}

	//Controles de la iluminación
	if (keys['Z'])
		LightPos[0] -= 1.0f; //Hacia la derecha

	if (keys['X'])
		LightPos[0] += 1.0f; //Hacia la izquierda

	if (keys['C'])
		LightPos[1] += 1.0f; //Hacia arriba

	if (keys['V'])
		LightPos[1] -= 1.0f; //Hacia abajo

	if (keys['B'])
		LightPos[2] += 1.0f; //Hacia adelante

	if (keys['N'])
		LightPos[2] -= 1.0f; //Hacia atrás
	*/

	return TRUE;
}

int ProcessKeys(FMOD_SYSTEM *system, FMOD_RESULT result)
{
	if((GetAsyncKeyState('Q')&1) ==1) //Otra forma de usar el teclado: Detecta el evento de pulsar la tecla
	{
		result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[1], 0, &channel[1]);
		ERRCHECK(result);

	}
	
	
	
	
	return 1;
}

// Balas de los malos
void AniMagic()
{
	for( int i = 0; i < enemigos.size(); i++ )
	{
		if( ( enemigos[i].getPos().z < 6666.0f ) )
		{
			//enemigos[i].startAnim();

			if( enemigos[i].Reload() <= 0 )
			{
				if( bill[i] == 0 )
				{
					CVector disparo;
					disparo = enemigos[i].attack();
					bill[i] = new Bala( disparo, player );
					esferas[ i + 2 ].radio = bill[ i ]->bala.radio;
					esferas[ i + 2 ].Pos = bill[ i ]->bala.Pos;
				}
				else
				{
					delete bill.at(i);
					bill[i] = 0;
				}

			}
		}
	}

}