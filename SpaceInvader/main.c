#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
/*a bilioteca de dados eh incluida junto com a de tela*/
#include "lib_tela.h"

#define VENCEU 1
#define PERDEU 2
#define CONTINUA 0

#define LIMITE_TIROS 5

void movimenta_aliens(t_jogo *jg);
void movimenta_tiro(t_listaG *tiro);	
void movimenta_bomba(t_listaG *bomba);
void movimenta_naveMae(t_listaG *naveMae);

int colisao_Objetos(t_listaG *l1, t_listaG *l2, int larg_2, int comp_2);
int colisao_Canhao(t_jogo *jg);
int colisao_alien(t_jogo *jogo);
void colisao_barreira(t_jogo *jg);
void colisao_alienBarreira(t_jogo *jg);

void atira_alien(t_jogo *jg);
void atira_canhao(t_jogo *jg);

int acao_jogador(WINDOW *tela, t_jogo *jg);

int gera_aleatorio(int limiar);

void fim_rodada(WINDOW *tela, t_jogo *jg, int op);

int verifica_fim_jogo(t_jogo *jg);
void recomeca(t_jogo *jg, WINDOW **tela);

int main()
{
	t_jogo jogo;
	WINDOW *tela_jogo;
	srand(time(NULL));
    iniciaDados(&jogo);
    inicia_pontos_vida(&jogo);

	if (!inicia_tela(&tela_jogo))
	{
		delwin(tela_jogo);
		endwin();
		printf("A tela deve ter no minimo 38 x 100\n");
		return 0;
	}

	long int ciclo = 1;
	int situacao_jogo;
	while(1)
	{
		ciclo++;
		/*retira aliens que estaao explodindo*/
		if((ciclo % 5) == 0)
			limpa_Alien(&jogo);
		/*VERIFICA FIM DE JOGO*/
		situacao_jogo = verifica_fim_jogo(&jogo);
		if (situacao_jogo == VENCEU)
		{
			fim_rodada(tela_jogo, &jogo, 2);
			recomeca(&jogo, &tela_jogo);
			ciclo = 0;
		}
		else if (situacao_jogo == PERDEU)
		{
			fim_rodada(tela_jogo, &jogo, 1);
			break;
		}

		/*VERIFICA COLISOES*/
		colisao_alien(&jogo);
		colisao_Objetos(&(jogo.tiro), &(jogo.bomba), 1, 1);
		colisao_barreira(&jogo);
		colisao_alienBarreira(&jogo);
		if(! lista_vazia(&(jogo.naveMae)))
			if(colisao_Objetos(&(jogo.tiro), &(jogo.naveMae), LARG_NVMAE, ALT_NVMAE))
				incrementa_pontuacao(&jogo, 25);
		
		/*MOVIMENTA ELELEMTOS*/
		if((ciclo % velocidade_Alien(&(jogo.aliens))) == 0)
			movimenta_aliens(&jogo);
		if((ciclo % velocidade_Objeto(&(jogo.tiro))) == 0) 
			movimenta_tiro(&(jogo.tiro));
		if((ciclo % velocidade_Objeto(&(jogo.tiro))) == 0) 
			movimenta_bomba(&(jogo.bomba));
		if((! lista_vazia(&(jogo.naveMae))) 
			&& ((ciclo % velocidade_Objeto(&(jogo.naveMae))) == 0)) 
				movimenta_naveMae(&(jogo.naveMae));
						
		/*FAZ A ACAO Q O JOGADOR ESCOLHER*/
		/*se o jogador escolher sair do jogo retorna 0*/
		if(! acao_jogador(tela_jogo, &jogo))
		{
			/*se o usuario encerrar apartida*/
			fim_rodada(tela_jogo, &jogo, 1);
			break;
		}
						
		
		/*ELEMENTOS GERADOS ALEATORIAMENTE*/
		if(gera_aleatorio(50))
			atira_alien(&jogo);
		if((gera_aleatorio(5)) && (lista_vazia(&(jogo.naveMae))))
			insereObjeto(POS_INI_NVMAE_X, POS_INI_NVMAE_Y, &(jogo.naveMae));
		
		/*desenha tela atual*/
		imprime_tela(tela_jogo, &jogo);

		/*delay*/
		usleep(35000);		
	}
	return 0;
}

void fim_rodada(WINDOW *tela, t_jogo *jg, int op)
{
	wclear(tela);
	clear();
	switch(op)
	{
		case 1:
			tela_perdeu(tela, jg);
			break;
		case 2:
			tela_venceu(tela);
			break;
	}
	usleep(5000000);
	delwin(tela);
	endwin();
}

