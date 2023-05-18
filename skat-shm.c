// Exercise "bidding at a skat game"

// filename: "skat-shm.c"

/* compiler call: see makefile */

// Lessons to learn: how to deal with processes and shared memory.
 
// This module contains OS specific functions based on 
// processes, shared memory (shared) global variables.
// You don't need to know skat rules to understand this module...

#include <spawn.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "glob-defs.h"
#include "skat-defs.h" 


// (parent process:) signal handler

void SigHandler(int nSigNo)
{
}

// init communication channels. Here: Shared memory.
// Return TRUE if successfull:

BOOL InitCommunication(void)
{
	return TRUE;
}

// Remove all communication channels:

void cleanup(void)
{
}


// Support function for the main()-Thread:
// Create, init and start the other players "Listener" and "Teller",
// start own work ("dealer").
// Return TRUE if successfull:

BOOL CreatePlayer(void)
{
	printf("Attention: empty function CreatePlayer() - no game!\n");
	return TRUE;
}


// Place a bid and wait for opponent's answer.
// Return TRUE:  Opponent is keeping up.
// Return FALSE: Opponent is resigning:

BOOL giveBid(int nBid, struct tPlayer * pIch) {
	return (FALSE);
}


// Hear next bid
// Return 0, if opponent is resigning:

int hearBid(struct tPlayer * pIch) {
	return (0);
}

// Answer a bid - handshake: see giveBid():

int answerABid(BOOL bKeepUp, struct tPlayer * pIch)
{
	return 0;
}


// Wake up the currently sleeping dealer and ask him to place a bid:

void wakeUpDealer(struct tPlayer * pIch)
{
}


// (dealer process:) wait for the wakeup call:

void waitForWakeupCall(struct tPlayer * pIch)
{
}
