#include "game.h"

#include <QLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

#include "chess/piece/piece.h"
#include "config.h"

// See https://stackoverflow.com/a/6852937/18713517
#define QUOTE(x) #x
#define TO_STRING(x) QUOTE(x)

Game::Game(QWidget *parent)
    : QMainWindow(parent),
      whiteTeam(Chess::Color::White),
      blackTeam(Chess::Color::Black),
      board(new Gui::Board(this)) {

    setWindowTitle(TO_STRING(PROJECT_NAME));

    board->addTeamStartingPosition(whiteTeam);
    board->addTeamStartingPosition(blackTeam);

    // Connect each of the GUI square's "pressed" signal to this object's
    // "squarePressed" slot.
    // See https://doc.qt.io/qt-6/signalsandslots.html
    for (const auto &row: this->board->getSquares()) {
        for (const auto square: row) {
            connect(square, &Gui::Square::pressed, this, &Game::squarePressed);
        }
    }

    setCentralWidget(this->board);

    createActions();

    this->turnLabel = new QLabel("It is White's turn");
    statusBar()->addPermanentWidget(this->turnLabel);

    statusBar()->setSizeGripEnabled(false);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    adjustSize();
    setFixedSize(size());
}

void Game::createActions() {
    QMenu *fileMenu = menuBar()->addMenu("&File");

    QAction *exitAct = fileMenu->addAction("E&xit", this, &QWidget::close);
    exitAct->setShortcut(Qt::CTRL | Qt::Key_Q);

    QMenu *editMenu = menuBar()->addMenu("&Edit");

    this->restartAction = editMenu->addAction("&Restart Game", this, &Game::restart);
    this->restartAction->setShortcut(Qt::CTRL | Qt::Key_R);

    QMenu *viewMenu = menuBar()->addMenu("&View");

    this->clearHighlightsAction
        = viewMenu->addAction("&Clear Highlights", this, &Game::clearHighlights);
    this->clearHighlightsAction->setShortcut(Qt::ALT | Qt::SHIFT | Qt::Key_X);

    viewMenu->addSeparator();

    this->zoomInAction = viewMenu->addAction("Zoom &In", this, &Game::zoomIn);
    this->zoomInAction->setShortcut(QKeySequence::ZoomIn);

    this->zoomOutAction = viewMenu->addAction("Zoom &Out", this, &Game::zoomOut);
    this->zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    QMenu *helpMenu = menuBar()->addMenu("&Help");

    helpMenu->addAction("&About", this, &Game::about);
}

/**
 * Slot designated to be invoked whenever a square has been pressed with the
 * mouse in the GUI.
 *
 * <p> This function handles highlighting the square and, if there is a chess piece
 * standing on the square, possible moves for that piece.
 * If a square has already been highlighted and the user presses a square which
 * is a possible move, the piece on the highlighted square will be moved to
 * the pressed square.
 *
 * @param square A reference to the GUI square object which was pressed
 */
void Game::squarePressed(Gui::Square &square) {
    if (&square == highlightedSquare) {
        assert(square.getState() == Gui::Square::State::Highlighted);
        clearHighlights();
        return;
    }

    if (square.getState() == Gui::Square::State::PossibleMove) {
        assert(highlightedSquare != nullptr);
        assert(!highlightedSquare->isEmpty());

        if (highlightedSquare->getPiece()->color == this->turn) {
            move(*highlightedSquare, square);

        } else {
            statusBar()->showMessage(
                (this->turn == Chess::Color::White) ? "Black cannot move right now"
                                                    : "White cannot move right now",
                2000);
        }
        return;
    }

    clearHighlights();
    if (square.isEmpty())
        return;

    highlightedSquare = &square;
    square.setState(Gui::Square::State::Highlighted);

    Chess::BoardState state{
        this->board->getBoardRepresentation(),
        square.getPosition(),
    };

    const auto possibleMoves = square.getPiece()->possibleMoves(state);
    this->board->highlightPossibleMoves(possibleMoves);
}

/**
 * Restart the game, so all pieces are standing at their starting positions,
 * and it is white's turn to move.
 */
void Game::restart() {
    setTurn(Chess::Color::White);

    this->highlightedSquare = nullptr;
    this->board->reset();

    whiteTeam.reset();
    blackTeam.reset();

    board->addTeamStartingPosition(whiteTeam);
    board->addTeamStartingPosition(blackTeam);

    statusBar()->showMessage("Restarted the game", 2000);
}

void Game::clearHighlights() {
    if (!this->highlightedSquare)
        return;

    this->board->clearHighlights();
    this->highlightedSquare = nullptr;
}

void Game::zoomIn() {
    this->board->setSquarePixelSize(
        this->board->getSquarePixelSize() + SQUARE_SIZE_ADJUST_OFFSET);
    setFixedSize(sizeHint());

    this->zoomOutAction->setDisabled(false);
}

void Game::zoomOut() {
    this->board->setSquarePixelSize(
        this->board->getSquarePixelSize() - SQUARE_SIZE_ADJUST_OFFSET);
    setFixedSize(sizeHint());

    if (this->board->getSquarePixelSize() <= 25)
        this->zoomOutAction->setDisabled(true);
}

void Game::about() {
    QMessageBox::about(this, "About " TO_STRING(PROJECT_NAME),
                       "<p><b>" TO_STRING(PROJECT_NAME) " v"
                       TO_STRING(VERSION_MAJOR) "."
                       TO_STRING(VERSION_MINOR) "."
                       TO_STRING(VERSION_PATCH)
                       "</b></p>"
                       "<p>" TO_STRING(DESCRIPTION) "</p>");
}

/**
 * Move the piece standing at the 'from' position to the 'to' position.
 * If there is a piece already standing on the 'to' position, that piece
 * is captured, and is therefore removed from the chess board.
 *
 * @param from The position where the moving piece is standing
 * @param to The position to which the piece will move
 */
void Game::move(Gui::Square &from, Gui::Square &to) {
    if (!to.isEmpty()) {
        const auto &piece = to.getPiece();
        if (piece->color == Chess::Color::White)
            whiteTeam.capturePiece(piece);
        else
            blackTeam.capturePiece(piece);
    }

    this->board->move(from.getPosition(), to.getPosition());

    clearHighlights();
    nextTurn();
}

void Game::nextTurn() {
    setTurn((this->turn == Chess::Color::White) ? Chess::Color::Black
                                                : Chess::Color::White);
}

void Game::setTurn(Chess::Color newTurn) {
    if (newTurn == this->turn)
        return;

    this->turn = newTurn;

    if (this->turn == Chess::Color::White)
        this->turnLabel->setText("It is White's turn");
    else
        this->turnLabel->setText("It is Black's turn");
}
