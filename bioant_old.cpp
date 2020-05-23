/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                         B   I   O   A   N   T                       //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                        Projeto de Dissertacao                       //
//                   Mestrado em Sistemas de Computacao                //
//                             PUC-Campinas                            //
//                                                                     //
//                     Aluno: Marvin Oliver Schneider                  //
//                                                                     //
//                Orientador: Prof. Dr. Joao Luis Garcia Rosa          //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

// TAREFAS
// 1. Delay de Subida
// 2. Implementacao reste algoritmo
// 3. Finalizacao Debug

// Gott, bitte vergib mir meine Fehler sofern ich sie nicht mehr begehe.

/////////////////////////////////////////////////////////////////////////
// 1. INCLUDES                                                         //
/////////////////////////////////////////////////////////////////////////
#include "stdafx.h"  // requisito do Visual C++ .Net
#include "stdio.h"   // funcoes padrao (1)
#include "stdlib.h"  // funcoes padrao (2)
#include "math.h"    // funcoes matematicas
#include "conio.h"   // console - principalmente para getch()
#include "console.h" // funcoes de console facilitadas
#include "time.h" // para o "delay"

/////////////////////////////////////////////////////////////////////////
// 2. CONSTANTES                                                       //
/////////////////////////////////////////////////////////////////////////

const char *versao = "0.12";

const int simples  = 0; // para as caixas de mensagem
const int alerta   = 1;
const int pergunta = 2;
const int aviso    = 3;
const int chave    = 4;

const int mais     = 1; // fases de propagacao
const int menos    = 0;

const int entradas_neuronios = 0;
const int neuronios_saidas   = 1;
const int saidas_neuronios   = 2;

const int quebra   = 60; // nro de elementos por pagina para o dump

const int nro_entradas  = 10;  // parametros da rede
const int nro_neuronios_max = 100;
const int nro_saidas    = 10;

const int max_colunas_menu = 16;  // parametrizacao do menu
const int max_linhas_menu  = 14;

const int max_x_tela = 80; // apenas para melhor legibilidade

const int caixa_padrao = 12; // y-coordenada do inicio das caixas de mensagem

const float desligado = -1.0; // para inicializacao das ativacoes

//const int EN  = 0;  // para o GeneRec  // SEM USO -- VERSAO ANTIGA
//const int NSI = 1;
//const int NSV = 2;

const float taxa_aprendizado = float(0.05);

//const float Euler = 2; //exp(1); //2.178;

const int max_mundo_x = 80;  // dimensoes do mundo virtual
const int max_mundo_y = 45;
const int max_mundo_z = 5;

const int vazio     = 0;   // codificacao do mundo virtual
const int ninho     = 1;
const int parede    = 2;
const int terra     = 3;
const int agua      = 4;
const int predador  = 5;
const int margem    = 6;  // apenas para visualizacao
const int outra     = 7;
const int comida0   = 10;
const int comida1   = 15;
const int comida2   = 19;

// simulacao
const int nro_formigas_max = 100;

// geral
const int sem_presenca = 0;
//const int com_presenca = 10;

// visao
const int longe				= 1;
const int perto				= 2;
const int muito_perto		= 3;
const int muito_perto_baixo = 4;
const int muito_perto_alto	= 5;

//coordenadas
//const int no_ninho = 99;

//estomago
const int cheio = 99;

// especificacao do numero de saidas do ninho
const int nro_saidas_ninho = 4; // aqui nao da para mexer sem mudar a estrutura em baixo

// visualizacao da formiga
const char formiga00  = char(24);  // para cima
const char formiga45  = '/';
const char formiga90  = char(27);  // para a direita
const char formiga135 = '\\';
const char formiga180 = char(26);  // para baixo
const char formiga225 = '/';
const char formiga270 = char(28);  // para a esquera;
const char formiga315 = '\\';

// escolha de visualizacoes
const int v_alomonio  = 1;
const int v_feromonio = 2;
const int v_ninho     = 3;
const int v_alimento  = 4;
const int v_barulho   = 5;
const int v_normal    = 6;

// sem formigas
const int sem_formiga = 99;

// olhos de luz
const int intenso = 2;
const int medio   = 1;
const int fraco   = 0;

// para comparacoes de cheiros no historico
const int c_alomonio	= 0;
const int c_feromonio	= 1;
const int c_ninho		= 2;
const int c_alimento	= 3;

// estados de vida da formiga
const int viva     = 0;
const int esmagada = 1;
const int comida   = 2;
const int fome     = 3;
const int afogada = 4;

/////////////////////////////////////////////////////////////////////////
// 3. TIPOS                                                            //
/////////////////////////////////////////////////////////////////////////

typedef struct _entrada_tipo {
	float valor;
} ENTRADA_TIPO;

typedef struct _sinapse_tipo {
	float valor;
} SINAPSE_TIPO;

typedef struct _neuronio_tipo {
	float valor, ativacao[2];
} NEURONIO_TIPO;

typedef struct _saida_tipo {
	float valor, ativacao[2], esperado;
} SAIDA_TIPO;

typedef struct _rede_recorrente_tipo {
	ENTRADA_TIPO entrada[nro_entradas];
	SINAPSE_TIPO sinapse_EN[nro_entradas][nro_neuronios_max];
	NEURONIO_TIPO neuronio[nro_neuronios_max];
	SINAPSE_TIPO sinapse_NS[nro_neuronios_max][nro_saidas];
	SAIDA_TIPO saida[nro_saidas];
} REDE_RECORRENTE_TIPO;

typedef struct _item_menu { // parte da estrutura de menu
	char *descricao, *codigo;
} ITEM_MENU;

typedef struct _mundo_virtual_conjunto {
	short int paisagem, cheiro_alomonio, cheiro_alimento, cheiro_ninho, barulho;
	int cheiro_feromonio;
} MUNDO_VIRTUAL_CONJUNTO; 

typedef struct _mundo_virtual {
	MUNDO_VIRTUAL_CONJUNTO elemento[max_mundo_x][max_mundo_y][max_mundo_z];
	bool predador[2];
} MUNDO_VIRTUAL;

typedef struct _formiga_tipo {
	short int coord_x, coord_y, coord_z;
	short int visao[8][5];
	short int olfato_alomonio[5], olfato_ninho[5], olfato_alimento[5], olfato_feromonio[5];
	short int tato, audicao, estomago, estomago_social;
	int direcao;
	bool alerta;
	int descanso;
	bool no_ninho;
	short int vida;
	short int decisao;
	short int demora_subida;
} FORMIGA_TIPO;

typedef struct _saida_do_ninho_tipo {
	short int x, y, z;
} SAIDA_DO_NINHO_TIPO;

typedef struct _debug_doze_ifs {
	short int caso, linha;
} DEBUG_DOZE_IFS;

typedef struct _parametros_tipo {
	bool modo_neural;
	short int numero_formigas;
	short int neuronios_escondidos;
	bool trace_default;
	long int limite_simulacao;
	short int degradacao_feromonio;
	short int degradacao_alomonio;
	short int alcance_feromonio;
	short int alcance_alomonio;
	short int alcance_cheiro_ninho;
	short int alcance_cheiro_alimento;
	int iteracoes_delay;
	bool atualizacao_cheiro_alimentos;
	bool aprendizado_nao_supervisionado;
	short int gasto_energia;
	short int visual;
} PARAMETROS_TIPO;

typedef struct _sapato_tipo {
	short int limite_baixo_x, limite_alto_x, limite_baixo_y, limite_alto_y;
} SAPATO_TIPO;


/////////////////////////////////////////////////////////////////////////
// 4. VARIAVEIS                                                        //
/////////////////////////////////////////////////////////////////////////

ITEM_MENU menu[max_colunas_menu][max_linhas_menu]; // estrutura de menu e adicionais
int max_comp_menu[max_colunas_menu];
int cabecalho_pos[max_colunas_menu];

int i, x_cursor, y_cursor;  // para o gerenciamento da execucao do menu
char *codigo_extraido, opcao;

int elementos; // elementos na janela do dump

REDE_RECORRENTE_TIPO rede;

MUNDO_VIRTUAL mundo;

FORMIGA_TIPO formiga[nro_formigas_max];

SAIDA_DO_NINHO_TIPO saida_ninho[nro_saidas_ninho];

long int iteracao_mestre;

PARAMETROS_TIPO parametros;

int max_feromonio, min_feromonio, passo_feromonio;

SAPATO_TIPO sapato, passaro; // o passaro nao eh um sapato, mas o esquema de limites eh o mesmo

bool passaro_presente;

/////////////////////////////////////////////////////////////////////////
// 5. PROTOTIPOS                                                       //
/////////////////////////////////////////////////////////////////////////

////////// ROTINAS DE TESTE /////////

void teclas(void);

////////// ROTINAS IO SIMPLIFICADO /////////

void cls(void);						    // deleta conteudo da tela
void cls_janela(int,int);			    // deleta uma faixa da tela (de y1 a y2)
void janelao(void);						// janela grande
void limpa_janelao(void);				// restaura a cor de fundo em cima do janelao
void limpa_dropdown(void);				// limpa os menus dropdown
void limpa_caixa(void);					// limpa para caixa de mensagem
void cor_menu(void);				    // seta a cor padrao do cabecalho
void cor_menu_selecionado(void);	    // seta a cor de "item selecionado" do cabecalho
void cor_menu2(void);				    // seta a cor padrao do dropdown
void cor_menu_selecionado2(void);	    // seta a cor de "item selecionado" do dropdown
void cor_fundo(void);				    // seta a cor de fundo da tela
void home(void);					    // vai para x=0 y=0 (como em BASIC)
void center(int,char*);					// centraliza o texto em uma linha especifica
void janela(int,int,int,int);           // visualizacao de uma janela na tela
int caixa_mensagem(int,int,int,char*);  // visualizacao de uma caixa de mensagem
void espera(void);						// espera ate qualquer tecla e pressionada
void restore(void);						// restaura a tela principal
void escreve_numero(int);				// escreve um numero com dois digitos

////////// ROTINAS PARA IDENTIFICACAO /////////

void subtitulo(void);					// imprime o rodape com o titulo

////////// ROTINAS REFERENTES AO MENU /////////

void zera_menu(void);					// zera o conteudo da estrutura menu
void ajuste_parametros_menu(void);		// ajusta os parametros de controle
void init_menu_principal(void);			// inicializacao do menu principal
void init_menu_parametros(void);
void atualiza_cabecalho(void);			// atualiza o cabecalho do menu
void dropdown(void);					// mostra a parte dropdown
void executa_menu(void);				// gerenciador do menu dropdown

////////// ROTINAS REFERENTES AO ARQUIVO /////////

void arquivo_gravar_pesos(void);
void arquivo_carregar_pesos(void);
void arquivo_sair_do_programa(void);

////////// ROTINAS REFERENTES A REDE /////////

float sigmoide(float);					// Funcao de propagacao da rede
void propagacao_escondidos(int);
void propagacao_saidas(int);
void aprendizado_generec(void);
void rede_inicializar(int);
void rede_treinar(void);
void rede_testar_propagacao(void);
void rede_testar_aprendizado(void);
void prepara_dump(char*);
void rede_dump(void);

////////// ROTINAS REFERENTES AO ALGORITMO SIMBOLICO /////////

/// apoio
void muda_direcao_aleatoriamente(int);
void solta_alomonio(int);
void solta_feromonio(int);
void coma(int);
void aumenta_passo(int);
void diminui_passo(int);
bool sem_atracao(int);
void inverte_direcao(int);
bool caminho_livre(int);
bool positivo_e_crescente(int,int);
bool positivo_e_decrescente(int,int);

/// gerenciamento de sensores
int objeto_frente(int);
int objeto_esquerda(int);
int objeto_direita(int);
int objeto_acima(int);
int objeto_abaixo(int);

/// gerenciamento de acao
void fugir_perigo(int,bool);
void voltar_normalidade(int);
void evitar_colisao(int);
void pegar_comida(int);
void sair_alerta(int);
void sair_caminho_ruim(int);
void entrar_ninho(int);
void seguir_atracao_frente(int);
void buscar_sinal(int);
void seguir_atracao_ambigua(int);
void seguir_atracao_direita(int);
void seguir_atracao_esquerda(int);
void gasta_energia(int);

// controladora simbolica
DEBUG_DOZE_IFS doze_ifs(int,bool);

////////// ROTINAS REFERENTES A SIMULACAO /////////

int devolve_formiga_z(int,int);
void comida_por_passaro(int);
void visualiza_passaro(void);
void define_passaro(void);
void pezao(void);
void limpa_predador(void);
void decide_morte(int);
void simulacao_inicializar(void);
void limpa_teclas(void);
void devolve_cor_decisao(int);
void acha_min_max_feromonio(void);
void time_delay(int);
void mascara_parametros(void);
void parametros_padrao(void);
void mostra_parametros(void);
void degradar_feromonio(void);
void degradar_alomonio(void);
void imprima_formiga(int);
void boom(bool);
bool posica_disponivel(int,int);
void atualiza_formigas(bool);
void preenche_linha_x(int,int,int,int,int);
void preenche_linha_y(int,int,int,int,int);
void atualiza_cheiro_alimento(int,int,int);
void atualiza_cheiro_alimentos(void);
void inicializa_cheiro_ninho(void);
void estabelece_visao(int);
void estabelece_tato(int);
void estabelece_olfato(int);
void estabelece_audicao(int);
void simulacao_inicializar_mundo(void);
void visualiza_ponto(int,int,int,int,int,int);
void mostra_formigas(void);
void simulacao_mostrar_mundo(int);
void simulacao_debug(void);
void init_formigas(void);
int ajusta_altura(int);
bool out_of_bounds(int,int,int);
int devolve_x_mais(int);
int devolve_y_mais(int);
void ir_para_frente(int,bool);
void ir_para_tras(int,bool);
void vira_sentido_horario(int);
void vira_sentido_contra_horario(int);
bool saia_do_ninho(int);
void obtenha_dados_sensores(int);
void ajusta_cor(int);
void mostra_sentidos(int);
void simulacao_testar_sensores(int);
void simulacao_parametros(void);
void simulacao_executar(void);

////////// ROTINAS REFERENTES A AJUDA /////////

void ajuda_referencia_rapida(void);
void ajuda_sobre(void);

////////// ROTINAS APENAS DESENVOLVIMENTO /////

void random_test(void);
void ascii(void);

