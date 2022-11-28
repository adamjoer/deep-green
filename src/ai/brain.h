#pragma once

#include <QPromise>

#include "../chess/board.h"
#include "node.h"

namespace Ai {

    void selectMove(QPromise<Chess::Move> &promise, const Chess::Board &board);
}
