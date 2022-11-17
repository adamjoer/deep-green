// #include <QApplication>

//#include "game.h"
#include "chess/board.h"

int main(int argc, char *argv[]) {
    std::string fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";
    auto board = Chess::Board(fen);
/*    QApplication application(argc, argv);

    Game game;
    game.show();

    return application.exec();
    */
}
