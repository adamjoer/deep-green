#pragma once

#include "bitboard.h"
#include "move.h"

#include <array>
#include <vector>

namespace Chess {

    enum class Color : int {
        White,
        Black,
    };
    constexpr Color oppositeTeam(Color color) {
        return color == Color::White ? Color::Black : Color::White;
    }

    class Board {
    public:
        [[nodiscard]]
        std::vector<Move> pseudoLegalMoves() const;

        [[nodiscard]]
        std::vector<Move> pseudoLegalMoves(PieceType piece) const;

        void pseudoLegalMoves(PieceType piece, std::vector<Move> &moves) const;

        [[nodiscard]]
        std::vector<Move> pseudoLegalMoves(Square square) const;

        void pseudoLegalMoves(Square square, std::vector<Move> &moves) const;

        [[nodiscard]]
        Bitboard teamOccupiedSquares(Color color) const;

        [[nodiscard]]
        PieceType pieceAt(Square square) const;

        [[nodiscard]]
        PieceType pieceAt(Square square, Color color) const;

    private:
        /**
         * Bitboards for current state of the game, indexed by enums (Color and PieceType).
         */
        std::array<std::array<Bitboard, 6>, 2> bitboards;

        /**
         * The color of the team whose playerTurn to move it currently is
         */
        Color playerTurn{Color::White};

        static Bitboard rookAttacks(Square square, Bitboard occupiedSquares);

        static Bitboard bishopAttacks(Square square, Bitboard occupiedSquares);

        static Bitboard knightAttacks(Square square);

        static Bitboard kingAttacks(Square square);

        static Bitboard queenAttacks(Square square, Bitboard occupiedSquares);

        static Bitboard pawnAttacks(Square square, Bitboard occupiedSquares, Color color);

        /**
         * Bitboards with attack rays for sliding pieces, indexed by enums (Direction and Square)
         */
        static const std::array<std::array<Bitboard, 64>, 8> attackRayMasks;

        /**
         * Bitboards with attack masks for knight pieces, indexed by the Square enum
         */
        static const std::array<Bitboard, 64> knightAttackMasks;

        /**
         * Bitboards with attack masks for king pieces,indexed by the Square enum
         */
        static const std::array<Bitboard, 64> kingAttackMasks;

        /**
         * Bitboards with attack masks for king pieces,indexed by the Color enum then the Square enum
         */
        static const std::array<std::array<Bitboard, 64>, 2> pawnAttackMasks;

        static std::array<Bitboard, 64> generateAttackRayMasks(Direction direction);

        static Bitboard generateAttackRayMask(Direction direction, Square square);

        static std::array<Bitboard, 64> generateKnightAttackMasks();

        static Bitboard generateKnightAttackMask(Square square);

        static std::array<Bitboard, 64> generateKingAttackMasks();

        static Bitboard generateKingAttackMask(Square square);

        static std::array<std::array<Bitboard, 64>, 2> generatePawnAttackMasks();

        static Bitboard generatePawnAttackMask(Square square, Color color);

        static Bitboard slidingAttack(Square square, Direction direction,
                                      Bitboard occupiedSquares);

        static Bitboard positiveRayAttack(Square square, Direction direction,
                                          Bitboard occupiedSquares);

        static Bitboard negativeRayAttack(Square square, Direction direction,
                                          Bitboard occupiedSquares);
    };
}
