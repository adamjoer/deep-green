#include "board.h"

namespace Chess {
    static constexpr Bitboard twoRank{Square::A2, Square::B2, Square::C2, Square::D2,
                                      Square::E2, Square::F2, Square::G2, Square::H2};
    static constexpr Bitboard sevenRank{Square::A7, Square::B7, Square::C7, Square::D7,
                                        Square::E7, Square::F7, Square::G7, Square::H7};

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

    const std::array<Bitboard, 64> Board::kingAttackMasks = generateKingAttackMasks();

    const std::array<std::array<Bitboard, 64>, 2> Board::pawnAttackMasks = generatePawnAttackMasks();

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

        auto generateKnightAttack = [&](auto square, auto direction) {
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

        generateKnightAttack(square, Direction::North);
        generateKnightAttack(square, Direction::East);
        generateKnightAttack(square, Direction::South);
        generateKnightAttack(square, Direction::West);

        return attack;
    }

    std::array<Bitboard, 64> Board::generateKingAttackMasks() {
        std::array<Bitboard, 64> attacks;

        for (int i = 0; i < attacks.size(); ++i) {
            attacks[i] = generateKingAttackMask(Square(i));
        }

        return attacks;
    }

    Bitboard Board::generateKingAttackMask(Chess::Square square) {
        Bitboard attackMask;

        for (int i = 0; i < 8; i++) {
            auto attackSquare = Bitboard::squareToThe(Direction(i), square);
            if (attackSquare != Square::None)
                attackMask.setOccupancyAt(attackSquare);
        }
        return attackMask;
    }

    std::array<std::array<Bitboard, 64>, 2> Board::generatePawnAttackMasks() {
        std::array<std::array<Bitboard, 64>, 2> attackMasks;

        for (int i = 0; i < attackMasks.size(); ++i) {
            for (int j = 0; j < attackMasks[i].size(); ++j) {
                attackMasks[i][j] = generatePawnAttackMask(Square(j), Color(i));
            }
        }

        return attackMasks;
    }

    Bitboard Board::generatePawnAttackMask(Square square, Color color) {
        Direction attackDirection;
        switch (color) {
            case Color::Black:
                attackDirection = Direction::South;
                break;
            case Color::White:
                attackDirection = Direction::North;
                break;
        }

        Bitboard attack;

        auto attackSquare = Bitboard::squareToThe(attackDirection, square);
        if (attackSquare != Square::None)
            attack.setOccupancyAt(attackSquare);

        auto startRank = (color == Color::White) ? twoRank : sevenRank;
        if (startRank.isOccupiedAt(square))
            attack.setOccupancyAt(Bitboard::squareToThe(attackDirection, attackSquare));

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

    Bitboard Board::knightAttacks(Square square) {
        return knightAttackMasks[static_cast<int>(square)];
    }

    Bitboard Board::kingAttacks(Square square) {
        return kingAttackMasks[static_cast<int>(square)];
    }

    Bitboard Board::queenAttacks(Square square, Bitboard occupiedSquares) {
        return {
                rookAttacks(square, occupiedSquares) |
                bishopAttacks(square, occupiedSquares)
        };
    }

    Bitboard Board::pawnAttacks(Square square, Bitboard occupiedSquares, Color color) {
        Direction attackDirection;
        Bitboard startRank;
        Square captureEast;
        Square captureWest;

        switch (color) {
            case Color::White:
                attackDirection = Direction::North;
                startRank = twoRank;
                captureEast = Bitboard::squareToThe(Direction::NorthEast, square);
                captureWest = Bitboard::squareToThe(Direction::NorthWest, square);
                break;

            case Color::Black:
                attackDirection = Direction::South;
                startRank = sevenRank;
                captureEast = Bitboard::squareToThe(Direction::SouthEast, square);
                captureWest = Bitboard::squareToThe(Direction::SouthWest, square);
                break;
        }

        Bitboard attacks;

        auto attackMask = pawnAttackMasks[static_cast<int>(color)][static_cast<int>(square)];
        if (attackMask.isOverlappingWith(occupiedSquares)) {

            auto nextSquare = Bitboard::squareToThe(attackDirection, square);
            if (startRank.isOccupiedAt(square) && !occupiedSquares.isOccupiedAt(nextSquare))
                attacks.setOccupancyAt(nextSquare);

        } else {
            attacks = attackMask;
        }

        if (captureEast != Square::None && occupiedSquares.isOccupiedAt(captureEast))
            attacks.setOccupancyAt(captureEast);

        if (captureWest != Square::None && occupiedSquares.isOccupiedAt(captureWest))
            attacks.setOccupancyAt(captureWest);

        return attacks;
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
}
