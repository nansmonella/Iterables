#pragma once
#include "Iterable.tpp"


///////////////////////////////////////////////////////////////////////////REPEAT///////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class RepeatIterator : public IIterator<T> {
private:
    T mV, mNext;
    bool mHasIterationStarted;
public:
    RepeatIterator(T v) : mV(v), mNext(v), mHasIterationStarted(false) { }

    virtual T next() {
        if (!mHasIterationStarted) {
            mHasIterationStarted = true;
        }
        else {
            mNext = mV;
        }
        return mNext;
    }

    virtual ~RepeatIterator() {
    }
};

template<typename T>
class RepeatIterable : public IIterable<T> {
private:
    T mV;
public:
    RepeatIterable(T v) : mV(v) {}

    virtual IIterator<T>* iter() const {
        return new RepeatIterator<T>(mV);
    }

    virtual ~RepeatIterable() {
    }
};

template<typename T>
IIterable<T>* repeat(T v) { //Creates an iterable representing the infinite sequence v, v, v, ...
    return new RepeatIterable<T>(v);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////CYCLE///////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class CycleIterator : public IIterator<T> {
private:
    IIterator<T>* mIterator;
    IIterable<T>* mIterable;
public:
    CycleIterator(IIterable<T>* it) : mIterator(it->iter()), mIterable(it){
    }

    virtual T next() {
        try {
            return mIterator->next();
        } catch (const StopIteration&) { //reached end, start back from the beginning
            delete mIterator;            
            mIterator = mIterable->iter();
            return mIterator->next();
        }
    }

    virtual ~CycleIterator() {
        delete mIterator;
    }
};

template<typename T>
class CycleIterable : public IIterable<T> {
private:
    IIterable<T>* mIterable;
public:
    CycleIterable(IIterable<T>* i) : mIterable(i) {}

    virtual IIterator<T>* iter() const {
        return new CycleIterator<T>(mIterable);
    }

    virtual ~CycleIterable() {
        delete mIterable;
    }
};

template<typename T>
IIterable<T>* cycle(IIterable<T>* i) { //Creates an iterable that cycles through i infinitely many times. That is, it represents the sequence i[0], ... , i[len(i) - 1]; i[0]; ...,; i[len(i) -1], i[0], ... If i is empty then so is the resulting iterable.If len(i) = 1, then the result is equivalent to i.
    return new CycleIterable<T>(i);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////COUNT///////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class CountIterator : public IIterator<T> {
private:
    T mStart, mDelta, mNext;
    bool mHasIterationStarted;
public:
    CountIterator(T start, T delta) : mStart(start), mDelta(delta), mNext(start), mHasIterationStarted(false) { }

    virtual T next() {
        if (!mHasIterationStarted) {
            mHasIterationStarted = true;
        } else {
            mNext += mDelta;
        }
        return mNext;
    }

    virtual ~CountIterator() {
    }
};

template<typename T>
class CountIterable : public IIterable<T> {
private:
    T mStart;
    T mDelta;
public:
    CountIterable(T start, T delta) : mStart(start), mDelta(delta) {}

    virtual IIterator<T>* iter() const {
        return new CountIterator<T>(mStart, mDelta);
    }

    virtual ~CountIterable() {
    }
};

