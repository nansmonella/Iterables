#include "Iterable.tpp"
#include "IterTools.tpp"

#include <iostream>
using std::cout;
using std::endl;

template<typename T> void printAndDestroy(IIterable<T> *i)
{
    IIterator<T> *iterator = i->iter();

    try
    {
        while(true)
        {
            cout << iterator->next() << " ";
        }
    }
    catch (StopIteration &)
    {
        cout << endl;
        delete iterator;
    }

    delete i;
}

int main()
{
    printAndDestroy(take(3, repeat(0.f)));
    printAndDestroy(take(5, skip(3, count(1.f, 2.f))));
    printAndDestroy(take(5, accumulate(count(1.f, 2.f))));
    printAndDestroy(take(8, cycle(take(3, count(1.f, 1.f)))));
    printAndDestroy(concat(take(1, repeat(4.f)),
                           take(1, repeat(2.f))));
    printAndDestroy(take(5, filter(count(1.f, 1.f),
                                   alternate(repeat(true), repeat(false)))));
    return 0;
}