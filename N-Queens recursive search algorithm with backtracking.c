/******************************************************************************/
/******************************* prologue *************************************/
/*                                                                            */
/*                                                                            */
/*                       Advanced C Programming                               */
/*                                                                            */
/*              Author: Ronald Inselberg                                      */
/*                                                                            */
/*              Topic: N-Queens recursion with backtracking                   */
/*                                                                            */
/*              Date: February 24, 2014                                       */
/*                                                                            */
/*              Objective: Recursive search algorithm with backtracking       */
/*                                                                            */
/*              Comments:                                                     */
/*                                                                            */
/*              I reused Queue (FIFO) from previous assignment, as a way to   */
/*              parse and store all the user inputs before starting the	      */
/*              loop that calls the N-Queens algorithm for any/all            */
/*              queen #1 column numbers that are specified by the user.       */
/*                                                                            */
/*				      The user is prompted to enter all input data from the         */
/*              console interface.                                            */
/*                                                                            */
/*              The user is required to enter however many queen #1 column    */
/*              numbers as a comma(,) or blank separated list of strings,     */
/*              until the string "Q" is encountered, which terminates user    */
/*              input. If "Q" is not encountered before EOL or \n, the        */
/*              program requests more line(s) of input, until a "Q" string    */
/*              is encountered. Strings that are > 1 char or strings that     */
/*				      do not convert to a valid column number (0 to BOARDSIZE-1)    */
/*              are ignored. All input data is recorded in the output file.   */
/*                                                                            */
/******************************************************************************/

// include files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// preprocessor directives for queue implementation
#define MAXQUEUE 100	// size of FIFO (queue)
#define INITIALIZEQ -1	// value used to initialize queue.front and queue.back


// preprocessor directives for N-Queens algorithm
#define BOARDSIZE 8
#define DIAGONAL ((2 * BOARDSIZE) - 1)
#define DOWNOFFSET 7
#define LOG_TO_OUTPUT_FILE true
#define CONSOLE_ONLY false


// global declarations for queue (FIFO), each element is int
typedef struct queue {
	int front, back;
	int queueElement[MAXQUEUE];
} QUEUE;


// global declarations for N-Queens algorithm
int queenCol[BOARDSIZE];
bool colFree[BOARDSIZE];
bool upFree[DIAGONAL];
bool downFree[DIAGONAL];
int queenCount = -1;


// output file must be global
FILE *outputFile;


// prototype function declarations follow

// prototype function declarations for generic processing requirements
void myExit(int, bool);
void logToConsoleAndOutputFile(char *, char *);

// prototype function declarations for queue implementation
bool queueIsEmpty(QUEUE *);
int deQueue(QUEUE *);
void enQueue(QUEUE *, int);

// prototype function declarations for N-Queens algorithm
void clearBoard();
void writeBoard();
void addQueen();


// all function definitions follow

// log exit status and terminate
void myExit(int exitStatus, bool logToOutputFile) {
	const char * const programTerminationMsg = "terminating with exit status %i\n";
	printf(programTerminationMsg, exitStatus);
	if (logToOutputFile) {
		fprintf(outputFile, programTerminationMsg, exitStatus);
	}
	exit(exitStatus);

}

// log to console and output file
void logToConsoleAndOutputFile(char *msgString, char *outputString) {
	if (outputString == NULL) {
		printf(msgString);
		fprintf(outputFile, msgString);
	} else {
		printf(msgString, outputString);
		fprintf(outputFile, msgString, outputString);
	}
}

// test for queue empty condition
bool queueIsEmpty(QUEUE *pointerToQueue) {
	if (pointerToQueue->front == pointerToQueue->back) {
		return true;
	} else {
		return false;
	}
}

// remove element from front of queue and return it as int
int deQueue(QUEUE *pointerToQueue) {
	if (queueIsEmpty(pointerToQueue)) {
		logToConsoleAndOutputFile("queue underflow \n", NULL);
		myExit(-1, LOG_TO_OUTPUT_FILE);

	}
	if (pointerToQueue->front == MAXQUEUE-1) {
		pointerToQueue->front = 0;
	} else {
		(pointerToQueue->front)++;
	}
	return pointerToQueue->queueElement[pointerToQueue->front];
}

