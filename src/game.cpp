#include "game.h"

#include <QLayout>
#include <QMenuBar>
#include <QActionGroup>
#include <QStatusBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QtConcurrent>

#include "config.h"
#include "ai/brain.h"

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
    updateTurn();

    statusBar()->setSizeGripEnabled(false);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    adjustSize();
    setFixedSize(size());
}

Game::~Game() {
    if (this->aiFuture.isRunning()) {
        this->aiFuture.cancel();
        this->aiFuture.waitForFinished();
    }
}

void Game::createActions() {
    QMenu *fileMenu = menuBar()->addMenu("&File");

    QAction *exitAct = fileMenu->addAction("E&xit", this, &QWidget::close);
    exitAct->setShortcut(Qt::CTRL | Qt::Key_Q);

    QMenu *editMenu = menuBar()->addMenu("&Edit");

    this->inputFenAction = editMenu->addAction("&Input FEN", this, &Game::inputFen);
    this->inputFenAction->setShortcut(Qt::CTRL | Qt::Key_I);

    this->outputFenAction = editMenu->addAction("&Output FEN", this, &Game::outputFen);
    this->outputFenAction->setShortcut(Qt::CTRL | Qt::Key_O);

    editMenu->addSeparator();

    auto *playAsWhiteAction = editMenu->addAction("Play as White", this, &Game::playAsWhite);
    playAsWhiteAction->setCheckable(true);

    auto *playAsBlackAction = editMenu->addAction("Play as Black", this, &Game::playAsBlack);
    playAsBlackAction->setCheckable(true);

    editMenu->addSeparator();

    this->resetAction = editMenu->addAction("&Reset Game", this, &Game::reset);
    this->resetAction->setShortcut(Qt::CTRL | Qt::Key_R);

    QMenu *viewMenu = menuBar()->addMenu("&View");

    this->clearHighlightsAction
            = viewMenu->addAction("&Clear Highlights", this, &Game::clearHighlights);
    this->clearHighlightsAction->setShortcut(Qt::ALT | Qt::SHIFT | Qt::Key_X);

    this->flipBoardAction = viewMenu->addAction("&Flip Board", this, &Game::flipBoard);
    this->flipBoardAction->setShortcut(Qt::CTRL | Qt::Key_F);

    viewMenu->addSeparator();

    this->zoomInAction = viewMenu->addAction("Zoom &In", this, &Game::zoomIn);
    this->zoomInAction->setShortcut(QKeySequence::ZoomIn);

    this->zoomOutAction = viewMenu->addAction("Zoom &Out", this, &Game::zoomOut);
    this->zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    QMenu *helpMenu = menuBar()->addMenu("&Help");

    helpMenu->addAction("&About", this, &Game::about);

    auto *teamColorGroup = new QActionGroup(this);
    teamColorGroup->addAction(playAsWhiteAction);
    teamColorGroup->addAction(playAsBlackAction);
    playAsWhiteAction->setChecked(true);
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

        if (highlightedSquare->getPiece()->color == this->playerColor) {
            performMove(Chess::Move(highlightedSquare->getPosition(), square.getPosition(),
                                    square.isEmpty() ? std::nullopt
                                                     : std::make_optional(square.getPiece()->type)));

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

void Game::performMove(const Chess::Move &move) {
    this->chessBoard.performMove(move);
    this->guiBoard->performMove(move.from, move.to);

    clearHighlights();

    updateTurn();
}

void Game::performAiMove() {
    if (this->aiFuture.isRunning()) {
        this->aiFuture.cancel();
        this->aiFuture.waitForFinished();
    }

    (this->aiFuture = QtConcurrent::run(Ai::selectMove, this->chessBoard))
            .then([&](Chess::Move move) {
                Game::performMove(move);
            });
}

void Game::inputFen() {
    bool ok;
    const auto input = QInputDialog::getText(this, "Input FEN", "FEN string:",
                                             QLineEdit::Normal, {}, &ok, {},
                                             Qt::ImhNoAutoUppercase | Qt::ImhNoPredictiveText);
    if (!ok)
        return;

    const auto inputStd = input.toStdString();
    if (!Chess::Board::isValidFen(inputStd)) {
        // FIXME: Notify user in a bit more elegant way?
        statusBar()->showMessage("Invalid FEN", 2000);
        return;
    }

    if (this->aiFuture.isRunning()) {
        this->aiFuture.cancel();
        this->aiFuture.waitForFinished();
    }

    chessBoard.parseFen(inputStd);

    this->highlightedSquare = nullptr;
    this->guiBoard->set(this->chessBoard);
    updateTurn();
}

void Game::outputFen() {
    QMessageBox messageBox(this);
    messageBox.setWindowTitle("Output FEN");
    messageBox.setText(QString(this->chessBoard.generateFen().c_str()));
    messageBox.setTextInteractionFlags(Qt::TextSelectableByMouse);
    messageBox.exec();
}

void Game::playAsWhite() {
    setPlayerColor(Chess::Color::White);
}

void Game::playAsBlack() {
    setPlayerColor(Chess::Color::Black);
}

void Game::setPlayerColor(Chess::Color color) {
    if (this->playerColor == color)
        return;

    this->playerColor = color;

    auto targetOriginCorner = (this->playerColor == Chess::Color::White) ? Qt::BottomLeftCorner
                                                                         : Qt::TopRightCorner;
    if (this->guiBoard->getOriginCorner() != targetOriginCorner)
        this->guiBoard->flip();

    reset();
}

/**
 * Reset the game, so all pieces are standing at their starting positions,
 * and it is white's turn to move.
 */
void Game::reset() {
    if (this->aiFuture.isRunning()) {
        this->aiFuture.cancel();
        this->aiFuture.waitForFinished();
    }

    this->chessBoard.reset();

    this->guiBoard->set(this->chessBoard);

    this->highlightedSquare = nullptr;
    updateTurn();

    statusBar()->showMessage("Game reset", 2000);
}

void Game::clearHighlights() {
    if (!this->highlightedSquare)
        return;

    this->guiBoard->clearHighlights();
    this->highlightedSquare = nullptr;
}

void Game::flipBoard() {
    this->guiBoard->flip();
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

void Game::updateTurn() {
    if (this->chessBoard.turnToMove() == Chess::Color::White) {
        this->turnLabel->setText("It is White's turn to move");

    } else {
        this->turnLabel->setText("It is Black's turn to move");
    }

    if (this->chessBoard.turnToMove() != this->playerColor)
        performAiMove();
}
