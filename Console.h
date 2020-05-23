//console.h 

// *******************************************************************
// MODULO DE CONSOLE
//
// Uso: Para dar cor de uma maneira facil a aplicacoes de console)
//
// Primeira versão: Fábio Vinicius Binder
// Melhorias:       Marvin Oliver Schneider
// *******************************************************************

//Includes necessários
#include <wtypes.h>
#include <wincon.h>
#include <string.h>

//Constantes para cores específicas
#define XBACKGROUND_BLACK        (WORD) 0x0000
#define XBACKGROUND_BLUE         (WORD) 0x0010
#define XBACKGROUND_GREEN        (WORD) 0x0020
#define XBACKGROUND_CYAN         (WORD) 0x0030
#define XBACKGROUND_RED          (WORD) 0x0040
#define XBACKGROUND_PURPLE       (WORD) 0x0050
#define XBACKGROUND_BROWN        (WORD) 0x0060
#define XBACKGROUND_LIGHTGRAY    (WORD) 0x0070
#define XBACKGROUND_DARKGRAY     (WORD) 0x0080
#define XBACKGROUND_LIGHTBLUE    (WORD) 0x0090
#define XBACKGROUND_LIGHTGREEN   (WORD) 0x00a0
#define XBACKGROUND_LIGHTCYAN    (WORD) 0x00b0
#define XBACKGROUND_LIGHTRED     (WORD) 0x00c0
#define XBACKGROUND_LIGHTPURPLE  (WORD) 0x00d0
#define XBACKGROUND_YELLOW       (WORD) 0x00e0
#define XBACKGROUND_WHITE        (WORD) 0x00f0

#define XFOREGROUND_BLACK        (WORD) 0x0000
#define XFOREGROUND_BLUE         (WORD) 0x0001
#define XFOREGROUND_GREEN        (WORD) 0x0002
#define XFOREGROUND_CYAN         (WORD) 0x0003
#define XFOREGROUND_RED          (WORD) 0x0004
#define XFOREGROUND_PURPLE       (WORD) 0x0005
#define XFOREGROUND_BROWN        (WORD) 0x0006
#define XFOREGROUND_LIGHTGRAY    (WORD) 0x0007
#define XFOREGROUND_DARKGRAY     (WORD) 0x0008
#define XFOREGROUND_LIGHTBLUE    (WORD) 0x0009
#define XFOREGROUND_LIGHTGREEN   (WORD) 0x000a
#define XFOREGROUND_LIGHTCYAN    (WORD) 0x000b
#define XFOREGROUND_LIGHTRED     (WORD) 0x000c
#define XFOREGROUND_LIGHTPURPLE  (WORD) 0x000d
#define XFOREGROUND_YELLOW       (WORD) 0x000e
#define XFOREGROUND_WHITE        (WORD) 0x000f

// !! Prototipos a seguir !!

// *****************************************************************************
// Funcao: CLRSCR
//
// Descricao: Limpa a tela da console com determinadas cores de escrita e fundo
//
// Parametros: clrscr( cor_esrita | cor_fundo );
//
// Exemplo: clrscr( FOREGROUND_YELLOW | BACKGROUND_BLACK );
// *****************************************************************************

void clrscr( WORD atributos );

// *****************************************************************************
// Funcao: GOTOXY
//
// Descricao: Coloca o cursor em determinada posicao
//
// Parametros: gotoxy(x,y);
//
// Exemplo: gotoxy(2,15);
// *****************************************************************************

void gotoxy (int x, int y);

// *****************************************************************************
// Funcao: SETCOLOR
//
// Descricao: Muda as cores de escrita e fundo
//
// Parametros: setcolor( cor_esrita | cor_fundo );
//
// Exemplo: setcolor( FOREGROUND_YELLOW | BACKGROUND_BLACK );
// *****************************************************************************

void setcolor ( WORD atributos );

// !! Funcoes a seguir !!

void clrscr( WORD atributos )
{
	#define MY_BUFSIZE 1024			// Tamanho do buffer para o título da janela

	COORD coordScreen = { 0, 0 };   // Posição do cursor
	BOOL bSuccess;					// Auxiliar para armazenamento do retorno das funções
	DWORD cCharsWritten,
	      dwConSize;                // Número de caracteres atuais da janela
	CONSOLE_SCREEN_BUFFER_INFO csbi;// Informações sobre a janela
	HANDLE hConsole;				// Handle para a janela
 

	//Recupera o handle da janela de console
	hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

	//Recupera informações sobre a janela
	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );

	//Calcula número de caracteres
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
 
	//Preenche janela com brancos
    bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
							dwConSize, coordScreen, &cCharsWritten );
 
	//Preenche janela com o atributo escolhido
	bSuccess = FillConsoleOutputAttribute( hConsole, atributos,
							dwConSize, coordScreen, &cCharsWritten );
 
	//Posiciona o cursor no canto superior esquerdo 
	bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
}

void gotoxy (int x, int y)
{
	HANDLE hConsole;			// Handle para a janela
	COORD coord = { x, y };		// Posição do cursor

	//Recupera o handle da janela de console
	hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

	//Posiciona o cursor no canto superior esquerdo 
	SetConsoleCursorPosition( hConsole, coord );
}

void setcolor ( WORD atributos )
{
	HANDLE hConsole;			// Handle para a janela

	//Recupera o handle da janela de console
	hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

	//Altera os atributos da janela
	SetConsoleTextAttribute (hConsole, atributos);

}