template<typename T>
IIterable<T>* count(T start, T delta) { // Creates an iterable representing the infinite sequence start, start + delta, start + delta + delta, . . .
    return new CountIterable<T>(start, delta);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////TAKE///////////////////////////////////////////////////////////////////////////////////////


template<typename T>
class TakeIterator : public IIterator<T> {
private:
    unsigned mK;
    IIterator<T>* mIterator;
    unsigned mCounter;
    T mNext;
public:
    TakeIterator(unsigned k, IIterator<T>* it) : mK(k), mIterator(it), mCounter(0) { }

    virtual T next() {
        if (mCounter++ < mK) {
            mNext = mIterator->next();
        } else {
            throw StopIteration();
        }
        return mNext;
    }

    virtual ~TakeIterator() {
        delete mIterator;
    }
};

template<typename T>
class TakeIterable : public IIterable<T> {
private:
    unsigned mK;
    IIterable<T>* mIterable;
public:
    TakeIterable(unsigned k, IIterable<T>* i) : mK(k), mIterable(i) {}

    virtual IIterator<T>* iter() const {
        return new TakeIterator<T>(mK, mIterable->iter());
    }

    virtual ~TakeIterable() {
        delete mIterable;
    }
};

template<typename T>
IIterable<T>* take(unsigned k, IIterable<T>* i) { //Creates an iterable representing the first k elements of i. That is,  i[0], ..., i[k − 1]
    return new TakeIterable<T>(k, i);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////SKIP///////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class SkipIterator : public IIterator<T> {
    private:
        unsigned index;
        IIterator<T> *mIterator;
        bool mHasIterationStarted;
    
    public:
        SkipIterator(unsigned k, IIterator<T> *iterator) : index(k),
                                                           mIterator(iterator),
                                                           mHasIterationStarted(false)
        {
        }

        virtual T next() {
            if(!mHasIterationStarted) {
                for(unsigned j=0; j<index; j++){
                    mIterator->next();
                }
                mHasIterationStarted = true;
                return mIterator->next();
            }
            else {
                return mIterator->next();
            }

        }
        
        virtual ~SkipIterator() {
            delete mIterator;
        }
};

template <typename T>
class SkipIterable : public IIterable<T> {
    private:
        unsigned index;
        IIterable<T> *mIterable;
    
    public:
        SkipIterable(unsigned k, IIterable<T> *iterable) : index(k),
                                                           mIterable(iterable)
        {
        }
    
        virtual IIterator<T>* iter() const {
        return new SkipIterator<T>(index, mIterable->iter());
        }   

        virtual ~SkipIterable() {
        delete mIterable;
        }
        
};

template <typename T> 
IIterable <T> * skip(unsigned k, IIterable <T> *i) {
    return new SkipIterable<T>(k, i);
}

///////////////////////////////////////////////////////////////////////////CONCAT///////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
class ConcatIterator : public IIterator<T> {
private:
    IIterator<T>* mIt1;
    IIterator<T>* mIt2;
    bool switchToIt2;
public:
    ConcatIterator(IIterator<T>* it1, IIterator<T>* it2) : mIt1(it1), mIt2(it2), switchToIt2(false) {
    }

    virtual T next() {
        try {
            return switchToIt2 ? mIt2->next() : mIt1->next();
        } catch (const StopIteration&) { //reached end of first iterator, continue with second iterator
            switchToIt2 = true;
            return mIt2->next();
        }
    }

    virtual ~ConcatIterator() {
        delete mIt1;
        delete mIt2;
    }
};

template<typename T>
class ConcatIterable : public IIterable<T> {
private:
    IIterable<T>* mIt1;
    IIterable<T>* mIt2;
public:
    ConcatIterable(IIterable<T>* it1, IIterable<T>* it2) : mIt1(it1), mIt2(it2) {}

    virtual IIterator<T>* iter() const {
        return new ConcatIterator<T>(mIt1->iter(), mIt2->iter());
    }

    virtual ~ConcatIterable() {
        delete mIt1;
        delete mIt2;
    }
};

template<typename T>
IIterable<T>* concat(IIterable<T>* i, IIterable<T>* j) { //Concatenates two iterables, i and j. That is, i[0], ... , i[len(i) - 1], j[0], ... , j[len(j) - 1] If len(i) = 1, then the result is equivalent to i.
    return new ConcatIterable<T>(i, j);
}

///////////////////////////////////////////////////////////////////////////ALTERNATE///////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class AlternateIterator : public IIterator<T> {
private:
    IIterator<T>* mIt1;
    IIterator<T>* mIt2;
    bool isIt1;
    bool switchToIt2;
    bool alwaysUseIt2;
public:
    AlternateIterator(IIterator<T>* it1, IIterator<T>* it2) : mIt1(it1), mIt2(it2), switchToIt2(false), alwaysUseIt2(false) { }

    virtual T next() {
        try {
            if (alwaysUseIt2 || switchToIt2) {
                isIt1 = false;
                switchToIt2 = false;
                return mIt2->next();
            } else {
                isIt1 = true;
                switchToIt2 = true;
                return mIt1->next();
            }
        } catch (const StopIteration&) { //reached end of an iterator, continue with the other iterator
            if (isIt1) {
                alwaysUseIt2 = true; // exception was thrown during a mIt1->next(), mIt1 is exhausted, use mIt2 from now on
                return mIt2->next();
            } else {
                alwaysUseIt2 = false; // exception was thrown during a mIt2->next(), mIt2 is exhausted, use mIt1 from now on
                return mIt1->next();
            }
        }
    }

    virtual ~AlternateIterator() {
        delete mIt1;
        delete mIt2;
    }
};

template<typename T>
class AlternateIterable : public IIterable<T> {
private:
    IIterable<T>* mIt1;
    IIterable<T>* mIt2;
public:
    AlternateIterable(IIterable<T>* it1, IIterable<T>* it2) : mIt1(it1), mIt2(it2) {}

    virtual IIterator<T>* iter() const {
        return new AlternateIterator<T>(mIt1->iter(), mIt2->iter());
    }

    virtual ~AlternateIterable() {
        delete mIt1;
        delete mIt2;
    }
};

template<typename T>
IIterable<T>* alternate(IIterable<T>* i, IIterable<T>* j) { //Alternates between two iterables i and j. That is, i[0], j[0], i[1], j[1], ... If either i or j is exhausted before the other one, the iterable continues only with the elements of the non-exhausted iterable.
    return new AlternateIterable<T>(i, j);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////FILTER///////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class FilterIterator : public IIterator<T> {
private:
    IIterator<T>* mIt;
    IIterator<bool>* mFilter;
public:
    FilterIterator(IIterator<T>* i, IIterator<bool>* f) : mIt(i), mFilter(f) { }

    virtual T next() {
        while (!mFilter->next()) { //iterate until you find a true
            mIt->next();
        }
        return mIt->next();        
    }

    virtual ~FilterIterator() {
        delete mIt;
        delete mFilter;
    }
};

template<typename T>
class FilterIterable : public IIterable<T> {
private:
    IIterable<T>* mIt;
    IIterable<bool>* mFilter;
public:
    FilterIterable(IIterable<T>* it, IIterable<bool>* f) : mIt(it), mFilter(f) {}

    virtual IIterator<T>* iter() const {
        return new FilterIterator<T>(mIt->iter(), mFilter->iter());
    }

    virtual ~FilterIterable() {
        delete mIt;
        delete mFilter;
    }
};

template<typename T>
IIterable<T>* filter(IIterable<T>* i, IIterable<bool>* f) { //Filters i with a Boolean iterable f. That is, returns an iterable representing i[x1], i[x2], i[x3], ... where the sequence(xp) is the increasing sequence of indices such that f[xp] = true. If i and f have different lengths, the iteration finishes when either i or f is exhausted.
    return new FilterIterable<T>(i, f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





























