#include "board.h"

#include <sstream>
#include <regex>

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
        this->kings[0] = Square::E1;
        this->kings[1] = Square::E8;
        this->playerTurn = Color::White;
    }

    void Board::clear() {
        this->bitboards = {};
    }

    State Board::state() {
        auto moves = legalMoves();
        if (moves.empty()) {
            // Check if current player is checkmate
            if (squareThreatened(kings[static_cast<int>(turnToMove())], oppositeTeam(turnToMove())))
                return turnToMove() == Color::White ? State::BlackWinner : State::WhiteWinner;
            else
                return State::Tied;
        }
        // TODO: Check if only kings remain

        // Check if 50 move rule is in effect
        if (halfMoveCounter - counterReset >= 50)
            return State::Tied;

        return State::On;
    }

    void Board::performMove(Move move) {
        assert(isMovePseudoLegal(move));

        int playerIndex = static_cast<int>(this->playerTurn);

        auto &team = this->bitboards[playerIndex];

        auto piece = removePieceAt(move.from, this->playerTurn);
        team[static_cast<int>(piece)].setOccupancyAt(move.to);

        if (piece == PieceType::King) {
            kings[playerIndex] = move.to;
            switch (playerTurn) {
                case Color::White:
                    if (castlingRights[playerIndex][0] == 0 and move.from == Square::E1)
                        castlingRights[playerIndex][0] = fullMoveCounter;
                    break;
                case Color::Black:
                    if (castlingRights[playerIndex][0] == 0 and move.from == Square::E8)
                        castlingRights[playerIndex][0] = fullMoveCounter;
                    break;
            }
        }

        if (piece == PieceType::Rook) {
            switch (playerTurn) {
                case Color::White:
                    if (castlingRights[playerIndex][1] == 0 and move.from == Square::A1)
                        castlingRights[playerIndex][1] = fullMoveCounter;
                    else if (castlingRights[playerIndex][2] == 0 and move.from == Square::H1)
                        castlingRights[playerIndex][2] = fullMoveCounter;
                    break;
                case Color::Black:
                    if (castlingRights[playerIndex][1] == 0 and move.from == Square::A8)
                        castlingRights[playerIndex][1] = fullMoveCounter;
                    else if (castlingRights[playerIndex][2] == 0 and move.from == Square::H8)
                        castlingRights[playerIndex][2] = fullMoveCounter;
                    break;
            }
        }

        switch (Castling(move.castle)) {
            case Castling::WhiteKing:
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].clearOccupancyAt(
                        Square::H1);
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].setOccupancyAt(
                        Square::F1);
                break;
            case Castling::WhiteQueen:
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].clearOccupancyAt(
                        Square::A1);
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].setOccupancyAt(
                        Square::D1);
                break;
            case Castling::BlackKing:
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].clearOccupancyAt(
                        Square::H8);
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].setOccupancyAt(
                        Square::F8);
                break;
            case Castling::BlackQueen:
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].clearOccupancyAt(
                        Square::A8);
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].setOccupancyAt(
                        Square::D8);
                break;
            case Castling::None:
                break;
        }


        if (move.dropPiece) {
            Square dropSquare = move.to;
            if (move.enPassantCapture)
                dropSquare = (this->playerTurn == Color::White) ?
                             Bitboard::squareToThe(Direction::South, dropSquare) :
                             Bitboard::squareToThe(Direction::North, dropSquare);

            auto &enemyTeam
                    = this->bitboards[static_cast<int>(oppositeTeam(this->playerTurn))];
            enemyTeam[static_cast<int>(*move.dropPiece)].clearOccupancyAt(dropSquare);

            if (move.dropPiece == PieceType::Rook) {
                if (dropSquare == Square::A1)
                    castlingRights[0][1] = fullMoveCounter;
                else if (dropSquare == Square::A8)
                    castlingRights[1][1] = fullMoveCounter;
                else if (dropSquare == Square::H1)
                    castlingRights[0][2] = fullMoveCounter;
                else if (dropSquare == Square::H8)
                    castlingRights[1][2] = fullMoveCounter;
            }
        }

        enPassant = move.enPassant;


        ++this->halfMoveCounter;

        if (move.dropPiece or piece == PieceType::Pawn) {
            previousResetValue = counterReset;
            counterReset = halfMoveCounter;
        }

        if (this->playerTurn == Color::Black)
            ++this->fullMoveCounter;

        this->enPassant = Square::None;

        this->playerTurn = oppositeTeam(playerTurn);

        this->movesMade.push_back(move);
    }

    void Board::undoMove() {
        auto move = this->movesMade.back();
        movesMade.pop_back();
        this->playerTurn = oppositeTeam(playerTurn);
        int playerIndex = static_cast<int>(playerTurn);

        auto &team = this->bitboards[static_cast<int>(this->playerTurn)];

        auto piece = removePieceAt(move.to, this->playerTurn);
        team[static_cast<int>(piece)].setOccupancyAt(move.from);

        if (piece == PieceType::King)
            kings[playerIndex] = move.from;

        if (piece == PieceType::King) {
            kings[playerIndex] = move.from;
            if (move.from == Square::E1 && castlingRights[0][0] == fullMoveCounter)
                castlingRights[0][0] = 0;
            if (move.from == Square::E8 && castlingRights[1][0] == fullMoveCounter)
                castlingRights[1][0] = 0;

        }

        // Castling
        switch (Castling(move.castle)) {
            case Castling::WhiteKing:
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].clearOccupancyAt(
                        Square::F1);
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].setOccupancyAt(
                        Square::H1);
                castlingRights[0][0] = 0;
                castlingRights[0][2] = 0;
                break;
            case Castling::WhiteQueen:
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].clearOccupancyAt(
                        Square::D1);
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].setOccupancyAt(
                        Square::A1);
                castlingRights[0][0] = 0;
                castlingRights[0][1] = 0;
                break;
            case Castling::BlackKing:
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].clearOccupancyAt(
                        Square::F8);
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].setOccupancyAt(
                        Square::H8);
                castlingRights[1][0] = 0;
                castlingRights[1][2] = 0;
                break;
            case Castling::BlackQueen:
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].clearOccupancyAt(
                        Square::D8);
                this->bitboards[playerIndex][static_cast<int>(PieceType::Rook)].setOccupancyAt(
                        Square::A8);
                castlingRights[1][0] = 0;
                castlingRights[1][2] = 0;
                break;
            case Castling::None:
                break;
        }

        if (piece == PieceType::Rook) {
            if (move.from == Square::A1 && castlingRights[0][1] == fullMoveCounter)
                castlingRights[0][1] = 0;
            if (move.from == Square::A8 && castlingRights[1][1] == fullMoveCounter)
                castlingRights[1][1] = 0;
            if (move.from == Square::H1 && castlingRights[0][2] == fullMoveCounter)
                castlingRights[0][2] = 0;
            if (move.from == Square::H8 && castlingRights[1][2] == fullMoveCounter)
                castlingRights[1][2] = 0;
        }

        if (this->playerTurn == Color::Black)
            --this->fullMoveCounter;

        if (move.dropPiece) {
            Square dropSquare = move.to;

            if (move.enPassantCapture)
                dropSquare = (this->playerTurn == Color::White) ?
                             Bitboard::squareToThe(Direction::South, dropSquare) :
                             Bitboard::squareToThe(Direction::North, dropSquare);

            auto &enemyTeam
                    = this->bitboards[static_cast<int>(oppositeTeam(this->playerTurn))];
            enemyTeam[static_cast<int>(*move.dropPiece)].setOccupancyAt(dropSquare);

            if (move.dropPiece == PieceType::Rook) {
                if (dropSquare == Square::A1 && castlingRights[0][1] == fullMoveCounter)
                    castlingRights[0][1] = 0;
                else if (dropSquare == Square::A8 && castlingRights[1][1] == fullMoveCounter)
                    castlingRights[1][1] = 0;
                else if (dropSquare == Square::H1 && castlingRights[0][2] == fullMoveCounter)
                    castlingRights[0][2] = 0;
                else if (dropSquare == Square::H8 && castlingRights[1][2] == fullMoveCounter)
                    castlingRights[1][2] = 0;
            }
        }

        if (movesMade.empty())
            this->enPassant = Square::None;
        else
            this->enPassant = movesMade.back().enPassant;

        --this->halfMoveCounter;

        if (move.dropPiece or piece == PieceType::Pawn) {
            counterReset = previousResetValue;
        }
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

        clear();

        auto itr = fen.begin();

        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (std::isupper(*itr)) { // White piece
                    if (*itr == 'K') kings[0] = Square(rank * 8 + file);
                    bitboards[0][static_cast<int>(charToPiece(*itr))].setOccupancyAt(Square(rank * 8 + file));

                } else if (std::islower(*itr)) { // Black piece
                    if (*itr == 'k') kings[1] = Square(rank * 8 + file);
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
        for (int i = 0; i < 3; ++i) {
            castlingRights[0][i] = -1;
            castlingRights[1][i] = -1;
        }
        if (*itr == 'K') {
            castlingRights[0][0] = 0;
            castlingRights[0][2] = 0;
            itr++;
        }
        if (*itr == 'Q') {
            castlingRights[0][0] = 0;
            castlingRights[0][1] = 0;
            itr++;
        }
        if (*itr == 'k') {
            castlingRights[1][0] = 0;
            castlingRights[1][2] = 0;
            itr++;
        }
        if (*itr == 'q') {
            castlingRights[1][0] = 0;
            castlingRights[1][1] = 0;
            itr++;
        }
        if (*itr == '-')
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
            movesMade.emplace_back(Square::A1, Square::A1, enPassant);
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
        bool castling{false};
        if (!castlingRights[0][0] && !castlingRights[0][2]) {
            result << 'K';
            castling = true;
        }
        if (!castlingRights[0][0] && !castlingRights[0][1]) {
            result << 'Q';
            castling = true;
        }
        if (!castlingRights[1][0] && !castlingRights[1][2]) {
            result << 'k';
            castling = true;
        }
        if (!castlingRights[1][0] && !castlingRights[1][1]) {
            result << 'q';
            castling = true;
        }
        if (!castling)
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

    bool Board::isValidFen(const std::string &fen) {

        // https://regex101.com/r/zDBOFz/2
        static const std::regex fenRegex(
                R"(^((?:[pnbrqkPNBRQK1-8]{1,8}\/){7}[pnbrqkPNBRQK1-8]{1,8})\s+(b|w)\s+(-|K?Q?k?q?)\s+(-|[a-h][3-6])\s+(\d+)\s+(\d+)\s*$)");

        std::smatch fenMatches;
        if (!std::regex_match(fen, fenMatches, fenRegex))
            return false;

        const auto pieces = fenMatches[1].str();

        bool isWhiteKingPresent = false;
        bool isBlackKingPresent = false;

        int rankLength = 0;
        for (auto c: pieces) {
            if (c == '/') {
                if (rankLength != 8)
                    return false;
                rankLength = 0;
                continue;
            }

            if (std::isdigit(c))
                rankLength += c - '0';
            else {
                if (c == 'K') {
                    if (isWhiteKingPresent)
                        return false;
                    isWhiteKingPresent = true;

                } else if (c == 'k') {
                    if (isBlackKingPresent)
                        return false;
                    isBlackKingPresent = true;
                }

                ++rankLength;
            }
        }

        return (rankLength == 8 && isWhiteKingPresent && isBlackKingPresent);
    }


    bool Board::isLegal() const {
        return !squareThreatened(kings[static_cast<int>(oppositeTeam(playerTurn))], playerTurn);
    }

    bool Board::squareThreatened(Chess::Square square, Chess::Color opponentColor) const {
        return squaresThreatened(opponentColor).isOccupiedAt(square);
    }

    Bitboard Board::squaresThreatened(Chess::Color opponentColor) const {
        Bitboard occupiedSquares = teamOccupiedSquares(Color::White) | teamOccupiedSquares(Color::Black);
        Bitboard targetedSquares;

        auto addAttackMasks = [&](Bitboard bitboard, PieceType piece) -> void {
            for (int i = 0; i < 64; ++i) {
                if (bitboard.isOccupiedAt(Square(i))) {
                    Bitboard attacks;
                    switch (piece) {
                        case PieceType::King:
                            attacks = kingAttacks(Square(i));
                            break;
                        case PieceType::Queen:
                            attacks = queenAttacks(Square(i), occupiedSquares);
                            break;
                        case PieceType::Rook:
                            attacks = rookAttacks(Square(i), occupiedSquares);
                            break;
                        case PieceType::Bishop:
                            attacks = bishopAttacks(Square(i), occupiedSquares);
                            break;
                        case PieceType::Knight:
                            attacks = knightAttacks(Square(i));
                            break;
                        case PieceType::Pawn:
                            attacks = pawnThreatens(Square(i), opponentColor);
                            break;
                    }
                    targetedSquares |= attacks;
                }
            }
        };

        for (int i = 0; i < 6; ++i)
            addAttackMasks(bitboards[static_cast<int>(opponentColor)][i], PieceType(i));
        return targetedSquares;
    }

    bool Board::canCastleThrough(Square square, Bitboard occupiedSquares) const {
        return !squareThreatened(square, oppositeTeam(playerTurn)) && !occupiedSquares.isOccupiedAt(square);
    }

    std::vector<Move> Board::legalMoves() {
        std::vector<Move> movesToFilter = pseudoLegalMoves();
        std::vector<Move> resultVector;


        for (auto move: movesToFilter) {
            this->performMove(move);
            if (this->isLegal()) {
                resultVector.emplace_back(move);
            }
            this->undoMove();
        }
        if (resultVector.empty()) {
            // TODO: End condition.
        }

        return resultVector;
    }

    std::vector<Move> Board::legalMoves(Square square) {
        std::vector<Move> movesToFilter = pseudoLegalMoves(square);
        std::vector<Move> resultVector;


        for (auto move: movesToFilter) {
            this->performMove(move);
            if (this->isLegal()) {
                resultVector.emplace_back(move);
            }
            this->undoMove();
        }
        if (resultVector.empty()) {
            // TODO: End condition.
        }

        return resultVector;
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

        // If the selected square isn't from the current player, there are no valid moves
        if (!ourSquares.isOccupiedAt(square))
            return;

        const auto piece = pieceAt(square);

        Bitboard attacks;
        switch (piece) {
            case PieceType::King:
                attacks = kingAttacks(square);
                // Castling
                if (!squareThreatened(kings[static_cast<int>(playerTurn)],
                                      oppositeTeam(playerTurn))) {
                    if (playerTurn == Color::White) {
                        if (!castlingRights[0][0] and !castlingRights[0][2]) {
                            if (canCastleThrough(Square::F1, occupiedSquares) &&
                                canCastleThrough(Square::G1, occupiedSquares)) {
                                moves.emplace_back(square, Square::G1,
                                                   static_cast<int>(Castling::WhiteKing));
                            }
                        }
                        if (!castlingRights[0][0] and !castlingRights[0][1]) {
                            if (canCastleThrough(Square::D1, occupiedSquares) &&
                                canCastleThrough(Square::C1, occupiedSquares) &&
                                !occupiedSquares.isOccupiedAt(Square::B1)) {
                                moves.emplace_back(square, Square::C1,
                                                   static_cast<int>(Castling::WhiteQueen));
                            }
                        }
                    } else {
                        if (!castlingRights[1][0] and !castlingRights[1][2]) {
                            if (canCastleThrough(Square::F8, occupiedSquares) &&
                                canCastleThrough(Square::G8, occupiedSquares)) {
                                moves.emplace_back(square, Square::G8,
                                                   static_cast<int>(Castling::BlackKing));
                            }
                        }
                        if (!castlingRights[1][0] and !castlingRights[1][1]) {
                            if (canCastleThrough(Square::D8, occupiedSquares) &&
                                canCastleThrough(Square::C8, occupiedSquares) &&
                                !occupiedSquares.isOccupiedAt(Square::B8)) {
                                moves.emplace_back(square, Square::C8,
                                                   static_cast<int>(Castling::BlackQueen));
                            }
                        }
                    }
                }
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
                if (enPassant != Square::None) {
                    Square dropSquare;
                    switch (color) {
                        case Color::White:
                            dropSquare = Bitboard::squareToThe(Direction::South, enPassant);
                            if (square == Bitboard::squareToThe(Direction::SouthWest, enPassant) ||
                                square == Bitboard::squareToThe(Direction::SouthEast, enPassant))
                                moves.emplace_back(square, enPassant, true, dropSquare);
                            break;
                        case Color::Black:
                            dropSquare = Bitboard::squareToThe(Direction::North, enPassant);
                            if (square == Bitboard::squareToThe(Direction::NorthWest, enPassant) ||
                                square == Bitboard::squareToThe(Direction::NorthEast, enPassant))
                                moves.emplace_back(square, enPassant, true, dropSquare);
                            break;
                    }
                }
                Bitboard startRank;
                Direction attackDirection;
                switch (color) {
                    case Color::White:
                        startRank = twoRank;
                        attackDirection = Direction::North;
                        break;
                    case Color::Black:
                        startRank = sevenRank;
                        attackDirection = Direction::South;
                        break;
                }
                if (startRank.isOccupiedAt(square)) {
                    Square firstSquare = Bitboard::squareToThe(attackDirection, square);
                    Square secondSquare = Bitboard::squareToThe(attackDirection, firstSquare);
                    if (!(occupiedSquares.isOccupiedAt(firstSquare) or occupiedSquares.isOccupiedAt(secondSquare))) {
                        moves.emplace_back(square, secondSquare, firstSquare);
                    }
                }
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

    Bitboard Board::pawnThreatens(Chess::Square square, Chess::Color color) {
        Square captureWest = (color == Color::White) ? Bitboard::squareToThe(Direction::NorthWest, square)
                                                     : Bitboard::squareToThe(Direction::SouthWest, square);
        Square captureEast = (color == Color::White) ? Bitboard::squareToThe(Direction::NorthEast, square)
                                                     : Bitboard::squareToThe(Direction::SouthEast, square);

        Bitboard attacks;

        if (captureWest != Square::None)
            attacks.setOccupancyAt(captureWest);
        if (captureEast != Square::None)
            attacks.setOccupancyAt(captureEast);

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
        Direction attackDirection = (color == Color::White) ? Direction::North : Direction::South;

        Bitboard attack;

        auto attackSquare = Bitboard::squareToThe(attackDirection, square);
        if (attackSquare != Square::None)
            attack.setOccupancyAt(attackSquare);

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
        if (!board.castlingRights[0][0] and !board.castlingRights[0][2])
            os << 'K';
        if (!board.castlingRights[0][0] and !board.castlingRights[0][1])
            os << 'Q';
        if (!board.castlingRights[1][0] and !board.castlingRights[1][2])
            os << 'k';
        if (!board.castlingRights[1][0] and !board.castlingRights[1][1])
            os << 'q';
        os << '\n';

        return os;
    }
}