int verifica_fim_jogo(t_jogo *jg)
{
	/*se nao existe mais aliens*/
	int quant;
	quantidadeAliens(&quant, &(jg->aliens));
	if(quant == 0)
		return VENCEU;
	/*se o canhao foi atingido por uma bomba*/
	if(colisao_Canhao(jg))
	{
		usleep(500000);
		if(decrementa_vida(jg))
			return CONTINUA;
		else
			return PERDEU;
	}
	/*se os aliens aterrissaram*/
	int altAlien;
	alturaInferiorAlien(&altAlien, jg);
	if(altAlien >= POS_INI_CANHAO_Y)
		return PERDEU;
	return CONTINUA;
}

void recomeca(t_jogo *jg, WINDOW **tela)
{
	int velocidade;
	velocidade = velocidade_Alien(&(jg->aliens));
	apaga_jogo(jg); 
	iniciaDados(jg);
	/*aumenta a velocidade, exceto se chegou no limite*/
	if(velocidade > 2)
		altera_velocidade(velocidade-2, &(jg->aliens));
	/*reinicia tela*/
	delwin(*tela);
	inicia_tela(tela);
}

void atira_canhao(t_jogo *jg)
{
	int posX, posY;
	int quant;
	quantidadeObjetos(&quant, &(jg->tiro));
	inicia_atual(&(jg->canhao));
	/*pega a posicao do canhao e adiciona um tiro na posicao y+1*/
	dados_atual(&posX, &posY, &(jg->canhao));
	/*se houver tiros disponiveis
	* e se nao houver um tiro nessa posicao*/
	if((quant < LIMITE_TIROS) &&  
		(busca_objeto(posX+2, posY-1, &(jg->tiro)) == 0))
			insereObjeto(posX+2, posY-1, &(jg->tiro));	
}

void atira_alien(t_jogo *jg)
{
	int posX, posY;
	int quant, ind, i;
	quantidadeAliens(&quant, &(jg->aliens));
	ind = rand() % quant;
	inicia_atualAlien(&(jg->aliens));
	for(i = 0; i < ind; i++)
		incrementa_atualAlien(&(jg->aliens));
	dados_atualAlien(&posX, &posY, &(jg->aliens));
	insereObjeto(posX+2, posY+3, &(jg->bomba));
}

int gera_aleatorio(int limiar)
{
	int prob_objeto;
	prob_objeto = rand()%1000;
	return prob_objeto < limiar;
}

void movimenta_tiro(t_listaG *tiro)
{
	int posX, posY, quant, i;
	inicia_atual(tiro);
	quantidadeObjetos(&quant, tiro);
	inicia_atual(tiro);
	for(i = 0; i<quant; i++)
	{
		dados_atual(&posX, &posY, tiro);
		if(posY <= 1)
			apagaObjeto(posX, posY, tiro);
		else
			altera_atual(posX, posY-1, tiro);
		incrementa_atual(tiro);	
	}

}

void movimenta_bomba(t_listaG *bomba)
{
	/*se saiu da tela apaga*/
	int posX, posY, quant, i;
	inicia_atual(bomba);
	quantidadeObjetos(&quant, bomba);
	for(i = 0; i<quant; i++)
	{
		dados_atual(&posX, &posY, bomba);
		if(posY >= ALT_TELA-2)
			apagaObjeto(posX, posY, bomba);
		else
			altera_atual(posX, posY+1, bomba);
		incrementa_atual(bomba);	
	}
}

void movimenta_naveMae(t_listaG *naveMae)
{
	int posX, posY;
	altera_posCanhao(naveMae, 1);
	/*SE SAIU DA TELA APAGA*/
	inicia_atual(naveMae);
	dados_atual(&posX, &posY, naveMae);
	if(posX >= LARG_TELA + 8)
		apagaObjeto(posX, posY, naveMae);
}

int acao_jogador(WINDOW *tela, t_jogo *jg)
{
	int tecla, posX, posY;
	inicia_atual(&(jg->canhao));
	dados_atual(&posX, &posY, &(jg->canhao));
	tecla = wgetch(tela);
	switch(tecla)
	{
		case KEY_RIGHT:
			/*se nao esta no limite da tela*/
			if(posX+5 < LARG_TELA - 1)
				altera_posCanhao(&(jg->canhao), 1);
			break;
		case KEY_LEFT:
			/*se nao esta no limite da tela*/
			if(posX > 1)
				altera_posCanhao(&(jg->canhao), 0);
			break;
		case 32:
			atira_canhao(jg);
			break;
		case 27:
			return 0;
			break;
	}
	return 1;
}

