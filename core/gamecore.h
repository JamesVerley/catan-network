// region INCLUDES

#include <iostream>
#include <cstdlib>
#include <ctime>

// (try to) use as little c++ as possible, and mainly c.

/*
 * James Verley, MVP backend engine for catan.
 *
 * Plug and play Header-level API for full control of the game
 *
 * Designed to be as simple as possible,
 * Will improve cache efficiency later
 *
 * Structure:
 * -Initialisation
 * -State
 * -Validation methods
 * -Action methods
 */

// endregion

// region DEFINITIONS
#define n_resources 5
#define n_specialcards 5

#define max_players 7

#define banktrade_req 4

#define t_left 0
#define t_topleft 1
#define t_topright 2
#define t_right 3
#define t_bottomright 4
#define t_bottomleft 5

// example
#define getmax(a,b) ((a)>(b)?(a):(b))

#define bottomleft(x,y) x-1+y%2 + (y+1)*boardWidth
#define bottomright(x,y) x+y%2 + (y+1)*boardWidth
#define right(x,y) x+1 + y*boardWidth
#define left(x,y) x-1 + y*boardWidth
#define topleft(x,y) x-1+y%2 + (y-1)*boardWidth
#define topright(x,y) x+y%2 + (y-1)*boardWidth

#define id_bottomleft(id) id+boardWidth-1 + (id/boardWidth+1)%2
#define id_bottomright(id) id+boardWidth + (id/boardWidth+1)%2
#define id_right(id) id+1
#define id_left(id) id-1
#define id_topleft(id) id-boardWidth-1 + (id/boardWidth-1)%2
#define id_topright(id) id-boardWidth + (id/boardWidth-1)%2

// precompiler flags
// #define VERBOSE
#define BAD_CODE

#define succ(x) x+1

/* // cool stuff
 *__asm__("movl %edx, %eax\n\t"
        "addl $2, %eax\n\t");

        */

// endregion

namespace gamecore{

    // region mechanism-definitions

    enum tileset{empty, ocean, forest, pasture, field, hill, mountain, desert};
    enum resources{brick, lumber, wool, grain, ore, any, noresource};
    enum developmentcard{knight, roadbuilding, yearofplenty, monopoly, victorypoint, nodevcard};
    enum building{nobuilding, settlement, city};

    struct recipes{
        char settlement[5];
        char city[5];
        char development[5];
        char road[5];
    };

    recipes gameRecipes = {
            {1,1,1,1,0},
            {0,2,0,0,3},
            {0,0,1,1,1},
            {1,1,0,0,0},
    };

    struct player {
        char resource_cards[n_resources];
        char development_cards[n_specialcards];
    };

    // each tile uniquely identifies topleft 3 corners and 3 edges
    struct tile {
        tileset landtype;
        building c_type[3];
        int c_player[3];
        int e_player[3]; // 0 = no road, >=1 = owned by player
        resources port_resources=noresource;
        int port_resource_count=0;
        int dice_no;
    }; // Ao(So(A, corners), tiles)

    struct TurnState{
        int currentTurn = 0;
        bool openTrade = false;
        int openTrades[n_resources]={0};
        int openTraderFilter[max_players]={0};
        int tradeSend[n_resources]={0};
        int tradeReceive[n_resources]={0};
        int dieRoll1=0;
        int dieRoll2=0;
        int totalDieRoll=0;
        int trueScoreEstimate;
        int hiddenScoreEstimate;
        developmentcard devcardplay; // a devcard is currently being played
        int devcard_progressflag; // flag to be used in any devcard play
        int allplayers_flag; // set 1's up to number of players
        int knightLocation=-1; // default -1
        int knightControl=-1; // player in control of knight
    }; // this shouldn't technically be public in order to limit potential callers (and future functionality that could be broken more easily)
    // This is only "fine" if it never has to change.
    // Caveats:
    // The amount of simultaneous trade offers stays at 1
    // Can only have 2 die, which should then support more than 6 sides for larger boards
    // It should change but I can't be bothered to plan far ahead enough.
    // Actually this was a mistake all this data should be packed into the places they will be accessed with anyway
    // Doesn't really matter tbh

    // endregion

    class game {
    public:
// region public state initialisation
        int playercount;
        int boardWidth;
        int boardHeight;

        TurnState turnState;

