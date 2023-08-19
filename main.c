// Amaan Shah, UIC, Spring 2021
// Project 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE   1
#define FALSE  0

// Node:
struct nodeStruct
{
 char data;
 int x;
 int y;
 struct nodeStruct*  next;
}; 
typedef struct nodeStruct node;
typedef node* nodePtr; 

// Maze:
typedef struct mazeStruct
{
	char** arr;
 	int xsize, ysize;
 	int xstart, ystart;
 	int xend, yend;
} maze;

// Prototypes:
void dump(node** head);
void push(node** head, int xpos, int ypos, maze *m, int *coins, int DBM);
void init(node** head, int xstart, int ystart, maze *m, int DBM);
int is_empty(node* head);
node* top(node* head);
void pop(node** head, int DBM);
char checkNeighbor(node** head, maze *m);
void clear(node** head, int DBM);
void reverseStack(node** head1, node** head2);


int main (int argc, char **argv)
{
	maze m1;
	int xpos, ypos;
	int i,j;
	char c;
	int numFiles = 0;
	int coins = 0;

	FILE *src;

	// Verify the proper number of command line arguments:
	if(argc > 3) {
	    printf("Usage: %s <input file name>\n", argv[0]);
	    exit(-1);
	}

	// Debug Mode:
    int DBM = FALSE;
    for (int i = 0; i < argc; i++) {
    	if (strcmp(argv[i],"-d") == 0) {
    		DBM = TRUE;
    	}
    }

	// Check if there are multiple valid input files:
	for (int i = 1; i < argc; i++) {
	  	if ( (src = fopen (argv[i], "r")) != NULL) {
	  		numFiles++;
	  		fclose(src);
	  	}
	}
	if (numFiles > 1) {
	  	printf("Too many input files.\n");
	  	exit(-1);
	}

	// Try to open the input file:
	if ( ( src = fopen( argv[1], "r" )) == NULL )
	{
	    printf ( "Can't open input file: %s", argv[1] );
	    exit(-1);
    }

  	// Read in data:
  	int count = 0;
  	int scan = 0;
	scan = fscanf (src, "%d %d", &m1.xsize, &m1.ysize);
	if (scan == 2) {
		count++;
	}
  	scan = fscanf (src, "%d %d", &m1.xstart, &m1.ystart);
  	if (scan == 2) {
  		count++;
  	}
  	scan = fscanf (src, "%d %d", &m1.xend, &m1.yend);
  	if (scan == 2) {
  		count++;
  	}

  	// If data is invalid, print errors:
	if (count < 3) {
	  	printf("Invalid data file.\n");
	  	exit(-1);
	} else if (m1.xsize <= 0 || m1.ysize <= 0) {
		 printf("Maze sizes must be greater than 0.\n");
		 exit(-1);
	} 


	printf ("size: %d, %d\n", m1.xsize, m1.ysize);
	if (m1.xstart < 0 || m1.xstart > m1.xsize) {
	  	printf("Start/End position outside of maze range.\n");
	  	exit(-1);
	} else if (m1.ystart < 0 || m1.ystart > m1.ysize) {
	  	printf("Start/End position outside of maze range.\n");
	  	exit(-1);
	} else {
		printf ("start: %d, %d\n", m1.xstart, m1.ystart);
	}

	if (m1.xend < 0 || m1.xend > m1.xsize) {
	  	printf("Start/End position outside of maze range.\n");
	  	exit(-1);
	} else if (m1.yend < 0 || m1.yend > m1.ysize) {
	  	printf("Start/End position outside of maze range.\n");
	  	exit(-1);
	} else {
		printf ("end: %d, %d\n", m1.xend, m1.yend);
	}

	// Allocate Dynamic Maze:
	int col = m1.ysize + 2;
	m1.arr = (char**) malloc((m1.xsize + 2) * sizeof(char*));
	for (int i = 0; i < (m1.xsize + 2); i++) {
		m1.arr[i] = (char*) malloc((m1.ysize + 2) * sizeof(char));
	}

  	// Initialize Maze:
  	for (i = 0; i < m1.xsize+2; i++)
     	for (j = 0; j < m1.ysize+2; j++)
       	m1.arr[i][j] = '.';

  	// Create Border for Maze:
  	for (i=0; i < m1.xsize+2; i++) {
  		m1.arr[i][0] = '*';
     	m1.arr[i][m1.ysize+1] = '*';
    }
  	for (i=0; i < m1.ysize+2; i++) {
    	m1.arr[0][i] = '*';
    	m1.arr[m1.xsize+1][i] = '*';
    }

  	// Mark Start and End:
  	m1.arr[m1.xstart][m1.ystart] = 's';
  	m1.arr[m1.xend][m1.yend] = 'e';

  	// Read in Data from File:
 	while (fscanf(src, "%d %d %c", &xpos, &ypos, &c) != EOF) {
 		int valid = TRUE;
 		if (xpos < 0 || xpos > m1.xsize) {
 			printf("Invalid coordinates: outside of maze range.\n");
 			valid = FALSE;
 		} else if (ypos < 0 || ypos > m1.ysize) {
 			printf("Invalid coordinates: outside of maze range.\n");
 			valid = FALSE;
 		} else if ( (xpos == m1.xstart && ypos == m1.ystart) || (xpos == m1.xend && ypos == m1.yend)) {
 			printf("Invalid coordinates: attempting to block start/end position.\n");
 			valid = FALSE;
 		}

 		if (valid == TRUE) {
	 		// check type
	 		if (c == 'b' || c == 'c') {
		 		if (c == 'b') {
			    	m1.arr[xpos][ypos] = '*';
			    } else if (c == 'c') {
			    	m1.arr[xpos][ypos] = 'C';
			    }
	 		}else {
	 			printf("Invalid type: type is not recognized.\n");
	 		}  
 		}
  	}

   fclose(src);

  /* print out the initial maze */
  for (i = 0; i < m1.xsize+2; i++)
    {
     for (j = 0; j < m1.ysize+2; j++)
       printf ("%c", m1.arr[i][j]);
     printf("\n");
    }


    // Create head and init stack with the starting coordinates:
    node* stack = NULL;
    node* revStack = NULL;
    init(&stack, m1.xstart, m1.ystart, &m1, DBM);

    // Navigate the Maze:
    while (is_empty(stack) == 0) {
    	node* t = top(stack);
    	int curX = t->x;
    	int curY = t->y;

    	if(curX == m1.xend && curY == m1.yend) { // at the end position
    		break;
    	} 

    	char next = checkNeighbor(&stack, &m1);
    	if (next == 'd') {
    		push(&stack, curX + 1, curY, &m1, &coins, DBM);
    	} else if (next == 'r') {
    		push(&stack, curX, curY + 1, &m1, &coins, DBM);
    	} else if (next == 'u') {
    		push(&stack, curX - 1, curY, &m1, &coins, DBM);
    	} else if (next == 'l') {
    		push(&stack, curX, curY - 1, &m1, &coins, DBM);
    	} else { //pop the top 
    		pop(&stack, DBM);
    	}
    }

    printf("\n");
	// stack is empty
    if (is_empty(stack) == 1) { 					
    	printf("This maze has no solution.\n");

    }
    else {
    	printf("The maze has a solution.\n");
    	printf("The amount of coins collected: %d\n", coins);
    	printf("The path from start to end:\n");
    	reverseStack(&stack, &revStack);
    	dump(&revStack);
    }

    clear(&stack, DBM);
    DBM = FALSE;
    clear(&revStack, DBM);

    // Deallocate Maze:
   	for (int i = 0; i < m1.xsize+2; i++) {
   		free(m1.arr[i]);
   	}
   	free(m1.arr);
   	return 0;
}

