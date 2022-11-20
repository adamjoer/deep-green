#pragma once

#include "bitboard.h"
#include "move.h"

#include <array>
#include <vector>
#include <ostream>

namespace Chess {

    enum class Color : int {
        White,
        Black,
    };

    std::ostream &operator<<(std::ostream &os, Color color);

    constexpr Color oppositeTeam(Color color) {
        return color == Color::White ? Color::Black : Color::White;
    }

    enum class CastlingRightFlag : uint8_t {
        WhiteKing = 0x01,
        WhiteQueen = 0x02,
        BlackKing = 0x04,
        BlackQueen = 0x08,
    };

    enum class Castling {
        WhiteKing = 1,
        WhiteQueen = 2,
        BlackKing = 4,
        BlackQueen = 8
    };

    class Board {
    public:
        explicit Board(const std::string &fen);

        Board();

        void reset();

        void clear();

        void performMove(Move move);

        void undoMove(Move move);

        [[nodiscard]]
        Color turnToMove() const;

        void parseFen(const std::string &fen);

        [[nodiscard]]
        std::string generateFen() const;

        [[nodiscard]]
        std::vector<Move> pseudoLegalMoves() const;

        [[nodiscard]]
        std::vector<Move> pseudoLegalMoves(PieceType piece) const;

        void pseudoLegalMoves(PieceType piece, std::vector<Move> &moves) const;

        [[nodiscard]]
        std::vector<Move> pseudoLegalMoves(Square square) const;

        void pseudoLegalMoves(Square square, std::vector<Move> &moves) const;

        [[nodiscard]]
        std::vector<Move> pseudoLegalMoves(Square square, Color color) const;

        void pseudoLegalMoves(Square square, Color color, std::vector<Move> &moves) const;

        [[nodiscard]]
        bool isMovePseudoLegal(Move move) const;

        [[nodiscard]]
        Bitboard teamOccupiedSquares(Color color) const;

        [[nodiscard]]
        PieceType pieceAt(Square square) const;

        [[nodiscard]]
        PieceType pieceAt(Square square, Color color) const;

        friend std::ostream &operator<<(std::ostream &os, const Board &board);

    private:
        /**
         * Bitboards for current state of the game, indexed by enums (Color and PieceType).
         */
        std::array<std::array<Bitboard, 6>, 2> bitboards;

        /**
         * Variables pertaining to current game state.
         */
        uint8_t castlingRights{};
        Square enPassant{Square::None};
        int halfMoveCounter{0};
        int fullMoveCounter{0};

        /**
         * The color of the team whose turn to move it currently is
         */
        Color playerTurn{Color::White};

        PieceType removePieceAt(Square square);

        PieceType removePieceAt(Square square, Color color);

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
