// Ian Scheetz
// 11/22/24

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

struct process
{
    int alloc[3];
    int max[3];
    int need[3];
};

void CalculateNeed(struct process* p)
{
    for(int b = 0; b < 3; b++)
    {
        p->need[b] = p->max[b] - p->alloc[b];
    }
}

//less than or equal
bool LTOE(int p1[3], int p2[3])
{
    for(int d = 0; d < 3; d++)
    {
        if(p1[d] > p2[d])
        {
            return false;
        }
    }
    return true;
}

int main()
{
    int available[3] = {3,3,2};
    struct process p[5] = {{{0,1,0},{7,5,3},{0,0,0}},
                        {{2,0,0},{3,2,2},{0,0,0}},
                        {{3,0,2},{9,0,2},{0,0,0}},
                        {{2,1,1},{2,2,2},{0,0,0}},
                        {{0,0,2},{4,3,3},{0,0,0}}
                        };

    //calculate need for each process
    for(int a = 0; a < 5; a++)
    {
        CalculateNeed(&p[a]);
    }
    
    int work[3];
    work[0]= available[0];
    work[1] = available[1];
    work[2] = available[2];
    int finished[5] = {0,0,0,0,0};
    int seq[5];
    
    int seqI = 0;
    while (seqI < 5)
    {
        bool next = false;
        for(int e = 0; e < 5; e++)
        {
            
            // check if process is unfinished
            if(finished[e] == 0)
            {
                if(LTOE(p[e].need, work))
                {
                    // if unfinished and need is less than work
                    // free allocated resources and set process as finished 
                    seq[seqI++] = e;
                    
                    finished[e] = 1;
                    for(int f = 0; f < 3; f++)
                    {
                        work[f] += p[e].alloc[f];
                    }
                    next = true;
                }
                
            }
        }
        if (!next)
        {
            printf("System in unsafe state.");
            break;
        }
    } 
    {
        printf("Safe sequence is: ");
            for(int g = 0; g < 5; g++)
            {
                printf("P");
                printf("%d", seq[g]);
                printf("\n");
            }
    }
}
