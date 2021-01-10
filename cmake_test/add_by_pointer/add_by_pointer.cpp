#include "add_by_pointer.hpp"

void sum (int* first, int* second, int* third)
{
    *third = *first + *second;
}