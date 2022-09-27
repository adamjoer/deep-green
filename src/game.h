#pragma once

#include <utility>
#include <array>
#include <vector>
#include <memory>

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QAction>

#include "chess/piece/piece.h"
#include "chess/team.h"
#include "gui/board.h"

class Game final : public QMainWindow {
Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);

    ~Game() override = default;

    void reset();

    [[nodiscard]]
    std::vector<Chess::Position> getPossibleMoves(Chess::Position position) const;

    bool move(const Chess::Position &from, const Chess::Position &to);

    [[nodiscard]]
    Chess::Color turn() const { return this->currentTurn; }

private slots:

    void zoomIn();

    void zoomOut();

    void about();

private:
    const static int SQUARE_SIZE_ADJUST_OFFSET = 5;

    Gui::Board *board;
    QLabel *turnLabel{nullptr};

    QAction *zoomInAction{nullptr};
    QAction *zoomOutAction{nullptr};
    QAction *disableBoardAction{nullptr};

    void createActions();

    Chess::Team whiteTeam;
    Chess::Team blackTeam;

    Chess::Color currentTurn = Chess::Color::White;
};