// add int as a new element at the back of the queue
void enQueue(QUEUE *pointerToQueue, int i) {
	if (pointerToQueue->back == MAXQUEUE-1) {
		pointerToQueue->back = 0;
	} else {
		(pointerToQueue->back)++;
	}
	if (pointerToQueue->front == pointerToQueue->back) {
		printf("queue overflow \n");
		fprintf(outputFile, "queue overflow \n");
		exit(-1);

	}
	pointerToQueue->queueElement[pointerToQueue->back] = i;
	return;
}

// initialization for N-Queens algorithm
void clearBoard(void) {
	for (int i = 0; i < BOARDSIZE; i++) {
		colFree[i] = true;
		queenCol[i] = -1;
	}
	for (int j = 0; j < DIAGONAL; j++) {
		upFree[j] = downFree[j] = true;
	}
	queenCount = -1;
}

// output for N-Queens solution
void writeBoard(void) {
	static int qCount = 0;
	for (int col = 0; col < BOARDSIZE; col++) {
		if (queenCol[qCount] == col) {
			if (qCount == 0) {
				fprintf(outputFile, "U  ");		// queen #1 is "U" because user positioned it
			} else {
				fprintf(outputFile, "Q  ");
			}
		} else {
			fprintf(outputFile, "*  ");
		}
	}
	fprintf(outputFile, "\n");
	if (qCount++ < BOARDSIZE - 1) {
		writeBoard();
	}
	qCount = 0;
}

// N-Queens algorithm, position next queen, using backtracking to arrive at a solution
void addQueen(void) {
	queenCount++;	// keeps track of which queen, which queen determines which row
	// for the given row, look for an open square, column by column, starting with leftmost column (0)
	for (int col= 0; col < BOARDSIZE; col++) {
		if (colFree[col] && upFree[queenCount + col] && downFree[queenCount - col + DOWNOFFSET]) {
			queenCol[queenCount] = col;							// found an open square, position queen "here"
			colFree[col] = false;								// queen attacks all squares in this column
			upFree[queenCount + col] = false;					// attacks all squares on upper diagonal
			downFree[queenCount - col + DOWNOFFSET] = false;	// attacks all squares on lower diagonal
			if (queenCount == BOARDSIZE - 1) {
				// all queens positioned, have arrived at solution
				// calls to write solution to output file
				fprintf(outputFile, "%d-Queens solution for queen #1 in column %d\n\n", BOARDSIZE, queenCol[0]);
				for (int j = 0; j < BOARDSIZE; j++) {
					fprintf(outputFile, "Q%d->%d  ", j+1, queenCol[j]);
				}
				fprintf(outputFile, "\n\n");
				writeBoard();
				fprintf(outputFile, "\n\n");
			} else {
				// try to position the next queen
				addQueen();
			}
			if (queenCount < 1) {
				// backtracked all the way to queen #1, cannot remove this queen
				// but this never happens!
				fprintf(outputFile, "cannot find solution for queen #1 in column %d\n\n", queenCol[0]);
				continue;
			} else {
				// cannot find an open square for next queen
				// need to backtrack by removing this queen
				colFree[col] = true;
				queenCount--;
				upFree[queenCount + col] = true;
				downFree[queenCount - col + DOWNOFFSET] = true;
			}
		}
	}
}

