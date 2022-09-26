#pragma once

#include <memory>
#include <vector>
#include <memory>

#include "piece/piece.h"
#include "piece/king.h"
#include "piece/queen.h"
#include "piece/rook.h"
#include "piece/bishop.h"
#include "piece/knight.h"
#include "piece/pawn.h"

namespace Chess {

    class Team {
    public:
        static const int SIZE = 8;
        static const int QUEEN_COLUMN = 3;

        Team() = delete;

        explicit Team(Color color)
            : color(color) {
            reset();
        }

        ~Team() = default;

        void reset() {
            queen = std::make_shared<Queen>(color);

            officers.clear();
            officers = {
                std::make_shared<Rook>(color),
                std::make_shared<Knight>(color),
                std::make_shared<Bishop>(color),

                std::make_shared<King>(color),
                std::make_shared<Bishop>(color),
                std::make_shared<Knight>(color),
                std::make_shared<Rook>(color),
            };

            pawns.clear();
            for (int i = 0; i < SIZE; ++i)
                pawns.push_back(std::make_shared<Pawn>(color));
        }

        [[nodiscard]] bool isFull() const {
            return queen &&
                   officers.size() == SIZE - 1 &&
                   pawns.size() == SIZE;
        }

        [[nodiscard]] Color getColor() const {
            return color;
        }

        [[nodiscard]] const std::shared_ptr<Queen> &getQueen() const {
            return queen;
        }

        [[nodiscard]] const std::vector<std::shared_ptr<Piece>> &getOfficers() const {
            return officers;
        };

        [[nodiscard]] const std::vector<std::shared_ptr<Pawn>> &getPawns() const {
            return pawns;
        };

    private:
        const Color color;

        std::shared_ptr<Queen> queen;
        std::vector<std::shared_ptr<Piece>> officers;
        std::vector<std::shared_ptr<Pawn>> pawns;
    };
}
