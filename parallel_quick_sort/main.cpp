#include <iostream>
#include <list>
#include <future>
#include <chrono>

template <typename T>
auto sequential_quick_sort(std::list<T> input) -> std::list<T>
{
    if ( input.empty() ) {
        return input;
    }
    
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    
    T const & pivot = *result.begin();
    
    auto devide = std::partition(input.begin(), input.end(), [&](T const & t) {
        return t < pivot;
    });
    
    std::list<T> lower;
    lower.splice(lower.begin(), input, input.begin(), devide);

    std::list<T> sorted_lower = sequential_quick_sort(std::move(lower));
    
    std::list<T> sorted_higher = sequential_quick_sort(std::move(input));
    
    result.splice(result.end(), sorted_higher);
    result.splice(result.begin(), sorted_lower);
    
    return result;
}

template <typename T>
auto parallel_quick_sort(std::list<T> input) -> std::list<T>
{
    if ( input.empty() ) {
        return input;
    }
    
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    
    T const & pivot = *result.begin();
    
    auto devide = std::partition(input.begin(), input.end(), [&](T const & t) {
        return t < pivot;
    });
    
    std::list<T> lower;
    lower.splice(lower.begin(), input, input.begin(), devide);
    
    auto sorted_lower = std::async(&parallel_quick_sort<T>, std::move(lower));
    
    std::list<T> sorted_higher = parallel_quick_sort(std::move(input));
    
    result.splice(result.end(), sorted_higher);
    result.splice(result.begin(), sorted_lower.get());
    
    return result;
}

int main(int argc, const char * argv[]) {
    
    size_t elements_count = 2000;
    std::list<ssize_t> list = {2, 4, 6, 8, 10, 1, 3, 5, 7, 9 };
    for ( size_t index = 0; index < elements_count; ++index ) {
        list.push_back(elements_count - index);
    }
    
    
    {
        
        auto start = std::chrono::high_resolution_clock::now();
        auto result = sequential_quick_sort(list);
        auto end = std::chrono::high_resolution_clock::now();
        std::ostream_iterator<ssize_t> output_iterator { std::cout, " " };
//        std::copy(result.cbegin(), result.cend(), output_iterator);
//        std::cout << std::endl;
        std::cout << "sequential quick sort: " << (end - start).count() << std::endl;
    }
    
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = parallel_quick_sort(list);
        auto end = std::chrono::high_resolution_clock::now();
        std::ostream_iterator<ssize_t> output_iterator { std::cout, " " };
//        std::copy(result.cbegin(), result.cend(), output_iterator);
//        std::cout << std::endl;
        std::cout << "parallel quick sort: " << (end - start).count() << std::endl;
    }
    
    return 0;
}
