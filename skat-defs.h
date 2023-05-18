// Exercise "bidding at a skat game"

// filename: "skat-defs.h"

/* compiler call: see makefile */

// This header file contains the declaration of global types and variables,
// and function prototypes. Implementation: See skat-main.c


// Every player process ist in one of the following states:
enum stateOfGame {waiting, hearing, tellingBids, resigning, playing}; 

extern int sequenceOfBids[NUMSPW];

extern const char * gcTasksInTheGame[];

extern BOOL gb_FirstLap;       // if TRUE, listener and teller are competing, the dealer ist still waiting

struct tPlayer {
	const char *     pcTask;   // role of a player (dealer, listener, teller)
	enum stateOfGame nState;   // the first player (dealer) starts in state of waiting, the second "hearing", the third "tellingBids"
	int              nMaxBid;  // upper bound for player's bids
};

extern struct tPlayer skatPlayers[3];
extern struct tPlayer * gp_Me;   // pointer to my own player data

int bidding(struct tPlayer * pMyself);
BOOL giveBid(int nBid, struct tPlayer * pIch);
int hearBid(struct tPlayer * pIch);
int answerABid(BOOL bKeepUp, struct tPlayer * pIch);
void wakeUpDealer(struct tPlayer * pIch);
void waitForWakeupCall(struct tPlayer * pIch);
