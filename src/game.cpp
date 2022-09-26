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
      whiteTeam(Chess::Color::White),
      blackTeam(Chess::Color::Black),
      board(new Gui::Board(this)) {

    setWindowTitle("Chess");

    board->addTeamStartingPosition(whiteTeam);
    board->addTeamStartingPosition(blackTeam);

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

    QMenu *viewMenu = menuBar()->addMenu("&View");

    this->zoomInAction = viewMenu->addAction("Zoom &In", this, &Game::zoomIn);
    this->zoomInAction->setShortcut(QKeySequence::ZoomIn);

    this->zoomOutAction = viewMenu->addAction("Zoom &Out", this, &Game::zoomOut);
    this->zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction("&About", this, &Game::about);
}

void Game::zoomIn() {
    this->board->zoom(SQUARE_SIZE_ADJUST_OFFSET);
    setFixedSize(sizeHint());

    this->zoomOutAction->setDisabled(false);
}

void Game::zoomOut() {
    this->board->zoom(-SQUARE_SIZE_ADJUST_OFFSET);
    setFixedSize(sizeHint());

    if (this->board->getSquarePixelSize() <= 25)
        this->zoomOutAction->setDisabled(true);
}

void Game::about() {
    QMessageBox::about(this, "About Chess",
                       "<p><b>Chess v"
                       TO_STRING(CHESS_VERSION_MAJOR) "."
                       TO_STRING(CHESS_VERSION_MINOR) "."
                       TO_STRING(CHESS_VERSION_PATCH)
                       "</b></p>"
                       "<p>" TO_STRING(CHESS_DESCRIPTION) "</p>");
}

/**
 * Reset the game, so all pieces are standing at their starting positions,
 * and it is white's turn to move.
 */
void Game::reset() {
    this->currentTurn = Chess::Color::White;
    whiteTeam.reset();
    blackTeam.reset();
}

/**
 * Get all the positions to which the piece standing at 'position' can move.
 *
 * @param position The position of the piece whose possible moves will be returned
 * @return A vector containing all the positions the piece can move to
 */
std::vector<Chess::Position> Game::getPossibleMoves(const Chess::Position position) const {
    return {};
}

/**
 * Move the piece standing at the 'from' position to the 'to' position.
 * If there is a piece already standing on the 'to' position, that piece
 * is captured, and is therefore removed from the chess board.
 *
 * @param from The position where the moving piece is standing
 * @param to The position to which the piece will move
 * @return Whether the piece at the 'from' position was moved
 */
bool Game::move(const Chess::Position &from, const Chess::Position &to) {
    return false;
}