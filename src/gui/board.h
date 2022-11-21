#pragma once

#include <QWidget>

#include "square.h"

namespace Gui {

    class Board final : public QWidget {
    Q_OBJECT

    public:
        explicit Board(QWidget *parent);

        explicit Board(QWidget *parent, const Chess::Board &chessBoard);

        void performMove(Chess::Square from, Chess::Square to);

        void set(const Chess::Board &chessBoard);

        void clearHighlights();

        void highlightPossibleMoves(const std::vector<Chess::Move> &possibleMoves);

        [[nodiscard]]
        int getPixelSize() const { return this->pixelSize; }

        void setPixelSize(int newPixelSize);

        [[nodiscard]]
        const std::array<Square *, 64> &getSquares() const {
            return this->squares;
        }

    private:
        static constexpr int DEFAULT_PIXEL_SIZE = 600;

        int pixelSize{DEFAULT_PIXEL_SIZE};

        std::array<Square *, 64> squares{nullptr};

        void createLayout();
    };
}
