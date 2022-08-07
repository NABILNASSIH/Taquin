#include <stdio.h>
//#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "board.h"




int cur_blank, new_blank;

int findCase(char *board, int num_case)
{
    int i;
    
    for(i = 0; i<MAX_BOARD; i++)
        if(board[i] == num_case)
            return i ;
    
    return -1 ;
}

// Initialize a new Game for the nQueens problem: an empty board..
Item *initGame(int level)
{
    int i;
    Item *node;
    char easy[MAX_BOARD] = {1,2,5,3,0,4,7,8,6};
    char medium[MAX_BOARD] = {7,4,8,2,5,6,3,1,0};
    char difficult[MAX_BOARD] = {8,0,7,5,6,1,3,2,4};

    node = nodeAlloc();
	
    if(level==1)
        initBoard(node, easy);
    else if(level==2)
        initBoard(node, medium) ;
    else if(level==3)
        initBoard(node, difficult) ;
    
    node->blank=findCase(node->board,0);
    node->depth = 0;

    return node;
}

// print a board
void printBoard( Item *node )
{
  int i, j;
  assert(node);
	printf("\n");
	for (j=0 ; j<WH_BOARD; j++) 
    if (j==0) 
      printf(" ___");
    else 
      printf("____");
  printf("\n");
	
  for (i=0 ; i < MAX_BOARD ; i++) {
    if (i%WH_BOARD == 0) 
      printf("|");
    if (node->board[i] == 0) 
      printf("   |");
    else 
      printf("%2d |", node->board[i]);
    if (((i+1)%WH_BOARD) == 0) {
			printf("\n");
			for (j=0; j<WH_BOARD; j++) 
        if (j==0) 
          printf(" ___"); 
        else 
          printf("____"); 
        printf("\n");
		}
  }
	printf("\n");
}


// initialize node's state from a given board
void initBoard(Item *node, char *board) {
	assert( node );
	int i ;
	node->size = MAX_BOARD;
    node->board = calloc(MAX_BOARD, sizeof(char));
  
	/* Copy board */
    for (i = 0; i<MAX_BOARD; i++){
        node->board[i] = board[i];
    }
}





// Return 0 if all numbers are placed. Positive otherwise
// Ie: nb numbers that still need to be placed.
// Equivalent de getSimpleHeuristic
double evaluateBoard(Item *node) {
    char target[MAX_BOARD] = {1,2,3,4,5,6,7,8,0};
    int i, compteur = 0;
    
    for(i=0;i<MAX_BOARD;i++)
        if(node->board[i] != target[i])
            compteur++;
    
    return compteur ;
}



double getManhattanHeuristic(Item *node)
{
    int i_case,i_sol,j_case,j_sol,num_case,pos,pos_finale;
    double value = 0;
        for(num_case=0; num_case<MAX_BOARD; num_case++)
    {
        pos = findCase(node->board,num_case) ;
        pos_finale = (num_case-1)%MAX_BOARD ;
        if(pos != pos_finale)
        {
            i_case = pos / WH_BOARD ;
            j_case = pos % WH_BOARD ;
            i_sol = pos_finale / WH_BOARD ;
            j_sol = pos_finale % WH_BOARD ; 
            value = value + abs(i_case-i_sol) + abs(j_case-j_sol) ;
        }
    }
    

    return 0 ;
}

// Test if position pos is valid with respect to node's state

int isValidPosition( Item *node, int pos )
{
	 int i, j, ii, jj ;
    cur_blank = node->blank ;
    ii = cur_blank / WH_BOARD;
    jj = cur_blank % WH_BOARD;
	i= pos /WH_BOARD;
    j= pos % WH_BOARD;
 
    if((abs(ii-i) +abs( jj-j))==1) return 1;     // POSITION VALIDE
	 
	 return 0; // POSITION INVALIDE
}



// Return a new item where a new queen is added at position pos if possible. NULL if not valid
Item *getChildBoard( Item *node, int num_pos )
{
    Item *child_p = NULL;
    
	if ( isValidPosition(node, num_pos) == 1 )
		
    {    
	    new_blank = num_pos ;
        /* allocate and init child node */
        child_p = nodeAlloc() ;
        initBoard(child_p, node->board) ;

		/* Make move */
        child_p->board[cur_blank] = child_p->board[new_blank];
        child_p->board[new_blank] = 0;
        child_p->blank=new_blank;
        //printBoard(child_p) ;
        
		/* Link child to parent for backtrack */
        child_p->parent = node;
        
        /* Value of depth */
        child_p->depth = node->depth +1 ;
        
        /* Value of cost */
        child_p->g = child_p->depth ;
        child_p->h = getManhattanHeuristic(child_p) ; 
        child_p->f = child_p->h + child_p->g ;
        
    }
  return child_p;
}



