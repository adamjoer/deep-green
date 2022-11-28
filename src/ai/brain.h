#pragma once

#include "../chess/board.h"
#include "node.h"

namespace Ai {

    Chess::Move selectMove(const Chess::Board &board);

    int negaMax(Node &node, int depth, int alpha, int beta, int color = 1);
}
