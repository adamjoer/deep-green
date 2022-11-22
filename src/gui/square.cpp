#include "square.h"
#include "board.h"

#include <QPainter>

namespace Gui {
    Square::Square(QWidget *parent, int rank, int file)
            : QWidget(parent),
              rank(rank),
              file(file),
              defaultColor((file + rank) % 2 == 0 ? DEFAULT_DARK_COLOR
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
                             QString(QChar(this->piece->symbol())));
        }

        const auto *parentBoard = dynamic_cast<Board *>(parent());
        if (!parentBoard)
            return;

        const auto boardOrigin = parentBoard->getBoardOriginCorner();
        const int algebraicNotationFile = boardOrigin == Qt::Corner::BottomLeftCorner ? 0 : 7;
        const int algebraicNotationRank = boardOrigin == Qt::Corner::BottomLeftCorner ? 0 : 7;

        if (this->file == algebraicNotationFile ||
            this->rank == algebraicNotationRank) {
            font.setPixelSize(contentsRect.height() / 3);
            painter.setFont(font);
            painter.setPen(defaultColor.rgb() == DEFAULT_DARK_COLOR ? DEFAULT_LIGHT_COLOR
                                                                    : DEFAULT_DARK_COLOR);

            contentsRect -= {2, 0, 2, 0};

            if (this->file == algebraicNotationFile) {
                painter.drawText(contentsRect, Qt::AlignLeft | Qt::AlignTop,
                                 QString(QChar('1' + this->rank)));
            }

            if (this->rank == algebraicNotationRank) {
                painter.drawText(contentsRect, Qt::AlignRight | Qt::AlignBottom,
                                 QString(QChar('a' + this->file)));
            }
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