        game(int _playercount=4, int _boardWidth=7, int _boardHeight=7) {
            playercount=_playercount;
            boardWidth=_boardWidth;
            boardHeight=_boardHeight;

            turnState.allplayers_flag = ~(INT_MAX<<_playercount); // all players if devcard_progressflag==allplayers_flag

            initialise_game();
        };
// endregion

// region Action API
#ifdef VERBOSE
        void printboard() {
            for (int i = 0; i < boardHeight; ++i) {
                if (i % 2 == 1)
                    std::cout << " ";
                for (int j = 0; j < boardWidth; ++j) {
                    std::cout << " " << tileptr[i * boardWidth + j].dice_no;
                }
                std::cout << std::endl;
            }
        }
#endif

// omit gamestate checks for now. these functions trust the gameState

        bool placeRoad(int id, int edge, int player) { // put road on edge adjacent to tileptr[id], done

            // ideally would pre-load and post-save a kernel of affected tiles. but this is simpler
            if (road_validtile(id, edge)) {
                if (valid_road(id, edge, player)) {
                    if (take_resources(player, gameRecipes.road)) {
                        tileptr[id].e_player[edge] = player;
                        return true;
                    }
                }
            }
            return false;
        } // done

        bool placeSettlement(int id, int corner, int player) {
            if (building_validtile(id, corner)) {
                if (valid_building(id, corner, player, settlement)) {
                    if (take_resources(player, gameRecipes.settlement)) {
                        tileptr[id].c_type[corner] = settlement;
                        tileptr[id].c_player[corner] = player;
                        return true;
                    }
                }
            }
            return false;
        } // done

        bool placeCity(int id, int corner, int player) {
            if (building_validtile(id, corner)) {
                if (valid_building(id, corner, player, city)) {
                    if (take_resources(player, gameRecipes.city)) {
                        if (*tileptr[id].c_type == settlement) {
                            *tileptr[id].c_type = city;
                            return true;
                        }
                    }
                }
            }
            return false;
        } // done

        bool buyDevelopmentCard(int player) {
            developmentcard devcard;
            if (numdevcards>0){
                if (take_resources(player, gameRecipes.development)) {
                    if (take_random_devcard(&devcard)) {
                        playerptr[player].development_cards[devcard] += 1;
                        return true;
                    }
#ifdef VERBOSE
                    printf("COULD NOT TAKE DEVCARD");
#endif
                }
            }
            return false;
        } // done

        bool playDevelopmentCard(int player, developmentcard devcard){

            if (playerptr[player].development_cards[devcard]==0)
                return false;

            // remove devcard from player
            playerptr[player].development_cards[devcard]--;
            devcardlist[numdevcards] = devcard;
            numdevcards++;


            turnState.devcardplay=devcard;
            turnState.devcard_progressflag=0;

            return true;
        } // done, resets devcard flag and transfers inv

        bool bankTrade(int player, int resource_list[n_resources], resources resource_request) {
            // takes the first count of a certain resource above porttrade_req or banktrade_req and produces 1 of resource_request
            for (int i=0;i<n_resources;i++){
                if (resource_list[i]>=banktrade_req) {
                    playerptr[player].resource_cards[resource_request]++;
                    playerptr[player].resource_cards[i]-=banktrade_req;
                    return true;
                }
            }
            return false;
        } // TODO - ports

        bool tradeOffer(int player, int receive[], int send[], int mask[]) {

            // if (!turnState.openTrade and turnState.devcardplay==nodevcard) { // breakout into a "nextmove ready" and "nextplayer ready" func? // no validation here
                turnState.openTrade = true;
                for (int i = 0; i < playercount; i++) { turnState.openTraderFilter[i] = mask[i]; }
                for (int i = 0; i < n_resources; i++) { turnState.tradeReceive[i] = receive[i]; }
                for (int i = 0; i < n_resources; i++) { turnState.tradeSend[i] = send[i]; }
        }

        int borderedBuildings(int tileId){ // bitvec of occupying neighbors
            // TODO - remove need for abs
            int r = 0;

            // centre
            r &= 1 << abs(tileptr[tileId].c_player[0]);
            r &= 1 << abs(tileptr[tileId].c_player[1]);
            r &= 1 << abs(tileptr[tileId].c_player[2]);

            // bottomright - top / left
            r &= 1 << abs(tileptr[id_bottomright(tileId)].c_player[1]);
            r &= 1 << abs(tileptr[id_bottomright(tileId)].c_player[2]);

            // right - left / bottom
            r &= 1 << abs(tileptr[id_right(tileId)].c_player[1]);
            r &= 1 << abs(tileptr[id_right(tileId)].c_player[0]);

            return r;
        }

