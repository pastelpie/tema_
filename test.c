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
  // char **matr;//matricea dupa aplicarea modificarilor
};
typedef struct Ntree tree;

tree *nod_nou(Node *modif)
{
  tree *aux = malloc(sizeof(tree));
  aux->modificari = modif;
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
    // m[i][M] = '\0';
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

// add la finalul listei un nod cu coord x si y
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

// add la finalul listei dublu inlantuite un nod complet
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

void free_tree(tree *nod)
{
  if (!nod)
    return;
  freelist(nod->modificari);
  free_tree(nod->st);
  free_tree(nod->dr);
  free(nod);
}

// aplica modif pe matr
void changes(char **m, Node *mod)
{
  for (Node *p = mod; p; p = p->next)
    m[p->x][p->y] = p->stare;
}

// determina modif dintre o gen si urm dupa regula
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

// task3 arb de generatii construit pana la adancimea K
void construieste_arbore(tree *nod,char **matr, int N, int M, int adancime, int K)
{
  if (adancime == K)
    return;

  
  char **auxb = aloc_matr(N, M);
  copiazamatr(auxb,matr, N);
  char **auxs = aloc_matr(N, M);
  copiazamatr(auxs,matr, N);

  // regula B(fiul stang)
  Node *fb = NULL; // lista celule care isi schimba starea de la o gen la alta pt reg B
  calc_changes(auxb, N, M, 0, &fb);
  
  changes(auxb, fb);
  nod->st = nod_nou(fb);
  construieste_arbore(nod->st,auxb, N, M, adancime + 1, K);
  free_matr(auxb,N);

  // regula standard(fiul drept)
  Node *fs = NULL; // lista celule care isi schimba starea de la o gen la alta pt reg standard
  calc_changes(auxs, N, M, 1, &fs);
  
  changes(auxs, fs);
  nod->dr = nod_nou(fs);
  construieste_arbore(nod->dr,auxs, N, M, adancime + 1, K);
  free_matr(auxs,N);


}

// parcurge preordine si afis toate matr din arbore
void afisare_final(FILE *fout, tree *nod, char **matr, int N, int M)
{
  if (!nod)
    return;

  // copie a matr primite sa nu o stric
  char **copie = aloc_matr(N, M);
  copiazamatr(copie, matr, N);

  changes(copie, nod->modificari);

  for (int i = 0; i < N; i++)
    fprintf(fout, "%s\n", copie[i]);
  fprintf(fout, "\n");

  //merge recursiv pe subarbori
  afisare_final(fout, nod->st, copie, N, M);

  char **copie2 = aloc_matr(N, M);
  copiazamatr(copie2, matr, N);
  changes(copie2,nod->modificari);

  afisare_final(fout, nod->dr, copie2, N, M);

  free_matr(copie,N);
  free_matr(copie2,N);
}

// bonus task2 pdf
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

int main(int argc, const char *argv[])
{

  FILE *fin = fopen(argv[1], "rt");
  FILE *fout = fopen(argv[2], "wt");

  int task, N, M, K;
  int vii;
  fscanf(fin, "%d", &task);
  fscanf(fin, "%d %d", &N, &M);
  fscanf(fin, "%d", &K);

  char **mi = aloc_matr(N, M);       // matricea initiala
  char **mf = aloc_matr(N, M);       // matricea finala
  char **matrinit = aloc_matr(N, M); // copie pt t3

  for (int i = 0; i < N; i++)
    fscanf(fin, "%s", mi[i]); // citeste in stare initiala

  for (int i = 0; i < N; i++)
    strcpy(matrinit[i], mi[i]); // copiaza starea initiala pt arbore

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
    Node *rootmodif = NULL;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++)
        if (matrinit[i][j] == 'X')
          add_sfarsit(&rootmodif, i, j, 'X');

    tree *root=nod_nou(rootmodif);
    char **copie=aloc_matr(N,M);
    copiazamatr(copie,matrinit,N);

    construieste_arbore(root,copie,N,M,0,K);
    afisare_final(fout,root,copie,N,M);
    free_tree(root);
    free_matr(copie,N);
  }

  free_matr(mi, N);
  free_matr(mf, N);
  free_matr(matrinit, N);

  return 0;
}
