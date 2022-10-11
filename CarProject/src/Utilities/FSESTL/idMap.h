#pragma once

namespace stl {

    template<typename ElementType>
    class IdMap
    {
    public:
        using iterator = typename std::vector<ElementType>::iterator;
        using const_iterator = typename std::vector<ElementType>::const_iterator;

        // Iterators

        const_iterator begin() const { return elements_.begin(); }
        const_iterator end() const { return elements_.end(); }

        // Element access

        Span<const ElementType> elements() const { return elements_; }

        // Capacity

        bool empty() const { return sorted_ids_.empty(); }
        size_t size() const { return sorted_ids_.size(); }

        // Modifiers

        std::pair<iterator, bool> insert(int id, const ElementType& element);
        std::pair<iterator, bool> insert(int id, ElementType&& element);
        size_t erase(int id);
        void clear();

        // Lookup

        iterator find(int id);
        const_iterator find(int id) const;
        bool contains(int id) const;

    private:
        std::vector<ElementType> elements_;
        std::vector<int> sorted_ids_;
    };

    template<typename ElementType>
    std::pair<typename IdMap<ElementType>::iterator, bool> IdMap<ElementType>::insert(
        const int id,
        const ElementType& element)
    {
        auto lower_bound = std::lower_bound(sorted_ids_.begin(), sorted_ids_.end(), id);

        if (lower_bound != sorted_ids_.end() && id == *lower_bound)
        {
            return std::make_pair(
                std::next(elements_.begin(), std::distance(sorted_ids_.begin(), lower_bound)), false);
        }

        auto insert_element_at =
            std::next(elements_.begin(), std::distance(sorted_ids_.begin(), lower_bound));

        sorted_ids_.insert(lower_bound, id);
        return std::make_pair(elements_.insert(insert_element_at, element), true);
    }

    template<typename ElementType>
    std::pair<typename IdMap<ElementType>::iterator, bool> IdMap<ElementType>::insert(
        const int id,
        ElementType&& element)
    {
        auto lower_bound = std::lower_bound(sorted_ids_.begin(), sorted_ids_.end(), id);

        if (lower_bound != sorted_ids_.end() && id == *lower_bound)
        {
            return std::make_pair(
                std::next(elements_.begin(), std::distance(sorted_ids_.begin(), lower_bound)), false);
        }

        auto insert_element_at =
            std::next(elements_.begin(), std::distance(sorted_ids_.begin(), lower_bound));

        sorted_ids_.insert(lower_bound, id);
        return std::make_pair(elements_.insert(insert_element_at, std::move(element)), true);
    }

    template<typename ElementType>
    size_t IdMap<ElementType>::erase(const int id)
    {
        auto lower_bound = std::lower_bound(sorted_ids_.begin(), sorted_ids_.end(), id);

        if (lower_bound == sorted_ids_.end() || id != *lower_bound)
        {
            return 0ull;
        }

        auto erase_element_at =
            std::next(elements_.begin(), std::distance(sorted_ids_.begin(), lower_bound));

        sorted_ids_.erase(lower_bound);
        elements_.erase(erase_element_at);

        return 1ull;
    }

    template<typename ElementType>
    void IdMap<ElementType>::clear()
    {
        elements_.clear();
        sorted_ids_.clear();
    }

    template<typename ElementType>
    typename IdMap<ElementType>::iterator IdMap<ElementType>::find(const int id)
    {
        const auto lower_bound = std::lower_bound(sorted_ids_.cbegin(), sorted_ids_.cend(), id);
        return (lower_bound == sorted_ids_.cend() || *lower_bound != id)
            ? elements_.end()
            : std::next(elements_.begin(), std::distance(sorted_ids_.cbegin(), lower_bound));
    }

    template<typename ElementType>
    typename IdMap<ElementType>::const_iterator IdMap<ElementType>::find(const int id) const
    {
        const auto lower_bound = std::lower_bound(sorted_ids_.cbegin(), sorted_ids_.cend(), id);
        return (lower_bound == sorted_ids_.cend() || *lower_bound != id)
            ? elements_.cend()
            : std::next(elements_.cbegin(), std::distance(sorted_ids_.cbegin(), lower_bound));
    }

    template<typename ElementType>
    bool IdMap<ElementType>::contains(const int id) const
    {
        const auto lower_bound = std::lower_bound(sorted_ids_.cbegin(), sorted_ids_.cend(), id);

        if (lower_bound == sorted_ids_.cend())
        {
            return false;
        }

        return *lower_bound == id;
    }
}
