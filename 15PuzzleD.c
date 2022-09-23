/*
15 Puzzle Game
By: zly
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int NrTiles = 16;
int EmptyTile;
int UndoMoves[1000];
int NrMoves = 0;

struct Node {
    int data;
    struct Node* next;
};

struct Node* head = NULL;

void Add_Node(int value);
void Print_Game();
int Check_Game();
void Game_Start();
void Next_Move(int move);
int End_Game();
void Undo_Move();
void Save_Game();
void Load_Game();
void Exit_Game();
void Game_Menu();


int main(void)
{
    system("clear");
    printf("\033[0;31m");
    printf("\n\nHello! This is a 15 Puzzle Game");
    printf("\nPress enter to start the game...");
    printf("\033[0m");
    getchar();

    srand(time(0));
    Game_Start();

    while(!End_Game()) 
        Game_Menu();

    printf("\033[1;31m");
    printf("\n\n+-------------+");
    printf("\n|   YOU WON   |");
    printf("\n+-------------+\n\n");
    printf("\033[0m");
    
    return 0;
}

void Print_Game()  //print a visual way to observe the game
{
    system("clear");
    printf("\n");

    for (int k = 0; k <= 20; k++)
    {
        if (k == 0 || k == 20) 
        {
            printf("\033[0;35m");
            printf("+");
        }
        else 
        {
            printf("\033[0;36m");
            printf("-");
        }
    }
    printf("\n");

    struct Node* temp = head;
    int i = 0;

    while (temp != NULL)
    {
        if (i % 4 == 0)
        {
            printf("\033[0;36m");
            printf("|");
        }
        if (temp->data == 0) printf("    ");
        else 
        {
            printf("\033[0m");
            if (temp->data < 10) printf("  %d ", temp->data);
            else printf(" %d ", temp->data);
        }
        if ((i+1) % 4 != 0)
        {
            printf("\033[0;35m");
            printf("|");
        }
        else if ((i+1) % 4 == 0) 
        {
            printf("\033[0;36m");
            printf("|");
        }
        if ((i+1) % 4 == 0 && i != NrTiles-1) 
        {
            printf("\033[0;35m");
            printf("\n|-------------------|\n");
        }
        temp = temp->next;
        i++;
    }


    printf("\n");
    for (int k = 0; k <= 20; k++)
    {
        if (k == 0 || k == 20) 
        {
            printf("\033[0;35m");
            printf("+");
        }
        else 
        {
            printf("\033[0;36m");
            printf("-");
        }
    }

    Game_Menu();
}

int Check_Game(int a[])   //check if the game is solvable
{
    int inv = 0;
    for (int i = 0; i < NrTiles; ++i)
        if (a[i])
            for (int j = 0; j < i; ++j)
                if (a[j] > a[i])
                    ++inv;
    for (int i = 0; i < NrTiles; ++i)
        if (a[i] == 0)
            inv += 1 + i / 4;

    if (inv & 1) return 0;
    else return 1;
}

void Add_Node(int value)
{
    struct Node* new = (struct Node*)malloc(sizeof(struct Node));
    struct Node* temp = head;

    new->data = value;
    new->next = NULL;

    if (head == NULL)
    {
        head = new;
        return;
    }
    while(temp->next != NULL)
        temp = temp->next;

    temp->next = new;
    return;
}

void Game_Start()    //create the list with order of numbers of the game
{
    if (head != NULL)
    {
        struct Node* temp = head;
        struct Node* next = NULL;
        while (temp != NULL)
        {
            next = temp->next;
            free(temp);
            temp = next;
        }
        head = NULL;
    }

    int arr[NrTiles];
    for (int i = 0; i < NrTiles; i++)    // fill array
        arr[i] = i;

    for (int i = 0; i < NrTiles; i++)    // shuffle array
    {    
        int temp = arr[i];
        int randomIndex = rand() % NrTiles;
        arr[i] = arr[randomIndex];
        arr[randomIndex] = temp;
    }

    for (int i = 0; i < NrTiles; i++)
        if (!arr[i]) EmptyTile = i;

    if (Check_Game(arr)) 
    {
        for (int i = 0; i < NrTiles; i++)
            Add_Node(arr[i]);
        Print_Game();
    }
    else Game_Start();
}

void Next_Move(int move)  //execute the selected move
{
    int pos, flag = 0;
    
    
    while(!flag)   
    {
        flag = 0;
        int i = 0;
        struct Node* temp = head;

        while(temp != NULL)
        {
            if (temp->data == move)   //check if the move is possible
            {
                pos = i;
                if (i != EmptyTile - 1 && i != EmptyTile + 1 && i != EmptyTile - 4 && i != EmptyTile + 4)
                {
                    printf("\033[0;31m");
                    printf("\n   ERROR! Impossible move \n");
                    printf("\033[0;35m");
                    printf("\nEnter the number you want to move: ");
                    printf("\033[0m");
                    scanf("%d", &move);
                    break;
                }  
                else if (EmptyTile == 3 || EmptyTile == 7 || EmptyTile == 11)
                {
                    if (pos == EmptyTile + 1)
                    {
                        printf("\033[0;31m");
                        printf("\n   ERROR! Impossible move \n");
                        printf("\033[0;35m");
                        printf("\nEnter the number you want to move: ");
                        printf("\033[0m");
                        scanf("%d", &move); 
                        break;
                    }
                }
                else if (EmptyTile == 4 || EmptyTile == 8 || EmptyTile == 12)
                {
                    if (pos == EmptyTile - 1)
                    {
                        printf("\033[0;31m");
                        printf("\n   ERROR! Impossible move \n");
                        printf("\033[0;35m");
                        printf("\nEnter the number you want to move: ");
                        printf("\033[0m");
                        scanf("%d", &move);
                        break;
                    }
                }
                flag = 1;
            }

            temp = temp->next;
            i++;
        }
    }

    int i = 0;
    struct Node* temp = head;
    while (temp != NULL)
    {
        if (i == pos) temp->data = 0;
        if (i == EmptyTile) temp->data = move;
        temp = temp->next;
        i++;
    }
    UndoMoves[NrMoves] = EmptyTile;
    NrMoves++;
    EmptyTile = pos;

    Print_Game();
}

int End_Game()  //check the condition for ending the game
{ 
    struct Node* temp = head;
    int i = 0;

    while(temp != NULL)
    {
        if (i == NrTiles-1 && temp->data != 0) return 0;
        else if (temp->data != i + 1 && i != NrTiles-1) return 0; 
        temp = temp->next;
        i++;
    }
    return 1;
}

void Undo_Move()  //undo the last move
{
    struct Node* temp = head;
    int i = 0, a[NrTiles];

    while(temp != NULL)
    {
        a[i] = temp->data;
        temp = temp->next;
        i++;
    }

    int t = a[UndoMoves[NrMoves - 1]];
    a[UndoMoves[NrMoves - 1]] = 0;
    a[EmptyTile] = t;
    EmptyTile = UndoMoves[NrMoves - 1];
    NrMoves--;

    temp = head;
    i = 0;
    while(temp != NULL)
    {
        temp->data = a[i];
        temp = temp->next;
        i++;
    }

    Print_Game();
}

void Save_Game()  //option to save the current progress of the game
{
    FILE* fp = NULL;
    fp = fopen("save15Puzzle.txt", "w+");
    
    struct Node* temp= head;

    while (temp != NULL)
    {
        fprintf(fp, "%d ", temp->data);
        temp = temp->next;
    }
    fprintf(fp, "%d ", EmptyTile);

    fclose(fp);

    printf("\033[1;31m");
    printf("Game Saved\n\n");
    
    printf("\033[1;31m");
    printf("Do you want to close the game? (y/n) ");
    printf("\033[0m");
    
    char answer[100];
    scanf("%s", answer);
    if (!strcmp(answer, "y")) Exit_Game();
    else Game_Menu();
}

void Load_Game()  //load the last saved game
{
    FILE* fp;
    fp = fopen("save15Puzzle.txt", "r");

    struct Node* temp= head;

    while (temp != NULL)
    {
        fscanf(fp, "%d", &temp->data);
        temp = temp->next;
    }
    fscanf(fp, "%d", &EmptyTile);

    fclose(fp);
    Print_Game();
}

void Exit_Game()  //clode the game and program
{
    struct Node* temp = head;
    struct Node* next = NULL;
    while (temp != NULL)
    {
        next = temp->next;
        free(temp);
    	temp = next;
    }
    head = NULL;
    
    printf("Closing game...\n\n");
    exit(0);
}

void Game_Menu()  //menu with options of the game
{
    printf("\033[1;36m");
    printf("\n\n+---------------------------------------------+");
    printf("\n| MENU:                                       |");
    printf("\n|   ");
    printf("\033[0;37m");
    printf("Save");
    printf("\033[1;36m");
    printf("                                      |");
    printf("\n|   ");
    printf("\033[0;37m");
    printf("Resume");
    printf("\033[1;36m");
    printf("                                    |");
    printf("\n|   ");
    printf("\033[0;37m");
    printf("Restart");
    printf("\033[1;36m");
    printf("                                   |");
    printf("\n|   ");
    printf("\033[0;37m");
    printf("Undo");
    printf("\033[1;36m");
    printf("                                      |");
    printf("\n|   ");
    printf("\033[0;37m");
    printf("Play: Enter the number you want to move");
    printf("\033[1;36m");
    printf("   |");
    printf("\n|   ");
    printf("\033[0;37m");
    printf("Exit");
    printf("\033[1;36m");
    printf("                                      |");
    printf("\n+---------------------------------------------+");

    char option[100];
    char list[15][5] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};
    printf("\033[0;35m");
    printf("\n\nEnter the option you want to select: ");
    printf("\033[0m");

    scanf("%s", option);

    int flag = 0;
    for (int i = 0; i < NrTiles-1; i++)
        if (!strcmp(option, list[i]))
        {
            int num = atoi(list[i]);
            Next_Move(num);
            flag = 1;
        }
    if (!flag)
    {
        if (!strcmp(option, "Restart")) Game_Start();
        else if (!strcmp(option, "restart")) Game_Start();
        else if (!strcmp(option, "Undo")) Undo_Move();
        else if (!strcmp(option, "undo")) Undo_Move();
        else if (!strcmp(option, "Save")) Save_Game();
        else if (!strcmp(option, "save")) Save_Game();
        else if (!strcmp(option, "Resume")) Load_Game();
        else if (!strcmp(option, "resume")) Load_Game();
        else if (!strcmp(option, "Exit")) Exit_Game();
        else if (!strcmp(option, "exit")) Exit_Game();
        else 
        {
            printf("\033[0;31m");
            printf("   ERROR! There is no such option");
            printf("\033[0m");
            Game_Menu();
        }
    }
}
