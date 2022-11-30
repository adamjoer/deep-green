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

    enum class Castling {
        None = 0,
        WhiteKing = 1,
        WhiteQueen = 2,
        BlackKing = 3,
        BlackQueen = 4,
    };

    enum class State {
        On,
        WhiteWinner,
        BlackWinner,
        Tied,
    };

    class Board {
    public:
        explicit Board(const std::string &fen);

        Board();

        Board(const Board &other)
            : bitboards(other.bitboards),
              enPassant(other.enPassant),
              halfMoveCounter(other.halfMoveCounter),
              fullMoveCounter(other.fullMoveCounter),
              playerTurn(other.playerTurn) {
            for (int i = 0; i < 3; ++i) {
                castlingRights[0][i] = other.castlingRights[0][i];
                castlingRights[1][i] = other.castlingRights[1][i];
            }
            kings[0] = other.kings[0];
            kings[1] = other.kings[1];
        };

        void reset();

        void clear();

        [[nodiscard]]
        State state();

        void performMove(Move move);

        void undoMove();

        [[nodiscard]]
        Color turnToMove() const;

        void parseFen(const std::string &fen);

        [[nodiscard]]
        std::string generateFen() const;

        [[nodiscard]]
        static bool isValidFen(const std::string &fen);

        [[nodiscard]]
        bool isLegal() const;

        [[nodiscard]]
        bool squareThreatened(Square square, Color opponentColor) const;

        [[nodiscard]]
        Bitboard squaresThreatened(Color opponentColor) const;

        [[nodiscard]]
        bool canCastleThrough(Square square, Bitboard occupiedSquares) const;

        [[nodiscard]]
        std::vector<Move> legalMoves();

        [[nodiscard]]
        std::vector<Move> legalMoves(Square square);

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
        int castlingRights[2][3] = {0, 0, 0, 0, 0,
                                    0}; // Keeps track of when castling rights were revoked (move number)
        Square enPassant{Square::None};
        int halfMoveCounter{0};
        int counterReset{0};
        int previousResetValue{0};
        int fullMoveCounter{0};
        Square kings[2]{Square::None, Square::None};

        std::vector<Move> movesMade;

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

        static Bitboard pawnThreatens(Square square, Color color);

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

        static Bitboard slidingAttack(Square square, Direction direction,
                                      Bitboard occupiedSquares);

        static std::array<Bitboard, 64> generateAttackRayMasks(Direction direction);

        static Bitboard generateAttackRayMask(Direction direction, Square square);

        static std::array<Bitboard, 64> generateKnightAttackMasks();

        static Bitboard generateKnightAttackMask(Square square);

        static std::array<Bitboard, 64> generateKingAttackMasks();

        static Bitboard generateKingAttackMask(Square square);

        static std::array<std::array<Bitboard, 64>, 2> generatePawnAttackMasks();

        static Bitboard generatePawnAttackMask(Square square, Color color);
    };
}
