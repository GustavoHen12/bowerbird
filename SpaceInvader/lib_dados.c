#include <stdio.h>
#include<stdlib.h>

#include"lib_dados.h"

/*-------------LISTA------------*/
int lista_vazia(t_listaG *l)
{
  return l->quant == 0;
}

int inicializa_listaGeral (t_listaG *l, int vel)
{
  t_nodo *s1, *s2;
  s1 = (t_nodo*) malloc (sizeof(t_nodo));
  s2 = (t_nodo*) malloc (sizeof(t_nodo));
  if((s1 == NULL) || (s2==NULL))
    return 0;
  /*faz a ligacao entre os sentinelas*/
  s1->prox = s2;
  s1->prev = NULL;
  s2->prox = NULL;
  s2->prev = s1;
  s1->posX = 0;
  s1->posY = 0;
  s2->posX = 0;
  s2->posY = 0;
  /*inicia a lista*/
  l->ini = s1;
  l->fim = s2;
  l->atual = NULL;
  l->quant = 0;
  l->velocidade = vel;
  return 1;
}

int inicializa_listaAlien (t_alien *l, int vel)
{
  nodoAlien_t *s1, *s2;
  s1 = (nodoAlien_t*) malloc (sizeof(nodoAlien_t));
  s2 = (nodoAlien_t*) malloc (sizeof(nodoAlien_t));
  if((s1 == NULL) || (s2==NULL))
    return 0;
  /*faz a ligacao entre os sentinelas*/
  s1->prox = s2;
  s1->prev = NULL;
  s2->prox = NULL;
  s2->prev = s1;
  s1->posX = 0;
  s1->posY = 0;
  s2->posX = 0;
  s2->posY = 0;
  /*inicia a lista*/
  l->ini = s1;
  l->fim = s2;
  l->atual = NULL;
  l->quant = 0;
  l->velocidade = vel;
  l->direcao = 0;
  return 1;
}

int inicia_atual(t_listaG *l)
{
  if (l->quant == 0)
    return 0;
  l->atual = l->ini->prox;
  return 1;
}

int inicia_atualAlien(t_alien *l)
{
  if (l->quant == 0)
    return 0;
  l->atual = l->ini->prox;
  return 1;
}

/*
  Insere o elemento item no final da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int insereObjeto(int posX, int posY,  t_listaG *l)
{
  t_nodo *new;
  new = (t_nodo *) malloc(sizeof(t_nodo));
  if (new == NULL)
    return 0;

  /*insere no fim da lista*/
  new->posX = posX;
  new->posY = posY;
  new->prox = l->fim;
  new->prev = l->fim->prev;

  l->fim->prev->prox = new;
  l->fim->prev = new;
  l->quant = l->quant + 1;

  return 1;
}

int insereAlien(int posX, int posY, int tipo,  t_jogo *jg)
{
  nodoAlien_t *new;
  new = (nodoAlien_t *) malloc(sizeof(nodoAlien_t));
  if (new == NULL)
    return 0;
  t_alien *l;
  l = &(jg->aliens);

  new->posX = posX;
  new->posY = posY;
  new->tipo = tipo;
  new->estado = 1;

  new->prox = l->fim;
  new->prev = l->fim->prev;

  l->fim->prev->prox = new;
  l->fim->prev = new;
  l->quant++;

  return 1;
}

int apagaObjeto(int posX, int posY, t_listaG *l)
{
  t_nodo *p;
  /*coloca chave nas sentinelas*/
  l->ini->posX = posX;
  l->ini->posY = posY;
  l->fim->posX = posX;
  l->fim->posY = posY;

  /*percorre a lista a partir do primeiro elemento a partir da sentinela*/
  p = l->ini->prox;
  while((p->posX != posX) || (p->posY != posY))
    p = p->prox;
  /*verifica se o elemento encontrado eh as sentinelas*/
  if((p->prox == NULL) || (p->prev == NULL))
    return 0;
  /*retira elemento da lista*/
  p->prev->prox = p->prox;
  p->prox->prev = p->prev;
  free(p);
  l->quant--;
  return 1;
}

int apagaAlien(int posX, int posY, t_jogo *jg)
{
  t_alien *l;
  l = &(jg->aliens);

  nodoAlien_t *p;
  /*coloca chave nas sentinelas*/
  l->ini->posX = posX;
  l->ini->posY = posY;
  l->fim->posX = posX;
  l->fim->posY = posY;
  /*percorre a lista a partir do primeiro elemento a partir da sentinela*/
  p = l->ini->prox;
  while((p->posX != posX) || (p->posY != posY))
    p = p->prox;
  /*verifica se o elemento encontrado eh as sentinelas*/
  if((p->prox == NULL) || (p->prev == NULL))
    return 0;
  /*retira elemento da lista*/
  p->prev->prox = p->prox;
  p->prox->prev = p->prev;
  free(p);
  l->quant--;
  return 1;
}

