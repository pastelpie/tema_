#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// vecini
int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

typedef char Data;

struct Elem
{
  int x, y;   // coordonatele celulei
  Data stare; // starea celulei
  struct Elem *next, *prev;
};
typedef struct Elem Node;

struct Ntree
{
  Node *modificari;
  struct Ntree *st, *dr;
  char **matr;//matricea dupa aplicarea modificarilor
};
typedef struct Ntree tree;

tree *nod_nou(Node *modif, char **matr)
{
  tree *aux = malloc(sizeof(tree));
  aux->modificari = modif;
  aux->matr = matr;
  aux->st = NULL;
  aux->dr = NULL;
  return aux;
}

char **aloc_matr(int N, int M)
{
  char **m = malloc(N * sizeof(char *));
  for (int i = 0; i < N; i++)
  {
    m[i] = malloc((M + 1) * sizeof(char));
    //m[i][M] = '\0';
  }
  return m;
}

void freelist(Node *head) // elibereaza mem alocata pt lista
{
  
  while (head)
  {
    Node *aux = head;
    head = head->next;
    free(aux);
  }
}

//add la finalul listei un nod cu coord x si y
Node *add_modif(Node *head, int x, int y)
{
  Node *nou = malloc(sizeof(Node));
  nou->x = x;
  nou->y = y;
  nou->next = NULL;
  if (!head)
    return nou;

  Node *p = head;
  while (p->next)
    p = p->next;
  p->next = nou;
  return head;
}

//add la finalul listei dublu inlantuite un nod complet
void add_sfarsit(Node **head, int x, int y, Data stare)
{
  Node *nou = malloc(sizeof(Node));
  nou->x = x;
  nou->y = y;
  nou->stare = stare;
  nou->next = NULL;
  nou->prev = NULL;
  if (!*head)
  {
    *head = nou;
    return;
  }
  Node *aux = *head; // parcurge lista pana la ultimjul nod
  while (aux->next)
    aux = aux->next;
  aux->next = nou;
  nou->prev = aux;
}

int nrvecinivii(char **m, int N, int M, int x, int y)
{
  int ct = 0;

  for (int i = 0; i < 8; i++)
  {
    int nx = x + dx[i];
    int ny = y + dy[i];
    if (nx >= 0 && nx < N && ny >= 0 && ny < M && m[nx][ny] == 'X')
      ct++;
  }
  return ct;
}


// copiaza matr sursa(I) in destinatie(F)
void copiazamatr(char **F, char **I, int N)
{
  for (int i = 0; i < N; i++)
    strcpy(F[i], I[i]);
}

void free_matr(char **m, int N)
{
  for (int i = 0; i < N; i++)
    free(m[i]);
  free(m);
}

void free_tree(tree *nod,int N)
{
  if (!nod)
    return;
  freelist(nod->modificari);
  free_matr(nod->matr,N);
  free_tree(nod->st,N);
  free_tree(nod->dr,N);
  free(nod);
}


//aplica modif pe matr 
void changes(char **m, Node *mod)
{
  for (Node *p = mod; p; p = p->next)
    m[p->x][p->y] = p->stare;
}


//determina modif dintre o gen si urm dupa regula 
void calc_changes(char **m, int N, int M, int rule, Node **rez)
{
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < M; j++)
    {
      int vii = nrvecinivii(m, N, M, i, j);
      char aux;
      if (rule == 0)
      {
        if (vii == 2)
          aux = 'X';
        else
          aux = '+';
      }
      else if (m[i][j] == 'X')
      {
        if (vii == 2 || vii == 3)
          aux = 'X';
        else
          aux = '+';
      }
      else
      {
        if (vii == 3)
          aux = 'X';
        else
          aux = '+';
      }

      if (m[i][j] != aux)
        add_sfarsit(rez, i, j, aux);
    }
  }
}


//task3 arb de generatii
void construieste_arbore(tree *nod, int N, int M, int adancime, int K)
{
  if (adancime == K)
    return;
  // regula B(fiul stang)
  Node *fb = NULL;
  calc_changes(nod->matr, N, M, 0, &fb);
  char **auxb = aloc_matr(N, M);
  copiazamatr(auxb, nod->matr, N);
  changes(auxb, fb);
  nod->st = nod_nou(fb, auxb);
  construieste_arbore(nod->st, N, M, adancime + 1, K);

  // regula standard(fiul drept)
  Node *fs = NULL;
  calc_changes(nod->matr, N, M, 1, &fs);
  char **auxs = aloc_matr(N, M);
  copiazamatr(auxs, nod->matr, N);
  changes(auxs, fs);
  nod->dr = nod_nou(fs, auxs);
  construieste_arbore(nod->dr, N, M, adancime + 1, K);
}


//parcurge preordine si afis toate matr din arbore 
void afisare_final(FILE *fout, tree *nod, int N)
{
  if (!nod)
    return;
  for (int i = 0; i < N; i++)
    fprintf(fout, "%s\n", nod->matr[i]);
  fprintf(fout, "\n");
  afisare_final(fout, nod->st, N);
  afisare_final(fout, nod->dr, N);
}