void movimenta_aliens(t_jogo *jg)
{
	int esquerda, direita;
	/*verifica se atingiu lateral da tela*/
	/*se atingiu a funcao chamada abaixa os alien e muda a direcao*/
	extremosAlien(&esquerda, &direita, jg);
	if(((esquerda <= 1)&&(direcao_alien(&(jg->aliens)) == 1)) || (direita >= LARG_TELA-1))
	{
		altera_posAlien(&(jg->aliens), 3);	
		/*aumenta a velocidade dos aliens*/
		int velocidade;
		velocidade = velocidade_Alien(&(jg->aliens));
		if(velocidade > 1)
			altera_velocidade(velocidade-1, &(jg->aliens));
	}

	if(direcao_alien(&(jg->aliens)) == 0)
		altera_posAlien(&(jg->aliens), 0);
	else
		altera_posAlien(&(jg->aliens), 1);
	/*inverte para ir para o proximo sprite*/
	inverte_estadoAlien(&(jg->aliens));
}

int colisao_Canhao(t_jogo *jg)
{
	int bomba_posX, bomba_posY, quantBomba, canhao_posX, canhao_posY, i;

	/*pega dados do canhao*/
	inicia_atual(&(jg->canhao));
	dados_atual(&canhao_posX, &canhao_posY, &(jg->canhao));
	/*pega dados das bombas*/
	quantidadeObjetos(&quantBomba, &(jg->bomba));	
	inicia_atual(&(jg->bomba));
	/*compara a posicao de todas as bombas com a posicao do canhao*/
	for (i = 0; i < quantBomba; i++)
	{
		dados_atual(&bomba_posX, &bomba_posY, &(jg->bomba));
		incrementa_atual(&(jg->bomba));
		if((bomba_posX >= canhao_posX) && (bomba_posX <= canhao_posX + 4))
		{
			if((bomba_posY >= canhao_posY) && (bomba_posY <= canhao_posY + 2))
			{
				apagaObjeto(bomba_posX, bomba_posY, &(jg->bomba));
				return 1;	
			}
		}
	}
	return 0;	
}

void colisao_barreira(t_jogo *jg)
{
	/*colisao das barreiras com tiro*/
	colisao_Objetos(&(jg->tiro), &(jg->barreira0), 1, 1);
	colisao_Objetos(&(jg->tiro), &(jg->barreira1), 1, 1);
	colisao_Objetos(&(jg->tiro), &(jg->barreira2), 1, 1);
	colisao_Objetos(&(jg->tiro), &(jg->barreira3), 1, 1);

	/*colisao das barreiras com bomba*/
	colisao_Objetos(&(jg->bomba), &(jg->barreira0), 1, 1);
	colisao_Objetos(&(jg->bomba), &(jg->barreira1), 1, 1);
	colisao_Objetos(&(jg->bomba), &(jg->barreira2), 1, 1);
	colisao_Objetos(&(jg->bomba), &(jg->barreira3), 1, 1);
}

int colisao_alien( t_jogo *jogo)
{
	int tirosY, tirosX, alienY, alienX, tipo, estado;
	int quant, quantAlien, altAlien, i, j;

	/*recebe um vetor com as posicoes dos tiros no
	 *eixo Y, X e a quantidade de tiros na tela*/
	quantidadeObjetos(&quant, &(jogo->tiro));
	quantidadeAliens(&quantAlien, &(jogo->aliens));

	inicia_atual(&(jogo->tiro));
	
	/*recebe a altura do alien na parte mais inferior  da tela*/
	alturaInferiorAlien(&altAlien, jogo);
	for (i = 0; i < quant; i++)
	{
		dados_atual(&tirosX, &tirosY, &(jogo->tiro));
		incrementa_atual(&(jogo->tiro));
		/*se algum tiro esta em uma altura igual ou superior
		* ao ultimo alien, ele eh um possivel candidato */
		if(tirosY <= altAlien)
		{
			/*percorre a lista de aliens, verificando
			* se algum esta dentro do alcance do tiro*/
			j = 0;
			inicia_atualAlien(&(jogo->aliens));
			while (j < quantAlien)
			{
				dados_atualAlien(&alienX, &alienY, &(jogo->aliens));
				/*verifica se o alien nao esta explodindo*/
				outros_dadosAlien(&estado, &tipo, &(jogo->aliens));
				incrementa_atualAlien(&(jogo->aliens));
				/*verifica se o alien esta dentro do alcance do tiro
				* com relacao ao eixo  Y e depois eixo X*/
				if((estado == 1) || (estado == -1)){
					if((tirosY >= alienY) && ( tirosY < (alienY + COMP_ALIEN))){
						if ((tirosX >= alienX) && ( tirosX < (alienX + LARG_ALIEN))){		
							apagaObjeto(tirosX, tirosY, &(jogo->tiro));
							incrementa_pontuacao(jogo, 10);
							altera_estadoAlien(alienX, alienY, jogo);
							break;	
						}	
					}
				}
				j++;
			}
		}
	}
	return 1;
}

