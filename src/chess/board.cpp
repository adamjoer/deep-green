#include "board.h"

namespace Chess {
    const std::array<std::array<Bitboard, 64>, 8> Board::attackRays{
        generateAttackRays(Direction::North),
        generateAttackRays(Direction::NorthEast),
        generateAttackRays(Direction::East),
        generateAttackRays(Direction::SouthEast),
        generateAttackRays(Direction::South),
        generateAttackRays(Direction::SouthWest),
        generateAttackRays(Direction::West),
        generateAttackRays(Direction::NorthWest),
    };

    std::array<Bitboard, 64> Board::generateAttackRays(Direction direction) {
        std::array<Bitboard, 64> ray;

        for (int i = 0; i < ray.size(); i++)
            ray[i] = generateAttackRay(direction, Square(i));

        return ray;
    }

    Bitboard Board::generateAttackRay(Direction direction, Square square) {
        Bitboard ray;

        while (true) {
            square = Bitboard::squareToThe(direction, square);
            if (square == Square::None)
                break;

            ray.setOccupancyAt(square);
        }

        return ray;
    }

    Bitboard Board::bishopAttacks(Square square, Bitboard blockers) {
        // See https://rhysre.net/fast-chess-move-generation-with-magic-bitboards.html

        Bitboard attacks;

        attacks |= attackRays[static_cast<int>(Direction::NorthEast)][static_cast<int>(square)];
        if (auto maskedBlockers = attackRays[static_cast<int>(Direction::NorthEast)][static_cast<int>(square)] & blockers) {
            int blockerIndex = bitScanForward(maskedBlockers);
            attacks ^= attackRays[static_cast<int>(Direction::NorthEast)][blockerIndex];
        }

        attacks |= attackRays[static_cast<int>(Direction::SouthEast)][static_cast<int>(square)];
        if (auto maskedBlockers = attackRays[static_cast<int>(Direction::SouthEast)][static_cast<int>(square)] & blockers) {
            int blockerIndex = bitScanReverse(maskedBlockers);
            attacks ^= attackRays[static_cast<int>(Direction::SouthEast)][blockerIndex];
        }

        attacks |= attackRays[static_cast<int>(Direction::SouthWest)][static_cast<int>(square)];
        if (auto maskedBlockers = attackRays[static_cast<int>(Direction::SouthWest)][static_cast<int>(square)] & blockers) {
            int blockerIndex = bitScanReverse(maskedBlockers);
            attacks ^= attackRays[static_cast<int>(Direction::SouthWest)][blockerIndex];
        }

        attacks |= attackRays[static_cast<int>(Direction::NorthWest)][static_cast<int>(square)];
        if (auto maskedBlockers = attackRays[static_cast<int>(Direction::NorthWest)][static_cast<int>(square)] & blockers) {
            int blockerIndex = bitScanForward(maskedBlockers);
            attacks ^= attackRays[static_cast<int>(Direction::NorthWest)][blockerIndex];
        }

        return attacks;
    }

    int Board::bitScanForward(Bitboard bitboard) {
        // TODO
        return 0;
    }

    int Board::bitScanReverse(Bitboard bitboard) {
        // TODO
        return 0;
    }

    void Board::printAttackRays(Direction direction, Square square) {
        if (square != Square::None) {
            std::cout << square << ":\n"
                      << attackRays[static_cast<int>(direction)][static_cast<int>(square)]
                      << '\n';
        } else {
            auto bitboards = attackRays[static_cast<int>(direction)];
            for (int i = 0; i < bitboards.size(); ++i) {
                std::cout << Square(i) << ":\n"
                          << bitboards[i] << '\n';
            }
        }
    }
}
