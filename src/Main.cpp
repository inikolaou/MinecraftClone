#include <iostream>
#include <string>
#include <utils/Game.h>

using namespace std;


int main(void) {
    try {
        Game* game = new Game();
        game->run();
    } catch (exception& ex) {
        cout << ex.what() << endl;
        getchar();
        return -1;
    }

    return 0;
}