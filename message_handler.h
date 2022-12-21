// CATAN MESSAGE HANDLER LIBRARY

// Created by James on 25/11/2022.

// LIBRARY PURPOSE
/*
 * Validates messages
 * Interfaces with core game functions
 * Reads and submits valid core gamestate
*/


// DEPENDENCIES
/*
 * The networking library, to queue and validate messages
 * RDBMS library, user profiles
 * gamecore, processing game state
 */

// REST API
/*
 *
 */

#include "../core/gamecore.h"
// #include "lobby.h" // separate state system for gamecore, message handler and lobby

enum gc_api{

};

void process_message(char* msg){
    /*
     * Message composition:
     * int api id
     * int player id
     *
     */

    switch
}