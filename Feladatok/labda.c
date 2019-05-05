#include<stdio.h>
#include<unistd.h>


void DrawMap(int r, int c, int cr, int cc)
{

    for (int i = 0; i<(r+2); i++)
    {

        for (int j = 0; j < (c+2); j++)
        {

            if (i == (cr+1) && j == (cc+1))
            {
                printf("o");
            }
            else
            {
                printf(" ");
            }

            if (i == 0 || j == 0 || i == (r+1) || j == (c+1) )
            {
                printf(".");
            }
            
        }
        printf("\n");
    }

}



int main()
{

    int row;
    int coloumn;

    printf("number of rows -> "); scanf("%d",&row); printf("\n");
    printf("number of coloumns -> "); scanf("%d",&coloumn); printf("\n");

    int yn = 1;
    

    int CurrentRow = row /2;
    int CurrentColoumn = coloumn/2;
    int prevRow = CurrentRow - 1;
    int prevCol = CurrentColoumn - 1;


    while (1)
    {

    DrawMap(row, coloumn, CurrentRow,CurrentColoumn);

    sleep(1);

    if ( CurrentRow == 0)
    {
        prevRow = -1;
    }
    if (CurrentRow == row-1)
    {
        prevRow = row+1;
    }
    if (CurrentColoumn == 0)
    {
        prevCol = -1;
    }
    if (CurrentColoumn == coloumn-1)
    {
        prevCol = coloumn+1;
    }



    if (CurrentRow < prevRow)
    {
        CurrentRow--;
    }
    else { CurrentRow++; }
    if (CurrentColoumn < prevCol)
    {
        CurrentColoumn--;
    } else {CurrentColoumn++;}


    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    }

    





    return 0;
}