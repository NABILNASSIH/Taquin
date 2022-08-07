#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <malloc.h>

#include "list.h"
#include "board.h"


list_t openList_p;
list_t closedList_p;


void showSolution( Item *goal )
{
  int i = 0, j;

  printf("\nSolution:");

  while (goal) {
    printBoard( goal );
		goal = goal->parent;
		i++;
  }

  printf("\nLength of the solution = %d\n", i-1);
  printf("Size of open list = %d\n", openList_p.numElements);
  printf("Size of closed list = %d\n", closedList_p.numElements);
  return;
}


void bfs( void )
{
  Item *cur_node, *child_p, *temp;
  int i;

	/* While items are on the open list */
	while ( listCount(&openList_p) ) 
	{ 
		/* Get the first item on the open list */
		cur_node = popFirst(&openList_p);

		/* Add it to the "visited" list */
        addLast( &closedList_p, cur_node );
      
    	/* Do we have a solution? */
    	if ( evaluateBoard(cur_node) == 0.0 ) 
    	{
      		showSolution(cur_node);
      		return;
    	} 
    	else 
    	{
      		/* Enumerate possible movements */
      		for(i = 0; i < 9; i++) 
      		{
        		child_p = getChildBoard( cur_node, i );

        		/* Ignore this child if already visited or invalid */
        		if (child_p != NULL && onList(&closedList_p, child_p->board) == NULL) 
        		{
                	/* Add child node to openList */
                	addLast( &openList_p, child_p );
            	}
        	}
      
    	}
  	}
  return;
}

void dfs( void )
{
    Item *cur_node, *child_p, *temp;
    int i;
    
    /* While items are on the open list */
    while ( listCount(&openList_p) ) 
    {        
        /* Get the first item on the open list */
        cur_node = popFirst(&openList_p);
        
        /* Add it to the "visited" list */
        addLast( &closedList_p, cur_node );

        /* Do we have a solution? */
        if ( evaluateBoard(cur_node) == 0.0 ) 
        {
            showSolution(cur_node);
            return;  
        } 
        else 
        {
            /* Enumerate possible movements */
            for(i = 0; i < 9; i++) 
            {
                child_p = getChildBoard( cur_node, i );
                
                /* Ignore this child if already visited or invalid */
                if (child_p != NULL && onList(&closedList_p, child_p->board) == NULL) 
                { 
                    /* Add child node to openList */
                    addFirst( &openList_p, child_p );  
                }
            }
        }
    }   
    return;
}

void ucs( void )
{
    Item *cur_node, *child_p, *temp;
    int i;
    
    cur_node = nodeAlloc();
    cur_node->f = 0.0 ;

    /* While items are on the open list */
    while ( listCount(&openList_p) ) 
    {         
        /* Get the first item on the open list */
        cur_node = popBest(&openList_p);

        /* Add it to the "visited" list */
        addLast( &closedList_p, cur_node );
        
        /* Do we have a solution? */
        if ( evaluateBoard(cur_node) == 0.0 ) 
        {
            showSolution(cur_node);
            return;    
        } 
        else 
        {
            /* Enumerate adjacent states */
            for(i = 0; i < 9; i++) 
            {
                child_p = getChildBoard( cur_node, i );
                
                /* It's a valid child */
                if (child_p != NULL) 
                { 
                    temp = onList(&openList_p, child_p->board) ;
                    
                    if(temp!=NULL && temp->f > child_p->f)
                        delList(&openList_p, temp) ;
                    
                    /* Ignore this child if already visited */
                    if (onList(&closedList_p, child_p->board) == NULL)
                    {
                        /* Add child node to openList */
                        addLast( &openList_p, child_p );
                    }
                }
            }
        }
    }
    return;
}


void astar( void )
{
    Item *cur_node, *child_p, *temp;
    int i;
    
    cur_node = nodeAlloc();
    while ( listCount(&openList_p) ) { /* While items are on the open list */
        
        /* Get the first item on the open list */
        cur_node = popBest(&openList_p);
        
        //printf("%d  %f\n", listCount(&openList_p), evaluateBoard( cur_node ));
        
        /* Add it to the "visited" list */
        addLast( &closedList_p, cur_node );
        
        /* Do we have a solution? */
        if ( evaluateBoard(cur_node) == 0.0 ) {
            showSolution(cur_node);
            return;
            
        } else {
            /* Enumerate possible movements */
            for(i = 0; i < 9; i++) {
                child_p = getChildBoard( cur_node, i );
                
                if (child_p != NULL) { // it's a valid child!
                    /* Ignore this child if already visited */
                    if (onList(&closedList_p, child_p->board) == NULL){
                        temp = onList(&openList_p, child_p->board) ;
                    
                        /* Si le cout de temp est superieur au cout de childp */
                        if(temp!=NULL && temp->f > child_p->f)
                            delList(&openList_p, temp) ;
                    
                        /* Add child node to openList */
                        addLast( &openList_p, child_p );
                    }
                }
            }
        }
    }
    
    return;
}

int CHOIX(int argc, char* argv[])
{
	int level = atoi(argv[2]);

	if(argc!=3)
	{
		printf("Erreur la ligne de commande doit être de la forme suivante :\n");
		printf("./taquin <mode> <level>\n");
		return 1;
	}

	else if( strcmp(argv[1],"bfs")!=0 && 
  		strcmp(argv[1],"dfs")!=0 &&
  		strcmp(argv[1],"ucs")!=0 &&
  		strcmp(argv[1],"astar")!=0 )
  	{
  		printf("Invalid execution mode\n");
  		printf("Valid modes: bfs dfs ucs astar\n");
  		return 1;
  	}

  	else if(level<1 || level>3)
  	{
  		printf("Invalid level\n");
  		printf("Valid levels: 1 2 3\n");
  		return 1;
  	}

  	// All arguments are valid
  	return 0;
} 	



int main(int argc, char* argv[])
{      
	if(CHOIX(argc,argv)!=0)
		exit(1);

  	/* Init lists */
  	initList(&openList_p);
  	initList(&closedList_p);

	printf("\nInitial:");
	Item *initial_state = initGame(atoi(argv[2]));
	printBoard( initial_state );

	printf("\nSearching ...\n");
	addLast( &openList_p, initial_state );

    if(strcmp(argv[1],"bfs")==0)
    	bfs() ;
    else if(strcmp(argv[1],"dfs")==0)
    	dfs();
    else if(strcmp(argv[1],"ucs")==0)
    	ucs();
    else if(strcmp(argv[1],"astar")==0)
    	astar();  
    
	printf("Finished!\n");

	/* clean lists */
	cleanupList( &openList_p );
    cleanupList( &closedList_p );

  	return 0;
}
