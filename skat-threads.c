// Exercise "bidding at a skat game"

// filename: "skat-threads.c"

/* compiler call: see makefile */

// Lessons to learn: how to deal with threads, handshake communication.

// This module contains OS specific functions based on threads and global variables.
// You don't need to know skat rules to understand this module...

#include <spawn.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h> // required for Ubuntu as a Windows 10 subsystem
#include <string.h>


#include "glob-defs.h"
#include "skat-defs.h"
#include "skat-comm.h"

int g_Bid = 0;	  // current bid - this global variable may be used by all players
int g_accept = 0; // accept-flag for handshake
int g_valid = 0;  // valid-flag for handshake
pthread_t p1;
pthread_t p2;
pthread_t p3;
pthread_cond_t waker = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;

// Thread function (Listener or Teller):
// Insert your code here!!!
// ...

void *play(void *arg)
{
	int pos;
	pos = (int *)arg;
	struct tPlayer player;
	if (pos < 3)
	{
		player.nMaxBid = skatPlayers[pos].nMaxBid;
		player.nState = skatPlayers[pos].nState;
		player.pcTask = skatPlayers[pos].pcTask;
		skatPlayers[pos] = player;
		printf("Player created in , player max bid = %d \n", skatPlayers[pos].nMaxBid);
	}
	// pthread_t tid;
	printf("Player created in thread in: %d\n", pos);
	bidding(&skatPlayers[pos]);
}

// init communication channels. Here: Simple variables
// Return TRUE if successfull:

BOOL InitCommunication(void)
{
	g_Bid = 0;
	g_accept = 0;
	g_valid = 0;
	return TRUE;
}

// Remove all communication channels:

void cleanup(void)
{

	// is there anything to remove?
}

// Support function for the main()-Thread:
// Create, init and start the other players "Listener" and "Teller",
// start own work ("dealer").
// Return TRUE if successfull:

BOOL CreatePlayer(void)
{
	// printf("Attention: empty function CreatePlayer() - no game! Output will be nonsense!\n");

	// Create 2 player-threads "Listener" and "Teller".
	// The corresponding thread function should start bidding immediately:
	// Insert your code here!!!
	// ...
	struct tPlayer dealer;
	dealer.nMaxBid = skatPlayers[0].nMaxBid;
	dealer.nState = skatPlayers[0].nState;
	dealer.pcTask = skatPlayers[0].pcTask;
	skatPlayers[0] = dealer;
	for (size_t i = 1; i < 3; i++)
	{
		if (i == 1)
		{
			pthread_create(&p1, NULL, play, (void *)i);
			sleep(2);
		}
		else
		{
			pthread_create(&p2, NULL, play, (void *)i);
			sleep(2);
		}
	}
	
	bidding(&skatPlayers[0]); // main thread (Dealer) starts bidding
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	return TRUE;
}

// Place a bid and wait for opponent's answer.
// Return TRUE:  Opponent is keeping up.
// Return FALSE: Opponent is resigning:

BOOL giveBid(int nBid, struct tPlayer *pIch)
{
	// print current bid to console:
	printf("Thread %ld (%s): %d...\n",
		   pthread_self(), // my thread ID
		   pIch->pcTask,   // my role (dealer, listener, teller)
		   nBid);		   // current bid

	// handshake: wait for opponent to accept a new bid
	// ...
	while (g_accept != 0)
	{
		usleep(100);
	}
	printf("My answer is : %d\n", nBid);
	//  set global variable to hold new bid
	//  ...
	pthread_mutex_lock(&lock);
	g_Bid = nBid;
	// handshake: global variable now holds a valid new number
	// ...
	g_valid = 1;
	pthread_mutex_unlock(&lock);
	// handshake: wait for opponent's to accept the new bid
	// ...

	// Print current state of the bidding to the console:
	if (nBid > 0)
	{ // ...only applicable, if I did't already resign
		if (g_Bid > 0)
		{
			printf("Thread %ld (%s): opponent is keeping up...\n", pthread_self(), pIch->pcTask);
		}
		else
		{
			printf("Thread %ld (%s): opponent is resigning...\n", pthread_self(), pIch->pcTask);
		}
	}

	// handshake: wait for opponent's to accept the new bid
	// ...
	while (g_accept == 0)
	{
		//printf("valid in give bid is %d\n",g_valid);
		// printf("Teller is telling %d \n",g_Bid);
		usleep(100);
	}
	pthread_mutex_lock(&lock);
	g_valid = 0;
	pthread_mutex_unlock(&lock);
	return (g_Bid>0); // change this ...
}
// Hear next bid
// Return FALSE, if opponent is resigning:

int hearBid(struct tPlayer *pIch)
{
	// handshake: wait for a NEW bid
	// ...
	int bid;
	while (g_valid == 0)
	{
		//printf("Global is now %d\n", g_valid);
		//printf("I got here in hear bid but I am sleeping!\n");
		usleep(100);
	}
	
	pthread_mutex_lock(&lock);
	bid = g_Bid;
	
	pthread_mutex_unlock(&lock);
	// get new bid from global variable and return it:
	return (bid); // change this ...
}

// Answer a bid - handshake: see giveBid():

int answerABid(BOOL bKeepUp, struct tPlayer *pIch)
{
	if (bKeepUp)
	{
		// print to the screen, that we are still keeping up:
		printf("Thread %ld (%s): OK!\n", pthread_self(), pIch->pcTask);
	}
	else
	{
		// resign:
		// clear global variable
		// ...
		pthread_mutex_lock(&lock);
		g_Bid = 0; // condition for waking dealer.
		pthread_mutex_unlock(&lock);
		
		printf("Thread %ld (%s): resigning!\n", pthread_self(), pIch->pcTask);
	}
	// handshake: indicate, we got the message:
	// ...
	pthread_mutex_lock(&lock);
	g_accept = 1;
	pthread_mutex_unlock(&lock);
	// handshake: wait for senders reaction
	// ...
	while (g_valid != 0)
	{
		usleep(100);
	}

	// handshake: indicate, that we got sender's reaction:
	// ...
	pthread_mutex_lock(&lock);
	g_accept = 0;
	pthread_mutex_unlock(&lock);
	return 0;
}

// Wake up the currently sleeping dealer and ask him to place a bid:

void wakeUpDealer(struct tPlayer *pIch)
{
	printf("Thread %ld: I'm going to wake up the Dealer!\n", pthread_self());

	// do some appropriate wakeup call:
	// ...
	pthread_mutex_lock(&lock);
	if (g_Bid == 0)
	{
		printf("Waking up the dealer\n");
		pthread_cond_signal(&waker);
	}
	pthread_mutex_unlock(&lock);
}

// (dealer thread:) wait for the wakeup call:

void waitForWakeupCall(struct tPlayer *pIch)
{
	
	pthread_mutex_lock(&lock);
	if(g_Bid != 0)
	{
		printf("Waiting to be unlocked!!!\n");
		pthread_cond_wait(&waker, &lock);
	}
	pthread_mutex_unlock(&lock);
}
