#include "board.h"

#include <sstream>

namespace Chess {
    static constexpr Bitboard twoRank{Square::A2, Square::B2, Square::C2, Square::D2,
                                      Square::E2, Square::F2, Square::G2, Square::H2};
    static constexpr Bitboard sevenRank{Square::A7, Square::B7, Square::C7, Square::D7,
                                        Square::E7, Square::F7, Square::G7, Square::H7};

    static constexpr std::array<std::array<Bitboard, 6>, 2> startingPosition{
            {
                    {
                            Bitboard(Square::E1),
                            Bitboard(Square::D1),
                            Bitboard(Square::A1, Square::H1),
                            Bitboard(Square::C1, Square::F1),
                            Bitboard(Square::B1, Square::G1),
                            twoRank,
                    },
                    {
                            Bitboard(Square::E8),
                            Bitboard(Square::D8),
                            Bitboard(Square::A8, Square::H8),
                            Bitboard(Square::C8, Square::F8),
                            Bitboard(Square::B8, Square::G8),
                            sevenRank,
                    },
            }
    };

    const std::array<std::array<Bitboard, 64>, 8> Board::attackRayMasks{
            generateAttackRayMasks(Direction::NorthWest),
            generateAttackRayMasks(Direction::North),
            generateAttackRayMasks(Direction::NorthEast),
            generateAttackRayMasks(Direction::East),
            generateAttackRayMasks(Direction::SouthEast),
            generateAttackRayMasks(Direction::South),
            generateAttackRayMasks(Direction::SouthWest),
            generateAttackRayMasks(Direction::West),
    };

    const std::array<Bitboard, 64> Board::knightAttackMasks = generateKnightAttackMasks();

    const std::array<Bitboard, 64> Board::kingAttackMasks = generateKingAttackMasks();

    const std::array<std::array<Bitboard, 64>, 2> Board::pawnAttackMasks = generatePawnAttackMasks();

    Board::Board() {
        reset();
    }

    Board::Board(const std::string &fen) {
        parseFen(fen);
    }

    void Board::reset() {
        this->bitboards = startingPosition;
        this->playerTurn = Color::White;
    }

    void Board::performMove(Move move) {
        assert(isMovePseudoLegal(move));

        // TODO: Check for "en passant" square

        auto &team = this->bitboards[static_cast<int>(this->playerTurn)];

        auto piece = removePieceAt(move.from, this->playerTurn);
        team[static_cast<int>(piece)].setOccupancyAt(move.to);

        if (move.dropPiece) {
            auto &enemyTeam
                    = this->bitboards[static_cast<int>(oppositeTeam(this->playerTurn))];
            enemyTeam[static_cast<int>(*move.dropPiece)].clearOccupancyAt(move.to);
        }

        // TODO: Set "en passant" square if relevant

        // TODO: Reset halfMoveCounter if capture or pawn performMove.
        ++this->halfMoveCounter;
        if (this->playerTurn == Color::Black)
            ++this->fullMoveCounter;

        this->enPassant = Square::None;

        this->playerTurn = oppositeTeam(playerTurn);
    }

    void Board::undoMove(Move move) {
        // TODO
    }

    Color Board::turnToMove() const {
        return this->playerTurn;
    }