/////////////////////////////////////////////////////////////////////////
// 6. FUNCOES                                                          //
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  teclas                                        //
//                                                                     //
// CARACTERISTICAS    :  para descobrir os codigos de "teclas          //
// especiais" como as setas, enter                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  nao sera usado na versao final, apenas teste  //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void teclas(void)
{
	char tecla_teste;

	tecla_teste=' ';

	printf("Teste de teclas - sair com [x]\n\n");

	while(tecla_teste!='x')
	{
		tecla_teste=getch();
		printf("Tecla: %c Ord: %i\n",tecla_teste,abs(tecla_teste));
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  time_delay                                    //
//                                                                     //
// CARACTERISTICAS    :  implementacao de delay em segundos            //
//                                                                     //
// PARAMETROS         :  segundos de delay                             //
// OBSERVACOES        :  (lamentavel ter que criar isso nao mao....)   //
//                                                                     //
// ULTIMA ATUALIZACAO :  17/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void time_delay(int t)
{
	long initial, final;
	long ltime;

	initial=time(&ltime);
	final=initial+t;

	while(time(&ltime) < final);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  cls                                           //
//                                                                     //
// CARACTERISTICAS    :  deleta a tela com as cores padrao de fundo    //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  nome do comando em analogia a BASIC           //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void cls()
{
	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  cls_janela                                    //
//                                                                     //
// CARACTERISTICAS    :  deleta uma faixa da janela de y_inicio a      //
// y_fim                                                               //
// PARAMETROS         :  linha de inicio, linha de fim                 //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void cls_janela(int y_inicio,int y_fim)
{
	int i_janela, j_janela;

	cor_fundo();

	for(i_janela=y_inicio;i_janela<y_fim;i_janela++)
	{
		gotoxy(0,i_janela);
	
		for(j_janela=0;j_janela<max_x_tela;j_janela++)
			printf(" ");
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  janelao                                       //
//                                                                     //
// CARACTERISTICAS    :  cria uma janela grande no centro da tela      //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void janelao()
{
	limpa_dropdown();

	cor_menu2();

	janela(0,2,78,19);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  limpa_caixa_padrao                            //
//                                                                     //
// CARACTERISTICAS    :  limpa na tela para a caixa de posicao e       //
// tamanho padrao                                                      //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  27/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void limpa_caixa()
{
	cls_janela(caixa_padrao,caixa_padrao+3);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  limpa_janelao                                 //
//                                                                     //
// CARACTERISTICAS    :  limpa o conteudo do janelao e restaura a      //
// cor de fundo                                                        //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void limpa_janelao()
{
	cls_janela(2,22);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  limpa_dropdown                                //
//                                                                     //
// CARACTERISTICAS    :  tira os menus de dropdown da tela             //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void limpa_dropdown()
{
	cls_janela(1,max_linhas_menu+1);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  cor_menu                                      //
//                                                                     //
// CARACTERISTICAS    :  seta o esquema de cor do cabecalho do menu    //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void cor_menu()
{
	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  cor_menu_selecionado                          //
//                                                                     //
// CARACTERISTICAS    :  seta o esquema de cores de "item selecionado" //
// do cabecalho de menu                                                //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void cor_menu_selecionado()
{
	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTPURPLE );
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  cor_menu2                                     //
//                                                                     //
// CARACTERISTICAS    :  seta o esquema de cor padrao do menu dropdown //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void cor_menu2()
{
	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  cor_menu_selecionado2                         //
//                                                                     //
// CARACTERISTICAS    :  "item selecionado" do menu dropdown           //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void cor_menu_selecionado2()
{
	setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  cor_fundo                                     //
//                                                                     //
// CARACTERISTICAS    :  seta o esquema de cor de fundo                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void cor_fundo()
{
	setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLUE );
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  home                                          //
//                                                                     //
// CARACTERISTICAS    :  cursor para posicao 0,0                       //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void home()
{
	gotoxy(0,0);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  center                                        //
//                                                                     //
// CARACTERISTICAS    :  centraliza texto em uma linha                 //
//                                                                     //
// PARAMETROS         :  linha e o texto em si                         //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void center(int linha, char* texto)
{
	int comprimento, posicao;

	comprimento=int(strlen(texto));

	posicao=(max_x_tela - comprimento) / 2;

	gotoxy(posicao,linha);

	printf(texto);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  subtitulo                                     //
//                        
//
// CARACTERISTICAS    :  coloca o titulo do projeto em uma barra no    //
// fundo da tela                                                       //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void subtitulo()
{
	cor_menu();
	gotoxy(0,55);
	printf(" Sistema BIOANT (Versao %s) -- Copyright (c) 2003 Marvin Oliver Schneider     ",versao);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  janela                                        //
//                                                                     //
// CARACTERISTICAS    :  mostra uma janela na tela                     //
//                                                                     //
// PARAMETROS         :  canto esquerdo superior e extensao para a     //
// direita e para baixo                                                //
// OBSERVACOES        :  essa funcao e muito mais complicada do que    //
// precisava ser, pois foi adaptada de um dialogo com linhas em volta  //
// dele; a funcao nao foi modificada para facilitar uma eventual       //
// reinclusao desta caracteristica                                     //
// ULTIMA ATUALIZACAO :  22/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void janela(int canto_x, int canto_y, int ext_x, int ext_y)
{
	int i, j;
	gotoxy(canto_x,canto_y);

	printf(" ");
	for(i=0;i<ext_x;i++) printf(" ");
	printf(" ");

	for(i=0;i<ext_y;i++)
	{
		gotoxy(canto_x,canto_y+i+1);
		printf(" ");
		for(j=0;j<ext_x;j++)
			printf(" ");
		printf(" ");
	}

	gotoxy(canto_x,canto_y+ext_y+1);
	printf(" ");
	for(i=0;i<ext_x;i++) printf(" ");
	printf(" ");

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  caixa_mensagem                                //
//                                                                     //
// CARACTERISTICAS    :  mostra uma caixa de mensagem na tela; funcao  //
// multiuso para varios tipos de caixa                                 //
// PARAMETROS         :  tipo da caixa sendo:                          //
//                           1. alerta     ou                          //
//                           2. aviso      ou                          //
//                           3. chave      ou                          //
//                           4. pergunta   ou                          //
//                           5. simples                                //
//                       coordenada x (0 = centralizado)               //
//                       coordenada y do canto superior                //
//                       dialogo => texto a ser mostrado               //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/08/03                                      //
/////////////////////////////////////////////////////////////////////////

BOOL caixa_mensagem(int tipo, int x, int y, char* dialogo)
{
	int comprimento_topo, comprimento_dialogo, comprimento_x, comprimento_y;

	char ch;

	int confirmacao;

	char* topo;

	topo=NULL;

	confirmacao=false;

	switch(tipo)
	{
	case alerta:   topo=" ALERTA "; 
		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTRED );
		break;
	case pergunta: topo=" PERGUNTA "; 
		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTCYAN );
		break;
	case aviso:    topo=" AVISO "; 
		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );
		break;
	case simples: topo=NULL;
		setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );
		break;
	case chave: topo=NULL;
		setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );
		break;
	}

	comprimento_dialogo=int(strlen(dialogo));

	if(topo!=NULL)
		comprimento_topo=int(strlen(topo)); else comprimento_topo=0;

	comprimento_y=1;

	if(tipo==simples || tipo==chave) comprimento_y=-1;

	if(comprimento_topo>comprimento_dialogo) 
		comprimento_x=comprimento_topo; else comprimento_x=comprimento_dialogo;

	if(x==0) x=(80-comprimento_x-2)/2; // se x=0 entao assume que o dialogo deve ser posto no meio da tela

	janela(x,y,comprimento_x,comprimento_y);

	gotoxy(x+1+(comprimento_x-comprimento_dialogo)/2,y+1);

	if(tipo==simples || tipo==chave)
		gotoxy(x+1+(comprimento_x-comprimento_dialogo)/2,y);

	printf("%s",dialogo);

	gotoxy(x+1+(comprimento_x-comprimento_topo)/2,y);

	if(topo!=NULL)
		printf("%s",topo);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

	if(tipo==alerta || tipo==aviso)
	{
		gotoxy(x+1+(comprimento_x-4)/2,y+2);
		printf(" OK ");
		gotoxy(x+2+(comprimento_x-4)/2,y+2);
		getch();
	}

	if(tipo==pergunta)
	{
		gotoxy(x+1+(comprimento_x-5)/2,y+2);
		printf(" S/N ");
		gotoxy(x+3+(comprimento_x-5)/2,y+2);
		ch=' ';
		while(ch!='N' && ch!='S' && ch!='J' && ch!='Y')
		{
			ch=getch();
			ch=toupper(ch);
		}
		if(ch=='S') confirmacao=true;
	} else
		confirmacao=true;	// por convencao as outras caixas devolvem "verdadeiro" !!!

	if(tipo==simples || tipo==chave)
	{
		gotoxy(x,y);
	}

	cor_fundo();

	return confirmacao;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  espera                                        //
//                                                                     //
// CARACTERISTICAS    :  "press any key to continue"                   //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void espera()
{
	caixa_mensagem(simples,0,22,"Pressione qualquer tecla para continuar.");

	getch();

	cls_janela(22,23);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  restore                                       //
//                                                                     //
// CARACTERISTICAS    :  restaura a tela principal                     //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  28/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void restore()
{
	x_cursor=0;
	y_cursor=0;

	cls();
	init_menu_principal();
	atualiza_cabecalho();
	ajuda_sobre();
	
	home();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  escreve_numero                                //
//                                                                     //
// CARACTERISTICAS    :  escreve um numero com dois digitos            //
//                                                                     //
// PARAMETROS         :  numero a ser escrito na tela                  //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  28/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void escreve_numero(int numero)
{
	if(numero<10) printf("0%i",numero); else printf("%i",numero); 
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  zera_menu                                     //
//                                                                     //
// CARACTERISTICAS    :  zera o conteudo da estrutura de menu          //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void zera_menu()
{
	int i_menu, j_menu;

	for(i_menu=0;i_menu<max_colunas_menu;i_menu++)
		for(j_menu=0;j_menu<max_linhas_menu;j_menu++)
		{
			menu[i_menu][j_menu].descricao=" ";
			if(j_menu==0) menu[i_menu][j_menu].codigo="??"; else
				menu[i_menu][j_menu].codigo=" ";
		}

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  ajuste_parametros_menu                        //
//                                                                     //
// CARACTERISTICAS    :  ajusta os parametros de controle necessarios  //
// apos a definicao do conteudo do menu                                //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void ajuste_parametros_menu()
{
	int i_menu, j_menu, posicao_atual;

	posicao_atual=0;

	for(i_menu=0;i_menu<max_colunas_menu;i_menu++)
	{
		max_comp_menu[i_menu]=0;

		cabecalho_pos[i_menu]=posicao_atual;
		posicao_atual=posicao_atual + int(strlen(menu[i_menu][0].descricao)) + 2;

		for(j_menu=1;j_menu<max_linhas_menu;j_menu++)
		
			if(int(strlen(menu[i_menu][j_menu].descricao))>max_comp_menu[i_menu])
				max_comp_menu[i_menu]=int(strlen(menu[i_menu][j_menu].descricao));
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  init_menu_principal                           //
//                                                                     //
// CARACTERISTICAS    :  inicializa e prepara o conteudo do menu       //
// dropdown principal                                                  //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/08/03, 15/10/03                            //
/////////////////////////////////////////////////////////////////////////

void init_menu_principal()
{
	zera_menu();

	menu[0][0].descricao="BIOANT";
	
	menu[0][1].descricao="Sair do Programa";
	menu[0][1].codigo="XX";

	menu[1][0].descricao="Rede";
	
	menu[1][1].descricao="Inicializar";
	menu[1][1].codigo="IN";
	menu[1][2].descricao="Treinar";
	menu[1][2].codigo="TR";
	menu[1][3].descricao="Testar Propagacao";
	menu[1][3].codigo="TP";
	menu[1][4].descricao="Testar Aprendizado";
	menu[1][4].codigo="TA";
	menu[1][5].descricao="Dump";
	menu[1][5].codigo="DP";

	menu[2][0].descricao="Simulacao";
	
	menu[2][1].descricao="Inicializar";
	menu[2][1].codigo="IA";
	menu[2][2].descricao="Parametros";
	menu[2][2].codigo="PA";
	menu[2][3].descricao="Executar";
	menu[2][3].codigo="EX";
	menu[2][4].descricao="Teste Parado";
	menu[2][4].codigo="DB";

	//menu[3][0].descricao="Ajuda";
	
	//menu[3][1].descricao="Referencia Rapida";
	//menu[3][1].codigo="RE";
	//menu[3][2].descricao="Sobre";
	//menu[3][2].codigo="SO";

	ajuste_parametros_menu();

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  init_menu_parametros                          //
//                                                                     //
// CARACTERISTICAS    :  inicializa e prepara o conteudo do menu       //
// dropdown de parametros                                              //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  15/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void init_menu_parametros()
{
	zera_menu();

	menu[0][0].descricao="BIOANT";
	
	menu[0][1].descricao="Voltar ao Menu Principal";
	menu[0][1].codigo="VL";

	menu[1][0].descricao="A"; // Modo de Operacao
	
	menu[1][1].descricao="Algoritmo Simbolico";
	menu[1][1].codigo="A1";
	menu[1][2].descricao="Rede Neural Artificial";
	menu[1][2].codigo="A2";
	
	menu[2][0].descricao="B"; // Numero de Formigas
	
	menu[2][1].descricao="1";
	menu[2][1].codigo="B1";
	menu[2][2].descricao="3";
	menu[2][2].codigo="B2";
	menu[2][3].descricao="10";
	menu[2][3].codigo="B3";
	menu[2][4].descricao="20";
	menu[2][4].codigo="B4";
    menu[2][5].descricao="30";
	menu[2][5].codigo="B5";
    menu[2][6].descricao="40";
	menu[2][6].codigo="B6";
    menu[2][7].descricao="50";
	menu[2][7].codigo="B7";
    menu[2][8].descricao="60";
	menu[2][8].codigo="B8";
    menu[2][9].descricao="70";
	menu[2][9].codigo="B9";
    menu[2][10].descricao="80";
	menu[2][10].codigo="BA";
    menu[2][11].descricao="90";
	menu[2][11].codigo="BB";
	menu[2][12].descricao="100";
	menu[2][12].codigo="BC";

	menu[3][0].descricao="C"; // Neuronios Escondidos
	
	menu[3][1].descricao="2";
	menu[3][1].codigo="C1";
	menu[3][2].descricao="10";
	menu[3][2].codigo="C2";
	menu[3][3].descricao="20";
	menu[3][3].codigo="C3";
	menu[3][4].descricao="30";
	menu[3][4].codigo="C4";
	menu[3][5].descricao="40";
	menu[3][5].codigo="C5";
	menu[3][6].descricao="50";
	menu[3][6].codigo="C6";
	menu[3][7].descricao="60";
	menu[3][7].codigo="C7";
	menu[3][8].descricao="70";
	menu[3][8].codigo="C8";
	menu[3][9].descricao="80";
	menu[3][9].codigo="C9";
	menu[3][10].descricao="90";
	menu[3][10].codigo="CA";
	menu[3][11].descricao="100";
	menu[3][11].codigo="CB";

	menu[4][0].descricao="D"; // Trace Default
	
	menu[4][1].descricao="Ligado";
	menu[4][1].codigo="D1";
	menu[4][2].descricao="Desligado";
	menu[4][2].codigo="D2";

	menu[5][0].descricao="E"; // Limite Simulacao
	
	menu[5][1].descricao="1000";
	menu[5][1].codigo="E1";
	menu[5][2].descricao="5000";
	menu[5][2].codigo="E2";
	menu[5][3].descricao="10000";
	menu[5][3].codigo="E3";
	menu[5][4].descricao="50000";
	menu[5][4].codigo="E4";
	menu[5][5].descricao="100000";
	menu[5][5].codigo="E5";
	menu[5][6].descricao="Sem Limite";
	menu[5][6].codigo="E6";

	menu[6][0].descricao="F"; // Degradacao Feromonio
	
	menu[6][1].descricao="Cada 5";
	menu[6][1].codigo="F1";
	menu[6][2].descricao="Cada 10";
	menu[6][2].codigo="F2";
	menu[6][3].descricao="Cada 20";
	menu[6][3].codigo="F3";
	menu[6][4].descricao="Cada 50";
	menu[6][4].codigo="F4";
	menu[6][5].descricao="Cada 100";
	menu[6][5].codigo="F5";
	menu[6][6].descricao="Sem Atualizacao";
	menu[6][6].codigo="F6";

	menu[7][0].descricao="G"; // Degradacao Alomonio
	
	menu[7][1].descricao="Cada 5";
	menu[7][1].codigo="G1";
	menu[7][2].descricao="Cada 10";
	menu[7][2].codigo="G2";
	menu[7][3].descricao="Cada 20";
	menu[7][3].codigo="G3";
	menu[7][4].descricao="Cada 50";
	menu[7][4].codigo="G4";
	menu[7][5].descricao="Cada 100";
	menu[7][5].codigo="G5";
	menu[7][6].descricao="Sem Atualizacao";
	menu[7][6].codigo="G6";

	menu[8][0].descricao="H"; // Alcance Feromonio
	
	menu[8][1].descricao="2";
	menu[8][1].codigo="H1";
	menu[8][2].descricao="4";
	menu[8][2].codigo="H2";
	menu[8][3].descricao="6";
	menu[8][3].codigo="H3";
	menu[8][4].descricao="8";
	menu[8][4].codigo="H4";
	menu[8][5].descricao="10";
	menu[8][5].codigo="H5";
	menu[8][6].descricao="12";
	menu[8][6].codigo="H6";
	menu[8][7].descricao="14";
	menu[8][7].codigo="H7";
	menu[8][8].descricao="16";
	menu[8][8].codigo="H8";
	menu[8][9].descricao="18";
	menu[8][9].codigo="H9";
	menu[8][10].descricao="20";
	menu[8][10].codigo="HA";
	menu[8][11].descricao="Nenhum";
	menu[8][11].codigo="HB";

	
	menu[9][0].descricao="I"; // Alcance Alomonio
	
	menu[9][1].descricao="2";
	menu[9][1].codigo="I1";
	menu[9][2].descricao="4";
	menu[9][2].codigo="I2";
	menu[9][3].descricao="6";
	menu[9][3].codigo="I3";
	menu[9][4].descricao="8";
	menu[9][4].codigo="I4";
	menu[9][5].descricao="10";
	menu[9][5].codigo="I5";
	menu[9][6].descricao="12";
	menu[9][6].codigo="I6";
	menu[9][7].descricao="14";
	menu[9][7].codigo="I7";
	menu[9][8].descricao="16";
	menu[9][8].codigo="I8";
	menu[9][9].descricao="18";
	menu[9][9].codigo="I9";
	menu[9][10].descricao="20";
	menu[9][10].codigo="IA";
	menu[9][11].descricao="Nenhum";
	menu[9][11].codigo="IB";

	menu[10][0].descricao="J"; // Alcance Cheiro Ninho
	
	menu[10][1].descricao="20";
	menu[10][1].codigo="J1";
	menu[10][2].descricao="25";
	menu[10][2].codigo="J2";
	menu[10][3].descricao="30";
	menu[10][3].codigo="J3";
	menu[10][4].descricao="35";
	menu[10][4].codigo="J4";
	menu[10][5].descricao="40";
	menu[10][5].codigo="J5";
	menu[10][6].descricao="45";
	menu[10][6].codigo="J6";
	menu[10][7].descricao="50";
	menu[10][7].codigo="J7";
	menu[10][8].descricao="55";
	menu[10][8].codigo="J8";
	menu[10][9].descricao="60";
	menu[10][9].codigo="J9";
	menu[10][10].descricao="Nenhum";
	menu[10][10].codigo="JA";

	menu[11][0].descricao="K"; // Alcance Cheiro Alimento
	
	menu[11][1].descricao="2";
	menu[11][1].codigo="K1";
	menu[11][2].descricao="4";
	menu[11][2].codigo="K2";
	menu[11][3].descricao="6";
	menu[11][3].codigo="K3";
	menu[11][4].descricao="8";
	menu[11][4].codigo="K4";
	menu[11][5].descricao="10";
	menu[11][5].codigo="K5";
	menu[11][6].descricao="12";
	menu[11][6].codigo="K6";
	menu[11][7].descricao="14";
	menu[11][7].codigo="K7";
	menu[11][8].descricao="16";
	menu[11][8].codigo="K8";
	menu[11][9].descricao="18";
	menu[11][9].codigo="K9";
	menu[11][10].descricao="20";
	menu[11][10].codigo="KA";
	menu[11][11].descricao="Nenhum";
	menu[11][11].codigo="KB";

	menu[12][0].descricao="L"; // Delay                  
	
	menu[12][1].descricao="0";
	menu[12][1].codigo="L1";
	menu[12][2].descricao="1";
	menu[12][2].codigo="L2";
	menu[12][3].descricao="2";
	menu[12][3].codigo="L3";
	
	menu[13][0].descricao="M"; // Atualizacao Cheiro Alimentos
	
	menu[13][1].descricao="Ligado";
	menu[13][1].codigo="M1";
	menu[13][2].descricao="Desligado";
	menu[13][2].codigo="M2";
	
	menu[14][0].descricao="N"; // Aprendizado Supervisionado   
	
	menu[14][1].descricao="Ligado";
	menu[14][1].codigo="N1";
	menu[14][2].descricao="Desligado";
	menu[14][2].codigo="N2";

	menu[15][0].descricao="O"; // Aprendizado Supervisionado   

	menu[15][1].descricao="Cada 5";
	menu[15][1].codigo="O1";
	menu[15][2].descricao="Cada 10";
	menu[15][2].codigo="O2";
	menu[15][3].descricao="Cada 20";
	menu[15][3].codigo="O3";
	menu[15][4].descricao="Cada 50";
	menu[15][4].codigo="O4";
	menu[15][5].descricao="Cada 100";
	menu[15][5].codigo="O5";
	menu[15][6].descricao="Sem Atualizacao";
	menu[15][6].codigo="O6";

	ajuste_parametros_menu();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  atualiza_cabecalho                            //
//                                                                     //
// CARACTERISTICAS    :  mostra o cabecalho em funcao dos paramentros  //
// atuais                                                              //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void atualiza_cabecalho()
{
	cor_menu();

	home();

	for(i=0;i<max_x_tela;i++)
		printf(" ");

	for(i=0;i<max_colunas_menu;i++)
	{
		if(x_cursor==i)
			cor_menu_selecionado(); else
			cor_menu();

		gotoxy(cabecalho_pos[i],0);
		printf(" %s ",menu[i][0].descricao);
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  dropdown                                      //
//                                                                     //
// CARACTERISTICAS    :  mostra a janela dropdown atual                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void dropdown()
{
	int dropdown_i, dropdown_j, restante;

	for(dropdown_i=1;dropdown_i<max_linhas_menu;dropdown_i++)
	{
		if(menu[x_cursor][dropdown_i].descricao!=" ")
		{
			if(dropdown_i==y_cursor)
				cor_menu_selecionado2(); else
				cor_menu2();

			gotoxy(cabecalho_pos[x_cursor],dropdown_i);

			printf(" %s",menu[x_cursor][dropdown_i].descricao);

			restante=max_comp_menu[x_cursor]-int(strlen(menu[x_cursor][dropdown_i].descricao))+1;

			for(dropdown_j=0;dropdown_j<restante;dropdown_j++) printf(" ");
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  executa_menu                                  //
//                                                                     //
// CARACTERISTICAS    :  gerencia a execucao do menu dropdown          //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  21/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void executa_menu()
{
	codigo_extraido="";

	gotoxy(cabecalho_pos[x_cursor],y_cursor);

	opcao=getch();

	switch(opcao)
	{

	case char(72): if(y_cursor>0) 
				   {
					   if(y_cursor==1)
							cls_janela(1,max_linhas_menu);
					   y_cursor--; 
				   }
				   break;

	case char(75): if(x_cursor>0) 
				   {
					   x_cursor--;
					   atualiza_cabecalho();
					   if(y_cursor!=0) 
					   {
						   y_cursor=1;
						   cls_janela(1,max_linhas_menu);
					   }
				   }
				   break;

	case char(80): if(y_cursor<max_linhas_menu-1)
					   if(menu[x_cursor][y_cursor+1].descricao!=" ")
					  	   y_cursor++; 
				   break;

	case char(77): if(x_cursor<max_colunas_menu-1)
					   if(menu[x_cursor+1][1].descricao!=" ")
					   {
						   x_cursor++;
						   atualiza_cabecalho();
						   if(y_cursor!=0)
						   {
							   y_cursor=1;
							   cls_janela(1,max_linhas_menu);
						   }
					   }
					   break;

	case char(13): if(y_cursor==0) y_cursor=1; else 
				   	   codigo_extraido=menu[x_cursor][y_cursor].codigo; 
				    break;
	case char(27): if(y_cursor>0)
				   { 
					   y_cursor=0; 
					   cls_janela(1,max_linhas_menu); 
				   } else
					   codigo_extraido="ZZ"; break;
	}
	if(y_cursor>0) dropdown();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  arquivo_gravar_pesos                          //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void arquivo_gravar_pesos()
{
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  arquivo_carregar_pesos                        //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void arquivo_carregar_pesos()
{
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  arquivo_sair_do_programa                      //
//                                                                     //
// CARACTERISTICAS    :  pergunta por seguranca se o programa deve ser //
// terminado e preenche a variavel com dummy se aplicavel              //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void arquivo_sair_do_programa()
{
	if(caixa_mensagem(pergunta,0,caixa_padrao,"Realmente Sair?")==false)
	{
		codigo_extraido="??";	// nao confirmou, entao nao deixe sair!
		limpa_caixa();
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  sigmoide                                      //
//                                                                     //
// CARACTERISTICAS    :  funcao de ativacao da rede                    //
//                                                                     //
// PARAMETROS         :  entrada do neuronio                           //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  27/08/03                                      //
/////////////////////////////////////////////////////////////////////////

float sigmoide(float valor)
{
	valor=float(1.0/(1.0+exp(-valor))); // sigmoide:=1.0/(1.0+EXP(-soma));

	return valor;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  rede_inicializar                              //
//                                                                     //
// CARACTERISTICAS    :  silencioso = perguntar ou nao se a rede deve  //
// ser inicializada; no startup do sistema nao havera pergunta!        //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void rede_inicializar(int silencioso)
{
	int inicializar_rede, init_i, init_j;

	if(silencioso==false)
	{
		if(caixa_mensagem(pergunta,0,caixa_padrao,"Inicializar a rede (os pesos atuais serao perdidos)?"))
			inicializar_rede=true; else
			inicializar_rede=false;

		limpa_caixa();

	} else
		inicializar_rede=true;

	if(inicializar_rede)
	{

		// inicializacao da rede significa basicamente inicializacao das sinapses
		// o que e feito a seguir com valores entre -0.1 e 0.1

		// inicializacao das sinapses entre entradas e camada escondida

		for(init_i=0;init_i<nro_entradas;init_i++)
			for(init_j=0;init_j<parametros.neuronios_escondidos;init_j++)
				rede.sinapse_EN[init_i][init_j].valor=float(rand()/(RAND_MAX*5.0)-0.1);

		// inicializacao das sinapses entre a camada escondida e as saidas ("IDA")

		for(init_i=0;init_i<parametros.neuronios_escondidos;init_i++)
			for(init_j=0;init_j<nro_saidas;init_j++)
				rede.sinapse_NS[init_i][init_j].valor=float(rand()/(RAND_MAX*5.0)-0.1);

		// o resto das inicializacoes e feito apenas por seguranca, pois estes valores
		// devem ser definidos dinamicamente num evento posterior

		// inicializacao das entradas

		for(init_i=0;init_i<nro_entradas;init_i++)
		{
			rede.entrada[init_i].valor=0.0;
		}

		// inicializacao dos neuronios na camada escondida

		for(init_i=0;init_i<parametros.neuronios_escondidos;init_i++)
		{
			rede.neuronio[init_i].valor=0.0;
			rede.neuronio[init_i].ativacao[mais]=desligado;
			rede.neuronio[init_i].ativacao[menos]=desligado;
		}

		// inicializacao das saidas

		for(init_i=0;init_i<nro_saidas;init_i++)
		{
			rede.saida[init_i].valor=0.0;
			rede.saida[init_i].ativacao[mais]=0.0;
			rede.saida[init_i].ativacao[menos]=0.0;
			rede.saida[init_i].esperado=0.0;
		}

		if(silencioso==false)
		{
			caixa_mensagem(aviso,0,caixa_padrao,"Rede inicializada com sucesso.");

			limpa_caixa();
		}
		
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  propagacao_escondidos_v00                     //
//                                                                     //
// CARACTERISTICAS    :  faz a propagacao na camada escondida conforme //
// algoritmo Almeida-Pineda em redes recorrentes                       //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  04/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void propagacao_escondidos(int fase)
{
	int i, j, k;

	for(j=0;j<parametros.neuronios_escondidos;j++)
	{
		rede.neuronio[j].valor=0.0; // inicializacao

		for(i=0;i<nro_entradas;i++)  // soma com as entradas e pesos
		{
			rede.neuronio[j].valor=
				float(rede.neuronio[j].valor+
				(rede.entrada[i].valor *
				rede.sinapse_EN[i][j].valor *
				1.0));
		}

		for(k=0;k<nro_saidas;k++)  // soma com as ativacoes das saidas
		{
			rede.neuronio[j].valor=
				float(rede.neuronio[j].valor+
				(rede.sinapse_NS[j][k].valor *
				rede.saida[k].ativacao[fase] *
				1.0));
		}

		rede.neuronio[j].ativacao[fase]=sigmoide(rede.neuronio[j].valor);   // estabelecer ativacao
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  propagacao_saidas_v00                         //
//                                                                     //
// CARACTERISTICAS    :  cuida da propagacao nas saidas conforme o     //
// algoritmo de Almeida-Pineda para redes recorrentes                  //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  04/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void propagacao_saidas(int fase)
{
	int j, k;

	if(fase==mais)
		for(k=0;k<nro_saidas;k++)
			rede.saida[k].ativacao[mais]=rede.saida[k].esperado;  // na fase mais sao os valores os esperados!!

	if(fase==menos)
		for(k=0;k<nro_saidas;k++)
		{
			rede.saida[k].valor=0.0;   // inicializa a soma

			for(j=0;j<parametros.neuronios_escondidos;j++)  // soma com as ativacoes dos escondidos
				rede.saida[k].valor=
				float(rede.saida[k].valor +
				(rede.sinapse_NS[j][k].valor *
				rede.neuronio[j].ativacao[menos] *   // *** CHECAR ESTE PONTO 
				1.0));

			rede.saida[k].ativacao[menos]=sigmoide(rede.saida[k].valor); // ativacao
			
		}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  rede_treinar                                  //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void rede_treinar()
{
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  rede_testar_propagacao                        //
//                                                                     //
// CARACTERISTICAS    :  oferece metodos para avaliar a propagacao da  //
// rede                                                                //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  27/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void rede_testar_propagacao()
{
	if(caixa_mensagem(pergunta,0,caixa_padrao,"Colocar valores DUMMY para teste?"))
	{
		rede.entrada[0].valor=1.0;
		rede.entrada[2].valor=1.0;	// rede com no minimo 3 entradas!!!

		rede.saida[0].esperado=1.0; // ... e no minimo 3 saidas
		rede.saida[2].esperado=1.0;
	}

	// propagacao conforme fluxo em rede recorrente

	// o sistema pergunta a cada etapa se um dump devera ser executado

	// a cada loop existe a possibilidade de interrupcao

	limpa_caixa();

	propagacao_saidas(mais);

	if(caixa_mensagem(pergunta,0,caixa_padrao,"Saidas (Fase +) OK! Dump?"))
		rede_dump(); else limpa_caixa();

	propagacao_escondidos(mais);

	if(caixa_mensagem(pergunta,0,caixa_padrao,"Escondidos (Fase +) OK! Dump?"))
		rede_dump(); else limpa_caixa();

	propagacao_saidas(menos);
	
	if(caixa_mensagem(pergunta,0,caixa_padrao,"Saidas (Fase -) OK! Dump?"))
		rede_dump(); else limpa_caixa();

	propagacao_escondidos(menos);

	if(caixa_mensagem(pergunta,0,caixa_padrao,"Escondidos (Fase -) OK! Dump?"))
		rede_dump(); else limpa_caixa();

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  aprendizado_generec                           //
//                                                                     //
// CARACTERISTICAS    :  rotina de aprendizado da rede (generec)       //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :                                                //
//                                                                     //
// ULTIMA ATUALIZACAO :  04/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void aprendizado_generec()    
{
	int i, j, k;

	float delta;

	for(i=0;i<nro_entradas;i++)		// Ajuste das sinapses entre entradas e camada escondida
	{
		for(j=0;j<parametros.neuronios_escondidos;j++)
		{
			delta=float(taxa_aprendizado*(rede.neuronio[j].ativacao[mais]-rede.neuronio[j].ativacao[menos])*rede.entrada[i].valor*1.0);

			rede.sinapse_EN[i][j].valor=rede.sinapse_EN[i][j].valor+delta;
		}
	}

	for(j=0;j<parametros.neuronios_escondidos;j++)    // Ajuste das sinapses entre a camada escondida e as saidas
	{
		for(k=0;k<nro_saidas;k++)
		{
			delta=taxa_aprendizado*(rede.saida[k].ativacao[mais]-rede.saida[k].ativacao[menos])*rede.neuronio[j].ativacao[menos];

			rede.sinapse_NS[j][k].valor=rede.sinapse_NS[j][k].valor+delta;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  rede_testar_aprendizado                       //
//                                                                     //
// CARACTERISTICAS    :  teste simples de aprendizado                  //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  04/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void rede_testar_aprendizado()
{
	int master;

	if(caixa_mensagem(pergunta,0,caixa_padrao,"Colocar valores DUMMY para teste?"))
	{
		rede.entrada[0].valor=1.0;
		rede.entrada[2].valor=1.0;	// rede com no minimo 3 entradas!!!

		rede.saida[0].esperado=1.0; // ... e no minimo 3 saidas
		rede.saida[2].esperado=1.0;
	}

	propagacao_saidas(mais);

	for(master=0;master<1000;master++)
	{
		propagacao_escondidos(menos);

		propagacao_saidas(menos);

		propagacao_escondidos(mais);

		aprendizado_generec();
	}

	rede_dump();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  prepara_dump                                  //
//                                                                     //
// CARACTERISTICAS    :  prepara uma nova pagina para mostrar dados    //
// dump                                                                //
// PARAMETROS         :  texto do cabecalho                            //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void prepara_dump(char* texto_dump)
{
	janelao();
	
	elementos=0;

	center(2,"DUMP DA REDE");

	center(3,texto_dump);

	gotoxy(0,5);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  rede_dump                                     //
//                                                                     //
// CARACTERISTICAS    :  executar o dump da rede                       //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void rede_dump()
{
	int dump_i, dump_j;

	// *** DUMP DAS ENTRADAS ***

	prepara_dump("ENTRADAS");

	for(dump_i=0;dump_i<nro_entradas;dump_i++)
	{
		elementos++;

		printf("[%i]=%f; ",dump_i,rede.entrada[dump_i].valor);

		if(elementos>quebra)
		{
			espera();
			prepara_dump("ENTRADAS");
		}
	}

	espera();

	// *** DUMP DA PRIMEIRA CAMADA DE SINAPSES ***

	prepara_dump("SINAPSES ENTRE ENTRADAS E CAMADA ESCONDIDA");

	for(dump_i=0;dump_i<nro_entradas;dump_i++)
	{
		for(dump_j=0;dump_j<parametros.neuronios_escondidos;dump_j++)
		{
			elementos++;

			printf("[%i,%i]=%f; ",dump_i,dump_j,rede.sinapse_EN[dump_i][dump_j].valor);

			if(elementos>quebra)
			{
				espera();
				prepara_dump("SINAPSES ENTRE ENTRADAS E CAMADA ESCONDIDA");
			}
		}
	}

	espera();

	// *** DUMP DA CAMADA DE NEURONIOS ESCONDIDOS ***

	prepara_dump("NEURONIOS ESCONDIDOS");

	for(dump_i=0;dump_i<parametros.neuronios_escondidos;dump_i++)
	{
		elementos++;

		printf("[%i] D=%f A+=%f A-=%f; ",
			dump_i,rede.neuronio[dump_i].valor,rede.neuronio[dump_i].ativacao[mais],rede.neuronio[dump_i].ativacao[menos]);

		if(elementos>(quebra/3))
		{
			espera();
			prepara_dump("NEURONIOS ESCONDIDOS");
		}
	}

	espera();

	// *** DUMP DA SEGUNDO CAMADA DE SINAPSES ***

	prepara_dump("SINAPSES ENTRE CAMADA ESCONDIDA E SAIDAS (IDA)");

	for(dump_i=0;dump_i<parametros.neuronios_escondidos;dump_i++)
	{
		for(dump_j=0;dump_j<nro_saidas;dump_j++)
		{
			elementos++; 

			printf("[%i,%i]=%f; ",dump_i,dump_j,rede.sinapse_NS[dump_i][dump_j].valor);

			if(elementos>quebra)
			{
				espera();
				prepara_dump("SINAPSES ENTRE CAMADA ESCONDIDA E SAIDAS (IDA)");
			}
		}
	}

	espera();

	// *** DUMP DAS SAIDAS ***

	prepara_dump("SAIDAS");

	for(dump_i=0;dump_i<nro_saidas;dump_i++)
	{
		elementos++;

		printf("[%i] D=%f A+=%f A-=%f; ",
			dump_i,rede.saida[dump_i].valor,rede.saida[dump_i].ativacao[mais],rede.saida[dump_i].ativacao[menos]);

		if(elementos>(quebra/3))
		{
			espera();
			prepara_dump("SAIDAS");
		}
	}

	espera();

	limpa_janelao();

	dropdown();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  positivo_e_crescente                          //
//                                                                     //
// CARACTERISTICAS    :  comparacao de cheiros lembrados               //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool positivo_e_crescente(int index_formiga, int valor)
{
	bool flag;

	flag=false;

	switch(valor)
	{
	case c_alomonio: if(formiga[index_formiga].olfato_alomonio[0]>0 &&
						 formiga[index_formiga].olfato_alomonio[0]>=formiga[index_formiga].olfato_alomonio[1])
						 flag=true; break;
	case c_alimento: if(formiga[index_formiga].olfato_alimento[0]>0 &&
						 formiga[index_formiga].olfato_alimento[0]>=formiga[index_formiga].olfato_alimento[1])
						 flag=true; break;
	case c_feromonio: if(formiga[index_formiga].olfato_feromonio[0]>0 &&
						 formiga[index_formiga].olfato_feromonio[0]>=formiga[index_formiga].olfato_feromonio[1])
						 flag=true; break;
	case c_ninho: if(formiga[index_formiga].olfato_ninho[0]>0 &&
						 formiga[index_formiga].olfato_ninho[0]>=formiga[index_formiga].olfato_ninho[1])
						 flag=true; break;
	}

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  positivo_e_decrescente                        //
//                                                                     //
// CARACTERISTICAS    :  comparacao de cheiros lembrados               //
// atual                                                               //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool positivo_e_decrescente(int index_formiga, int valor)
{
	bool flag;

	flag=false;

	switch(valor)
	{
	case c_alomonio: if(formiga[index_formiga].olfato_alomonio[0]>0 &&
						 formiga[index_formiga].olfato_alomonio[0]<formiga[index_formiga].olfato_alomonio[1])
						 flag=true; break;
	case c_alimento: if(formiga[index_formiga].olfato_alimento[0]>0 &&
						 formiga[index_formiga].olfato_alimento[0]<formiga[index_formiga].olfato_alimento[1])
						 flag=true; break;
	case c_feromonio: if(formiga[index_formiga].olfato_feromonio[0]>0 &&
						 formiga[index_formiga].olfato_feromonio[0]<formiga[index_formiga].olfato_feromonio[1])
						 flag=true; break;
	case c_ninho: if(formiga[index_formiga].olfato_ninho[0]>0 &&
						 formiga[index_formiga].olfato_ninho[0]<formiga[index_formiga].olfato_ninho[1])
						 flag=true; break;
	}

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  solta_alomonio                                //
//                                                                     //
// CARACTERISTICAS    :  solta o cheiro de alerta a partir da formiga  //
// atual                                                               //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  06/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void solta_alomonio(int index_formiga)
{
	int limite_x_menos, limite_x_mais, limite_y_menos, limite_y_mais, limite_z_mais, limite_z_menos;
	
	int x_atual, y_atual, z_atual, x_diff, y_diff, z_diff, cheiro_produzido, alcance;

	alcance=parametros.alcance_alomonio;

	// inicializacao

	limite_x_menos=formiga[index_formiga].coord_x-alcance;
	limite_x_mais=formiga[index_formiga].coord_x+alcance+1;
	
	limite_y_menos=formiga[index_formiga].coord_y-alcance;
	limite_y_mais=formiga[index_formiga].coord_y+alcance+1;

	limite_z_menos=formiga[index_formiga].coord_z-alcance;
	limite_z_mais=formiga[index_formiga].coord_z+alcance+1;

	// ajuste a margens se aplicavel

	if(limite_x_menos<0) limite_x_menos=0;
	if(limite_y_menos<0) limite_y_menos=0;
	if(limite_z_menos<0) limite_z_menos=0;

	if(limite_x_mais>max_mundo_x) limite_x_mais=max_mundo_x;
	if(limite_y_mais>max_mundo_y) limite_y_mais=max_mundo_y;
	if(limite_z_mais>max_mundo_z) limite_z_mais=max_mundo_z;

	for(x_atual=limite_x_menos;x_atual<limite_x_mais;x_atual++)
		for(y_atual=limite_y_menos;y_atual<limite_y_mais;y_atual++)
			for(z_atual=limite_z_menos;z_atual<limite_z_mais;z_atual++)
			{
				x_diff=abs(x_atual-formiga[index_formiga].coord_x);
				y_diff=abs(y_atual-formiga[index_formiga].coord_y);
				z_diff=abs(z_atual-formiga[index_formiga].coord_z);

				cheiro_produzido=
					99-x_diff-y_diff-z_diff;

				if(cheiro_produzido>mundo.elemento[x_atual][y_atual][z_atual].cheiro_alomonio &&
					cheiro_produzido>(99-alcance))
					mundo.elemento[x_atual][y_atual][z_atual].cheiro_alomonio=cheiro_produzido;
			}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  solta_feromonio                               //
//                                                                     //
// CARACTERISTICAS    :  solta o cheiro de recrutamento a partir da    //
// formiga atual                                                       //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  06/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void solta_feromonio(int index_formiga)
{
	int limite_x_menos, limite_x_mais, limite_y_menos, limite_y_mais, limite_z_mais, limite_z_menos;
	
	int x_atual, y_atual, z_atual, x_diff, y_diff, z_diff, cheiro_produzido, alcance;

	alcance=parametros.alcance_feromonio;

	// inicializacao

	limite_x_menos=formiga[index_formiga].coord_x-alcance;
	limite_x_mais=formiga[index_formiga].coord_x+alcance+1;
	
	limite_y_menos=formiga[index_formiga].coord_y-alcance;
	limite_y_mais=formiga[index_formiga].coord_y+alcance+1;

	limite_z_menos=formiga[index_formiga].coord_z-alcance;
	limite_z_mais=formiga[index_formiga].coord_z+alcance+1;

	// ajuste a margens se aplicavel

	if(limite_x_menos<0) limite_x_menos=0;
	if(limite_y_menos<0) limite_y_menos=0;
	if(limite_z_menos<0) limite_z_menos=0;

	if(limite_x_mais>max_mundo_x) limite_x_mais=max_mundo_x;
	if(limite_y_mais>max_mundo_y) limite_y_mais=max_mundo_y;
	if(limite_z_mais>max_mundo_z) limite_z_mais=max_mundo_z;

	for(x_atual=limite_x_menos;x_atual<limite_x_mais;x_atual++)
		for(y_atual=limite_y_menos;y_atual<limite_y_mais;y_atual++)
			for(z_atual=limite_z_menos;z_atual<limite_z_mais;z_atual++)
			{
				x_diff=abs(x_atual-formiga[index_formiga].coord_x);
				y_diff=abs(y_atual-formiga[index_formiga].coord_y);
				z_diff=abs(z_atual-formiga[index_formiga].coord_z);

				cheiro_produzido=
					99-x_diff-y_diff-z_diff;

				if(cheiro_produzido>mundo.elemento[x_atual][y_atual][z_atual].cheiro_feromonio &&
					cheiro_produzido>(99-alcance))
					mundo.elemento[x_atual][y_atual][z_atual].cheiro_feromonio=cheiro_produzido;
				
				//cheiro_produzido=
				//	alcance*6+1-x_diff-y_diff-z_diff;

				//if(cheiro_produzido>mundo.elemento[x_atual][y_atual][z_atual].cheiro_feromonio &&
				//	cheiro_produzido>(99-alcance))
				//	mundo.elemento[x_atual][y_atual][z_atual].cheiro_feromonio=
				//	mundo.elemento[x_atual][y_atual][z_atual].cheiro_feromonio+cheiro_produzido;
			}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  come                                          //
//                                                                     //
// CARACTERISTICAS    :  come se tiver comida a frente                 //
//                                                                     //
// PARAMETROS         :  formiga atual                                 //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  06/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void come(int index_formiga)
{
	int x_atual, y_atual, z_atual, x_mais, y_mais, x_destino, y_destino;

	x_atual=formiga[index_formiga].coord_x;
	y_atual=formiga[index_formiga].coord_y;
	z_atual=formiga[index_formiga].coord_z;

	x_mais=devolve_x_mais(formiga[index_formiga].direcao);
	y_mais=devolve_y_mais(formiga[index_formiga].direcao);

	x_destino=x_atual+x_mais;
	y_destino=y_atual+y_mais;

	if((mundo.elemento[x_destino][y_destino][z_atual].paisagem==agua ||
		(mundo.elemento[x_destino][y_destino][z_atual].paisagem>comida0 &&
		mundo.elemento[x_destino][y_destino][z_atual].paisagem<=comida2)) &&
		formiga[index_formiga].estomago_social<89)
	{
		formiga[index_formiga].estomago_social=formiga[index_formiga].estomago_social+11;

		if(mundo.elemento[x_destino][y_destino][z_atual].paisagem>comida0)
		{
			mundo.elemento[x_destino][y_destino][z_atual].paisagem--;
			if(mundo.elemento[x_destino][y_destino][z_atual].paisagem==comida0)
			{
				mundo.elemento[x_destino][y_destino][z_atual].paisagem=vazio;

				visualiza_ponto(x_destino,y_destino,z_atual,sem_formiga,99,99);
			}

			if(parametros.atualizacao_cheiro_alimentos) atualiza_cheiro_alimentos();
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  caminho_livre                                 //
//                                                                     //
// CARACTERISTICAS    :  retorna se o caminho na frente da formiga esta//
// livre ou nao                                                        //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool caminho_livre(int index_formiga)
{
	bool flag;

	int rnd;

	rnd=(rand()*11)/RAND_MAX;

	flag=true;

	obtenha_dados_sensores(index_formiga);

	if(formiga[index_formiga].tato==margem || // artificio para nao colar na margem
		formiga[index_formiga].tato==agua) // NAO SOBE NA AGUA!!!
		flag=false;

	if(formiga[index_formiga].tato==outra && rnd<6)
		flag=false;		// artificio para nao colar em outra formiga, mas NAO SEMPRE para evitar
						// travamentos permanentes quando so ha formigas em volta

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  sem_atracao                                   //
//                                                                     //
// CARACTERISTICAS    :  retorna se nao ha atracao a frente            //
// livre ou nao                                                        //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool sem_atracao(int index_formiga)
{
	bool flag;

	flag=false;

	obtenha_dados_sensores(index_formiga);

	if(formiga[index_formiga].visao[3][2]==sem_presenca) flag=true;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  aumenta_passo                                 //
//                                                                     //
// CARACTERISTICAS    :  coloca a formiga em alerta                    //
// um caminho livre                                                    //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  procedimento apenas para maior claridade      //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void aumenta_passo(int index_formiga)
{
	formiga[index_formiga].alerta=true;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  diminui_passo                                 //
//                                                                     //
// CARACTERISTICAS    :  coloca a formiga em modo normal               //
// um caminho livre                                                    //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  procedimento apenas para maior claridade      //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void diminui_passo(int index_formiga)
{
	formiga[index_formiga].alerta=false;
}


/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  muda_aleatoriamente_direcao                   //
//                                                                     //
// CARACTERISTICAS    :  muda aleatoriamente de direcao para dentro de //
// um caminho livre                                                    //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void muda_aleatoriamente_direcao(int index_formiga)
{
	int rnd, i, form_x, form_y, form_z;

	form_x=formiga[index_formiga].coord_x;
	form_y=formiga[index_formiga].coord_y;
	form_z=formiga[index_formiga].coord_z;

	if(mundo.elemento[form_x][form_y][form_z].paisagem!=predador)
	{

	do
	{
		rnd=(rand()*8)/RAND_MAX;

		//printf("      %i       ",rnd);

		//getch();

		if(rnd<=0) rnd=1;

	    for(i=0;i<rnd;i++)
			vira_sentido_horario(index_formiga);	

	}while(caminho_livre(index_formiga)==false);

	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inverte_direcao                               //
//                                                                     //
// CARACTERISTICAS    :  inverte a direcao da formiga usando os olhos  //
// de luz                                                              //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  esse procedimento eh um artificio que poderia //
// ser realizado muito mais facilmente, mas foi feito assim para       //
// conservar a plausibilidade biologica mesmo no processamento!!!      //
// ULTIMA ATUALIZACAO :  11/10/01                                      //
/////////////////////////////////////////////////////////////////////////

void inverte_direcao(int index_formiga)
{
	int situacao_inicial[2], situacao_desejada[2], i;

	situacao_inicial[0]=formiga[index_formiga].visao[4][4];
	situacao_inicial[1]=formiga[index_formiga].visao[6][4];

	for(i=0;i<2;i++)		// calcula o inverso dos olhos de luz
	{
		if(situacao_inicial[i]==fraco) situacao_desejada[i]=intenso; else
			if(situacao_inicial[i]==intenso) situacao_desejada[i]=fraco; else
				situacao_desejada[i]=situacao_inicial[i]; // medio
	}

	// estabelece a situacao final

	while(formiga[index_formiga].visao[4][4]!=situacao_desejada[0] ||
		formiga[index_formiga].visao[6][4]!=situacao_desejada[1])
	{
		vira_sentido_horario(index_formiga);
		obtenha_dados_sensores(index_formiga);
	}

	// CUIDADO: laco infinito se algo estiver errado aqui
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  objeto_frente                                 //
//                                                                     //
// CARACTERISTICAS    :  retorna o codigo referente ao objeto a frente //
//                                                                     //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/01                                      //
/////////////////////////////////////////////////////////////////////////

int objeto_frente(int index_formiga)
{
	int flag;

	flag=formiga[index_formiga].visao[3][2];

	if(flag==muito_perto)
		if(formiga[index_formiga].visao[3][0]==muito_perto)
			flag=muito_perto_alto; else
			flag=muito_perto_baixo;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  objeto_esquerda                               //
//                                                                     //
// CARACTERISTICAS    :  retorna o codigo referente ao objeto a        //
// esquerda                                                            //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/01                                      //
/////////////////////////////////////////////////////////////////////////

int objeto_esquerda(int index_formiga)
{
	int flag, i;

	flag=sem_presenca;

	for(i=0;i<3;i++)
	{
		if(formiga[index_formiga].visao[i][2]>flag)
			flag=formiga[index_formiga].visao[i][2];
	}

	if(flag==muito_perto)
	{
		for(i=0;i<3;i++)
		{
			if(formiga[index_formiga].visao[i][0]==muito_perto)
				flag=muito_perto_alto;
		}
	}

	if(flag==muito_perto) flag=muito_perto_baixo;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  objeto_direita                                //
//                                                                     //
// CARACTERISTICAS    :  retorna o codigo refernte ao objeto a         //
// direita                                                             //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/01                                      //
/////////////////////////////////////////////////////////////////////////

int objeto_direita(int index_formiga)
{
	int flag;

	flag=sem_presenca;

	for(i=5;i<8;i++)
	{
		if(formiga[index_formiga].visao[i][2]>flag)
			flag=formiga[index_formiga].visao[i][2];
	}

	if(flag==muito_perto)
	{
		for(i=5;i<8;i++)
		{
			if(formiga[index_formiga].visao[i][0]==muito_perto)
				flag=muito_perto_alto;
		}
	}

	if(flag==muito_perto) flag=muito_perto_baixo;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  objeto_acima                                  //
//                                                                     //
// CARACTERISTICAS    :  retorna o codigo referente ao objeto acima    //
//                                                                     //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/01                                      //
/////////////////////////////////////////////////////////////////////////

int objeto_acima(int index_formiga)
{
	int flag;

	flag=formiga[index_formiga].visao[0][4];

	//if(flag) printf(" ACIMA ");

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  objeto_abaixo                                 //
//                                                                     //
// CARACTERISTICAS    :  retorna o codigo referente ao objeto acima    //
//                                                                     //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/01                                      //
/////////////////////////////////////////////////////////////////////////

int objeto_abaixo(int index_formiga)
{
	int flag;

	flag=formiga[index_formiga].visao[0][3];

	//if(flag) printf(" ACIMA ");

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  fugir_perigo                                  //
//                                                                     //
// CARACTERISTICAS    :  muda aleatoriamente de direcao para dentro de //
// um caminho livre, solta alomonio e aumenta o passo                  //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/01                                      //
/////////////////////////////////////////////////////////////////////////

void fugir_perigo(int index_formiga, bool alo)
{
	muda_aleatoriamente_direcao(index_formiga);
	if(alo) solta_alomonio(index_formiga);
	aumenta_passo(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  voltar_normalidade                            //
//                                                                     //
// CARACTERISTICAS    :  diminui passo ao normal                       //
//                                                                     //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void voltar_normalidade(int index_formiga)
{
	diminui_passo(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  evitar_colisao                                //
//                                                                     //
// CARACTERISTICAS    :  saia da direcao da outra formiga (45o para    //
// direita ou esquerda aleatoriamente)                                 //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void evitar_colisao(int index_formiga)
{
	int rnd;

	rnd=(rand()*11)/RAND_MAX;

	if(rnd<6)
		vira_sentido_horario(index_formiga); else
		vira_sentido_contra_horario(index_formiga);

	if(caminho_livre(index_formiga)==false)		// seguranca caso o procedimento nao produza bons resultados
		muda_aleatoriamente_direcao(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  pegar_comida                                  //
//                                                                     //
// CARACTERISTICAS    :  coma, depois inverte a direcao (olhos de luz) //
// e solte feromonio ate chegar ao ninho                               //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/2003                                    //
/////////////////////////////////////////////////////////////////////////

void pegar_comida(int index_formiga)
{
	while((formiga[index_formiga].tato>comida0 || formiga[index_formiga].tato==agua) && 
		formiga[index_formiga].estomago_social<90)
	{
		come(index_formiga);
		obtenha_dados_sensores(index_formiga);
	}

	inverte_direcao(index_formiga);

	if(caminho_livre(index_formiga)==false)		// seguranca caso o procedimento nao produza bons resultados
		muda_aleatoriamente_direcao(index_formiga);

	// quando ha algo no estomago social a formiga automaticamente solta feromonio
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  sair_alerta                                   //
//                                                                     //
// CARACTERISTICAS    :  aumenta o passo e continua andando na mesma   //
// direcao                                                             //
// PARAMETROS         :  index_formiga                                 //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void sair_alerta(int index_formiga)
{
	aumenta_passo(index_formiga);

	if(caminho_livre(index_formiga)==false)		
		muda_aleatoriamente_direcao(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  sair_caminho_ruim                             //
//                                                                     //
// CARACTERISTICAS    :  muda aleatoriamente de direcao para dentro de //
// um caminho livre                                                    //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void sair_caminho_ruim(int index_formiga)
{
	muda_aleatoriamente_direcao(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  entrar_ninho                                  //
//                                                                     //
// CARACTERISTICAS    :  continua andando na mesma direcao entrando no //
// ninho - la come, descansa e saia novamente                          //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void entrar_ninho(int index_formiga, bool modo_mapa)
{
	int x_atual, y_atual, z_atual;

	//gotoxy(0,3);

	//printf("entrada de formiga %i requisitada",index_formiga);

	x_atual=formiga[index_formiga].coord_x;
	y_atual=formiga[index_formiga].coord_y;
	z_atual=formiga[index_formiga].coord_z;

	mundo.elemento[x_atual][y_atual][z_atual].paisagem=vazio;

	if(modo_mapa) visualiza_ponto(x_atual,y_atual,z_atual,sem_formiga,99,99);

	formiga[index_formiga].coord_x=35; // lugar "dummy" no ninho
	formiga[index_formiga].coord_y=25;
	formiga[index_formiga].coord_z=0;

	formiga[index_formiga].no_ninho=true;

	formiga[index_formiga].descanso=(rand()*parametros.numero_formigas*5)/RAND_MAX;

	// saida por procedimentos automaticos apos o descanso  
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  seguir_atracao_frente                         //
//                                                                     //
// CARACTERISTICAS    :  continua andando na mesma direcao             //
//                                                                     //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void seguir_atracao_frente(int index_formiga)
{
	// sem nada para ser feito ate o momento

	if(caminho_livre(index_formiga)==false)		
		muda_aleatoriamente_direcao(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  buscar_sinal                                  //
//                                                                     //
// CARACTERISTICAS    :  se movimenta aleatoriamente                   //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void buscar_sinal(int index_formiga)
{
	int rnd;

	rnd=(rand()*10)/RAND_MAX;

	if(rnd==0)
		muda_aleatoriamente_direcao(index_formiga); // mudar a direcao em 10% dos casos, senao passar reto

	if(caminho_livre(index_formiga)==false)		// seguranca caso o procedimento nao produza bons resultados
		muda_aleatoriamente_direcao(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  seguir_atracao_ambigua                        //
//                                                                     //
// CARACTERISTICAS    :  vira aleatoriamente a direita ou a esquerda   //
//                                                                     //
// PARAMETROS         :  index_formiga                                 //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void seguir_atracao_ambigua(int index_formiga)
{
	int rnd;

	rnd=(rand()*11)/RAND_MAX;

	if(rnd<6) seguir_atracao_direita(index_formiga); else
		seguir_atracao_esquerda(index_formiga);

	if(caminho_livre(index_formiga)==false)		// seguranca caso o procedimento nao produza bons resultados
		muda_aleatoriamente_direcao(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  seguir_atracao_direita                        //
//                                                                     //
// CARACTERISTICAS    :  vira a direita                                //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void seguir_atracao_direita(int index_formiga)
{
	vira_sentido_horario(index_formiga);
	if(sem_atracao(index_formiga)) vira_sentido_horario(index_formiga);
	if(sem_atracao(index_formiga)) vira_sentido_horario(index_formiga);

	if(caminho_livre(index_formiga)==false)		// seguranca caso o procedimento nao produza bons resultados
		muda_aleatoriamente_direcao(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  seguir_atracao_esquerda                       //
//                                                                     //
// CARACTERISTICAS    :  vira a esquerda                               //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void seguir_atracao_esquerda(int index_formiga)
{
	vira_sentido_contra_horario(index_formiga);
	if(sem_atracao(index_formiga)) vira_sentido_contra_horario(index_formiga);
	if(sem_atracao(index_formiga)) vira_sentido_contra_horario(index_formiga);

	if(caminho_livre(index_formiga)==false)		// seguranca caso o procedimento nao produza bons resultados
		muda_aleatoriamente_direcao(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  gasta_energia                                 //
//                                                                     //
// CARACTERISTICAS    :  gasta energia da formiga atual                //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  16/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void gasta_energia(int index_formiga)
{
	if(formiga[index_formiga].estomago>0) formiga[index_formiga].estomago--;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  doze_ifs                                      //
//                                                                     //
// CARACTERISTICAS    :  conjunto de regras do algoritmo simbolico     //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/03                                      //
/////////////////////////////////////////////////////////////////////////

DEBUG_DOZE_IFS doze_ifs(int index_formiga, bool modo_debug)
{
	DEBUG_DOZE_IFS info;

	info.caso=99;
	info.linha=0;

	obtenha_dados_sensores(index_formiga);

	// IF (1)
	if(formiga[index_formiga].tato==predador)
	{
		info.caso=1;
		info.linha=1;
	}
	if(objeto_acima(index_formiga)==muito_perto && formiga[index_formiga].no_ninho==false &&
		formiga[index_formiga].estomago>10)
	{
		info.caso=1;
		info.linha=2;
	}
	if(positivo_e_crescente(index_formiga,c_alomonio) && formiga[index_formiga].estomago>10)
	{
		info.caso=1;
		info.linha=3;
	}
	if(formiga[index_formiga].audicao>=10 && formiga[index_formiga].estomago>10)
	{
		info.caso=1;
		info.linha=4;
	}
	
	// IF (2)
	if(info.caso==99)
	{
	if(objeto_acima(index_formiga)==sem_presenca &&
		formiga[index_formiga].tato!=predador &&
		formiga[index_formiga].olfato_alomonio[0]==0 &&
		formiga[index_formiga].audicao<10 &&
		formiga[index_formiga].alerta==true)
	{
		info.caso=2;
		info.linha=5;
	}
	}

	// IF (3)
	if(info.caso==99)
	{
		if(objeto_frente(index_formiga)==muito_perto_baixo &&
		objeto_acima(index_formiga)==sem_presenca &&
		formiga[index_formiga].tato==outra &&
		formiga[index_formiga].olfato_alomonio[0]==0 &&
		formiga[index_formiga].audicao<10)
	{
		info.caso=3;
		info.linha=6;
	}
	}

	// IF (4)
	if(info.caso==99)
	{
		if(objeto_acima(index_formiga)==sem_presenca &&		// um pouco otimizado abaixo
		formiga[index_formiga].olfato_alomonio[0]==0 &&
		formiga[index_formiga].audicao<10 &&
		formiga[index_formiga].estomago_social<90)
	{
		if(formiga[index_formiga].tato==agua)
		{
			info.caso=4;
			info.linha=7;
		}
		if(formiga[index_formiga].tato>comida0)
		{
			info.caso=4;
			info.linha=8;
		}
	}
	}

	// IF (5)
	if(info.caso==99)
	{
		if(objeto_frente(index_formiga)!=muito_perto_alto &&
		objeto_acima(index_formiga)==sem_presenca &&
		formiga[index_formiga].tato!=agua &&
		formiga[index_formiga].tato!=predador &&
		positivo_e_decrescente(index_formiga,c_alomonio) &&
		formiga[index_formiga].audicao<10)
	{
		info.caso=5;
		info.linha=9;
	}
	}

	// IF (6)
	if(info.caso==99)
	{
		if(objeto_acima(index_formiga)==sem_presenca &&
		formiga[index_formiga].tato==margem)
	{
		info.caso=6;
		info.linha=10;
	}
		
	//if(objeto_frente(index_formiga)==muito_perto_alto &&
	//objeto_acima(index_formiga)==sem_presenca)
	//{
	//	info.caso=6;
	//	info.linha=11;
	//}

	if(objeto_acima(index_formiga)==sem_presenca &&
	formiga[index_formiga].olfato_alomonio[0]==0 &&
	positivo_e_decrescente(index_formiga,c_feromonio) &&
	formiga[index_formiga].audicao<10 && 
	formiga[index_formiga].estomago>10 &&
	formiga[index_formiga].estomago_social<10)
	{
		info.caso=6;
		info.linha=12;
	}

	if(objeto_acima(index_formiga)==sem_presenca &&
	formiga[index_formiga].olfato_alomonio[0]==0 &&
	positivo_e_decrescente(index_formiga,c_ninho) &&
	formiga[index_formiga].audicao<10 &&
	formiga[index_formiga].estomago<=10)
	{
		info.caso=6;
		info.linha=13;
	}

	if(objeto_acima(index_formiga)==sem_presenca &&
	formiga[index_formiga].olfato_alomonio[0]==0 &&
	positivo_e_decrescente(index_formiga,c_ninho) &&
	formiga[index_formiga].audicao<10 &&
	formiga[index_formiga].estomago_social>=10)
	{
		info.caso=6;
		info.linha=14;
	}

	if(objeto_acima(index_formiga)==sem_presenca &&
	formiga[index_formiga].olfato_alomonio[0]==0 &&
	positivo_e_decrescente(index_formiga,c_alimento) &&
	formiga[index_formiga].audicao<10 &&
	formiga[index_formiga].estomago>10 &&
	formiga[index_formiga].estomago_social<10)
	{
		info.caso=6;
		info.linha=15;
	}
	}

	// IF (7)
	if(info.caso==99)
	{
		if(objeto_frente(index_formiga)==muito_perto_baixo &&	// otimizacao abaixo
		objeto_acima(index_formiga)==sem_presenca &&
		formiga[index_formiga].olfato_alomonio[0]==0 &&
		formiga[index_formiga].audicao<10 &&
		formiga[index_formiga].tato==ninho)
	{
		if(formiga[index_formiga].estomago<=10)
		{
			info.caso=7;
			info.linha=16;
		}
		if(formiga[index_formiga].estomago_social>=10)
		{
			info.caso=7;
			info.linha=17;
		}
	}
	}

	// IF (8) OLFATO

	if(info.caso==99)
	{

		if(objeto_frente(index_formiga)!=muito_perto_alto &&
		formiga[index_formiga].tato!=agua &&
		formiga[index_formiga].tato!=predador)
		{

			if(positivo_e_crescente(index_formiga,c_feromonio) &&
			formiga[index_formiga].estomago>10 &&
			formiga[index_formiga].estomago_social<10)
			{
				info.caso=8;
				info.linha=23;
			}

			if(positivo_e_crescente(index_formiga,c_feromonio) &&
			formiga[index_formiga].estomago<=10)
			{
				info.caso=8;
				info.linha=24;
			}

			if(positivo_e_crescente(index_formiga,c_ninho) &&
			formiga[index_formiga].estomago_social>=10)
			{
				info.caso=8;
				info.linha=25;
			}

			if(formiga[index_formiga].olfato_feromonio==0 &&
			positivo_e_crescente(index_formiga,c_alimento) &&
			formiga[index_formiga].estomago>10 &&
			formiga[index_formiga].estomago_social<10)
			{
				info.caso=8;
				info.linha=26;
			}
		}
	}

	// IF (8) VISAO

	if(info.caso==99)
	{
		if(objeto_acima(index_formiga)==sem_presenca && // otimizacao
		formiga[index_formiga].olfato_alomonio[0]==0 &&
		formiga[index_formiga].audicao<10)
	{
		if(objeto_frente(index_formiga)==longe)
		//objeto_esquerda(index_formiga)<=longe &&
		//objeto_direita(index_formiga)<=longe)
		//formiga[index_formiga].olfato_feromonio[0]==0 && // olfato tratado acima com prioridade
		//formiga[index_formiga].olfato_ninho[0]==0 &&
		//formiga[index_formiga].olfato_alimento[0]==0)
		{
			info.caso=8;
			info.linha=18;
		}

		if(objeto_frente(index_formiga)==perto)
		//objeto_esquerda(index_formiga)<=perto &&
		//objeto_direita(index_formiga)<=perto)
		//formiga[index_formiga].olfato_feromonio[0]==0 &&  // olfato tratado acima
		//formiga[index_formiga].olfato_ninho[0]==0 &&
		//formiga[index_formiga].olfato_alimento[0]==0)
		{
			info.caso=8;
			info.linha=19;
		}

		if(objeto_frente(index_formiga)==muito_perto_baixo &&
		formiga[index_formiga].tato==sem_presenca)
		//formiga[index_formiga].olfato_feromonio[0]==0 &&  // olfato tratado acima
		//formiga[index_formiga].olfato_ninho[0]==0 &&
		//formiga[index_formiga].olfato_alimento[0]==0)
		{
			info.caso=8;
			info.linha=20;
		}

		if(objeto_frente(index_formiga)==muito_perto_baixo &&
		formiga[index_formiga].tato==parede)
		//formiga[index_formiga].olfato_feromonio[0]==0 &&  // olfato tratado acima
		//formiga[index_formiga].olfato_ninho[0]==0 &&
		//formiga[index_formiga].olfato_alimento[0]==0)
		{
			info.caso=8;
			info.linha=21;
		}

		if(objeto_frente(index_formiga)==muito_perto_baixo &&
		formiga[index_formiga].tato==terra)
		//formiga[index_formiga].olfato_feromonio[0]==0 &&  // olfato tratado acima
		//formiga[index_formiga].olfato_ninho[0]==0 &&
		//formiga[index_formiga].olfato_alimento[0]==0)
		{
			info.caso=8;
			info.linha=22;
		}
	}
	}
	
	// IF (9)
	if(info.caso==99)
	{
	if(objeto_frente(index_formiga)==sem_presenca &&
		objeto_esquerda(index_formiga)==sem_presenca &&
		objeto_direita(index_formiga)==sem_presenca &&
		objeto_acima(index_formiga)==sem_presenca &&
		formiga[index_formiga].olfato_alomonio[0]==0 &&
		formiga[index_formiga].olfato_feromonio[0]==0 &&
		formiga[index_formiga].olfato_ninho[0]==0 &&
		formiga[index_formiga].olfato_alimento[0]==0 &&
		formiga[index_formiga].audicao<10)
	{
		info.caso=9;
		info.linha=27;
	}
	}

	// IF (10)
	if(info.caso==99)
	{
		if((objeto_acima(index_formiga)==sem_presenca && // otimizacao
	//	formiga[index_formiga].olfato_alomonio[0]==0 && // o cheiro ja foi tratado acima
	//	formiga[index_formiga].olfato_feromonio[0]==0 &&
	//	formiga[index_formiga].olfato_ninho[0]==0 &&
	//	formiga[index_formiga].olfato_alimento[0]==0 &&
		formiga[index_formiga].audicao<10))
	{
		if(objeto_frente(index_formiga)==sem_presenca &&
		objeto_esquerda(index_formiga)==longe &&
		objeto_direita(index_formiga)==longe)
		{
			info.caso=10;
			info.linha=28;
		}
	
		if(objeto_frente(index_formiga)<=longe &&
		objeto_esquerda(index_formiga)==perto &&
		objeto_direita(index_formiga)==perto)
		{
			info.caso=10;
			info.linha=29;
		}

		if(objeto_frente(index_formiga)<=perto &&
		objeto_esquerda(index_formiga)==muito_perto_baixo &&
		objeto_direita(index_formiga)==muito_perto_baixo)
		{
			info.caso=10;
			info.linha=30;
		}
	}
	}

	// IF (11)
	if(info.caso==99)
	{
		if(objeto_acima(index_formiga)==sem_presenca && // otimizacao
		//formiga[index_formiga].olfato_alomonio[0]==0 && // cheiro ja tratado acima
		//formiga[index_formiga].olfato_feromonio[0]==0 &&
		//formiga[index_formiga].olfato_ninho[0]==0 &&
		//formiga[index_formiga].olfato_alimento[0]==0 &&
		formiga[index_formiga].audicao<10)
	{

		if(objeto_frente(index_formiga)==sem_presenca &&
		objeto_esquerda(index_formiga)==sem_presenca &&
		objeto_direita(index_formiga)==longe)
		{
			info.caso=11;
			info.linha=31;
		}

		if(objeto_frente(index_formiga)<=longe &&
		objeto_esquerda(index_formiga)<=longe &&
		objeto_direita(index_formiga)==perto)
		{
			info.caso=11;
			info.linha=32;
		}

		if(objeto_frente(index_formiga)<=perto &&
		objeto_esquerda(index_formiga)<=perto &&
		objeto_direita(index_formiga)==muito_perto_baixo)
		{
			info.caso=11;
			info.linha=33;
		}
	}
	}

	// IF (12)
	if(info.caso==99)
	{
		if(objeto_acima(index_formiga)==sem_presenca && // otimizacao
		//formiga[index_formiga].olfato_alomonio[0]==0 &&
		//formiga[index_formiga].olfato_feromonio[0]==0 &&
		//formiga[index_formiga].olfato_ninho[0]==0 &&
		//formiga[index_formiga].olfato_alimento[0]==0 &&
		formiga[index_formiga].audicao<10)
	{

		if(objeto_frente(index_formiga)==sem_presenca &&
		objeto_esquerda(index_formiga)==longe &&
		objeto_direita(index_formiga)==sem_presenca) 
		{
			info.caso=12;
			info.linha=34;
		}

		if(objeto_frente(index_formiga)<=longe &&
		objeto_esquerda(index_formiga)==perto &&
		objeto_direita(index_formiga)<=longe)
		{
			info.caso=12;
			info.linha=35;
		}

		if(objeto_frente(index_formiga)<=perto &&
		objeto_esquerda(index_formiga)==muito_perto_baixo &&
		objeto_direita(index_formiga)<=perto)
		{
			info.caso=12;
			info.linha=36;
		}
	}
	}

	formiga[index_formiga].decisao=info.caso;

	if(modo_debug==false)
	{
		switch(info.caso)
		{
		case 1: if(info.linha!=3) fugir_perigo(index_formiga,true);
			       else fugir_perigo(index_formiga,false); break;
		case 2: voltar_normalidade(index_formiga); break;
		case 3: evitar_colisao(index_formiga); break;
		case 4: pegar_comida(index_formiga); break;
		case 5: sair_alerta(index_formiga); break;
		case 6: sair_caminho_ruim(index_formiga); break;
		case 7: entrar_ninho(index_formiga,true); break;
		case 8: seguir_atracao_frente(index_formiga); break;
		case 9: buscar_sinal(index_formiga); break;
		case 10: seguir_atracao_ambigua(index_formiga); break;
		case 11: seguir_atracao_direita(index_formiga); break;
		case 12: seguir_atracao_esquerda(index_formiga); break;
		case 99: buscar_sinal(index_formiga); break;		// caso "default"
		}
	}

	return info;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  preenche_linha_x                              //
//                                                                     //
// CARACTERISTICAS    :  define uma faixa do mundo virtual variando a  //
// coordenada x                                                        //
// PARAMETROS         :  inicio e fim de x, y e z fixos e valor        //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void preenche_linha_x(int inicio, int fim, int y_pos, int z_pos, int valor)
{
	int local;

	for(local=inicio;local<fim;local++)
		mundo.elemento[local][y_pos][z_pos].paisagem=valor;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  preenche_linha_y                              //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void preenche_linha_y(int inicio, int fim, int x_pos, int z_pos, int valor)
{
	int local;

	for(local=inicio;local<fim;local++)
		mundo.elemento[x_pos][local][z_pos].paisagem=valor;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inicializa_cheiro_ninho                       //
//                                                                     //
// CARACTERISTICAS    :  inicializa o cheiro do ninho no inicio do     //
// programa                                                            //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  procedimento otimizado da versao 1            //
//                                                                     //
// ULTIMA ATUALIZACAO :  07/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void inicializa_cheiro_ninho()
{

	int i, j, k, limite_x_menos, limite_x_mais, limite_y_menos, limite_y_mais, limite_z_mais, limite_z_menos;
	
	int x_atual, y_atual, z_atual, x_diff, y_diff, z_diff, cheiro_produzido, alcance;

	alcance=parametros.alcance_cheiro_ninho;

	for(i=0;i<max_mundo_x;i++)
		for(j=0;j<max_mundo_y;j++)
			for(k=0;k<max_mundo_z;k++)
				mundo.elemento[i][j][k].cheiro_ninho=0;

	for(k=0;k<max_mundo_z;k++)
		for(j=0;j<max_mundo_y;j++)
			for(i=0;i<max_mundo_x;i++)
			{
				if(mundo.elemento[i][j][k].paisagem==ninho)
				{

					// inicializacao

					limite_x_menos=i-alcance;
					limite_x_mais=i+alcance+1;
	
					limite_y_menos=j-alcance;
					limite_y_mais=j+alcance+1;

					limite_z_menos=k-alcance;
					limite_z_mais=k+alcance+1;

					// ajuste a margens se aplicavel

					if(limite_x_menos<0) limite_x_menos=0;
					if(limite_y_menos<0) limite_y_menos=0;
					if(limite_z_menos<0) limite_z_menos=0;

					if(limite_x_mais>max_mundo_x) limite_x_mais=max_mundo_x;
					if(limite_y_mais>max_mundo_y) limite_y_mais=max_mundo_y;
					if(limite_z_mais>max_mundo_z) limite_z_mais=max_mundo_z;

					for(x_atual=limite_x_menos;x_atual<limite_x_mais;x_atual++)
						for(y_atual=limite_y_menos;y_atual<limite_y_mais;y_atual++)
							for(z_atual=limite_z_menos;z_atual<limite_z_mais;z_atual++)
							{
				
								x_diff=abs(x_atual-i);
								y_diff=abs(y_atual-j);
								z_diff=abs(z_atual-k);

								cheiro_produzido=
									99-x_diff-y_diff-z_diff;

								if(cheiro_produzido>mundo.elemento[x_atual][y_atual][z_atual].cheiro_ninho &&
									cheiro_produzido>(99-alcance))
									mundo.elemento[x_atual][y_atual][z_atual].cheiro_ninho=cheiro_produzido;
							}
				}
			}

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  atualiza_cheiro_alimento                      //
//                                                                     //
// CARACTERISTICAS    :  atualiza o cheiro de alimento de um lugar     //
// especifico                                                          //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  06/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void atualiza_cheiro_alimento(int x_pos, int y_pos, int z_pos)
{
	int limite_x_menos, limite_x_mais, limite_y_menos, limite_y_mais, limite_z_mais, limite_z_menos;
	
	int x_atual, y_atual, z_atual, x_diff, y_diff, z_diff, cheiro_produzido, comparativo, alcance;

	alcance=parametros.alcance_cheiro_alimento;

	// inicializacao

	limite_x_menos=x_pos-alcance;
	limite_x_mais=x_pos+alcance+1;
	
	limite_y_menos=y_pos-alcance;
	limite_y_mais=y_pos+alcance+1;

	limite_z_menos=z_pos-alcance;
	limite_z_mais=z_pos+alcance+1;

	// ajuste a margens se aplicavel

	if(limite_x_menos<0) limite_x_menos=0;
	if(limite_y_menos<0) limite_y_menos=0;
	if(limite_z_menos<0) limite_z_menos=0;

	if(limite_x_mais>max_mundo_x) limite_x_mais=max_mundo_x;
	if(limite_y_mais>max_mundo_y) limite_y_mais=max_mundo_y;
	if(limite_z_mais>max_mundo_z) limite_z_mais=max_mundo_z;

	for(x_atual=limite_x_menos;x_atual<limite_x_mais;x_atual++)
		for(y_atual=limite_y_menos;y_atual<limite_y_mais;y_atual++)
			for(z_atual=limite_z_menos;z_atual<limite_z_mais;z_atual++)
			{
				
				x_diff=abs(x_atual-x_pos);
				y_diff=abs(y_atual-y_pos);
				z_diff=abs(z_atual-z_pos);

				cheiro_produzido=99-x_diff-y_diff-z_diff;

				if(cheiro_produzido>mundo.elemento[x_atual][y_atual][z_atual].cheiro_alimento &&
					cheiro_produzido>(99-alcance))
					mundo.elemento[x_atual][y_atual][z_atual].cheiro_alimento=cheiro_produzido;
			}

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  atualiza_cheiro_alimentos                     //
//                                                                     //
// CARACTERISTICAS    :  atualiza o cheiro dos alimentos               //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  06/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void atualiza_cheiro_alimentos()
{
	int x, y, z;

	for(x=0;x<max_mundo_x;x++)
		for(y=0;y<max_mundo_y;y++)
			for(z=0;z<max_mundo_z;z++)
				mundo.elemento[x][y][z].cheiro_alimento=0;

	for(x=0;x<max_mundo_x;x++)
		for(y=0;y<max_mundo_y;y++)
			for(z=0;z<max_mundo_z;z++)
				if(mundo.elemento[x][y][z].paisagem==agua || 
					(mundo.elemento[x][y][z].paisagem>comida0 &&
					mundo.elemento[x][y][z].paisagem<=comida2))
						atualiza_cheiro_alimento(x,y,z);
			
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inicializar_mundo                             //
//                                                                     //
// CARACTERISTICAS    :  inicializa o mundo virtual                    //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  24/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void simulacao_inicializar_mundo()
{
	int i, j, k;

	// checagem de direcoes

	for(i=0;i<max_mundo_x;i++)
		for(j=0;j<max_mundo_y;j++)
			for(k=0;k<max_mundo_z;k++)
			{
				mundo.elemento[i][j][k].paisagem=vazio;  // inicialmente vazio
				mundo.elemento[i][j][k].cheiro_alimento=0;
				mundo.elemento[i][j][k].cheiro_alomonio=0;
				mundo.elemento[i][j][k].cheiro_feromonio=0;
				mundo.elemento[i][j][k].cheiro_ninho=0;
				mundo.elemento[i][j][k].barulho=1;	// barulho base
			}

	// Paredes em todos os niveis

	for(k=0;k<max_mundo_z-1;k++)
	{
		preenche_linha_y(0,20,11,k,parede);
		preenche_linha_y(0,25,15,k,parede);
		preenche_linha_y(29,max_mundo_y,15,k,parede);
		preenche_linha_y(24,max_mundo_y,11,k,parede);
		preenche_linha_x(1,11,9,k,parede);
		preenche_linha_x(0,11,39,k,parede);
	}

	// agua

	preenche_linha_x(63,65,0,0,agua);
	preenche_linha_x(62,66,1,0,agua);
	preenche_linha_x(62,66,2,0,agua);
	preenche_linha_x(63,65,3,0,agua);

	// comida

	preenche_linha_x(3,5,3,0,comida2);
	preenche_linha_x(2,6,4,0,comida2);
	preenche_linha_x(2,6,5,0,comida2);
	preenche_linha_x(2,6,6,0,comida2);
	preenche_linha_x(3,5,7,0,comida2);

	preenche_linha_x(2,8,32,0,comida2);
	preenche_linha_x(1,9,33,0,comida2);
	preenche_linha_x(1,9,34,0,comida2);
	preenche_linha_x(2,8,35,0,comida2);

	preenche_linha_x(3,5,41,0,comida2);
	preenche_linha_x(2,6,42,0,comida2);
	preenche_linha_x(3,5,43,0,comida2);

	preenche_linha_x(40,42,38,0,comida1);

	// elevacao

	for(i=15;i<35;i++)
		preenche_linha_x(58,80,i,0,terra);

	preenche_linha_y(17,33,57,0,terra);

	preenche_linha_x(59,80,14,0,terra);
	preenche_linha_x(62,80,13,0,terra);
	preenche_linha_x(65,80,12,0,terra);
	preenche_linha_x(67,80,11,0,terra);

	for(j=5;j<11;j++)
		preenche_linha_x(80-j,80,j,0,terra);

	for(j=35;j<39;j++)
		preenche_linha_x(58+(j-34)*4,80,j,0,terra);

	preenche_linha_x(75,80,39,0,terra);

	// ninho nivel 0

	for(j=15;j<33;j++)
		preenche_linha_x(26,44,j,0,ninho);

	preenche_linha_x(27,44,14,0,ninho);
	preenche_linha_x(29,44,13,0,ninho);
	preenche_linha_x(30,43,12,0,ninho);
	preenche_linha_x(32,42,11,0,ninho);
	preenche_linha_x(34,41,10,0,ninho);

	preenche_linha_x(28,42,33,0,ninho);

	preenche_linha_y(17,23,25,0,ninho);
	preenche_linha_y(28,31,25,0,ninho);

	preenche_linha_y(17,31,44,0,ninho);
	preenche_linha_y(22,30,45,0,ninho);
	preenche_linha_y(26,29,46,0,ninho);

	// comida nivel 1

	preenche_linha_x(75,79,14,1,comida1);
	preenche_linha_x(74,80,15,1,comida1);
	preenche_linha_x(73,79,16,1,comida1);
	preenche_linha_x(73,78,17,1,comida1);
	preenche_linha_x(74,76,18,1,comida1);

	preenche_linha_x(75,78,31,1,comida2);
	preenche_linha_x(73,79,32,1,comida2);
	preenche_linha_x(72,79,33,1,comida2);
	preenche_linha_x(73,77,34,1,comida2);

	// terra nivel 1

	for(j=20;j<31;j++)
		preenche_linha_x(60,67,j,1,terra);

	preenche_linha_x(60,66,19,1,terra);
	preenche_linha_x(60,65,18,1,terra);
	preenche_linha_x(61,64,17,1,terra);
	preenche_linha_x(62,64,16,1,terra);

	preenche_linha_x(62,67,31,1,terra);
	preenche_linha_x(64,66,32,1,terra);

	preenche_linha_y(25,28,59,1,terra);
	preenche_linha_y(21,24,67,1,terra);

	// ninho nivel 1

	for(j=19;j<29;j++)
		preenche_linha_x(30,42,j,1,ninho);

	preenche_linha_y(20,23,29,1,ninho);

	preenche_linha_y(23,28,42,1,ninho);
	preenche_linha_y(24,27,43,1,ninho);

	preenche_linha_x(32,41,18,1,ninho);
	preenche_linha_x(34,41,17,1,ninho);
	preenche_linha_x(36,40,16,1,ninho);
	
	preenche_linha_x(31,40,29,1,ninho);
	preenche_linha_x(36,39,30,1,ninho);

	// terra nivel 2

	preenche_linha_y(22,30,62,2,terra);
	preenche_linha_y(20,31,63,2,terra);
	preenche_linha_y(23,30,64,2,terra);

	// ninho nivel 2

	for(j=21;j<27;j++)
		preenche_linha_x(32,40,j,2,ninho);

	preenche_linha_x(35,39,20,2,ninho);

	preenche_linha_y(21,23,31,2,ninho);

	preenche_linha_y(23,26,40,2,ninho);

	preenche_linha_x(34,39,27,2,ninho);

	// inicializacao dos cheiros
	atualiza_cheiro_alimentos();
	inicializa_cheiro_ninho();

	// predadores inicialmente nao presentes
	mundo.predador[0]=false; // sapato nao presente
	mundo.predador[1]=false; // passaro nao presente

	// 4 saidas no ninho a seguir

	saida_ninho[0].x=35;
	saida_ninho[0].y=23;
	saida_ninho[0].z=3;

	saida_ninho[1].x=36;
	saida_ninho[1].y=23;
	saida_ninho[1].z=3;

	saida_ninho[2].x=35;
	saida_ninho[2].y=24;
	saida_ninho[2].z=3;

	saida_ninho[3].x=36;
	saida_ninho[3].y=24;
	saida_ninho[3].z=3;

	sapato.limite_baixo_x=0;
	sapato.limite_alto_x=20;
	sapato.limite_baixo_y=0;
	sapato.limite_alto_y=5;

	passaro.limite_baixo_x=0;
	passaro.limite_alto_x=10;
	passaro.limite_baixo_y=0;
	passaro.limite_alto_y=10;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  devolve_cor_decisao                           //
//                                                                     //
// CARACTERISTICAS    :  devolve a cor da decisao para visualizacao    //
// da formiga                                                          //
// PARAMETROS         :  numero da decisao                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  17/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void devolve_cor_decisao(int dec)
{
	switch(dec)
	{
	case 1: setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTRED ); break; // fugir do perigo
	case 2: setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGREEN ); break; // voltar a normalidade
	case 3: setcolor( XFOREGROUND_WHITE | XBACKGROUND_CYAN ); break; // evitar colisao
	case 4: setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTCYAN ); break; // pegar comida
	case 5: setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW ); break; // sair do alerta
	case 6: setcolor( XFOREGROUND_WHITE | XBACKGROUND_BROWN ); break; // sair do caminho ruim
	case 7: setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY ); break; // entrar no ninho
	case 8: setcolor( XFOREGROUND_WHITE | XBACKGROUND_GREEN ); break; // seguir atracao frente
	case 9: setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY ); break; // buscar sinais
	case 10: setcolor( XFOREGROUND_WHITE | XBACKGROUND_PURPLE ); break; // seguir atracao ambigua
	case 11: setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLUE ); break; // seguir atracao esquerda
	case 12: setcolor( XFOREGROUND_WHITE | XBACKGROUND_RED ); break; // seguir atracao direita
	case 99: setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE ); break; // nenhuma regra se aplica
	}

	//printf(" %i ",dec);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  imprima_formiga                               //
//                                                                     //
// CARACTERISTICAS    :  mostra a formiga na tela com a inclinacao     //
// de acordo com a direcao dela                                        //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void imprima_formiga(int index_formiga)
{
	//switch(formiga[index_formiga].coord_z)
	//{
	//case 0: setcolor( XFOREGROUND_BLACK | XBACKGROUND_DARKGRAY ); break;
	//case 1: setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY ); break;
	//case 2: setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE ); break;
	//case 3: setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW ); break;
	//case 4: setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_YELLOW ); break;
	//}

	devolve_cor_decisao(formiga[index_formiga].decisao);

	if(formiga[index_formiga].vida!=viva)
		setcolor( XFOREGROUND_LIGHTPURPLE | XBACKGROUND_PURPLE );
	
	if(formiga[index_formiga].vida==viva && formiga[index_formiga].no_ninho==false)
	{
		switch(formiga[index_formiga].direcao)
		{
		case 0: printf("%c",char(124)); break;   // "|"
		case 45: printf("%c",char(47)); break;   // "/"
		case 90: printf("%c",char(45)); break;   // "-"
		case 135: printf("%c",char(92)); break;  // "\"
		case 180: printf("%c",char(124)); break; // "|"
		case 225: printf("%c",char(47)); break;  // "/"
		case 270: printf("%c",char(45)); break;  // "-"
		case 315: printf("%c",char(92)); break;  // "\"
		}
	} else
		if(formiga[index_formiga].vida==esmagada) printf("#"); else
			if(formiga[index_formiga].vida==fome) printf("o"); else
				if(formiga[index_formiga].vida==afogada) printf("~"); else
					if(formiga[index_formiga].no_ninho) printf("@"); else
						printf(" ");
}

int devolve_formiga_z(int xc, int yc)
{
	int flag;

	flag=4;

	if(mundo.elemento[xc][yc][0].paisagem==vazio)
		return 0;

	while(mundo.elemento[xc][yc][flag-1].paisagem==vazio)
		flag--;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  visualiza_ponto                               //
//                                                                     //
// CARACTERISTICAS    :  produz a visualizacao de um ponto no ambiente //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  07/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void visualiza_ponto(int x, int y, int z, int index_formiga, int ol_x, int ol_y)
{
	int acha_elemento, codigo_elemento, valor_feromonio;
	
	if(ol_x!=99) gotoxy(ol_x,ol_y); else
		gotoxy(x,y+2);

	codigo_elemento=mundo.elemento[x][y][z].paisagem;
	
	acha_elemento=z;

	if(codigo_elemento==vazio)
	{
				
		while(mundo.elemento[x][y][acha_elemento].paisagem==vazio &&
			acha_elemento>0)
			acha_elemento--;
	
		if(mundo.elemento[x][y][acha_elemento].paisagem==vazio)

		setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );
	}

	codigo_elemento=mundo.elemento[x][y][acha_elemento].paisagem;
	
	if(codigo_elemento>comida0 && codigo_elemento<=comida2)
	{
		setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_WHITE );
	}
	
	switch(codigo_elemento)
	{
		case ninho: if(acha_elemento==0) setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_DARKGRAY );
		if(acha_elemento==1) setcolor( XFOREGROUND_DARKGRAY | XBACKGROUND_LIGHTGRAY );
		if(acha_elemento==2) setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_WHITE );
		break;

		case parede: if(acha_elemento==0) setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_RED ); 
		if(acha_elemento==1) setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_LIGHTPURPLE );
		if(acha_elemento>1) setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_LIGHTRED );
		break;
		
		case agua: setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_LIGHTBLUE );
		break;
		
		case terra: if(acha_elemento==0) setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BROWN );
		if(acha_elemento==1) setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_GREEN );
		if(acha_elemento==2) setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_LIGHTGREEN );
		break;
		
		case predador: setcolor( XFOREGROUND_WHITE | XBACKGROUND_YELLOW );
		break;

		case outra: setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_LIGHTPURPLE );
		break;
	}

	if(index_formiga==sem_formiga || formiga[index_formiga].vida==comida) 
	{

		z=devolve_formiga_z(x,y);

	switch(parametros.visual)
	{
	case v_alomonio: if(mundo.elemento[x][y][z].cheiro_alomonio!=0) 
						 printf("%i",mundo.elemento[x][y][z].cheiro_alomonio/11); else
						 printf(" "); break;

	case v_feromonio: if(mundo.elemento[x][y][z].cheiro_feromonio!=0) 
					  {
						  acha_min_max_feromonio();
						  valor_feromonio=mundo.elemento[x][y][z].cheiro_feromonio / passo_feromonio;
						  if(valor_feromonio==10) printf("A"); else
						  printf("%i",valor_feromonio); 
					  } else
						  printf(" "); break;

	case v_ninho: if(mundo.elemento[x][y][z].cheiro_ninho!=0) 
					  printf("%i",mundo.elemento[x][y][z].cheiro_ninho/11); else
					  printf(" "); break;

	case v_alimento: if(mundo.elemento[x][y][z].cheiro_alimento!=0) 
						 printf("%i",mundo.elemento[x][y][z].cheiro_alimento/11); else
						 printf(" "); break;

	case v_barulho: if(mundo.elemento[x][y][z].barulho!=0)
						printf("%i",mundo.elemento[x][y][z].barulho/11); else
						printf(" "); break;
	case v_normal : printf(" "); break;
	}

	} else
	imprima_formiga(index_formiga);

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  mostra_formigas                               //
//                                                                     //
// CARACTERISTICAS    :  mostra as formigas no ambiente                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void mostra_formigas()
{
	int i;

	for(i=0;i<parametros.numero_formigas;i++)
		if(formiga[i].no_ninho==false)
			visualiza_ponto(formiga[i].coord_x,formiga[i].coord_y,formiga[i].coord_z,i,99,99);

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  acha_min_max_feromonio                        //
//                                                                     //
// CARACTERISTICAS    :  acha os limites de feromonio no ambiente      //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  17/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void acha_min_max_feromonio()
{
	int x, y, z;

	min_feromonio=mundo.elemento[0][0][0].cheiro_feromonio; // inicializacao
	max_feromonio=mundo.elemento[0][0][0].cheiro_feromonio;

	for(x=0;x<max_mundo_x;x++)
		for(y=0;y<max_mundo_y;y++)
			for(z=0;z<max_mundo_z;z++)
			{
				if(mundo.elemento[x][y][z].cheiro_feromonio>max_feromonio)
					max_feromonio=mundo.elemento[x][y][z].cheiro_feromonio;

				if(mundo.elemento[x][y][z].cheiro_feromonio<min_feromonio)
				{
					//printf("%i=%i=%i => %i",x,y,z,mundo.elemento[x][y][z].cheiro_feromonio);
					//getch();
					min_feromonio=mundo.elemento[x][y][z].cheiro_feromonio;
				}
			}

	passo_feromonio = (max_feromonio - min_feromonio) / 10;

	if(passo_feromonio < 1) passo_feromonio=1;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  simulacao_mostrar_mundo                       //
//                                                                     //
// CARACTERISTICAS    :  mostra o mundo virtual de cima                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void simulacao_mostrar_mundo(int nivel)
{
	int i, j;

	gotoxy(0,0);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	printf("                   M A P A   D O   M U N D O   V I R T U A L                    ");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_DARKGRAY );

	gotoxy(0,1);

	printf(" MARGEM - MARGEM - MARGEM - MARGEM - MARGEM - MARGEM - MARGEM - MARGEM - MARGEM ");

	gotoxy(0,47);

	printf(" MARGEM - MARGEM - MARGEM - MARGEM - MARGEM - MARGEM - MARGEM - MARGEM - MARGEM ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	gotoxy(0,48);

	printf(" ITERACAO:                      NIVEL:   FORMIGA:     ELEMENTO:                 ");

	gotoxy(64,48);

	switch(parametros.visual)
	{
	case v_normal:		printf("NORMAL         "); break;
	case v_feromonio:	printf("FEROMONIO      "); break;
	case v_ninho:		printf("CHEIRO NINHO   "); break;
	case v_alimento:	printf("CHEIRO ALIMENTO"); break;
	case v_barulho:		printf("BARULHO        "); break;
	case v_alomonio:	printf("ALOMONIO       "); break;
	}

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

	gotoxy(1,49);

	printf("PAISAGEM:");

	gotoxy(17,49);

	printf("FORMIGAS:");

	gotoxy(1,50);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_DARKGRAY );

	printf(" ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	printf(" ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	printf(" ");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

	printf(" NINHO");



	gotoxy(1,51);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	printf("   ");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

	printf(" ACUCAR");



	gotoxy(1,52);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_BLUE );

	printf("   ");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

	printf(" AGUA");



	gotoxy(1,53);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_BROWN );

	printf(" ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_GREEN );

	printf(" ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGREEN );

	printf(" ");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

	printf(" TERRA");



	gotoxy(1,54);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_RED );

	printf(" ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTPURPLE );

	printf(" ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTRED );

	printf(" ");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

	printf(" PAREDE");



	gotoxy(1,55);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

	printf("   ");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

	printf(" PREDADOR");

	

	for(i=0;i<6;i++)
	{
		gotoxy(17,50+i);
		devolve_cor_decisao(i+1);
		printf("|");

		setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );
		printf(" ");

		switch(i)
		{
		case 0: printf("FUGIR_PERIGO"); break;
		case 1: printf("VOLTAR_NORMALIDADE"); break;
		case 2: printf("EVITAR_COLISAO"); break;
		case 3: printf("PEGAR_COMIDA"); break;
		case 4: printf("SAIR_ALERTA"); break;
		case 5: printf("SAIR_CAMINHO_RUIM"); break;
		}
	}

	for(i=0;i<6;i++)
	{
		gotoxy(39,50+i);
		devolve_cor_decisao(i+7);
		printf("|");
		
		setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );
		printf(" ");

		switch(i)
		{
		case 0: printf("ENTRAR_NINHO"); break;
		case 1: printf("SEGUIR_ATRACAO_FRENTE"); break;
		case 2: printf("BUSCAR SINAL"); break;
		case 3: printf("SEGUIR_ATRACAO_AMBIGUA"); break;
		case 4: printf("SEGUIR_ATRACAO_ESQUERDA"); break;
		case 5: printf("SEGUIR_ATRACAO_DIREITA"); break;
		}
	}

	devolve_cor_decisao(99);

	gotoxy(66,50);

	printf("|");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );
	printf(" DEFAULT");


	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	if(parametros.visual==v_feromonio)
	{
		acha_min_max_feromonio();
		//gotoxy(2,2);
		//printf("%i  %i  %i  ",max_feromonio,min_feromonio,passo_feromonio);
		//getch();
	}

	for(i=0;i<max_mundo_x;i++)
		for(j=0;j<max_mundo_y;j++)
		{
			visualiza_ponto(i,j,nivel,sem_formiga,99,99);
		}

	mostra_formigas();

	home();

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  init_formigas                                 //
//                                                                     //
// CARACTERISTICAS    :  faz a inicializacao inicial da colonia        //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  28/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void init_formigas()
{
	int i, j, k;

	for(i=0;i<parametros.numero_formigas;i++)
	{
		for(j=0;j<8;j++)
			for(k=0;k<3;k++)
				formiga[i].visao[j][k]=sem_presenca;

		formiga[i].olfato_alomonio[0]=sem_presenca;
		formiga[i].olfato_feromonio[0]=sem_presenca;
		formiga[i].olfato_ninho[0]=sem_presenca;
		formiga[i].olfato_alimento[0]=sem_presenca;

		formiga[i].tato=sem_presenca;
		formiga[i].audicao=sem_presenca;

		formiga[i].estomago=cheio;
		formiga[i].estomago_social=vazio;

		formiga[i].coord_x=35;
		formiga[i].coord_y=25;
		formiga[i].coord_z=0;

		formiga[i].no_ninho=true;

		formiga[i].direcao=0;

		formiga[i].alerta=false;

		formiga[i].descanso=((rand()*parametros.numero_formigas*5) / RAND_MAX);

		formiga[i].vida=viva;
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  saia_do_ninho                                 //
//                                                                     //
// CARACTERISTICAS    :  ordena uma formiga a sair do ninho            //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  pode nao ter sucesso de acordo com congestio- //
// namentos nas saidas                                                 //
// ULTIMA ATUALIZACAO :  28/09/03                                      //
/////////////////////////////////////////////////////////////////////////

bool saia_do_ninho(int index_formiga)
{
	int valor_random;

	valor_random=int(rand()*4.0/(RAND_MAX));

	//gotoxy(0,1);

	//printf("saida de formiga %i requisitada   ",index_formiga);

	//getch();

	if(mundo.elemento[saida_ninho[valor_random].x][saida_ninho[valor_random].y][saida_ninho[valor_random].z].paisagem!=vazio) // alguem na entrada??
		return false;

	formiga[index_formiga].coord_x=saida_ninho[valor_random].x;
	formiga[index_formiga].coord_y=saida_ninho[valor_random].y;
	formiga[index_formiga].coord_z=saida_ninho[valor_random].z;

	formiga[index_formiga].no_ninho=false;

	formiga[index_formiga].descanso=0;

	formiga[index_formiga].estomago_social=0;
	formiga[index_formiga].estomago=99;

	return true;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  out_of_bounds                                 //
//                                                                     //
// CARACTERISTICAS    :  define se uma determinada posicao esta fora   //
// do tabuleiro                                                        //
// PARAMETROS         :  posicao(x,y,z)                                //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  29/09/03                                      //
/////////////////////////////////////////////////////////////////////////

bool out_of_bounds(int x, int y, int z)
{
	bool flag;

	flag=false;

	if(x<0 || x>=max_mundo_x ||
		y<0 || y>=max_mundo_y ||
		z<0 || z>=max_mundo_z)
		flag=true;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  estabelece_visao                              //
//                                                                     //
// CARACTERISTICAS    :  estabelece a visao de uma determinada formiga //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  falta fazer mais testes e comentar            //
//                                                                     //
// ULTIMA ATUALIZACAO :  29/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void estabelece_visao(int index_formiga)
{
	int x_mais, y_mais, z_code, x_atual, y_atual, z_atual, salva_direcao, distancia, virando, preencher, olho_luz[2];
	bool nenhum_objeto, objeto_achado, objeto_acima;

	if(formiga[index_formiga].no_ninho==false && formiga[index_formiga].vida==viva)
	{
	
	salva_direcao=formiga[index_formiga].direcao; 
	// esta funcao faz uso das rotinas de movimentacao virando a formiga e assim eh necessario
	// salvar a situacao inicial

	for(virando=0;virando<3;virando++)
		vira_sentido_contra_horario(index_formiga);

	// espelho

	for(virando=0;virando<8;virando++)
	{
		for(z_code=3;z_code>-1;z_code--)
		{
			x_mais=devolve_x_mais(formiga[index_formiga].direcao);
			y_mais=devolve_y_mais(formiga[index_formiga].direcao);

			x_atual=formiga[index_formiga].coord_x;
			y_atual=formiga[index_formiga].coord_y;
			z_atual=formiga[index_formiga].coord_z;

			distancia=0;

			nenhum_objeto=false;
			objeto_achado=false;

			while(nenhum_objeto==false && objeto_achado==false)
			{
				x_atual=x_atual+x_mais;
				y_atual=y_atual+y_mais;
				if(z_code==0) z_atual++; else
					if((z_code==1) && (distancia % 2==0))
						z_atual++; else
						if((z_code==3) && (distancia % 2==0))
							z_atual--;

				if(out_of_bounds(x_atual,y_atual,z_atual)) nenhum_objeto=true; else
					if(mundo.elemento[x_atual][y_atual][z_atual].paisagem!=vazio)
						objeto_achado=true; else distancia++;
			}

			if(nenhum_objeto) formiga[index_formiga].visao[virando][z_code]=sem_presenca; else
				if(distancia>40) formiga[index_formiga].visao[virando][z_code]=sem_presenca; else
					if(distancia>15) formiga[index_formiga].visao[virando][z_code]=longe; else
						if(distancia>5) formiga[index_formiga].visao[virando][z_code]=perto; else
							formiga[index_formiga].visao[virando][z_code]=muito_perto;

		}
		if(virando!=3) vira_sentido_horario(index_formiga); // caminho a frente duplicado
	}

	// vista para cima

	x_atual=formiga[index_formiga].coord_x;
	y_atual=formiga[index_formiga].coord_y;
	z_atual=formiga[index_formiga].coord_z;

	objeto_acima=false;

	while(z_atual<(max_mundo_z-1))
	{
		z_atual++;
		if(mundo.elemento[x_atual][y_atual][z_atual].paisagem!=vazio)
			objeto_acima=true;
	}

	for(preencher=0;preencher<4;preencher++)
		if(objeto_acima) formiga[index_formiga].visao[preencher][4]=muito_perto; else
			formiga[index_formiga].visao[preencher][4]=sem_presenca;

	// olhos de luz

	switch(salva_direcao)
	{
	case 0: olho_luz[0]=intenso; olho_luz[1]=intenso; break;
	case 45: olho_luz[0]=intenso; olho_luz[1]=medio; break;
	case 90: olho_luz[0]=intenso; olho_luz[1]=fraco; break;
	case 135: olho_luz[0]=medio; olho_luz[1]=fraco; break;
	case 180: olho_luz[0]=fraco; olho_luz[1]=fraco; break;
	case 225: olho_luz[0]=fraco; olho_luz[1]=medio; break;
	case 270: olho_luz[0]=fraco; olho_luz[1]=intenso; break;
	case 315: olho_luz[0]=medio; olho_luz[1]=intenso; break;
	}

	formiga[index_formiga].visao[4][4]=olho_luz[0];
	formiga[index_formiga].visao[5][4]=olho_luz[0];
	formiga[index_formiga].visao[6][4]=olho_luz[1];
	formiga[index_formiga].visao[7][4]=olho_luz[1];

	formiga[index_formiga].direcao=salva_direcao; // restaura a direcao inicial

	} else
	{
		for(x_atual=0;x_atual<8;x_atual++)
			for(y_atual=0;y_atual<5;y_atual++)
				formiga[index_formiga].visao[x_atual][y_atual]=0;
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  estabelece_tato                               //
//                                                                     //
// CARACTERISTICAS    :  estabelece o sentido de tato para a formiga   //
// atual                                                               //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  30/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void estabelece_tato(int index_formiga)
{
	int x_mais, y_mais, x_atual, y_atual, z_atual;

	if(formiga[index_formiga].no_ninho==false && formiga[index_formiga].vida==viva)
	{
	
	x_mais=devolve_x_mais(formiga[index_formiga].direcao);
	y_mais=devolve_y_mais(formiga[index_formiga].direcao);

	x_atual=formiga[index_formiga].coord_x;
	y_atual=formiga[index_formiga].coord_y;
	z_atual=formiga[index_formiga].coord_z;

	x_atual=x_atual+x_mais;
	y_atual=y_atual+y_mais;

	if(out_of_bounds(x_atual,y_atual,z_atual)) formiga[index_formiga].tato=margem; else
		formiga[index_formiga].tato=mundo.elemento[x_atual][y_atual][z_atual].paisagem;

	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  estabelece_olfato                             //
//                                                                     //
// CARACTERISTICAS    :  estabelece o sentido de olfato da formiga     //
// atual                                                               //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  30/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void estabelece_olfato(int index_formiga)
{
	int x_atual, y_atual, z_atual, x_anterior, y_anterior, z_anterior, i;

	x_atual=formiga[index_formiga].coord_x;
	y_atual=formiga[index_formiga].coord_y;
	z_atual=formiga[index_formiga].coord_z;

	// OBTER DADOS DO PONTO ATUAL

    formiga[index_formiga].olfato_alomonio[0]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_alomonio;
	formiga[index_formiga].olfato_feromonio[0]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_feromonio;
	formiga[index_formiga].olfato_ninho[0]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_ninho;
	formiga[index_formiga].olfato_alimento[0]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_alimento;

	if(formiga[index_formiga].no_ninho || formiga[index_formiga].vida!=viva)
	{
		for(i=0;i<5;i++)
		{
			formiga[index_formiga].olfato_alomonio[i]=0;
			formiga[index_formiga].olfato_feromonio[i]=0;
			formiga[index_formiga].olfato_ninho[i]=0;
			formiga[index_formiga].olfato_alimento[i]=0;
		}
	} else
	{

		// OBTER DADOS DO CANTO ESQUERDA A FRENTE

		vira_sentido_contra_horario(index_formiga); // canto esquerdo 45 graus a frente

		x_anterior=formiga[index_formiga].coord_x;
		y_anterior=formiga[index_formiga].coord_y;
		z_anterior=formiga[index_formiga].coord_z;

		ir_para_frente(index_formiga,true);

		x_atual=formiga[index_formiga].coord_x;
		y_atual=formiga[index_formiga].coord_y;
		z_atual=formiga[index_formiga].coord_z;

		if(x_anterior==x_atual && y_anterior==y_atual && z_anterior==z_atual) // posicao nao mudou, entao a formiga nao consegui movimento
		{																		// e o cheiro da posicao eh 0
			formiga[index_formiga].olfato_alomonio[2]=0;
			formiga[index_formiga].olfato_feromonio[2]=0;
			formiga[index_formiga].olfato_ninho[2]=0;
			formiga[index_formiga].olfato_alimento[2]=0;
		} else
		{
			formiga[index_formiga].olfato_alomonio[2]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_alomonio;
			formiga[index_formiga].olfato_feromonio[2]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_feromonio;
			formiga[index_formiga].olfato_ninho[2]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_ninho;
			formiga[index_formiga].olfato_alimento[2]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_alimento;

			ir_para_tras(index_formiga,true); // desfazer o movimento feito
		}

		// OBTER DADOS DO CAMPO A FRENTE

		vira_sentido_horario(index_formiga);

		x_anterior=formiga[index_formiga].coord_x;
		y_anterior=formiga[index_formiga].coord_y;
		z_anterior=formiga[index_formiga].coord_z;

		ir_para_frente(index_formiga,true);

		x_atual=formiga[index_formiga].coord_x;
		y_atual=formiga[index_formiga].coord_y;
		z_atual=formiga[index_formiga].coord_z;

		if(x_anterior==x_atual && y_anterior==y_atual && z_anterior==z_atual) // posicao nao mudou, entao a formiga nao consegui movimento
		{																		// e o cheiro da posicao eh 0
			formiga[index_formiga].olfato_alomonio[3]=0;
			formiga[index_formiga].olfato_feromonio[3]=0;
			formiga[index_formiga].olfato_ninho[3]=0;
			formiga[index_formiga].olfato_alimento[3]=0;
		} else
		{
			formiga[index_formiga].olfato_alomonio[3]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_alomonio;
			formiga[index_formiga].olfato_feromonio[3]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_feromonio;
			formiga[index_formiga].olfato_ninho[3]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_ninho;
			formiga[index_formiga].olfato_alimento[3]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_alimento;

			ir_para_tras(index_formiga,true); // desfazer o movimento feito
		}

		// OBTER DADOS DO CANTO DIREITA A FRENTE

		vira_sentido_horario(index_formiga);

		x_anterior=formiga[index_formiga].coord_x;
		y_anterior=formiga[index_formiga].coord_y;
		z_anterior=formiga[index_formiga].coord_z;

		ir_para_frente(index_formiga,true);

		x_atual=formiga[index_formiga].coord_x;
		y_atual=formiga[index_formiga].coord_y;
		z_atual=formiga[index_formiga].coord_z;

		if(x_anterior==x_atual && y_anterior==y_atual && z_anterior==z_atual) // posicao nao mudou, entao a formiga nao consegui movimento
		{																		// e o cheiro da posicao eh 0
			formiga[index_formiga].olfato_alomonio[4]=0;
			formiga[index_formiga].olfato_feromonio[4]=0;
			formiga[index_formiga].olfato_ninho[4]=0;
			formiga[index_formiga].olfato_alimento[4]=0;
		} else
		{
			formiga[index_formiga].olfato_alomonio[4]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_alomonio;
			formiga[index_formiga].olfato_feromonio[4]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_feromonio;
			formiga[index_formiga].olfato_ninho[4]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_ninho;
			formiga[index_formiga].olfato_alimento[4]=mundo.elemento[x_atual][y_atual][z_atual].cheiro_alimento;

			ir_para_tras(index_formiga,true); // desfazer o movimento feito
		}

		vira_sentido_contra_horario(index_formiga); // RESTAURAR POSICAO INICIAL!!!

	}

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  estabelece_audicao                            //
//                                                                     //
// CARACTERISTICAS    :  estabelece o sentido de audicao para a formiga//
// atual                                                               //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  30/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void estabelece_audicao(int index_formiga)
{
	int x_atual, y_atual, z_atual;

	if(formiga[index_formiga].no_ninho==false && formiga[index_formiga].vida==viva)
	{
	
	x_atual=formiga[index_formiga].coord_x;
	y_atual=formiga[index_formiga].coord_y;
	z_atual=formiga[index_formiga].coord_z;

	formiga[index_formiga].audicao=mundo.elemento[x_atual][y_atual][z_atual].barulho;

	} else
		formiga[index_formiga].audicao=0;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  obtenha_dados_7                        //
//                                                                     //
// CARACTERISTICAS    :  obtem os dados dos sensores da formiga atual  //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void obtenha_dados_sensores(int index_formiga)
{
	estabelece_visao(index_formiga);
	estabelece_tato(index_formiga);
	estabelece_olfato(index_formiga);
	estabelece_audicao(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  ajusta_cor                                    //
//                                                                     //
// CARACTERISTICAS    :  visualiza um numero usando cores              //
// monitoramento                                                       //
// PARAMETROS         :  numero                                        //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  30/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void ajusta_cor(int numero)
{
	if(numero<1) setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_BLACK ); else
		if(numero<26) setcolor( XFOREGROUND_YELLOW | XBACKGROUND_BLACK ); else
			if(numero<51) setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLACK ); else
				if(numero<76) setcolor( XFOREGROUND_LIGHTCYAN | XBACKGROUND_BLACK ); else
					setcolor( XFOREGROUND_LIGHTGREEN | XBACKGROUND_BLACK ); 
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  vira_sentido_contra_horario                   //
//                                                                     //
// CARACTERISTICAS    :  vira a formiga atual no sentido contra o      //
// horario                                                             //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  28/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void vira_sentido_contra_horario(int index_formiga)
{
	formiga[index_formiga].direcao=formiga[index_formiga].direcao-45;

	if(formiga[index_formiga].direcao==-45) formiga[index_formiga].direcao=315;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  vira_sentido_horario                          //
//                                                                     //
// CARACTERISTICAS    :  vira a formiga atual no sentido do horario    //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  28/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void vira_sentido_horario(int index_formiga)
{
	formiga[index_formiga].direcao=formiga[index_formiga].direcao+45;

	if(formiga[index_formiga].direcao==360) formiga[index_formiga].direcao=0;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  ajusta_altura                                 //
//                                                                     //
// CARACTERISTICAS    :  ajusta automaticamente a altura da formiga    //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  desta forma a formiga sempre se movimenta de  //
// uma forma pseudo-horizontal que porem faz sentido no ambiente por   //
// duas razoes: 1. o ambiente apesar de ser 3D eh baixo e formigas     //
// conseguem subir sem problemas, ou seja, ficar grudada na parede nao //
// faz sentido 2. a visualizacao de cima certamente nao mudaria muito  //
// ULTIMA ATUALIZACAO :  28/09/03                                      //
/////////////////////////////////////////////////////////////////////////

int ajusta_altura(int form_x, int form_y)
{
	int altura, form_z, calc;

	altura=0;  // se a condicao abaixo nao pegar significa que estamos em altura 0 (uma checagem
	           // produziria um crash!!)

	for(calc=4;calc>0;calc--)
		if(mundo.elemento[form_x][form_y][calc].paisagem==vazio &&
			mundo.elemento[form_x][form_y][calc-1].paisagem!=vazio)
			altura=calc;

	form_z=altura;

	if(mundo.elemento[form_x][form_y][form_z].paisagem!=vazio) printf("ERRO DE ALTURA!!!");

	return altura;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  devolve_x_mais                                //
//                                                                     //
// CARACTERISTICAS    :  fornece o delta(x) do angulo atual            //
// possivel                                                            //
// PARAMETROS         :  angulo                                        //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  29/09/03                                      //
/////////////////////////////////////////////////////////////////////////

int devolve_x_mais(int direcao_local)
{
	int flag; 

	switch(direcao_local)
	{
	case 0: flag=0; break;
	case 45: flag=1; break;
	case 90: flag=1; break;
	case 135: flag=1; break;
	case 180: flag=0; break;
	case 225: flag=-1; break;
	case 270: flag=-1; break;
	case 315: flag=-1; break;
	}

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  devolve_y_mais                                //
//                                                                     //
// CARACTERISTICAS    :  fornece o delta(y) do angulo atual            //
// possivel                                                            //
// PARAMETROS         :  angulo                                        //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  29/09/03                                      //
/////////////////////////////////////////////////////////////////////////

int devolve_y_mais(int direcao_local)
{
	int flag;

	switch(direcao_local)
	{
	case 0: flag=-1; break;
	case 45: flag=-1; break;
	case 90: flag=0; break;
	case 135: flag=1; break;
	case 180: flag=1; break;
	case 225: flag=1; break;
	case 270: flag=0; break;
	case 315: flag=-1; break;
	}

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  posicao_disponivel                            //
//                                                                     //
// CARACTERISTICAS    :  retorna se ha uma posicao disponivel para     //
// entrar                                                              //
// PARAMETROS         :  coordenada x e coordenada y                   //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  12/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool posicao_disponivel(int x, int y)
{
	bool flag;
	int z, pos_x, pos_y, pos_z;

	flag=false;

	for(z=0;z<max_mundo_z;z++)
	{
		if(mundo.elemento[x][y][z].paisagem==vazio) 
			flag=true;
		if(mundo.elemento[x][y][z].paisagem==outra)
			return false;
	}
	
	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  ir_para_frente                                //
//                                                                     //
// CARACTERISTICAS    :  movimenta a formiga atual para a frente caso  //
// possivel                                                            //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  29/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void ir_para_frente(int index_formiga, bool hipotetico)
{
	// ATENCAO - CONVENCAO: 0 Graus significa virada para o norte

	int x_plus, y_plus, x_novo, y_novo, x_atual, y_atual, z_atual, z_novo;

	x_plus=devolve_x_mais(formiga[index_formiga].direcao);
	y_plus=devolve_y_mais(formiga[index_formiga].direcao);

	mundo.elemento[formiga[index_formiga].coord_x][formiga[index_formiga].coord_y][formiga[index_formiga].coord_z].paisagem=vazio;

	x_novo=formiga[index_formiga].coord_x+x_plus;
	y_novo=formiga[index_formiga].coord_y+y_plus;

	if(x_novo>=0 && x_novo<max_mundo_x && y_novo>=0 && y_novo<max_mundo_y)
		if(posicao_disponivel(x_novo,y_novo))
	{
		z_novo=ajusta_altura(x_novo,y_novo);

		formiga[index_formiga].coord_x=x_novo;
		formiga[index_formiga].coord_y=y_novo;
		formiga[index_formiga].coord_z=z_novo;

		x_atual=formiga[index_formiga].coord_x;
		y_atual=formiga[index_formiga].coord_y;
		z_atual=formiga[index_formiga].coord_z;

		// gravar valores historicos

		if(hipotetico==false)
		{

		formiga[index_formiga].olfato_alomonio[1]=formiga[index_formiga].olfato_alomonio[0];
		formiga[index_formiga].olfato_feromonio[1]=formiga[index_formiga].olfato_feromonio[0];
		formiga[index_formiga].olfato_ninho[1]=formiga[index_formiga].olfato_ninho[0];
		formiga[index_formiga].olfato_alimento[1]=formiga[index_formiga].olfato_alimento[0];

		}

	}

	mundo.elemento[formiga[index_formiga].coord_x][formiga[index_formiga].coord_y][formiga[index_formiga].coord_z].paisagem=outra;

	if(formiga[index_formiga].estomago_social>=10) solta_feromonio(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  ir_para_tras                                  //
//                                                                     //
// CARACTERISTICAS    :  vai para tras com a formiga atual se possivel //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  29/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void ir_para_tras(int index_formiga, bool hipotetico)
{
	int x_plus, y_plus, x_novo, y_novo, z_novo, x_atual, y_atual, z_atual;

	x_plus=-(devolve_x_mais(formiga[index_formiga].direcao));
	y_plus=-(devolve_y_mais(formiga[index_formiga].direcao));

	x_novo=formiga[index_formiga].coord_x+x_plus;
	y_novo=formiga[index_formiga].coord_y+y_plus;

	mundo.elemento[formiga[index_formiga].coord_x][formiga[index_formiga].coord_y][formiga[index_formiga].coord_z].paisagem=vazio;

	if(x_novo>=0 && x_novo<max_mundo_x && y_novo>=0 && y_novo<max_mundo_y)
		if(posicao_disponivel(x_novo,y_novo))
	{
		
		z_novo=ajusta_altura(x_novo,y_novo);

		formiga[index_formiga].coord_x=x_novo;
		formiga[index_formiga].coord_y=y_novo;
		formiga[index_formiga].coord_z=z_novo;
		
		x_atual=formiga[index_formiga].coord_x;
		y_atual=formiga[index_formiga].coord_y;
		z_atual=formiga[index_formiga].coord_z;

		// gravar valores historicos

		if(hipotetico==false)
		{

		formiga[index_formiga].olfato_alomonio[1]=formiga[index_formiga].olfato_alomonio[0];
		formiga[index_formiga].olfato_feromonio[1]=formiga[index_formiga].olfato_feromonio[0];
		formiga[index_formiga].olfato_ninho[1]=formiga[index_formiga].olfato_ninho[0];
		formiga[index_formiga].olfato_alimento[1]=formiga[index_formiga].olfato_alimento[0];

		}

	}

	mundo.elemento[formiga[index_formiga].coord_x][formiga[index_formiga].coord_y][formiga[index_formiga].coord_z].paisagem=outra;

	if(formiga[index_formiga].estomago_social>=10) solta_feromonio(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  mostra_interpretacao_visao                    //
//                                                                     //
// CARACTERISTICAS    :  interpreta os codigos de visao para fins de   //
// visualizacao no modo debug                                          //
// PARAMETROS         :  codigo                                        //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  12/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void mostra_interpretacao_visao(int codigo)
{
	switch(codigo)
	{
	case sem_presenca:      setcolor( XBACKGROUND_BLACK | XFOREGROUND_DARKGRAY );
							printf(" ----------------- "); break;

	case longe:             setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTGRAY );
							printf("       LONGE       "); break;

	case perto:             setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTGRAY );
							printf("       PERTO       "); break;

	case muito_perto:       setcolor( XBACKGROUND_BLACK | XFOREGROUND_WHITE );
							printf("    MUITO PERTO    "); break;

	case muito_perto_baixo: setcolor( XBACKGROUND_BLACK | XFOREGROUND_WHITE );
							printf(" MUITO PERTO BAIXO "); break;

	case muito_perto_alto:  setcolor( XBACKGROUND_BLACK | XFOREGROUND_WHITE );
							printf(" MUITO PERTO ALTO  "); break;
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  explica_caso                                  //
//                                                                     //
// CARACTERISTICAS    :  mostra a decisao consequente dos algoritmos   //
//                                                                     //
// PARAMETROS         :  posicao y da linha e o codigo                 //
// OBSERVACOES        :  para poder usar com os dois algoritmos        //
//                                                                     //
// ULTIMA ATUALIZACAO :  18/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void explica_caso(int y_pos, int codigo)
{
	switch(codigo)
	{
	case 1:  center(y_pos,"FUGIREI DO PERIGO"); break;
	case 2:  center(y_pos,"VOLTAREI A NORMALIDADE"); break;
	case 3:  center(y_pos,"EVITAREI UMA COLISAO"); break;
	case 4:  center(y_pos,"PEGAREI ALIMENTO"); break;
	case 5:  center(y_pos,"SAIREI DA ZONA DE ALERTA"); break;
	case 6:  center(y_pos,"SAIREI DE UM CAMINHO NAO PROVEITOSO"); break;
	case 7:  center(y_pos,"ENTRAREI NO NINHO"); break;
	case 8:  center(y_pos,"SEGUIREI UM PONTO DE ATRACAO A FRENTE"); break;
	case 9:  center(y_pos,"BUSCAREI SINAIS PARA ORIENTACAO"); break;
	case 10: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO AMBIGUO"); break;
	case 11: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO A DIREITA"); break;
	case 12: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO A ESQUERDA"); break;
	case 99: center(y_pos,"BUSCAREI SINAIS MAIS FORTES"); break;
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  mostra_sentidos                               //
//                                                                     //
// CARACTERISTICAS    :  mostra todos os sentidos da formiga na tela de//
// monitoramento                                                       //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  28/09/03                                      //
/////////////////////////////////////////////////////////////////////////

void mostra_sentidos(int index_formiga)
{
	int i, j, x_pos, y_pos, v_x, v_y, temp;

	DEBUG_DOZE_IFS d12;

	// visao

	// "espelho"

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	gotoxy(30,3);

	printf(" FORMIGA ATUAL: %i",index_formiga);

	for(i=0;i<8;i++)
		for(j=0;j<4;j++)
		{
			x_pos=i*3+8; 
			if(i>3) x_pos--;

			y_pos=j+9;
			if(i==0 || i==7) y_pos++;
			if(i<2 || i>5) y_pos++;

			gotoxy(x_pos,y_pos);

			switch(formiga[index_formiga].visao[i][j])
			{
				case muito_perto: setcolor( XFOREGROUND_DARKGRAY | XBACKGROUND_BLACK ); break;
				case perto: setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_DARKGRAY ); break;
				case longe: setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY ); break;
				case sem_presenca : setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_WHITE ); break;
			}

			printf("=%i",formiga[index_formiga].visao[i][j]);
		}

	// vista para cima

	if(formiga[index_formiga].visao[0][4]==sem_presenca)
		setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_WHITE ); else
		setcolor( XFOREGROUND_DARKGRAY | XBACKGROUND_BLACK );
	
	gotoxy(9,8);

	printf("=%i=",formiga[index_formiga].visao[0][4]);

	gotoxy(26,8);

	printf("=%i=",formiga[index_formiga].visao[0][4]);

	// olhos de luz

	switch(formiga[index_formiga].visao[4][4])
	{
	case intenso: setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_YELLOW ); break;
	case medio: setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_RED ); break;
	case fraco: setcolor( XFOREGROUND_RED | XBACKGROUND_BLACK ); break;
	}

	gotoxy(5,7);

	printf("=%i=",formiga[index_formiga].visao[4][4]);

	switch(formiga[index_formiga].visao[6][4])
	{
	case intenso: setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_YELLOW ); break;
	case medio: setcolor( XFOREGROUND_LIGHTRED | XBACKGROUND_RED ); break;
	case fraco: setcolor( XFOREGROUND_RED | XBACKGROUND_BLACK ); break;
	}

	gotoxy(30,7);

	printf("=%i=",formiga[index_formiga].visao[6][4]);

	// olfato
	
	// alomonio

	gotoxy(50,7);

	ajusta_cor(formiga[index_formiga].olfato_alomonio[2]);		// canto esquerdo frente
	escreve_numero(formiga[index_formiga].olfato_alomonio[2]);

	gotoxy(53,7);

	ajusta_cor(formiga[index_formiga].olfato_alomonio[3]);		// frente
	escreve_numero(formiga[index_formiga].olfato_alomonio[3]);

	gotoxy(56,7);

	ajusta_cor(formiga[index_formiga].olfato_alomonio[4]);
	escreve_numero(formiga[index_formiga].olfato_alomonio[4]);	// canto direito frente

	gotoxy(53,9);

	ajusta_cor(formiga[index_formiga].olfato_alomonio[0]);		// atual
	escreve_numero(formiga[index_formiga].olfato_alomonio[0]);

	gotoxy(57,9);

	ajusta_cor(formiga[index_formiga].olfato_alomonio[1]);
	escreve_numero(formiga[index_formiga].olfato_alomonio[1]);	// historico

	// feromonio

	gotoxy(69,7);

	ajusta_cor(formiga[index_formiga].olfato_feromonio[2]);	// canto esquerdo frente
	escreve_numero(formiga[index_formiga].olfato_feromonio[2]);

	gotoxy(72,7);

	ajusta_cor(formiga[index_formiga].olfato_feromonio[3]);	// frente
	escreve_numero(formiga[index_formiga].olfato_feromonio[3]);

	gotoxy(75,7);

	ajusta_cor(formiga[index_formiga].olfato_feromonio[4]);
	escreve_numero(formiga[index_formiga].olfato_feromonio[4]);	// canto direito frente

	gotoxy(72,9);

	ajusta_cor(formiga[index_formiga].olfato_feromonio[0]);	// atual
	escreve_numero(formiga[index_formiga].olfato_feromonio[0]);

	gotoxy(76,9);

	ajusta_cor(formiga[index_formiga].olfato_feromonio[1]);
	escreve_numero(formiga[index_formiga].olfato_feromonio[1]);	// historico

	// ninho

	gotoxy(50,11);

	ajusta_cor(formiga[index_formiga].olfato_ninho[2]);	// canto esquerdo frente
	escreve_numero(formiga[index_formiga].olfato_ninho[2]);

	gotoxy(53,11);

	ajusta_cor(formiga[index_formiga].olfato_ninho[3]);	// frente
	escreve_numero(formiga[index_formiga].olfato_ninho[3]);

	gotoxy(56,11);

	ajusta_cor(formiga[index_formiga].olfato_ninho[4]);
	escreve_numero(formiga[index_formiga].olfato_ninho[4]);	// canto direito frente

	gotoxy(53,13);

	ajusta_cor(formiga[index_formiga].olfato_ninho[0]);	// atual
	escreve_numero(formiga[index_formiga].olfato_ninho[0]);

	gotoxy(57,13);

	ajusta_cor(formiga[index_formiga].olfato_ninho[1]);
	escreve_numero(formiga[index_formiga].olfato_ninho[1]);	// historico

	// alimento
	
	gotoxy(69,11);

	ajusta_cor(formiga[index_formiga].olfato_alimento[2]);	// canto esquerdo frente
	escreve_numero(formiga[index_formiga].olfato_alimento[2]);
	
	gotoxy(72,11);

	ajusta_cor(formiga[index_formiga].olfato_alimento[3]);	// frente
	escreve_numero(formiga[index_formiga].olfato_alimento[3]);
	
	gotoxy(75,11);

	ajusta_cor(formiga[index_formiga].olfato_alimento[4]);
	escreve_numero(formiga[index_formiga].olfato_alimento[4]);	// canto direito frente

	gotoxy(72,13);

	ajusta_cor(formiga[index_formiga].olfato_alimento[0]);	// atual
	escreve_numero(formiga[index_formiga].olfato_alimento[0]);
	
	gotoxy(76,13);

	ajusta_cor(formiga[index_formiga].olfato_alimento[1]);
	escreve_numero(formiga[index_formiga].olfato_alimento[1]);	// historico

	// tato

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLACK );

	gotoxy(18,17);

	switch(formiga[index_formiga].tato)
	{
	case vazio    : setcolor( XFOREGROUND_DARKGRAY | XBACKGROUND_BLACK);
		            printf(" -------- "); break;
	case ninho    : printf("  NINHO   "); break;
	case parede   : printf("  PAREDE  "); break;
	case terra    : printf("  TERRA   "); break;
	case agua     : printf("   AGUA   "); break;
	case predador : printf(" PREDADOR "); break;
	case margem   : printf(" (MARGEM) "); break;
	case outra    : printf("  FORMIGA "); break;
	}

	if(formiga[index_formiga].tato>=comida0)
		printf(" COMIDA %i  ",formiga[index_formiga].tato-comida0);

	// audicao

	gotoxy(66,17);

	ajusta_cor(formiga[index_formiga].audicao);
	escreve_numero(formiga[index_formiga].audicao);

	// coordenadas

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLACK );

	gotoxy(9,21);

	escreve_numero(formiga[index_formiga].coord_x);

	gotoxy(16,21);

	escreve_numero(formiga[index_formiga].coord_y);

	gotoxy(23,21);

	escreve_numero(formiga[index_formiga].coord_z);

	gotoxy(32,21);

	if(formiga[index_formiga].direcao<10) 
		printf("00%i",formiga[index_formiga].direcao); else
		if(formiga[index_formiga].direcao<100)
			printf("0%i",formiga[index_formiga].direcao); else
			printf("%i",formiga[index_formiga].direcao);

	// estomago

	gotoxy(57,21);

	ajusta_cor(formiga[index_formiga].estomago_social);
	escreve_numero(formiga[index_formiga].estomago_social);

	gotoxy(70,21);

	ajusta_cor(formiga[index_formiga].estomago);
	escreve_numero(formiga[index_formiga].estomago);

	d12=doze_ifs(index_formiga,true);

	setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTGRAY );

	gotoxy(3,43);

	escreve_numero(d12.caso);

	gotoxy(3,44);

	escreve_numero(d12.linha);

	setcolor( XBACKGROUND_BLACK | XFOREGROUND_WHITE );
	
	center(43,"                                                                   ");

	if(formiga[index_formiga].vida==viva && formiga[index_formiga].no_ninho==false)
	{
		explica_caso(43,d12.caso);
	} else
	{
		gotoxy(3,43);

		escreve_numero(0);

		gotoxy(3,44);

		escreve_numero(0);

		if(formiga[index_formiga].vida!=viva)
			center(43,"( + + + + + + + + + + + )"); else
			center(43,"(ZZ ZZ ZZ ZZ ZZ)");
	}

	center(44,"                                                                   ");

	if(formiga[index_formiga].vida==viva && formiga[index_formiga].no_ninho==false)
	{
	
	switch(d12.linha)
	{
	case 0:  center(44,"POIS NAO HA OUTRO CASO QUE SE APLICA"); break;
	case 1:  center(44,"POIS HA UM PREDADOR A FRENTE (TATO)"); break;
	case 2:  center(44,"POIS HA UM OBJETO ACIMA DE MIM"); break;
	case 3:  center(44,"POIS HA CHEIRO DE ALOMONIO AUMENTANDO"); break;
	case 4:  center(44,"POIS HA MUITO BARULHO AQUI"); break;
	case 5:  center(44,"POIS NAO HA MAIS PERIGO APARENTE"); break;
	case 6:  center(44,"POIS HA UMA OUTRA FORMIGA NA MINHA FRENTE"); break;
	case 7:  center(44,"POIS HA AGUA A FRENTE E AINDA TENHO ESPACO"); break;
	case 8:  center(44,"POIS HA COMIDA A FRENTE E AINDA TENHO ESPACO"); break;
	case 9:  center(44,"POIS O CHEIRO DE ALOMONIO ESTA DIMINUINDO"); break;
	case 10: center(44,"POIS HA UMA MARGEM DO MUNDO A FRENTE"); break;
	case 11: center(44,"POIS HA UM OBJETO ALTO DEMAIS A FRENTE"); break;
	case 12: center(44,"POIS O FEROMONIO ESTA DIMINUINDO"); break;
	case 13: center(44,"POIS O CHEIRO DO NINHO ESTA DIMINUINDO E ESTOU COM FOME"); break;
	case 14: center(44,"POIS O CHEIRO DO NINHO ESTA DIMINUINDO E ESTOU COM ALIMENTO"); break;
	case 15: center(44,"POIS O CHEIRO DE ALIMENTO ESTA DIMINUINDO E EU POSSO LEVAR"); break;
	case 16: center(44,"POIS O NINHO ESTA A FRENTE E ESTOU COM FOME"); break;
	case 17: center(44,"POIS O NINHO ESTA A FRENTE E ESTOU COM ALIMENTO"); break;
	case 18: center(44,"POIS ESTOU VENDO UM OBJETO A FRENTE LONGE"); break;
	case 19: center(44,"POIS ESTOU VENDO UM OBJETO A FRENTE PERTO"); break;
	case 20: center(44,"POIS ESTOU VENDO UM OBJETO A FRENTE MUITO PERTO E NAO CONHECO"); break;
	case 21: center(44,"POIS HA UMA PAREDE BAIXA A FRENTE"); break;
	case 22: center(44,"POIS HA UMA ELEVACAO DE TERRA BAIXA A FRENTE"); break;
	case 23: center(44,"POIS O CHEIRO DE FEROMONIO ESTA AUMENTANDO"); break;
	case 24: center(44,"POIS O CHEIRO DO NINHO ESTA AUMENTANDO E ESTOU COM FOME"); break;
	case 25: center(44,"POIS O CHEIRO DO NINHO ESTA AUMENTANDO E TENHO ALIMENTO"); break;
	case 26: center(44,"POIS O CHEIRO DE ALIMENTO ESTA AUMENTANDO E POSSO PEGAR"); break;
	case 27: center(44,"POIS NAO HA SINAL ALGUM"); break;
	case 28: center(44,"POIS HA DOIS OBJETOS LONGE"); break;
	case 29: center(44,"POIS HA DOIS OBJETOS PERTO"); break;
	case 30: center(44,"POIS HA DOIS OBJETOS MUITO PERTO"); break;
	case 31: center(44,"POIS HA UM OBJETO A DIREITA LONGE"); break;
	case 32: center(44,"POIS HA UM OBJETO A DIREITA PERTO"); break;
	case 33: center(44,"POIS HA UM OBJETO A DIREITA MUITO PERTO"); break;
	case 34: center(44,"POIS HA UM OBJETO A ESQUERDA LONGE"); break;
	case 35: center(44,"POIS HA UM OBJETO A ESQUERDA PERTO"); break;
	case 36: center(44,"POIS HA UM OBJETO A ESQUERDA MUITO PERTO"); break;
	}

	} else
		if(formiga[index_formiga].vida!=viva)
			center(44,"( + + + + + + + + + + + )"); else
			center(44,"(VERIFIQUE VALOR DA VARIAVEL \"DESCANSO - ITERACOES\")");

	center(48,"                                                                   ");

	gotoxy(16,25);

	temp=objeto_frente(index_formiga);

	mostra_interpretacao_visao(temp);

	gotoxy(16,26);

	temp=objeto_esquerda(index_formiga);

	mostra_interpretacao_visao(temp);

	gotoxy(16,27);

	temp=objeto_direita(index_formiga);

	mostra_interpretacao_visao(temp);

	gotoxy(16,28);

	temp=objeto_acima(index_formiga);

	mostra_interpretacao_visao(temp);

	gotoxy(16,29);

	temp=objeto_abaixo(index_formiga);

	mostra_interpretacao_visao(temp);

	gotoxy(54,25);

	switch(formiga[index_formiga].alerta)
	{
	case true:	setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTRED );
				printf("  LIGADO   "); break;

	case false: setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTGREEN );
				printf(" DESLIGADO "); break;
	}

	gotoxy(51,29);

	switch(formiga[index_formiga].vida)
	{
	case viva:	setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTGREEN );
				printf("       VIVA       "); break;
	case esmagada: setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTRED );
				printf(" MORTA (ESMAGADA) "); break;
	case comida: setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTRED );
				printf("  MORTA (COMIDA)  "); break;
	case fome: setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTRED );
				printf("   MORTA (FOME)   "); break;
	case afogada: setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTRED );
				printf("  MORTA (AFOGADA) "); break;
	}

	for(i=-3;i<4;i++)
		for(j=-3;j<4;j++)
		{
			v_x=formiga[index_formiga].coord_x+i;
			v_y=formiga[index_formiga].coord_y+j;

			if(out_of_bounds(v_x,v_y,0))
			{
				setcolor( XBACKGROUND_BLUE | XFOREGROUND_WHITE );
				gotoxy(19+i,36+j);
				printf(" ");
			} else
				if(i!=0 || j!=0)
					visualiza_ponto(v_x,v_y,4,sem_formiga,19+i,36+j);
		}

	gotoxy(19,36);

	imprima_formiga(index_formiga);

	gotoxy(52,33);

	switch(formiga[index_formiga].no_ninho)
	{
	case false:	setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTGREEN );
				printf(" FORA DO NINHO  "); break;

	case true: setcolor( XBACKGROUND_BLACK | XFOREGROUND_LIGHTRED );
				printf("    NO NINHO    "); break;
	}

	gotoxy(65,37);

	ajusta_cor(formiga[index_formiga].descanso);
	escreve_numero(formiga[index_formiga].descanso);

	home();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  pode                                          //
//                                                                     //
// CARACTERISTICAS    :  devolve, se a formiga pode se movimentar ou   //
// nao                                                                 //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  apenas para fins de simplificacao do codigo   //
//                                                                     //
// ULTIMA ATUALIZACAO :  12/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool pode(int index_formiga)
{
	bool flag;

	flag=true;

	if(formiga[index_formiga].no_ninho || formiga[index_formiga].vida!=viva)
		flag=false;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  simulacao_testar_sensores                     //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void simulacao_testar_sensores(int index_formiga)
{
	char movimento;

	int i;

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	center(1," VISUALIZACAO DETALHADA DA FORMIGA ATUAL ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	gotoxy(16,5);

	printf(" VISAO ");

	gotoxy(55,5);

	printf(" OLFATO ");

	gotoxy(16,15);

	printf(" TATO ");

	gotoxy(55,15);

	printf(" AUDICAO ");

	gotoxy(13,19);

	printf(" COORDENADAS ");

	gotoxy(55,19);

	printf(" ESTOMAGO ");

	gotoxy(8,23);

	printf(" INTERPRETADOR VISUAL ");

	gotoxy(51,23);

	printf(" ESTADO DE ALERTA ");

	gotoxy(52,27);

	printf(" ESTADO DE VIDA ");

	gotoxy(14,31);

	printf(" MINI MAPA ");

	gotoxy(51,31);

	printf(" MODO DE OPERACAO ");

	gotoxy(55,35);

	printf(" DESCANSO ");

	center(41," ANALISE DOZE IFS (ALGORITMO SIMBOLICO) ");

	center(46," ANALISE COM A REDE (ALGORITMO CONEXIONISTA) ");

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	gotoxy(53,37);

	printf("iteracoes = //");

	gotoxy(5,25);

	printf("frente   = .................");

	gotoxy(5,26);

	printf("esquerda = .................");
	
	gotoxy(5,27);

	printf("direita  = .................");

	gotoxy(5,28);

	printf("acima    = .................");

	gotoxy(5,29);

	printf("p/ baixo = .................");

	gotoxy(5,21);

	printf("x = // y = // z = // dir = //");

	gotoxy(48,21);

	printf("social = // pessoal = //");

	gotoxy(11,17);

	printf("tipo = //////////");

	gotoxy(52,17);

	printf("intensidade = /");

	gotoxy(42,7);

	printf("alom. = //=//=//  ferom. = //=//=//");

	gotoxy(42,8);

	printf("           ||                 ||");

	gotoxy(42,9);

	printf("           // (//)            // (//)");

	gotoxy(42,11);

	printf("ninho = //=//=//  alim.  = //=//=//");

	gotoxy(42,12);

	printf("           ||                 ||");

	gotoxy(42,13);

	printf("           // (//)            // (//)");

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLACK );

	home();

	//getch();	// DEBUG DA TELA

	obtenha_dados_sensores(index_formiga);

	mostra_sentidos(index_formiga);
	
	movimento=' ';

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	center(51," TECLAS ");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLUE );

	center(53,"[CurUp] PARA FRENTE, [CurDown] PARA TRAS, [CurRight] VIRA SENTIDO HORARIO,");
	center(54,"[CurLeft] VIRA SENTIDO CONTRA HORARIO, [a] SOLTA ALOMONIO, [f] FEROMONIO,");
	center(55,"[l] MUDA ALERTA, [s] SAIA DO NINHO, [v] MUDA VIDA, [g] FUGIR PERIGO,");
	center(56,"[n] VOLTAR NORMALIDADE, [o] EVITAR COLISAO, [c] PEGAR COMIDA, [i] SAIR ALERTA,");
	center(57,"[r] SAIR CAMINHO RUIM, [e] ENTRAR NINHO, [t] ATRACAO FRENTE, [b] BUSCAR SINAL,");
	center(58,"[u] ATRACAO AMBIGUA, [d] ATRACAO DIREITA, [q] ATRACAO ESQUERDA, [esc] SAIR");

	while(movimento!=char(27))
	{
		movimento=getch();

		switch(movimento)
		{
		case char(77): if(pode(index_formiga)) vira_sentido_horario(index_formiga); break;
		case char(75): if(pode(index_formiga)) vira_sentido_contra_horario(index_formiga); break;
		case char(72): if(pode(index_formiga)) ir_para_frente(index_formiga,false); break;
		case char(80): if(pode(index_formiga)) ir_para_tras(index_formiga,false); break;
		case 'a': if(pode(index_formiga)) solta_alomonio(index_formiga); break;
		case 'f': if(pode(index_formiga)) solta_feromonio(index_formiga); break;
		case 'l': if(pode(index_formiga)) formiga[index_formiga].alerta=!formiga[index_formiga].alerta; break;
		case 's': if(formiga[index_formiga].no_ninho) saia_do_ninho(index_formiga); break;
		case 'v': formiga[index_formiga].vida++; 
					if(formiga[index_formiga].vida>afogada) formiga[index_formiga].vida=viva; break;
		case 'g': if(pode(index_formiga)) fugir_perigo(index_formiga,true); break;
		case 'n': if(pode(index_formiga)) voltar_normalidade(index_formiga); break;
		case 'o': if(pode(index_formiga)) evitar_colisao(index_formiga); break;
		case 'c': if(pode(index_formiga)) pegar_comida(index_formiga); break;
		case 'i': if(pode(index_formiga)) sair_alerta(index_formiga); break;
		case 'r': if(pode(index_formiga)) sair_caminho_ruim(index_formiga); break;
		case 'e': if(pode(index_formiga)) entrar_ninho(index_formiga,false); break;
		case 't': if(pode(index_formiga)) seguir_atracao_frente(index_formiga); break;
		case 'b': if(pode(index_formiga)) buscar_sinal(index_formiga); break;
		case 'u': if(pode(index_formiga)) seguir_atracao_ambigua(index_formiga); break;
		case 'd': if(pode(index_formiga)) seguir_atracao_direita(index_formiga); break;
		case 'q': if(pode(index_formiga)) seguir_atracao_esquerda(index_formiga); break;
		}

		obtenha_dados_sensores(index_formiga);

		mostra_sentidos(index_formiga);
	}

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLACK );
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  parametros_padrao                             //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void parametros_padrao()
{
	parametros.modo_neural=false;
	parametros.numero_formigas=20;
	parametros.neuronios_escondidos=20;
	parametros.trace_default=false;
	parametros.limite_simulacao=0;
	parametros.degradacao_feromonio=20;
	parametros.degradacao_alomonio=50;
	parametros.alcance_feromonio=2;
	parametros.alcance_alomonio=6;
	parametros.alcance_cheiro_ninho=40;
	parametros.alcance_cheiro_alimento=15;
	parametros.iteracoes_delay=0;
	parametros.atualizacao_cheiro_alimentos=true;
	parametros.aprendizado_nao_supervisionado=false;
	parametros.gasto_energia=50;
	parametros.visual=v_normal;
}

void mascara_parametros()
{
	int i;

	setcolor( XBACKGROUND_LIGHTGRAY | XFOREGROUND_BLACK );
	
	janela(10,20,60,16);
	
	center(20,"PARAMETROS DA SIMULACAO");

	gotoxy(12,22);
	printf("(A) Mode de Operacao---------->");

	gotoxy(12,23);
	printf("(B) Numero de Formigas-------->");

	gotoxy(12,24);
	printf("(C) Neuronios Escondidos------>");

	gotoxy(12,25);
	printf("(D) Trace Default------------->");

	gotoxy(12,26);
	printf("(E) Limite Simulacao---------->");

	gotoxy(12,27);
	printf("(F) Degradacao Feromonio------>");

	gotoxy(12,28);
	printf("(G) Degradacao Alomonio------->");

	gotoxy(12,29);
	printf("(H) Alcance Feromonio--------->");

	gotoxy(12,30);
	printf("(I) Alcance Alomonio---------->");

	gotoxy(12,31);
	printf("(J) Alcance Cheiro Ninho------>");

	gotoxy(12,32);
	printf("(K) Alcance Cheiro Alimentos-->");

	gotoxy(12,33);
	printf("(L) Delay de Simulacao-------->");

	gotoxy(12,34);
	printf("(M) Atual. Cheiro Alimentos--->");

	gotoxy(12,35);
	printf("(N) Apr. Nao Supervisionado--->");

	gotoxy(12,36);
	printf("(O) Gasto Energia------------->");

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY );

	for(i=22;i<37;i++)
	{
		gotoxy(43,i);
		printf("                          ");
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  mostra_parametros                             //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void mostra_parametros()
{
	setcolor( XFOREGROUND_YELLOW | XBACKGROUND_DARKGRAY );
	gotoxy(43,22);
	if(parametros.modo_neural)
		printf("REDE NEURAL         "); else
		printf("ALGORITMO SIMBOLICO ");

	gotoxy(43,23);
	printf("%i  ",parametros.numero_formigas);

	gotoxy(43,24);
	printf("%i  ",parametros.neuronios_escondidos);

	gotoxy(43,25);
	if(parametros.trace_default)
		printf("LIGADO    "); else
		printf("DESLIGADO ");

	gotoxy(43,26);
	if(parametros.limite_simulacao==0)
		printf("SEM LIMITE "); else
		printf("%i       ",parametros.limite_simulacao);

	gotoxy(43,27);
	if(parametros.degradacao_feromonio==0)
		printf("SEM ATUALIZACAO "); else
		printf("CADA %i         ",parametros.degradacao_feromonio);

	gotoxy(43,28);
	if(parametros.degradacao_alomonio==0)
		printf("SEM ATUALIZACAO "); else
		printf("CADA %i         ",parametros.degradacao_alomonio);

	gotoxy(43,29);
	if(parametros.alcance_feromonio==0)
		printf("NENHUM "); else
		printf("%i       ",parametros.alcance_feromonio);

	gotoxy(43,30);
	if(parametros.alcance_alomonio==0)
		printf("NENHUM "); else
		printf("%i       ",parametros.alcance_alomonio);

	gotoxy(43,31);
	if(parametros.alcance_cheiro_ninho==0)
		printf("NENHUM "); else
		printf("%i       ",parametros.alcance_cheiro_ninho);

	gotoxy(43,32);
	if(parametros.alcance_cheiro_alimento==0)
		printf("NENHUM "); else
		printf("%i       ",parametros.alcance_cheiro_alimento);

	gotoxy(43,33);
	printf("%i       ",parametros.iteracoes_delay);

	gotoxy(43,34);
	if(parametros.atualizacao_cheiro_alimentos) 
		printf("LIGADO    "); else
		printf("DESLIGADO ");

	gotoxy(43,35);
	if(parametros.aprendizado_nao_supervisionado) 
		printf("LIGADO    "); else
		printf("DESLIGADO ");

	gotoxy(43,36);
	if(parametros.gasto_energia==0)
		printf("SEM ATUALIZACAO "); else
		printf("CADA %i         ",parametros.gasto_energia);

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  simulacao_parametros                          //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void simulacao_parametros()
{
	int nro_anterior_formigas;

	nro_anterior_formigas=parametros.numero_formigas;

	x_cursor=0;
	y_cursor=0;

	cls();
	init_menu_parametros();
	atualiza_cabecalho();

	mascara_parametros();
	mostra_parametros();

	while(codigo_extraido!="VL" && codigo_extraido!="ZZ")
	{
		executa_menu();

		if(codigo_extraido=="A1") parametros.modo_neural=false;	// modo de operacao (A)
		if(codigo_extraido=="A2") parametros.modo_neural=true;

		if(codigo_extraido=="B1") parametros.numero_formigas=1;	// numero de formigas (B)
		if(codigo_extraido=="B2") parametros.numero_formigas=3;
		if(codigo_extraido=="B3") parametros.numero_formigas=10;
		if(codigo_extraido=="B4") parametros.numero_formigas=20;
		if(codigo_extraido=="B5") parametros.numero_formigas=30;
		if(codigo_extraido=="B6") parametros.numero_formigas=40;
		if(codigo_extraido=="B7") parametros.numero_formigas=50;
		if(codigo_extraido=="B8") parametros.numero_formigas=60;
		if(codigo_extraido=="B9") parametros.numero_formigas=70;
		if(codigo_extraido=="BA") parametros.numero_formigas=80;
		if(codigo_extraido=="BB") parametros.numero_formigas=90;
		if(codigo_extraido=="BC") parametros.numero_formigas=100;

		if(codigo_extraido=="C1") parametros.neuronios_escondidos=2;	// neuronios escondidos (C)
		if(codigo_extraido=="C2") parametros.neuronios_escondidos=10;
		if(codigo_extraido=="C3") parametros.neuronios_escondidos=20;
		if(codigo_extraido=="C4") parametros.neuronios_escondidos=30;
		if(codigo_extraido=="C5") parametros.neuronios_escondidos=40;
		if(codigo_extraido=="C6") parametros.neuronios_escondidos=50;
		if(codigo_extraido=="C7") parametros.neuronios_escondidos=60;
		if(codigo_extraido=="C8") parametros.neuronios_escondidos=70;
		if(codigo_extraido=="C9") parametros.neuronios_escondidos=80;
		if(codigo_extraido=="CA") parametros.neuronios_escondidos=90;
		if(codigo_extraido=="CB") parametros.neuronios_escondidos=100;

		if(codigo_extraido=="D1") parametros.trace_default=true;	// trace default (D)
		if(codigo_extraido=="D2") parametros.trace_default=false;

		if(codigo_extraido=="E1") parametros.limite_simulacao=1000;	// limite da simulacao (E)
		if(codigo_extraido=="E2") parametros.limite_simulacao=5000;
		if(codigo_extraido=="E3") parametros.limite_simulacao=10000;
		if(codigo_extraido=="E4") parametros.limite_simulacao=50000;
		if(codigo_extraido=="E5") parametros.limite_simulacao=100000;
		if(codigo_extraido=="E6") parametros.limite_simulacao=0;

		if(codigo_extraido=="F1") parametros.degradacao_feromonio=5;	// degradacao feromonio (F)
		if(codigo_extraido=="F2") parametros.degradacao_feromonio=10;
		if(codigo_extraido=="F3") parametros.degradacao_feromonio=20;
		if(codigo_extraido=="F4") parametros.degradacao_feromonio=50;
		if(codigo_extraido=="F5") parametros.degradacao_feromonio=100;
		if(codigo_extraido=="F6") parametros.degradacao_feromonio=0;

		if(codigo_extraido=="G1") parametros.degradacao_alomonio=5;	// degradacao alomonio (G)
		if(codigo_extraido=="G2") parametros.degradacao_alomonio=10;
		if(codigo_extraido=="G3") parametros.degradacao_alomonio=20;
		if(codigo_extraido=="G4") parametros.degradacao_alomonio=50;
		if(codigo_extraido=="G5") parametros.degradacao_alomonio=100;
		if(codigo_extraido=="G6") parametros.degradacao_alomonio=0;

		if(codigo_extraido=="H1") parametros.alcance_feromonio=2;	// alcance feromonio (H)
		if(codigo_extraido=="H2") parametros.alcance_feromonio=4;
		if(codigo_extraido=="H3") parametros.alcance_feromonio=6;
		if(codigo_extraido=="H4") parametros.alcance_feromonio=8;
		if(codigo_extraido=="H5") parametros.alcance_feromonio=10;
		if(codigo_extraido=="H6") parametros.alcance_feromonio=12;
		if(codigo_extraido=="H7") parametros.alcance_feromonio=14;
		if(codigo_extraido=="H8") parametros.alcance_feromonio=16;
		if(codigo_extraido=="H9") parametros.alcance_feromonio=18;
		if(codigo_extraido=="HA") parametros.alcance_feromonio=20;
		if(codigo_extraido=="HB") parametros.alcance_feromonio=0;

		if(codigo_extraido=="I1") parametros.alcance_alomonio=2;	// alcance alomonio (I)
		if(codigo_extraido=="I2") parametros.alcance_alomonio=4;
		if(codigo_extraido=="I3") parametros.alcance_alomonio=6;
		if(codigo_extraido=="I4") parametros.alcance_alomonio=8;
		if(codigo_extraido=="I5") parametros.alcance_alomonio=10;
		if(codigo_extraido=="I6") parametros.alcance_alomonio=12;
		if(codigo_extraido=="I7") parametros.alcance_alomonio=14;
		if(codigo_extraido=="I8") parametros.alcance_alomonio=16;
		if(codigo_extraido=="I9") parametros.alcance_alomonio=18;
		if(codigo_extraido=="IA") parametros.alcance_alomonio=20;
		if(codigo_extraido=="IB") parametros.alcance_alomonio=0;

		if(codigo_extraido=="J1") parametros.alcance_cheiro_ninho=20;	// alcance cheiro ninho (J)
		if(codigo_extraido=="J2") parametros.alcance_cheiro_ninho=25;
		if(codigo_extraido=="J3") parametros.alcance_cheiro_ninho=30;
		if(codigo_extraido=="J4") parametros.alcance_cheiro_ninho=35;
		if(codigo_extraido=="J5") parametros.alcance_cheiro_ninho=40;
		if(codigo_extraido=="J6") parametros.alcance_cheiro_ninho=45;
		if(codigo_extraido=="J7") parametros.alcance_cheiro_ninho=50;
		if(codigo_extraido=="J8") parametros.alcance_cheiro_ninho=55;
		if(codigo_extraido=="J9") parametros.alcance_cheiro_ninho=60;
		if(codigo_extraido=="JA") parametros.alcance_cheiro_ninho=0;

		if(codigo_extraido=="K1") parametros.alcance_cheiro_alimento=2;	// alcance cheiro alimento (K)
		if(codigo_extraido=="K2") parametros.alcance_cheiro_alimento=4;
		if(codigo_extraido=="K3") parametros.alcance_cheiro_alimento=6;
		if(codigo_extraido=="K4") parametros.alcance_cheiro_alimento=8;
		if(codigo_extraido=="K5") parametros.alcance_cheiro_alimento=10;
		if(codigo_extraido=="K6") parametros.alcance_cheiro_alimento=12;
		if(codigo_extraido=="K7") parametros.alcance_cheiro_alimento=14;
		if(codigo_extraido=="K8") parametros.alcance_cheiro_alimento=16;
		if(codigo_extraido=="K9") parametros.alcance_cheiro_alimento=18;
		if(codigo_extraido=="KA") parametros.alcance_cheiro_alimento=20;
		if(codigo_extraido=="KB") parametros.alcance_cheiro_alimento=0;

		if(codigo_extraido=="L1") parametros.iteracoes_delay=0;	// delay (L)
		if(codigo_extraido=="L2") parametros.iteracoes_delay=1;
		if(codigo_extraido=="L3") parametros.iteracoes_delay=2;

		if(codigo_extraido=="M1") parametros.atualizacao_cheiro_alimentos=true;	// atualizacao cheiro alimento (M)
		if(codigo_extraido=="M2") parametros.atualizacao_cheiro_alimentos=false;
		
		if(codigo_extraido=="N1") parametros.aprendizado_nao_supervisionado=true;	// aprendizado nao supervisionado (N)
		if(codigo_extraido=="N2") parametros.aprendizado_nao_supervisionado=false;

		if(codigo_extraido=="O1") parametros.gasto_energia=5;	// degradacao alomonio (G)
		if(codigo_extraido=="O2") parametros.gasto_energia=10;
		if(codigo_extraido=="O3") parametros.gasto_energia=20;
		if(codigo_extraido=="O4") parametros.gasto_energia=50;
		if(codigo_extraido=="O5") parametros.gasto_energia=100;
		if(codigo_extraido=="O6") parametros.gasto_energia=0;

		mostra_parametros();
	}

	if(nro_anterior_formigas!=parametros.numero_formigas) 
	{
		init_formigas();
		simulacao_inicializar_mundo();
	}

	codigo_extraido="  ";

	inicializa_cheiro_ninho();
	atualiza_cheiro_alimentos();

	restore();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  decide_morte                                  //
//                                                                     //
// CARACTERISTICAS    :  decide quando uma formiga morre               //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  18/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void decide_morte(int index_formiga)
{
	int form_x, form_y, form_z;

	form_x=formiga[index_formiga].coord_x;
	form_y=formiga[index_formiga].coord_y;
	form_z=formiga[index_formiga].coord_z;

	if(mundo.elemento[form_x][form_y][form_z].paisagem==predador) // esmagada pelo predador
		formiga[index_formiga].vida=esmagada;

	if(form_z!=0)
		if(mundo.elemento[form_x][form_y][form_z-1].paisagem==agua) // afogada na agua
			formiga[index_formiga].vida=afogada;

	if(formiga[index_formiga].estomago<=0)	// morta de fome
		formiga[index_formiga].vida=fome;

	if(formiga[index_formiga].vida!=viva) 
	{
		solta_alomonio(index_formiga);	// cheiro da morte
		visualiza_ponto(form_x,form_y,form_z,index_formiga,99,99);
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  comida_por_passaro                            //
//                                                                     //
// CARACTERISTICAS    :  decide se o passaro come uma formiga ao       //
// alcance dele                                                        //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  18/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void comida_por_passaro(int index_formiga)
{
	int rnd, form_x, form_y, form_z;

	rnd=(11*rand())/RAND_MAX;

	if(rnd>8)
	{
		form_x=formiga[index_formiga].coord_x;
		form_y=formiga[index_formiga].coord_y;
		form_z=formiga[index_formiga].coord_z;

		if(form_x>=passaro.limite_baixo_x &&
			form_x<=passaro.limite_alto_x &&
			form_y>=passaro.limite_baixo_y &&
			form_y<=passaro.limite_alto_y)
		{
			formiga[index_formiga].vida=comida;
			solta_alomonio(index_formiga);	// cheiro da morte
			mundo.elemento[form_x][form_y][form_z].paisagem=vazio; // efetivamente retirada!!
			visualiza_ponto(form_x,form_y,form_z,index_formiga,99,99);
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  atualiza_formigas                             //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void atualiza_formigas(bool alerta_ligado)
{
	int laco_formigas, x_anterior, y_anterior, z_anterior;

	for(laco_formigas=0;laco_formigas<parametros.numero_formigas;laco_formigas++)
	{
		if(formiga[laco_formigas].vida==viva)
		{
			decide_morte(laco_formigas);
			if(passaro_presente) comida_por_passaro(laco_formigas);
		}
		
		if(formiga[laco_formigas].vida==viva && (alerta_ligado==false || formiga[laco_formigas].alerta))
		{
		
			if(formiga[laco_formigas].no_ninho==false)
			{
				if(parametros.gasto_energia!=0)		// reduzir estomago
					if(iteracao_mestre % parametros.gasto_energia == 0)
						gasta_energia(laco_formigas);

				visualiza_ponto(formiga[laco_formigas].coord_x,formiga[laco_formigas].coord_y,
					formiga[laco_formigas].coord_z,laco_formigas,99,99);

				doze_ifs(laco_formigas,false);

				if(formiga[laco_formigas].no_ninho==false)
				{

					x_anterior=formiga[laco_formigas].coord_x;
					y_anterior=formiga[laco_formigas].coord_y;
					z_anterior=formiga[laco_formigas].coord_z;

					ir_para_frente(laco_formigas,false);

					visualiza_ponto(x_anterior,y_anterior,z_anterior,sem_formiga,99,99);
				
					visualiza_ponto(formiga[laco_formigas].coord_x,formiga[laco_formigas].coord_y,
						formiga[laco_formigas].coord_z,laco_formigas,99,99);
				}
		
			} else
			{
				formiga[laco_formigas].descanso--;
				if(formiga[laco_formigas].descanso<=0) saia_do_ninho(laco_formigas);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  degradar_feromonio                            //
//                                                                     //
// CARACTERISTICAS    :  reduz todo feromonio em 1                     //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  12/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void degradar_feromonio()
{
	int x, y, z;

	for(x=0;x<max_mundo_x;x++)
		for(y=0;y<max_mundo_y;y++)
			for(z=0;z<max_mundo_z;z++)
				if(mundo.elemento[x][y][z].cheiro_feromonio>0)
					mundo.elemento[x][y][z].cheiro_feromonio--;
					//int(mundo.elemento[x][y][z].cheiro_feromonio/2);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  degradar_alomonio                             //
//                                                                     //
// CARACTERISTICAS    :  reduz todo alomonio em 1                      //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  12/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void degradar_alomonio()
{
	int x, y, z;

	for(x=0;x<max_mundo_x;x++)
		for(y=0;y<max_mundo_y;y++)
			for(z=0;z<max_mundo_z;z++)
				if(mundo.elemento[x][y][z].cheiro_alomonio>0)
					mundo.elemento[x][y][z].cheiro_alomonio--;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  visualiza_passaro                             //
//                                                                     //
// CARACTERISTICAS    :  visualiza o lugar do passaro na tela          //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  18/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void visualiza_passaro()
{
	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	gotoxy(passaro.limite_baixo_x,passaro.limite_baixo_y+2);
	printf("+");

	gotoxy(passaro.limite_alto_x,passaro.limite_baixo_y+2);
	printf("+");

	gotoxy(passaro.limite_baixo_x,passaro.limite_alto_y+2);
	printf("+");

	gotoxy(passaro.limite_alto_x,passaro.limite_alto_y+2);
	printf("+");
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  define_passaro                                //
//                                                                     //
// CARACTERISTICAS    :  define o lugar onde o passaro deve ficar      //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  18/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void define_passaro()
{
	short int x, y, z;

	char controle;

	controle=' ';

	while(controle!=char(13) && controle!=char(27))
	{
		visualiza_passaro();

		controle=getch();

		visualiza_ponto(passaro.limite_baixo_x,passaro.limite_baixo_y,4,sem_formiga,99,99);
		visualiza_ponto(passaro.limite_alto_x,passaro.limite_baixo_y,4,sem_formiga,99,99);
		visualiza_ponto(passaro.limite_baixo_x,passaro.limite_alto_y,4,sem_formiga,99,99);
		visualiza_ponto(passaro.limite_alto_x,passaro.limite_alto_y,4,sem_formiga,99,99);

		switch(controle)
		{
		case char(77): if(passaro.limite_alto_x<max_mundo_x-1)
					   {
						   passaro.limite_alto_x++;
						   passaro.limite_baixo_x++;
					   } break;
		case char(75): if(passaro.limite_baixo_x>0)
					   {
						   passaro.limite_alto_x--;
						   passaro.limite_baixo_x--;
					   } break;
		case char(72): if(passaro.limite_baixo_y>0)
					   {
						   passaro.limite_alto_y--;
						   passaro.limite_baixo_y--;
					   } break;
		case char(80): if(passaro.limite_alto_y<max_mundo_y-1)
					   {
						   passaro.limite_alto_y++;
						   passaro.limite_baixo_y++;
					   } break;
		}
	}

	if(controle==char(13)) passaro_presente=true;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  pezao                                         //
//                                                                     //
// CARACTERISTICAS    :  implementa um pe que esmaga as formigas       //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  18/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void pezao()
{
	short int x, y, z;

	char controle;

	controle=' ';

	while(controle!=char(13) && controle!=char(27))
	{
		setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

		gotoxy(sapato.limite_baixo_x,sapato.limite_baixo_y+2);
		printf("%c",char(218));

		gotoxy(sapato.limite_alto_x,sapato.limite_baixo_y+2);
		printf("%c",char(191));

		gotoxy(sapato.limite_baixo_x,sapato.limite_alto_y+2);
		printf("%c",char(192));

		gotoxy(sapato.limite_alto_x,sapato.limite_alto_y+2);
		printf("%c",char(217));

		controle=getch();

		visualiza_ponto(sapato.limite_baixo_x,sapato.limite_baixo_y,4,sem_formiga,99,99);
		visualiza_ponto(sapato.limite_alto_x,sapato.limite_baixo_y,4,sem_formiga,99,99);
		visualiza_ponto(sapato.limite_baixo_x,sapato.limite_alto_y,4,sem_formiga,99,99);
		visualiza_ponto(sapato.limite_alto_x,sapato.limite_alto_y,4,sem_formiga,99,99);

		switch(controle)
		{
		case char(77): if(sapato.limite_alto_x<max_mundo_x-1)
					   {
						   sapato.limite_alto_x++;
						   sapato.limite_baixo_x++;
					   } break;
		case char(75): if(sapato.limite_baixo_x>0)
					   {
						   sapato.limite_alto_x--;
						   sapato.limite_baixo_x--;
					   } break;
		case char(72): if(sapato.limite_baixo_y>0)
					   {
						   sapato.limite_alto_y--;
						   sapato.limite_baixo_y--;
					   } break;
		case char(80): if(sapato.limite_alto_y<max_mundo_y-1)
					   {
						   sapato.limite_alto_y++;
						   sapato.limite_baixo_y++;
					   } break;
		}
	}

	if(controle==char(13))
	{

		for(x=sapato.limite_baixo_x;x<=sapato.limite_alto_x;x++)
			for(y=sapato.limite_baixo_y;y<=sapato.limite_alto_y;y++)
				for(z=0;z<max_mundo_z;z++)
				{
					if(((x!=sapato.limite_baixo_x && x!=sapato.limite_alto_x) ||
						(y!=sapato.limite_baixo_y && y!=sapato.limite_alto_y)) &&
						x!=sapato.limite_baixo_x+6)
						mundo.elemento[x][y][z].paisagem=predador;
				}

		simulacao_mostrar_mundo(4);

		atualiza_cheiro_alimentos();

		inicializa_cheiro_ninho();
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  limpa_predador                                //
//                                                                     //
// CARACTERISTICAS    :  retira todos os elementos de predador do      //
// ambiente                                                            //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  18/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void limpa_predador()
{
	int x, y, z;

	for(x=0;x<max_mundo_x;x++)
		for(y=0;y<max_mundo_y;y++)
			for(z=0;z<max_mundo_z;z++)
				if(mundo.elemento[x][y][z].paisagem==predador)
					mundo.elemento[x][y][z].paisagem=vazio;

	simulacao_mostrar_mundo(4);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  simulacao_executar                            //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void simulacao_executar()
{
	char teste;

	iteracao_mestre=0;

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLACK );

	simulacao_mostrar_mundo(4);

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

	center(57," PARA COMECAR COM A SIMULACAO PRESSIONAR QUALQUER TECLA ");

	teste=getch();

	gotoxy(1,56);

	printf("TECLAS:");

	center(57," [d] MODO DEBUG, [t] MODO TRACE, [p] PE, [m] SEM PE, [s] DEFINE PASSARO, ");
	center(58," [o] LIGA/DESLIGA PASSARO [esc] SAIR ");

	parametros.visual=v_normal;

	teste= ' ';

	while(teste!=char(27) && (parametros.limite_simulacao==0 || iteracao_mestre<parametros.limite_simulacao))
	{

		if(parametros.visual==v_feromonio || parametros.visual==v_alomonio || parametros.visual==v_barulho ||
			parametros.visual==v_alimento)
			simulacao_mostrar_mundo(4);

		gotoxy(11,48);

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		printf("%i",iteracao_mestre);

		atualiza_formigas(false);	// todas as formigas

		if(parametros.iteracoes_delay==2) time_delay(1);

		atualiza_formigas(true);	// apenas as formigas com alerta ligado

		while(kbhit()) teste=getch();

		switch(teste)
		{
		case 'p': pezao();
			teste=' '; break;
		case 'd': limpa_teclas();
			simulacao_debug();
			limpa_teclas();
			teste=' ';
			//parametros.visual=v_normal;
			simulacao_mostrar_mundo(4);
			setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );
			gotoxy(1,56);
			printf("TECLAS:");
			center(57," [d] MODO DEBUG, [t] MODO TRACE, [p] PE, [l] SEM PE, [s] DEFINE PASSARO, ");
			center(58," [o] LIGA/DESLIGA PASSARO, [esc] SAIR ");
			break;
		case 'l': limpa_predador();
			teste=' '; break;
		case 's': define_passaro();
			teste=' '; break;
		case 'o': passaro_presente=!passaro_presente;
			teste=' '; 
			parametros.visual=v_normal;
			simulacao_mostrar_mundo(4);
			break;
		case 'm': parametros.visual=v_normal; 
			teste=' '; 
			simulacao_mostrar_mundo(4);
			break;
		case 'a': parametros.visual=v_alomonio;
			teste=' '; 
			simulacao_mostrar_mundo(4);
			break;
		case 'f': parametros.visual=v_feromonio;
			teste=' '; 
			simulacao_mostrar_mundo(4);
			break;
		case 'n': parametros.visual=v_ninho;
			teste=' '; 
			simulacao_mostrar_mundo(4);
			break;
		case 'c': parametros.visual=v_alimento;
			teste=' '; 
			simulacao_mostrar_mundo(4);
			break;
		case 'b': parametros.visual=v_barulho;
			teste=' '; 
			simulacao_mostrar_mundo(4);
			break;
		}

		if(passaro_presente) visualiza_passaro();

		iteracao_mestre++;

		if(parametros.iteracoes_delay>0) time_delay(1);

		if(iteracao_mestre % parametros.degradacao_feromonio==0) degradar_feromonio();

		if(iteracao_mestre % parametros.degradacao_alomonio==0) degradar_alomonio();
	}

	caixa_mensagem(aviso,0,4,"A SIMULACAO TERMINOU");

	restore();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  ajuda_referencia_rapida                       //
//                                                                     //
// CARACTERISTICAS    :                                                //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :                                                //
/////////////////////////////////////////////////////////////////////////

void ajuda_referencia_rapida()
{
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  boom                                          //
//                                                                     //
// CARACTERISTICAS    :  barulho alto do sapato                        //
//                                                                     //
// PARAMETROS         :  varrer (colocar barulho ou tirar)             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void boom(bool varrer)
{
	int x, y, z;

	for(x=0;x<max_mundo_x;x++)
		for(y=0;y<max_mundo_y;y++)
			for(z=0;z<max_mundo_z;z++)
				if(varrer) mundo.elemento[x][y][z].barulho=99; else
					mundo.elemento[x][y][z].barulho=1;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  limpa_teclas                                  //
//                                                                     //
// CARACTERISTICAS    :  limpa as duas linhas da descricao de teclas   //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  18/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void limpa_teclas()
{
	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

	center(57,"                                                                              ");
	center(58,"                                                                              ");
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  simulacao_debug                               //
//                                                                     //
// CARACTERISTICAS    :  faz o debug do ambiente                       //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  22/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void simulacao_debug()
{
	int nivel, visualizacao, formiga_atual;

	bool atualiza;

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLACK );

	nivel=4;

	visualizacao=v_normal;

	char escolha;

	escolha=' ';

	formiga_atual=0;

	atualiza=true;

	//preenche_linha_x(30,40,0,0,predador);

	//preenche_linha_x(20,30,0,4,predador);

	while(escolha!='v' && escolha!=char(27))
	{
		if(atualiza) simulacao_mostrar_mundo(nivel);

		setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

		gotoxy(1,56);

		printf("TECLAS:");

		center(57," [0-4] NIVEL, [a] ALOMONIO, [f] FEROMONIO, [n] NINHO, [c] ALIMENTO, ");
		center(58," [b] BARULHO, [m] NORMAL, [cursor] FORMIGA, [o] BOOM! [-] SILENCIO, [esc] SAIR ");

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		gotoxy(39,48);

		printf("%i",nivel);

		gotoxy(50,48);
	
		printf("%i ",formiga_atual+1);

		if(formiga[formiga_atual].no_ninho==false)
		{
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

			gotoxy(formiga[formiga_atual].coord_x,formiga[formiga_atual].coord_y+2);

			printf("X");
		}

		escolha=getch();

		atualiza=false;

		switch(escolha)
		{
			case '0' : if(nivel!=0) atualiza=true; nivel=0; break;
			case '1' : if(nivel!=1) atualiza=true; nivel=1; break;
			case '2' : if(nivel!=2) atualiza=true; nivel=2; break;
			case '3' : if(nivel!=3) atualiza=true; nivel=3; break;
			case '4' : if(nivel!=4) atualiza=true; nivel=4; break;
			case 'a' : if(parametros.visual!=v_alomonio) atualiza=true; parametros.visual=v_alomonio; break;
			case 'f' : if(parametros.visual!=v_feromonio) atualiza=true; parametros.visual=v_feromonio; break;
			case 'n' : if(parametros.visual!=v_ninho) atualiza=true; parametros.visual=v_ninho; break;
			case 'c' : if(parametros.visual!=v_alimento) atualiza=true; parametros.visual=v_alimento; break;
			case 'b' : if(parametros.visual!=v_barulho) atualiza=true; parametros.visual=v_barulho; break;
			case 'm' : if(parametros.visual!=v_normal) atualiza=true; parametros.visual=v_normal; break;
			case char(72): if(formiga_atual>0) formiga_atual--; mostra_formigas(); break;
			case char(80): if(formiga_atual<parametros.numero_formigas-1) formiga_atual++; mostra_formigas(); break;
			case char(13): simulacao_testar_sensores(formiga_atual); atualiza=true; break;
			case 'o' : boom(true); atualiza=true; break;
			case '-' : boom(false); atualiza=true; break;
		}
	}
	
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  simulacao_inicializar                         //
//                                                                     //
// CARACTERISTICAS    :  zerar o mundo virtual                         //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  18/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void simulacao_inicializar()
{
	init_formigas();
	simulacao_inicializar_mundo();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  ajuda_sobre                                   //
//                                                                     //
// CARACTERISTICAS    :  mostra a caixa "sobre o sistema"              //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  15/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void ajuda_sobre()
{
	setcolor( XBACKGROUND_LIGHTGRAY | XFOREGROUND_BLACK );

	janela(10,18,60,17);

	center(18,"SOBRE O SISTEMA");

	center(20,"B I O A N T");

	center(22,"AGENTE INTELIGENTE ATRAVES DE REDE NEURAL BIOLOGICAMENTE");

	center(23,"PLAUSIVEL PARA SIMULAR UMA FORMIGA TIPICA");

	center(25,"Sistema de Dissertacao de Mestrado");

	center(26,"Pontificia Universidade Catolica de Campinas");

	center(28,"Orientador: Prof. Dr. Joao Luis Garcia Rosa");

	center(30,"Autor: Marvin Oliver Schneider");

	center(32,"Maiores informacoes e documentacao disponivel em:");

	center(34,"http://www.marvinschneider.hpg.com.br");

	center(36,"Campinas, Outubro de 2003");

	cor_menu_selecionado2();

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  random_test                                   //
//                                                                     //
// CARACTERISTICAS    :  fornece alguns valores de random              //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  apenas para o desenvolvimento por causa da    //
// funcao horrivel que o Visual C++ disponibiliza....                  //
// ULTIMA ATUALIZACAO :  28/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void random_test()
{
	int i;

	int valor;

	for(i=0;i<200;i++)
	{
		valor=int(rand()*nro_saidas_ninho/(RAND_MAX));
		printf("%i\n",valor);
		if(valor==4) printf("\n\n\n\n\n");
	}

	getch();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  ascii                                         //
//                                                                     //
// CARACTERISTICAS    :  imprime a tabela ascii                        //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  apenas para o desenvolvimento                 //
//                                                                     //
// ULTIMA ATUALIZACAO :  28/08/03                                      //
/////////////////////////////////////////////////////////////////////////

void ascii()
{
	int i;

	for(i=0;i<256;i++)
		printf("%i %c      ",i,char(i));

	getch();
}

/////////////////////////////////////////////////////////////////////////
// 7. PRINCIPAL                                                        //
/////////////////////////////////////////////////////////////////////////

void main()
{	
//	random_test();
//	ascii();

	rede_inicializar(true);

	parametros_padrao();

	simulacao_inicializar_mundo();
	init_formigas();

	init_menu_principal();
	codigo_extraido="";

	x_cursor=0; // menu
	y_cursor=0;

	cls();

	caixa_mensagem(aviso,0,12,"Favor maximizar a janela para melhor visualizacao!");

	cls();

	atualiza_cabecalho();

	//subtitulo();

	home();

	ajuda_sobre();

	while(codigo_extraido!="XX" && codigo_extraido!="ZZ")
	{
		executa_menu();

		if(codigo_extraido=="SV") arquivo_gravar_pesos(); 
		if(codigo_extraido=="LD") arquivo_carregar_pesos();
		if(codigo_extraido=="XX" || codigo_extraido=="ZZ") arquivo_sair_do_programa();

		if(codigo_extraido=="IN") rede_inicializar(false);
		if(codigo_extraido=="TR") rede_treinar();
		if(codigo_extraido=="TP") rede_testar_propagacao();
		if(codigo_extraido=="TA") rede_testar_aprendizado();
		if(codigo_extraido=="DP") rede_dump();

		if(codigo_extraido=="IA") 
		{
			if(caixa_mensagem(pergunta,0,caixa_padrao,"Inicializar o ambiente (a situacao atual sera perdida)?"))
			simulacao_inicializar();
			limpa_caixa();
		}
		if(codigo_extraido=="PA") simulacao_parametros();
		if(codigo_extraido=="EX") simulacao_executar();
		if(codigo_extraido=="DB") {
			simulacao_debug();
			restore();
		}
		
		//if(codigo_extraido=="RE") ajuda_referencia_rapida();
		//if(codigo_extraido=="SO") ajuda_sobre();

	}
}