        bool placeKnight(int player, int tileId){
            if (turnState.devcardplay==knight and
            turnState.knightLocation!=tileId and
            (borderedBuildings(tileId)&(~(1<<player)))>0) // other players border the tile
            { // correct card to play
                    turnState.knightLocation = tileId;

                    // end the devcard play
                    turnState.devcardplay = nodevcard;
            }
        }

        bool yearOfPlentyChoice(int player, resources resource){
            // let an eligible player take any 2 resources from the bank
            if (turnState.devcardplay==yearofplenty) {
                playerptr[player].resource_cards[resource]++; // give resource to player

                if (turnState.devcard_progressflag++ == 2){ // inc and check progress
                    turnState.devcardplay=nodevcard; // end the cardplay
                    return true;
                }
            }
            return false;
        }

        // region metadata
        void updateScores(){

        }
        // endregion

// endregion

    private:
// region private state initialisation
#ifdef BAD_CODE
        int getAdjacentId(int x, int y, int direction) {
            switch (direction) {
                default:
#ifdef VERBOSE
                    std::cout << "Direction " << direction << " invalid\n"; // debug
#endif
                case t_bottomleft:
                    if (x > 0 && y < boardHeight - 1)
                        return bottomleft(x, y);
                case t_bottomright:
                    if (x < boardWidth - 1 && y < boardHeight - 1)
                        return bottomright(x, y);
                case t_left:
                    if (x > 0)
                        return left(x, y);
                case t_right:
                    if (x < boardWidth - 1)
                        return right(x, y);
                case t_topleft:
                    if (x > 0 && y > 0)
                        return topleft(x, y);
                case t_topright:
                    if (x < boardWidth - 1 && y > 0)
                        return topright(x, y);
            }
            return -1;
        } // done. USE id_<dir> MACROs INSTEAD!
#endif

        void random_board() {
            // rule: use only the values specified but randomise positions
            int tile_stkptr = 18;
            int dierolls[18] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8,
                                9, 9, 10, 10, 11, 11, 12};

            tileset tiletypes[19] = {forest, forest, forest, forest, pasture, pasture, pasture,
                                     pasture, field, field, field, field, hill, hill,
                                     hill, mountain, mountain, mountain};

            int positions[19]; // 0 to 18 from left to right, top down -> tilemap indexes


            // if even, take 1 from start arr(i+i%2)
            //
            int o = boardHeight / 2;
            int numTiles = boardWidth * boardHeight;
            int i;

            // creating blank tiles
            for (i = 0; i < numTiles; i++) {
                tileptr[i] = {};
            }

            int r, dieswap;

            for (i = 0; i < tile_stkptr; i++) {
                r = rand() % tile_stkptr;
                dieswap = dierolls[i];
                dierolls[i] = dierolls[r];
                dierolls[r] = dieswap;
            }
            tileset temp;
            for (i = 0; i < tile_stkptr; i++) {
                r = rand() % tile_stkptr;
                temp = tiletypes[i];
                tiletypes[i] = tiletypes[r];
                tiletypes[r] = temp;
            }

            // seeding island
            int centreid = boardWidth * (boardHeight / 2) + boardWidth / 2;
            tileptr[centreid].landtype = desert;

            // expanding island
            tileset landtype;
            int adj_id;
            for (int k = 3; k < 5; k++) { // 3 and 5 chosen because it's radius 2, and greater than landtype 2 which is greater than ocean and void
                for (i = 0; i < numTiles; i++) {
                    landtype = tileptr[i].landtype;
                    if (landtype != empty)
                        for (int d = 0; d < 6; d++) {
                            adj_id = getAdjacentId(i % boardWidth, i / boardWidth, d);
                            if (adj_id > -1) // none out of bounds
                                if (landtype == k - 1 && tileptr[adj_id].landtype == 0)
                                    tileptr[adj_id].landtype = static_cast<tileset>(k);
                        }
                }
            }

            // filling in island

            for (i = 0; i < numTiles; i++) {
                landtype = tileptr[i].landtype;

                if (landtype == 0) {
                    tileptr[i].landtype = ocean;
                } else if (landtype == 2) {
                    tileptr[i].landtype = desert;
                    tileptr[i].dice_no = 7;
                } else {
                    tile_stkptr--;
                    tileptr[i].landtype = tiletypes[tile_stkptr];
                    tileptr[i].dice_no = dierolls[tile_stkptr];
                }
            }
        } // mostly a hack. done