//bonus task2 pdf
void t2bonus(FILE *fout, char **genk, Node **stiva, int K)
{
  for (int i = K - 1; i >= 0; i--)
  {
    Node *c = stiva[i]; // elem curent parcurs
    while (c)
    {
      int a = c->x;
      int b = c->y;
      if (genk[a][b] == 'X')
        genk[a][b] = '+';
      else
        genk[a][b] = 'X';
      c = c->next;
    }
  }
  for (int i = 0; genk[i]; i++)
    fprintf(fout, "%s\n", genk[i]);
}

int main(int argc,const char *argv[])
{

  FILE *fin = fopen(argv[1], "rt");
  FILE *fout = fopen(argv[2], "wt");

  int task, N, M, K;
  int vii;
  fscanf(fin, "%d", &task);
  fscanf(fin, "%d %d", &N, &M);
  fscanf(fin, "%d", &K);

  char **mi = aloc_matr(N, M); // matricea initiala
  char **mf = aloc_matr(N, M); // matricea finala
  char **matrinit = aloc_matr(N, M);//copie pt t3

  for (int i = 0; i < N; i++)
    fscanf(fin, "%s", mi[i]); // citeste in stare initiala

  for (int i = 0; i < N; i++)
    strcpy(matrinit[i], mi[i]);//copiaza starea initiala pt arbore

  if (task == 1)
  {

    for (int i = 0; i < N; i++)
      fprintf(fout, "%s\n", mi[i]);

    if (K > 0)
      fprintf(fout, "\n");

    for (int k = 0; k < K; k++)
    {
      for (int i = 0; i < N; i++)
      {
        for (int j = 0; j < M; j++)
        {
          vii = nrvecinivii(mi, N, M, i, j);
          if (mi[i][j] == 'X')
          {
            if (vii == 2 || vii == 3)
              mf[i][j] = 'X';
            else
              mf[i][j] = '+';
          }
          else
          {
            if (vii == 3)
              mf[i][j] = 'X';
            else
              mf[i][j] = '+';
          }
        }
        mf[i][M] = '\0';
      }

      for (int i = 0; i < N; i++)
        fprintf(fout, "%s\n", mf[i]);

      if (k < K - 1)
        fprintf(fout, "\n");
      copiazamatr(mi, mf, N);
    }
    fprintf(fout, "\n");
  }

  else if (task == 2)
  { /*
    for (int k = 0; k < K; k++)
    {
      for (int i = 0; i < N; i++)
      {
        for (int j = 0; j < M; j++)
        {
          vii = nrvecinivii(mi, N, M, i, j);
          if (mi[i][j] == 'X')
          {
            if (vii == 2 || vii == 3)
              mf[i][j] = 'X';
            else
              mf[i][j] = '+';
          }
          else
          {
            if (vii == 3)
              mf[i][j] = 'X';
            else
              mf[i][j] = '+';
          }
        }
        mf[i][M] = '\0';
      }

      // lista modificata
      Node *lista = NULL;
      for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
          if (mi[i][j] != mf[i][j])
            add_sfarsit(&lista, i, j, mf[i][j]);

      Node *aux = lista;
      fprintf(fout, "%d ", k + 1);

      while (aux->next)
      {
        fprintf(fout, "%d %d ", aux->x, aux->y);
        aux = aux->next;
      }
      fprintf(fout, "%d %d", aux->x, aux->y);


      if (k < K)
        fprintf(fout, "\n");
      copiazamatr(mi, mf, N);
      freelist(lista);
    }
    */

    // task2 refacut
    Node **stiva = malloc(K * sizeof(Node *));
    for (int k = 0; k < K; k++)
    {
      for (int i = 0; i < N; i++)
      {
        for (int j = 0; j < M; j++)
        {
          vii = nrvecinivii(mi, N, M, i, j);
          if (mi[i][j] == 'X')
          {
            if (vii == 2 || vii == 3)
              mf[i][j] = 'X';
            else
              mf[i][j] = '+';
          }
          else
          {
            if (vii == 3)
              mf[i][j] = 'X';
            else
              mf[i][j] = '+';
          }
        }
        mf[i][M] = '\0';
      }

      Node *lista = NULL;
      for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
          if (mi[i][j] != mf[i][j])
            lista = add_modif(lista, i, j);

      stiva[k] = lista;

      // afisare generatie
      fprintf(fout, "%d ", k + 1);
      Node *aux = lista;
      while (aux && aux->next)
      {
        fprintf(fout, "%d %d ", aux->x, aux->y);
        aux = aux->next;
      }
      if (aux)
        fprintf(fout, "%d %d", aux->x, aux->y);
      if (k < K)
        fprintf(fout, "\n");

      copiazamatr(mi, mf, N);
    }
    for (int i = 0; i < K; i++)
      freelist(stiva[i]);
    free(stiva);
  }
  else if (task == 3)
  {
    Node *radmodif=NULL;
    for(int i=0;i<N;i++)
      for(int j=0;j<M;j++)
        if(matrinit[i][j]=='X')
          add_sfarsit(&radmodif,i,j,'X');

    char **radmatr=aloc_matr(N,M);
    copiazamatr(radmatr,matrinit,N);

    tree *rad=nod_nou(radmodif,radmatr);
    construieste_arbore(rad,N,M,0,K);
    afisare_final(fout,rad,N);
    free_tree(rad,N);

  }

  free_matr(mi, N);
  free_matr(mf, N);
  free_matr(matrinit, N);

  return 0;
}
