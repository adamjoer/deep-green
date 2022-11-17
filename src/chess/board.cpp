#include "board.h"

namespace Chess {
    const std::array<std::array<Bitboard, 64>, 8> Board::attackRayMasks{
            generateAttackRayMasks(Direction::NorthWest),
            generateAttackRayMasks(Direction::North),
            generateAttackRayMasks(Direction::NorthEast),
            generateAttackRayMasks(Direction::East),
            generateAttackRayMasks(Direction::SouthEast),
            generateAttackRayMasks(Direction::South),
            generateAttackRayMasks(Direction::SouthWest),
            generateAttackRayMasks(Direction::West),
    };

    const std::array<Bitboard, 64> Board::knightAttackMasks = generateKnightAttackMasks();

    std::array<Bitboard, 64> Board::generateAttackRayMasks(Direction direction) {
        std::array<Bitboard, 64> ray;

        for (int i = 0; i < ray.size(); i++)
            ray[i] = generateAttackRayMask(direction, Square(i));

        return ray;
    }

    Bitboard Board::generateAttackRayMask(Direction direction, Square square) {
        Bitboard ray;

        while (true) {
            square = Bitboard::squareToThe(direction, square);
            if (square == Square::None)
                break;

            ray.setOccupancyAt(square);
        }

        return ray;
    }

     std::array<Bitboard, 64> Board::generateKnightAttackMasks() {
        std::array<Bitboard, 64> rookAttacks;

        for (int i = 0; i < rookAttacks.size(); ++i) {
            rookAttacks[i] = generateKnightAttackMask(Square(i));
        }

        return rookAttacks;
    }

    Bitboard Board::generateKnightAttackMask(Square square) {
        Bitboard attack;

        auto lambda = [&](auto square, auto direction) {
            square = Bitboard::squareToThe(direction, square);
            if (square == Square::None)
                return;

            Square left;
            Square right;
            switch (direction) {
                case Direction::North:
                    left = Bitboard::squareToThe(Direction::NorthWest, square);
                    right = Bitboard::squareToThe(Direction::NorthEast, square);
                    break;
                case Direction::East:
                    left = Bitboard::squareToThe(Direction::NorthEast, square);
                    right = Bitboard::squareToThe(Direction::SouthEast, square);
                    break;
                case Direction::South:
                    left = Bitboard::squareToThe(Direction::SouthWest, square);
                    right = Bitboard::squareToThe(Direction::SouthEast, square);
                    break;
                case Direction::West:
                    left = Bitboard::squareToThe(Direction::SouthWest, square);
                    right = Bitboard::squareToThe(Direction::NorthWest, square);
                    break;
                default:
                    left = right = Square::None;
                    break;
            }

            if (left != Square::None)
                attack.setOccupancyAt(left);
            if (right != Square::None)
                attack.setOccupancyAt(right);
        };

        lambda(square, Direction::North);
        lambda(square, Direction::East);
        lambda(square, Direction::South);
        lambda(square, Direction::West);

        return attack;
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

    Bitboard Board::knightAttacks(Square square, Bitboard occupiedSquares) {
        return knightAttackMasks[static_cast<int>(square)] & ~occupiedSquares;
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
        auto attackRay = attackRayMasks[static_cast<int>(direction)][static_cast<int>(square)];

        if (auto blockers = attackRay & occupiedSquares) {
            auto blockerIndex = blockers.bitScanForward();
            attackRay ^= attackRayMasks[static_cast<int>(direction)][blockerIndex];
        }

        return attackRay;
    }

    Bitboard Board::negativeRayAttack(Square square, Direction direction,
                                      Bitboard occupiedSquares) {
        auto attackRay = attackRayMasks[static_cast<int>(direction)][static_cast<int>(square)];

        if (auto blockers = attackRay & occupiedSquares) {
            auto blockerIndex = blockers.bitScanReverse();
            attackRay ^= attackRayMasks[static_cast<int>(direction)][blockerIndex];
        }

        return attackRay;
    }

    void Board::printAttackRays(Direction direction, Square square) {
        if (square != Square::None) {
            std::cout << square << ":\n"
                      << attackRayMasks[static_cast<int>(direction)][static_cast<int>(square)]
                      << '\n';
        } else {
            auto bitboards = attackRayMasks[static_cast<int>(direction)];
            for (int i = 0; i < bitboards.size(); ++i) {
                std::cout << Square(i) << ":\n"
                          << bitboards[i] << '\n';
            }
        }
    }
}
