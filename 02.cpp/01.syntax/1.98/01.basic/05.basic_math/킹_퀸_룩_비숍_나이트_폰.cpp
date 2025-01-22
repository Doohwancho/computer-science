#include <iostream>

int main() {
    int king, queen, rook, bishop, knight, pawn;

    std::cin >> king >> queen >> rook >> bishop >> knight >> pawn;

    king = 1 - king;
    queen = 1 - queen;
    rook = 2 - rook;
    bishop = 2 - bishop;
    knight = 2 - knight;
    pawn = 8 - pawn;

    std::cout << king << " " << queen << " " << rook << " " << bishop << " " << knight << " " << pawn;

    return 0;
}