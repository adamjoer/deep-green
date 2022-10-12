#pragma once

#include <memory>

#include <QWidget>
#include <QRgb>
#include <QColor>
#include <utility>

#include "../chess/piece/piece.h"

namespace Gui {

    class Square final : public QWidget {
    Q_OBJECT

    public:
        enum class State {
            Default,
            Highlighted,
            PossibleMove,
        };

        static const int DEFAULT_PIXEL_SIZE = 75;

        explicit Square(QWidget *parent, int row, int column);

        std::shared_ptr<Chess::Piece> getPiece() {
            return this->piece;
        };

        void setPiece(const std::shared_ptr<Chess::Piece> &newPiece);

        [[nodiscard]]
        Chess::Position getPosition() const { return this->position; }

        [[nodiscard]]
        State getState() const { return this->state; }

        void setState(State newState);

        [[nodiscard]]
        bool isEmpty() const { return this->piece == nullptr; }

    signals:

        void pressed(Gui::Square &square);

    private:
        static const QRgb DEFAULT_LIGHT_COLOR = 0xFFD6B18B;
        static const QRgb DEFAULT_DARK_COLOR = 0xFFA57A60;

        // FIXME: Find prettier colors
        static const QRgb HIGHLIGHT_COLOR = 0xFFFF0000;
        static const QRgb POSSIBLE_MOVE_COLOR = 0xFF0000FF;

        std::shared_ptr<Chess::Piece> piece{nullptr};

        const Chess::Position position;

        const QColor defaultColor;

        State state = State::Default;

        void mousePressEvent(QMouseEvent *event) override;

        void paintEvent(QPaintEvent *event) override;
    };

}
