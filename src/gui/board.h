#pragma once

#include <QWidget>
#include <QLabel>

#include "square.h"
#include "../chess/team.h"

namespace Gui {

    class Board final : public QWidget {
    Q_OBJECT

    public:
        static const int SIZE = 8;

        explicit Board(QWidget *parent);

        void zoom(int offset);

        void addTeamStartingPosition(const Chess::Team &team);

        [[nodiscard]]
        int getSquarePixelSize() const { return this->squarePixelSize; }

    private:
        int squarePixelSize;

        std::array<std::array<Square *, SIZE>, SIZE> squares{nullptr};
    };
}
