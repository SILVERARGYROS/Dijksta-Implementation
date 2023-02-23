// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Debug constants
#define DEBUG 1
#define DEBUG_GRAPH 1
#define DEBUG_WARNINGS_OFF 0
#define SELF_LOOPS_OFF 0
#define FIXED_EXAMPLE 1

// Global variables
int** Graph;
int Vertices;
int Edges;
int i;
int j;


// Color palette, source:https://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define RED		"\x1B[31m"
#define BLUE	"\x1B[36m"
#define GREEN	"\x1B[32m"
#define DEF		"\x1B[0m"

// Supporting Functions
void showGraph();

int main(void)
{
    srand(time(NULL));
    if(DEBUG_WARNINGS_OFF){system("tput clear");}

    if(FIXED_EXAMPLE){
        Vertices = 9
        Edges = 18;

        // Initializing Graph
        Graph = (int**)malloc(sizeof(int*)*Vertices);
        for(i = 0; i < Vertices; i++)
        {
            Graph[i] = (int*)malloc(sizeof(int)*Vertices);
        }
        
        // Creating Example Map
        Graph[0][1] = 1;
        Graph[0][2] = 1;
        Graph[0][3] = 1;
        Graph[1][7] = 1;
        Graph[2][1] = 1;
        Graph[2][3] = 1;
        Graph[2][5] = 1;
        Graph[3][8] = 1;
        Graph[4][2] = 1;
        Graph[4][5] = 1;
        Graph[4][6] = 1;
        Graph[4][8] = 1;
        Graph[5][1] = 1;
        Graph[6][5] = 1;
        Graph[6][7] = 1;
        Graph[6][8] = 1;
        Graph[8][7] = 1;
    }
    else
    {
        // Asking for number of vertices
        printf("Please enter the number of vertices: ");
        do{
            scanf("%d", &Vertices);\
            if(Vertices <= 0)
            {
                printf("Please enter a valid number of vertices: ");
            }
        }while(Vertices <= 0);

        // Asking for number of edges
        printf("Please enter the number of edges: ");
        do{
            scanf("%d", &Edges);\
            if(Edges < 0)
            {
                printf("Please enter a valid number of edges: ");
            }
        }while(Edges < 0);

        // Initializing Graph
        Graph = (int**)malloc(sizeof(int*)*Vertices);
        for(i = 0; i < Vertices; i++)
        {
            Graph[i] = (int*)malloc(sizeof(int)*Vertices);
        }
        for(i = 0; i < Vertices; i++)
        {
            for(j = 0; j < Vertices; j++)
            {
                Graph[i][j] = 0;
            }
        }

        // Filling Graph with edges
        for(i = 0; i < Edges; i++)
        {
            int x;
            int y;
            
            // Picking random vertices --> vertex X connects with vertex Y
            do{
                x = rand() % Vertices;
                y = rand() % Vertices;
            }while(x == y && SELF_LOOPS_OFF);

            if(DEBUG){printf(GREEN "x == %d y == %d\n" DEF, x, y);}

            Graph[x][y] = 1;
        }

    }

    if(DEBUG_GRAPH){showGraph();}

    return 0;
}

void showGraph(){
    int i;
    int j;
    int k;

    for(i = 0; i < Vertices; i++)
    {
        if(i == 0)
        {
            printf("\t  ");
            for(k = 0; k < Vertices; k++)
            { 
                printf(BLUE " %d\t" DEF, k);
            }
            printf("\n\t ");
            for(k = 0; k < Vertices; k++)
            { 
                printf(BLUE "--------" DEF);
            }
            k = 0;
            printf("\n");
        }

        for(j = 0; j < Vertices; j++)
        {
            if(j == 0)
            {
                if(k <= 9)
                {
                    printf(BLUE "\t%d |" DEF, k);
                }
                else
                {
                    printf(BLUE "\t%d|" DEF, k);
                }
                k++;
            }

            printf("%d\t ", Graph[i][j]);
        }
        printf("\n");
    }
}