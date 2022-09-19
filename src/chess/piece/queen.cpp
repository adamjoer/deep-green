#include "queen.h"

namespace Chess {
    std::vector<Position> Queen::possibleMoves(const Board &board) const {
        assert(board[this->position.first][this->position.second].get() == this);

        // TODO: Implement
        return {};
    }
}