    void Board::parseFen(const std::string &fen) {
        auto charToPiece = [](char c) -> PieceType {
            switch (toupper(c)) {
                case 'K':
                    return PieceType::King;
                case 'Q':
                    return PieceType::Queen;
                case 'R':
                    return PieceType::Rook;
                case 'B':
                    return PieceType::Bishop;
                case 'N':
                    return PieceType::Knight;
                default:
                    return PieceType::Pawn;
            }
        };

        auto itr = fen.begin();

        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (std::isupper(*itr)) { // White piece
                    bitboards[0][static_cast<int>(charToPiece(*itr))].setOccupancyAt(Square(rank * 8 + file));

                } else if (std::islower(*itr)) { // Black piece
                    bitboards[1][static_cast<int>(charToPiece(*itr))].setOccupancyAt(Square(rank * 8 + file));

                } else if (std::isdigit(*itr)) { // Empty squares
                    file += *itr - '1';
                }
                itr++;
            }
            itr++;
        }

        // Set player turn
        playerTurn = (*itr == 'w') ? Color::White : Color::Black;
        itr += 2;

        // Set castling bits
        castlingRights = 0;
        if (*itr == 'K') {
            castlingRights |= static_cast<uint8_t>(CastlingRightFlag::WhiteKing);
            itr++;
        }
        if (*itr == 'Q') {
            castlingRights |= static_cast<uint8_t>(CastlingRightFlag::WhiteQueen);
            itr++;
        }
        if (*itr == 'k') {
            castlingRights |= static_cast<uint8_t>(CastlingRightFlag::BlackKing);
            itr++;
        }
        if (*itr == 'q') {
            castlingRights |= static_cast<uint8_t>(CastlingRightFlag::BlackQueen);
            itr++;
        }
        if (!castlingRights)
            itr++;
        itr++;
        // En passant parsing.
        if (*itr == '-') {
            enPassant = Square::None;
            itr += 2;

        } else {
            int file = *itr - 'a'; // Zero indexed file number
            itr++;
            int rank = *itr - '1'; // Zero indexed rank number
            enPassant = Square(rank * 8 + file);
            itr += 2;
        }

        if (*itr == '0') {
            halfMoveCounter = 0;
            itr++;

        } else {
            halfMoveCounter = *itr - '0';
            itr++;
            if (*itr != ' ') {
                halfMoveCounter = halfMoveCounter * 10 + *itr - '0';
                itr++;
            }
        }

        itr++;
        fullMoveCounter = 0;
        while (itr != fen.end()) {
            fullMoveCounter = fullMoveCounter * 10 + *itr - '0';
            itr++;
        }
    }

    std::string Board::generateFen() const {
        std::ostringstream result;

        Bitboard fullBoard;
        for (auto &bitboard: bitboards)
            for (auto bb: bitboard)
                fullBoard |= bb;

        static const char *const pieces = "KQRBNPkqrbnp";

        for (int rank = 7; rank > -1; --rank) {
            for (int file = 0; file < 8; ++file) {

                auto square = static_cast<Square>(rank * 8 + file);
                if (fullBoard.isOccupiedAt(square))
                    for (int i = 0; i < 12; ++i) {
                        if (bitboards[i / 6][i % 6].isOccupiedAt(square)) {
                            result << pieces[i];
                            break;
                        }
                    }

                else {
                    char blankSpaceCounter{'0'};
                    while (!fullBoard.isOccupiedAt(square) and file < 8) {
                        ++blankSpaceCounter;
                        ++file;
                        square = Square(rank * 8 + file);
                    }
                    result << blankSpaceCounter;
                    --file; // Fixes increment error
                }
            }
            if (rank > 0)
                result << '/';
            else
                result << ' ';
        }

        result << (playerTurn == Color::White ? "w " : "b ");

        if (castlingRights & static_cast<uint8_t>(CastlingRightFlag::WhiteKing))
            result << 'K';
        if (castlingRights & static_cast<uint8_t>(CastlingRightFlag::WhiteQueen))
            result << 'Q';
        if (castlingRights & static_cast<uint8_t>(CastlingRightFlag::BlackKing))
            result << 'k';
        if (castlingRights & static_cast<uint8_t>(CastlingRightFlag::BlackQueen))
            result << 'q';
        if (!castlingRights)
            result << '-';
        result << ' ';

        if (enPassant == Square::None) {
            result << "- ";
        } else {
            result << enPassant << ' ';
        }

        result << halfMoveCounter << ' ' << fullMoveCounter;

        return result.str();
    }

    std::vector<Move> Board::pseudoLegalMoves() const {
        std::vector<Move> moves;

        for (int pieceIndex = 0; pieceIndex < 6; ++pieceIndex)
            pseudoLegalMoves(PieceType(pieceIndex), moves);

        return moves;
    }

    std::vector<Move> Board::pseudoLegalMoves(PieceType piece) const {
        std::vector<Move> moves;
        pseudoLegalMoves(piece, moves);
        return moves;
    }

    void Board::pseudoLegalMoves(PieceType piece, std::vector<Move> &moves) const {
        Bitboard bitboard = this->bitboards[static_cast<int>(this->playerTurn)][static_cast<int>(piece)];

        for (int i = 0; i < 64; ++i) {
            auto from = Square(i);
            if (!bitboard.isOccupiedAt(from))
                continue;

            pseudoLegalMoves(from, moves);
        }
    }

    std::vector<Move> Board::pseudoLegalMoves(Chess::Square square) const {
        std::vector<Move> moves;
        pseudoLegalMoves(square, moves);
        return moves;
    }

    void Board::pseudoLegalMoves(Square square, std::vector<Move> &moves) const {
        pseudoLegalMoves(square, this->playerTurn, moves);
    }

    std::vector<Move> Board::pseudoLegalMoves(Square square, Color color) const {
        std::vector<Move> moves;
        pseudoLegalMoves(square, color, moves);
        return moves;
    }

    void Board::pseudoLegalMoves(Square square, Color color, std::vector<Move> &moves) const {
        const auto ourSquares = teamOccupiedSquares(color);
        const auto enemySquares = teamOccupiedSquares(oppositeTeam(color));
        const auto occupiedSquares = ourSquares | enemySquares;

        const auto piece = pieceAt(square);

        Bitboard attacks;
        switch (piece) {
            case PieceType::King:
                attacks = kingAttacks(square);
                break;
            case PieceType::Queen:
                attacks = queenAttacks(square, occupiedSquares);
                break;
            case PieceType::Rook:
                attacks = rookAttacks(square, occupiedSquares);
                break;
            case PieceType::Bishop:
                attacks = bishopAttacks(square, occupiedSquares);
                break;
            case PieceType::Knight:
                attacks = knightAttacks(square);
                break;
            case PieceType::Pawn:
                attacks = pawnAttacks(square, occupiedSquares, color);
                break;
        }

        if (!attacks)
            return;

        attacks &= ~ourSquares;

        for (int j = 0; j < 64; ++j) {
            auto to = Square(j);
            if (!attacks.isOccupiedAt(to))
                continue;

            moves.emplace_back(square, to, enemySquares.isOccupiedAt(to)
                                           ? std::make_optional(pieceAt(to))
                                           : std::nullopt);
        }
    }

    bool Board::isMovePseudoLegal(Move move) const {
        if (move.from == Square::None || move.to == Square::None)
            return false;

        const auto legalMoves = pseudoLegalMoves(move.from);
        return std::find(legalMoves.begin(), legalMoves.end(), move)
               != legalMoves.end();
    }

    Bitboard Board::teamOccupiedSquares(Color color) const {
        Bitboard occupiedSquares;

        auto team = this->bitboards[static_cast<int>(color)];
        for (auto bitboard: team)
            occupiedSquares |= bitboard;

        return occupiedSquares;
    }

    PieceType Board::pieceAt(Chess::Square square) const {
        for (auto team: this->bitboards) {
            for (int i = 0; i < team.size(); ++i) {
                if (team[i].isOccupiedAt(square))
                    return PieceType(i);
            }
        }

        assert(false);
        return PieceType::Pawn;
    }

    PieceType Board::pieceAt(Chess::Square square, Color color) const {
        auto team = this->bitboards[static_cast<int>(color)];
        for (int i = 0; i < team.size(); ++i) {
            if (team[i].isOccupiedAt(square))
                return PieceType(i);
        }

        assert(false);
        return PieceType::Pawn;
    }

    PieceType Board::removePieceAt(Square square) {
        for (auto &team: this->bitboards) {
            for (int i = 0; i < team.size(); ++i) {
                auto &bitboard = team[i];
                if (bitboard.isOccupiedAt(square)) {
                    bitboard.clearOccupancyAt(square);
                    return PieceType(i);
                }
            }
        }

        assert(false);
        return PieceType::Pawn;
    }

    PieceType Board::removePieceAt(Square square, Color color) {
        auto &team = this->bitboards[static_cast<int>(color)];
        for (int i = 0; i < team.size(); ++i) {
            auto &bitboard = team[i];
            if (bitboard.isOccupiedAt(square)) {
                bitboard.clearOccupancyAt(square);
                return PieceType(i);
            }
        }

        assert(false);
        return PieceType::Pawn;
    }

    Bitboard Board::rookAttacks(Square square, Bitboard occupiedSquares) {
        return {
                slidingAttack(square, Direction::North, occupiedSquares) |
                slidingAttack(square, Direction::East, occupiedSquares) |
                slidingAttack(square, Direction::South, occupiedSquares) |
                slidingAttack(square, Direction::West, occupiedSquares)
        };
    }

    Bitboard Board::bishopAttacks(Square square, Bitboard occupiedSquares) {
        return {
                slidingAttack(square, Direction::NorthWest, occupiedSquares) |
                slidingAttack(square, Direction::NorthEast, occupiedSquares) |
                slidingAttack(square, Direction::SouthEast, occupiedSquares) |
                slidingAttack(square, Direction::SouthWest, occupiedSquares)
        };
    }

    Bitboard Board::knightAttacks(Square square) {
        return knightAttackMasks[static_cast<int>(square)];
    }

    Bitboard Board::kingAttacks(Square square) {
        // TODO: Add castling moves, if castling bits are 1. Use playerTurn variable to switch.
        // TODO: All other attack methods need to check if square is threatened by BRQ, then check possible pin.
        // TODO: King may not move into check or stay in check. Suggestion: Check all legal moves generated and
        //  see whether the move makes king non-threatened. This is also an alternative to pin checks, and provides
        //  a clear end condition - if no move exists after this check, then it's checkmate and opposing player wins.
        return kingAttackMasks[static_cast<int>(square)];
    }

    Bitboard Board::queenAttacks(Square square, Bitboard occupiedSquares) {
        return {
                rookAttacks(square, occupiedSquares) |
                bishopAttacks(square, occupiedSquares)
        };
    }

    Bitboard Board::pawnAttacks(Square square, Bitboard occupiedSquares, Color color) {
        Direction attackDirection;
        Bitboard startRank;
        Square captureEast;
        Square captureWest;

        switch (color) {
            case Color::White:
                attackDirection = Direction::North;
                startRank = twoRank;
                captureEast = Bitboard::squareToThe(Direction::NorthEast, square);
                captureWest = Bitboard::squareToThe(Direction::NorthWest, square);
                break;

            case Color::Black:
                attackDirection = Direction::South;
                startRank = sevenRank;
                captureEast = Bitboard::squareToThe(Direction::SouthEast, square);
                captureWest = Bitboard::squareToThe(Direction::SouthWest, square);
                break;
        }

        Bitboard attacks;

        auto attackMask = pawnAttackMasks[static_cast<int>(color)][static_cast<int>(square)];
        if (!attackMask.isOverlappingWith(occupiedSquares)) {
            attacks = attackMask;

        } else if (startRank.isOccupiedAt(square)) {
            const auto nextSquare = Bitboard::squareToThe(attackDirection, square);
            if (!occupiedSquares.isOccupiedAt(nextSquare))
                attacks.setOccupancyAt(nextSquare);
        }

        if (captureEast != Square::None && occupiedSquares.isOccupiedAt(captureEast))
            attacks.setOccupancyAt(captureEast);

        if (captureWest != Square::None && occupiedSquares.isOccupiedAt(captureWest))
            attacks.setOccupancyAt(captureWest);

        return attacks;
    }

    Bitboard Board::slidingAttack(Square square, Direction direction,
                                  Bitboard occupiedSquares) {
        auto attackRay = attackRayMasks[static_cast<int>(direction)][static_cast<int>(square)];

        if (auto blockers = attackRay & occupiedSquares) {

            int blockerIndex;
            switch (direction) {
                case Direction::NorthWest:
                case Direction::North:
                case Direction::NorthEast:
                case Direction::East:
                    blockerIndex = blockers.bitScanForward();
                    break;

                case Direction::SouthEast:
                case Direction::South:
                case Direction::SouthWest:
                case Direction::West:
                    blockerIndex = blockers.bitScanReverse();
                    break;

                default:
                    assert(false);
                    return {};
            }

            attackRay ^= attackRayMasks[static_cast<int>(direction)][blockerIndex];
        }

        return attackRay;
    }

    std::array<Bitboard, 64> Board::generateAttackRayMasks(Direction direction) {
        std::array<Bitboard, 64> ray;

        for (int i = 0; i < ray.size(); i++)
            ray[i] = generateAttackRayMask(direction, Square(i));

        return ray;
    }

    Bitboard Board::generateAttackRayMask(Direction direction, Square square) {
        Bitboard ray;

        while (true) {
            square = Bitboard::squareToThe(direction, square);
            if (square == Square::None)
                break;

            ray.setOccupancyAt(square);
        }

        return ray;
    }

    std::array<Bitboard, 64> Board::generateKnightAttackMasks() {
        std::array<Bitboard, 64> rookAttacks;

        for (int i = 0; i < rookAttacks.size(); ++i) {
            rookAttacks[i] = generateKnightAttackMask(Square(i));
        }

        return rookAttacks;
    }

    Bitboard Board::generateKnightAttackMask(Square square) {
        Bitboard attack;

        auto generateKnightAttack = [&](auto square, auto direction) {
            square = Bitboard::squareToThe(direction, square);
            if (square == Square::None)
                return;

            Square left;
            Square right;
            switch (direction) {
                case Direction::North:
                    left = Bitboard::squareToThe(Direction::NorthWest, square);
                    right = Bitboard::squareToThe(Direction::NorthEast, square);
                    break;
                case Direction::East:
                    left = Bitboard::squareToThe(Direction::NorthEast, square);
                    right = Bitboard::squareToThe(Direction::SouthEast, square);
                    break;
                case Direction::South:
                    left = Bitboard::squareToThe(Direction::SouthWest, square);
                    right = Bitboard::squareToThe(Direction::SouthEast, square);
                    break;
                case Direction::West:
                    left = Bitboard::squareToThe(Direction::SouthWest, square);
                    right = Bitboard::squareToThe(Direction::NorthWest, square);
                    break;
                default:
                    left = right = Square::None;
                    break;
            }

            if (left != Square::None)
                attack.setOccupancyAt(left);
            if (right != Square::None)
                attack.setOccupancyAt(right);
        };

        generateKnightAttack(square, Direction::North);
        generateKnightAttack(square, Direction::East);
        generateKnightAttack(square, Direction::South);
        generateKnightAttack(square, Direction::West);

        return attack;
    }

    std::array<Bitboard, 64> Board::generateKingAttackMasks() {
        std::array<Bitboard, 64> attacks;

        for (int i = 0; i < attacks.size(); ++i) {
            attacks[i] = generateKingAttackMask(Square(i));
        }

        return attacks;
    }

    Bitboard Board::generateKingAttackMask(Chess::Square square) {
        Bitboard attackMask;

        for (int i = 0; i < 8; i++) {
            auto attackSquare = Bitboard::squareToThe(Direction(i), square);
            if (attackSquare != Square::None)
                attackMask.setOccupancyAt(attackSquare);
        }
        return attackMask;
    }

    std::array<std::array<Bitboard, 64>, 2> Board::generatePawnAttackMasks() {
        std::array<std::array<Bitboard, 64>, 2> attackMasks;

        for (int i = 0; i < attackMasks.size(); ++i) {
            for (int j = 0; j < attackMasks[i].size(); ++j) {
                attackMasks[i][j] = generatePawnAttackMask(Square(j), Color(i));
            }
        }

        return attackMasks;
    }

    Bitboard Board::generatePawnAttackMask(Square square, Color color) {
        Direction attackDirection;
        switch (color) {
            case Color::Black:
                attackDirection = Direction::South;
                break;
            case Color::White:
                attackDirection = Direction::North;
                break;
        }

        Bitboard attack;

        auto attackSquare = Bitboard::squareToThe(attackDirection, square);
        if (attackSquare != Square::None)
            attack.setOccupancyAt(attackSquare);

        auto startRank = (color == Color::White) ? twoRank : sevenRank;
        if (startRank.isOccupiedAt(square))
            attack.setOccupancyAt(Bitboard::squareToThe(attackDirection, attackSquare));

        return attack;
    }

    std::ostream &operator<<(std::ostream &os, Color color) {
        static const char *const names[2]{"White", "Black"};
        return os << names[static_cast<int>(color)];
    }

    std::ostream &operator<<(std::ostream &os, const Board &board) {
        auto printTeam = [&](const Color color) {
            os << color << ":\n";
            const auto team = board.bitboards[static_cast<int>(color)];
            for (int i = 0; i < team.size(); ++i) {
                os << PieceType(i) << ":\n"
                   << team[i] << '\n';
            }
        };

        printTeam(Color::White);
        printTeam(Color::Black);

        os << "Turn:\t\t" << board.playerTurn << '\n';
        os << "Half moves:\t" << board.halfMoveCounter << '\n';
        os << "Full moves:\t" << board.fullMoveCounter << '\n';
        os << "En passant:\t" << board.enPassant << '\n';
        os << "Castling:\t";
        if (board.castlingRights & static_cast<uint8_t>(CastlingRightFlag::WhiteKing))
            os << 'K';
        if (board.castlingRights & static_cast<uint8_t>(CastlingRightFlag::WhiteQueen))
            os << 'Q';
        if (board.castlingRights & static_cast<uint8_t>(CastlingRightFlag::BlackKing))
            os << 'k';
        if (board.castlingRights & static_cast<uint8_t>(CastlingRightFlag::BlackQueen))
            os << 'q';
        os << '\n';

        return os;
    }
}
