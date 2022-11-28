#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QAction>
#include <QFuture>

#include "chess/board.h"
#include "gui/board.h"
#include "gui/square.h"

class Game final : public QMainWindow {
Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);

    ~Game() override;

private slots:

    void squarePressed(Gui::Square &square);

    void inputFen();

    void outputFen();

    void playAsWhite();

    void playAsBlack();

    void reset();

    void clearHighlights();

    void flipBoard();

    void zoomIn();

    void zoomOut();

    void about();

private:
    const static int SQUARE_SIZE_ADJUST_OFFSET = 40;

    Chess::Board chessBoard;

    Gui::Board *guiBoard;

    Chess::Color playerColor{Chess::Color::White};

    QLabel *turnLabel;

    QAction *inputFenAction{nullptr};
    QAction *outputFenAction{nullptr};
    QAction *resetAction{nullptr};
    QAction *clearHighlightsAction{nullptr};
    QAction *flipBoardAction{nullptr};
    QAction *zoomInAction{nullptr};
    QAction *zoomOutAction{nullptr};

    Gui::Square *highlightedSquare{nullptr};

    QFuture<Chess::Move> aiFuture;

    void createActions();

    void performMove(const Chess::Move &move);

    void performAiMove();

    void updateTurn();

    void setPlayerColor(Chess::Color color);
};
