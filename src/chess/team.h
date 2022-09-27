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
        static const int SIZE = 16;
        static const int QUEEN_COLUMN = 3;

        Team() = delete;

        explicit Team(Color color)
            : color(color) {
            reset();
        }

        ~Team() = default;

        void reset() {
            this->queen = std::make_shared<Queen>(color);

            this->officers.clear();
            this->officers = {
                std::make_shared<Rook>(this->color),
                std::make_shared<Knight>(this->color),
                std::make_shared<Bishop>(this->color),

                std::make_shared<King>(this->color),
                std::make_shared<Bishop>(this->color),
                std::make_shared<Knight>(this->color),
                std::make_shared<Rook>(this->color),
            };

            this->pawns.clear();
            for (int i = 0; i < SIZE / 2; ++i)
                this->pawns.push_back(std::make_shared<Pawn>(this->color));
        }

        [[nodiscard]]
        bool isFull() const {
            return this->queen &&
                   this->officers.size() == (SIZE / 2) - 1 &&
                   this->pawns.size() == (SIZE / 2);
        }

        [[nodiscard]]
        Color getColor() const {
            return this->color;
        }

        [[nodiscard]]
        const std::shared_ptr<Queen> &getQueen() const {
            return this->queen;
        }

        [[nodiscard]]
        const std::vector<std::shared_ptr<Piece>> &getOfficers() const {
            return this->officers;
        };

        [[nodiscard]]
        const std::vector<std::shared_ptr<Pawn>> &getPawns() const {
            return this->pawns;
        };

    private:
        const Color color;

        std::shared_ptr<Queen> queen;
        std::vector<std::shared_ptr<Piece>> officers;
        std::vector<std::shared_ptr<Pawn>> pawns;
    };
}
