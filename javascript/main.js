/*             ________________________________________________
 *            /                                                \
 *           |    _________________________________________     |
 *           |   |                                         |    |
 *           |   |  Catan 2.0                              |    |
 *           |   |      Jack Humphrey                      |    |
 *           |   |          /jackphumphrey                 |    |
 *           |   |      James Verley                       |    |
 *           |   |          /JamesVerley                   |    |
 *           |   |      Tuesday, December 20, 2022         |    |
 *           |   |                                         |    |
 *           |   | You may think you know what the         |    |
 *           |   | following code does.                    |    |
 *           |   | But you dont. Trust me.                 |    |
 *           |   | Fiddle with it, and youll spend many    |    |
 *           |   | sleepless nights cursing the moment you |    |
 *           |   | thought youd be clever enough to        |    |
 *           |   | "optimize" the code below.              |    |
 *           |   | Now close this file and go play with    |    |
 *           |   | something else.                         |    |
 *           |   |_________________________________________|    |
 *           |                                                  |
 *            \_________________________________________________/
 *                   \___________________________________/
 *                ___________________________________________
 *             _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_
 *          _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_
 *       _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_
 *    _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_
 * _-'.-.-.-.-.-. .---.-. .-------------------------. .-.---. .---.-.-.-.`-_
 *:-------------------------------------------------------------------------:
 *`---._.-------------------------------------------------------------._.---'
 *                             -Roland Hangg-
 */

// ----------------------------------------------------------------
// 
// ----------------------------------------------------------------
import {ResourceCard, DevelopmentCard, BuildingCostCard, SpecialCard} from "/cards.js";

var db = {
    card : {
        resource : {
            // brick
            🧱 : {
                amount : 0,
                id : []
            },
            // lumber
            🪵 : {
                amount : 0,
                id : []
            },
            // ore
            🪨 : {
                amount : 0,
                id : []
            },
            // grain
            🌾 : {
                amount : 0,
                id : []
            },
            // wool
            🐑 : {
                amount : 0,
                id : []
            }
        },
        development : {
            // knight
            🛡 : {
                amount : 0,
                id : []
            },
            // progress
            ➡️ : {
                amount : 0,
                id : []
            },
            // victorypoint
            ⭐️ : {
                amount : 0,
                id : []
            }
        },
        buildingcost : {
            amount : 0,
            id : []
        },
        special : {
            // longestroad
            🛣 : {
                id : []
            },
            // largestarmy
            🗡 : {
                id : []
            }
        }
    },
    // dice
    🎲 : {
        1 : 0,
        2 : 0
    }
}