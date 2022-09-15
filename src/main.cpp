#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cctype>

#include "chess/game.h"

using namespace std;

int main() {
    Chess::Game game;

    string status = "Welcome";
    vector<Chess::Position> highlightedMoves;

    while (true) {
        wcout << "\033[1;1H\033[2J";
        game.printBoard(highlightedMoves);

        wcout << "Status: " << status.c_str()
              << "\nIt is "
              << (game.turn() == Chess::Color::White ? "white's" : "black's")
              << " turn\n"
                 ">";

        string input;
        cin >> input;

        if (input == "exit") {
            break;
        } else if (input == "reset") {
            game.reset();
            highlightedMoves.clear();

            status = "Reset game";
            continue;
        }

        // TODO: Use proper algebraic notation?
        //       https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
        smatch moveRegexMatches;
        if (!regex_match(input, moveRegexMatches,
                         regex("^([a-hA-H][1-8])(?:->([a-hA-H][1-8]))?$"))) {
            status = "Invalid input";
            continue;
        }

        Chess::Position from(
            tolower(moveRegexMatches[1].str()[0]) - 'a',
            moveRegexMatches[1].str()[1] - '0' - 1
        );

        try {
            if (!moveRegexMatches[2].matched) {
                highlightedMoves = game.getPossibleMoves(from);
                status = "Highlighted possible moves";

            } else {
                Chess::Position to(
                    tolower(moveRegexMatches[2].str()[0]) - 'a',
                    moveRegexMatches[2].str()[1] - '0' - 1
                );

                if (game.move(from, to)) {
                    highlightedMoves.clear();
                    status = "Moved piece";

                } else {
                    status = "Invalid move";
                }
            }

        } catch (exception &exception) {
            status = exception.what();
            continue;
        }
    }

    return 0;
}
