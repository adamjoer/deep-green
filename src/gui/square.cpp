#include "square.h"

#include <QPainter>

namespace Gui {
    Square::Square(QWidget *parent, int rank, int file)
            : QWidget(parent),
              rank(rank),
              file(file),
              defaultColor((file + rank) % 2 == 0 ? DEFAULT_DARK_COLOR
                                                  : DEFAULT_LIGHT_COLOR),
              highlightColor((file + rank) % 2 == 0 ? HIGHLIGHT_DARK_COLOR
                                                    : HIGHLIGHT_LIGHT_COLOR),
              possibleMoveColor((file + rank) % 2 == 0 ? POSSIBLE_MOVE_DARK_COLOR
                                                       : POSSIBLE_MOVE_LIGHT_COLOR) {
    }

    void Square::paintEvent(QPaintEvent *event) {
        QWidget::paintEvent(event);

        auto contentsRect = this->contentsRect();

        QPainter painter(this);
        painter.setPen(Qt::NoPen);

        switch (this->state) {
            case State::Default:
            case State::PossibleMove:
                if (!this->isRecentMove()) {
                    painter.setBrush(this->defaultColor);
                    break;
                }

            case State::Highlighted:
                painter.setBrush(this->highlightColor);
                break;
        }
        painter.drawRect(contentsRect);

        if (this->state == State::PossibleMove) {
            // If a destination square is empty, it should contain a circle.
            // If it is occupied, it should contain a ring.

            painter.setBrush(this->possibleMoveColor);
            if (isEmpty()) {
                painter.drawEllipse(contentsRect.center(),
                                    contentsRect.width() / 6, contentsRect.height() / 6);

            } else {
                painter.drawEllipse(contentsRect.center(),
                                    contentsRect.width() / 2, contentsRect.height() / 2);

                painter.setBrush(!this->isRecentMove() ? this->defaultColor : this->highlightColor);
                painter.drawEllipse(contentsRect.center(),
                                    (contentsRect.width() / 2) - (contentsRect.width() / 10),
                                    (contentsRect.height() / 2) - (contentsRect.height() / 10));
            }
        }

        auto font = painter.font();

        if (this->file == 0 || this->rank == 0) {
            font.setPixelSize(contentsRect.height() / 3);
            painter.setFont(font);
            painter.setPen(defaultColor.rgb() == DEFAULT_DARK_COLOR ? DEFAULT_LIGHT_COLOR
                                                                    : DEFAULT_DARK_COLOR);

            contentsRect -= {2, 0, 2, 0};

            if (this->file == 0) {
                painter.drawText(contentsRect, Qt::AlignLeft | Qt::AlignTop,
                                 QString(QChar('1' + this->rank)));
            }

            if (this->rank == 0) {
                painter.drawText(contentsRect, Qt::AlignRight | Qt::AlignBottom,
                                 QString(QChar('a' + this->file)));
            }
        }

        if (!isEmpty()) {
            font.setPixelSize(contentsRect.height());
            painter.setFont(font);
            painter.setPen(QColorConstants::Black);
            painter.drawText(contentsRect, Qt::AlignCenter,
                             QString(QChar(this->piece->symbol())));
        }
    }

    void Square::mousePressEvent(QMouseEvent *event) {
        // Emit "pressed" signal
        emit pressed(*this);
    }

    void Square::setPiece(std::optional<Piece> newPiece) {
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

    void Square::setRecentMove(bool isRecentMove) {
        if (isRecentMove == this->recentMove)
            return;

        this->recentMove = isRecentMove;
        update();
    }

    bool Piece::operator==(const Piece &rhs) const {
        return type == rhs.type &&
               color == rhs.color;
    }

    bool Piece::operator!=(const Piece &rhs) const {
        return !(rhs == *this);
    }

    wchar_t Piece::symbol() const {
        static const wchar_t symbols[2][6]{
                {L'♔', L'♕', L'♖', L'♗', L'♘', L'♙'},
                {L'♚', L'♛', L'♜', L'♝', L'♞', L'♟'},
        };

        return symbols[static_cast<int>(this->color)][static_cast<int>(this->type)];
    }

    std::wostream &operator<<(std::wostream &os, const Piece &piece) {
        return os << piece.symbol();
    }
}
