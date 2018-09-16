#ifndef CONSTANTS_H
#define CONSTANTS_H

// Number of players connecting to server
const int NUM_PLAYERS = 2;

// Initial score of each player when game starts
const int INITIAL_SCORE = 0;

// Number of turns game is played
const int NUM_TURNS = 100;

// Delay time (in ms) between each turn game updates
const int SLEEP_TIME = 1000;

// Length of message buffer used to store information received from clients
const int MESSAGE_BUFFER_LENGTH = 512;

#endif