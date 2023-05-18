// Exercise "bidding at a skat game"

// Lesson to learn: how to deal with proceses, pipes, signals, queues, etc.
 
// This module just contains the skat stuff.
// There are just a few operating system calls like fork().
 
// Other modulees will deal with interprocess communication.
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "glob-defs.h"
#include "skat-defs.h"
#include "skat-comm.h"

int sequenceOfBids[NUMSPW] = {18, 20, 22, 23, 27, 33, 35, 36, 40, 44, 45, 46, 48, 50};

const char * gcTasksInTheGame[] = {"Dealer  ", "Listener", "Teller  "};

int sequenceOfBids[NUMSPW];

BOOL gb_FirstLap = TRUE;     // TRUE: the first two players are competing, the third is still waiting

struct tPlayer skatPlayers[3];

int bidding(struct tPlayer * pMyself);
BOOL giveBid(int nBid, struct tPlayer * pIch);
int hearBid(struct tPlayer * pIch);
int answerABid(BOOL bKeepUp, struct tPlayer * pIch);
void wakeUpDealer(struct tPlayer * pIch);

int main(int argc, char *argv[])
{
	int i;

	BOOL bCorrectCall = (argc >= 4);
	if (bCorrectCall) {
		for (i=1; i<=3; i++) {
			int nGameNumber = atoi(argv[i]);
			if((nGameNumber >= sequenceOfBids[0]) && (nGameNumber <= sequenceOfBids[NUMSPW-1])) {
				skatPlayers[i-1].nMaxBid = nGameNumber;
			}
			else bCorrectCall = FALSE;
		}
	}
	if (bCorrectCall) {
		printf ("\nSkat simulation with three processes:\nDealer will bit up to %d,\nListener will bid up to %d,\nTeller will bid up to %d\n\n", skatPlayers[0].nMaxBid, skatPlayers[1].nMaxBid, skatPlayers[2].nMaxBid);
	}
	else {
		printf ("Please start programm with three bids, e. g. %s 18 46 23\n\n",argv[0]);
		return 1;
	}

	// init skat players:
	for (i=0; i<=2; i++) {
		skatPlayers[i].pcTask    = gcTasksInTheGame[i];
	}
	skatPlayers[0].nState      = waiting;     // Dealer starts in state of waiting
	skatPlayers[1].nState      = hearing;     // Listener starts in state of hearing
	skatPlayers[2].nState      = tellingBids; // Teller starts in state of tellingBids
	for (size_t i = 0; i < sizeof(skatPlayers)/sizeof(skatPlayers[0]); i++)
	{
		printf("%d  main process(%d): I have %d\n",(int)i+1,skatPlayers[i],skatPlayers[i].nState);
	}
	// init communication channels:
	if (!InitCommunication()) {
		cleanup();
		return EXIT_FAILURE;
	}
	
	// create players:
	if (!CreatePlayer()) {
		fprintf (stderr ,"Error creating players!\n") ;
		cleanup();
		return 1;
	}
	cleanup();  // hopefully no child process needs theese channels any more
	return 0;
}


int bidding(struct tPlayer * pMyself)
{
	BOOL bGameOver = FALSE;
	if (pMyself == NULL) {
		printf ("Process %d: I don't know, who I am...\n", getpid());
		return 1;
	}
	printf ("Process %d (%s): I'm going to bid until %d!\n", getpid(), pMyself->pcTask, pMyself->nMaxBid);

	if (pMyself->nState == waiting) {
		printf ("Process %d (%s): I have to wait for the wakeup call...\n", getpid(), pMyself->pcTask);
		waitForWakeupCall(pMyself);
		printf("Prozess %d (%s): Good morning - now I have to tell bids!\n", getpid(), pMyself->pcTask);
		gb_FirstLap = FALSE;           // second lap starts now...
		pMyself->nState = tellingBids; // ...and I have  to tell bids!
	}
	
	// start bidding now:
	int nBid = 0;
	int nBidsIndex = 0;
	int nOpponentsBid = 0;

	while (!bGameOver) {
		switch (pMyself->nState) {
			case hearing:
			//printf("I got here after lock!!");
				nOpponentsBid = hearBid(pMyself);
				//printf("opo bid= %d\n",nOpponentsBid);
				if (nOpponentsBid == 0) { // opponent is resigning, no more communication
				
					if (gb_FirstLap) {
						wakeUpDealer(pMyself); // ask the dealer for higher bids now
						gb_FirstLap = FALSE;
					}
					else {
						pMyself->nState = playing;  // all opponents are resigning
						answerABid(TRUE, pMyself);  // to complete handshake
					}
				}
				else {  // opponent is still bidding
					if (nOpponentsBid <= pMyself->nMaxBid) { // I keep up
						nBid = nOpponentsBid;
						answerABid(TRUE, pMyself);
					}
					else {  // I have to resign
						answerABid(FALSE, pMyself);
						pMyself->nState = resigning; 
					}
				}
				break;
			case tellingBids:
				if ((nBid < pMyself->nMaxBid) && (nBidsIndex < NUMSPW-1)) {
					nBid = sequenceOfBids[nBidsIndex++]; // increase bid
					if (!giveBid(nBid, pMyself)) { // opponent is resigning
						if (gb_FirstLap) {
							gb_FirstLap = FALSE;
							pMyself->nState = hearing; // I must tell bids no more. Now I have to hear the dealer's bids.
							wakeUpDealer(pMyself); // ask the dealer for higher bids now
						}
						else {
							pMyself->nState = playing;
						}
					}
				}
				else {
					giveBid(0, pMyself);  // tell opponent, I will no longer tell bids 
					pMyself->nState = resigning; 
				}
				break;

			case resigning:
				printf ("Process %d (%s): resigning at %d!\n", getpid(), pMyself->pcTask, nBid);
				bGameOver = TRUE;
				break;

			case playing:
				printf ("Process %d (%s): Hurray, I'm playing!\n", getpid(), pMyself->pcTask);
				bGameOver = TRUE;
				break;
		}
		sleep(1);
	}
}


