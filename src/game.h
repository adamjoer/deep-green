#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QAction>

#include "chess/team.h"
#include "gui/square.h"
#include "gui/board.h"

class Game final : public QMainWindow {
Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);

    ~Game() override = default;

private slots:

    void squarePressed(Gui::Square &square);

    void restart();

    void clearHighlights();

    void zoomIn();

    void zoomOut();

    void about();

private:
    const static int SQUARE_SIZE_ADJUST_OFFSET = 5;

    Gui::Board *board;
    QLabel *turnLabel;

    QAction *restartAction{nullptr};
    QAction *clearHighlightsAction{nullptr};
    QAction *zoomInAction{nullptr};
    QAction *zoomOutAction{nullptr};

    Chess::Team whiteTeam;
    Chess::Team blackTeam;

    Chess::Color turn = Chess::Color::White;

    Gui::Square *highlightedSquare{nullptr};

    void createActions();

    void move(Gui::Square &from, Gui::Square &to);

    void nextTurn();

    void setTurn(Chess::Color newTurn);
};
