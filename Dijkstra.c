// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Debug constants
#define DEBUG 0
#define DEBUG_RESULTS 0
#define VIEW_GRAPH 0
#define DEBUG_SPOT 0
#define DEBUG_VALUES 0
#define CLEAR_SCREEN 1
#define SELF_LOOPS_OFF 1
#define COST_LIMIT 40
#define FIXED_EXAMPLE 0
#define STATIC_EDGES 0

// Color palette, source:https://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define RED		"\x1B[31m"
#define BLUE	"\x1B[36m"
#define GREEN	"\x1B[32m"
#define DEF		"\x1B[0m"

// Global variables
int** Graph;
int Vertices;
int Edges;
double p;
int i;
int j;
int* Set;
clock_t start, end;
double cpu_time_used;

// Supporting Functions
void showGraph();
void Dikjstra(int** Graph, int source);
int getMin(int* dist);
int setEmpty();

int main(void)
{
    if(CLEAR_SCREEN){system("tput clear");}
    srand(time(NULL));

    if(FIXED_EXAMPLE){
        Vertices = 9;
        Edges = 18;

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
                Graph[i][j] = 99999;
            }
        }
        
        // Creating Example Map
        Graph[0][1] = 15;
        Graph[0][2] = 13;
        Graph[0][3] = 5;
        Graph[1][7] = 12;
        Graph[2][1] = 2;
        Graph[2][3] = 18;
        Graph[2][5] = 6;
        Graph[3][8] = 31;
        Graph[4][2] = 6;
        Graph[4][5] = 1;
        Graph[4][6] = 9;
        Graph[4][8] = 14;
        Graph[5][1] = 8;
        Graph[5][7] = 0;
        Graph[6][5] = 16;
        Graph[6][7] = 7;
        Graph[6][8] = 10;
        Graph[8][7] = 11;
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

        if(STATIC_EDGES)
        {
            // Asking for exact number of edges
            printf("Please enter the number of edges: ");
            do{
                scanf("%d", &Edges);\
                if(Edges < 0 || Edges < Vertices*(Vertices))
                {
                    printf("Please enter a valid number of edges: ");
                }
            }while(Edges < 0 || Edges < Vertices*Vertices);

        }
        else
        {
            // Asking for p value
            printf("Please enter the value of constant p: ");
            do{
                scanf("%lf", &p);\
                if(p <= 0 || p > 1)
                {
                    printf("Please enter a valid value for p: ");
                }
            }while(p <= 0 || p > 1);

            Edges = (int) (Vertices * Vertices * p) - (SELF_LOOPS_OFF * Vertices);
            if(DEBUG_VALUES){printf(GREEN "Edges == %d\n" DEF, Edges);}
        }


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
                Graph[i][j] = 99999;
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

                if(DEBUG_SPOT){printf(RED "Looking for random cordinates\n" DEF);}
            }while((x == y && SELF_LOOPS_OFF) || Graph[x][y] != 99999 );

            if(DEBUG){printf(GREEN "x == %d y == %d\n" DEF, x, y);}

            Graph[x][y] = rand() % COST_LIMIT;
        }

    }

    if(VIEW_GRAPH){showGraph();}
    
    // Setting up timer to measure execution time of Dikjstra. 
    start = clock();

    if(DEBUG_SPOT){printf(RED "Start of Dikstra\n" DEF);}

    Dikjstra(Graph, 0);

    if(DEBUG_SPOT){printf(RED "End of Dikstra\n" DEF);}

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Dijstra algorithm was successfully executed in %lf seconds\nfor %d vertices\nand %d edges.\n", cpu_time_used, Vertices, Edges);
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

void Dikjstra(int** Graph, int source)
{
    int v;

    if(DEBUG_SPOT){printf(RED "\nDEBUG 1\n" DEF);}

    // Initializing arrays and Set
    int* dist = (int*)malloc(sizeof(int)*Vertices);
    int* prev = (int*)malloc(sizeof(int)*Vertices);
    Set = (int*)malloc(sizeof(int)*Vertices);

    if(DEBUG_SPOT){printf(RED "\nDEBUG 2\n" DEF);}

    // ∞ is symbolized with 99999
    for(i = 0; i < Vertices; i++)
    {
        dist[i] = 99999;
        Set[i] = 1;
        prev[i] = -1;
    }
    prev[0] = 0;
    dist[source] = 0;

    if(DEBUG_SPOT){printf(RED "\nDEBUG 3\n" DEF);}


    while(!setEmpty()){
        int u = getMin(dist);

        if(DEBUG_VALUES){printf(BLUE "u = %d\n" DEF, u);}

        Set[u] = 0;

        for(v = 0; v < Vertices; v++)
        {
            if(DEBUG_VALUES){printf(BLUE "v = %d\n" DEF, v);}

            if(Graph[u][v] >= 0 && Set[v] != 0) //for every neighbor
            {
                int alt = dist[u] + Graph[u][v];

                if(DEBUG_VALUES){printf(BLUE "alt = %d\ndist[u]=%d + Graph[u][v]=%d;\n" DEF, alt, dist[u], Graph[u][v]);}
                if(DEBUG_VALUES){printf(BLUE "dist[v] = %d\n" DEF, dist[v]);}

                if(alt < dist[v])
                {
                    dist[v] = alt;
                    if(DEBUG_VALUES){printf(RED "dist[v] turned to = %d\n" DEF, dist[v]);}
                    prev[v] = u;
                }
            }
        }
    }

    if(DEBUG_SPOT){printf(RED "\nDEBUG 4\n" DEF);}


    if(DEBUG_RESULTS){
        printf(GREEN "\nFINAL RESULTS:\n\n");
        // return dist[] ?? print it instead 
        for(i = 0; i < Vertices; i++)
        {
            printf("dist[%d] = %d\n", i, dist[i]);
        }
        printf("\n");
        // return prev[] ?? print it instead 
        for(i = 0; i < Vertices; i++)
        {
            printf("prev[%d] = %d\n", i, prev[i]);
        }
        printf("\n" DEF);
    }

    printf("Starting from node 0:\n");
    for(i = 1; i < Vertices; i++)
    {
        if(dist[i] == 99999)
        {
            printf("There is no path for node %d.\n", i);
        }
        else
        {
            printf("We can go to node %d through node %d with cost %d.\n", i, prev[i], dist[i]);
        }
    }
    printf("\n");
     
}

int setEmpty(){
    for(i = 0; i < Vertices; i++)
    {
        if(Set[i] != 0)
        {
            return 0;
        }
    }
    return 1;
}

int getMin(int* dist)
{
    int min = 999999999;
    int indexMin = 999999999;
    int k;

    for(k = 0; k < Vertices; k++)
    {
        if(dist[k] < min && Set[k] != 0)
        {
            min = dist[k];
            indexMin = k;
        }
    }
    return indexMin;
}