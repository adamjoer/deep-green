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

    void reset();

    [[nodiscard]]
    std::vector<Chess::Position> getPossibleMoves(Chess::Position position) const;

    void move(Gui::Square &from, Gui::Square &to);

private slots:

    void squarePressed(Gui::Square &square);

    void clearHighlights();

    void zoomIn();

    void zoomOut();

    void about();

private:
    const static int SQUARE_SIZE_ADJUST_OFFSET = 5;

    Gui::Board *board;
    QLabel *turnLabel{nullptr};

    QAction *clearHighlightsAction{nullptr};
    QAction *zoomInAction{nullptr};
    QAction *zoomOutAction{nullptr};

    void createActions();

    void nextTurn();

    void setTurn(Chess::Color newTurn);

    Chess::Team whiteTeam;
    Chess::Team blackTeam;

    Chess::Color turn = Chess::Color::White;

    Gui::Square *highlightedSquare{nullptr};
};