int busca_objeto(int posX, int posY, t_listaG *l)
{
  t_nodo *p;
  /*coloca chave nas sentinelas*/
  l->ini->posX = posX;
  l->ini->posY = posY;
  l->fim->posX = posX;
  l->fim->posY = posY;

  p = l->ini->prox;
  while((p->posX != posX) || (p->posY != posY))
    p = p->prox;
  /*verifica se o elemento encontrado eh as sentinelas*/
  if((p->prox == NULL) || (p->prev == NULL))
    return 0;
  /*se nao eh as sentinelas retorna 1*/
  return 1;
}

void dados_atual(int *posX, int *posY, t_listaG *l)
{
  if(l->atual->prox == NULL)
    return;
  *posX = l->atual->posX;
  *posY = l->atual->posY;
}

void dados_atualAlien(int *posX, int *posY, t_alien *l)
{
  if(l->atual->prox == NULL)
    return;
  *posX = l->atual->posX;
  *posY = l->atual->posY;
}

void outros_dadosAlien(int *estado, int *tipo, t_alien *l)
{
  if(l->atual->prox == NULL)
    return;
  *estado = l->atual->estado;
  *tipo = l->atual->tipo;
}

int velocidade_Alien(t_alien *aliens)
{
  return aliens->velocidade;
}

int velocidade_Objeto(t_listaG *l)
{
  return l->velocidade;
}

void quantidadeObjetos(int *quant, t_listaG *l)
{
  *quant = l->quant;
}

void quantidadeAliens(int *quant, t_alien *l)
{
  *quant = l->quant;
}

int direcao_alien(t_alien *aliens)
{
  return aliens->direcao;
}

void altera_atual(int posX, int posY, t_listaG *l)
{

  l->atual->posX = posX;
  l->atual->posY = posY;
}

int altera_estadoAlien(int posX, int posY, t_jogo *jg)
{
  t_alien *l;
  l = &(jg->aliens);

  nodoAlien_t *p;
  /*coloca chave nas sentinelas*/
  l->ini->posX = posX;
  l->ini->posY = posY;
  l->fim->posX = posX;
  l->fim->posY = posY;
  /*percorre a lista a partir do primeiro elemento a partir da sentinela*/
  p = l->ini->prox;
  while((p->posX != posX) || (p->posY != posY))
    p = p->prox;
  /*verifica se o elemento encontrado eh as sentinelas*/
  if((p->prox == NULL) || (p->prev == NULL))
    return 0;

  p->estado = 2;
  return 1;
}

void altera_velocidade(int velocidade, t_alien *aliens)
{
  aliens->velocidade = velocidade;
}

void incrementa_atual(t_listaG *l)
{
  if(l->atual->prox == NULL)
    return;
  l->atual = l->atual->prox;
}

int incrementa_atualAlien(t_alien *l)
{
  if(l->atual->prox == NULL)
    return 0;
  l->atual = l->atual->prox;
  return 1;
}

void destroi_lista(t_listaG *l)
{
  t_nodo *p, *r;
  p=l->ini;
  while(p->prox != NULL)
  {
    r = p;
    p = p->prox;
    free(r);
  }
  free(p);
  l->ini = NULL;
  l->fim = NULL;
  l->atual = NULL;
  l->quant = 0;
}

/*-------------OUTRAS------------*//*-------------OUTRAS------------*//*-------------OUTRAS------------*//*-------------OUTRAS------------*/

