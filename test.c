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

void freelist(Node *head) // elibereaza mem alocata pt lista
{
  Node *aux;
  while (head)
  {
    aux = head;
    head = head->next;
    free(aux);
  }
}

int nrvecinivii(char **m, int N, int M, int x, int y)
{
  int ct = 0;
  int nx, ny;
  for (int i = 0; i < 8; i++)
  {
    nx = x + dx[i];
    ny = y + dy[i];
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

int main(int argc, char *argv[])
{
  FILE *fin = fopen(argv[1], "rt");
  FILE *fout = fopen(argv[2], "wt");

  int task, N, M, K;
  int vii;
  fscanf(fin, "%d", &task);
  fscanf(fin, "%d %d", &N, &M);
  fscanf(fin, "%d", &K);

  char **mi = malloc(N * sizeof(char *)); // matricea initiala
  char **mf = malloc(N * sizeof(char *)); // matricea finala
  for (int i = 0; i < N; i++)
  {
    mi[i] = malloc((M + 1) * sizeof(char));
    mf[i] = malloc((M + 1) * sizeof(char));
  }

  for (int i = 0; i < N; i++)
    fscanf(fin, "%s", mi[i]); // citeste in stare initiala

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
  {
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
  }

  for (int i = 0; i < N; i++)
  {
    free(mi[i]);
    free(mf[i]);
  }
  free(mi);
  free(mf);
  /*
  fclose(fin);
  fclose(fout);*/
  return 0;
}
