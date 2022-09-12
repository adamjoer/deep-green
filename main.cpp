#include <iostream>

#include "chess/game.h"

int main() {
    Chess::Game game;
    const auto possibleMoves = game.getPossibleMoves({3, 3});

    std::wcout << '{';
    bool first = true;
    for (const auto &move: possibleMoves) {
        if (!first)
            std::wcout << ", ";
        else
            first = false;

        std::wcout << '[' << move.first << ", " << move.second << ']';
    }
    std::wcout << "}\n";

    game.printBoard(possibleMoves);

    return 0;
}