int colisao_Objetos(t_listaG *l1, t_listaG *l2, int larg_2, int alt_2)
{
	int l1_posY, l1_posX, l2_posY, l2_posX;
	int l1_quant, l2_quant, i;
	int colisoes = 0;
	
	quantidadeObjetos(&l1_quant, l1);
		
	inicia_atual(l1);
	/*compara, se ocorrer uma colisao apaga ambos os objetos*/
	/*compara todas as posicoes dos objetos da lista l1 com todos da lista l2*/
	for(i = 0; i<l1_quant; i++)
	{
		/*incrementa o atual para nao ahver problemas quando remover*/
		dados_atual(&l1_posX, &l1_posY, l1);
		incrementa_atual(l1);
		/*coloca o atual no inicio do vetor*/
		inicia_atual(l2);
		quantidadeObjetos(&l2_quant, l2);
		while(l2_quant > 0)
		{
			dados_atual(&l2_posX, &l2_posY, l2);
			incrementa_atual(l2);
			/*compara com o anterior para verificar se nao se cruzaram*/
			if (((l1_posY >= l2_posY) && (l1_posY <= l2_posY + alt_2 - 1))
				||((l1_posY-1 >= l2_posY) && (l1_posY-1 <= l2_posY + alt_2 - 1)))
			{ 
				if ((l1_posX >= l2_posX) && (l1_posX <= l2_posX + larg_2 -1))
				{
					apagaObjeto(l1_posX, l1_posY, l1);
					apagaObjeto(l2_posX, l2_posY, l2);
					colisoes++;
					break;
				}
			}
			l2_quant--;
		}
	}
	/*retorna se houve colisao*/
	return colisoes != 0;
}

void colisao_alienBarreira(t_jogo *jg)
{
	int altAlien;
	alturaInferiorAlien(&altAlien, jg);
	if(altAlien+COMP_ALIEN < LINHA_BARREIRA)
		return;
	int alien_posX, alien_posY, i, alien_quant;

	inicia_atualAlien(&(jg->aliens));
	inicia_atual(&(jg->barreira0));
	inicia_atual(&(jg->barreira1));
	inicia_atual(&(jg->barreira2));
	inicia_atual(&(jg->barreira3));
	quantidadeAliens(&alien_quant, &(jg->aliens));
	dados_atualAlien(&alien_posX, &alien_posY, &(jg->aliens));
	/*percorre a lista ate chegar no alien que esta na altura da barreira*/
	while((alien_quant > 0)&&(alien_posY+COMP_ALIEN < LINHA_BARREIRA))
	{
		incrementa_atualAlien(&(jg->aliens));
		dados_atualAlien(&alien_posX, &alien_posY, &(jg->aliens));
		alien_quant--;
	}

	/*passa a posicao dos pedacos inferiores do alien*/
	for(i=0; i < LARG_ALIEN; i++){	
		/*se a posicao do alien passada estiver alguma barreira ele apaga
		* caso contrario ele retorna zero*/
		apagaObjeto(alien_posX +i, alien_posY + COMP_ALIEN - 1, &(jg->barreira0));
		apagaObjeto(alien_posX +i, alien_posY + COMP_ALIEN - 1, &(jg->barreira1));
		apagaObjeto(alien_posX +i, alien_posY + COMP_ALIEN - 1, &(jg->barreira2));
		apagaObjeto(alien_posX +i, alien_posY + COMP_ALIEN - 1, &(jg->barreira3));	
	}	
	/*verifica as barreiras*/
	while(incrementa_atualAlien(&(jg->aliens)))
	{
		for(i=0; i < LARG_ALIEN; i++)
		{		
			dados_atualAlien(&alien_posX, &alien_posY, &(jg->aliens));
			apagaObjeto(alien_posX +i, alien_posY + COMP_ALIEN - 1, &(jg->barreira0));
			apagaObjeto(alien_posX +i, alien_posY + COMP_ALIEN - 1, &(jg->barreira1));
			apagaObjeto(alien_posX +i, alien_posY + COMP_ALIEN - 1, &(jg->barreira2));
			apagaObjeto(alien_posX +i, alien_posY + COMP_ALIEN - 1, &(jg->barreira3));	
		}
	}
}