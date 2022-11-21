#pragma once

#include <ostream>
#include <optional>

#include <QWidget>
#include <QRgb>
#include <QColor>

#include "../chess/move.h"
#include "../chess/board.h"

namespace Gui {
    struct Piece {
        Piece(Chess::PieceType type, Chess::Color color)
                : type(type),
                  color(color) {
        }

        Chess::PieceType type;
        Chess::Color color;

        bool operator==(const Piece &rhs) const;

        bool operator!=(const Piece &rhs) const;

        [[nodiscard]]
        wchar_t symbol() const;

        friend std::wostream &operator<<(std::wostream &os, const Piece &piece);
    };

    class Square final : public QWidget {
    Q_OBJECT

    public:
        enum class State {
            Default,
            Highlighted,
            PossibleMove,
        };

        explicit Square(QWidget *parent, int rank, int file);

        [[nodiscard]]
        std::optional<Piece> getPiece() const {
            return this->piece;
        }

        void setPiece(std::optional<Piece> newPiece);

        [[nodiscard]]
        Chess::Square getPosition() const {
            return Chess::Square(this->rank * 8 + file);
        }

        [[nodiscard]]
        State getState() const { return this->state; }

        void setState(State newState);

        [[nodiscard]]
        bool isEmpty() const { return this->piece == std::nullopt; }

    signals:

        void pressed(Gui::Square &square);

    private:
        static const QRgb DEFAULT_LIGHT_COLOR = 0xFFD6B18B;
        static const QRgb DEFAULT_DARK_COLOR = 0xFFA57A60;

        // FIXME: Find prettier colors
        static const QRgb HIGHLIGHT_COLOR = 0xFFFF0000;
        static const QRgb POSSIBLE_MOVE_COLOR = 0xFF0000FF;

        std::optional<Piece> piece{std::nullopt};

        const int rank;
        const int file;

        const QColor defaultColor;

        State state{State::Default};

        void mousePressEvent(QMouseEvent *event) override;

        void paintEvent(QPaintEvent *event) override;
    };

}
