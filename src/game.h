#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QAction>

#include "chess/board.h"
#include "gui/board.h"
#include "gui/square.h"

class Game final : public QMainWindow {
Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);

    ~Game() override = default;

private slots:

    void squarePressed(Gui::Square &square);

    void performMove(const Chess::Move &move);

    void inputFen();

    void outputFen();

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

    QLabel *turnLabel;

    QAction *inputFenAction{nullptr};
    QAction *outputFenAction{nullptr};
    QAction *resetAction{nullptr};
    QAction *clearHighlightsAction{nullptr};
    QAction *flipBoardAction{nullptr};
    QAction *zoomInAction{nullptr};
    QAction *zoomOutAction{nullptr};

    Gui::Square *highlightedSquare{nullptr};

    void createActions();

    void setTurn(Chess::Color newTurn);
};