void dump(node** head) {
	node* temp = *head;
    while (temp != NULL) {
    	printf("(%d,%d) ", temp->x, temp->y);
    	temp = temp->next;
    }
    printf("\n");
}

void init(node** head, int xstart, int ystart, maze *m, int DBM) {
	node* cur = (node*) malloc(sizeof(node));
	cur->data = 'v';
	m->arr[xstart][ystart] = 'v';
	cur->x = xstart;
	cur->y = ystart;

	cur->next = *head;
	*head = cur;

	if (DBM == TRUE) {
		printf("(%d,%d) pushed into the stack.\n", xstart, ystart);
	}
}

void push(node** head, int xpos, int ypos, maze *m, int *coins, int DBM) {
	if (m->arr[xpos][ypos] == 'C') {
		*coins = *coins + 1;
	}
	node* cur = (node*) malloc(sizeof(node));
	cur->data = 'v';
	m->arr[xpos][ypos] = 'v';
	cur->x = xpos;
	cur->y = ypos;

	cur->next = *head;
	*head = cur;
	if (DBM == TRUE) {
		printf("(%d,%d) pushed into the stack.\n", xpos, ypos);
	}
}

int is_empty(node* head) {
	if (head == NULL) {
		return TRUE;
	} else {
		return FALSE;
	}
}

node* top(node* head) {
	return head;
}

void pop(node** head, int DBM) {
	node* temp = *head;
	if (temp != NULL) {
		*head = temp->next;
		if (DBM == TRUE) {
			printf("(%d,%d) popped off the stack.\n", temp->x, temp->y);
		}
		free(temp);
	}

}

char checkNeighbor(node** head, maze *m) {
	node* cur = *head;
	char next;
	int x = cur->x;
	int y = cur->y;

	if (m->arr[x + 1][y] != '*' && m->arr[x + 1][y] != 'v') { // down
		next = 'd';
		return next;
	} 
	else if (m->arr[x][y + 1] != '*' && m->arr[x][y + 1] != 'v') { // right
		next = 'r';
		return next;
	}
	else if (m->arr[x - 1][y] != '*' && m->arr[x - 1][y] != 'v') { // up
		next = 'u';
		return next;
	}
	else if (m->arr[x][y - 1] != '*' && m->arr[x][y - 1] != 'v') { // left
		next = 'l';
		return next;
	}
	else { //else -> false
		next = 'q';
		return next;
	}
}

void clear(node** head, int DBM) {
	node* cur = *head;
	node* next = NULL;

	while (cur != NULL) {
		next = cur->next;
		if (DBM == TRUE) {
			printf("(%d,%d) popped off the stack.\n", cur->x, cur->y);
		}

		free(cur);
		cur = next;
	}
	*head = NULL;
}

void reverseStack(node** head1, node** head2) {
	node* revtmp = *head1;
	while (revtmp != NULL) {
		node* tmp = (node*) malloc(sizeof(node));
		tmp->data = revtmp->data;
		tmp->x = revtmp->x;
		tmp->y = revtmp->y;
		tmp->next = *head2;
		*head2 = tmp;
		revtmp = revtmp->next;
	}
}