#include "../api/message_handler.h" // encapsulates gamecore
//#include "lobby.h"

/*
 * TODO:
 *  - MOTD LIST
 *  - MAKE MONEY
 *  - AI API (based on brute force)
 *  - Also based on a clever greedy algorithm (way later) but it has to have a good way of scripting the rules
 */

struct client{ // info about a connected player
    char* nickname;
    char* address;
    // char* port;

    char* token; // verification
};

void test_game(client host){
    // minimum viable gameplay loop ;)

    // player connection:
    /*
     * nickname
     * address
     * color depends on connection order
     *
     * which one is host
     *
     *
     */

    client *clients; // struct client *clients; // idk why put "struct"..?
    clients = (struct client *) malloc(max_players * sizeof(struct client));

    clients[0] = host; // first player is always host
}

int main() {
    //std::cout << (int)playerptr[i].resource_cards[0] << std::endl;
    std::srand(std::time(0));

    gamecore::game game1 = gamecore::game();

    test_game(client{});

    // game1.printboard();

    return 0;
}

