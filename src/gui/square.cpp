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

        const auto contentsRect = this->contentsRect();

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

        if (this->piece) {
            QFont font = painter.font();
            font.setPixelSize(contentsRect.height());
            painter.setFont(font);
            painter.setPen(QColorConstants::Black);
            painter.drawText(contentsRect, Qt::AlignCenter,
                             QString(QChar(piece->symbol)));
        }
    }

    void Square::mousePressEvent(QMouseEvent *event) {
        // Emit "pressed" signal
        emit pressed(*this);
    }

    void Square::setState(Gui::Square::State newState) {
        if (newState == this->state)
            return;

        this->state = newState;

        update();
    }
}
