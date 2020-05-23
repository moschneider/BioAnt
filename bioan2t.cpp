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
#include "string.h" // operacoes de string

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

const int quebra   = 190; // nro de elementos por pagina para o dump

const int nro_entradas      = 11;  // parametros da rede
const int nro_neuronios_max = 20;
const int nro_saidas        = 20;

const int max_colunas_menu = 16;  // parametrizacao do menu
const int max_linhas_menu  = 14;

const int max_x_tela = 80; // apenas para melhor legibilidade

const int caixa_padrao = 12; // y-coordenada do inicio das caixas de mensagem

const float desligado = -1.0; // para inicializacao das ativacoes

//const int EN  = 0;  // para o GeneRec  // SEM USO -- VERSAO ANTIGA
//const int NSI = 1;
//const int NSV = 2;

//const float taxa_aprendizado = float(0.5);

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
const int nro_formigas_max = 101;

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
const int v_normal    = 0;
const int v_alomonio  = 1;
const int v_feromonio = 2;
const int v_ninho     = 3;
const int v_alimento  = 4;
const int v_barulho   = 5;

// sem formigas
const int sem_formiga = 999;

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

// limites especificas de simulacao
const int ate_sem_acucar = -1;
const int ate_morte      = -2;

// gerenciamento de sequencias
const int max_sequencia = 15;
const float no_data     = 5.0;

// macro-codes
const int code_escape                 = 0;
const int code_saia_do_ninho          = 1;
const int code_explore_ambiente       = 2;
const int code_segue_cheiro_feromonio = 3;
const int code_segue_cheiro_alimento  = 4;
const int code_pegue_alimento         = 5;
const int code_ache_cheiro_ninho      = 6;
const int code_segue_cheiro_ninho     = 7;
const int code_entre_no_ninho         = 8;
const int code_alertar_perigo         = 9;
const int code_fugir_do_perigo        = 10;
const int code_flush                  = 11;
const int code_apos_init              = 99;

/////////////////////////////////////////////////////////////////////////
// 3. TIPOS                                                            //
/////////////////////////////////////////////////////////////////////////

typedef struct _entrada_tipo {
	float valor;
	bool off;
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
	short int macro_code;
	short int repeticao;
} FORMIGA_TIPO;

typedef struct _saida_do_ninho_tipo {
	short int x, y, z;
} SAIDA_DO_NINHO_TIPO;

typedef struct _debug_doze_ifs {
	short int caso, linha;
} DEBUG_DOZE_IFS;

typedef struct _parametros_tipo {
	// ambiente
	short int numero_formigas;
	int degradacao_feromonio;
	int degradacao_alomonio;
	short int alcance_feromonio;
	short int alcance_alomonio;
	short int alcance_cheiro_ninho;
	short int alcance_cheiro_alimento;
	bool aprendizado_nao_supervisionado;
	int gasto_energia;
	
	// rede
	short int neuronios_escondidos;
	long int limite_generec;
	float percentagem_recorrencia;
	float taxa_aprendizado;
	int action_timeout;

	// simulacao
	bool modo_neural;
	bool trace;
	long int limite_simulacao;
	int iteracoes_delay;
	short int visual;
	
	// log
	bool log_ligado;

	// runtime
	short int posicao_cursor;	

} PARAMETROS_TIPO;

typedef struct _sapato_tipo {
	short int limite_baixo_x, limite_alto_x, limite_baixo_y, limite_alto_y;
} SAPATO_TIPO;

typedef struct _numero_bin {
	short int digit[8];
} NUMERO_BIN;

typedef struct _sequencia_tipo {
	char *descricao;
	ENTRADA_TIPO e[max_sequencia][nro_entradas];
	SAIDA_TIPO s[max_sequencia][nro_saidas];
	int posicao;
	int numero;
} SEQUENCIA_TIPO;


/////////////////////////////////////////////////////////////////////////
// 4. VARIAVEIS                                                        //
/////////////////////////////////////////////////////////////////////////

ITEM_MENU menu[max_colunas_menu][max_linhas_menu]; // estrutura de menu e adicionais
int max_comp_menu[max_colunas_menu];
int cabecalho_pos[max_colunas_menu];

int i, x_cursor, y_cursor;  // para o gerenciamento da execucao do menu
char *codigo_extraido, opcao;

int elementos; // elementos na janela do dump

REDE_RECORRENTE_TIPO rede[nro_formigas_max], rede_backup;

MUNDO_VIRTUAL mundo;

FORMIGA_TIPO formiga[nro_formigas_max];

SAIDA_DO_NINHO_TIPO saida_ninho[nro_saidas_ninho];

long int iteracao_mestre;

PARAMETROS_TIPO parametros;

int max_feromonio, min_feromonio, passo_feromonio;

SAPATO_TIPO sapato, passaro; // o passaro nao eh um sapato, mas o esquema de limites eh o mesmo

bool passaro_presente;

short int bin[8];	// para decodificacao binaria

bool aprendizado_ok[45];

short int filho[nro_saidas];

SEQUENCIA_TIPO sequencia_atual;

FILE *arquivo, *debug, *network, *aval;

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
int xrand(int);							// gerador de numeros aleatorios mais simples!!

////////// ROTINAS PARA PARAMETRIZACAO /////////

bool limite_alto_ok(void);
bool limite_baixo_ok(void);
void aumenta_parametro(void);
void diminui_parametro(void);
int traduz_cursor(void);
void bioant_parametros(void);
void mostra_janela_parametros(void);

////////// ROTINAS PARA IDENTIFICACAO /////////

void subtitulo(void);					// imprime o rodape com o titulo

////////// ROTINAS REFERENTES AO MENU /////////

void zera_menu(void);					// zera o conteudo da estrutura menu
void ajuste_parametros_menu(void);		// ajusta os parametros de controle
void init_menu_principal(void);			// inicializacao do menu principal
void atualiza_cabecalho(void);			// atualiza o cabecalho do menu
void dropdown(void);					// mostra a parte dropdown
void executa_menu(void);				// gerenciador do menu dropdown

////////// ROTINAS REFERENTES AO ARQUIVO /////////

void arquivo_sair_do_programa(void);

////////// ROTINAS REFERENTES A REDE /////////

float sigmoide(float);					// Funcao de propagacao da rede
void propagacao_escondidos(int,int);
void propagacao_saidas(int,int);
void aprendizado_generec(int);
void rede_inicializar(int,bool);
void clone_brains(void);
void rede_treinar(void);
void rede_testar_propagacao(int);
void rede_testar_aprendizado(int);
void prepara_dump(char*);
void rede_dump_entradas(int);
void rede_dump_saidas(int);
void rede_dump(int);
void dump_entradas_saidas(int);
NUMERO_BIN dec_to_bin(int);
void init_bin(void);
void situacao_para_entradas(int);
void gera_saidas_desejadas(int,int);
void gera_situacao_aleatoria(void);
void dump_valores_formiga_100(void);
int maior_saida_rede(int);
int funcionar_rede(int);
void funcionar_aprendizado(int);
void flush_saidas(int);
void flush_entradas(int);
void flush_entradas_off(int);
void configura_numero_de_filhos(void);
void init_sequencia(void);
void gerencia_sequencia_atual(void);
void situacao_para_sequencia(int);
void grava_sequencia_atual(void);
void le_sequencia_atual(void);
void abre_arquivo(void);
void deleta_arquivo(void);
float extrair(char);
void dados_treinamento_para_rede(int,int);
void abre_arquivo_rede(void);
void grava_rede(void);
void le_rede(void);
int conta_entradas_positivas(int);

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
bool objeto_chegando_acima(int);
bool olfato_frente_maior(int,int);
bool olfato_direita_maior(int,int);
bool olfato_esquerda_maior(int,int);
bool olfato_frente_menor(int,int);
bool olfato_direita_menor(int,int);
bool olfato_esquerda_menor(int,int);
bool travada(int);
bool existe_barulho(void);
bool sem_acucar(void);
bool todas_mortas(void);

/// gerenciamento de sensores
int objeto_frente(int);
int objeto_esquerda(int);
int objeto_direita(int);
int objeto_acima(int);
int objeto_abaixo(int);

/// gerenciamento de acao
void fugir_perigo(int);
void fugir_alomonio(int);
void voltar_normalidade(int);
void evitar_colisao(int);
void pegar_comida(int);
void sair_alerta(int);
void sair_caminho_ruim(int);
void entrar_ninho(int,bool);
void seguir_atracao_frente(int);
void buscar_sinal(int);
void seguir_atracao_ambigua(int);
void seguir_atracao_direita(int);
void seguir_atracao_esquerda(int);
void gasta_energia(int);

// controladora simbolica
DEBUG_DOZE_IFS doze_ifs(int,bool,bool);

////////// ROTINAS REFERENTES A SIMULACAO /////////

void legenda(bool);
void visualiza_formigas(void);
int devolve_formiga_z(int,int);
void comida_por_passaro(int);
void visualiza_passaro(void);
void define_passaro(void);
void pezao(void);
void limpa_predador(void);
void decide_morte(int);
void simulacao_inicializar(void);
void limpa_teclas(void);
void devolve_cor_fases(int);
void devolve_cor_decisao(int);
//void acha_min_max_feromonio(void);
void time_delay(int);
void parametros_padrao(void);
void degradar_feromonio(void);
void degradar_alomonio(void);
void imprima_formiga(int);
void boom(bool);
bool posica_disponivel(int,int);
void atualiza_formigas_neural(void);
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
int ajusta_altura(int,int);
bool out_of_bounds(int,int,int);
int devolve_x_mais(int);
int devolve_y_mais(int);
bool ir_para_frente(int,bool);
void ir_para_tras(int,bool);
void vira_sentido_horario(int);
void vira_sentido_contra_horario(int);
bool saia_do_ninho(int);
void obtenha_dados_sensores(int);
void ajusta_cor(int);
void mostra_sentidos(int);
void simulacao_testar_sensores(int);
void simulacao_executar(void);
void executa_acao(int,int);

////////// ROTINAS REFERENTES A AJUDA /////////

void ajuda_referencia_rapida(void);
void ajuda_sobre(void);

////////// ROTINAS APENAS DESENVOLVIMENTO /////

void random_test(void);
void ascii(void);
void teste_conversor(void);
void teste_gerador(void);
void explica_linha(int);
void dump_curto_entradas_saidas(int);
void dump_compacto_rede(int);
void devolve_descricao_code(int);

///////// ROTINAS DE ARQUIVO LOG //////////////

void abre_arquivo_debug(void);
void deleta_arquivo_debug(void);
void mostra_log(void);
void explica_linha_debug(int);
void explica_saida_debug(int);
void dump_entradas_saidas_concentrador(void);
void dump_entradas_saidas_aprendizado_continuo(void);
void dump_entradas_saidas_funcionamento_rede(void);

///////// ROTINAS REFERENTES AS MACRO-INTERPRETADORAS //////////

bool inter_perigo_presente(int);
bool inter_objeto_visual_presente(int);
bool inter_tato_alimento_presente(int);
bool inter_entrada_ninho_presente(int);
bool inter_tato_margem_presente(int);
bool inter_cheiro_alomonio_presente(int);
bool inter_cheiro_feromonio_presente(int);
bool inter_cheiro_ninho_presente(int);
bool inter_cheiro_alimento_presente(int);
bool inter_fome_presente(int);
bool inter_carga_presente(int);

///////// ROTINAS REFERENTES AS MACRO-CONTROLADORAS //////////
void desliga_entradas_off(int);
bool caso_escape(int);
bool trata_eventos_menores(int);
void macro_saia_do_ninho(int);
void macro_explore_ambiente(int);
void macro_segue_cheiro_feromonio(int);
void macro_segue_cheiro_alimento(int);
void macro_pegue_alimento(int);
void macro_ache_cheiro_ninho(int);
void macro_segue_cheiro_ninho(int);
void macro_entre_no_ninho(int);
void macro_alertar_perigo(int);
void macro_fugir_do_perigo(int);

///////////////// IO ENTRADAS SAIDAS /////////////////////

void devolve_descricao_entrada(int);
void devolve_descricao_saida(int);

///////////////// AVALIACAO /////////////////////

void abre_arquivo_avaliacao();
long int devolve_quantidade_alimento();
void escreve_dados_avaliacao(int);

void flush_repeticao_formigas(void);

/////////////////////////////////////////////////////////////////////////
// 6. FUNCOES                                                          //
/////////////////////////////////////////////////////////////////////////

