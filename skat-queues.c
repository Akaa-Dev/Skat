// Exercise "bidding at a skat game"

// filename: "skat-queues.c"

/* compiler call: see makefile */

// Lessons to learn: how to deal with processes and queues.
 
// This module contains OS specific functions based on 
// processes, shared memory (shared) global variables.
// You don't need to know skat rules to understand this module...

#include <spawn.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
 
#include "glob-defs.h"
#include "skat-defs.h" 


// Create and init communication channels (queues + signal).
// Return TRUE, if successfull:

BOOL InitCommunication(void)
{
	return TRUE;
}


// Remove allcommunication channels:

void cleanup(void)
{
}


// Create child processes "Listener" and "Teller".
// Return TRUE if successful.

BOOL CreatePlayer(void)
{
	printf("Attention: empty function CreatePlayer() - no game!\n");
	return TRUE;
}


// Tell a bid and wait for opponent's answer.
// Return TRUE:  Opponent is keeping on.
// Return FALSE: Opponent is resigning:

BOOL giveBid(int nBid, struct tPlayer * pIch) 
{
	return FALSE;
}



// hear next bid. Return 0, if opponent had to resign:

int hearBid(struct tPlayer * pIch) 
{
	return 0;
}

int answerABid(BOOL bKeepUp, struct tPlayer * pIch)
{
	return 0;
}


// (Child processes:) Ask the currently inactive dealer to join the race and place higher bids:

void wakeUpDealer(struct tPlayer * pIch)
{
}


// (dealer process:) wait for the wakeup call:

void waitForWakeupCall(struct tPlayer * pIch)
{
}

