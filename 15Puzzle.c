/*
15 Puzzle Game
By: zly
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int EmptyTile;
int UndoMoves[1000];
int NrMoves = 0;

void Print_Game(int a[]);
int Check_Game(int a[]);
int Game_Start(int arr[]);
int Next_Move(int a[], int move);
int End_Game(int a[]);
void Undo_Move(int a[]);
int Save_Game(int a[]);
int Load_Game(int a[]);
void Exit_Game();
void Game_Menu(int arr[]);


int main(void)
{
    system("clear");
    printf("\033[0;31m");
    printf("\n\nHello! This is a 15 Puzzle Game");
    printf("\nPress enter to start the game...");
    printf("\033[0m");
    getchar();

    int arr[16];

    srand(time(0));
    Game_Start(arr);

    while(!End_Game(arr)) 
        Game_Menu(arr);

    printf("\033[1;31m");
    printf("\n\n+-------------+");
    printf("\n|   YOU WON   |");
    printf("\n+-------------+\n\n");
    printf("\033[0m");
    
    return 0;
}

void Print_Game(int a[])  //print a visual way to observe the game
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

    for (int i = 0; i < 16; i++)
    {
        if (i % 4 == 0)
        {
            printf("\033[0;36m");
            printf("|");
        }
        if (a[i] == 0) printf("    ");
        else 
        {
            printf("\033[0m");
            if (a[i] < 10) printf("  %d ", a[i]);
            else printf(" %d ", a[i]);
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
        if ((i+1) % 4 == 0 && i != 15) 
        {
            printf("\033[0;35m");
            printf("\n|-------------------|\n");
        }
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

    Game_Menu(a);
}

int Check_Game(int a[])   //check if the game is solvable
{
    int inv = 0;
    for (int i=0; i<16; ++i)
        if (a[i])
            for (int j=0; j<i; ++j)
                if (a[j] > a[i])
                    ++inv;
    for (int i=0; i<16; ++i)
        if (a[i] == 0)
            inv += 1 + i / 4;

    if (inv & 1) Game_Start(a);
    else Print_Game(a);
}

int Game_Start(int arr[])    //create the list with order of numbers of the game
{
    for (int i = 0; i < 16; i++)    // fill array
        arr[i] = i;

    for (int i = 0; i < 16; i++)    // shuffle array
    {    
        int temp = arr[i];
        int randomIndex = rand() % 16;
        arr[i] = arr[randomIndex];
        arr[randomIndex] = temp;
    }

    for (int i = 0; i < 16; i++)
        if (!arr[i]) EmptyTile = i;

    if (Check_Game(arr)) Print_Game(arr);
    else Game_Start(arr);
}

int Next_Move(int a[], int move)  //execute the selected move
{
    int pos, flag = 0;
    
    
    while(!flag)   
    {
        flag = 0;

        for (int i = 0; i < 16; i++)
        {
            if (a[i] == move)   //check if the move is possible
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
        }
    }
    
    a[pos] = 0;
    a[EmptyTile] = move;
    UndoMoves[NrMoves] = EmptyTile;
    NrMoves++;
    EmptyTile = pos;

    Print_Game(a);
}

int End_Game(int a[])  //check the condition for ending the game
{ 
    for (int i = 0; i < 16; i++)
    {
        if (i == 15 && a[i] != 0) return 0;
        else if (a[i] != i + 1 && i != 15) return 0; 
    }
    return 1;
}

void Undo_Move(int a[])  //undo the last move
{
    int temp = a[UndoMoves[NrMoves - 1]];
    a[UndoMoves[NrMoves - 1]] = 0;
    a[EmptyTile] = temp;
    EmptyTile = UndoMoves[NrMoves - 1];
    NrMoves--;

    Print_Game(a);
}

int Save_Game(int a[])  //option to save the current progress of the game
{
    FILE* fp = NULL;
    fp = fopen("save15Puzzle.txt", "w+");
    
    for (int i = 0; i < 16; i++)
        fprintf(fp, "%d ", a[i]);
    fprintf(fp, "%d ", EmptyTile);
    fprintf(fp, "%d ", NrMoves);
    for (int i = 0; i < NrMoves; i++)
        fprintf(fp, "%d ", UndoMoves[i]);

    fclose(fp);

    printf("\033[1;31m");
    printf("Game Saved\n\n");
    
    printf("\033[1;31m");
    printf("Do you want to close the game? (y/n) ");
    printf("\033[0m");
    
    char answer[100];
    scanf("%s", answer);
    if (!strcmp(answer, "y")) Exit_Game();
    else Game_Menu(a);
}

int Load_Game(int a[])  //load the last saved game
{
    FILE* fp;
    fp = fopen("save15Puzzle.txt", "r");

    for (int i = 0; i < 18; i++)
    {
        if (i == 16) fscanf(fp, "%d", &EmptyTile);
        else if (i == 17) fscanf(fp, "%d", &NrMoves);
        else if (i > 17) fscanf(fp, "%d", &UndoMoves[i]);
        else fscanf(fp, "%d", &a[i]);
    }

    fclose(fp);
    Print_Game(a);
}

void Exit_Game()  //clode the game and program
{
    printf("Closing game...\n\n");
    exit(0);
}

void Game_Menu(int arr[])  //menu with options of the game
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
    for (int i = 0; i < 15; i++)
        if (!strcmp(option, list[i]))
        {
            int num = atoi(list[i]);
            Next_Move(arr, num);
            flag = 1;
        }
    if (!flag)
    {
        if (!strcmp(option, "Restart")) Game_Start(arr);
        else if (!strcmp(option, "restart")) Game_Start(arr);
        else if (!strcmp(option, "Undo")) Undo_Move(arr);
        else if (!strcmp(option, "undo")) Undo_Move(arr);
        else if (!strcmp(option, "Save")) Save_Game(arr);
        else if (!strcmp(option, "save")) Save_Game(arr);
        else if (!strcmp(option, "Resume")) Load_Game(arr);
        else if (!strcmp(option, "resume")) Load_Game(arr);
        else if (!strcmp(option, "Exit")) Exit_Game();
        else if (!strcmp(option, "exit")) Exit_Game();
        else 
        {
            printf("\033[0;31m");
            printf("   ERROR! There is no such option");
            printf("\033[0m");
            Game_Menu(arr);
        }
    }
}
