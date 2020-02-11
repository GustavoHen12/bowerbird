#define QUANT_ALIENS_X 11
#define QUANT_ALIENS_Y 5
#define POS_INI_ALIENSX 1
#define POS_INI_ALIENSY 7
#define LARG_ALIEN 4
#define COMP_ALIEN 3
#define DIST_ALIEN 1
#define VELOCIDADE_ALIENS 15

#define LINHA_BARREIRA 30
#define DIST_BARREIRA 15
#define MARGEM_BARREIRA 14
#define ALT_BARREIRA 3
#define LARG_BARREIRA 8
#define QUANT_BARREIRAS 4

#define POS_INI_CANHAO_X 34
#define POS_INI_CANHAO_Y 34
#define VELOCIDADE_CANHAO 1

#define POS_INI_NVMAE_X -8
#define POS_INI_NVMAE_Y 1
#define LARG_NVMAE 8 
#define ALT_NVMAE 3
#define VELOCIDADE_NVMAE 2

#define VELOCIDADE_BOMBA 4
#define VELOCIDADE_TIRO 3



/*todos os elementos do jogo sao listas, algumas possuem diferentes atributos*/
/*LISTA ALIEN*/
struct nodoA {
    int posX, posY; /*posicao do canto superior esquerdo de cada alien*/
    int estado; /*estado 1->vivo 0->explodindo, se morto eh retirado da lista*/
    int tipo; /*cada linha tem um tipo de alien*/
    struct nodoA *prox;
    struct nodoA *prev;
};
typedef struct nodoA nodoAlien_t;

struct listaA {
    nodoAlien_t *ini;
    nodoAlien_t *atual;
    nodoAlien_t *fim;
    int quant, velocidade, direcao; /*velocidade dos alien na tela e quantidade de aliens vivos ou explodindo*/
};
typedef struct listaA t_alien;

/*LISTA GERAL*/
/*A lista geral sera utilizada para a maioria dos objetos do jogo*/
struct nodo {
    int posX, posY; /*posicao do canto superior esquerdo da nave*/
    struct nodo *prox;
    struct nodo *prev;
};
typedef struct nodo t_nodo;

struct listaG {
    t_nodo *ini;
    t_nodo *atual;
    t_nodo *fim;
    int velocidade, quant;
};
typedef struct listaG t_listaG;

struct jogo{
  t_alien aliens; /*declara a lista de aliens*/
  t_listaG canhao, naveMae, tiro, bomba;
  t_listaG barreira0, barreira1, barreira2, barreira3;
  int pontuacao, vidas;
};
typedef struct jogo t_jogo;

/*-------------LISTA------------*/
int lista_vazia(t_listaG *l);
/*  inicializa      */
/*retorna 1 se conseguiu inicializar a lista corretamente*/
int inicializa_listaGeral(t_listaG *l, int vel);
int inicializa_listaAliens(t_alien *l, int vel);
int inicia_atual(t_listaG *l);
int inicia_atualAlien(t_alien *l);

/*  insere      */
/*considerando uma posicao X e Y insere um objeto no final da lista
* retorna 1 se a insercao ocorreu bem*/
int insereObjeto(int posX, int posY,  t_listaG *l);

/*insere um alien de posicao (X, Y) e tipo, no fim da lista
* retorna 1 se ocorreu corretamente*/
int insereAlien(int posX, int posY, int tipo, t_jogo *jg);

/*  apaga       */
int apagaObjeto(int posX, int posY, t_listaG *l);
int apagaAlien(int posX, int posY, t_jogo *jg);

/*  consulta        */
/*retorna os dados para qual o atual esta apontando
* se estiver apontando para invalido ou vazio nao altera os valores*/
void dados_atual(int *posX, int *posY, t_listaG *l);
void dados_atualAlien(int *posX, int *posY, t_alien *l);
void outros_dadosAlien(int *estado, int *tipo, t_alien *l);

/*se o objeto de posicao posX e posY existir retorna 1 caso contrario 0*/
int busca_objeto(int posX, int posY, t_listaG *l);

/*retorna a o dados de velocidade da lista*/
int velocidade_Alien(t_alien *aliens);
int velocidade_Objeto(t_listaG *l);

/*coloca em quanto tamanho da lista, ou a quantidade de objetos nela*/
void quantidadeObjetos(int *quant, t_listaG *l);
void quantidadeAliens(int *quant, t_alien *l);

/*retorna a direcao para a qual os aliens entao seguindo (0->, <-1)*/
int direcao_alien(t_alien *aliens);

/*  altera      */
void altera_atual(int posX, int posY, t_listaG *l);
/*altera o estado do alien passado para explodindo*/
int altera_estadoAlien(int posX, int posY, t_jogo *jg);
/*altera a velocidade da lista de aliens*/
void altera_velocidade(int velocidade, t_alien *aliens);

/*  manipula atual*/
/*se o atual nao estiver no ultimo elemento ele passa para o prox*/
void incrementa_atual(t_listaG *l);

/*se nao conseguiu avancar(fim da lista) retorna 0*/
int incrementa_atualAlien(t_alien *l);

/*-------------OUTRAS------------*/
int iniciaDados(t_jogo *jg);

void inicia_pontos_vida(t_jogo *jogo);

/*retira da lista todos os aliens qque estao explodindo*/
void limpa_Alien(t_jogo *jg);

/*destroi todas as listas do jogo*/
int apaga_jogo(t_jogo *jg);

/*consulta a lista e rretorna a posicao do alien mais a esquerda e mais a direita*/
void extremosAlien(int *esquerda, int *direitas, t_jogo *jg);

/*consulta lista e retorna a posicao do alien mais inferior
* soma-se a sua parte superior seu comprimentos*/
void alturaInferiorAlien(int *altAlien, t_jogo *jg);

/*incrementa ou decrementa posX do canhao, dependendo dda direcao passada*/
void altera_posCanhao(t_listaG *canhao, int dir);

/*se os aliens estiveren indo para esquerda(0) vao para direita(1), e vice-versa*/
void altera_posAlien(t_alien *aliens, int dir);

/*faz os aliens inverterem seu estado para altterar os sprites*/
void inverte_estadoAlien(t_alien *l);

/*aumenta a pontuacao em pont*/
void incrementa_pontuacao(t_jogo *jg, int pont);

/*perde uma vida, retorna 1 se maior que zero*/
int decrementa_vida(t_jogo *jg);