#include "board.h"

namespace Chess {
    const std::array<std::array<Bitboard, 64>, 8> Board::attackRays{
        generateAttackRays(Direction::NorthWest),
        generateAttackRays(Direction::North),
        generateAttackRays(Direction::NorthEast),
        generateAttackRays(Direction::East),
        generateAttackRays(Direction::SouthEast),
        generateAttackRays(Direction::South),
        generateAttackRays(Direction::SouthWest),
        generateAttackRays(Direction::West),
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

    Bitboard Board::rookAttacks(Square square, Bitboard occupiedSquares) {
        return {
            slidingAttack(square, Direction::North, occupiedSquares) |
            slidingAttack(square, Direction::East, occupiedSquares) |
            slidingAttack(square, Direction::South, occupiedSquares) |
            slidingAttack(square, Direction::West, occupiedSquares)
        };
    }

    Bitboard Board::bishopAttacks(Square square, Bitboard occupiedSquares) {
        return {
            slidingAttack(square, Direction::NorthWest, occupiedSquares) |
            slidingAttack(square, Direction::NorthEast, occupiedSquares) |
            slidingAttack(square, Direction::SouthEast, occupiedSquares) |
            slidingAttack(square, Direction::SouthWest, occupiedSquares)
        };
    }

    Bitboard Board::slidingAttack(Square square, Direction direction,
                                  Bitboard occupiedSquares) {
        switch (direction) {
            case Direction::NorthWest:
            case Direction::North:
            case Direction::NorthEast:
            case Direction::East:
                return positiveRayAttack(square, direction, occupiedSquares);

            case Direction::SouthEast:
            case Direction::South:
            case Direction::SouthWest:
            case Direction::West:
                return negativeRayAttack(square, direction, occupiedSquares);

            default:
                assert(false);
                return {};
        }
    }

    Bitboard Board::positiveRayAttack(Square square, Direction direction,
                                      Bitboard occupiedSquares) {
        auto attackRay = attackRays[static_cast<int>(direction)][static_cast<int>(square)];

        if (auto blockers = attackRay & occupiedSquares) {
            auto blockerIndex = bitScanForward(blockers);
            attackRay ^= attackRays[static_cast<int>(direction)][blockerIndex];
        }

        return attackRay;
    }

    Bitboard Board::negativeRayAttack(Square square, Direction direction,
                                      Bitboard occupiedSquares) {
        auto attackRay = attackRays[static_cast<int>(direction)][static_cast<int>(square)];

        if (auto blockers = attackRay & occupiedSquares) {
            auto blockerIndex = bitScanReverse(blockers);
            attackRay ^= attackRays[static_cast<int>(direction)][blockerIndex];
        }

        return attackRay;
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
