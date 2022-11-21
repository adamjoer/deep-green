#include "game.h"

#include <QLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

#include "config.h"

// See https://stackoverflow.com/a/6852937/18713517
#define QUOTE(x) #x
#define TO_STRING(x) QUOTE(x)

Game::Game(QWidget *parent)
        : QMainWindow(parent),
          guiBoard(new Gui::Board(this, chessBoard)) {

    setWindowTitle(TO_STRING(PROJECT_NAME));

    // Connect each of the GUI square's "pressed" signal to this object's
    // "squarePressed" slot.
    // See https://doc.qt.io/qt-6/signalsandslots.html
    for (const auto square: this->guiBoard->getSquares()) {
        connect(square, &Gui::Square::pressed, this, &Game::squarePressed);
    }

    setCentralWidget(this->guiBoard);

    createActions();

    this->turnLabel = new QLabel();
    statusBar()->addPermanentWidget(this->turnLabel);
    setTurn(this->chessBoard.turnToMove());

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

    this->resetAction = editMenu->addAction("&Reset Game", this, &Game::reset);
    this->resetAction->setShortcut(Qt::CTRL | Qt::Key_R);

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

        if (highlightedSquare->getPiece()->color == this->chessBoard.turnToMove()) {
            chessBoard.performMove(Chess::Move(highlightedSquare->getPosition(), square.getPosition(),
                                               square.isEmpty() ? std::nullopt
                                                                : std::make_optional(square.getPiece()->type)));
            guiBoard->performMove(highlightedSquare->getPosition(), square.getPosition());

            clearHighlights();

            setTurn(this->chessBoard.turnToMove());

        } else {
            statusBar()->showMessage("It is not this team's turn to move right now", 2000);
        }
        return;
    }

    clearHighlights();
    if (square.isEmpty())
        return;

    highlightedSquare = &square;
    square.setState(Gui::Square::State::Highlighted);

    const auto moves = chessBoard.pseudoLegalMoves(square.getPosition(), square.getPiece()->color);
    guiBoard->highlightPossibleMoves(moves);
}

/**
 * Reset the game, so all pieces are standing at their starting positions,
 * and it is white's turn to move.
 */
void Game::reset() {
    setTurn(Chess::Color::White);

    clearHighlights();

    this->chessBoard.reset();
    this->guiBoard->set(this->chessBoard);

    statusBar()->showMessage("Game reset", 2000);
}

void Game::clearHighlights() {
    if (!this->highlightedSquare)
        return;

    this->guiBoard->clearHighlights();
    this->highlightedSquare = nullptr;
}

void Game::zoomIn() {
    this->guiBoard->setPixelSize(
            this->guiBoard->getPixelSize() + SQUARE_SIZE_ADJUST_OFFSET);
    setFixedSize(sizeHint());

    this->zoomOutAction->setDisabled(false);
}

void Game::zoomOut() {
    this->guiBoard->setPixelSize(
            this->guiBoard->getPixelSize() - SQUARE_SIZE_ADJUST_OFFSET);
    setFixedSize(sizeHint());

    if (this->guiBoard->getPixelSize() <= 200)
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

void Game::setTurn(Chess::Color newTurn) {
    if (newTurn == Chess::Color::White)
        this->turnLabel->setText("It is White's turn to move");
    else
        this->turnLabel->setText("It is Black's turn to move");
}
