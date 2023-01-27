#pragma once

// ***********************
// * ITERABLE INTERFACES *
// ***********************
#include <stdexcept>

class StopIteration : std::exception {};

template<typename T>
class IIterator
{
    public:
        virtual T next() = 0;
        virtual ~IIterator() {};
};

template<typename T>
class IIterable
{
    public:
        virtual IIterator<T> * iter() const = 0;
        virtual ~IIterable() {};
};

// ********************************
// * FUNCTIONS YOU MUST IMPLEMENT *
// ********************************
template<typename T>
IIterable<T> * repeat(T v);

template<typename T>
IIterable<T> * count(T start, T delta);

template<typename T>
IIterable<T> * skip(unsigned k, IIterable<T> *i);

template<typename T>
IIterable<T> * take(unsigned k, IIterable<T> *i);

template<typename T>
IIterable<T> * cycle(IIterable<T> *i);

template<typename T>
IIterable<T> * concat(IIterable<T> *i, IIterable<T> *j);

template<typename T>
IIterable<T> * alternate(IIterable<T> *i, IIterable<T> *j);

template<typename T>
IIterable<T> * filter(IIterable<T> *i, IIterable<bool> *f);

// ********************************************
// * ACCUMULATE IMPLEMENTATION PROVIDED BY US *
// ********************************************
template<typename T>
class AccumulateIterator : public IIterator<T>
{
    private:
        IIterator<T> *mIterator;
        T mAccumulation;
        bool mHasIterationStarted;


    public:
        AccumulateIterator(IIterator<T> *iterator) : mIterator(iterator),
                                                     mAccumulation(),
                                                     mHasIterationStarted(false)
        {
        }

        virtual T next()
        {
            if (!mHasIterationStarted)
            {
                mAccumulation = mIterator->next();
                mHasIterationStarted = true;
            }
            else
            {
                mAccumulation += mIterator->next();
            }

            return mAccumulation;
        }

        virtual ~AccumulateIterator()
        {
            delete mIterator;
        }
};

template<typename T>
class AccumulateIterable : public IIterable<T>
{
    private:
        IIterable<T> *mIterable;

    public:
        AccumulateIterable(IIterable<T> *iterable) : mIterable(iterable)
        {
        }

        virtual IIterator<T> * iter() const
        {
            return new AccumulateIterator<T>(mIterable->iter());
        }

        virtual ~AccumulateIterable()
        {
            delete mIterable;
        }
};

template<typename T>
IIterable<T> * accumulate(IIterable<T> *i)
{
    return new AccumulateIterable<T>(i);
}