        developmentcard devcardlist[25] = {
                knight,knight,knight,knight,knight,knight,knight,knight,
                knight,knight,knight,knight,knight,knight,
                roadbuilding,roadbuilding,
                yearofplenty,yearofplenty,
                monopoly,monopoly,
                victorypoint,victorypoint,victorypoint,victorypoint,victorypoint
        };

        // the trusted inventories problem - no entity tracing
        int numdevcards = sizeof(devcardlist)/sizeof(int);


        struct tile *tileptr;
        struct player *playerptr;

        void initialise_game() {
            std::cout << "Initialising game" << std::endl;
            tileptr = (struct tile *) malloc(boardWidth * boardHeight * sizeof(struct tile));
            playerptr = (struct player *) malloc(playercount * sizeof(struct player));

            random_board();
        }
// endregion

// region validations
        bool valid_road(int id, int edge, int player) {
            if (tileptr[id].e_player[edge]==player) // can't place an existing road
                return false;

            switch (edge) {
                default:
                    return false;
                case 0:
                    // e1
                    return (tileptr[id_bottomleft(id)].e_player[1] == player) ||
                           (tileptr[id_bottomleft(id)].e_player[2] == player) ||
                           (tileptr[id_left(id)].e_player[2] == player) ||
                           (tileptr[id].e_player[1] == player);
                case 1:
                    // e2
                    return (tileptr[id_left(id)].e_player[2] == player) ||
                           (tileptr[id_topright(id)].e_player[0] == player) ||
                           (tileptr[id].e_player[2] == player) ||
                           (tileptr[id].e_player[0] == player);
                case 2:
                    // e3
                    return (tileptr[id].e_player[1] == player) ||
                           (tileptr[id_topright(id)].e_player[0] == player) ||
                           (tileptr[id_right(id)].e_player[0] == player) ||
                           (tileptr[id_right(id)].e_player[1] == player);
            }
        } // evaluates connectivity of new edge placement with other roads and buildings, but not tile

        bool valid_building(int id, int corner, int player, int type) {
            if (type == tileptr[id].c_type[corner] + 1 &&
                (tileptr[id].c_player[corner] == player || tileptr[id].c_player[corner] == 0)) {

                switch (corner) {
                    default:
                        return false;
                    case 0:
                        return (tileptr[id_bottomleft(id)].e_player[1] == player) ||
                               (tileptr[id_bottomleft(id)].e_player[2] == player) ||
                               (tileptr[id].e_player[0] == player);
                    case 1:
                        return (tileptr[id].e_player[0] == player) ||
                               (tileptr[id].e_player[1] == player) ||
                               (tileptr[id_left(id)].e_player[2] == player);
                    case 2:
                        return (tileptr[id].e_player[1] == player) ||
                               (tileptr[id].e_player[2] == player) ||
                               (tileptr[id_topright(id)].e_player[0] == player);
                }
            }
            return false;
        } // done

        bool road_validtile(int id, int edge) {
            if (tileptr[id].landtype > 1)
                return true;

            switch (edge) {
                default:
                    break;
                case 0:
                    return (tileptr[id_left(id)].landtype > 1);
                case 1:
                    return (tileptr[id_topleft(id)].landtype > 1);
                case 2:
                    return (tileptr[id_topright(id)].landtype > 1);
            }

            return false;
        } // done

        bool building_validtile(int id, int corner) {
            // one of 3 corners, corner must have no
            if (tileptr[id].landtype > 1)
                return true;

            switch (corner) {
                default:
                    break;
                case 0:
                    return (tileptr[id_bottomleft(id)].landtype > 1 || tileptr[id_left(id)].landtype > 1);
                case 1:
                    return (tileptr[id_left(id)].landtype > 1 || tileptr[id_topleft(id)].landtype > 1);
                case 2:
                    return (tileptr[id_topleft(id)].landtype > 1 || tileptr[id_topright(id)].landtype > 1);
            }

            return false;
        } // done
// endregion

// region utility
        bool take_resources(int player, char tally[]){
            int i;
            // check
            for (i=0; i<n_resources; i++){
                if (playerptr[player].resource_cards[i]<tally[i]){
                    return false;
                }
            }

            // spend
            for (i=0; i<n_resources; i++){playerptr[player].resource_cards[i]-=tally[i];}

            return true;
        }

        bool take_random_devcard(developmentcard* devcard){
            if (numdevcards>0){
                int _i = rand() % numdevcards;
                *devcard = devcardlist[_i];
                devcardlist[_i] = devcardlist[numdevcards-1];
                return true;
            }
            return false;
        } // done

// endregion
    };
}
