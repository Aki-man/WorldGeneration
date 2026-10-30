#include "Square.h"

bool Square::operator==(Square square)
{
    return this->begin == square.begin && this->end == square.end;
}
