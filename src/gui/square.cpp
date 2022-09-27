#include "square.h"

#include <iostream>

#include <QPainter>

namespace Gui {
    Square::Square(QWidget *parent, bool isDark)
        : QWidget(parent),
          color(isDark ? DARK_COLOR : LIGHT_COLOR) {
    }

    void Square::paintEvent(QPaintEvent *event) {
        QWidget::paintEvent(event);

        const auto contentsRect = this->contentsRect();

        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(this->color);
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
        // TODO: Notify Game
        std::cout << "Square pressed!\n";
    }
}
