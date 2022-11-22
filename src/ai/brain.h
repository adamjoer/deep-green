#pragma once

#include "../chess/board.h"
#include "node.h"

namespace Ai {

    class Brain {
    public:
        int negaMax(Node &node, int depth, int alpha, int beta, int color);

        static int staticEvaluation(const Chess::Board &chessBoard);

    private:
        static constexpr int pieceWeights[6]{
                10000,
                900,
                500,
                300,
                300,
                100,
        };

        static constexpr std::array<std::array<std::array<int, 64>, 6>, 2> positionWeights{
                {
                        {
                                {
                                        {
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                        },
                                        {
                                                2, 3, 4, 3, 4, 3, 3, 2,
                                                2, 3, 4, 4, 4, 4, 3, 2,
                                                3, 4, 4, 4, 4, 4, 4, 3,
                                                3, 3, 4, 4, 4, 4, 3, 3,
                                                2, 3, 3, 4, 4, 3, 3, 2,
                                                2, 2, 2, 3, 3, 2, 2, 2,
                                                2, 2, 2, 2, 2, 2, 2, 2,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                        },
                                        {
                                                9, 9, 11, 10, 11, 9, 9, 9,
                                                4, 6, 7, 9, 9, 7, 6, 4,
                                                9, 10, 10, 11, 11, 10, 10, 9,
                                                8, 8, 8, 9, 9, 8, 8, 8,
                                                6, 6, 5, 6, 6, 5, 6, 6,
                                                4, 5, 5, 5, 5, 5, 5, 4,
                                                3, 4, 4, 6, 6, 4, 4, 3,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                        },
                                        {
                                                2, 3, 4, 4, 4, 4, 3, 2,
                                                4, 7, 7, 7, 7, 7, 7, 4,
                                                3, 5, 6, 6, 6, 6, 5, 3,
                                                3, 5, 7, 7, 7, 7, 5, 3,
                                                4, 5, 6, 8, 8, 6, 5, 4,
                                                4, 5, 5, -2, -2, 5, 5, 4,
                                                5, 5, 5, 3, 3, 5, 5, 5,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                        },
                                        {
                                                -2, 2, 7, 9, 9, 7, 2, -2,
                                                1, 4, 12, 13, 13, 12, 4, 1,
                                                5, 11, 18, 19, 19, 18, 11, 5,
                                                3, 10, 14, 14, 14, 14, 10, 3,
                                                0, 5, 8, 9, 9, 8, 5, 0,
                                                -3, 1, 3, 4, 4, 3, 1, -3,
                                                -5, -3, -1, 0, 0, -1, -3, -5,
                                                -7, -5, -4, -2, -2, -4, -5, -7,
                                        },
                                        {
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                7, 7, 13, 23, 26, 13, 7, 7,
                                                -2, -2, 4, 12, 15, 4, -2, -2,
                                                -3, -3, 2, 9, 11, 2, -3, -3,
                                                -4, -4, 0, 6, 8, 0, -4, -4,
                                                -4, -4, 0, 4, 6, 0, -4, -4,
                                                -1, -1, 1, 5, 6, 1, -1, -1,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                        },
                                }
                        },
                        {
                                {
                                        {
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                        },
                                        {
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                2, 2, 2, 2, 2, 2, 2, 2,
                                                2, 2, 2, 3, 3, 2, 2, 2,
                                                2, 3, 3, 4, 4, 3, 3, 2,
                                                3, 3, 4, 4, 4, 4, 3, 3,
                                                3, 4, 4, 4, 4, 4, 4, 3,
                                                2, 3, 4, 4, 4, 4, 3, 2,
                                                2, 3, 4, 3, 4, 3, 3, 2,
                                        },
                                        {
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                3, 4, 4, 6, 6, 4, 4, 3,
                                                4, 5, 5, 5, 5, 5, 5, 4,
                                                6, 6, 5, 6, 6, 5, 6, 6,
                                                8, 8, 8, 9, 9, 8, 8, 8,
                                                9, 10, 10, 11, 11, 10, 10, 9,
                                                4, 6, 7, 9, 9, 7, 6, 4,
                                                9, 9, 11, 10, 11, 9, 9, 9,
                                        },
                                        {
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                5, 5, 5, 3, 3, 5, 5, 5,
                                                4, 5, 5, -2, -2, 5, 5, 4,
                                                4, 5, 6, 8, 8, 6, 5, 4,
                                                3, 5, 7, 7, 7, 7, 5, 3,
                                                3, 5, 6, 6, 6, 6, 5, 3,
                                                4, 7, 7, 7, 7, 7, 7, 4,
                                                2, 3, 4, 4, 4, 4, 3, 2,
                                        },
                                        {
                                                -7, -5, -4, -2, -2, -4, -5, -7,
                                                -5, -3, -1, 0, 0, -1, -3, -5,
                                                -3, 1, 3, 4, 4, 3, 1, -3,
                                                0, 5, 8, 9, 9, 8, 5, 0,
                                                3, 10, 14, 14, 14, 14, 10, 3,
                                                5, 11, 18, 19, 19, 18, 11, 5,
                                                1, 4, 12, 13, 13, 12, 4, 1,
                                                -2, 2, 7, 9, 9, 7, 2, -2,
                                        },
                                        {
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                                -1, -1, 1, 5, 6, 1, -1, -1,
                                                -4, -4, 0, 4, 6, 0, -4, -4,
                                                -4, -4, 0, 6, 8, 0, -4, -4,
                                                -3, -3, 2, 9, 11, 2, -3, -3,
                                                -2, -2, 4, 12, 15, 4, -2, -2,
                                                7, 7, 13, 23, 26, 13, 7, 7,
                                                0, 0, 0, 0, 0, 0, 0, 0,
                                        },
                                }
                        },
                }
        };
    };
}
