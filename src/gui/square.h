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
        static const int DEFAULT_PIXEL_SIZE = 75;

        explicit Square(QWidget *parent, int row, int column);

        std::shared_ptr<Chess::Piece> getPiece() {
            return this->piece;
        };

        void setPiece(const std::shared_ptr<Chess::Piece> &newPiece) {
            this->piece = newPiece;
        }

        [[nodiscard]]
        int getRow() const { return this->row; }

        [[nodiscard]]
        int getColumn() const { return this->column; }

    signals:

        void pressed(Square &square);

    private:
        static const QRgb LIGHT_COLOR = 0xd6b18b;
        static const QRgb DARK_COLOR = 0xa57a60;

        std::shared_ptr<Chess::Piece> piece{nullptr};

        const int row;
        const int column;

        const QColor color;

        void mousePressEvent(QMouseEvent *event) override;

        void paintEvent(QPaintEvent *event) override;
    };

}
