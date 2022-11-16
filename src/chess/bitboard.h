#pragma once

#include <cstdint>
#include <array>
#include <cassert>
#include <iostream>

namespace Chess {

    enum Square {
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
    std::ostream &operator<<(std::ostream &os, Square square);

    enum class Direction {
        North,
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
        NorthWest,
    };

    class Bitboard {
    public:
        Bitboard() = default;

        Bitboard(const Bitboard &other) = default;

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

        static Square squareToThe(Direction direction, Square square);

        friend std::ostream &operator<<(std::ostream &os, const Bitboard &bitboard);

    private:
        uint64_t bits{0};
    };
}