// function main begins program execution
int main(void) {

	// initialize the queue that holds the user's column number inputs to "empty"
	QUEUE q, *storeInputs;
	q.front = q.back = INITIALIZEQ;
	storeInputs = &q;

	#define BUFFERSIZE 1024				// set input buffer size for 1024 chars (max)
	char stringIn[BUFFERSIZE];				// buffer for all chars entered until EOL or "return" key

	bool userRT;						// encountered user requested termination string 'Q'
	bool invalidInput;					// encountered invalid input string
	char *token;						// used to parse input buffer into comma(,) or blank separated strings
	int intValue;						// int value when input string is converted to column number
	int nStrings;						// keeps track of how many strings are converted

	// declare pointers to predefined text messages, avoids duplication of strings
	const char * const outputFileOpenMsg = "outputFile.txt opened for write access\n";
	const char * const outputFileCloseMsg = "outputFile.txt was closed without error\n";
	const char * const outputFileCloseErrorMsg = "file close error on outputFile.txt\n";
	const char * const invalidInputMsg = "encountered unexpected user input from console\n";
	const char * const userTerminationRequest = "encountered user termination request from console\n";
	const char * const queueRequest = "queueing request for 8-Queens with queen #1 in column %s\n";

	// dynamic execution begins here, start by opening output file
	if ((outputFile  = fopen("outputFile.txt", "w"))==NULL) {
		printf ("outputFile.txt could not be opened with write access\n");
		myExit(-1, CONSOLE_ONLY);

	}
	// log to console and output file
	logToConsoleAndOutputFile(outputFileOpenMsg, NULL);

	userRT = false;
	nStrings  = 0;

	logToConsoleAndOutputFile("program accepts comma(,) or blank separated list of column numbers (0-7), Q to terminate\n", NULL);

	while (true) {
		logToConsoleAndOutputFile("\nrequesting input from user\n", NULL);
		fgets(stringIn, BUFFERSIZE, stdin);
		logToConsoleAndOutputFile("parsing input data -> %s", stringIn);
		token = strtok(stringIn, " ,\n");
		while (token != NULL) {
			invalidInput = false;
			if (strcmp(token,"Q") == 0) {
				userRT = true;					// encountered user termination request Q
				break;							// stop parsing input buffer for more strings

			}
			if (strlen(token) != 1) {			// ignore any string that is > 1 char
				invalidInput = true;
			}
			intValue = token[0] - '0';
			if (intValue < 0 || intValue > BOARDSIZE -1) {
				invalidInput = true;			// ignore any string that does not convert to a valid column number
			}
			logToConsoleAndOutputFile("\nprocessing string from input data -> %s\n", token);
			if (invalidInput) {
				logToConsoleAndOutputFile(invalidInputMsg, NULL);
			} else {
				logToConsoleAndOutputFile(queueRequest, token);
				enQueue(storeInputs, intValue);	// add queen #1 column number to queue
				nStrings++;						// keep track of how many in queue
			}
			token = strtok(NULL, " ,\n");		// next string from input buffer
		}
		if (userRT) {
			break;								// already got termination request Q, do not get another line of input

		}
		printf("termination request 'Q' not received, waiting for more input:\n");
	}	// loop to read next line of input from console

	// finished parsing input data from user, now process any/all queen #1 column numbers in queue
	fprintf(outputFile, "\n\n");
	for (int i = 1; i <= nStrings; i++) {
		int numOut;
		// set up N-Queen initial conditions
		clearBoard();
		// ready to position queen #1
		queenCount = 0;
		// get queen #1 column number from queue
		queenCol[queenCount] = (numOut = deQueue(storeInputs));
		// "close" all squares that are attacked by queen #1
		colFree[numOut] = false;								// close squares in all other rows on queen #1 column
		upFree[queenCount + numOut] = false;					// close squares on upper diagonal from queen #1
		downFree[queenCount - numOut + DOWNOFFSET] = false;		// close squares on lower diagonal from queen #1
		addQueen();												// starts backtracking algorithm
	}
	logToConsoleAndOutputFile(userTerminationRequest, NULL);

	// close output file and log file status
	if (fclose(outputFile) != 0) {
		printf("%s", outputFileCloseErrorMsg);
		myExit(-1, CONSOLE_ONLY);

	} else {
		printf("%s", outputFileCloseMsg);
	}
	// log out message for user and terminate as requested
	myExit(0, CONSOLE_ONLY);
}

