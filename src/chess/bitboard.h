#pragma once

#include <cstdint>
#include <array>
#include <cassert>

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

        explicit Bitboard(uint64_t bits)
                : bits(bits) {
        }

//        explicit BitBoard(std::array<std::array<bool, 8>, 8> array);

        [[nodiscard]]
        constexpr bool isOccupiedAt(Square square) const {
            assert(square != Square::None);
            return this->bits & (1ULL << static_cast<int>(square));
        }

        constexpr void setOccupancyAt(Square square) {
            assert(square != Square::None);
            this->bits |= (1ULL << static_cast<int>(square));
        }

        constexpr void clearOccupancyAt(Square square) {
            assert(square != Square::None);
            this->bits ^= (1ULL << static_cast<int>(square));
        }

        void print() const;

        static Square squareToThe(Direction direction, Square square);

    private:
        uint64_t bits{0};

    };
}
