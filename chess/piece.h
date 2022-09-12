#pragma once

enum class Type {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn,
};

enum class Color {
    White,
    Black,
};

struct Piece {
    Piece() = delete;

    Piece(Type type, Color color)
        : type(type),
          color(color) {
    }

    ~Piece() = default;

    const Type type;
    const Color color;

    bool hasMoved = false;
};
