#include <ncurses.h>

#include "lib_dados.h"

#define ALT_TELA 38
#define LARG_TELA 100
/*DESENHO ALIEN*/
#define ALIEN_T11_1 "/__\\"
#define ALIEN_T11_2 ">->-"
#define ALIEN_T11_3 "[OO]"
#define ALIEN_T12_1 "/--\\"
#define ALIEN_T12_2 "->->"
#define ALIEN_T12_3 "[OO]"


#define ALIEN_T21_1 "{**}"
#define ALIEN_T21_2 "/oo\\"
#define ALIEN_T21_3 "\\vv/"
#define ALIEN_T22_1 "{**}"
#define ALIEN_T22_2 "/oo\\"
#define ALIEN_T22_3 "//\\\\"


#define ALIEN_T31_1 "[^^]"
#define ALIEN_T31_2 "\\MM/"
#define ALIEN_T31_3 "{><}"
#define ALIEN_T32_1 "[00]"
#define ALIEN_T32_2 "/MM\\"
#define ALIEN_T32_3 "{><}"

#define EXPLODINDO1 "\\../"
#define EXPLODINDO2 " .. "
#define EXPLODINDO3 "/ |\\"

/*DESENHO CANHAO*/
#define CANHAO1 "-/^\\-"
#define CANHAO2 "/___\\"

#define TIRO "|"
#define BOMBA "$"
#define BARREIRA "="

int inicia_tela(WINDOW **tela);

void imprime_area_tela(WINDOW *tela);

void imprime_aliens(WINDOW *tela, t_jogo *jogo);

void imprime_canhao(WINDOW *tela, t_listaG *canhao);

void imprime_tiro(WINDOW *tela, t_listaG *tiro);

void imprime_bomba(WINDOW *tela, t_listaG *bomba);

void imprime_barreira(WINDOW *tela, t_jogo *jg);

void imprime_naveMae(WINDOW *tela, t_listaG *naveMae);

void imprime_pontuacao(t_jogo *jg);

void tela_venceu(WINDOW *tela);

void tela_perdeu(WINDOW *tela, t_jogo *jg);

void imprime_tela(WINDOW *tela_jogo, t_jogo *jogo);