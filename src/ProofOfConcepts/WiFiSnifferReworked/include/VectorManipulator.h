#pragma once

#include <vector>
#include <algorithm>

namespace VectorManipulator
{
    /// @brief Deletes all indecies provided between beginIndeciesIter and endIndeciesIter on a sorted indecies collection
    /// @tparam T Items inside the vector
    /// @tparam Iter Iterator of the items in the indecies collection
    /// @param vector The vector from where to delete
    /// @param beginIndeciesIter The begining iterator of the indecies collection that need to be deleted from vector
    /// @param endIndeciesIter The end iterator of the indecies collection that need to be deleted from vector
    template <typename T, typename Iter>
    void SortedIndeciesDelete(std::vector<T> &vector, Iter beginIndeciesIter, Iter endIndeciesIter)
    {
        std::size_t currentVectorIndex = 0;

        Iter removeIter = beginIndeciesIter;
        const auto predicate = [&](const T&)
        {
            if (removeIter != endIndeciesIter && *removeIter == currentVectorIndex++)
            {
                removeIter++;
                return true;
            }
            
            return false;
        };

        vector.erase(std::remove_if(vector.begin(), vector.end(), predicate), vector.end());
    }

    /// @brief Deletes all indecies provided between beginIndeciesIter and endIndeciesIter on an unsorted indecies collection
    /// @tparam T Items inside the vector
    /// @tparam Iter Iterator of the items in the indecies collection
    /// @param vector The vector from where to delete
    /// @param beginIndeciesIter The begining iterator of the indecies collection that need to be deleted from vector
    /// @param endIndeciesIter The end iterator of the indecies collection that need to be deleted from vector
    template <typename T, typename Iter>
    void UnsortedIndeciesDelete(std::vector<T> &vector, Iter beginIter, Iter endIter)
    {
        std::size_t currentVectorIndex = 0;

        const auto pred = [&](const T&)
        {
            //Essentially a long .contains()
            return std::find(beginIter, endIter, currentVectorIndex++) != endIter;
        };

        vector.erase(std::remove_if(vector.begin(), vector.end(), pred), vector.end());
    }

    /// @brief Deletes all indecies provided between beginIndeciesIter and endIndeciesIter of a collection
    /// @tparam T Items inside the vector
    /// @tparam Iter Iterator of the items in the indecies collection
    /// @param vector The vector from where to delete
    /// @param beginIndeciesIter The begining iterator of the indecies collection that need to be deleted from vector
    /// @param endIndeciesIter The end iterator of the indecies collection that need to be deleted from vector
    template <typename T, typename Iter>
    void DeleteIndecies(std::vector<T> &vector, Iter beginIndeciesIter, Iter endIndeciesIter)
    {
        if (std::is_sorted(beginIndeciesIter, endIndeciesIter))
        {
            SortedIndeciesDelete(vector, beginIndeciesIter, endIndeciesIter);
        }
        else
        {
            UnsortedIndeciesDelete(vector, beginIndeciesIter, endIndeciesIter);
        }
    }
}