int iniciaDados(t_jogo *jg)
{

  /*inicializa as listas*/
  if(! inicializa_listaGeral(&(jg->tiro), VELOCIDADE_TIRO)) return 0;
  if(! inicializa_listaGeral(&(jg->bomba), VELOCIDADE_BOMBA)) return 0;
  if(! inicializa_listaAlien(&(jg->aliens), VELOCIDADE_ALIENS)) return 0;
  if(! inicializa_listaGeral(&(jg->naveMae), VELOCIDADE_NVMAE)) return 0;
  if(! inicializa_listaGeral(&(jg->canhao), VELOCIDADE_CANHAO)) return 0;
  int i, j, k,posX, posY;

  /*cria canhao*/
  insereObjeto(POS_INI_CANHAO_X, POS_INI_CANHAO_Y, &(jg->canhao));

  /*cria nave mae*/
  insereObjeto(POS_INI_NVMAE_X, POS_INI_NVMAE_Y, &(jg->naveMae));

  /*inicializa a barreira*/
  if(! inicializa_listaGeral(&(jg->barreira0), 0)) return 0;
  if(! inicializa_listaGeral(&(jg->barreira1), 0)) return 0;
  if(! inicializa_listaGeral(&(jg->barreira2), 0)) return 0;
  if(! inicializa_listaGeral(&(jg->barreira3), 0)) return 0;
  posY = LINHA_BARREIRA;
  for(j = 0; j < LARG_BARREIRA; j++)
  {
    for(k = 0; k < ALT_BARREIRA; k++)
    {
      /*constroi o bloco da barreira*/
      posX = MARGEM_BARREIRA;
      insereObjeto( posX+j , posY+k , &(jg->barreira0));
      posX = MARGEM_BARREIRA + DIST_BARREIRA + LARG_BARREIRA;
      insereObjeto( posX+j , posY+k , &(jg->barreira1));
      posX = MARGEM_BARREIRA + 2*(DIST_BARREIRA + LARG_BARREIRA);
      insereObjeto( posX+j , posY+k , &(jg->barreira2));
      posX = MARGEM_BARREIRA + 3*(DIST_BARREIRA + LARG_BARREIRA);
      insereObjeto( posX+j , posY+k , &(jg->barreira3));
    }
  }

  /*configura aliens*/
  for (i = 0; i < QUANT_ALIENS_Y; i++)
  {
    for(j = 0; j < QUANT_ALIENS_X; j++)
    {
        /*calcula a posicao do alien*/
        posX = j*(LARG_ALIEN+DIST_ALIEN)+POS_INI_ALIENSX;
        posY = i*(COMP_ALIEN+DIST_ALIEN)+POS_INI_ALIENSY;
        /*insere o alien na lista de aliens
         *todas as outras configuracoes do alien(tipo, estado), sera feita na funcao*/
        if(i==0)
          insereAlien(posX, posY, 0, jg);
        else if((i>0) &&(i<3))
          insereAlien(posX, posY, 1, jg);
        else if((i>=3) &&(i<QUANT_ALIENS_Y))
          insereAlien(posX, posY, 2, jg);
    }
  }


  return 1;
}

void inicia_pontos_vida(t_jogo *jogo)
{
  jogo->pontuacao = 0;
  jogo->vidas = 3;
}

void limpa_Alien(t_jogo *jg)
{
  t_alien*l;
  l = &(jg->aliens);

  nodoAlien_t *p;
  p = l->ini->prox;
  while(p->prox != NULL)
  {
    if(p->estado == 2)
      apagaAlien(p->posX, p->posY, jg);
    p = p->prox;
  }
}

int apaga_jogo(t_jogo *jg)
{
  destroi_lista(&(jg->barreira3));
  destroi_lista(&(jg->barreira2));
  destroi_lista(&(jg->barreira1));
  destroi_lista(&(jg->barreira0));
  destroi_lista(&(jg->naveMae));
  destroi_lista(&(jg->canhao));
  destroi_lista(&(jg->tiro));
  destroi_lista(&(jg->bomba));
  return 1;
}

void extremosAlien(int *esquerda, int *direitas, t_jogo *jg)
{
  t_alien*l;
  l = &(jg->aliens);

  int esq, dir;
  nodoAlien_t *p;
  p = l->ini->prox;
  esq = l->ini->prox->posX;
  dir = l->ini->prox->posX;

  while(p->prox != NULL)
  {
    if(p->posX > dir)
      dir = p->posX;
    if(p->posX < esq)
      esq = p->posX;
    p = p->prox;
  }
  *esquerda = esq;
  *direitas = dir + LARG_ALIEN;
}

void alturaInferiorAlien(int *altAlien, t_jogo *jg)
{
  t_alien*l;
  l = &(jg->aliens);

  int menor;
  nodoAlien_t *p;
  p = l->ini->prox;
  menor = l->ini->prox->posY;

  while(p->prox != NULL)
  {
    if(p->posY > menor)
      menor = p->posY;
    p = p->prox;
  }
  *altAlien = menor+COMP_ALIEN - 1;
}

void altera_posCanhao(t_listaG *canhao, int dir)
{
  t_nodo *p;
  p = canhao->ini->prox;
  if(dir == 1)
    p->posX = p->posX+1;
  else
    p->posX = p->posX-1;
}

void altera_posAlien(t_alien *aliens, int dir)
{
  nodoAlien_t *p;
  p = aliens->ini->prox;
  while(p->prox != NULL)
  {
    if(dir == 3)
      p->posY = p->posY+1;
    else if (dir == 1)
      p->posX = p->posX-1;
    else if (dir == 0)
      p->posX = p->posX+1;     
    p = p->prox;
  }
  if (dir == 3)
  {
    if(aliens->direcao == 0)
      aliens->direcao = 1;
    else
      aliens->direcao = 0;
  }
  
}

void inverte_estadoAlien(t_alien *l)
{
  nodoAlien_t *p;
  p = l->ini->prox;

  while(p->prox != NULL)
  {
    if(p->estado != 2)
      p->estado = p->estado * -1;
    p = p->prox;
  }
}


void incrementa_pontuacao(t_jogo *jg, int pont)
{
  jg->pontuacao = jg->pontuacao + pont;
}

int decrementa_vida(t_jogo *jg)
{
  jg->vidas = jg->vidas - 1;
  if (jg->vidas > 0)
    return 1;
  return 0;
}