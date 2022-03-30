#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The cities and roads can be viewed as a rooted tree from any city
typedef struct City
{
    int id; 
    struct AdjacentList *Nearby;
    int res;
    int visited;
} City;
City* AllocCity(int id, City** NearCities);

typedef struct Kingdom
{
    City **Cities;
    City *Capital; 
    City *Resort;
} Kingdom; 

typedef struct Note
{
    City* city;
    struct Note* nxt;
} Note;
Note* AllocNote(int id, Note * nxt);
typedef struct AdjacentList
{
    Note* head;
} AdjacentList; 

void TeacherLightningBearsKingdom();
void Query(int c);
void RoadConstruct(int from, int dest);
void Init();
void PreCal();
int FindResort(City* curCity);
void CalResult(AdjacentList* neighbors, int value);

Kingdom *BearKingdom;
int N, Q, S, R;
int main()
{
    TeacherLightningBearsKingdom();
    return 0;
}
void TeacherLightningBearsKingdom()
{
    Init();
    for (int q = 0, c; q < Q; q++)
    {
        scanf(" %d", &c);
        Query(c);
    }
    return ;
}
void Query(int c)
{
    if (S == R) printf("%d\n", R);
    else if (S == c) printf("%d\n", S);
    else if (R == c) printf("%d\n", R);
    else
    {
        printf("%d\n", BearKingdom->Cities[c]->res);
    }
    return; 
}
void Init()
{
    scanf(" %d %d %d %d", &N, &Q, &S, &R);
    BearKingdom = (Kingdom *) malloc(sizeof(Kingdom));
    BearKingdom->Cities = (City **) calloc(N+1, sizeof(City *));
    for (int i = 1; i <= N; i++) BearKingdom->Cities[i] = AllocCity(i, NULL);
    BearKingdom->Capital = BearKingdom->Cities[S];
    BearKingdom->Resort = BearKingdom->Cities[R];
    for (int i = 1, from, dest; i < N; i++)
    {
        scanf("%d %d", &from, &dest);
        RoadConstruct(from, dest);
    }
    PreCal();
    return ;
}
void PreCal()
{
    FindResort(BearKingdom->Capital);
}
int FindResort(City* curCity)
{
    curCity->visited = 1;
    if (curCity == BearKingdom->Resort)
    {
        CalResult(curCity->Nearby, curCity->res);
        return 1;
    }
    else
    {
        Note* note = curCity->Nearby->head;
        while (note != NULL)
        {
            // if found the path to Resort
            if (!note->city->visited)
            {
                if (FindResort(note->city))
                {
                    CalResult(curCity->Nearby, curCity->res);
                    return 1;
                }
            }
            note = note->nxt;
        }
    }
    curCity->visited = 0;
    return 0;
}
void CalResult(AdjacentList* neighbors, int value)
{
    Note* neighbor = neighbors->head;
    while (neighbor != NULL)
    {
        // if this city didnt connect by capital and resort
        if (!neighbor->city->visited)
        {
            neighbor->city->visited = 1;
            neighbor->city->res = value;
            CalResult(neighbor->city->Nearby, value);
        }
        neighbor = neighbor->nxt; 
    }
    return; 
}
City* AllocCity(int id, City** NearCities)
{
    City* aCity = (City *) malloc(sizeof(City));
    aCity->id = id; 
    aCity->Nearby = (AdjacentList *) malloc(sizeof(AdjacentList));
    aCity->Nearby->head = NULL; 
    aCity->res = id;
    aCity->visited = 0;
    return aCity; 
}
Note* AllocNote(int id, Note* nxt)
{
    Note* aNote = (Note *) malloc(sizeof(Note));
    aNote->city = BearKingdom->Cities[id];
    aNote->nxt = nxt; 
    return aNote;
}
void RoadConstruct(int from, int dest)
{
    BearKingdom->Cities[from]->Nearby->head = AllocNote(dest, BearKingdom->Cities[from]->Nearby->head);
    BearKingdom->Cities[dest]->Nearby->head = AllocNote(from, BearKingdom->Cities[dest]->Nearby->head);;
}
