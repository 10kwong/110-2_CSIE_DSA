#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
void ERROR() {while(1){} exit(0);}
// 89 testcases with Close operations

typedef struct Person
{
    int id; 
    struct Person *nxt;
    struct Person *prv;
} Person;
Person* Person_alloc(int identity, Person *nextPerson, Person *previousPerson);

typedef struct Group
{
    int gp; 
    int direction; // 1 == normal, 0 == reverse
    Person *head;
    Person *tail;

    struct Group *nxt;
    struct Group *prv;
} Group;
void DirectionChange(Group *gp) {gp->direction = !gp->direction;}
Group* Group_alloc(int GroupID, Person* firstPerson, Group* nextGroup, Group* previousGroup);
void DisplayGroup(Group *gp);

typedef struct Bathroom
{
    Group *head;
    Group *tail; 
    int closed; // default not close = 0, closed = 1
} Bathroom; 

Bathroom *Bathrooms;
Group* **Table; 
int M, N, K;
void PurpleCow();
void Init();
void FreeAll();
void Display();
void DisplayFromTail();

void enter(int i, int j, int m);
void leave(int m);
void go(int m);
void close(int m);
int main()
{
    PurpleCow();
    return 0;
}
void PurpleCow()
{
    scanf(" %d %d %d", &M, &N, &K);
    Init();
    char op[10]; 
    int i = 0, j = 0, m = 0;
    for (int situation = 0 ; situation < N ; situation++)
    {
        scanf(" %s", op);
        if (op[0] == 'e')
        {
            scanf(" %d %d %d", &i, &j, &m);
            enter(i, j, m);
        }
        else if (op[0] == 'l')
        {
            scanf(" %d ", &m);
            leave(m);
        }
        else if (op[0] == 'g')
        {
            scanf(" %d ", &m);
            go(m);
        }
        else if (op[0] == 'c')
        {
            scanf(" %d ", &m);
            close(m);
        }
    }
    Display();
    FreeAll();
}
void enter(int i, int j, int m)
{
    if (Bathrooms[m].closed) return;
    // No group i doesn't exist in Bathroom m
    if (Table[m][i] == NULL)
    {
        // create a Group Node with Group id i that contains a Person Node with id j
        Person *newPerson = Person_alloc(j, NULL, NULL);
        Group* newGroupNode = Group_alloc(i, newPerson, NULL, NULL);
        // if Bathroom m is empty, simply just insert new group 
        if (Bathrooms[m].head == NULL && Bathrooms[m].tail == NULL)
        {
            // printf("First Person %d from Group %d has entered.\n", newGroupNode->head->id, newGroupNode->gp);
            Bathrooms[m].head = newGroupNode; 
            Bathrooms[m].tail = newGroupNode;
        }
        else // insert new group to the back
        {
            // printf("Find Person %d from new Group %d, insert to the Tail\n", newGroupNode->head->id, newGroupNode->gp);
            newGroupNode->nxt = Bathrooms[m].tail;
            Bathrooms[m].tail->prv = newGroupNode;
            Bathrooms[m].tail = Bathrooms[m].tail->prv;
        }
        // update Table, group i now exists in bathroom m
        Table[m][i] = newGroupNode; 
    }
    else 
    {
        Group* sameGroupNode = Table[m][i]; 
        if (sameGroupNode->head == sameGroupNode->tail) DirectionChange(sameGroupNode);
        if (sameGroupNode->direction)
        {
            Person* newPerson = Person_alloc(j, sameGroupNode->tail, NULL); 
            // printf("Find Old Group %d, Jump to Person %d.\n", i, sameGroupNode->tail->id);
            sameGroupNode->tail->prv = newPerson; 
            sameGroupNode->tail = sameGroupNode->tail->prv;
        }
        else
        {
            Person* newPerson = Person_alloc(j, NULL, sameGroupNode->head); 
            // printf("Find Old Group %d, Jump to Person %d.\n", i, sameGroupNode->head->id);
            sameGroupNode->head->nxt = newPerson; 
            sameGroupNode->head = sameGroupNode->head->nxt;
        }
    }
}
void leave(int m)
{
    if (Bathrooms[m].closed) return; 
    Group* lastGroupInBathroom = Bathrooms[m].tail;
     // printf("Remove Person %d from Tail Group\n", lastGroupInBathroom->tail->id);
    // have only one node in last group
    if (lastGroupInBathroom->head == lastGroupInBathroom->tail)
    {
        // if next group is not the same group 
        // update Table, remove group m from Table
        Table[m][Bathrooms[m].tail->gp] = NULL;
        
        // if there are more than one group node in bathroom
        if (Bathrooms[m].head != Bathrooms[m].tail)
        {
            // update tail pointer, go to next group
            // printf("Tail Group %d is now eliminated\n", lastGroupInBathroom->gp);
            Bathrooms[m].tail = Bathrooms[m].tail->nxt; 
            Bathrooms[m].tail->prv = NULL;
        }
        else // have only one group in bathroom 
        {   
            // printf("Group %d is removed at tail, Bathroom is now empty\n", lastGroupInBathroom->gp);
            Bathrooms[m].head = Bathrooms[m].tail = NULL;
        }
        free(lastGroupInBathroom);
    }
    else if (lastGroupInBathroom->direction)
    {
        // get last person in group
        Person* lastPerson = lastGroupInBathroom->tail; 
        // group tail steps forward
        Bathrooms[m].tail->tail = Bathrooms[m].tail->tail->nxt;
        Bathrooms[m].tail->tail->prv = NULL;
        free(lastPerson);
    }
    else
    {
        // get last person in group
        Person* lastPerson = lastGroupInBathroom->head; 
        // group tail steps forward
        Bathrooms[m].tail->head = Bathrooms[m].tail->head->prv;
        Bathrooms[m].tail->head->nxt = NULL;
        free(lastPerson);
    }
    return; 
}
void go(int m)
{   
    if (Bathrooms[m].closed) return;
    Group* frontGroupInBathroom = Bathrooms[m].head;
    // printf("Remove Person %d from Front Group\n", frontGroupInBathroom->head->id);
    // have only one node in front group
    if (frontGroupInBathroom->head == frontGroupInBathroom->tail)
    {
        // if previous group is not the same group 
        // update Table, remove group m from Table
        Table[m][frontGroupInBathroom->gp] = NULL;

        // if there are more than one group node in bathroom
        if (Bathrooms[m].head != Bathrooms[m].tail)
        {
            // go to previous group 
            // printf("Front group %d is now eliminated\n", frontGroupInBathroom->gp);
            Bathrooms[m].head = Bathrooms[m].head->prv;
            Bathrooms[m].head->nxt = NULL;
        }
        else // have only one group in bathroom 
        {
            // printf("Group %d is removed at front, Bathroom is now empty\n", frontGroupInBathroom->gp);
            Bathrooms[m].head = Bathrooms[m].tail = NULL;
        }
        // Since there is only one node in group, we can just delete the whole group
        free(frontGroupInBathroom);
    }
    else if (frontGroupInBathroom->direction)
    {
        Person* firstPerson = frontGroupInBathroom->head; 
        Bathrooms[m].head->head = Bathrooms[m].head->head->prv;
        Bathrooms[m].head->head->nxt = NULL;
        free(firstPerson);
    }
    else
    {
        Person* firstPerson = frontGroupInBathroom->tail; 
        Bathrooms[m].head->tail = Bathrooms[m].head->tail->nxt;
        Bathrooms[m].head->tail->prv = NULL;
        free(firstPerson);
        
    }
    return;
}
void close(int m)
{
    // find bathroom 
    int j = m-1 >= 0? m-1:M-1;
    int cnt = 0;
    while (cnt < M-2 && Bathrooms[j].closed)
    {
        j--;
        cnt++;
        if (j < 0) j = M-1;
    }
    if (Bathrooms[j].head == NULL && Bathrooms[j].tail == NULL)
    {
        Bathrooms[j].head = Bathrooms[m].head; 
        Bathrooms[j].tail = Bathrooms[m].tail;
    }
    Group* trav = Bathrooms[m].tail; 
    Group* gptr; 
    while (trav != NULL)
    {
        gptr = trav; 
        // printf("Travelling Group %d, ", gptr->gp);
        // fflush(stdout);
        trav = trav->nxt; 
        if (trav != NULL) trav->prv = NULL;
        
        // needless to changeDir if only one Person in Group
        if (gptr->head != gptr->tail) DirectionChange(gptr);
        Table[m][gptr->gp] = NULL; 
        // if Group exist in Bathroom j already
        Group* origg = Table[j][gptr->gp];
        if (origg != NULL)
        {
            gptr->nxt = NULL;
            if (origg->direction == gptr->direction)
            {
                if (origg->direction)
                {
                    gptr->head->nxt = origg->tail; 
                    origg->tail->prv = gptr->head; 
                    origg->tail = gptr->tail; 
                    origg->tail->prv = NULL; 
                }
                else
                {
                    gptr->tail->prv = origg->head; 
                    origg->head->nxt = gptr->tail; 
                    origg->head = gptr->head; 
                    origg->head->nxt = NULL; 
                }
            }
            else
            {
                // insert gptr from tail to head
                if (origg->direction)
                {
                    Person* ptrav = gptr->tail;  
                    Person* pptr;
                    while (ptrav != NULL)
                    {
                        pptr = ptrav; 
                        // printf("walk through node %d..\n", pptr->id);
                        ptrav = ptrav->nxt; 

                        pptr->nxt = origg->tail; 
                        pptr->prv = NULL;
                        origg->tail->prv = pptr;
                        origg->tail = origg->tail->prv; 
                    }
                }
                else
                {
                    Person* ptrav = gptr->head;  
                    Person* pptr;
                    while (ptrav != NULL)
                    {
                        pptr = ptrav; 
                        // printf("walk through node %d..\n", pptr->id);
                        ptrav = ptrav->prv; 

                        pptr->prv = origg->head; 
                        pptr->nxt = NULL;
                        origg->head->nxt = pptr;
                        origg->head = origg->head->nxt; 
                    }
                }
            }
            gptr->head = NULL; 
            gptr->tail = NULL;
        }
        else
        {
            // update Table
            // printf("this is a new Group\n");
            // DisplayGroup(gptr);
            Table[j][gptr->gp] = gptr; 
            gptr->prv = NULL;
            // Gp DNE & Empty Bathroom
            if (Bathrooms[j].head == NULL && Bathrooms[j].tail == NULL)
            {
                Bathrooms[j].head = gptr; 
                Bathrooms[j].tail = gptr;
                gptr->nxt = NULL; 
            }
            else
            {
                gptr->nxt = Bathrooms[j].tail;
                Bathrooms[j].tail->prv = gptr; 
                Bathrooms[j].tail = Bathrooms[j].tail->prv; 
            }
        }
    }
    Bathrooms[m].head = NULL; 
    Bathrooms[m].tail = NULL; 
    Bathrooms[m].closed = 1; 
}
Person* Person_alloc(int identity, Person *nextPerson, Person *previousPerson)
{
    Person *thisPerson = (Person *) malloc(sizeof(Person));
    thisPerson->id = identity;
    thisPerson->nxt = nextPerson;
    thisPerson->prv = previousPerson;
    return thisPerson; 
}
Group* Group_alloc(int GroupID, Person* firstPerson, Group* nextGroup, Group* previousGroup)
{
    Group *thisGroup = (Group *) malloc(sizeof(Group));
    thisGroup->direction = 1; 
    thisGroup->gp = GroupID; 
    thisGroup->head = firstPerson;
    thisGroup->tail = firstPerson;
    thisGroup->nxt = nextGroup;
    thisGroup->prv = previousGroup;
    return thisGroup; 
}
void Init()
{
    Bathrooms = (Bathroom *) malloc(M * sizeof(Bathroom));
    Table = (Group ***) malloc(M * sizeof(Group **));
    for (int i = 0 ; i < M ; i++)
    {
        Bathrooms[i].head = NULL;
        Bathrooms[i].tail = NULL;
        Bathrooms[i].closed = 0; 
        Table[i] = (Group **) malloc(K * sizeof(Group*));
        for (int j = 0 ; j < K ; j++)
            Table[i][j] = NULL; 
    }
}
void FreeAll()
{
    return;
}
void Display()
{
    for (int i = 0; i < M; i++)
    {
        if (!Bathrooms[i].closed)
        {
            Group* this = Bathrooms[i].head;
            while (this != NULL)
            {
                // printf(" g%d| ", this->gp);
                DisplayGroup(this);
                this = this->prv;
            }
        }
        printf("\n");
    }
    // printf("===\n");
}
void DisplayGroup(Group *gp)
{
    if (gp->direction)
    {
        Person* p = gp->head; 
        while (p != NULL)
        {
            printf("%d ", p->id);
            p = p->prv;
        }
    }
    else 
    {
        Person* p = gp->tail; 
        while (p != NULL)
        {
            printf("%d ", p->id);
            p = p->nxt;
        }
    }
}
