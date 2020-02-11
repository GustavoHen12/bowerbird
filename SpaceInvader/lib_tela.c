#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib_tela.h"

int inicia_tela(WINDOW **tela)
{
	/*iniciaa tela padrao*/
	initscr();
    cbreak();              
    noecho();             
	nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

	/*iniciaa segunda tela onde sera imprresso o jogo*/
	*tela = newwin(ALT_TELA, LARG_TELA, 0, 0);
    nodelay(*tela, TRUE);
    keypad(*tela, TRUE);

    curs_set(FALSE);
    
	/*verifica se o tamanho da tela esta correto*/
	int lin, col;
	getmaxyx(stdscr, lin, col);
	if ((lin < ALT_TELA) && (col < LARG_TELA))
		return 0;

	/*inicia cores qque serao utilizaaddaaass durante o jogos*/
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_GREEN,COLOR_BLACK);
    init_pair(4,COLOR_CYAN,COLOR_BLACK);
    init_pair(5,COLOR_YELLOW,COLOR_BLACK);
	return 1;
}

void imprime_area_tela(WINDOW *tela)
{
	/*iniciaa aaass cores e imprime a borda da tela*/
	wattron(tela, COLOR_PAIR(1));
	box(tela, 0, 0);
}

void imprime_aliens(WINDOW *tela, t_jogo *jogo)
{
	int alienX, alienY, estado, tipo;
	int i, quantAlien;	
    wattron(tela, COLOR_PAIR(1));

	quantidadeAliens(&quantAlien, &(jogo->aliens));
	inicia_atualAlien(&(jogo->aliens));

	for(i = 0; i < quantAlien; i++)
	{
		dados_atualAlien(&alienX, &alienY, &(jogo->aliens));
		outros_dadosAlien(&estado, &tipo, &(jogo->aliens));
		if(estado == 2)
		{
			wattron(tela, COLOR_PAIR(5));
			mvwprintw(tela, alienY,alienX, EXPLODINDO1);
			mvwprintw(tela, alienY+1,alienX, EXPLODINDO2);
			mvwprintw(tela, alienY+2,alienX, EXPLODINDO3);
			wattron(tela, COLOR_PAIR(1));
		}
		else
		{
			if (estado == 1)
			{
				switch(tipo)
				{
				case 0:	
					mvwprintw(tela, alienY,alienX, ALIEN_T11_1);
					mvwprintw(tela, alienY+1,alienX, ALIEN_T11_2);
					mvwprintw(tela, alienY+2,alienX, ALIEN_T11_3);
					break;
				case 1:	
					mvwprintw(tela, alienY,alienX, ALIEN_T21_1);
					mvwprintw(tela, alienY+1,alienX, ALIEN_T21_2);
					mvwprintw(tela, alienY+2,alienX, ALIEN_T21_3);
					break;
				case 2:	
					mvwprintw(tela, alienY,alienX, ALIEN_T31_1);
					mvwprintw(tela, alienY+1,alienX, ALIEN_T31_2);
					mvwprintw(tela, alienY+2,alienX, ALIEN_T31_3);
					break;
				}
			}
			else
			{
				switch(tipo)
				{
					case 0:	
						mvwprintw(tela, alienY,alienX, ALIEN_T12_1);
						mvwprintw(tela, alienY+1,alienX, ALIEN_T12_2);
						mvwprintw(tela, alienY+2,alienX, ALIEN_T12_3);
						break;
					case 1:	
						mvwprintw(tela, alienY,alienX, ALIEN_T22_1);
						mvwprintw(tela, alienY+1,alienX, ALIEN_T22_2);
						mvwprintw(tela, alienY+2,alienX, ALIEN_T22_3);
						break;
					case 2:	
						mvwprintw(tela, alienY,alienX, ALIEN_T32_1);
						mvwprintw(tela, alienY+1,alienX, ALIEN_T32_2);
						mvwprintw(tela, alienY+2,alienX, ALIEN_T32_3);
						break;
				}
			}

		}
		incrementa_atualAlien(&(jogo->aliens));
	}
}

void imprime_canhao(WINDOW *tela, t_listaG *canhao)
{
	int canhaoX, canhaoY;
	wattron(tela, COLOR_PAIR(3));

	inicia_atual(canhao);
	dados_atual(&canhaoX, &canhaoY, canhao);
	
	mvwprintw(tela, canhaoY, canhaoX, CANHAO1);
	mvwprintw(tela, canhaoY+1,canhaoX, CANHAO2);
}

void imprime_tiro(WINDOW *tela, t_listaG *tiro)
{
	int tiroX, tiroY;
	int i, quant;
	wattron(tela, COLOR_PAIR(4));
	
	inicia_atual(tiro);
	quantidadeObjetos(&quant, tiro);
	for(i = 0; i < quant; i++)
	{
		dados_atual(&tiroX, &tiroY, tiro);
		incrementa_atual(tiro);
		mvwprintw(tela, tiroY, tiroX, TIRO);
	}
}

