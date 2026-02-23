#include "provided.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    SmartPlayer bp("Homer");
    SmartPlayer hp("Marge");
    Game g(4, 3, 3, &bp, &hp);
    g.play();
}
