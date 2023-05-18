// Exercise "bidding at a skat game"

// filename: "skat-comm.h"

// compiler call: see makefile

// This header file contains function prototypes. 
// to be implemented in the OS specific modules (e. g. skat-pipes.c),
// depending on the selected communication methods (e. g. pipes).


// Create and init communication channels.
// Return TRUE, if successfull:
BOOL InitCommunication(void);  

// Close and remove communication channels:
void cleanup(void);

// This funktion is used by the parent process/thread (dealer) to
// create and start the other two players ("listener" and "teller".
// Retrun TRUE, if successfull:
BOOL CreatePlayer(void);

// Hear a Bid (return value: the bid):
int hearBid(struct tPlayer * pIch);

// Answer a bid:
int answerABid(BOOL bKeepUp, struct tPlayer * pIch);

// wake up the waiting dealer:
void wakeUpDealer(struct tPlayer * pIch);


