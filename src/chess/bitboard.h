#pragma once

#include <cstdint>
#include <array>
#include <cassert>
#include <iostream>
#include <bit>

namespace Chess {

    enum class Square : int {
        None = -1,
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
    };

    static std::string squareToString[] = {
            "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
            "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
            "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
            "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
            "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
            "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
            "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
            "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    };

    std::ostream &operator<<(std::ostream &os, Square square);

    enum class PieceType : int {
        King,
        Queen,
        Rook,
        Bishop,
        Knight,
        Pawn,
    };
    std::ostream &operator<<(std::ostream &os, PieceType piece);

    enum class Direction : int {
        NorthWest,
        North,
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
    };

    class Bitboard {
    public:
        constexpr Bitboard() = default;

        constexpr Bitboard(const Bitboard &other) = default;

        constexpr explicit Bitboard(uint64_t bits)
                : bits(bits) {
        }

        constexpr explicit Bitboard(Square square) {
            setOccupancyAt(square);
        }

        template<typename... Args>
        constexpr explicit Bitboard(Args... args) {
            setOccupancyAt(args...);
        }

        ~Bitboard() = default;

        [[nodiscard]]
        constexpr bool isOccupiedAt(Square square) const {
            assert(square != Square::None);
            return this->bits & (1ULL << static_cast<int>(square));
        }

        constexpr void setOccupancyAt(Square square) {
            assert(square != Square::None);
            this->bits |= (1ULL << static_cast<int>(square));
        }

        template<typename... Args>
        constexpr void setOccupancyAt(Square square, Args... args) {
            setOccupancyAt(square);
            setOccupancyAt(args...);
        }

        constexpr void clearOccupancyAt(Square square) {
            assert(square != Square::None);
            this->bits &= ~(1ULL << static_cast<int>(square));
        }

        constexpr explicit operator bool() const {
            return static_cast<bool>(this->bits);
        }

        [[nodiscard]]
        constexpr Bitboard operator~() const {
            return Bitboard(~this->bits);
        }

        [[nodiscard]]
        constexpr Bitboard operator&(const Bitboard &other) const {
            return Bitboard(this->bits & other.bits);
        }

        [[nodiscard]]
        constexpr Bitboard operator|(const Bitboard &other) const {
            return Bitboard(this->bits | other.bits);
        }

        constexpr Bitboard operator^(const Bitboard &other) const {
            return Bitboard(this->bits ^ other.bits);
        }

        constexpr Bitboard &operator&=(const Bitboard &other) {
            this->bits &= other.bits;
            return *this;
        }

        constexpr Bitboard &operator|=(const Bitboard &other) {
            this->bits |= other.bits;
            return *this;
        }

        constexpr Bitboard &operator^=(const Bitboard &other) {
            this->bits ^= other.bits;
            return *this;
        }

        [[nodiscard]]
        constexpr bool isOverlappingWith(const Bitboard &other) const {
            return this->bits & other.bits;
        }

        [[nodiscard]]
        constexpr int bitScanForward() const {
            assert(this->bits != 0);
            return std::countr_zero(this->bits);
        }

        [[nodiscard]]
        constexpr int bitScanReverse() const {
            assert(this->bits != 0);
            return 63 - std::countl_zero(this->bits);
        }

        static Square squareToThe(Direction direction, Square square);

        friend std::ostream &operator<<(std::ostream &os, const Bitboard &bitboard);

    private:
        uint64_t bits{0};
    };
}
