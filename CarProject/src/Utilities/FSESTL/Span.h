namespace stl {

    template<typename ElementType>
    struct Span {

        using iterator = ElementType*;

        template<typename Container>
        Span(Container& c) :
            mBegin(c.data()), mEnd(mBegin + c.size()) {

        }

        iterator inline begin() const { return mBegin; }
        iterator inline end() const { return mEnd; }

    private:
        iterator mBegin;
        iterator mEnd;

    };
}