void flush_repeticao_formigas()
{
	int i;

	for(i=0;i<nro_formigas_max;i++)
		formiga[i].repeticao=0;
}

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

	initial=long(time(&ltime));
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
	//limpa_dropdown();

	cor_menu2();

	janela(0,2,78,54);
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
	cls_janela(2,58);
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
	caixa_mensagem(simples,0,56,"Pressione qualquer tecla para continuar.");

	getch();

	//cls_janela(22,23);
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
	//x_cursor=0;
	//y_cursor=0;

	cls();
	init_menu_principal();
	atualiza_cabecalho();
		
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
	
	menu[0][1].descricao="Parametros";
	menu[0][1].codigo="PA";
	menu[0][2].descricao="Sair";
	menu[0][2].codigo="XX";

	menu[1][0].descricao="Rede";
	
	menu[1][1].descricao="Inicializar";
	menu[1][1].codigo="IN";
	menu[1][2].descricao="Treinar";
	menu[1][2].codigo="TR";
	menu[1][3].descricao="Avaliar";
	menu[1][3].codigo="AA";
	menu[1][4].descricao="Dump Simples";
	menu[1][4].codigo="DE";
	menu[1][5].descricao="Dump Completo";
	menu[1][5].codigo="DP";
	menu[1][6].descricao="Gravar em Disco";
	menu[1][6].codigo="GR";
	menu[1][7].descricao="Ler de Disco";
	menu[1][7].codigo="LR";
	
	menu[2][0].descricao="Sequencias";

	menu[2][1].descricao="Programar";
	menu[2][1].codigo="MS";
	menu[2][2].descricao="Deletar Arquivo";
	menu[2][2].codigo="ZS";

	menu[3][0].descricao="Simulacao";
	
	menu[3][1].descricao="Inicializar";
	menu[3][1].codigo="IA";
	menu[3][2].descricao="Executar";
	menu[3][2].codigo="EX";
	menu[3][3].descricao="Debug";
	menu[3][3].codigo="DB";

	menu[4][0].descricao="Arquivo-LOG";

	menu[4][1].descricao="Mostrar";
	menu[4][1].codigo="ML";
	menu[4][2].descricao="Zerar";
	menu[4][2].codigo="ZL";

	menu[5][0].descricao="Ajuda";
	
	menu[5][1].descricao="Sobre";
	menu[5][1].codigo="SO";

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

		if(menu[i][0].descricao!=" ")
		{
			gotoxy(cabecalho_pos[i],0);
			printf(" %s ",menu[i][0].descricao);
		}
	}

	gotoxy(65,0);

	printf("(VERSAO BETA)");
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
// NOME DA FUNCAO     :  clone_brains                                  //
//                                                                     //
// CARACTERISTICAS    :  producao de 19 clones do cerebro 0            //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void clone_brains()
{
	int i;

	if(parametros.log_ligado)
	{
		fprintf(debug,"<REDE> *** CLONAGEM DE CEREBROS ***\n");
		fprintf(debug,"<REDE>\n");
		fprintf(debug,"<REDE> ANTES:\n");
		for(i=0;i<parametros.numero_formigas;i++)
			dump_compacto_rede(i);
	}

	for(i=1;i<parametros.numero_formigas;i++)
		rede[i]=rede[0];

	if(parametros.log_ligado)
	{
		fprintf(debug,"<REDE> DEPOIS:\n");
		for(i=0;i<parametros.numero_formigas;i++)
			dump_compacto_rede(i);
		fprintf(debug,"<REDE> \n");
	}
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

void rede_inicializar(int index_rede, bool silencioso)
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
				rede[index_rede].sinapse_EN[init_i][init_j].valor=float(rand()/(RAND_MAX*5.0)-0.1);

		// inicializacao das sinapses entre a camada escondida e as saidas ("IDA")

		for(init_i=0;init_i<parametros.neuronios_escondidos;init_i++)
			for(init_j=0;init_j<nro_saidas;init_j++)
				rede[index_rede].sinapse_NS[init_i][init_j].valor=float(rand()/(RAND_MAX*5.0)-0.1);

		// o resto das inicializacoes e feito apenas por seguranca, pois estes valores
		// devem ser definidos dinamicamente num evento posterior

		// inicializacao das entradas

		for(init_i=0;init_i<nro_entradas;init_i++)
		{
			rede[index_rede].entrada[init_i].valor=0.0;
			rede[index_rede].entrada[init_i].off=false;
		}

		// inicializacao dos neuronios na camada escondida

		for(init_i=0;init_i<parametros.neuronios_escondidos;init_i++)
		{
			rede[index_rede].neuronio[init_i].valor=0.0;
			rede[index_rede].neuronio[init_i].ativacao[mais]=desligado;
			rede[index_rede].neuronio[init_i].ativacao[menos]=desligado;
		}

		// inicializacao das saidas

		for(init_i=0;init_i<nro_saidas;init_i++)
		{
			rede[index_rede].saida[init_i].valor=0.0;
			rede[index_rede].saida[init_i].ativacao[mais]=0.0;
			rede[index_rede].saida[init_i].ativacao[menos]=0.0;
			rede[index_rede].saida[init_i].esperado=0.0;
		}

		if(silencioso==false)
		{
			caixa_mensagem(aviso,0,caixa_padrao,"Rede inicializada com sucesso.");

			limpa_caixa();
		}
		
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  flush_saidas                                  //
//                                                                     //
// CARACTERISTICAS    :  reseta as saidas esperadas para zero          //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  03/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void flush_saidas(int index_rede)
{
	int i;

	if(parametros.log_ligado)
	{
		fprintf(debug,"<REDE> *** FLUSH SAIDAS ***\n");
		fprintf(debug,"<REDE> (FORMIGA %i)\n",index_rede);
	}

	for(i=0;i<nro_saidas;i++)
	{
		rede[index_rede].saida[i].esperado=0;
		rede[index_rede].saida[i].ativacao[mais]=0;
		rede[index_rede].saida[i].ativacao[menos]=0;
		rede[index_rede].saida[i].valor=0;
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  flush_entradas                                //
//                                                                     //
// CARACTERISTICAS    :  reseta as entradas para zero                  //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void flush_entradas(int index_rede)
{
	int i;

	if(parametros.log_ligado)
	{
		fprintf(debug,"<REDE> *** FLUSH ENTRADAS ***\n");
		fprintf(debug,"<REDE> (FORMIGA %i)\n",index_rede);
	}

	for(i=0;i<nro_entradas;i++)
	{
		rede[index_rede].entrada[i].valor=0.0;
		//rede[index_rede].entrada[i].off=0;
	}
}


void flush_entradas_off(int index_rede)
{
	int i;

	if(parametros.log_ligado)
	{
		fprintf(debug,"<REDE> *** FLUSH DESLIGAMENTO ENTRADAS ***\n");
		fprintf(debug,"<REDE> (FORMIGA %i)\n",index_rede);
	}

	for(i=0;i<nro_entradas;i++)
	{
		rede[index_rede].entrada[i].off=0;
	}
}


/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  configura_numero_de_filhos                    //
//                                                                     //
// CARACTERISTICAS    :  configura o numero do filhos que cada entrada //
// pai possui                                                          //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  03/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void configura_numero_de_filhos()
{
	filho[0]=0;
	filho[1]=0;
	filho[2]=0;
	filho[3]=2;
	filho[4]=0;
	filho[5]=0;
	filho[6]=0;
	filho[7]=0;
	filho[8]=0;
	filho[9]=0;
	filho[10]=0;
	filho[11]=0;
	filho[12]=0;
	filho[13]=0;
	filho[14]=0;
	filho[15]=0;
	filho[16]=19;
	filho[17]=0;
	filho[18]=14;
	filho[19]=19;
	filho[20]=6;
	filho[21]=4;
	filho[22]=4;
	filho[23]=2;
	filho[24]=4;
	filho[25]=0;
	filho[26]=0;
	filho[27]=0;
	filho[28]=14;
	filho[29]=19;
	filho[30]=2;
	filho[31]=4;
	filho[32]=4;
	filho[33]=4;
	filho[34]=14;
	filho[35]=6;
	filho[36]=14;
	filho[37]=2;
	filho[38]=2;
	filho[39]=2;
	filho[40]=2;
	filho[41]=2;
	filho[42]=2;
	filho[43]=2;
	filho[44]=2;
	filho[45]=2;
	filho[46]=2;
	filho[47]=6;
	filho[48]=4;
}

void deleta_arquivo(void)
{
	fclose(arquivo);

	arquivo = fopen("bioant.seq","w+");

	if(arquivo==NULL)
	{
		caixa_mensagem(alerta,0,15,"Arquivo de sequencias (BIOANT.SEQ) nao pode ser aberto!!!");
		return;
	}

	fprintf(arquivo,"; **************************************************\n");
	fprintf(arquivo,"; BIOANT.SEQ                                        \n");
	fprintf(arquivo,"; Arquivo de sequencias do Sistema BIOANT           \n");
	fprintf(arquivo,"; 2003, Marvin Oliver Schneider                     \n");
	fprintf(arquivo,";                                                   \n");
	fprintf(arquivo,"; .....        = Titulo da Sequencia                \n");
	fprintf(arquivo,"; = n          = Numero do Conjunto (0-19)          \n");
	fprintf(arquivo,"; + ENTRADAS   = Inicio da parte de entradas        \n");
	fprintf(arquivo,"; - SAIDAS     = Inicio da parte de saidas          \n");
	fprintf(arquivo,"; ; blabla     = Comentario                         \n");
	fprintf(arquivo,";                                                   \n");
	fprintf(arquivo,"; Valores Entradas / Saida : 0 / 1 e 5 (=sem dados) \n");
	fprintf(arquivo,";                                                   \n");
	fprintf(arquivo,"; ATENCAO: Efetuar mudancas nos dados (a nao ser    \n");
	fprintf(arquivo,";          a geracao de novos conjuntos) diretamente\n");
	fprintf(arquivo,";          neste arquivo!                           \n");
	fprintf(arquivo,"; **************************************************\n\n");

	fseek( arquivo, 0, SEEK_END );
}

void abre_arquivo(void)
{
	arquivo = fopen("bioant.seq","r+");

	if(arquivo==NULL)
	{
		arquivo = fopen("bioant.seq","w+");

		if(arquivo==NULL)
		{
			caixa_mensagem(alerta,0,15,"Arquivo de sequencias (BIOANT.SEQ) nao pode ser aberto!!!");
			return;
		}

		fprintf(arquivo,"; **************************************************\n");
		fprintf(arquivo,"; BIOANT.SEQ                                        \n");
		fprintf(arquivo,"; Arquivo de sequencias do Sistema BIOANT           \n");
		fprintf(arquivo,"; 2003, Marvin Oliver Schneider                     \n");
		fprintf(arquivo,";                                                   \n");
		fprintf(arquivo,"; .....        = Titulo da Sequencia                \n");
		fprintf(arquivo,"; = n          = Numero do Conjunto (0-19)          \n");
		fprintf(arquivo,"; + ENTRADAS   = Inicio da parte de entradas        \n");
		fprintf(arquivo,"; - SAIDAS     = Inicio da parte de saidas          \n");
		fprintf(arquivo,"; ; blabla     = Comentario                         \n");
		fprintf(arquivo,";                                                   \n");
		fprintf(arquivo,"; Valores Entradas / Saida : 0 / 1 e 5 (=sem dados) \n");
		fprintf(arquivo,";                                                   \n");
		fprintf(arquivo,"; ATENCAO: Efetuar mudancas nos dados (a nao ser    \n");
		fprintf(arquivo,";          a geracao de novos conjuntos) diretamente\n");
		fprintf(arquivo,";          neste arquivo!                           \n");
		fprintf(arquivo,"; **************************************************\n\n");

	}

	fseek( arquivo, 0, SEEK_END );
}

void abre_arquivo_avaliacao(void)
{
	aval = fopen("bioant.avl","r+");

	if(aval==NULL)
	{
		aval = fopen("bioant.avl","w+");

		if(aval==NULL)
		{
			caixa_mensagem(alerta,0,15,"Arquivo de avaliacao (BIOANT.AVL) nao pode ser aberto!!!");
			return;
		}

		fprintf(aval,"; **************************************************\n");
		fprintf(aval,"; BIOANT.AVL                                        \n");
		fprintf(aval,"; Arquivo de avaliacao do Sistema BIOANT            \n");
		fprintf(aval,"; 2003, Marvin Oliver Schneider                     \n");
		fprintf(aval,"; **************************************************\n\n");

	}

	fseek(aval, 0, SEEK_END );
}

void abre_arquivo_rede(void)
{
	network = fopen("bioant.red","r+");

	if(network==NULL)
	{
		network = fopen("bioant.red","w+");

		if(network==NULL)
		{
			caixa_mensagem(alerta,0,15,"Arquivo de pesos (BIOANT.RED) nao pode ser aberto!!!");
			return;
		}

		fprintf(network,"; **************************************************\n");
		fprintf(network,"; BIOANT.RED                                        \n");
		fprintf(network,"; Arquivo de pesos da rede do Sistema BIOANT        \n");
		fprintf(network,"; 2003, Marvin Oliver Schneider                     \n");
		fprintf(network,";                                                   \n");
		fprintf(network,"; & SIN1       = Inicio das Sinapses entre          \n");
		fprintf(network,";                Entrada e Neuronios Escondidos     \n");
		fprintf(network,"; & SIN2       = Inicio das Sinapses entre          \n");
		fprintf(network,";              = Neuronios Escondidos e Saidas      \n");
		fprintf(network,"; * .../...    = Cabecalho Valor de Sinapse         \n");
		fprintf(network,"; **************************************************\n\n");
	}

	fseek( network, 0, SEEK_END );
}

void grava_rede()
{
	int i, j;

	network = fopen("bioant.red","w+");

	if(network==NULL)
	{
		caixa_mensagem(alerta,0,15,"Arquivo de pesos (BIOANT.RED) nao pode ser aberto!!!");
		return;
	}

	fprintf(network,"; **************************************************\n");
	fprintf(network,"; BIOANT.RED                                        \n");
	fprintf(network,"; Arquivo de pesos da rede do Sistema BIOANT        \n");
	fprintf(network,"; 2003, Marvin Oliver Schneider                     \n");
	fprintf(network,";                                                   \n");
	fprintf(network,"; & SIN1       = Inicio das Sinapses entre          \n");
	fprintf(network,";                Entrada e Neuronios Escondidos     \n");
	fprintf(network,"; & SIN2       = Inicio das Sinapses entre          \n");
	fprintf(network,";              = Neuronios Escondidos e Saidas      \n");
	fprintf(network,"; * .../...    = Cabecalho Valor de Sinapse         \n");
	fprintf(network,"; **************************************************\n\n");
	
	fseek( network, 0, SEEK_END );

	fprintf(network,"& SIN1\n");

	for(j=0;j<nro_neuronios_max;j++)
		for(i=0;i<nro_entradas;i++)
		{
			if(parametros.log_ligado)
				fprintf(debug,"<REDE GRAVACAO> rede[0].sinapse_EN[%i][%i].valor==%f\n",i,j,rede[0].sinapse_EN[i][j].valor);
			fprintf(network,"* %i/%i\n",i,j);
			fprintf(network,"%f\n",rede[0].sinapse_EN[i][j].valor);
		}

	fprintf(network,"& SIN2\n");

	for(j=0;j<nro_saidas;j++)
		for(i=0;i<nro_neuronios_max;i++)
		{
			if(parametros.log_ligado)
				fprintf(debug,"<REDE GRAVACAO> rede[0].sinapse_NS[%i][%i].valor==%f\n",i,j,rede[0].sinapse_NS[i][j].valor);
			fprintf(network,"* %i/%i\n",i,j);
			fprintf(network,"%f\n",rede[0].sinapse_NS[i][j].valor);
		}
}

float extrair(char ch)
{
	float flag;

	flag=5.0;

	if(ch=='1') flag=1.0; else
		if(ch=='0') flag=0.0;

	return flag;
}

void deleta_arquivo_debug(void)
{
	debug = fopen("bioant.log","w+");

	if(debug==NULL)
	{
		caixa_mensagem(alerta,0,15,"Arquivo de debug (BIOANT.LOG) nao pode ser aberto!!!");
		return;
	}

	if(parametros.log_ligado) fprintf(debug,"; **************************************************\n");
	if(parametros.log_ligado) fprintf(debug,"; BIOANT.LOG                                        \n");
	if(parametros.log_ligado) fprintf(debug,"; Arquivo de LOG do BIOANT                          \n");
	if(parametros.log_ligado) fprintf(debug,"; 2003, Marvin Oliver Schneider                     \n");
	if(parametros.log_ligado) fprintf(debug,"; **************************************************\n\n");

}

void abre_arquivo_debug(void)
{
	debug = fopen("bioant.log","r+");

	if(debug==NULL)
	{
		debug = fopen("bioant.log","w+");

		if(debug==NULL)
		{
			caixa_mensagem(alerta,0,15,"Arquivo de debug (BIOANT.LOG) nao pode ser aberto!!!");
			return;
		}

		if(parametros.log_ligado) fprintf(debug,"; **************************************************\n");
		if(parametros.log_ligado) fprintf(debug,"; BIOANT.LOG                                        \n");
		if(parametros.log_ligado) fprintf(debug,"; Arquivo de LOG do BIOANT                          \n");
		if(parametros.log_ligado) fprintf(debug,"; 2003, Marvin Oliver Schneider                     \n");
		if(parametros.log_ligado) fprintf(debug,"; **************************************************\n\n");

	}

	fseek(debug, 0, SEEK_END );
}

void le_sequencia_atual(void)
{
	char chLine[81];

	bool achou_descricao, achou_entradas, achou_saidas, terminou_entradas, finalizado;

	int pos, i;

	bool fim;

	fim=false;

	pos=0;

	achou_descricao=false;
	achou_entradas=false;
	terminou_entradas=false;
	achou_saidas=false;
	finalizado=false;

	// fseek( arquivo, 0, SEEK_SET );

	while(fgets(chLine,80,arquivo)!=NULL && pos<max_sequencia)
	{
		if(chLine[0]!='1' && chLine[0]!='5' && chLine[0]!=';' &&
			chLine[0]!='0' && chLine[0]!='=' && chLine[0]!='*' && chLine[0]!='\n'
			&& chLine[0]!='+' && chLine[0]!='-' && achou_descricao==false)
		{
			sequencia_atual.descricao=chLine;
			achou_descricao=true;
		}

		if(achou_entradas==true && achou_saidas==false && terminou_entradas==false)
		{
			for(i=0;i<nro_entradas;i++)
			{
				sequencia_atual.e[pos][i].valor=extrair(chLine[i]);
				//printf("e:%i %f %c...",i,sequencia_atual.e[pos][i].valor,chLine[i]);

				//getch();
			}

			terminou_entradas=true;
		}

		if(achou_saidas)
		{
			for(i=0;i<nro_saidas;i++)
			{
				sequencia_atual.s[pos][i].esperado=extrair(chLine[i]);

				//printf("s:%i %f %c...",i,sequencia_atual.s[pos][i].esperado,chLine[i]);

				//getch();
			}

			achou_saidas=false;
			achou_entradas=false;
			terminou_entradas=false;

			pos++;
		}

		if(achou_entradas==false && chLine[0]=='+')
			achou_entradas=true;

		if(achou_saidas==false && achou_entradas && chLine[0]=='-')
			achou_saidas=true;
	}

	sequencia_atual.numero++;

	if(pos==0 && sequencia_atual.numero!=1) 
	{
		fseek(arquivo,0,SEEK_SET);
		sequencia_atual.numero=0;
		le_sequencia_atual();
	}
}

void le_rede()
{
	int e, n, s, sector;

	char chLine[81];

	//abre_arquivo_rede();

	fseek(network,0,SEEK_SET);

	e=-1;

	n=0;

	s=0;

	sector=0;

	rede_inicializar(0,true);

	while(fgets(chLine,80,network)!=NULL)
	{
		//printf("oi");

		//getch();

		//if(parametros.log_ligado)
		//	fprintf(debug,"<REDE LEITURA> LEIA (ENTRADA:%i NEURONIO:%i SAIDA:%i SECTOR:%i\n",e,n,s,sector); 

		if(chLine[0]!=' ' && chLine[0]!=';')
		{
			if(sector==2 && chLine[0]=='*')
			{
				n++;
				if(n % nro_neuronios_max==0)
				{
					n=0;
					s++;
				}
			}

			if(sector==2 && chLine[0]!='*' && chLine[0]!='&')
			{
				rede[0].sinapse_NS[n][s].valor=atof(chLine);
				if(parametros.log_ligado)
					fprintf(debug,"<REDE LEITURA> rede[0].sinapse_NS[%i][%i].valor=%f\n",n,s,rede[0].sinapse_NS[n][s].valor);
			}

			if(sector==1 && chLine[0]=='*')
			{
				e++;
				if(e % nro_entradas==0)
				{
					e=0;
					n++;
				}
			}

			if(sector==1 && chLine[0]!='*' && chLine[0]!='&')
			{
				rede[0].sinapse_EN[e][n].valor=atof(chLine);
				if(parametros.log_ligado)
					fprintf(debug,"<REDE LEITURA> rede[0].sinapse_EN[%i][%i].valor=%f\n",e,n,rede[0].sinapse_EN[e][n].valor);
			}

			if(sector==1 && chLine[0]=='&')
			{
				n=0;
				sector=2;
			}

			if(sector==0 && chLine[0]=='&')
				sector=1;
		}
	}
}

void mostra_log()
{
	int counter;

	char chLine[81];

	char choice;

	fseek(debug,0,SEEK_SET);

	cls();

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	center(0," VISUALIZACAO DO ARQUIVO DE LOG ");

	gotoxy(0,2);

	counter=0;

	choice=' ';

	while(fgets(chLine,80,debug)!=NULL && choice!=char(27))
	{
		counter++;

		setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

		printf("%s",chLine);

		if(counter>52) 
		{
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

			center(56," [ESC] PARA SAIR, QUALQUER OUTRA TECLA PARA CONTINUAR ");
			choice=getch();
			counter=0;
			cls();
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

			center(0," VISUALIZACAO DO ARQUIVO DE LOG ");

			gotoxy(0,2);
		}
	}

	if(choice!=char(27)) espera();
}

void escreve_dados_avaliacao(int iteracao)
{
	fprintf(aval,"%i %i\n",iteracao,devolve_quantidade_alimento());
}

long int devolve_quantidade_alimento(void)
{
	int i, j, k;

	long int quantidade;

	quantidade=0;

	for(i=0;i<max_mundo_x;i++)
		for(j=0;j<max_mundo_y;j++)
			for(k=0;k<max_mundo_z;k++)
				if(mundo.elemento[i][j][k].paisagem>comida0)
					quantidade=quantidade+mundo.elemento[i][j][k].paisagem-comida0;

	return quantidade;
}

void grava_sequencia_atual(void)
{
	int i, j;

	abre_arquivo();
	
		fseek( arquivo, 0, SEEK_END );
		
		fprintf(arquivo,"%s\n",sequencia_atual.descricao);

		for(i=0;i<max_sequencia;i++)
		{
			fprintf(arquivo,"=% i\n",i);

			fprintf(arquivo,"+ ENTRADAS\n");

			for(j=0;j<nro_entradas;j++)
				fprintf(arquivo,"%i",int(sequencia_atual.e[i][j].valor));

			fprintf(arquivo,"\n");

			fprintf(arquivo,"- SAIDAS\n");

			for(j=0;j<nro_saidas;j++)
				fprintf(arquivo,"%i",int(sequencia_atual.s[i][j].esperado));

			fprintf(arquivo,"\n");

		}

		//for(i=0;i<nro_entradas;i++)

		fprintf(arquivo, "\n");
		fclose(arquivo);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  init_sequencia                                //
//                                                                     //
// CARACTERISTICAS    :  zerar a sequencia atual                       //
// entradas da rede                                                    //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  06/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void init_sequencia()
{
	int i, j;

	for(i=0;i<max_sequencia;i++)
		for(j=0;j<nro_entradas;j++)
			sequencia_atual.e[i][j].valor=no_data;

	for(i=0;i<max_sequencia;i++)
		for(j=0;j<nro_saidas;j++)
		{
			sequencia_atual.s[i][j].esperado=no_data;
			sequencia_atual.s[i][j].valor=no_data;
			sequencia_atual.s[i][j].ativacao[mais]=no_data;
			sequencia_atual.s[i][j].ativacao[menos]=no_data;
		}

	sequencia_atual.descricao="VOID";

	sequencia_atual.posicao=0;
	sequencia_atual.numero=0;
}

int devolve_y_seq(int valor)
{
	switch(valor)
	{
	case 0: return 6;	// entradas
	case 1: return 7;
	case 2: return 8;
	case 3: return 9;
	case 4: return 10;
	case 5: return 11;
	case 6: return 12;
	case 7: return 13;
	case 8: return 14;
	case 9: return 15;
	case 10: return 16;
	
	case 11: return 20;	// saidas
	case 12: return 21;
	case 13: return 22;
	case 14: return 23;
	case 15: return 24;
	case 16: return 25;
	case 17: return 26;
	case 18: return 27;
	case 19: return 28;
	case 20: return 29;

	case 21: return 31;
	case 22: return 32;
	case 23: return 33;
	case 24: return 34;
	case 25: return 35;
	case 26: return 36;
	case 27: return 37;
	case 28: return 38;
	case 29: return 39;
	case 30: return 40;
	}
}

void imprima_valor_atual_conjunto(int pos)
{
	int mem;

	mem=sequencia_atual.posicao;

	gotoxy(12,devolve_y_seq(pos));

	if(pos<nro_entradas)
	{
		if(sequencia_atual.e[mem][pos].valor==0.0)
		{
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_DARKGRAY  );
			printf("  DESLIGADO (0)  ");
		} else
		{
			setcolor( XFOREGROUND_YELLOW | XBACKGROUND_DARKGRAY  );
			printf("   LIGADO (1)    ");
		}
	} else
	{
		pos=pos-nro_entradas;
		if(sequencia_atual.s[mem][pos].esperado==0.0)
		{
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_DARKGRAY  );
			printf("  DESLIGADO (0)  ");
		} else
		{
			setcolor( XFOREGROUND_YELLOW | XBACKGROUND_DARKGRAY  );
			printf("   LIGADO (1)    ");
		}
	}
}

void mude_valor_seq(int pos)
{
	int mem;

	mem=sequencia_atual.posicao;

	if(pos<nro_entradas)
	{
		if(sequencia_atual.e[mem][pos].valor==0.0)
			sequencia_atual.e[mem][pos].valor=1.0; else
			sequencia_atual.e[mem][pos].valor=0.0;
	} else
	{
		pos=pos-nro_entradas;
		
		if(sequencia_atual.s[mem][pos].esperado==0.0)
			sequencia_atual.s[mem][pos].esperado=1.0; else
			sequencia_atual.s[mem][pos].esperado=0.0;
	}
}

void altera_conjunto_atual()
{
	int cursor_seq, i;

	char key;

	cursor_seq=0;

	janelao();

	gotoxy(65,3);

	printf("CONJUNTO %i ",sequencia_atual.posicao+1);

	gotoxy(2,3);

	if(sequencia_atual.numero==0)
		sequencia_atual.numero=1;

	printf("SEQUENCIA %i ",sequencia_atual.numero);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

	center(54," TECLAS: [CurUp] VALOR ACIMA, [CurDown] VALOR ABAIXO,              ");

	center(55,"         [CurRight][CurLeft] MUDAR VALOR, [enter] CONFIRMAR        ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );
	
	center(2,"ALTERACAO DO CONJUNTO ATUAL");

	setcolor( XBACKGROUND_WHITE | XFOREGROUND_BLACK );

	center(4," CONDICOES ");

	center(18," RESULTADOS ");

	key=' ';

	if(sequencia_atual.e[sequencia_atual.posicao][0].valor==no_data)
	{
		for(i=0;i<nro_entradas;i++) 
			sequencia_atual.e[sequencia_atual.posicao][i].valor=0.0;

		for(i=0;i<nro_saidas;i++)
			sequencia_atual.s[sequencia_atual.posicao][i].esperado=0.0;
	}

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY );

	for(i=0;i<(nro_entradas+nro_saidas);i++)
	{
		gotoxy(13,devolve_y_seq(i));
		imprima_valor_atual_conjunto(i);
	}

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	for(i=0;i<nro_entradas;i++)
	{
		gotoxy(7,devolve_y_seq(i));
		if(i<10)
			printf("(0%i) ",i); else
			printf("(%i) ",i);	

		gotoxy(30,devolve_y_seq(i));
		devolve_descricao_entrada(i);
	}

	for(i=0;i<nro_saidas;i++)
	{
		gotoxy(7,devolve_y_seq(i+nro_entradas));
		if(i<10)
			printf("(0%i) ",i); else
			printf("(%i) ",i);	

		gotoxy(30,devolve_y_seq(i+nro_entradas));
		devolve_descricao_saida(i);
	}

	while(key!=char(13))
	{
		imprima_valor_atual_conjunto(cursor_seq);
		
		setcolor( XFOREGROUND_DARKGRAY | XBACKGROUND_WHITE );

		gotoxy(12,devolve_y_seq(cursor_seq));

		printf(">");

		gotoxy(28,devolve_y_seq(cursor_seq));

		printf("<");

		key=getch();

		imprima_valor_atual_conjunto(cursor_seq);

		switch(key)
		{
		case char(72): if(cursor_seq>0) cursor_seq--; break;
		case char(80): if(cursor_seq<(nro_entradas+nro_saidas-1)) cursor_seq++; break;
		case char(75): mude_valor_seq(cursor_seq); break;
		case char(77): mude_valor_seq(cursor_seq); break;
		}
	}

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  gerencia_sequencia_atual                      //
//                                                                     //
// CARACTERISTICAS    :  faz um dump dos elementos atuais              //
// entradas da rede                                                    //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  06/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void gerencia_sequencia_atual()
{
	int dump_i, i;

	char key;

	key='+';

	while(key!=char(27))
	{
		
		janelao();
	
		center(2,"PROGRAMACAO SEQUENCIAS");

		gotoxy(0,5);

		setcolor( XBACKGROUND_WHITE | XFOREGROUND_BLACK );

		center(5," CONDICOES ");

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		printf("\n\n");

		if(sequencia_atual.e[sequencia_atual.posicao][0].valor==no_data)
		{
			printf(" *** MARCA DE ULTIMO CONJUNTO ***\n");
		}

		for(dump_i=0;dump_i<nro_entradas;dump_i++)
		{
			if(sequencia_atual.e[sequencia_atual.posicao][dump_i].valor==1)
			{
				printf(" (%i) ",dump_i);
				devolve_descricao_entrada(dump_i); 
				printf("\n");
			}
		}

		setcolor( XBACKGROUND_WHITE | XFOREGROUND_BLACK );

		center(19," RESULTADOS ");

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		printf("\n\n");

		if(sequencia_atual.s[sequencia_atual.posicao][0].esperado==no_data)
			printf(" *** MARCA DE ULTIMO CONJUNTO ***\n");

		for(dump_i=0;dump_i<nro_saidas;dump_i++)
		{
			elementos++;

			if(sequencia_atual.s[sequencia_atual.posicao][dump_i].esperado==1)
			{
				printf(" (%i) ",dump_i);
		
			devolve_descricao_saida(dump_i);

			printf(";\n");
			}

			if(dump_i==9) printf("\n");
		
		}

		gotoxy(65,3);

		printf("CONJUNTO %i ",sequencia_atual.posicao+1);

		gotoxy(2,3);

		if(sequencia_atual.numero==0)
			sequencia_atual.numero=1;

		printf("SEQUENCIA %i ",sequencia_atual.numero);

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

		center(53," TECLAS: [CurRight] PROXIMO CONJUNTO, [CurLeft] CONJUNTO ANTERIOR, ");

		center(54,"         [l] LE PROXIMA SEQUENCIA, [1] LE PRIMEIRA SEQUENCIA,      ");

		center(55,"         [d] DELETAR SEQUENCIA, [u] MARCA COMO ULTIMO CONJUNTO,    ");
		
		center(56,"         [a] ALTERA CONJUNTO, [g] GRAVAR SEQUENCIA, [esc] SAIR     ");

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		key=getch();

		switch(key)
		{
		case char(77): if(sequencia_atual.posicao<max_sequencia-1) sequencia_atual.posicao++; break;
		case char(75): if(sequencia_atual.posicao>0) sequencia_atual.posicao--; break;
		case 'l': le_sequencia_atual(); break;
		case 'd': init_sequencia(); break;
		case '1': fseek(arquivo,0,SEEK_SET); 
				sequencia_atual.numero=0;
			    le_sequencia_atual(); break;
		case 'a': altera_conjunto_atual(); break;
		case 'u': for(i=0;i<nro_entradas;i++) 
					sequencia_atual.e[sequencia_atual.posicao][i].valor=no_data;
					for(i=0;i<nro_saidas;i++)
					sequencia_atual.s[sequencia_atual.posicao][i].esperado=no_data; 
					break;
		case 'g': fclose(arquivo);
				  grava_sequencia_atual();
				  abre_arquivo();
				  init_sequencia();
				  break;
		}
	}

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  situacao_para_sequencia                       //
//                                                                     //
// CARACTERISTICAS    :  replica a situacao atual (entradas e saidas)  //
// para a sequencia atual                                              //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  06/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void situacao_para_sequencia(int index_formiga)
{
	int i;

	DEBUG_DOZE_IFS d12;

	//if(sequencia_atual.e[max_sequencia-1][0].valor==5)
	//{
		rede_backup=rede[index_formiga];

		situacao_para_entradas(index_formiga);

		d12=doze_ifs(index_formiga,true,true);

		gera_saidas_desejadas(index_formiga,d12.caso);

		if(parametros.log_ligado) fprintf(debug,"<gravacao seq.> @@@@@@@@@@@@@@@@@@@@@@@@ ONLINE @@@@@@@@@@@@@@@@@@@@@\n");

		if(parametros.log_ligado) fprintf(debug,"<gravacao seq.> (SEQUENCIA ATUAL) CONJUNTO *** %i *** \n",sequencia_atual.numero,sequencia_atual.posicao);

		for(i=0;i<nro_entradas;i++)
		{
			sequencia_atual.e[sequencia_atual.posicao][i].valor=rede[index_formiga].entrada[i].valor;
			if(rede[index_formiga].entrada[i].valor!=0.0)
			{
				if(parametros.log_ligado) fprintf(debug,"<gravacao seq.> ENTRADA %i VALOR %f ->",i,rede[index_formiga].entrada[i].valor);
				explica_linha_debug(i);
				if(parametros.log_ligado) fprintf(debug,"\n");
			}
		}

		for(i=0;i<nro_saidas;i++)
		{
			sequencia_atual.s[sequencia_atual.posicao][i].esperado=rede[index_formiga].saida[i].esperado;
			if(rede[index_formiga].saida[i].esperado!=0.0)
			{
				if(parametros.log_ligado) fprintf(debug,"<gravacao seq.> SAIDA %i VALOR %f ->",i,rede[index_formiga].saida[i].esperado);
				explica_saida_debug(i);
				if(parametros.log_ligado) fprintf(debug,"\n");
			}
		}

		rede[index_formiga]=rede_backup;

		if(sequencia_atual.posicao<max_sequencia) sequencia_atual.posicao++;
	//}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  herda_informacoes                             //
//                                                                     //
// CARACTERISTICAS    :  transfere as informacoes dos pais para os     //
// respectivos filhos                                                  //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  03/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void herda_informacoes(int index_rede)
{
	int pai_pos, filho_pos, filho_marcacao;

	filho_marcacao=49;

	for(pai_pos=0;pai_pos<49;pai_pos++)
	{
		for(filho_pos=0;filho_pos<filho[pai_pos];filho_pos++)
		{
			rede[index_rede].entrada[filho_marcacao].valor=rede[index_rede].entrada[pai_pos].valor;

			filho_marcacao++;
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

void propagacao_escondidos(int index_rede, int fase)
{
	int i, j, k;

	for(j=0;j<parametros.neuronios_escondidos;j++)
	{
		rede[index_rede].neuronio[j].valor=0.0; // inicializacao

		for(i=0;i<nro_entradas;i++)  // soma com as entradas e pesos
		{
			rede[index_rede].neuronio[j].valor=
				float(rede[index_rede].neuronio[j].valor+
				(rede[index_rede].entrada[i].valor *
				rede[index_rede].sinapse_EN[i][j].valor *
				1.0));
		}

		for(k=0;k<nro_saidas;k++)  // soma com as ativacoes das saidas
		{
			rede[index_rede].neuronio[j].valor=
				float(rede[index_rede].neuronio[j].valor+
				(rede[index_rede].sinapse_NS[j][k].valor *
				rede[index_rede].saida[k].ativacao[fase] *
				parametros.percentagem_recorrencia *
				1.0));
		}

		rede[index_rede].neuronio[j].ativacao[fase]=sigmoide(rede[index_rede].neuronio[j].valor);   // estabelecer ativacao
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

void propagacao_saidas(int index_rede, int fase)
{
	int j, k;

	if(fase==mais)
		for(k=0;k<nro_saidas;k++)
			rede[index_rede].saida[k].ativacao[mais]=rede[index_rede].saida[k].esperado;  // na fase mais sao os valores os esperados!!

	if(fase==menos)
		for(k=0;k<nro_saidas;k++)
		{
			rede[index_rede].saida[k].valor=0.0;   // inicializa a soma

			for(j=0;j<parametros.neuronios_escondidos;j++)  // soma com as ativacoes dos escondidos
				rede[index_rede].saida[k].valor=
				float(rede[index_rede].saida[k].valor +
				(rede[index_rede].sinapse_NS[j][k].valor *
				rede[index_rede].neuronio[j].ativacao[menos] *   // *** CHECAR ESTE PONTO 
				1.0));

			rede[index_rede].saida[k].ativacao[menos]=sigmoide(rede[index_rede].saida[k].valor); // ativacao
			
		}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  maior_saida_rede                              //
//                                                                     //
// CARACTERISTICAS    :  devolve o numero da saida com maior potencial //
// de ativacao (a saida a ser escolhida)                               //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  01/11/03                                      //
/////////////////////////////////////////////////////////////////////////

int maior_saida_rede(int index_rede)
{
	float flag;
	int pos, i;

	pos=0;
	flag=rede[index_rede].saida[0].ativacao[menos];

	for(i=0;i<10;i++)
	{
		if(rede[index_rede].saida[i].ativacao[menos]>flag)
		{
			flag=rede[index_rede].saida[i].ativacao[menos];
			pos=i;
		}
	}

	return pos;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  funcionar_rede                                //
//                                                                     //
// CARACTERISTICAS    :  faz um passo de funcionamento nao supervisio- //
// nado da rede                                                        //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  01/11/03                                      //
/////////////////////////////////////////////////////////////////////////

int funcionar_rede(int index_rede)
{
	int flag;

	char *temp;

	if(parametros.log_ligado)
	{
		fprintf(debug,"<REDE> *** FUNCIONAMENTO DA REDE ***\n");
		fprintf(debug,"<REDE> (FORMIGA %i)\n",index_rede);
		fprintf(debug,"<REDE>\n");
		fprintf(debug,"<REDE> ANTES:\n");
		dump_curto_entradas_saidas(index_rede);
	}

	propagacao_escondidos(index_rede,menos);

	propagacao_saidas(index_rede,menos);

	flag=(maior_saida_rede(index_rede)+1);

	if(parametros.log_ligado)
	{
		fprintf(debug,"<REDE> DEPOIS:\n");
		dump_curto_entradas_saidas(index_rede);
		fprintf(debug,"<REDE> R E S U L T A D O :  %i \n",flag);
		fprintf(debug,"<REDE>\n");
		fprintf(debug,"<REDE>\n");
	}

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  funcionar_aprendizado                         //
//                                                                     //
// CARACTERISTICAS    :  executa um passo de aprendizado supervisionado//
// na rede                                                             //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  01/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void funcionar_aprendizado(int index_rede)
{
	//dump_valores_formiga_100();

	//situacao_para_entradas(index_formiga);

	//gera_saidas_desejadas(index_formiga);

	propagacao_saidas(index_rede,mais);

//	for(master=0;master<parametros.passos_recorrencia;master++)
//	{

		propagacao_escondidos(index_rede,mais);

		propagacao_escondidos(index_rede,menos);

		propagacao_saidas(index_rede,menos);

		aprendizado_generec(index_rede);

//	}

	//rede_dump();
}

void explica_saida_debug(int linha)
{
	switch(linha+1)
	{
		case 1: if(parametros.log_ligado) fprintf(debug,"FUGIR_PERIGO"); break;
		case 2: if(parametros.log_ligado) fprintf(debug,"FUGIR_ALOMONIO"); break;
		case 3: if(parametros.log_ligado) fprintf(debug,"VOLTAR_NORMALIDADE"); break;
		case 4: if(parametros.log_ligado) fprintf(debug,"EVITAR_COLISAO"); break;
		case 5: if(parametros.log_ligado) fprintf(debug,"PEGAR_COMIDA"); break;
		case 6: if(parametros.log_ligado) fprintf(debug,"SAIR_ALERTA"); break;
		case 7: if(parametros.log_ligado) fprintf(debug,"SAIR_CAMINHO_RUIM"); break;
		case 8: if(parametros.log_ligado) fprintf(debug,"ENTRAR_NINHO"); break;
		case 9: if(parametros.log_ligado) fprintf(debug,"SEGUIR_ATRACAO_FRENTE (OLFATO)"); break;
		case 10: if(parametros.log_ligado) fprintf(debug,"SEGUIR_ATRACAO_AMBIGUA (OLFATO)"); break;
		case 11: if(parametros.log_ligado) fprintf(debug,"SEGUIR_ATRACAO_DIREITA (OLFATO)"); break;
		case 12: if(parametros.log_ligado) fprintf(debug,"SEGUIR_ATRACAO_ESQUERDA (OLFATO)"); break;
		case 13: if(parametros.log_ligado) fprintf(debug,"SEGUIR_ATRACAO_FRENTE (VISAO)"); break;
		case 14: if(parametros.log_ligado) fprintf(debug,"SEGUIR_ATRACAO_FRENTE (VISAO ABAIXO)"); break;
		case 15: if(parametros.log_ligado) fprintf(debug,"BUSCAR_SINAL"); break;
		case 16: if(parametros.log_ligado) fprintf(debug,"SEGUIR_ATRACAO_AMBIGUA (VISAO)"); break;
		case 17: if(parametros.log_ligado) fprintf(debug,"SEGUIR_ATRACAO_DIREITA (VISAO)"); break;
		case 18: if(parametros.log_ligado) fprintf(debug,"SEGUIR_ATRACAO_ESQUERDA (VISAO)"); break;
	}
}

void explica_linha_debug(int linha)
{
	switch(linha)
	{
	case 0: if(parametros.log_ligado) fprintf(debug,"objeto_frente==longe"); break;
	case 1: if(parametros.log_ligado) fprintf(debug,"objeto_frente==perto"); break;
	case 2: if(parametros.log_ligado) fprintf(debug,"objeto_frente==muito_perto_baixo || muito_perto_alto "); break;
	case 3: if(parametros.log_ligado) fprintf(debug,"estomago_social<10"); break;
	case 4: if(parametros.log_ligado) fprintf(debug,"objeto_direita==sem_presenca"); break;
	case 5: if(parametros.log_ligado) fprintf(debug,"objeto_direita==longe"); break;
	case 6: if(parametros.log_ligado) fprintf(debug,"objeto_direita==perto"); break;
	case 7: if(parametros.log_ligado) fprintf(debug,"objeto_direita==muito_perto_baixo"); break;
	case 8: if(parametros.log_ligado) fprintf(debug,"objeto_direita<=longe"); break;
	case 9: if(parametros.log_ligado) fprintf(debug,"objeto_direita<=perto"); break;
	case 10: if(parametros.log_ligado) fprintf(debug,"objeto_esquerda==sem_presenca"); break;
	case 11: if(parametros.log_ligado) fprintf(debug,"objeto_esquerda==longe"); break;
	case 12: if(parametros.log_ligado) fprintf(debug,"objeto_esquerda==perto"); break;
	case 13: if(parametros.log_ligado) fprintf(debug,"objeto_esquerda==muito_perto_baixo"); break;
	case 14: if(parametros.log_ligado) fprintf(debug,"objeto_esquerda<=longe"); break;
	case 15: if(parametros.log_ligado) fprintf(debug,"objeto_esquerda<=perto"); break;
	case 16: if(parametros.log_ligado) fprintf(debug,"objeto_acima==muito_perto"); break;
	case 17: if(parametros.log_ligado) fprintf(debug,"objeto_abaixo!=sem_presenca"); break;
	case 18: if(parametros.log_ligado) fprintf(debug,"objeto_chegando_acima==true"); break;
	case 19: if(parametros.log_ligado) fprintf(debug,"tato==predador"); break;
	case 20: if(parametros.log_ligado) fprintf(debug,"tato==outra"); break;
	case 21: if(parametros.log_ligado) fprintf(debug,"tato==agua"); break;
	case 22: if(parametros.log_ligado) fprintf(debug,"tato>comida0"); break;
	case 23: if(parametros.log_ligado) fprintf(debug,"tato!=agua"); break;
	case 24: if(parametros.log_ligado) fprintf(debug,"tato==ninho"); break;
	case 25: if(parametros.log_ligado) fprintf(debug,"tato==parede"); break;
	case 26: if(parametros.log_ligado) fprintf(debug,"tato==terra"); break;
	case 27: if(parametros.log_ligado) fprintf(debug,"tato==sem_presenca"); break;
	case 28: if(parametros.log_ligado) fprintf(debug,"tato==margem"); break;
	case 29: if(parametros.log_ligado) fprintf(debug,"audicao>10"); break;
	case 30: if(parametros.log_ligado) fprintf(debug,"estomago>10"); break;
	case 31: if(parametros.log_ligado) fprintf(debug,"estomago<=10"); break;
	case 32: if(parametros.log_ligado) fprintf(debug,"estomago_social<89"); break;
	case 33: if(parametros.log_ligado) fprintf(debug,"estomago_social>=10"); break;
	case 34: if(parametros.log_ligado) fprintf(debug,"alomonio==positivo_e_crescente"); break;
	case 35: if(parametros.log_ligado) fprintf(debug,"alomonio[0]==0"); break;
	case 36: if(parametros.log_ligado) fprintf(debug,"alomonio==positivo_e_decrescente"); break;
	case 37: if(parametros.log_ligado) fprintf(debug,"cheiro_ninho==positivo_e_crescente"); break;
	case 38: if(parametros.log_ligado) fprintf(debug,"olfato_frente_maior==feromonio"); break;
	case 39: if(parametros.log_ligado) fprintf(debug,"olfato_frente_maior==ninho"); break;
	case 40: if(parametros.log_ligado) fprintf(debug,"olfato_frente_maior==alimento"); break;
	case 41: if(parametros.log_ligado) fprintf(debug,"olfato_direita_maior==feromonio"); break;
	case 42: if(parametros.log_ligado) fprintf(debug,"olfato_direita_maior==ninho"); break;
	case 43: if(parametros.log_ligado) fprintf(debug,"olfato_direita_maior==alimento"); break;
	case 44: if(parametros.log_ligado) fprintf(debug,"olfato_esquerda_maior==feromonio"); break;
	case 45: if(parametros.log_ligado) fprintf(debug,"olfato_esquerda_maior==ninho"); break;
	case 46: if(parametros.log_ligado) fprintf(debug,"olfato_esquerda_maior==alimento"); break;
	case 47: if(parametros.log_ligado) fprintf(debug,"alerta==true"); break;
	case 48: if(parametros.log_ligado) fprintf(debug,"coord_z==2"); break;
	}

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  explica_linha                                 //
//                                                                     //
// CARACTERISTICAS    :  explica o que significa a entrada "linha"     //
// ligada                                                              //
// PARAMETROS         :  numero da entrada                             //
// OBSERVACOES        :  (para desenvolvimento)                        //
//                                                                     //
// ULTIMA ATUALIZACAO :  02/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void explica_linha(int linha)
{
	switch(linha)
	{
	case 0: printf("objeto_frente==longe"); break;
	case 1: printf("objeto_frente==perto"); break;
	case 2: printf("objeto_frente==muito_perto_baixo || muito_perto_alto "); break;
	case 3: printf("estomago_social<10"); break;
	case 4: printf("objeto_direita==sem_presenca"); break;
	case 5: printf("objeto_direita==longe"); break;
	case 6: printf("objeto_direita==perto"); break;
	case 7: printf("objeto_direita==muito_perto_baixo"); break;
	case 8: printf("objeto_direita<=longe"); break;
	case 9: printf("objeto_direita<=perto"); break;
	case 10: printf("objeto_esquerda==sem_presenca"); break;
	case 11: printf("objeto_esquerda==longe"); break;
	case 12: printf("objeto_esquerda==perto"); break;
	case 13: printf("objeto_esquerda==muito_perto_baixo"); break;
	case 14: printf("objeto_esquerda<=longe"); break;
	case 15: printf("objeto_esquerda<=perto"); break;
	case 16: printf("objeto_acima==muito_perto"); break;
	case 17: printf("objeto_abaixo!=sem_presenca"); break;
	case 18: printf("objeto_chegando_acima==true"); break;
	case 19: printf("tato==predador"); break;
	case 20: printf("tato==outra"); break;
	case 21: printf("tato==agua"); break;
	case 22: printf("tato>comida0"); break;
	case 23: printf("tato!=agua"); break;
	case 24: printf("tato==ninho"); break;
	case 25: printf("tato==parede"); break;
	case 26: printf("tato==terra"); break;
	case 27: printf("tato==sem_presenca"); break;
	case 28: printf("tato==margem"); break;
	case 29: printf("audicao>10"); break;
	case 30: printf("estomago>10"); break;
	case 31: printf("estomago<=10"); break;
	case 32: printf("estomago_social<89"); break;
	case 33: printf("estomago_social>=10"); break;
	case 34: printf("alomonio==positivo_e_crescente"); break;
	case 35: printf("alomonio[0]==0"); break;
	case 36: printf("alomonio==positivo_e_decrescente"); break;
	case 37: printf("cheiro_ninho==positivo_e_crescente"); break;
	case 38: printf("olfato_frente_maior==feromonio"); break;
	case 39: printf("olfato_frente_maior==ninho"); break;
	case 40: printf("olfato_frente_maior==alimento"); break;
	case 41: printf("olfato_direita_maior==feromonio"); break;
	case 42: printf("olfato_direita_maior==ninho"); break;
	case 43: printf("olfato_direita_maior==alimento"); break;
	case 44: printf("olfato_esquerda_maior==feromonio"); break;
	case 45: printf("olfato_esquerda_maior==ninho"); break;
	case 46: printf("olfato_esquerda_maior==alimento"); break;
	case 47: printf("alerta==true"); break;
	case 48: printf("coord_z==2"); break;
	}
}

float devolve_taxa_erro(int index_rede)
{
	float temp, flag;

	int i;

	flag=0;

	for(i=0;i<nro_saidas;i++)
	{
		temp=rede[index_rede].saida[i].ativacao[menos]-rede[index_rede].saida[i].esperado;
		if(temp<0) temp=-temp;
		flag=flag+temp;
	}

	return flag;
}

void avaliar_aprendizado()
{
	float taxa;

	int passos, um_bloco, total;

	int erros, saida_rede;

	char esc;

	int j;

	limpa_dropdown();

	erros=0;

	total=0;

	passos=0;

	um_bloco=10;

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	center(26,"                                  ");

	center(26," AVALIACAO DO APRENDIZADO ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	center(27,"                                  ");
	center(28,"                                  ");
	center(29,"                                  ");
	center(30,"                                  ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

	center(30," ESC ");

	gotoxy(25,28);

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY );

	printf("                              ");

	gotoxy(25,28);

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_LIGHTGREEN );

	esc=' '; 

	//gotoxy(1,1);

	if(parametros.log_ligado) fprintf(debug,"<avaliacao> COMECO DA AVALIACAO\n");

	while(passos<100 && esc!=char(27))
	{
		passos++;

		if(parametros.log_ligado) fprintf(debug,"<avaliacao> RODADA %i:\n",passos);

		if(kbhit()) esc=getch();

		//gera_situacao_aleatoria();

		le_sequencia_atual();

		flush_saidas(0);

		if(parametros.log_ligado) fprintf(debug,"<avaliacao> S E Q U E N C I A  <<< %i >>>\n",sequencia_atual.numero);

		for(i=0;i<max_sequencia;i++)
		{
			if(sequencia_atual.e[i][0].valor!=5)
			{

			dados_treinamento_para_rede(0,i);

			saida_rede=funcionar_rede(0);

			if(sequencia_atual.s[i][saida_rede-1].esperado!=1.0)
			{
				erros++;

				if(parametros.log_ligado) fprintf(debug,"<avaliacao> +++ E R R O +++ (%i) CONJUNTO %i SEQUENCIA %i .\n",i,erros,sequencia_atual.numero);

				for(j=0;j<nro_entradas;j++)
				{
					if(sequencia_atual.e[i][j].valor!=0.0)
					{
						if(parametros.log_ligado) fprintf(debug,"<avaliacao> ENTRADA[%i]=%f ",j,sequencia_atual.e[i][j].valor);
						explica_linha_debug(j);
						if(parametros.log_ligado) fprintf(debug,"\n");
					}
				}
				
				if(parametros.log_ligado) fprintf(debug,"<avaliacao> SAIDA CORRETA: [%i] ",saida_rede-1);

				explica_saida_debug(saida_rede-1);

				if(parametros.log_ligado) fprintf(debug,"\n");

				for(j=0;j<nro_saidas;j++)
					if(sequencia_atual.s[i][j].esperado!=0.0)
					{
						if(parametros.log_ligado) fprintf(debug,"<avaliacao> SAIDA REAL [%i]=%f ",j,sequencia_atual.s[i][j].esperado);
						explica_saida_debug(j);
						if(parametros.log_ligado) fprintf(debug,"\n");
					}

				
			} else 
			{
				if(parametros.log_ligado) fprintf(debug,"<avaliacao> CONJUNTO %i [%i]=",i,saida_rede-1);
				explica_saida_debug(saida_rede-1);
				if(parametros.log_ligado) fprintf(debug," !! OK !!\n");
			}

						total++;

			}
		}

		flush_saidas(0);

		//if(saida_rede!=(saida_esperada+1)) 
		//{
		//	erros++;
		//	printf("e:%i r:%i, ",saida_esperada+1,saida_rede);
		//}

		if((passos % um_bloco)==0)
			printf("   ");
	}

	if(parametros.log_ligado) fprintf(debug,"<avaliacao> FIM DA AVALIACAO\n");

	taxa=float(erros*100.0 / total*1.0);

	if(parametros.log_ligado) fprintf(debug,"<avaliacao> RESULTADO=%f PORCENTOS DE ERROS\n<avaliacao> ****\n",taxa);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	center(26,"                                  ");

	center(26," RESULTADO DA AVALIACAO ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	center(27,"                                  ");
	center(28,"                                  ");
	center(29,"                                  ");
	center(30,"                                  ");

	gotoxy(28,28);

	printf("Taxa de Erro ");
	
	if(taxa<10) setcolor( XBACKGROUND_LIGHTGREEN | XFOREGROUND_BLACK ); else
		if(taxa<20) setcolor( XBACKGROUND_YELLOW | XFOREGROUND_BLACK ); else
			setcolor( XBACKGROUND_LIGHTRED | XFOREGROUND_BLACK );
	
	printf(" %f ",taxa);

	setcolor( XBACKGROUND_LIGHTGRAY | XFOREGROUND_BLACK );

	printf(" %");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

	center(30," OK ");

	getch();

	restore();

	dropdown();
}

bool todos_casos_ok()
{
	int i;

	for(i=0;i<45;i++)
		if(aprendizado_ok[i]==false) return false;

	return true;
}

int devolve_casos_ok()
{
	int i, flag;

	flag=0;

	for(i=0;i<45;i++)
		if(aprendizado_ok[i]) flag++;

	return flag;
}

void dados_treinamento_para_rede(int index_rede, int conjunto)
{
	int i;

	if(sequencia_atual.e[conjunto][0].valor!=no_data)
	{

		for(i=0;i<nro_entradas;i++)
			rede[index_rede].entrada[i].valor=sequencia_atual.e[conjunto][i].valor;

//		herda_informacoes();

		for(i=0;i<nro_saidas;i++)
			rede[index_rede].saida[i].esperado=sequencia_atual.s[conjunto][i].esperado;
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
	char esc;

	int um_bloco, i, passos;

	limpa_dropdown();

	um_bloco=int(parametros.limite_generec/10);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	center(26,"                                  ");

	center(26," TREINAMENTO DA REDE ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	center(27,"                                  ");
	center(28,"                                  ");
	center(29,"                                  ");
	center(30,"                                  ");

	gotoxy(24,30);

	setcolor( XFOREGROUND_DARKGRAY | XBACKGROUND_LIGHTGRAY );

	printf("%i passos",parametros.limite_generec);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

	center(30," ESC ");

	gotoxy(25,28);

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY );

	printf("                              ");

	gotoxy(25,28);

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_LIGHTGREEN );

	esc=' '; 

	passos=0;

	flush_saidas(0);

	while(passos<parametros.limite_generec && esc!=char(27))
	{
		le_sequencia_atual();

		for(i=0;i<max_sequencia;i++)
		{
			dados_treinamento_para_rede(0,i);

			funcionar_aprendizado(0);
		}

		flush_saidas(0);

		passos++;

		if(kbhit()) esc=getch();

		if((passos % um_bloco)==0)
			printf("   ");

	}

	avaliar_aprendizado();

	restore();

	dropdown();
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

void rede_testar_propagacao(int index_rede)
{
	if(caixa_mensagem(pergunta,0,caixa_padrao,"Colocar valores DUMMY para teste?"))
	{
		rede[index_rede].entrada[0].valor=1.0;
		rede[index_rede].entrada[2].valor=1.0;	// rede com no minimo 3 entradas!!!

		rede[index_rede].saida[0].esperado=1.0; // ... e no minimo 3 saidas
		rede[index_rede].saida[2].esperado=1.0;
	}

	// propagacao conforme fluxo em rede recorrente

	// o sistema pergunta a cada etapa se um dump devera ser executado

	// a cada loop existe a possibilidade de interrupcao

	limpa_caixa();

	propagacao_saidas(index_rede,mais);

	if(caixa_mensagem(pergunta,0,caixa_padrao,"Saidas (Fase +) OK! Dump?"))
		rede_dump(index_rede); else limpa_caixa();

	propagacao_escondidos(index_rede,mais);

	if(caixa_mensagem(pergunta,0,caixa_padrao,"Escondidos (Fase +) OK! Dump?"))
		rede_dump(index_rede); else limpa_caixa();

	propagacao_saidas(index_rede,menos);
	
	if(caixa_mensagem(pergunta,0,caixa_padrao,"Saidas (Fase -) OK! Dump?"))
		rede_dump(index_rede); else limpa_caixa();

	propagacao_escondidos(index_rede,menos);

	if(caixa_mensagem(pergunta,0,caixa_padrao,"Escondidos (Fase -) OK! Dump?"))
		rede_dump(index_rede); else limpa_caixa();

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

void aprendizado_generec(int index_rede)    
{
	int i, j, k;

	float delta;

	for(i=0;i<nro_entradas;i++)		// Ajuste das sinapses entre entradas e camada escondida
	{
		for(j=0;j<parametros.neuronios_escondidos;j++)
		{
			delta=float(parametros.taxa_aprendizado*(rede[index_rede].neuronio[j].ativacao[mais]-rede[index_rede].neuronio[j].ativacao[menos])*rede[index_rede].entrada[i].valor*1.0);

			rede[index_rede].sinapse_EN[i][j].valor=rede[index_rede].sinapse_EN[i][j].valor+delta;
		}
	}

	for(j=0;j<parametros.neuronios_escondidos;j++)    // Ajuste das sinapses entre a camada escondida e as saidas
	{
		for(k=0;k<nro_saidas;k++)
		{
			delta=parametros.taxa_aprendizado*(rede[index_rede].saida[k].ativacao[mais]-rede[index_rede].saida[k].ativacao[menos])*rede[index_rede].neuronio[j].ativacao[menos];

			rede[index_rede].sinapse_NS[j][k].valor=rede[index_rede].sinapse_NS[j][k].valor+delta;
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

void rede_testar_aprendizado(int index_rede)
{
	int master;

	if(caixa_mensagem(pergunta,0,caixa_padrao,"Colocar valores DUMMY para teste?"))
	{
		rede[index_rede].entrada[0].valor=1.0;
		rede[index_rede].entrada[2].valor=1.0;	// rede com no minimo 3 entradas!!!

		rede[index_rede].saida[0].esperado=1.0; // ... e no minimo 3 saidas
		rede[index_rede].saida[2].esperado=1.0;
	}

	propagacao_saidas(index_rede,mais);

	for(master=0;master<1000;master++)
	{
		propagacao_escondidos(index_rede,menos);

		propagacao_saidas(index_rede,menos);

		propagacao_escondidos(index_rede,mais);

		aprendizado_generec(index_rede);
	}

	rede_dump(index_rede);
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
// NOME DA FUNCAO     :  dump_entradas_saidas                          //
//                                                                     //
// CARACTERISTICAS    :  dump apenas das entradas e saidas             //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  02/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void dump_entradas_saidas(int index_rede)
{
	rede_dump_entradas(index_rede);
	rede_dump_saidas(index_rede);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  rede_dump_saidas                              //
//                                                                     //
// CARACTERISTICAS    :  dump das saidas da rede                       //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  03/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void rede_dump_saidas(int index_rede)
{
		int dump_i;

	// *** DUMP DAS SAIDAS ***

	prepara_dump("SAIDAS");

	for(dump_i=0;dump_i<nro_saidas;dump_i++)
	{
		elementos++;

		if(dump_i<10) printf("[0%i]",dump_i); else printf("[%i]",dump_i);

		printf(" D=%f A+=",rede[index_rede].saida[dump_i].valor);
		
		if(rede[index_rede].saida[dump_i].ativacao[mais]<0.5)
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTRED ); else
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGREEN );

		printf("%f",rede[index_rede].saida[dump_i].ativacao[mais]);
		
		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		printf(" A-=");

		if(rede[index_rede].saida[dump_i].ativacao[menos]<0.5)
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTRED ); else
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGREEN );
		
		printf("%f",rede[index_rede].saida[dump_i].ativacao[menos]);

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		printf("; ");

		devolve_descricao_saida(dump_i);

		printf("\n\n");
		
		//if(elementos>(quebra/3))
		//{
		//	espera();
		//	prepara_dump("SAIDAS");
		//}
	}

	espera();

	limpa_janelao();

	restore();

	dropdown();

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  rede_dump_entradas                            //
//                                                                     //
// CARACTERISTICAS    :  dump das entradas da rede                     //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  03/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void rede_dump_entradas(int index_rede)
{
	int dump_i;

	// *** DUMP DAS ENTRADAS ***

	//dump_valores_formiga_100();

	prepara_dump("ENTRADAS");

	for(dump_i=0;dump_i<nro_entradas;dump_i++)
	{
		if(dump_i<10)
			printf("[0%i]=",dump_i); else
			printf("[%i]=",dump_i);
		
		if(rede[index_rede].entrada[dump_i].valor==1)
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGREEN ); else
					setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTRED );

		printf("%f",rede[index_rede].entrada[dump_i].valor);

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		printf("; ");

		devolve_descricao_entrada(dump_i);

		printf("\n\n");

	}

	espera();

	
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

void rede_dump(int index_rede)
{
		int dump_i, dump_j;

	rede_dump_entradas(index_rede);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	// *** DUMP DA PRIMEIRA CAMADA DE SINAPSES ***

	prepara_dump("SINAPSES ENTRE ENTRADAS E CAMADA ESCONDIDA");

	for(dump_i=0;dump_i<nro_entradas;dump_i++)
	{
		for(dump_j=0;dump_j<parametros.neuronios_escondidos;dump_j++)
		{
			elementos++;

			setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

			printf("[%i,%i]",dump_i,dump_j);

			setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );
			
			printf("=%f; ",rede[index_rede].sinapse_EN[dump_i][dump_j].valor);

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

		printf("  ");

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

		if(dump_i<10) 
			printf("[0%i]",dump_i); else
			printf("[%i]",dump_i);

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );
		
		printf(" D=%f A+=%f A-=%f; \n\n",rede[index_rede].neuronio[dump_i].valor,rede[index_rede].neuronio[dump_i].ativacao[mais],rede[index_rede].neuronio[dump_i].ativacao[menos]);

		if(elementos>(quebra/3))
		{
			espera();
			prepara_dump("NEURONIOS ESCONDIDOS");
		}
	}

	espera();

	// *** DUMP DA SEGUNDO CAMADA DE SINAPSES ***

	prepara_dump("SINAPSES ENTRE CAMADA ESCONDIDA E SAIDAS");

	for(dump_i=0;dump_i<parametros.neuronios_escondidos;dump_i++)
	{
		for(dump_j=0;dump_j<nro_saidas;dump_j++)
		{
			elementos++; 

			setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

			printf("[%i,%i]",dump_i,dump_j);

			setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );
			
			printf("=%f; ",rede[index_rede].sinapse_NS[dump_i][dump_j].valor);

			if(elementos>quebra)
			{
				espera();
				prepara_dump("SINAPSES ENTRE CAMADA ESCONDIDA E SAIDAS");
			}
		}
	}

	espera();

	rede_dump_saidas(index_rede);

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  dec_to_bin                                    //
//                                                                     //
// CARACTERISTICAS    :  converte um numero para binario               //
//                                                                     //
// PARAMETROS         :  numero decimal                                //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  30/10/03                                      //
/////////////////////////////////////////////////////////////////////////

NUMERO_BIN dec_to_bin(int numero_dec)
{
	NUMERO_BIN flag;

	short int pos;

	//printf("%i = ",numero_dec);

	for(pos=6;pos>=0;pos--)
	{
		if(numero_dec>=bin[pos])
		{
			flag.digit[pos]=1;
			numero_dec=numero_dec-bin[pos];
		} else
			flag.digit[pos]=0;
	}

	//for(pos=6;pos>=0;pos--)
	//	printf("%i ",flag.digit[pos]);

	//printf("\n");

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  init_bin                                      //
//                                                                     //
// CARACTERISTICAS    :  inicializa um erro para decodificar numeros   //
// binarios                                                            //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  apenas para facilidade                        //
//                                                                     //
// ULTIMA ATUALIZACAO :  30/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void init_bin()
{
	bin[0]=1;
	bin[1]=2;
	bin[2]=4;
	bin[3]=8;
	bin[4]=16;
	bin[5]=32;
	bin[6]=64;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  panico                                        //
//                                                                     //
// CARACTERISTICAS    :  determina se uma entrada de panico foi        //
// acionada                                                            //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//bool panico()
//{
//	if(rede.entrada[16].valor==1.0 ||	// veja explicacoes na tabela
//		rede.entrada[19].valor==1.0 ||
//		rede.entrada[29].valor==1.0 ||
//		rede.entrada[34].valor==1.0)
//		return true;

//	return false;
//}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  olfato                                        //
//                                                                     //
// CARACTERISTICAS    :  determina se uma entrada de olfato foi        //
// acionada                                                            //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//bool olfato()
//{
//	if(rede.entrada[36].valor==1.0 ||	// veja explicacoes na tabela
//		rede.entrada[37].valor==1.0 ||
//		rede.entrada[38].valor==1.0 ||
//		rede.entrada[39].valor==1.0 ||
//		rede.entrada[40].valor==1.0 ||
//		rede.entrada[41].valor==1.0 ||
//		rede.entrada[42].valor==1.0 ||
//		rede.entrada[43].valor==1.0 ||
//		rede.entrada[44].valor==1.0 ||
//		rede.entrada[45].valor==1.0 ||
//		rede.entrada[46].valor==1.0)
//		return true;

//	return false;
//}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  morrendo_de_fome                              //
//                                                                     //
// CARACTERISTICAS    :  determina se uma entrada de fome ou carga foi //
// acionada                                                            //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//bool morrendo_de_fome()
//{
//	if(rede.entrada[31].valor==1.0)	// veja explicacoes na tabela
//		return true;

//	return false;
//}

//bool com_carga()
//{
//	if(rede.entrada[33].valor==1.0)
//		return true;

//	return false;
//}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  visao                                         //
//                                                                     //
// CARACTERISTICAS    :  determina se uma entrada de visao foi         //
// acionada                                                            //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//bool visao()
//{
//	if(rede.entrada[0].valor==1.0 ||	// veja explicacoes na tabela
//		rede.entrada[1].valor==1.0 ||
//		rede.entrada[2].valor==1.0 ||
//		rede.entrada[5].valor==1.0 ||
//		rede.entrada[6].valor==1.0 ||
//		rede.entrada[7].valor==1.0 ||
//		rede.entrada[8].valor==1.0 ||
//		rede.entrada[9].valor==1.0 ||
//		rede.entrada[11].valor==1.0 ||
//		rede.entrada[12].valor==1.0 ||
//		rede.entrada[13].valor==1.0 ||
//		rede.entrada[14].valor==1.0 ||
//		rede.entrada[15].valor==1.0 ||
//		rede.entrada[17].valor==1.0 ||
//		rede.entrada[18].valor==1.0)
//		return true;

//	return false;
//}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  exploracao                                    //
//                                                                     //
// CARACTERISTICAS    :  determina se uma entrada de exploracao foi    //
// acionada                                                            //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//bool exploracao()
//{
//	if(rede.entrada[3].valor==1.0 &&
//		rede.entrada[30].valor==1.0)
//		return true;
//
//	return false;
//}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  elimina_tudo                                  //
//                                                                     //
// CARACTERISTICAS    :  elimina todos os "1.0"s nas entradas fora as  //
// de panico                                                           //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//void elimina_tudo()
//{
//	int i;
//
//	for(i=0;i<nro_entradas;i++)	// veja explicacoes na tabela
//	{
//		if(i!=16 && i!=19 && i!=29 && i!=34) // apenas os valores panico ficam
//			rede.entrada[i].valor=0.0;
//	}
//}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  elimina_visao                                 //
//                                                                     //
// CARACTERISTICAS    :  elimina todos os valores positivos nas        //
// entradas de visao                                                   //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//void elimina_visao()
//{
//	rede.entrada[0].valor=0.0;	// veja explicacoes na tabela
//	rede.entrada[1].valor=0.0;
//	rede.entrada[2].valor=0.0;
//	rede.entrada[4].valor=0.0;
//	rede.entrada[5].valor=0.0;
//	rede.entrada[6].valor=0.0;
//	rede.entrada[7].valor=0.0;
//	rede.entrada[8].valor=0.0;
//	rede.entrada[9].valor=0.0;
//	rede.entrada[10].valor=0.0;
//	rede.entrada[11].valor=0.0;
//	rede.entrada[12].valor=0.0;
//	rede.entrada[13].valor=0.0;
//	rede.entrada[14].valor=0.0;
//	rede.entrada[15].valor=0.0;
//	rede.entrada[17].valor=0.0;
//	rede.entrada[18].valor=0.0;
//}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  elimina_exploracao                            //
//                                                                     //
// CARACTERISTICAS    :  elimina todos os valores positivos nas        //
// entradas de exploracao                                              //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//void elimina_exploracao()
//{
//	rede.entrada[3].valor=0.0;	// veja explicacoes na tabela
//	rede.entrada[30].valor=0.0;
//	rede.entrada[32].valor=0.0;
//}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  elimina_estados                               //
//                                                                     //
// CARACTERISTICAS    :  elimina todos os valores positivos nas        //
// entradas de estados                                                 //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//void elimina_estados()
//{
//	rede.entrada[3].valor=0.0;	// veja explicacoes na tabela
//	rede.entrada[30].valor=0.0;
//	rede.entrada[31].valor=0.0;
//	rede.entrada[32].valor=0.0;
//	rede.entrada[33].valor=0.0;
//	rede.entrada[47].valor=0.0;
//	rede.entrada[48].valor=0.0;
//}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  elimina_estados                               //
//                                                                     //
// CARACTERISTICAS    :  elimina todos os valores positivos nas        //
// entradas de olfato alimento e feromonio                             //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//void elimina_olfato_alimento_e_feromonio()
//{
//	rede.entrada[38].valor=0.0;
//	rede.entrada[40].valor=0.0;
//	rede.entrada[41].valor=0.0;
//	rede.entrada[43].valor=0.0;
//	rede.entrada[44].valor=0.0;
//	rede.entrada[46].valor=0.0;
//}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  elimina_estados                               //
//                                                                     //
// CARACTERISTICAS    :  elimina todos os valores positivos nas        //
// entradas de olfato ninho                                            //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

//void elimina_olfato_ninho()
//{
//	rede.entrada[37].valor=0.0;
//	rede.entrada[39].valor=0.0;
//	rede.entrada[42].valor=0.0;
//	rede.entrada[45].valor=0.0;
//}

/*void dump_entradas_saidas_concentrador()
{
	int i;

	for(i=0;i<nro_entradas;i++)
	{
		if(rede.entrada[i].valor!=0.0)
		{
			if(parametros.log_ligado) fprintf(debug,"<concentrador> ENTRADA %i VALOR %f ===> ",i,rede.entrada[i].valor);
			explica_linha_debug(i);
			if(parametros.log_ligado) fprintf(debug,"\n");
		}
	}

	for(i=0;i<nro_saidas;i++)
	{
		if(rede.saida[i].esperado!=0.0)
		{
			if(parametros.log_ligado) fprintf(debug,"<concentrador> SAIDA %i VALOR %f ===> ",i,rede.saida[i].esperado);
			explica_saida_debug(i);
			if(parametros.log_ligado) fprintf(debug,"\n");
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  concentrador                                  //
//                                                                     //
// CARACTERISTICAS    :  modulo para eliminar estimulos erroneos       //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  (concentracao / competicao fixa nas entradas) //
//                                                                     //
// ULTIMA ATUALIZACAO :  08/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void concentrador()
{
	if(parametros.log_ligado) fprintf(debug,"<concentrador> *************** ONLINE ****************\n");

	if(parametros.log_ligado) fprintf(debug,"<concentrador> * * *  S I T U A C A O   A N T E R I O R  * * *\n");

	dump_entradas_saidas_concentrador();

	if(panico()) 
	{
		if(parametros.log_ligado) fprintf(debug,"<concentrador> PANICO se aplica: TODO RESTO sera eliminado\n");
		elimina_tudo();
	}
	//dump_entradas_saidas():

	if(morrendo_de_fome())
	{
		if(parametros.log_ligado) fprintf(debug,"<concentrador> MORRENDO DE FOME se aplica: EXPLORACAO, ALIMENTO e FEROMONIO serao eliminados\n");
		elimina_exploracao();
		elimina_olfato_alimento_e_feromonio();
	}

	if(com_carga())
	{
		if(parametros.log_ligado) fprintf(debug,"<concentrador> COM CARGA se aplica: ALIMENTO e FEROMONIO serao eliminados\n");
		elimina_olfato_alimento_e_feromonio();
	}

	if(exploracao())
	{
		if(parametros.log_ligado) fprintf(debug,"<concentrador> EXPLORACAO se aplica: CHEIRO NINHO sera eliminado\n");
		elimina_olfato_ninho();
	}

	if(olfato()) 
	{
		if(parametros.log_ligado) fprintf(debug,"<concentrador> OLFATO se aplica: VISAO sera eliminada\n");
		elimina_visao();
	}
	
	if(visao()) 
	{
		if(parametros.log_ligado) fprintf(debug,"<concentrador> VISAO se aplica: ESTADOS serao eliminados\n");
		elimina_estados();
	}

	if(parametros.log_ligado) fprintf(debug,"<concentrador> * * *  S I T U A C A O   F I N A L  * * *\n");

	dump_entradas_saidas_concentrador();
}*/

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  situacao_para_entradas                        //
//                                                                     //
// CARACTERISTICAS    :  decodifica a situacao atual dos sensores para //
// as entradas da rede                                                 //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  30/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void situacao_para_entradas(int index_formiga)
{
	int i;

	flush_entradas(index_formiga);

	if(parametros.log_ligado) 
	{
		fprintf(debug,"<REDE> *** SITUACAO PARA ENTRADAS ***\n");
		fprintf(debug,"<REDE> (FORMIGA %i)\n",index_formiga);
		fprintf(debug,"<REDE>\n");
		fprintf(debug,"<REDE> ANTES:\n");
		dump_curto_entradas_saidas(index_formiga);
	}

	rede[index_formiga].entrada[0].valor=inter_perigo_presente(index_formiga);

	rede[index_formiga].entrada[1].valor=inter_objeto_visual_presente(index_formiga);

	rede[index_formiga].entrada[2].valor=inter_tato_alimento_presente(index_formiga);

	rede[index_formiga].entrada[3].valor=inter_entrada_ninho_presente(index_formiga);

	rede[index_formiga].entrada[4].valor=inter_cheiro_alomonio_presente(index_formiga);

	rede[index_formiga].entrada[5].valor=inter_cheiro_feromonio_presente(index_formiga);

	rede[index_formiga].entrada[6].valor=inter_cheiro_ninho_presente(index_formiga);

	rede[index_formiga].entrada[7].valor=inter_cheiro_alimento_presente(index_formiga);

	rede[index_formiga].entrada[8].valor=inter_fome_presente(index_formiga);

	rede[index_formiga].entrada[9].valor=inter_carga_presente(index_formiga);

	rede[index_formiga].entrada[10].valor=formiga[index_formiga].no_ninho;	// sem interpretador, mas tb. sem importancia....

	if(parametros.log_ligado)
	{
		fprintf(debug,"<REDE> DEPOIS:\n");
		dump_curto_entradas_saidas(index_formiga);
	}

	for(i=0;i<10;i++)
		if(rede[index_formiga].entrada[i].off)
			rede[index_formiga].entrada[i].valor=0.0;

	if(parametros.log_ligado);
	{
		fprintf(debug,"<REDE> ENTRADAS INDESEJADAS DESLIGADAS - RESULTADO:\n");
		dump_curto_entradas_saidas(index_formiga);
		fprintf(debug,"<REDE>\n");
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  gera_saidas_desejadas                         //
//                                                                     //
// CARACTERISTICAS    :  gera as saidas desejadas para uma formiga e   //
// as transmite para a rede                                            //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  30/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void gera_saidas_desejadas(int index_formiga, int qual_caso)
{
	int i;

	for(i=0;i<nro_saidas;i++)
		rede[index_formiga].saida[i].esperado=0;

	qual_caso--;

	rede[index_formiga].saida[qual_caso].esperado=1;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  xrand                                         //
//                                                                     //
// CARACTERISTICAS    :  randomizador simplificado                     //
//                                                                     //
// PARAMETROS         :  limite maximo do randomizador                 //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  31/10/03                                      //
/////////////////////////////////////////////////////////////////////////

int xrand(int max)
{
	int flag;

	flag=(rand()*max)/(RAND_MAX+1);

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  dump_valores_formiga_100                      //
//                                                                     //
// CARACTERISTICAS    :  imprime todos os valores da formiga nro. 100  //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  (apenas para debug)                           //
//                                                                     //
// ULTIMA ATUALIZACAO :  31/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void dump_valores_formiga_100()
{
	DEBUG_DOZE_IFS d12;

	int i, j;

	cls();

	printf("\n\nDUMP DA FORMIGA 100\n\n");

	printf("\nVISAO\n");

	for(i=0;i<5;i++)
	{
		printf("\n");
		for(j=0;j<8;j++)
			printf("%i ",formiga[100].visao[j][i]);
	}

	printf("\n\nOLFATO ALOMONIO\n");

	for(i=0;i<5;i++) printf("%i ",formiga[100].olfato_alomonio[i]);

	printf("\n\nOLFATO FEROMONIO\n");

	for(i=0;i<5;i++) printf("%i ",formiga[100].olfato_feromonio[i]);

	printf("\n\nOLFATO NINHO\n");

	for(i=0;i<5;i++) printf("%i ",formiga[100].olfato_ninho[i]);

	printf("\n\nOLFATO ALIMENTO\n");
	
	for(i=0;i<5;i++) printf("%i ",formiga[100].olfato_alimento[i]);

	printf("\n\nTATO\n");

	printf("%i",formiga[100].tato);

	printf("\n\nAUDICAO\n");

	printf("%i",formiga[100].audicao);

	printf("\n\nESTOMAGO\n");

	printf("%i %i",formiga[100].estomago,formiga[100].estomago_social);

	printf("\n\nALERTA\n");

	printf("%i",formiga[100].alerta);

	d12=doze_ifs(100,true,true);

	printf("\n\nALGORITMO\n");

	printf("%i %i",d12.caso,d12.linha);

	getch();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  gera_situacao_aleatoria                       //
//                                                                     //
// CARACTERISTICAS    :  gera uma situacao aleatoria para a formiga    //
// "100" (nao simulada) com a finalidade do aprendizado randomico      //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  31/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void gera_situacao_aleatoria()
{
	int i, j, temp;

	for(i=0;i<8;i++)
		for(j=0;j<4;j++)
			formiga[100].visao[i][j]=xrand(4);

	temp=xrand(100);

	if(temp<95) temp=0;	else temp=3;	// POUCOS casos para a vista para cima

	for(i=0;i<4;i++) formiga[100].visao[i][4]=temp; // vista para cima

	temp=xrand(4);

	formiga[100].visao[4][4]=temp;	// olho de luz 1
	formiga[100].visao[5][4]=temp;

	temp=xrand(4);

	formiga[100].visao[6][4]=temp;	// olho de luz 2
	formiga[100].visao[7][4]=temp;

	for(i=0;i<5;i++)
	{
		formiga[100].olfato_alomonio[i]=xrand(200);
		formiga[100].olfato_feromonio[i]=xrand(200);
		formiga[100].olfato_ninho[i]=xrand(200);
		formiga[100].olfato_alimento[i]=xrand(200);

		if(formiga[100].olfato_alomonio[i]>99) formiga[100].olfato_alomonio[i]=0;
		if(formiga[100].olfato_feromonio[i]>99) formiga[100].olfato_feromonio[i]=0;
		if(formiga[100].olfato_ninho[i]>99) formiga[100].olfato_ninho[i]=0;
		if(formiga[100].olfato_alimento[i]>99) formiga[100].olfato_alimento[i]=0;
	}

	formiga[100].tato=xrand(30);

	if(formiga[100].tato>7) formiga[100].tato=formiga[100].tato+2;

	if(formiga[100].tato>19) formiga[100].tato=0;	// maior incidencia de "sem tato"

	formiga[100].audicao=xrand(100);

	if(formiga[100].audicao<95) formiga[100].audicao=1; else formiga[100].audicao=99;	// especifico para ambiente

	formiga[100].estomago=xrand(100);
	formiga[100].estomago_social=xrand(100);

	temp=xrand(100);

	if(temp>95) formiga[100].alerta=true; else formiga[100].alerta=false; // apenas 20% de alerta

	formiga[100].coord_x=1;
	formiga[100].coord_y=1;
	formiga[100].coord_z=1;

	formiga[100].vida=viva;
	formiga[100].no_ninho=false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  objeto_chegando_acima                         //
//                                                                     //
// CARACTERISTICAS    :  retorna se ha um objeto isolado chegando      //
// acima (potencial predador)                                          //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool objeto_chegando_acima(int index_formiga)
{
	bool flag;

	flag=false;

	if((formiga[index_formiga].visao[3][2]==sem_presenca ||
		formiga[index_formiga].visao[3][1]==sem_presenca) &&
		formiga[index_formiga].visao[3][0]==muito_perto)
		flag=true;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  ojeto_abaixo                                  //
//                                                                     //
// CARACTERISTICAS    :  retorna se ha um objeto olhando para baixo    //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/10/03                                      //
/////////////////////////////////////////////////////////////////////////

int objeto_abaixo(int index_formiga)
{
	return formiga[index_formiga].visao[3][3];
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  olfato_frente_maior                           //
//                                                                     //
// CARACTERISTICAS    :  retorna se um cheiro esta maior a frente da   //
// formiga                                                             //
// PARAMETROS         :  numero da formiga, numero do cheiro           //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool olfato_frente_maior(int index_formiga, int qual_cheiro)
{
	bool flag;

	short int valor_comp[3];

	flag=false;

	switch(qual_cheiro)
	{
	case c_ninho: valor_comp[0]=formiga[index_formiga].olfato_ninho[2];
					valor_comp[1]=formiga[index_formiga].olfato_ninho[3];
					valor_comp[2]=formiga[index_formiga].olfato_ninho[4];
					break;
	case c_alomonio: valor_comp[0]=formiga[index_formiga].olfato_alomonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_alomonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_alomonio[4];
					break;
	case c_feromonio: valor_comp[0]=formiga[index_formiga].olfato_feromonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_feromonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_feromonio[4];
					break;
	case c_alimento: valor_comp[0]=formiga[index_formiga].olfato_alimento[2];
					valor_comp[1]=formiga[index_formiga].olfato_alimento[3];
					valor_comp[2]=formiga[index_formiga].olfato_alimento[4];
					break;
	}

	if(valor_comp[1]>0 && valor_comp[1]>=valor_comp[2] && valor_comp[1]>=valor_comp[0])
		flag=true;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  olfato_direita_maior                          //
//                                                                     //
// CARACTERISTICAS    :  retorna se um cheiro esta maior a direita da  //
// formiga                                                             //
// PARAMETROS         :  numero da formiga, numero do cheiro           //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool olfato_direita_maior(int index_formiga, int qual_cheiro)
{
	bool flag;

	short int valor_comp[3];

	flag=false;

	switch(qual_cheiro)
	{
	case c_ninho: valor_comp[0]=formiga[index_formiga].olfato_ninho[2];
					valor_comp[1]=formiga[index_formiga].olfato_ninho[3];
					valor_comp[2]=formiga[index_formiga].olfato_ninho[4];
					break;
	case c_alomonio: valor_comp[0]=formiga[index_formiga].olfato_alomonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_alomonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_alomonio[4];
					break;
	case c_feromonio: valor_comp[0]=formiga[index_formiga].olfato_feromonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_feromonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_feromonio[4];
					break;
	case c_alimento: valor_comp[0]=formiga[index_formiga].olfato_alimento[2];
					valor_comp[1]=formiga[index_formiga].olfato_alimento[3];
					valor_comp[2]=formiga[index_formiga].olfato_alimento[4];
					break;
	}

	if(valor_comp[2]>0 && valor_comp[2]>=valor_comp[1] && valor_comp[2]>=valor_comp[0])
		flag=true;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  olfato_esquerda_maior                         //
//                                                                     //
// CARACTERISTICAS    :  retorna se um cheiro esta maior a esquerda da //
// formiga                                                             //
// PARAMETROS         :  numero da formiga, numero do cheiro           //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool olfato_esquerda_maior(int index_formiga, int qual_cheiro)
{
	bool flag;

	short int valor_comp[3];

	flag=false;

	switch(qual_cheiro)
	{
	case c_ninho: valor_comp[0]=formiga[index_formiga].olfato_ninho[2];
					valor_comp[1]=formiga[index_formiga].olfato_ninho[3];
					valor_comp[2]=formiga[index_formiga].olfato_ninho[4];
					break;
	case c_alomonio: valor_comp[0]=formiga[index_formiga].olfato_alomonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_alomonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_alomonio[4];
					break;
	case c_feromonio: valor_comp[0]=formiga[index_formiga].olfato_feromonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_feromonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_feromonio[4];
					break;
	case c_alimento: valor_comp[0]=formiga[index_formiga].olfato_alimento[2];
					valor_comp[1]=formiga[index_formiga].olfato_alimento[3];
					valor_comp[2]=formiga[index_formiga].olfato_alimento[4];
					break;
	}

	if(valor_comp[0]>0 && valor_comp[0]>=valor_comp[2] && valor_comp[0]>=valor_comp[1])
		flag=true;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  olfato_frente_menor                           //
//                                                                     //
// CARACTERISTICAS    :  retorna se um cheiro esta maior a frente da   //
// formiga                                                             //
// PARAMETROS         :  numero da formiga, numero do cheiro           //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool olfato_frente_menor(int index_formiga, int qual_cheiro)
{
	bool flag;

	short int valor_comp[3];

	flag=false;

	switch(qual_cheiro)
	{
	case c_ninho: valor_comp[0]=formiga[index_formiga].olfato_ninho[2];
					valor_comp[1]=formiga[index_formiga].olfato_ninho[3];
					valor_comp[2]=formiga[index_formiga].olfato_ninho[4];
					break;
	case c_alomonio: valor_comp[0]=formiga[index_formiga].olfato_alomonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_alomonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_alomonio[4];
					break;
	case c_feromonio: valor_comp[0]=formiga[index_formiga].olfato_feromonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_feromonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_feromonio[4];
					break;
	case c_alimento: valor_comp[0]=formiga[index_formiga].olfato_alimento[2];
					valor_comp[1]=formiga[index_formiga].olfato_alimento[3];
					valor_comp[2]=formiga[index_formiga].olfato_alimento[4];
					break;
	}

	if(valor_comp[1]>0 && valor_comp[1]<=valor_comp[2] && valor_comp[1]<=valor_comp[0])
		flag=true;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  olfato_direita_menor                          //
//                                                                     //
// CARACTERISTICAS    :  retorna se um cheiro esta maior a direita da  //
// formiga                                                             //
// PARAMETROS         :  numero da formiga, numero do cheiro           //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool olfato_direita_menor(int index_formiga, int qual_cheiro)
{
	bool flag;

	short int valor_comp[3];

	flag=false;

	switch(qual_cheiro)
	{
	case c_ninho: valor_comp[0]=formiga[index_formiga].olfato_ninho[2];
					valor_comp[1]=formiga[index_formiga].olfato_ninho[3];
					valor_comp[2]=formiga[index_formiga].olfato_ninho[4];
					break;
	case c_alomonio: valor_comp[0]=formiga[index_formiga].olfato_alomonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_alomonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_alomonio[4];
					break;
	case c_feromonio: valor_comp[0]=formiga[index_formiga].olfato_feromonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_feromonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_feromonio[4];
					break;
	case c_alimento: valor_comp[0]=formiga[index_formiga].olfato_alimento[2];
					valor_comp[1]=formiga[index_formiga].olfato_alimento[3];
					valor_comp[2]=formiga[index_formiga].olfato_alimento[4];
					break;
	}

	if(valor_comp[2]>0 && valor_comp[2]<=valor_comp[1] && valor_comp[2]<=valor_comp[0])
		flag=true;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  olfato_esquerda_menor                         //
//                                                                     //
// CARACTERISTICAS    :  retorna se um cheiro esta maior a esquerda da //
// formiga                                                             //
// PARAMETROS         :  numero da formiga, numero do cheiro           //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool olfato_esquerda_menor(int index_formiga, int qual_cheiro)
{
	bool flag;

	short int valor_comp[3];

	flag=false;

	switch(qual_cheiro)
	{
	case c_ninho: valor_comp[0]=formiga[index_formiga].olfato_ninho[2];
					valor_comp[1]=formiga[index_formiga].olfato_ninho[3];
					valor_comp[2]=formiga[index_formiga].olfato_ninho[4];
					break;
	case c_alomonio: valor_comp[0]=formiga[index_formiga].olfato_alomonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_alomonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_alomonio[4];
					break;
	case c_feromonio: valor_comp[0]=formiga[index_formiga].olfato_feromonio[2];
					valor_comp[1]=formiga[index_formiga].olfato_feromonio[3];
					valor_comp[2]=formiga[index_formiga].olfato_feromonio[4];
					break;
	case c_alimento: valor_comp[0]=formiga[index_formiga].olfato_alimento[2];
					valor_comp[1]=formiga[index_formiga].olfato_alimento[3];
					valor_comp[2]=formiga[index_formiga].olfato_alimento[4];
					break;
	}

	if(valor_comp[0]>0 && valor_comp[0]<=valor_comp[2] && valor_comp[0]<=valor_comp[1])
		flag=true;

	return flag;
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

			atualiza_cheiro_alimentos();
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

	if(ir_para_frente(index_formiga,true))
		ir_para_tras(index_formiga,true); else
		flag=false;

	return flag;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  travada                                       //
//                                                                     //
// CARACTERISTICAS    :  retorna se uma formiga esta travada em uma    //
// certa posicao                                                       //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  28/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool travada(int index_formiga)
{
	int giro;

	for(giro=0;giro<8;giro++)
	{
		vira_sentido_horario(index_formiga);

		if(caminho_livre(index_formiga)) return false;
	}

	return true;
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
	int form_x, form_y, form_z, rnd;

	if(travada(index_formiga)==false)
	{

		form_x=formiga[index_formiga].coord_x;
		form_y=formiga[index_formiga].coord_y;
		form_z=formiga[index_formiga].coord_z;

		if(mundo.elemento[form_x][form_y][form_z].paisagem!=predador)
		{

			do
			{
				rnd=(rand()*8)/RAND_MAX;

				if(rnd<=0) rnd=1;

				for(i=0;i<rnd;i++)
					vira_sentido_horario(index_formiga);	

			}while(caminho_livre(index_formiga)==false);

		}
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
// ULTIMA ATUALIZACAO :  11/10/03                                      //
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
// ULTIMA ATUALIZACAO :  11/10/03                                      //
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
// ULTIMA ATUALIZACAO :  11/10/03                                      //
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
// ULTIMA ATUALIZACAO :  11/10/03                                      //
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
// ULTIMA ATUALIZACAO :  11/10/03                                      //
/////////////////////////////////////////////////////////////////////////

int objeto_acima(int index_formiga)
{
	int flag;

	flag=formiga[index_formiga].visao[0][4];

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
// ULTIMA ATUALIZACAO :  09/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void fugir_perigo(int index_formiga)
{
	muda_aleatoriamente_direcao(index_formiga);
	solta_alomonio(index_formiga);
	aumenta_passo(index_formiga);
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  fugir_alomonio                                //
//                                                                     //
// CARACTERISTICAS    :  muda aleatoriamente de direcao para dentro de //
// um caminho livre e aumenta o passo                                  //
// PARAMETROS         :  numero de formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void fugir_alomonio(int index_formiga)
{
	muda_aleatoriamente_direcao(index_formiga);
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
	if((formiga[index_formiga].tato>comida0 || formiga[index_formiga].tato==agua) && 
		formiga[index_formiga].estomago_social<90)
	{
		come(index_formiga);
		obtenha_dados_sensores(index_formiga);
	}

	//inverte_direcao(index_formiga); //na nova versao nao ha mais isso!!!

	//if(caminho_livre(index_formiga)==false)		// seguranca caso o procedimento nao produza bons resultados
	//	muda_aleatoriamente_direcao(index_formiga);

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

	//if(index_formiga==0) flush_saidas();

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
	//int rnd;

	//rnd=(rand()*11)/RAND_MAX;

	//if(rnd<6) seguir_atracao_direita(index_formiga); else
		seguir_atracao_esquerda(index_formiga);

	//if(caminho_livre(index_formiga)==false)		// seguranca caso o procedimento nao produza bons resultados
	//	muda_aleatoriamente_direcao(index_formiga);
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
// NOME DA FUNCAO     :  executa_acao                                  //
//                                                                     //
// CARACTERISTICAS    :  modulo separado de execucao                   //
//                                                                     //
// PARAMETROS         :  numero da formiga, numero da acao             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  07/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void executa_acao(int index_formiga, int acao)
{
	switch(acao)
	{
		case 1: fugir_perigo(index_formiga); break;
		case 2: fugir_alomonio(index_formiga); break;
		case 3: voltar_normalidade(index_formiga); break;
		case 4: evitar_colisao(index_formiga); break;
		case 5: pegar_comida(index_formiga); break;
		case 6: sair_alerta(index_formiga); break;
		case 7: sair_caminho_ruim(index_formiga); break;
		case 8: entrar_ninho(index_formiga,true); break;
		case 9: seguir_atracao_frente(index_formiga); break;
		case 10: seguir_atracao_ambigua(index_formiga); break;
		case 11: seguir_atracao_direita(index_formiga); break;
		case 12: seguir_atracao_esquerda(index_formiga); break;
		case 13: seguir_atracao_frente(index_formiga); break;
		case 14: seguir_atracao_frente(index_formiga); break;
		case 15: buscar_sinal(index_formiga); break;
		case 16: seguir_atracao_ambigua(index_formiga); break;
		case 17: seguir_atracao_direita(index_formiga); break;
		case 18: seguir_atracao_esquerda(index_formiga); break;
		case 99: buscar_sinal(index_formiga); break;		// caso "default"
	}

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

DEBUG_DOZE_IFS doze_ifs(int index_formiga, bool modo_debug, bool modo_aprendizado)
{
	DEBUG_DOZE_IFS info;

	info.caso=99;	// default
	info.linha=99;	// default ---> caso nao seja encontrado nenhum caso cabivel aqui!!!

	if(modo_aprendizado==false) obtenha_dados_sensores(index_formiga);

	// IF (1)	=> FUGIR_PERIGO
	if(formiga[index_formiga].tato==predador)
	{
		info.caso=1;
		info.linha=1;
	}
	if(objeto_acima(index_formiga)==muito_perto)
	{
		info.caso=1;
		info.linha=2;
	}
	
	if(formiga[index_formiga].audicao>=10)
	{
		info.caso=1;
		info.linha=3;
	}

	// IF (2)	=> FUGIR_ALOMONIO
	if(info.caso==99)
		if(positivo_e_crescente(index_formiga,c_alomonio) && formiga[index_formiga].estomago>10)
		{
			info.caso=2;
			info.linha=4;
		}
	
	// IF (3)	=> VOLTAR_NORMALIDADE
	if(info.caso==99)
	{
		if(formiga[index_formiga].olfato_alomonio[0]==0 &&
			formiga[index_formiga].alerta==true)
		{
			info.caso=3;
			info.linha=5;
		}
	}

	// IF (4)	=> EVITAR_COLISAO
	if(info.caso==99)
	{
		if(formiga[index_formiga].tato==outra &&
		formiga[index_formiga].olfato_alomonio[0]==0)
		{
			info.caso=4;
			info.linha=6;
		}
	}

	// IF (5)	=> PEGAR_COMIDA
	if(info.caso==99)
	{
		if(formiga[index_formiga].olfato_alomonio[0]==0 &&
		formiga[index_formiga].audicao<10 &&
		formiga[index_formiga].estomago_social<89)
		{
			if(formiga[index_formiga].tato==agua)
			{
				info.caso=5;
				info.linha=7;
			}
			if(formiga[index_formiga].tato>comida0)
			{
				info.caso=5;
				info.linha=8;
			}
		}
	}

	// IF (6)	=> SAIR_ALERTA
	if(info.caso==99)
	{
		if(formiga[index_formiga].tato!=agua &&
		positivo_e_decrescente(index_formiga,c_alomonio))
		{
			info.caso=6;
			info.linha=9;
		}
	}

	// IF (7)	=> SAIR_CAMINHO_RUIM
	if(info.caso==99)
	{
		if(formiga[index_formiga].tato==margem)
		{
			info.caso=7;
			info.linha=10;
		}
		
		if(objeto_chegando_acima(index_formiga))
		{
			info.caso=7;
			info.linha=11;
		}

		/*if(formiga[index_formiga].olfato_alomonio[0]==0 &&
		positivo_e_decrescente(index_formiga,c_feromonio) &&
		formiga[index_formiga].audicao<10 && 
		formiga[index_formiga].estomago>10 &&
		formiga[index_formiga].estomago_social<10)
		{
			info.caso=7;
			info.linha=12;
		}

		if(formiga[index_formiga].olfato_alomonio[0]==0 &&
		positivo_e_decrescente(index_formiga,c_ninho) &&
		formiga[index_formiga].audicao<10 &&
		formiga[index_formiga].estomago<=10)
		{
			info.caso=7;
			info.linha=13;
		}

		if(formiga[index_formiga].olfato_alomonio[0]==0 &&
		positivo_e_decrescente(index_formiga,c_ninho) &&
		formiga[index_formiga].audicao<10 &&
		formiga[index_formiga].estomago_social>=10)
		{
			info.caso=7;
			info.linha=14;
		}

		if(formiga[index_formiga].olfato_alomonio[0]==0 &&
		positivo_e_decrescente(index_formiga,c_alimento) &&
		formiga[index_formiga].audicao<10 &&
		formiga[index_formiga].estomago>10 &&
		formiga[index_formiga].estomago_social<10)
		{
			info.caso=7;
			info.linha=15;
		}*/
	}

	// IF (8)	=> ENTRAR_NINHO
	if(info.caso==99)
	{
		if(formiga[index_formiga].olfato_alomonio[0]==0 &&
		formiga[index_formiga].tato==ninho &&
		formiga[index_formiga].coord_z==2)		// apenas entrada no ultimo nivel do ninho
		{
			if(formiga[index_formiga].estomago<=10)
			{
				info.caso=8;
				info.linha=16;
			}
			if(formiga[index_formiga].estomago_social>=10)
			{
				info.caso=8;
				info.linha=17;
			}
		}
	}

	// IF (9)	=> SEGUIR_ATRACAO_FRENTE (OLFATO)

	if(info.caso==99)
	{

		if(formiga[index_formiga].tato!=agua)
		{

			if(olfato_frente_maior(index_formiga,c_feromonio) &&
			formiga[index_formiga].estomago>10 &&
			formiga[index_formiga].estomago_social<10)
			{
				info.caso=9;
				info.linha=18;
			}

			if(olfato_frente_maior(index_formiga,c_ninho) &&
			formiga[index_formiga].estomago<=10 &&
			positivo_e_crescente(index_formiga,c_ninho))
			{
				info.caso=9;
				info.linha=19;
			}

			if(olfato_frente_maior(index_formiga,c_ninho) &&
			formiga[index_formiga].estomago_social>=10 &&
			positivo_e_crescente(index_formiga,c_ninho))
			{
				info.caso=9;
				info.linha=20;
			}

			//if(formiga[index_formiga].olfato_feromonio[0]==0 &&
			if(olfato_frente_maior(index_formiga,c_alimento) &&
			formiga[index_formiga].estomago>10 &&
			formiga[index_formiga].estomago_social<10) // &&
			//positivo_e_crescente(index_formiga,c_alimento))
			{
				info.caso=9;
				info.linha=21;
			}
		}
	}

	// IF (10)	=> SEGUIR_ATRACAO_AMBIGUA (OLFATO)

	if(info.caso==99)
	{

		if(olfato_direita_maior(index_formiga,c_feromonio) &&
		olfato_esquerda_maior(index_formiga,c_feromonio) &&
		formiga[index_formiga].estomago>10 &&
		formiga[index_formiga].estomago_social<10)
		{
			info.caso=10;
			info.linha=22;
		}

		if(olfato_direita_maior(index_formiga,c_ninho) &&
		olfato_esquerda_maior(index_formiga,c_ninho) &&
		formiga[index_formiga].estomago<=10)
		{
			info.caso=10;
			info.linha=23;
		}

		if(olfato_direita_maior(index_formiga,c_ninho) &&
		olfato_esquerda_maior(index_formiga,c_ninho) &&
		formiga[index_formiga].estomago_social>=10)
		{
			info.caso=10;
			info.linha=24;
		}

		//if(formiga[index_formiga].olfato_feromonio[0]==0 &&
		if(olfato_direita_maior(index_formiga,c_alimento) &&
		olfato_esquerda_maior(index_formiga,c_alimento) &&
		formiga[index_formiga].estomago>10 &&
		formiga[index_formiga].estomago_social<10)
		{
			info.caso=10;
			info.linha=25;
		}
	}

	// IF (11)	=> SEGUIR_ATRACAO_DIREITA (OLFATO)

	if(info.caso==99)
	{
		if(olfato_direita_maior(index_formiga,c_feromonio) &&
		formiga[index_formiga].estomago>10 &&
		formiga[index_formiga].estomago_social<10)
		{
			info.caso=11;
			info.linha=26;
		}

		if(olfato_direita_maior(index_formiga,c_ninho) &&
		formiga[index_formiga].estomago<=10)
		{
			info.caso=11;
			info.linha=27;
		}

		if(olfato_direita_maior(index_formiga,c_ninho) &&
		formiga[index_formiga].estomago_social>=10)
		{
			info.caso=11;
			info.linha=28;
		}

		//if(formiga[index_formiga].olfato_feromonio[0]==0 &&
		if(olfato_direita_maior(index_formiga,c_alimento) &&
		formiga[index_formiga].estomago>10 &&
		formiga[index_formiga].estomago_social<10)
		{
			info.caso=11;
			info.linha=29;
		}
	}

	// IF (12)	=> SEGUIR_ATRACAO_ESQUERDA (OLFATO)

	if(info.caso==99)
	{
		if(olfato_esquerda_maior(index_formiga,c_feromonio) &&
		formiga[index_formiga].estomago>10 &&
		formiga[index_formiga].estomago_social<10)
		{
			info.caso=12;
			info.linha=30;
		}

		if(olfato_esquerda_maior(index_formiga,c_ninho) &&
		formiga[index_formiga].estomago<=10)
		{
			info.caso=12;
			info.linha=31;
		}

		if(olfato_esquerda_maior(index_formiga,c_ninho) &&
		formiga[index_formiga].estomago_social>=10)
		{
			info.caso=12;
			info.linha=32;
		}

		//if(formiga[index_formiga].olfato_feromonio[0]==0 &&
		if(olfato_esquerda_maior(index_formiga,c_alimento) &&
		formiga[index_formiga].estomago>10 &&
		formiga[index_formiga].estomago_social<10)
		{
			info.caso=12;
			info.linha=33;
		}
	}

	// IF (13)	=> SEGUIR_ATRACAO_FRENTE (VISAO)

	if(info.caso==99)
	{
		if(formiga[index_formiga].tato!=agua)
		{
			if(objeto_frente(index_formiga)==longe)
			{
				info.caso=13;
				info.linha=34;
			}

			if(objeto_frente(index_formiga)==perto)
			{
				info.caso=13;
				info.linha=35;
			}

			if((objeto_frente(index_formiga)==muito_perto_baixo ||
			objeto_frente(index_formiga)==muito_perto_alto) &&
			formiga[index_formiga].tato==sem_presenca)
			{
				info.caso=13;
				info.linha=36;
			}

			if((objeto_frente(index_formiga)==muito_perto_baixo ||
			objeto_frente(index_formiga)==muito_perto_alto) &&
			formiga[index_formiga].tato==parede)
			{
				info.caso=13;
				info.linha=37;
			}

			if((objeto_frente(index_formiga)==muito_perto_baixo ||
			objeto_frente(index_formiga)==muito_perto_alto) &&
			formiga[index_formiga].tato==terra)
			{
				info.caso=13;
				info.linha=38;
			}
		}
	}

	// IF (14)	=> SEGUIR_ATRACAO_FRENTE (OBJETO ABAIXO)

	if(info.caso==99)
	{

		if(formiga[index_formiga].tato!=agua &&
		objeto_abaixo(index_formiga)!=sem_presenca)
		{
			info.caso=14;
			info.linha=39;
		}

	}
	
	// IF (15)	=> BUSCAR_SINAL
	if(info.caso==99)
	{
		if(objeto_esquerda(index_formiga)==sem_presenca &&
		objeto_direita(index_formiga)==sem_presenca)
		{
			info.caso=15;
			info.linha=40;
		}
	}

	// IF (16)	=> SEGUIR ATRACAO AMBIGUA (VISAO)
	if(info.caso==99)
	{
		if(objeto_esquerda(index_formiga)==longe &&
		objeto_direita(index_formiga)==longe)
		{
			info.caso=16;
			info.linha=41;
		}
	
		if(objeto_esquerda(index_formiga)==perto &&
		objeto_direita(index_formiga)==perto)
		{
			info.caso=16;
			info.linha=42;
		}

		if(objeto_esquerda(index_formiga)==muito_perto_baixo &&
		objeto_direita(index_formiga)==muito_perto_baixo)
		{
			info.caso=16;
			info.linha=43;
		}
	}

	// IF (17)	=> SEGUIR_ATRACAO_DIREITA (VISAO)
	if(info.caso==99)
	{
		if(objeto_esquerda(index_formiga)==sem_presenca &&
		objeto_direita(index_formiga)==longe)
		{
			info.caso=17;
			info.linha=44;
		}

		if(objeto_esquerda(index_formiga)<=longe &&
		objeto_direita(index_formiga)==perto)
		{
			info.caso=17;
			info.linha=45;
		}

		if(objeto_esquerda(index_formiga)<=perto &&
		objeto_direita(index_formiga)==muito_perto_baixo)
		{
			info.caso=17;
			info.linha=46;
		}
	}

	// IF (18)	=> SEGUIR_ATRACAO_ESQUERDA (VISAO)
	if(info.caso==99)
	{
		if(objeto_esquerda(index_formiga)==longe &&
		objeto_direita(index_formiga)==sem_presenca) 
		{
			info.caso=18;
			info.linha=47;
		}

		if(objeto_esquerda(index_formiga)==perto &&
		objeto_direita(index_formiga)<=longe)
		{
			info.caso=18;
			info.linha=48;
		}

		if(objeto_esquerda(index_formiga)==muito_perto_baixo &&
		objeto_direita(index_formiga)<=perto)
		{
			info.caso=18;
			info.linha=49;
		}
	}

	if(info.caso==99)
		info.caso=15;	// para evitar problemas no aprendizado da rede (saida nula etc....)

	formiga[index_formiga].decisao=info.caso;

	// execucao conforme controladora a seguir:

	if(modo_debug==false)
	{
		executa_acao(index_formiga,info.caso);
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
				if(mundo.elemento[i][j][k].paisagem==ninho && k==2)	// apenas o ultimo nivel emite cheiro
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
	
	int x_atual, y_atual, z_atual, x_diff, y_diff, z_diff, cheiro_produzido, alcance;

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
// NOME DA FUNCAO     :  devolve_cor_fases                             //
//                                                                     //
// CARACTERISTICAS    :  devolve a cor da fase (modo neural) para a    //
// visualizacao da formiga                                             //
// PARAMETROS         :  numero da decisao                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void devolve_cor_fases(int dec)
{
	// REDE ----> AMARELO
	// IDA -----> AZUL
	// VOLTA ---> VERDE
	// PERIGO --> VERMELHO
	// ERRO ----> MARROM

	switch(dec)
	{
	case code_escape 				: setcolor( XFOREGROUND_WHITE | XBACKGROUND_YELLOW ); break; 
	case code_saia_do_ninho 		: setcolor( XFOREGROUND_WHITE | XBACKGROUND_BROWN ); break; 
	case code_explore_ambiente 		: setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLUE ); break; 
	case code_segue_cheiro_feromonio : setcolor( XFOREGROUND_WHITE | XBACKGROUND_LIGHTBLUE ); break; 
	case code_segue_cheiro_alimento : setcolor( XFOREGROUND_WHITE | XBACKGROUND_PURPLE ); break; 
	case code_pegue_alimento 		: setcolor( XFOREGROUND_WHITE | XBACKGROUND_LIGHTPURPLE ); break; 
	case code_ache_cheiro_ninho 	: setcolor( XFOREGROUND_WHITE | XBACKGROUND_GREEN ); break; 
	case code_segue_cheiro_ninho 	: setcolor( XFOREGROUND_WHITE | XBACKGROUND_LIGHTGREEN ); break; 
	case code_entre_no_ninho 		: setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY ); break; 
	case code_alertar_perigo 		: setcolor( XFOREGROUND_WHITE | XBACKGROUND_RED ); break; 
	case code_fugir_do_perigo 		: setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY ); break; 
	case code_flush 				: setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE ); break; 
	case code_apos_init 			: setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE ); break; 
	}

	//printf(" %i ",dec);
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
	case 1: setcolor( XFOREGROUND_WHITE | XBACKGROUND_RED ); break; 
	case 2: setcolor( XFOREGROUND_WHITE | XBACKGROUND_RED ); break; 
	case 3: setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLUE ); break; 
	case 4: setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLUE ); break; 
	case 5: setcolor( XFOREGROUND_WHITE | XBACKGROUND_BROWN ); break; 
	case 6: setcolor( XFOREGROUND_WHITE | XBACKGROUND_RED ); break; 
	case 7: setcolor( XFOREGROUND_WHITE | XBACKGROUND_PURPLE ); break; 
	case 8: setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLUE ); break; 
	case 9: setcolor( XFOREGROUND_WHITE | XBACKGROUND_GREEN ); break; 
	case 10: setcolor( XFOREGROUND_WHITE | XBACKGROUND_GREEN ); break; 
	case 11: setcolor( XFOREGROUND_WHITE | XBACKGROUND_GREEN ); break; 
	case 12: setcolor( XFOREGROUND_WHITE | XBACKGROUND_GREEN ); break; 
	case 13: setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY ); break; 
	case 14: setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY ); break; 
	case 15: setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY ); break; 
	case 16: setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY ); break; 
	case 17: setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY ); break; 
	case 18: setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY ); break; 
	case 99: setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE ); break; 
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
	if(parametros.modo_neural)
		devolve_cor_fases(formiga[index_formiga].macro_code); else
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
	int acha_elemento, codigo_elemento;
	
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
						printf("%i",mundo.elemento[x][y][z].cheiro_feromonio/11); else
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
// NOME DA FUNCAO     :  legenda                                       //
//                                                                     //
// CARACTERISTICAS    :  para a colocacao da legenda na simulacao      //
//                                                                     //
// PARAMETROS         :  modo debug? s/n                               //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  29/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void legenda(bool modo_debug)
{
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

		switch(i)
		{
		case 0: devolve_cor_decisao(1); break;
		case 1: devolve_cor_decisao(3); break;
		case 2: devolve_cor_decisao(5); break;
		case 3: devolve_cor_decisao(7); break;
		case 4: devolve_cor_decisao(9); break;
		case 5: devolve_cor_decisao(13); break;
		}

		printf("|");

		setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );
		printf(" ");

		switch(i)
		{
		case 0: printf("TRATANDO PERIGO"); break;
		case 1: printf("ORGANIZANDO-SE"); break;
		case 2: printf("COMENDO"); break;
		case 3: printf("TRATANDO PROBLEMAS"); break;
		case 4: printf("SEGUINDO CHEIRO"); break;
		case 5: printf("SEGUINDO VISAO"); break;
		}
	}

	devolve_cor_decisao(99);

	gotoxy(40,50);

	printf("|");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

	printf(" DEFAULT");

	setcolor( XFOREGROUND_LIGHTPURPLE | XBACKGROUND_PURPLE );

	gotoxy(40,52);

	printf("o");

	gotoxy(42,52);

	printf("#");

	gotoxy(44,52);

	printf("~");

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY );

	gotoxy(40,51);

	printf("@");

	setcolor( XFOREGROUND_LIGHTGRAY | XBACKGROUND_BLACK );

    gotoxy(46,52);

	printf("MORTA");

	gotoxy(42,51);

	printf("NO NINHO");

	gotoxy(40,54);

	printf("LIMITE SIMULACAO:");

	gotoxy(40,55);

	if(parametros.limite_simulacao>0) printf("=> %i ITERACOES",parametros.limite_simulacao); else
		switch(parametros.limite_simulacao)
	{
		case ate_sem_acucar:printf("=> ATE O ACUCAR ACABAR"); break;
		case ate_morte: printf("=> ATE A MORTE DE TODAS FORMIGAS"); break;
		case 0: printf("=> SEM LIMITE ([esc] PARA TERMINAR)"); break;
	}

	if(modo_debug)
	{
		gotoxy(0,56);

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

		printf(" TECLAS                                                                         ");

	    setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		center(57," [0-4] NIVEL, [a] ALOMONIO, [f] FEROMONIO, [n] NINHO, [c] ALIMENTO,             ");
		center(58," [b] BARULHO, [m] NORMAL, [cursor] FORMIGA, [o] BOOM! [-] SILENCIO, [esc] SAIR  ");

		home();
	} else
	{
		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		center(56," TECLAS: [d] MODO DEBUG, [t][x] MODO TRACE, [p] PE, [l] SEM PE, [s] DEFINE      ");
		center(57,"     PASSARO, [o] LIGA/DESLIGA PASSARO, VISUALIZACOES: [m] NORMAL, [a] ALOMONIO,");
		center(58,"     [f] FEROMONIO, [n] NINHO, [c] ALIMENTO, [b] BARULHO, [esc] SAIR            ");

		home();
	}

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

		formiga[i].demora_subida=0;

		formiga[i].macro_code=code_saia_do_ninho;
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
// ULTIMA ATUALIZACAO :  28/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool saia_do_ninho(int index_formiga)
{
	int rnd_x, rnd_y;

	//if(index_formiga==0) flush_saidas();

	rnd_x=int(rand()*8.0/(RAND_MAX+1));
	rnd_y=int(rand()*6.0/(RAND_MAX+1));

	rnd_x=rnd_x+32;
	rnd_y=rnd_y+21;

	//valor_random=int(rand()*4.0/(RAND_MAX));

	//gotoxy(0,1);

	//printf("saida de formiga %i requisitada   ",index_formiga);

	//getch();

	if(mundo.elemento[rnd_x][rnd_y][3].paisagem!=vazio) // alguem na entrada??
		return false;

	formiga[index_formiga].coord_x=rnd_x;
	formiga[index_formiga].coord_y=rnd_y;
	formiga[index_formiga].coord_z=3;		// acima da ultima camada

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
	int z;

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

bool ir_para_frente(int index_formiga, bool hipotetico)
{
	// ATENCAO - CONVENCAO: 0 Graus significa virada para o norte

	bool flag;

	flag=false;

	int x_plus, y_plus, x_novo, y_novo, x_atual, y_atual, z_atual, z_novo;

	if(formiga[index_formiga].demora_subida>0 && hipotetico==false)
		formiga[index_formiga].demora_subida--; else
	{

		x_plus=devolve_x_mais(formiga[index_formiga].direcao);
		y_plus=devolve_y_mais(formiga[index_formiga].direcao);

		mundo.elemento[formiga[index_formiga].coord_x][formiga[index_formiga].coord_y][formiga[index_formiga].coord_z].paisagem=vazio;

		x_novo=formiga[index_formiga].coord_x+x_plus;
		y_novo=formiga[index_formiga].coord_y+y_plus;

		if(x_novo>=0 && x_novo<max_mundo_x && y_novo>=0 && y_novo<max_mundo_y)
			if(posicao_disponivel(x_novo,y_novo))
		{
			z_novo=ajusta_altura(x_novo,y_novo);

			if(hipotetico==false)
				formiga[index_formiga].demora_subida=abs(formiga[index_formiga].coord_z-z_novo);

			formiga[index_formiga].coord_x=x_novo;
			formiga[index_formiga].coord_y=y_novo;
			formiga[index_formiga].coord_z=z_novo;

			x_atual=formiga[index_formiga].coord_x;
			y_atual=formiga[index_formiga].coord_y;
			z_atual=formiga[index_formiga].coord_z;

			flag=true;

			// gravar valores historicos

			if(hipotetico==false && formiga[index_formiga].tato!=outra && formiga[index_formiga].tato!=margem)
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

	return flag;
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
		case 2:  center(y_pos,"FUGIREI DO ALOMONIO"); break;
		case 3:  center(y_pos,"VOLTAREI A NORMALIDADE"); break;
		case 4:  center(y_pos,"EVITAREI UMA COLISAO"); break;
		case 5:  center(y_pos,"PEGAREI ALIMENTO"); break;
		case 6:  center(y_pos,"SAIREI DA ZONA DE ALERTA"); break;
		case 7:  center(y_pos,"SAIREI DE UM CAMINHO NAO PROVEITOSO"); break;
		case 8:  center(y_pos,"ENTRAREI NO NINHO"); break;
		case 9:  center(y_pos,"SEGUIREI UM PONTO DE ATRACAO A FRENTE"); break;
		case 10: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO AMBIGUO"); break;
		case 11: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO A DIREITA"); break;
		case 12: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO A ESQUERDA"); break;
		case 13: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO A FRENTE"); break;
		case 14: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO A FRENTE"); break;
		case 15: center(y_pos,"BUSCAREI SINAIS PARA ORIENTACAO"); break;
		case 16: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO AMBIGUO"); break;
		case 17: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO A DIREITA"); break;
		case 18: center(y_pos,"SEGUIREI UM PONTO DE ATRACAO A ESQUERDA"); break;

		// DEFAULT
		case 99: center(y_pos,"BUSCAREI SINAIS MAIS FORTES"); break;
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  explica_linha                                 //
//                                                                     //
// CARACTERISTICAS    :  mostra a decisao consequente dos algoritmos   //
//                                                                     //
// PARAMETROS         :  posicao y da linha e o codigo                 //
// OBSERVACOES        :  para poder usar com os dois algoritmos        //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void explica_linha(int y_pos, int codigo)
{
	switch(codigo)
	{
		// FUGIR_PERIGO
		case 1:  center(y_pos,"POIS HA UM PREDADOR A FRENTE (TATO)"); break;
		case 2:  center(y_pos,"POIS HA UM OBJETO ACIMA DE MIM (PREDADOR?)"); break;
		case 3:  center(y_pos,"POIS HA MUITO BARULHO AQUI"); break;

		// FUGIR_ALOMONIO
		case 4:  center(y_pos,"POIS HA CHEIRO DE ALOMONIO AUMENTANDO"); break;

		// VOLTAR_NORMALIDADE
		case 5:  center(y_pos,"POIS NAO HA MAIS PERIGO APARENTE"); break;
		
		// EVITAR_COLISAO
		case 6:  center(y_pos,"POIS HA UMA OUTRA FORMIGA NA MINHA FRENTE"); break;

		// PEGAR_COMIDA
		case 7:  center(y_pos,"POIS HA AGUA A FRENTE E AINDA TENHO ESPACO"); break;
		case 8:  center(y_pos,"POIS HA COMIDA A FRENTE E AINDA TENHO ESPACO"); break;

		// SAIR_ALERTA
		case 9:  center(y_pos,"POIS O CHEIRO DE ALOMONIO ESTA DIMINUINDO"); break;

		// SAIR_CAMINHO_RUIM
		case 10: center(y_pos,"POIS HA UMA MARGEM DO MUNDO A FRENTE"); break;
		case 11: center(y_pos,"POIS HA UM OBJETO CHEGANDO ACIMA"); break;
		//case 12: center(y_pos,"POIS O FEROMONIO ESTA DIMINUINDO"); break;
		//case 13: center(y_pos,"POIS O CHEIRO DO NINHO ESTA DIMINUINDO E ESTOU COM FOME"); break;
		//case 14: center(y_pos,"POIS O CHEIRO DO NINHO ESTA DIMINUINDO E ESTOU COM ALIMENTO"); break;
		//case 15: center(y_pos,"POIS O CHEIRO DE ALIMENTO ESTA DIMINUINDO E EU POSSO LEVAR"); break;

		// ENTRAR_NINHO
		case 16: center(y_pos,"POIS O NINHO ESTA A FRENTE E ESTOU COM FOME"); break;
		case 17: center(y_pos,"POIS O NINHO ESTA A FRENTE E ESTOU COM ALIMENTO"); break;

		// SEGUIR_ATRACAO_FRENTE
		case 18: center(y_pos,"POIS HA BASTANTE FEROMONIO A FRENTE"); break;
		case 19: center(y_pos,"POIS HA CHEIRO DO NINHO A FRENTE E ESTOU COM FOME"); break;
		case 20: center(y_pos,"POIS HA CHEIRO DO NINHO A FRENTE E ESTOU COM ALIMENTO"); break;
		case 21: center(y_pos,"POIS HA BASTANTE CHEIRO DE ALIMENTO A FRENTE"); break;

		// SEGUIR_ATRACAO_AMBIGUA
		case 22: center(y_pos,"POIS HA A MESMA QUANTIDADE DE FEROMONIO DOS DOIS LADOS"); break;
		case 23: center(y_pos,"POIS ESTOU CHEIRANDO O NINHO DOS DOIS LADOS E ESTOU COM FOME"); break;
		case 24: center(y_pos,"POIS ESTOU CHEIRANDO O NINHO DOS DOIS LADOS E TENHO ALIMENTO"); break;
		case 25: center(y_pos,"POIS ESTOU CHEIRANDO ALIMENTO DOS DOIS LADOS E POSSO LEVAR"); break;

		// SEGUIR_ATRACAO_DIREITA
		case 26: center(y_pos,"POIS HA A MAIOR CONCENTRACAO DE FEROMONIO A DIREITA"); break;
		case 27: center(y_pos,"POIS O CHEIRO DO NINHO ESTA FORTE A DIREITA E ESTOU COM FOME"); break;
		case 28: center(y_pos,"POIS O CHEIRO DO NINHO ESTA FORTE A DIREITA E TENHO ALIMENTO"); break;
		case 29: center(y_pos,"POIS O CHEIRO DO ALIMENTO ESTA FORTE A DIREITA E POSSO LEVAR"); break;

		// SEGUIR_ATRACAO_ESQUERDA
		case 30: center(y_pos,"POIS HA A MAIOR CONCENTRACAO DE FEROMONIO A ESQUERDA"); break;
		case 31: center(y_pos,"POIS O CHEIRO DO NINHO ESTA FORTE A ESQUERDA E ESTOU COM FOME"); break;
		case 32: center(y_pos,"POIS O CHEIRO DO NINHO ESTA FORTE A ESQUERDA E TENHO ALIMENTO"); break;
		case 33: center(y_pos,"POIS O CHEIRO DO ALIMENTO ESTA FORTE A ESQUERDA E POSSO LEVAR"); break;

		// SEGUIR_ATRACAO_FRENTE
		case 34: center(y_pos,"POIS HA UM OBJETO A FRENTE LONGE"); break;
		case 35: center(y_pos,"POIS HA UM OBJETO A FRENTE PERTO"); break;
		case 36: center(y_pos,"POIS HA UM OBJETO A FRENTE MUITO PERTO (SEM TATO)"); break;
		case 37: center(y_pos,"POIS HA UMA PAREDE BAIXA A FRENTE"); break;
		case 38: center(y_pos,"POIS HA UMA PEQUENA ELEVACAO DE TERRA A FRENTE"); break;

		// SEGUIR_ATRACAO_FRENTE
		case 39: center(y_pos,"POIS HA ALGO ALI EM BAIXO"); break;

		// BUSCAR_SINAL
		case 40: center(y_pos,"POIS NAO HA SINAL ALGUM"); break;

		// SEGUIR_ATRACAO_AMBIGUA
		case 41: center(y_pos,"POIS HA DOIS OBJETOS LONGE"); break;
		case 42: center(y_pos,"POIS HA DOIS OBJETOS PERTO"); break;
		case 43: center(y_pos,"POIS HA DOIS OBJETOS MUITO PERTO"); break;

		// SEGUIR_ATRACAO_DIREITA
		case 44: center(y_pos,"POIS HA UM OBJETO A DIREITA LONGE"); break;
		case 45: center(y_pos,"POIS HA UM OBJETO A DIREITA PERTO"); break;
		case 46: center(y_pos,"POIS HA UM OBJETO A DIREITA MUITO PERTO"); break;

		// SEGUIR_ATRACAO_ESQUERDA
		case 47: center(y_pos,"POIS HA UM OBJETO A ESQUERDA LONGE"); break;
		case 48: center(y_pos,"POIS HA UM OBJETO A ESQUERDA PERTO"); break;
		case 49: center(y_pos,"POIS HA UM OBJETO A ESQUERDA MUITO PERTO"); break;

		// DEFAULT
		case 99:  center(y_pos,"POIS NAO HA OUTRO CASO QUE SE APLICA"); break;
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
	int i, j, x_pos, y_pos, v_x, v_y, temp, saida_rede;

	DEBUG_DOZE_IFS d12;

	// visao

	// "espelho"

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	gotoxy(28,3);

	printf(" CODIGO DA FORMIGA: %i ",index_formiga+1);

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

	gotoxy(45,33);

	ajusta_cor(formiga[index_formiga].demora_subida);
	escreve_numero(formiga[index_formiga].demora_subida);

	gotoxy(44,37);

	ajusta_cor(sequencia_atual.posicao);
	escreve_numero(sequencia_atual.posicao);

	d12=doze_ifs(index_formiga,true,false);

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
	
		explica_linha(44,d12.linha);

	} else
		if(formiga[index_formiga].vida!=viva)
			center(44,"( + + + + + + + + + + + )"); else
			center(44,"(VERIFIQUE VALOR DA VARIAVEL \"DESCANSO - ITERACOES\")");


	center(48,"                                                                   ");

	situacao_para_entradas(index_formiga);

	saida_rede=funcionar_rede(index_formiga);

	if(formiga[index_formiga].vida==viva && formiga[index_formiga].no_ninho==false)
	{
		explica_caso(48,saida_rede);
	} else
	{
		if(formiga[index_formiga].vida!=viva)
			center(48,"( + + + + + + + + + + + )"); else
			center(48,"(ZZ ZZ ZZ ZZ ZZ)");
	}

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

	for(i=-11;i<12;i++)
		for(j=-4;j<5;j++)
		{
			v_x=formiga[index_formiga].coord_x+i;
			v_y=formiga[index_formiga].coord_y+j;

			if(out_of_bounds(v_x,v_y,0))
			{
				setcolor( XBACKGROUND_BLUE | XFOREGROUND_WHITE );
				gotoxy(17+i,35+j);
				printf(" ");
			} else
				if(i!=0 || j!=0)
					visualiza_ponto(v_x,v_y,4,sem_formiga,17+i,35+j);
		}

	gotoxy(17,35);

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

void manda_mensagem(char *texto)
{
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
	DEBUG_DOZE_IFS d12;

	char movimento;

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

	gotoxy(4,32);

	printf(" ");

	gotoxy(4,33);

	printf("M");

	gotoxy(4,34);

	printf("A");

	gotoxy(4,35);

	printf("P");

	gotoxy(4,36);

	printf("A");

	gotoxy(4,37);

	printf(" ");

	gotoxy(51,31);

	printf(" MODO DE OPERACAO ");

	gotoxy(55,35);

	printf(" DESCANSO ");

	center(31," DEMORA DE SUBIDA ");

	center(35," GRAVACAO SEQUENCIA ");

	center(41," ANALISE \"BIOANT CASES\" (ALGORITMO SIMBOLICO) ");

	center(46," ANALISE \"BIOANT NET\" (ALGORITMO CONEXIONISTA) ");

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_BLUE );

	gotoxy(34,37);

	printf("posicao = //");

	gotoxy(33,33);

	printf("iteracoes = //");

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

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );

	center(52," TECLAS (pg. 1)                                      [1] pagina 1, [2] pagina 2 ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	         //................................................................................

	center(53," [CurUp] PARA FRENTE, [CurDown] PARA TRAS, [CurRight] VIRA SENTIDO HORARIO,     ");
	center(54," [CurLeft] VIRA SENTIDO CONTRA HORARIO, [a] SOLTA ALOMONIO, [f] FEROMONIO,      ");
	center(55," [l] MUDA ALERTA, [s] SAIA DO NINHO, [v] MUDA VIDA, [g] FUGIR PERIGO,           ");
	center(56," [n] VOLTAR NORMALIDADE, [o] EVITAR COLISAO, [c] PEGAR COMIDA, [i] SAIR ALERTA, ");
	center(57," [r] SAIR CAMINHO RUIM, [e] ENTRAR NINHO, [t] ATRACAO FRENTE, [b] BUSCAR SINAL, ");
	center(58," [u] ATRACAO AMBIGUA, [d] ATRACAO DIREITA, [q] ATRACAO ESQUERDA, [esc] SAIR     ");

	home();

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
		case 'g': if(pode(index_formiga)) fugir_perigo(index_formiga); break;
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
		case 'y': formiga[index_formiga].estomago=9; break;
		case '+': preenche_linha_x(30,35,0,4,predador); break;
		case '%': situacao_para_entradas(index_formiga); break;
		case '$': gera_saidas_desejadas(index_formiga,1); break;
		case '=': situacao_para_sequencia(index_formiga); 
				  d12=doze_ifs(index_formiga,false,false);
				  if(d12.caso!=5 && d12.caso!=3 && d12.caso!=8) 
					ir_para_frente(index_formiga,false);
				  if(formiga[index_formiga].demora_subida>0) 
						formiga[index_formiga].demora_subida=0;
				  if(sequencia_atual.posicao==20)
				  {
					 fclose(arquivo);
					grava_sequencia_atual(); 
					abre_arquivo();
					init_sequencia();
				  }
				break;
		case '*' :  fclose(arquivo);
					grava_sequencia_atual(); 
					abre_arquivo();
					init_sequencia();
					break;
		case '@' : init_sequencia();
			       break;
		case '1' : setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );
					center(52," TECLAS (pg. 1)                                      [1] pagina 1, [2] pagina 2 ");
					setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );
					center(53," [CurUp] PARA FRENTE, [CurDown] PARA TRAS, [CurRight] VIRA SENTIDO HORARIO,     ");
					center(54," [CurLeft] VIRA SENTIDO CONTRA HORARIO, [a] SOLTA ALOMONIO, [f] FEROMONIO,      ");
					center(55," [l] MUDA ALERTA, [s] SAIA DO NINHO, [v] MUDA VIDA, [g] FUGIR PERIGO,           ");
					center(56," [n] VOLTAR NORMALIDADE, [o] EVITAR COLISAO, [c] PEGAR COMIDA, [i] SAIR ALERTA, ");
					center(57," [r] SAIR CAMINHO RUIM, [e] ENTRAR NINHO, [t] ATRACAO FRENTE, [b] BUSCAR SINAL, ");
					center(58," [u] ATRACAO AMBIGUA, [d] ATRACAO DIREITA, [q] ATRACAO ESQUERDA, [esc] SAIR     ");
					home();
					break;
		case '2' : setcolor( XFOREGROUND_BLACK | XBACKGROUND_WHITE );
					center(52," TECLAS (pg. 2)                                      [1] pagina 1, [2] pagina 2 ");
					setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );
					center(53," [y] ESTOMAGO=9, [+] LINHA TESTE PREDADOR, [%] SITUACAO PARA ENTRADAS DA REDE,  ");
					center(54," [$] TESTAR GERACAO DE SAIDAS, [=] MEMORIZA CONJUNTO NA SEQUENCIA, [*] GRAVA A  ");
					center(55," SEQUENCIA ATUAL, [@] INICIALIZA SEQUENCIA,                                     ");
					center(56,"                                                                                ");
					center(57,"                                                                                ");
					center(58,"                                                                 [esc] SAIR     ");
					home();
					break;

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
	// MUNDO VIRTUAL
	parametros.numero_formigas=20;
	parametros.degradacao_feromonio=20;
	parametros.degradacao_alomonio=50;
	parametros.alcance_feromonio=4;
	parametros.alcance_alomonio=6;
	parametros.alcance_cheiro_ninho=50;
	parametros.alcance_cheiro_alimento=15;
	parametros.gasto_energia=50;
	
	// REDE NEURAL VIRTUAL
	parametros.neuronios_escondidos=20;
	parametros.limite_generec=10000;
	parametros.percentagem_recorrencia=1.0;
	parametros.taxa_aprendizado=0.7;
	parametros.action_timeout=200;
	
	// SIMULACAO
	parametros.modo_neural=true;
	parametros.trace=false;
	parametros.limite_simulacao=ate_sem_acucar;
	parametros.iteracoes_delay=0;
	parametros.visual=v_normal;

	// LOG
	parametros.log_ligado=false;

	// RUNTIME
	parametros.posicao_cursor=0;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  mostra_janela_parametros                      //
//                                                                     //
// CARACTERISTICAS    :  mostra a janela de parametros na tela         //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  01/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void mostra_janela_parametros()
{
	int i;

	setcolor( XBACKGROUND_LIGHTGRAY | XFOREGROUND_BLACK );

	for(i=16;i<51;i++)
		center(i,"                                                                     ");

	setcolor( XBACKGROUND_WHITE | XFOREGROUND_BLACK );

	center(16,"                                                                     ");

	center(16,"PARAMETROS DO SISTEMA BIOANT");

	setcolor( XBACKGROUND_LIGHTGRAY | XFOREGROUND_BLACK );

	center(18,"MUNDO VIRTUAL");

	gotoxy(8,20);
	printf("Numero de Formigas------------>");

	gotoxy(8,21);
	printf("Degradacao Feromonio---------->");

	gotoxy(8,22);
	printf("Degradacao Alomonio----------->");

	gotoxy(8,23);
	printf("Alcance Feromonio------------->");

	gotoxy(8,24);
	printf("Alcance Alomonio-------------->");

	gotoxy(8,25);
	printf("Alcance Cheiro Ninho---------->");

	gotoxy(8,26);
	printf("Alcance Cheiro Alimentos------>");

	gotoxy(8,27);
	printf("Gasto de Energia-------------->");

	center(29,"REDE NEURAL ARTIFICIAL");

	gotoxy(8,31);
	printf("Neuronios Escondidos---------->");

	gotoxy(8,32);
	printf("Taxa de Aprendizado----------->");

	gotoxy(8,33);
	printf("Taxa de Recorrencia----------->");

	gotoxy(8,34);
	printf("Limite Aprendizado------------>");

	center(36,"GERENCIAMENTO DA SIMULACAO");

	gotoxy(8,38);
	printf("Modo de Operacao-------------->");

	gotoxy(8,39);
	printf("Mode Trace-------------------->");

	gotoxy(8,40);
	printf("Limite de Simulacao----------->");

	gotoxy(8,41);
	printf("Delay de Simulacao------------>");

	gotoxy(8,42);
	printf("Visualizacao------------------>");

	center(44,"ARQUIVO DE LOG");

	gotoxy(8,46);
	printf("Geracao de Arquivo-LOG-------->");

	setcolor( XFOREGROUND_WHITE | XBACKGROUND_DARKGRAY );

	for(i=20;i<28;i++)
	{
		gotoxy(39,i);
		printf("                                ");
	}

	for(i=31;i<35;i++)
	{
		gotoxy(39,i);
		printf("                                ");
	}

	for(i=38;i<43;i++)
	{
		gotoxy(39,i);
		printf("                                ");
	}

	gotoxy(39,46);
	printf("                                ");

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGREEN );

	center(48," TECLAS: [CurUp][CurDown] SELECIONAR ITEM, [CurLeft][CurRight] "); 
    center(49,"         ALTERAR VALOR, [esc] SAIR                             ");
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  traduz_cursor                                 //
//                                                                     //
// CARACTERISTICAS    :  traduz a linha do cursor em linha fisica      //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  01/11/03                                      //
/////////////////////////////////////////////////////////////////////////

int traduz_cursor()
{
	switch(parametros.posicao_cursor)
	{
	case 0: return 20;
	case 1: return 21;
	case 2: return 22;
	case 3: return 23;
	case 4: return 24;
	case 5: return 25;
	case 6: return 26;
	case 7: return 27;

	case 8: return 31;
	case 9: return 32;
	case 10: return 33;
	case 11: return 34;

	case 12: return 38;
	case 13: return 39;
	case 14: return 40;
	case 15: return 41;
	case 16: return 42;

	case 17: return 46;
	}
}

void mostra_valor_parametro(bool ligado)
{
	if(ligado) setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW ); else
		setcolor( XFOREGROUND_BLACK | XBACKGROUND_DARKGRAY );

	gotoxy(39,traduz_cursor());

	switch(parametros.posicao_cursor)
	{
	// MUNDO VIRTUAL
	case 0: printf("%i ",parametros.numero_formigas); break;
	case 1: printf("cada %i passo(s) ",parametros.degradacao_feromonio); break;
	case 2: printf("cada %i passo(s) ",parametros.degradacao_alomonio); break;
	case 3: printf("%i casa(s) ",parametros.alcance_feromonio); break;
	case 4: printf("%i casa(s) ",parametros.alcance_alomonio); break;
	case 5: printf("%i casa(s) ",parametros.alcance_cheiro_ninho); break;
	case 6: printf("%i casa(s) ",parametros.alcance_cheiro_alimento); break;
	case 7: printf("casa %i passo(s) ",parametros.gasto_energia); break;

	// REDE NEURAL ARTIFICIAL
	case 8: printf("%i ",parametros.neuronios_escondidos); break;
	case 9: printf("%f ",parametros.taxa_aprendizado); break;
	case 10: printf("%f ",parametros.percentagem_recorrencia); break;
	case 11: printf("%i iteracoes ",parametros.limite_generec); break;

	// SIMULACAO
	case 12: if(parametros.modo_neural) printf("BIOANT NET (alg. conex.)  "); else
				 printf("BIOANT CASES (alg. simb.)  "); break;
	case 13: if(parametros.trace) printf("ligado      "); else
				 printf("desligado     "); break;
	case 14: if(parametros.limite_simulacao==ate_sem_acucar) printf("sem acucar         "); else
				 if(parametros.limite_simulacao==ate_morte) printf("morte das formigas  "); else
					 if(parametros.limite_simulacao==0) printf("desligado            "); else
						 printf("%i        ",parametros.limite_simulacao); break;
	case 15: printf("%i segundo(s) ",parametros.iteracoes_delay); break;
	case 16: switch(parametros.visual)
			 {
				case v_normal:    printf("normal    "); break;
				case v_alomonio:  printf("alomonio  "); break;
				case v_ninho:     printf("ninho     "); break;
				case v_feromonio: printf("feromonio "); break;
				case v_alimento:  printf("alimento  "); break;
				case v_barulho:   printf("barulho   "); break;
			 } break;
	case 17: if(parametros.log_ligado) printf("ligado      "); else
				 printf("desligado     "); break;
	}

	gotoxy(39,traduz_cursor());
}

bool limite_alto_ok()
{
	switch(parametros.posicao_cursor)
	{
	case 0: if(parametros.numero_formigas<99) return true; break;
	case 1: if(parametros.degradacao_feromonio<999) return true; break;
	case 2: if(parametros.degradacao_alomonio<999) return true; break;
	case 3: if(parametros.alcance_feromonio<99) return true; break;
	case 4: if(parametros.alcance_alomonio<99) return true; break;
	case 5: if(parametros.alcance_cheiro_ninho<99) return true; break;
	case 6: if(parametros.alcance_cheiro_alimento<99) return true; break;
	case 7: if(parametros.gasto_energia<999) return true; break;
	case 8: if(parametros.neuronios_escondidos<20) return true; break;
	case 9: if(parametros.taxa_aprendizado<1.0) return true; break;
	case 10: if(parametros.percentagem_recorrencia<1.0) return true; break;
	case 11: if(parametros.limite_generec<10000) return true; break;
	case 12: return true; break;
	case 13: return true; break;
	case 14: if(parametros.limite_simulacao<10000000) return true; break;
	case 15: if(parametros.iteracoes_delay<2) return true; break;
	case 16: if(parametros.visual<v_barulho) return true; break;
	case 17: return true; break;
	}

	return false;
}

bool limite_baixo_ok()
{
	switch(parametros.posicao_cursor)
	{
	case 0: if(parametros.numero_formigas>1) return true; break;
	case 1: if(parametros.degradacao_feromonio>1) return true; break;
	case 2: if(parametros.degradacao_alomonio>1) return true; break;
	case 3: if(parametros.alcance_feromonio>1) return true; break;
	case 4: if(parametros.alcance_alomonio>1) return true; break;
	case 5: if(parametros.alcance_cheiro_ninho>1) return true; break;
	case 6: if(parametros.alcance_cheiro_alimento>1) return true; break;
	case 7: if(parametros.gasto_energia>1) return true; break;
	case 8: if(parametros.neuronios_escondidos>2) return true; break;
	case 9: if(parametros.taxa_aprendizado>0.05) return true; break;
	case 10: if(parametros.percentagem_recorrencia>0.1) return true; break;
	case 11: if(parametros.limite_generec>10) return true; break;
	case 12: return true; break;
	case 13: return true; break;
	case 14: if(parametros.limite_simulacao>-2) return true; break;
	case 15: if(parametros.iteracoes_delay>0) return true; break;
	case 16: if(parametros.visual>v_normal) return true; break;
	case 17: return true; break;
	}

	return false;
}

void aumenta_parametro()
{
	switch(parametros.posicao_cursor)
	{
	case 0: parametros.numero_formigas++; return;
	case 1: parametros.degradacao_feromonio++; return;
	case 2: parametros.degradacao_alomonio++; return;
	case 3: parametros.alcance_feromonio++; return;
	case 4: parametros.alcance_alomonio++; return;
	case 5: parametros.alcance_cheiro_ninho++; return;
	case 6: parametros.alcance_cheiro_alimento++; return;
	case 7: parametros.gasto_energia++; return;
	case 8: parametros.neuronios_escondidos++; return;
	case 9: parametros.taxa_aprendizado=float(parametros.taxa_aprendizado+0.05);
		return;
	case 10: parametros.percentagem_recorrencia=parametros.percentagem_recorrencia+0.1; return;
	case 11: parametros.limite_generec=parametros.limite_generec+10; return;
	case 12: parametros.modo_neural=!parametros.modo_neural; return;
	case 13: parametros.trace=!parametros.trace; return;
	case 14: if(parametros.limite_simulacao<0) parametros.limite_simulacao++; 
		else parametros.limite_simulacao=parametros.limite_simulacao+1000; return;
	case 15: parametros.iteracoes_delay++; return;
	case 16: parametros.visual++; return;
	case 17: parametros.log_ligado=!parametros.log_ligado; return;
	}

}

void diminui_parametro()
{
	switch(parametros.posicao_cursor)
	{
	case 0: parametros.numero_formigas--; return;
	case 1: parametros.degradacao_feromonio--; return;
	case 2: parametros.degradacao_alomonio--; return;
	case 3: parametros.alcance_feromonio--; return;
	case 4: parametros.alcance_alomonio--; return;
	case 5: parametros.alcance_cheiro_ninho--; return;
	case 6: parametros.alcance_cheiro_alimento--; return;
	case 7: parametros.gasto_energia--; return;
	case 8: parametros.neuronios_escondidos--; return;
	case 9: parametros.taxa_aprendizado=parametros.taxa_aprendizado-0.05;
		return;
	case 10: parametros.percentagem_recorrencia=parametros.percentagem_recorrencia-0.1; return;
	case 11: parametros.limite_generec=parametros.limite_generec-10; return;
	case 12: parametros.modo_neural=!parametros.modo_neural; return;
	case 13: parametros.trace=!parametros.trace; return;
	case 14: if(parametros.limite_simulacao<=0) parametros.limite_simulacao--; else
				 parametros.limite_simulacao=parametros.limite_simulacao-1000; return;
	case 15: parametros.iteracoes_delay--; return;
	case 16: parametros.visual--; return;
	case 17: parametros.log_ligado=!parametros.log_ligado; return;
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  bioant_parametros                             //
//                                                                     //
// CARACTERISTICAS    :  ajuste de todos os parametros do bioant       //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  01/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void bioant_parametros()
{
	int i, pos_anterior;

	char escolha;

	mostra_janela_parametros();

	pos_anterior=parametros.posicao_cursor;

	for(i=0;i<18;i++)
	{
		parametros.posicao_cursor=i;
		mostra_valor_parametro(false);
	}

	parametros.posicao_cursor=pos_anterior;

	escolha=' ';

	while(escolha!=char(27))
	{
		setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

		gotoxy(39,traduz_cursor());

		printf("                                ");

		mostra_valor_parametro(true);

		escolha=getch();

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_DARKGRAY );

		gotoxy(39,traduz_cursor());

		printf("                                ");

		mostra_valor_parametro(false);

		switch(escolha)
		{
		case char(72): if(parametros.posicao_cursor>0)
						   parametros.posicao_cursor--;
						break;
		case char(80): if(parametros.posicao_cursor<17)
						   parametros.posicao_cursor++;
						break;
		case char(77): if(limite_alto_ok()) aumenta_parametro(); break;
		case char(75): if(limite_baixo_ok()) diminui_parametro(); break;
		}

	}
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
// NOME DA FUNCAO     :  visualiza_formigas                            //
//                                                                     //
// CARACTERISTICAS    :  visualiza todas as formigas na tela           //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  para usar com pe e passaro..                  //
//                                                                     //
// ULTIMA ATUALIZACAO :  29/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void visualiza_formigas()
{
	int form1;

	for(form1=0;form1<parametros.numero_formigas;form1++)
		if(formiga[form1].no_ninho==false)
			visualiza_ponto(formiga[form1].coord_x,formiga[form1].coord_y,
				formiga[form1].coord_z,form1,99,99);

}

/*void dump_entradas_saidas_funcionamento_rede()
{
	int i;

	for(i=0;i<nro_entradas;i++)
	{
		if(rede.entrada[i].valor!=0.0)
		{
			if(parametros.log_ligado) fprintf(debug,"<rede> ENTRADA %i VALOR %f ===> ",i,rede.entrada[i].valor);
			explica_linha_debug(i);
			if(parametros.log_ligado) fprintf(debug,"\n");
		}
	}

	for(i=0;i<nro_saidas;i++)
	{
		//if(rede.saida[i].esperado!=0.0)
		//{
			if(parametros.log_ligado) fprintf(debug,"<rede> SAIDA %i ESP. %f (+) %f (-) %f===> ",i,rede.saida[i].esperado,rede.saida[i].ativacao[mais],rede.saida[i].ativacao[menos]);
			explica_saida_debug(i);
			if(parametros.log_ligado) fprintf(debug,"\n");
		//}
	}

	i=maior_saida_rede();

	if(parametros.log_ligado) fprintf(debug,"<rede> >>>>> MAIOR SAIDA: %i ",i);
	explica_saida_debug(i);
	if(parametros.log_ligado) fprintf(debug,"\n");

}

void dump_entradas_saidas_aprendizado_continuo()
{
	DEBUG_DOZE_IFS d12;

	int i;

	for(i=0;i<nro_entradas;i++)
	{
		if(rede.entrada[i].valor!=0.0)
		{
			if(parametros.log_ligado) fprintf(debug,"<rede> ENTRADA %i VALOR %f ===> ",i,rede.entrada[i].valor);
			explica_linha_debug(i);
			if(parametros.log_ligado) fprintf(debug,"\n");
		}
	}

	for(i=0;i<nro_saidas;i++)
	{
		if(rede.saida[i].esperado!=0.0)
		{
			if(parametros.log_ligado) fprintf(debug,"<rede> SAIDA %i VALOR %f ===> ",i,rede.saida[i].esperado);
			explica_saida_debug(i);
			if(parametros.log_ligado) fprintf(debug,"\n");
		}
	}
}*/

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  atualiza_formigas_neural                      //
//                                                                     //
// CARACTERISTICAS    :  controladora hibrida de acoes das formigas    //
// no ambiente                                                         //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/2003                                    //
/////////////////////////////////////////////////////////////////////////

void atualiza_formigas_neural(bool alerta_ligado)
{
	int index_formiga, x_anterior, y_anterior, z_anterior;

	for(index_formiga=0;index_formiga<parametros.numero_formigas;index_formiga++)
	{
		if(parametros.trace) // modo trace
			getch();

		x_anterior=999;

		if(formiga[index_formiga].vida==viva && formiga[index_formiga].no_ninho==false) 
		{
			x_anterior=formiga[index_formiga].coord_x;
			y_anterior=formiga[index_formiga].coord_y;
			z_anterior=formiga[index_formiga].coord_z;
			
			decide_morte(index_formiga);		// causas de morte
			if(passaro_presente) comida_por_passaro(index_formiga);
		}

		if(formiga[index_formiga].vida==viva && formiga[index_formiga].descanso==0)
			//(formiga[index_formiga].alerta || alerta_ligado==false))	// atividades das formigas
		{
			if(parametros.log_ligado)
			{
				fprintf(debug,"<REDE> EM ACAO => FORMIGA: %i\n",index_formiga);
				fprintf(debug,"<REDE> ... MACRO: %i",formiga[index_formiga].macro_code);
				devolve_descricao_code(formiga[index_formiga].macro_code);
				fprintf(debug,"\n");
				fprintf(debug,"<REDE>\n");
			}
		
			if(parametros.gasto_energia!=0)		// reduzir estomago
				if(iteracao_mestre % parametros.gasto_energia == 0)
					gasta_energia(index_formiga);

			obtenha_dados_sensores(index_formiga);

			//if(formiga[index_formiga].repeticao>=parametros.action_timeout)
			//	formiga[index_formiga].macro_code=code_escape;

			switch(formiga[index_formiga].macro_code)	// distribuidor central
			{
			case code_escape 				:	//formiga[index_formiga].repeticao=0;
												situacao_para_entradas(index_formiga);
												if(conta_entradas_positivas(index_formiga)!=1 && parametros.log_ligado)
												{
													fprintf(debug,"\n\n+++ +++ +++ +++ +++ +++ +++ +++ +++\n");
													fprintf(debug,"ERRO NO NUMERO DE ENTRADAS (%i)\n",conta_entradas_positivas(index_formiga)); 
													dump_curto_entradas_saidas(index_formiga);
													fprintf(debug,"+++ +++ +++ +++ +++ +++ +++ +++ +++\n\n\n");
												}
												formiga[index_formiga].macro_code=funcionar_rede(index_formiga); 
												desliga_entradas_off(index_formiga);
												break;
			case code_saia_do_ninho 		: macro_saia_do_ninho(index_formiga); break;
			case code_explore_ambiente 		: macro_explore_ambiente(index_formiga); break;
			case code_segue_cheiro_feromonio 	: macro_segue_cheiro_feromonio(index_formiga); break;
			case code_segue_cheiro_alimento 	: macro_segue_cheiro_alimento(index_formiga); break;
			case code_pegue_alimento 		: macro_pegue_alimento(index_formiga); break;
			case code_ache_cheiro_ninho 		: macro_ache_cheiro_ninho(index_formiga); break;
			case code_segue_cheiro_ninho 		: macro_segue_cheiro_ninho(index_formiga); break;
			case code_entre_no_ninho 		: macro_entre_no_ninho(index_formiga); 
												if(x_anterior!=999) 
												visualiza_ponto(x_anterior,y_anterior,z_anterior,sem_formiga,99,99);  // visualizacao "delete"
												break;
			case code_alertar_perigo 		: macro_alertar_perigo(index_formiga); break;
			case code_fugir_do_perigo 		: macro_fugir_do_perigo(index_formiga); break;
			case code_flush 				: //formiga[index_formiga].repeticao=0;
												flush_entradas_off(index_formiga);
												flush_saidas(index_formiga); 
										if(formiga[index_formiga].no_ninho)
											formiga[index_formiga].macro_code=code_saia_do_ninho; else
											formiga[index_formiga].macro_code=code_explore_ambiente;
										break;
			case code_apos_init 			: formiga[index_formiga].macro_code=code_saia_do_ninho; break;
			}
		} else
		{
			//if(formiga[index_formiga].alerta || alerta_ligado==false)
			//{
				if(formiga[index_formiga].descanso>0)
					formiga[index_formiga].descanso--;
				flush_saidas(index_formiga);
				flush_entradas(index_formiga);
				rede[index_formiga].entrada[10].valor=1.0;
				formiga[index_formiga].macro_code=funcionar_rede(index_formiga);
				desliga_entradas_off(index_formiga);
			//dump_entradas_saidas(index_formiga);
			//}
		}

		if(formiga[index_formiga].no_ninho==false)		// visualizacao apos processamento
		{
			if(x_anterior!=999) 
				visualiza_ponto(x_anterior,y_anterior,z_anterior,sem_formiga,99,99);  // visualizacao "delete"

			visualiza_ponto(formiga[index_formiga].coord_x,formiga[index_formiga].coord_y,
				formiga[index_formiga].coord_z,index_formiga,99,99);
		}

	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  atualiza_formigas                             //
//                                                                     //
// CARACTERISTICAS    :  controladora simbolica das formigas no        //
// ambiente                                                            //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/2003                                    //
/////////////////////////////////////////////////////////////////////////

void atualiza_formigas(bool alerta_ligado)
{
	int laco_formigas, x_anterior, y_anterior, z_anterior, resultado, passos_flush, pf_run;

	DEBUG_DOZE_IFS d12;

	for(laco_formigas=0;laco_formigas<parametros.numero_formigas;laco_formigas++)
	{

		if(parametros.trace) 
			getch();

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

				d12=doze_ifs(laco_formigas,false,false);	// chama algoritmo simbolico

				//dump_entradas_saidas();

				if(formiga[laco_formigas].no_ninho==false)
				{

					x_anterior=formiga[laco_formigas].coord_x;
					y_anterior=formiga[laco_formigas].coord_y;
					z_anterior=formiga[laco_formigas].coord_z;

					// nao mover quando come, desliga alerta ou entra no ninho!!
					if(d12.caso!=5 && d12.caso!=3 && d12.caso!=8) 
						ir_para_frente(laco_formigas,false);

					visualiza_ponto(x_anterior,y_anterior,z_anterior,sem_formiga,99,99);
				
				}
		
			} else
			{
				formiga[laco_formigas].descanso--;
				if(formiga[laco_formigas].descanso<=0) saia_do_ninho(laco_formigas);
			}
		//}

		if(formiga[laco_formigas].no_ninho==false)
			visualiza_ponto(formiga[laco_formigas].coord_x,formiga[laco_formigas].coord_y,
				formiga[laco_formigas].coord_z,laco_formigas,99,99);
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
	visualiza_formigas();

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
		visualiza_formigas();

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

	bool break_off;

	//parametros.trace=parametros.trace_default;

	iteracao_mestre=0;

	clrscr( XFOREGROUND_WHITE | XBACKGROUND_BLACK );

	simulacao_mostrar_mundo(4);
	legenda(false);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

	caixa_mensagem(aviso,0,4,"A SIMULACAO COMECARA");

	simulacao_mostrar_mundo(4);

	home();

	teste= ' ';

	break_off=false;

	flush_repeticao_formigas();

	while(teste!=char(27) && 
		(break_off==false || 
		parametros.limite_simulacao==0 || 
		(iteracao_mestre<parametros.limite_simulacao && parametros.limite_simulacao!=ate_morte && parametros.limite_simulacao!=ate_sem_acucar))) // para nao fazer todos os testes na linha de while
	{

		if(parametros.visual==v_feromonio || parametros.visual==v_alomonio || parametros.visual==v_barulho ||
			parametros.visual==v_alimento)
			simulacao_mostrar_mundo(4);

		gotoxy(11,48);

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTGRAY );

		printf("%i",iteracao_mestre);

		if(parametros.modo_neural)
			atualiza_formigas_neural(false); else
		    atualiza_formigas(false);	// todas as formigas

		if(parametros.iteracoes_delay==2) time_delay(1);

		if(parametros.modo_neural)
			atualiza_formigas_neural(true); else
			atualiza_formigas(true);	// apenas as formigas com alerta ligado

		if(iteracao_mestre % 100==0)
			escreve_dados_avaliacao(iteracao_mestre);

		while(kbhit()) teste=getch();

		if(existe_barulho()) boom(false);

		switch(teste)
		{
		case 'p': pezao();
			teste=' '; 
			boom(true);
			break;
		case 'd': limpa_teclas();
			simulacao_debug();
			limpa_teclas();
			teste=' ';
			//parametros.visual=v_normal;
			simulacao_mostrar_mundo(4);
			legenda(false);
			break;
		case 'l': limpa_predador();
			teste=' '; break;
		case 's': define_passaro();
			teste=' '; break;
		case 'o': passaro_presente=!passaro_presente;
			teste=' '; 
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
		case 't': parametros.trace=true;
			break;
		case 'x':parametros.trace=false;
			break;
		}

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_YELLOW );

		gotoxy(60,49);

		if(parametros.trace) 
		{
			printf(" MODO TRACE ");
			teste=getch();
		} else
		{
			setcolor( XFOREGROUND_BLACK | XBACKGROUND_BLACK );
			printf("            ");
		}

		if(passaro_presente) visualiza_passaro();

		iteracao_mestre++;

		if(parametros.iteracoes_delay>0) time_delay(1);

		if(iteracao_mestre % parametros.degradacao_feromonio==0) degradar_feromonio();

		if(iteracao_mestre % parametros.degradacao_alomonio==0) degradar_alomonio();

		if(parametros.limite_simulacao==ate_sem_acucar)
			if(sem_acucar())
				break_off=true;

		if(parametros.limite_simulacao==ate_morte)
			if(todas_mortas())
				break_off=true;
	}

	caixa_mensagem(aviso,0,4,"A SIMULACAO TERMINOU");

	escreve_dados_avaliacao(iteracao_mestre);

	restore();

	dropdown();
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
// NOME DA FUNCAO     :  sem_acucar                                    //
//                                                                     //
// CARACTERISTICAS    :  ha ainda acucar no ambiente?                  //
//                                                                     //
// PARAMETROS         :                                                //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  27/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool sem_acucar()
{
	int x, y, z;

	for(x=0;x<max_mundo_x;x++)
		for(y=0;y<max_mundo_y;y++)
			for(z=0;z<max_mundo_z;z++)
				if(mundo.elemento[x][y][z].paisagem>comida0 &&
					mundo.elemento[x][y][z].paisagem<=comida2)
						return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  todas_mortas                                  //
//                                                                     //
// CARACTERISTICAS    :  todas as formigas morreram?                   //
//                                                                     //
// PARAMETROS         :                                                //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  27/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool todas_mortas()
{
	int x;

	for(x=0;x<parametros.numero_formigas;x++)
		if(formiga[x].vida==viva) return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  existe_barulho                                //
//                                                                     //
// CARACTERISTICAS    :  se existe barulho no ambiente                 //
//                                                                     //
// PARAMETROS         :                                                //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  26/10/03                                      //
/////////////////////////////////////////////////////////////////////////

bool existe_barulho()
{
	if(mundo.elemento[0][0][0].barulho>10) return true;

	return false;
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

	legenda(true);

    nivel=4;

	visualizacao=v_normal;

	char escolha;

	escolha=' ';

	formiga_atual=0;

	atualiza=true;

	while(escolha!=char(27))
	{
		gotoxy(60,50);

		setcolor( XFOREGROUND_BLACK | XBACKGROUND_LIGHTRED );

		printf(" MODO DEBUG ");

		if(atualiza) simulacao_mostrar_mundo(nivel);

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
			case char(13): simulacao_testar_sensores(formiga_atual); legenda(true); atualiza=true; break;
			case 'o' : boom(true); atualiza=true; break;
			case '-' : boom(false); atualiza=true; break;
		}
	}

	gotoxy(60,50);

	setcolor( XFOREGROUND_BLACK | XBACKGROUND_BLACK );

	printf("            ");
	
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
// NOME DA FUNCAO     :  devolve_descricao_entrada                     //
//                                                                     //
// CARACTERISTICAS    :  imprime a descricao de uma entrada na tela    //
//                                                                     //
// PARAMETROS         :  codigo da entrada                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void devolve_descricao_entrada(int codigo)
{
	switch(codigo)
	{
	case 0: printf("perigo presente"); break;
	case 1: printf("objeto visual presente"); break;
	case 2: printf("tato alimento presente"); break;
	case 3: printf("entrada ninho presente"); break;
	case 4: printf("cheiro alomonio presente"); break;
	case 5: printf("cheiro feromonio presente"); break;
	case 6: printf("cheiro ninho presente"); break;
	case 7: printf("cheiro alimento presente"); break;
	case 8: printf("fome presente"); break;
	case 9: printf("carga presente"); break;
	case 10: printf("prestes a sair do ninho"); break;
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  devolve_descricao_saida                       //
//                                                                     //
// CARACTERISTICAS    :  imprime o codigo de uma saida na tela         //
//                                                                     //
// PARAMETROS         :  codigo da saida                               //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void devolve_descricao_saida(int codigo)
{
	switch(codigo)
	{
	case 0: printf("saia do ninho!"); break;
	case 1: printf("explore ambiente!"); break;
	case 2: printf("segue cheiro feromonio!"); break;
	case 3: printf("segue cheiro alimento!"); break;
	case 4: printf("pegue alimento!"); break;
	case 5: printf("ache cheiro ninho!"); break;
	case 6: printf("segue cheiro ninho!"); break;
	case 7: printf("entre no ninho!"); break;
	case 8: printf("alerta perigo!"); break;
	case 9: printf("foge do perigo!"); break;
	case 10: printf("INIBIR perigo presente"); break;
	case 11: printf("INIBIR objeto visual presente"); break;
	case 12: printf("INIBIR tato alimento presente"); break;
	case 13: printf("INIBIR entrada ninho presente"); break;
	case 14: printf("INIBIR cheiro alomonio presente"); break;
	case 15: printf("INIBIR cheiro feromonio presente"); break;
	case 16: printf("INIBIR cheiro ninho presente"); break;
	case 17: printf("INIBIR cheiro alimento presente"); break;
	case 18: printf("INIBIR fome presente"); break;
	case 19: printf("INIBIR carga presente"); break;
	}
}


/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  devolve_descricao_code                        //
//                                                                     //
// CARACTERISTICAS    :  devolve a descricao do codigo de macro        //
//                                                                     //
// PARAMETROS         :  codigo                                        //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  15/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void devolve_descricao_code(int codigo)
{
	switch(codigo)
	{
	case 0: fprintf(debug,"code_escape (chama rede)"); return;
	case 1: fprintf(debug,"code_saia_do_ninho"); return;
	case 2: fprintf(debug,"code_explore_ambiente"); return;
	case 3: fprintf(debug,"code_segue_cheiro_feromonio"); return;
	case 4: fprintf(debug,"code_segue_cheiro_alimento"); return;
	case 5: fprintf(debug,"code_pegue_alimento"); return;
	case 6: fprintf(debug,"code_ache_cheiro_ninho"); return;
	case 7: fprintf(debug,"code_segue_cheiro_ninho"); return;
	case 8: fprintf(debug,"code_entre_no_ninho"); break;
	case 9: fprintf(debug,"code_alertar_perigo"); break;
	case 10: fprintf(debug,"code_fugir_do_perigo"); break;
	case 11: fprintf(debug,"code_flush"); break;
	case 12: fprintf(debug,"code_apos_init"); break;
	}
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
	setcolor( XBACKGROUND_WHITE | XFOREGROUND_BLACK );

	janela(10,18,60,1);

	center(18,"SOBRE O SISTEMA");

	setcolor( XBACKGROUND_LIGHTGRAY | XFOREGROUND_BLACK );

	janela(10,19,60,20);

	center(20,"B I O A N T");

	center(22,"SIMULACAO COMPUTACIONAL CONEXIONISTA BIOLOGICAMENTE");

	center(23,"PLAUSIVEL DE UM AMBIENTE COM FORMIGAS");

	center(25,"Sistema de Dissertacao de Mestrado");

	center(26,"Pontificia Universidade Catolica de Campinas");

	center(28,"Aluno:       Bel.Inf. Marvin Oliver Schneider ");

	center(30,"Orientador:  Prof. Dr. Joao Luis Garcia Rosa  ");

	center(32,"Testes Beta: Eng. Eurico Lacerda Junior       ");

	center(34,"Maiores informacoes e documentacao disponivel em:");

	center(36,"http://www.marvinschneider.hpg.com.br");

	center(38,"Campinas, Dezembro de 2003");

	cor_menu_selecionado2();

	center(40," OK ");

	getch();

	restore();

}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_perigo_presente                         //
//                                                                     //
// CARACTERISTICAS    :  ha algum perigo?                              //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_perigo_presente(int index_formiga)
{
	if(formiga[index_formiga].tato==predador ||
		objeto_acima(index_formiga)==muito_perto ||
		formiga[index_formiga].audicao>=10)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_objeto_visual_presente                  //
//                                                                     //
// CARACTERISTICAS    :  ha objetos a vista?                           //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_objeto_visual_presente(int index_formiga)
{
	if(objeto_frente(index_formiga)>=longe ||
		objeto_esquerda(index_formiga)>=longe ||
		objeto_direita(index_formiga)>=longe ||
		objeto_abaixo(index_formiga)>=longe)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_tato_alimento_presente                  //
//                                                                     //
// CARACTERISTICAS    :  ha alimento a frente?                         //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_tato_alimento_presente(int index_formiga)
{
	if(formiga[index_formiga].tato==agua ||
		formiga[index_formiga].tato>=comida0)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_entrada_ninho_presente                  //
//                                                                     //
// CARACTERISTICAS    :  a entrada do ninho esta a frente?             //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_entrada_ninho_presente(int index_formiga)
{
	if(formiga[index_formiga].tato==ninho &&
		formiga[index_formiga].coord_z==2)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_tato_margem_presente                    //
//                                                                     //
// CARACTERISTICAS    :  estou querendo sair para fora do mundo?       //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_tato_margem_presente(int index_formiga)
{
	if(formiga[index_formiga].tato==margem)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_cheiro_alomonio_presente                //
//                                                                     //
// CARACTERISTICAS    :  ha alomonio no ar?                            //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_cheiro_alomonio_presente(int index_formiga)
{
	if(formiga[index_formiga].olfato_alomonio[0]>0 ||
		formiga[index_formiga].olfato_alomonio[1]>0 ||
		formiga[index_formiga].olfato_alomonio[2]>0 ||
		formiga[index_formiga].olfato_alomonio[3]>0 ||
		formiga[index_formiga].olfato_alomonio[4]>0)  
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_cheiro_feromonio_presente               //
//                                                                     //
// CARACTERISTICAS    :  ha feromonio no ar?                           //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_cheiro_feromonio_presente(int index_formiga)
{
	if(formiga[index_formiga].olfato_feromonio[0] ||
		formiga[index_formiga].olfato_feromonio[1] ||
		formiga[index_formiga].olfato_feromonio[2] ||
		formiga[index_formiga].olfato_feromonio[3] ||
		formiga[index_formiga].olfato_feromonio[4])
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_cheiro_ninho_presente                   //
//                                                                     //
// CARACTERISTICAS    :  estou cheirando o ninho?                      //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_cheiro_ninho_presente(int index_formiga)
{
	if(formiga[index_formiga].olfato_ninho[0] ||
		formiga[index_formiga].olfato_ninho[1] ||
		formiga[index_formiga].olfato_ninho[2] ||
		formiga[index_formiga].olfato_ninho[3] ||
		formiga[index_formiga].olfato_ninho[4])
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_cheiro_alimento_presente                //
//                                                                     //
// CARACTERISTICAS    :  estou cheirando alimento por perto?           //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_cheiro_alimento_presente(int index_formiga)
{
	if(formiga[index_formiga].olfato_alimento[0] ||
		formiga[index_formiga].olfato_alimento[1] ||
		formiga[index_formiga].olfato_alimento[2] ||
		formiga[index_formiga].olfato_alimento[3] ||
		formiga[index_formiga].olfato_alimento[4])
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_fome_presente                           //
//                                                                     //
// CARACTERISTICAS    :  estou em situacao de fome?                    //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_fome_presente(int index_formiga)
{
	if(formiga[index_formiga].estomago<10)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  inter_carga_presente                          //
//                                                                     //
// CARACTERISTICAS    :  estou com carga?                              //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

bool inter_carga_presente(int index_formiga)
{
	if(formiga[index_formiga].estomago_social>=10)
		return true;

	return false;
}

bool trata_eventos_menores(int index_formiga)
{
	if(formiga[index_formiga].tato==outra)
	{
		evitar_colisao(index_formiga);
		ir_para_frente(index_formiga,false);
		return true;
	}

	if(formiga[index_formiga].tato==margem)
	{
		muda_aleatoriamente_direcao(index_formiga);
		ir_para_frente(index_formiga,false);
		return true;
	}

	//if(formiga[index_formiga].repeticao>=parametros.action_timeout)
	//	return true;

	return false;
}

void desliga_entradas_off(int index_formiga)
{
	int i;

	for(i=0;i<10;i++)
	{
		if(rede[index_formiga].saida[i+10].ativacao[menos]>=0.5)
			rede[index_formiga].entrada[i].off=true; else
			rede[index_formiga].entrada[i].off=false;
	}
}

int conta_entradas_positivas(int index_formiga)
{
	int i, flag;

	flag=0;

	for(i=0;i<nro_entradas;i++)
	{
		if(rede[index_formiga].entrada[i].valor!=0.0)
			flag++;
	}

	return flag;
}

bool caso_escape(int index_formiga)
{
	if(rede[index_formiga].entrada[0].off==false && inter_perigo_presente(index_formiga))
		return true;

	if(rede[index_formiga].entrada[1].off==false && inter_objeto_visual_presente(index_formiga))
		return true;

	if(rede[index_formiga].entrada[2].off==false && inter_tato_alimento_presente(index_formiga))
		return true;

	if(rede[index_formiga].entrada[3].off==false && inter_entrada_ninho_presente(index_formiga))
		return true;

	if(rede[index_formiga].entrada[4].off==false && inter_cheiro_alomonio_presente(index_formiga))
		return true;

	if(rede[index_formiga].entrada[5].off==false && inter_cheiro_feromonio_presente(index_formiga))
		return true;

	if(rede[index_formiga].entrada[6].off==false && inter_cheiro_ninho_presente(index_formiga))
		return true;

	if(rede[index_formiga].entrada[7].off==false && inter_cheiro_alimento_presente(index_formiga))
		return true;

	if(rede[index_formiga].entrada[8].off==false && inter_fome_presente(index_formiga))
		return true;

	if(rede[index_formiga].entrada[9].off==false && inter_carga_presente(index_formiga))
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  macro_saia_do_ninho                           //
//                                                                     //
// CARACTERISTICAS    :  controladora de saida do ninho                //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  11/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void macro_saia_do_ninho(int index_formiga)
{
	//printf("out:%i ",index_formiga);
	//getch();

	if(saia_do_ninho(index_formiga)==false)
		formiga[index_formiga].macro_code=code_saia_do_ninho; else
		formiga[index_formiga].macro_code=code_escape;

	//formiga[index_formiga].repeticao++;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  macro_explore_ambiente                        //
//                                                                     //
// CARACTERISTICAS    :  controladora de exploracao de ambiente        //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void macro_explore_ambiente(int index_formiga)
{
	
	if(caso_escape(index_formiga))
		formiga[index_formiga].macro_code=code_escape; else
	{
		formiga[index_formiga].macro_code=code_explore_ambiente;

		if(trata_eventos_menores(index_formiga)==false)
		{
			if(objeto_frente(index_formiga)>=longe)
			{
				seguir_atracao_frente(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(objeto_esquerda(index_formiga)>0 &&
				objeto_esquerda(index_formiga)==objeto_direita(index_formiga))
			{
				seguir_atracao_ambigua(index_formiga);
				ir_para_frente(index_formiga,false);
			} else	

			if(objeto_esquerda(index_formiga)>objeto_direita(index_formiga))
			{
				seguir_atracao_esquerda(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(objeto_direita(index_formiga)>objeto_esquerda(index_formiga))
			{
				seguir_atracao_direita(index_formiga);
				ir_para_frente(index_formiga,false);
			} else
			{
				buscar_sinal(index_formiga);
				ir_para_frente(index_formiga,false);
			}
		}
	}

	//formiga[index_formiga].repeticao++;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  macro_segue_cheiro_feromonio                  //
//                                                                     //
// CARACTERISTICAS    :  controladora de seguimento de feromonio       //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void macro_segue_cheiro_feromonio(int index_formiga)
{
	if(caso_escape(index_formiga))
		formiga[index_formiga].macro_code=code_escape; else
	{
		formiga[index_formiga].macro_code=code_segue_cheiro_feromonio;

		if(trata_eventos_menores(index_formiga)==false)
		{
			if(olfato_frente_maior(index_formiga,c_feromonio))
			{
				seguir_atracao_frente(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(olfato_esquerda_maior(index_formiga,c_feromonio) &&
				olfato_direita_maior(index_formiga,c_feromonio))
			{
				seguir_atracao_ambigua(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(olfato_esquerda_maior(index_formiga,c_feromonio))
			{
				seguir_atracao_esquerda(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(olfato_direita_maior(index_formiga,c_feromonio))
			{
				seguir_atracao_direita(index_formiga);
				ir_para_frente(index_formiga,false);
			} else
			{
				buscar_sinal(index_formiga);
				ir_para_frente(index_formiga,false);
			}
		}
	}

	//formiga[index_formiga].repeticao++;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  macro_segue_cheiro_alimento                   //
//                                                                     //
// CARACTERISTICAS    :  controladora de seguimento de cheiro de       //
// alimento                                                            //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void macro_segue_cheiro_alimento(int index_formiga)
{
	if(caso_escape(index_formiga))
		formiga[index_formiga].macro_code=code_escape; else
	{
		formiga[index_formiga].macro_code=code_segue_cheiro_alimento;

		if(trata_eventos_menores(index_formiga)==false)
		{
			if(olfato_frente_maior(index_formiga,c_alimento))
			{
				seguir_atracao_frente(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(olfato_esquerda_maior(index_formiga,c_alimento) &&
				olfato_direita_maior(index_formiga,c_alimento))
			{
				seguir_atracao_ambigua(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(olfato_esquerda_maior(index_formiga,c_alimento))
			{
				seguir_atracao_esquerda(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(olfato_direita_maior(index_formiga,c_alimento))
			{
				seguir_atracao_direita(index_formiga);
				ir_para_frente(index_formiga,false);
			} else
			{
				buscar_sinal(index_formiga);
				ir_para_frente(index_formiga,false);
			}

		}
	}

	//formiga[index_formiga].repeticao++;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  macro_pegue_alimento                          //
//                                                                     //
// CARACTERISTICAS    :  controladora de pegar alimento                //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void macro_pegue_alimento(int index_formiga)
{
	// BUG resolvido em 24/11/03 !!!

	if((formiga[index_formiga].tato<=comida0 && formiga[index_formiga].tato!=agua) ||
		formiga[index_formiga].estomago_social>=89)
	  formiga[index_formiga].macro_code=code_escape; else

	{
		formiga[index_formiga].macro_code=code_pegue_alimento;

		if(trata_eventos_menores(index_formiga)==false)
		{
			come(index_formiga);
		}
	}

	//formiga[index_formiga].repeticao++;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  macro_ache_cheiro_ninho                       //
//                                                                     //
// CARACTERISTICAS    :  controladora para achar o cheiro do ninho     //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void macro_ache_cheiro_ninho(int index_formiga)
{
	if(caso_escape(index_formiga))
		formiga[index_formiga].macro_code=code_escape; else

	{
		formiga[index_formiga].macro_code=code_ache_cheiro_ninho;


		if(trata_eventos_menores(index_formiga)==false)
		{
			if(objeto_frente(index_formiga)>=longe)
			{
				seguir_atracao_frente(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(objeto_esquerda(index_formiga)>0 &&
				objeto_esquerda(index_formiga)==objeto_direita(index_formiga))
			{
				seguir_atracao_ambigua(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(objeto_esquerda(index_formiga)>objeto_direita(index_formiga))
			{
				seguir_atracao_esquerda(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(objeto_direita(index_formiga)>objeto_esquerda(index_formiga))
			{
				seguir_atracao_direita(index_formiga);
				ir_para_frente(index_formiga,false);
			} else
			{
				buscar_sinal(index_formiga);
				ir_para_frente(index_formiga,false);
			}
		
		}
	}

	//formiga[index_formiga].repeticao++;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  macro_segue_cheiro_ninho                      //
//                                                                     //
// CARACTERISTICAS    :  controladora para seguimento do cheiro do     //
// ninho                                                               //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void macro_segue_cheiro_ninho(int index_formiga)
{
	if(caso_escape(index_formiga))
		formiga[index_formiga].macro_code=code_escape; else
	{

		if(trata_eventos_menores(index_formiga)==false)
		{
			formiga[index_formiga].macro_code=code_segue_cheiro_ninho;

			if(trata_eventos_menores(index_formiga)==false)
			{
				if(olfato_frente_maior(index_formiga,c_ninho))
				{
					seguir_atracao_frente(index_formiga);
					ir_para_frente(index_formiga,false);
				} else	

				if(olfato_esquerda_maior(index_formiga,c_ninho) &&
					olfato_direita_maior(index_formiga,c_ninho))
				{
					seguir_atracao_ambigua(index_formiga);
					ir_para_frente(index_formiga,false);
				} else

				if(olfato_esquerda_maior(index_formiga,c_ninho))
				{
					seguir_atracao_esquerda(index_formiga);
					ir_para_frente(index_formiga,false);
				} else

				if(olfato_direita_maior(index_formiga,c_ninho))
				{
					seguir_atracao_direita(index_formiga);
					ir_para_frente(index_formiga,false);
				} else
				{
					buscar_sinal(index_formiga);
					ir_para_frente(index_formiga,false);
				}
			}
		}
	}

	//formiga[index_formiga].repeticao++;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  macro_entre_no_ninho                          //
//                                                                     //
// CARACTERISTICAS    :  controladora para entrada no ninho            //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void macro_entre_no_ninho(int index_formiga)
{
	if(inter_entrada_ninho_presente(index_formiga))
		entrar_ninho(index_formiga,false); else
	{
		buscar_sinal(index_formiga);
		ir_para_frente(index_formiga,false);
	}

	//formiga[index_formiga].macro_code=code_flush;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  macro_alertar_perigo                          //
//                                                                     //
// CARACTERISTICAS    :  controladora de alerta de perigo              //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void macro_alertar_perigo(int index_formiga)
{
	if(parametros.log_ligado)
	{
		fprintf(debug,"<REDE> ========= FUGIR DO PERIGO ===========\n");
		dump_curto_entradas_saidas(index_formiga);
		fprintf(debug,"<REDE> =====================================\n");
	}

	if(inter_perigo_presente(index_formiga) && trata_eventos_menores(index_formiga)==false)
	{
		solta_alomonio(index_formiga);

		formiga[index_formiga].macro_code=code_fugir_do_perigo;
	} else
	{
		buscar_sinal(index_formiga);

		formiga[index_formiga].macro_code=code_explore_ambiente;
	}

	formiga[index_formiga].repeticao++;
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  macro_fugir_do_perigo                         //
//                                                                     //
// CARACTERISTICAS    :  controladora de evasao do perigo              //
//                                                                     //
// PARAMETROS         :  numero da formiga                             //
// OBSERVACOES        :  -                                             //
//                                                                     //
// ULTIMA ATUALIZACAO :  14/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void macro_fugir_do_perigo(int index_formiga)
{
	if(trata_eventos_menores(index_formiga)==false)
	{
		formiga[index_formiga].macro_code=code_fugir_do_perigo;

		if(trata_eventos_menores(index_formiga)==false)
		{
			if(olfato_frente_menor(index_formiga,c_alomonio))
			{
				seguir_atracao_frente(index_formiga);
				ir_para_frente(index_formiga,false);
			} else	

			if(olfato_esquerda_menor(index_formiga,c_alomonio) &&
				olfato_direita_menor(index_formiga,c_alomonio))
			{
				seguir_atracao_ambigua(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(olfato_esquerda_menor(index_formiga,c_alomonio))
			{
				seguir_atracao_esquerda(index_formiga);
				ir_para_frente(index_formiga,false);
			} else

			if(olfato_direita_menor(index_formiga,c_alomonio))
			{
				seguir_atracao_direita(index_formiga);
				ir_para_frente(index_formiga,false);
			} else
			{
				buscar_sinal(index_formiga);
				ir_para_frente(index_formiga,false);
				formiga[index_formiga].macro_code=code_escape;
			}
		}
	}

	formiga[index_formiga].repeticao++;
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

	valor=0;
	
	while(valor!=4)
	{
		valor=xrand(5);
		printf("%i ",valor);
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
// NOME DA FUNCAO     :  teste_conversor                               //
//                                                                     //
// CARACTERISTICAS    :  testa o comportamento do conversor decimal p/ //
// binario                                                             //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  apenas para o desenvolvimento                 //
//                                                                     //
// ULTIMA ATUALIZACAO :  30/10/03                                      //
/////////////////////////////////////////////////////////////////////////

void teste_conversor()
{
	dec_to_bin(1);
	dec_to_bin(2);
	dec_to_bin(4);
	dec_to_bin(64);
	dec_to_bin(65);
	dec_to_bin(65);
	dec_to_bin(63);
	dec_to_bin(87);
	dec_to_bin(88);
	dec_to_bin(85);
	dec_to_bin(90);
	dec_to_bin(94);
	dec_to_bin(99);
	dec_to_bin(96);

	getch();
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  teste_gerador                                 //
//                                                                     //
// CARACTERISTICAS    :  testa o gerador de situacao aleatoria         //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  apenas para o desenvolvimento                 //
//                                                                     //
// ULTIMA ATUALIZACAO :  01/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void teste_gerador()
{
	while(true)
	{
		gera_situacao_aleatoria();
		dump_valores_formiga_100();
	}
}

/////////////////////////////////////////////////////////////////////////
// NOME DA FUNCAO     :  dump_curto_entradas_saidas                    //
//                                                                     //
// CARACTERISTICAS    :  dump curto das entradas e saidas da rede      //
//                                                                     //
// PARAMETROS         :  -                                             //
// OBSERVACOES        :  apenas para o desenvolvimento                 //
//                                                                     //
// ULTIMA ATUALIZACAO :  15/11/03                                      //
/////////////////////////////////////////////////////////////////////////

void dump_curto_entradas_saidas(int index_formiga)
{
	int i;

	fprintf(debug,"<REDE> SITUACAO PARA FORMIGA: %i\n",index_formiga);

	fprintf(debug,"<REDE> ENTRADAS: ");
	
	for(i=0;i<nro_entradas;i++)
		fprintf(debug,"%f=(%i) ",rede[index_formiga].entrada[i].valor,rede[index_formiga].entrada[i].off);

	fprintf(debug,"\n");

	fprintf(debug,"<REDE> SAIDAS: ");

	for(i=0;i<nro_saidas;i++)
		fprintf(debug,"%f ",rede[index_formiga].saida[i].ativacao[menos]);

	fprintf(debug,"\n");

	fprintf(debug,"<REDE>\n");
}

void dump_compacto_rede(int index_formiga)
{
	int i, j;

	fprintf(debug,"<REDE> SITUACAO PARA FORMIGA: %i\n",index_formiga);

	fprintf(debug,"<REDE> ENTRADAS: ");
	
	for(i=0;i<nro_entradas;i++)
		fprintf(debug,"%f (%i)",rede[index_formiga].entrada[i].valor,rede[index_formiga].entrada[i].off);

	fprintf(debug,"\n");

	fprintf(debug,"<REDE> SINAPSES 1: ");
	
	for(i=0;i<nro_entradas;i++)
		for(j=0;j<parametros.neuronios_escondidos;j++)
			fprintf(debug,"%f ",rede[index_formiga].sinapse_EN[i][j].valor);

	fprintf(debug,"\n");

	fprintf(debug,"<REDE> ESCONDIDOS: ");
	
	for(i=0;i<parametros.neuronios_escondidos;i++)
		fprintf(debug,"%f=>%f",rede[index_formiga].neuronio[i].valor,rede[index_formiga].neuronio[i].ativacao);

	fprintf(debug,"\n");

	fprintf(debug,"<REDE> SINAPSES 2: ");
	
	for(i=0;i<parametros.neuronios_escondidos;i++)
		for(j=0;j<nro_saidas;j++)
			fprintf(debug,"%f ",rede[index_formiga].sinapse_NS[i][j].valor);

	fprintf(debug,"\n");

	fprintf(debug,"<REDE> SAIDAS: ");

	for(i=0;i<nro_saidas;i++)
		fprintf(debug,"%f=%f=%f",rede[index_formiga].saida[i].ativacao[menos],rede[index_formiga].saida[i].ativacao[mais],rede[index_formiga].saida[i].esperado);

	fprintf(debug,"\n");

	fprintf(debug,"<REDE>\n");

}

/////////////////////////////////////////////////////////////////////////
// 7. PRINCIPAL                                                        //
/////////////////////////////////////////////////////////////////////////

void main()
{	
//	random_test();
//	ascii();
	
	init_bin();

	init_sequencia();

//	teste_conversor();
//	teste_gerador();

	abre_arquivo();
	abre_arquivo_debug();
	abre_arquivo_rede();
	abre_arquivo_avaliacao();

//	getch();

	//explica_situacao_rede();

	configura_numero_de_filhos();

	parametros_padrao();

	//parametros.log_ligado=true;
	rede_inicializar(0,true);
	clone_brains();

	simulacao_inicializar_mundo();
	init_formigas();

	init_menu_principal();
	codigo_extraido="";

	parametros.visual=v_normal;

	x_cursor=0; // menu
	y_cursor=0;

	cls();

	caixa_mensagem(aviso,0,12,"Favor maximizar a janela para melhor visualizacao!");

	cls();

	atualiza_cabecalho();

	//subtitulo();

	home();

	ajuda_sobre();

//	rede_testar_aprendizado();

	while(codigo_extraido!="XX" && codigo_extraido!="ZZ")
	{
		executa_menu();

		if(codigo_extraido=="XX" || codigo_extraido=="ZZ") 
		{
			limpa_dropdown();
			arquivo_sair_do_programa();
			restore();
			y_cursor=0;
		}

		if(codigo_extraido=="IN") 
		{
			rede_inicializar(0,false);
			clone_brains();
		}
		if(codigo_extraido=="TR") 
		{
			rede_treinar();
			clone_brains();
		}
		if(codigo_extraido=="AA") avaliar_aprendizado();
		if(codigo_extraido=="DP") 
		{
			limpa_dropdown();
			rede_dump(0);
		}
		if(codigo_extraido=="DE") 
		{
			limpa_dropdown();
			dump_entradas_saidas(0);
		}

		if(codigo_extraido=="IA") 
		{
			if(caixa_mensagem(pergunta,0,caixa_padrao,"Inicializar o ambiente (a situacao atual sera perdida)?"))
			simulacao_inicializar();
			limpa_caixa();
			dropdown();
		}
		if(codigo_extraido=="PA") 
		{
			limpa_dropdown();
			bioant_parametros();
			restore();
			dropdown();
		}
		if(codigo_extraido=="EX") simulacao_executar();
		if(codigo_extraido=="DB") {
			simulacao_debug();
			restore();
			dropdown();
		}
		
		if(codigo_extraido=="SO") 
		{
			limpa_dropdown();
			ajuda_sobre();
			dropdown();
		}

		if(codigo_extraido=="MS")
		{
			limpa_dropdown();
			gerencia_sequencia_atual();
			restore();
			dropdown();
		}

		if(codigo_extraido=="GS")
		{
			simulacao_testar_sensores(0);
			restore();
			dropdown();
		}

		if(codigo_extraido=="ZS")
		{
			limpa_dropdown();
			if(caixa_mensagem(pergunta,0,15,"Zerar o arquivo BIOANT.SEQ?"))
				deleta_arquivo();
			restore();
			dropdown();
		}

		if(codigo_extraido=="ML")
		{
			mostra_log();
			restore();
			dropdown();
		}

		if(codigo_extraido=="ZL")
		{
			if(caixa_mensagem(pergunta,0,15,"Zerar o Arquivo de BIOANT.LOG?"))
				deleta_arquivo_debug();
			restore();
			dropdown();
		}

		if(codigo_extraido=="GR")
		{
			limpa_dropdown();
			if(caixa_mensagem(pergunta,0,15,"Isso vai sobrescrever o arquivo atual BIOANT.RED. Continuar?"))
			{
				grava_rede();
				restore();
				caixa_mensagem(aviso,0,15,"A rede atual foi gravada em BIOANT.RED.");
			}
			restore();
			dropdown();
		}

		if(codigo_extraido=="LR")
		{
			limpa_dropdown();
			if(caixa_mensagem(pergunta,0,15,"Isso vai sobrepor a rede atual. Continuar?"))
			{
				le_rede();
				restore();
				caixa_mensagem(aviso,0,15,"Os dados foram lidos de BIOANT.RED.");
			}
			restore();
			dropdown();
		}
	}

	fclose(arquivo);
	fclose(debug);
	fclose(aval);
}