void imprime_bomba(WINDOW *tela, t_listaG *bomba)
{
	int bombaX, bombaY;
	int i, quant;
	wattron(tela, COLOR_PAIR(5));
	inicia_atual(bomba);
	quantidadeObjetos(&quant, bomba);
	for(i = 0; i < quant; i++)
	{
		dados_atual(&bombaX, &bombaY, bomba);
		incrementa_atual(bomba);
		mvwprintw(tela, bombaY, bombaX, BOMBA);
	}
}

void imprime_barreira(WINDOW *tela, t_jogo *jg)
{
	int barreiraX, barreiraY;
	int i, quant;
	wattron(tela, COLOR_PAIR(3));
	inicia_atual(&(jg->barreira0));
	quantidadeObjetos(&quant, &(jg->barreira0));
	for(i = 0; i < quant; i++)
	{
		dados_atual(&barreiraX, &barreiraY, &(jg->barreira0));
		incrementa_atual(&(jg->barreira0));
		mvwprintw(tela, barreiraY, barreiraX, BARREIRA);
	}
	inicia_atual(&(jg->barreira1));
	quantidadeObjetos(&quant, &(jg->barreira1));
	for(i = 0; i < quant; i++)
	{
		dados_atual(&barreiraX, &barreiraY, &(jg->barreira1));
		incrementa_atual(&(jg->barreira1));
		mvwprintw(tela, barreiraY, barreiraX, BARREIRA);
	}
	inicia_atual(&(jg->barreira2));
	quantidadeObjetos(&quant, &(jg->barreira2));
	for(i = 0; i < quant; i++)
	{
		dados_atual(&barreiraX, &barreiraY, &(jg->barreira2));
		incrementa_atual(&(jg->barreira2));
		mvwprintw(tela, barreiraY, barreiraX, BARREIRA);
	}
	inicia_atual(&(jg->barreira3));
	quantidadeObjetos(&quant, &(jg->barreira3));
	for(i = 0; i < quant; i++)
	{
		dados_atual(&barreiraX, &barreiraY, &(jg->barreira3));
		incrementa_atual(&(jg->barreira3));
		mvwprintw(tela, barreiraY, barreiraX, BARREIRA);
	}
}

void imprime_naveMae(WINDOW *tela, t_listaG *naveMae)
{
	int posX, posY, i;
	char NAVE_MAE1[10] = "=/-\\--\\";
	char NAVE_MAE2[10] = " |-----|";
	char NAVE_MAE3[10] = "=\\-/--/";
	wattron(tela, COLOR_PAIR(2));
	inicia_atual(naveMae);
	dados_atual(&posX, &posY, naveMae);
	for(i=0; i < LARG_NVMAE	; i++)
	{
		if(((posX+i < LARG_TELA-1) && (posX + i > 1)))
		{
			mvwprintw(tela, posY, posX+i,"%c", NAVE_MAE1[i]);
			mvwprintw(tela, posY+1, posX+i,"%c", NAVE_MAE2[i]);
			mvwprintw(tela, posY+2, posX+i,"%c", NAVE_MAE3[i]);
		}
	}
}

void imprime_pontuacao(t_jogo *jg)
{
	int i;
	move(ALT_TELA, LARG_TELA/2);
	attron(COLOR_PAIR(1));
	printw("SCORE : %d		VIDAS :", jg->pontuacao);
	attron(COLOR_PAIR(2));
	for(i = 0; i < jg->vidas; i++)
	{
		printw(" ");
		 addch(ACS_DIAMOND);
	}
	
}

void tela_perdeu(WINDOW *tela, t_jogo *jg)
{
	wattron(tela, COLOR_PAIR(2));
	mvwprintw(tela, ALT_TELA/2, LARG_TELA/2,"VOCE FOI DERROTADO");
	mvwprintw(tela, (ALT_TELA/2)+1, LARG_TELA/2,"score: %d", jg->pontuacao);
	refresh();
	wrefresh(tela);
}

void tela_venceu(WINDOW *tela)
{
	wattron(tela, COLOR_PAIR(3));
	mvwprintw(tela, ALT_TELA/2, LARG_TELA/2,"PARABENS VOCE VENCEU!!!");
	mvwprintw(tela, (ALT_TELA/2)+1, LARG_TELA/2,"pronto para o proximo nivel?");
	refresh();
	wrefresh(tela);
}

void imprime_tela(WINDOW *tela_jogo, t_jogo *jogo)
{
	/*limpa tela anterior*/
	wclear(tela_jogo);
	clear();
	/*imprime tudo*/
	imprime_area_tela(tela_jogo);
	imprime_aliens(tela_jogo, jogo);
	imprime_canhao(tela_jogo, &(jogo->canhao));
	imprime_tiro(tela_jogo, &(jogo->tiro));
	imprime_bomba(tela_jogo, &(jogo->bomba));
	imprime_barreira(tela_jogo, jogo);
	if(!lista_vazia(&(jogo->naveMae)))
			imprime_naveMae(tela_jogo, &(jogo->naveMae));
	imprime_pontuacao(jogo);
	/*atualiza tela*/
	refresh();
	wrefresh(tela_jogo);
}
