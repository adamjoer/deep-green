#include "square.h"

#include <QPainter>

namespace Gui {
    Square::Square(QWidget *parent, int row, int column)
            : QWidget(parent),
              position(column, row),
              defaultColor((column + row) % 2 == 0 ? DEFAULT_DARK_COLOR
                                                   : DEFAULT_LIGHT_COLOR) {
    }

    void Square::paintEvent(QPaintEvent *event) {
        QWidget::paintEvent(event);

        auto contentsRect = this->contentsRect();

        QPainter painter(this);
        painter.setPen(Qt::NoPen);

        switch (this->state) {
            case State::Default:
                painter.setBrush(this->defaultColor);
                break;
            case State::Highlighted:
                painter.setBrush(QColor(HIGHLIGHT_COLOR));
                break;
            case State::PossibleMove:
                painter.setBrush(QColor(POSSIBLE_MOVE_COLOR));
                break;
        }

        painter.drawRect(contentsRect);

        auto font = painter.font();

        if (!isEmpty()) {
            font.setPixelSize(contentsRect.height());
            painter.setFont(font);
            painter.setPen(QColorConstants::Black);
            painter.drawText(contentsRect, Qt::AlignCenter,
                             QString(QChar(piece->symbol)));
        }

        if (this->position.first == 0 || this->position.second == 0) {
            font.setPixelSize(contentsRect.height() / 4);
            painter.setFont(font);
            painter.setPen(defaultColor.rgb() == DEFAULT_DARK_COLOR ? DEFAULT_LIGHT_COLOR
                                                                    : DEFAULT_DARK_COLOR);

            contentsRect -= {2, 0, 2, 0};

            if (this->position.first == 0) {
                painter.drawText(contentsRect, Qt::AlignLeft | Qt::AlignTop,
                                 QString(QChar('1' + this->position.second)));
            }

            if (this->position.second == 0) {
                painter.drawText(contentsRect, Qt::AlignRight | Qt::AlignBottom,
                                 QString(QChar('a' + this->position.first)));
            }
        }
    }

    void Square::mousePressEvent(QMouseEvent *event) {
        // Emit "pressed" signal
        emit pressed(*this);
    }

    void Square::setPiece(const std::shared_ptr<Chess::Piece> &newPiece) {
        if (newPiece == this->piece)
            return;

        this->piece = newPiece;

        update();
    }

    void Square::setState(Gui::Square::State newState) {
        if (newState == this->state)
            return;

        this->state = newState;

        update();
    }
}
