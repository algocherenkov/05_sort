#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <ctime>
#include <assert.h>
#include <fstream>
#include <math.h>

#define BOOST_TEST_MODULE test_main

#include <boost/test/included/unit_test.hpp>

constexpr int START_BUFF_SIZE = 20;
constexpr int END_BUFF_SIZE = 100000;
constexpr int NUMBER_SHUFFLED_ELEMENTS = 5;

template <typename T>
void insertionSort(std::vector<T>& array)
{
    for(size_t i = 0; i < array.size(); i++)
    {
        T x = array[i];
        size_t j = i - 1;
        while (j >= 0 && array[j] > x)
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = x;
    }
}

template <typename T>
void ShellSort(std::vector<T>& array, std::vector<int>& steps)
{
    for(const auto & step: steps)
    {
        for(size_t i = static_cast<size_t>(step); i < array.size(); i++)
        {
            T temp = array[i];
            auto j = i;
            for(; j >= static_cast<size_t>(step) && array[j - static_cast<size_t>(step)] > temp; j -= static_cast<size_t>(step))
            {
                array[j] = array[j - static_cast<size_t>(step)];
            }
            array[j] = temp;
        }
    }
}

void createFrankLazarusSeq(std::vector<int>& sequence, size_t arraySize)
{
    for(size_t i = 1; i < arraySize; i++)
    {
        auto step = 2*(arraySize/pow(2, i + 1)) + 1;

        sequence.push_back(static_cast<int>(step));
        if(static_cast<int>(step) == 1)
            break;
    }
}


void createHibbardSeq(std::vector<int>& sequence, size_t arraySize)
{
    for(size_t i = 1; i < arraySize; i++)
    {
        auto step = pow(2, i) - 1;
        if(step > arraySize)
            break;
        sequence.push_back(static_cast<int>(step));
    }

    std::reverse(sequence.begin(), sequence.end());
}


void createSegwickSeq(std::vector<int>& sequence, size_t arraySize)
{
    sequence.push_back(1);
    for(size_t i = 1; i < arraySize; i++)
    {
        auto step = pow(4, i) + 3*pow(2, i - 1) + 1;
        if(step > arraySize)
            break;
        sequence.push_back(static_cast<int>(step));
    }

    std::reverse(sequence.begin(), sequence.end());
}
template <typename T>
void createRandomShuffledBuff(std::vector<T>& buff, int N)
{
    assert(N > 0);
    std::random_device rd;
    std::mt19937 g(rd());

    for(int i = 0; i < N; i++)
    {
        buff.push_back(i);
    }

    std::shuffle(buff.begin(), buff.end(), g);
}

template <typename T>
void createTenPercentShuffledBuff(std::vector<T>& buff, int N)
{
    assert(N > 0);
    std::random_device rd;
    std::mt19937 g(rd());
    auto shuffledQuantity = N * 10 / 100;

    for(int i = 0; i < N; i++)
    {
        if(i % shuffledQuantity == 0)
        {
            std::srand(std::time(nullptr)); // use current time as seed for random generator
            T random_variable = std::rand();
            buff.push_back(random_variable);
        }
        buff.push_back(i);
    }
}

template <typename T>
void createNElementsShuffledBuff(std::vector<T>& buff, int N)
{
    assert(N > 0);
    std::random_device rd;
    std::mt19937 g(rd());
    auto shuffledQuantity = N / NUMBER_SHUFFLED_ELEMENTS;

    for(int i = 0; i < N; i++)
    {
        if(i % shuffledQuantity == 0)
        {
            std::srand(std::time(nullptr)); // use current time as seed for random generator
            T random_variable = std::rand();
            buff.push_back(random_variable);
        }
        buff.push_back(i);
    }
}

using namespace boost::unit_test;
BOOST_AUTO_TEST_SUITE(test_suite_main)

BOOST_AUTO_TEST_CASE(insertion_sort_test)
{
    std::ofstream file;

    file.open("insertion_sort_results.txt", std::ios::app);
    BOOST_CHECK_MESSAGE(file.is_open(), "cannot open file!");


    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createRandomShuffledBuff(array, i);

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        insertionSort(array);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\ninsertion_sort_test (random input data): total time " << duration << " us. For " << i << " elements" << std::endl;
    }

    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createTenPercentShuffledBuff(array, i);

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        insertionSort(array);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\ninsertion_sort_test (10% shuffled input data): total time " << duration << " us. For " << i << " elements" << std::endl;
    }

    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createNElementsShuffledBuff(array, i);

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        insertionSort(array);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\ninsertion_sort_test (only " << NUMBER_SHUFFLED_ELEMENTS << " elements shuffled in input data): total time " << duration << " us. For " << i << " elements" << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(shell_sort_test)
{
    std::ofstream file;

    file.open("shell_sort_results.txt", std::ios::app);
    BOOST_CHECK_MESSAGE(file.is_open(), "cannot open file!");


    /********************* Frank-Lazarus Sequence**********************/
    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createRandomShuffledBuff(array, i);

        std::vector<int> steps;
        createFrankLazarusSeq(steps, array.size());

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        ShellSort(array, steps);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\nshell_sort_test (random input data, Frank-Lazarus Sequence): total time " << duration << " us. For " << i << " elements" << std::endl;
    }

    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createTenPercentShuffledBuff(array, i);

        std::vector<int> steps;
        createFrankLazarusSeq(steps, array.size());

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        ShellSort(array, steps);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\nshell_sort_test (10% shuffled input data, Frank-Lazarus Sequence): total time " << duration << " us. For " << i << " elements" << std::endl;
    }

    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createNElementsShuffledBuff(array, i);

        std::vector<int> steps;
        createFrankLazarusSeq(steps, array.size());

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        ShellSort(array, steps);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\nshell_sort_test (only " << NUMBER_SHUFFLED_ELEMENTS << " elements shuffled in input data, Frank-Lazarus Sequence): total time " << duration << " us. For " << i << " elements" << std::endl;
    }

    /********************* Hibbard Sequence**********************/
    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createRandomShuffledBuff(array, i);

        std::vector<int> steps;
        createHibbardSeq(steps, array.size());

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        ShellSort(array, steps);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\nshell_sort_test (random input data, Hibbard Sequence): total time " << duration << " us. For " << i << " elements" << std::endl;
    }

    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createTenPercentShuffledBuff(array, i);

        std::vector<int> steps;
        createHibbardSeq(steps, array.size());

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        ShellSort(array, steps);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\nshell_sort_test (10% shuffled input data, Hibbard Sequence): total time " << duration << " us. For " << i << " elements" << std::endl;
    }

    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createNElementsShuffledBuff(array, i);

        std::vector<int> steps;
        createHibbardSeq(steps, array.size());

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        ShellSort(array, steps);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\nshell_sort_test (only " << NUMBER_SHUFFLED_ELEMENTS << " elements shuffled in input data, Hibbard Sequence): total time " << duration << " us. For " << i << " elements" << std::endl;
    }

    /********************* Segwick Sequence **********************/
    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createRandomShuffledBuff(array, i);

        std::vector<int> steps;
        createSegwickSeq(steps, array.size());

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        ShellSort(array, steps);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\nshell_sort_test (random input data, Segwick Sequence): total time " << duration << " us. For " << i << " elements" << std::endl;
    }

    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createTenPercentShuffledBuff(array, i);

        std::vector<int> steps;
        createSegwickSeq(steps, array.size());

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        ShellSort(array, steps);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\nshell_sort_test (10% shuffled input data, Segwick Sequence): total time " << duration << " us. For " << i << " elements" << std::endl;
    }

    for(int i = START_BUFF_SIZE; i <= END_BUFF_SIZE; i *= 2)
    {
        std::vector<int> array;
        createNElementsShuffledBuff(array, i);

        std::vector<int> steps;
        createSegwickSeq(steps, array.size());

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        ShellSort(array, steps);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        file << "\nshell_sort_test (only " << NUMBER_SHUFFLED_ELEMENTS << " elements shuffled in input data, Segwick Sequence): total time " << duration << " us. For " << i << " elements" << std::endl;
    }
}
BOOST_AUTO_TEST_SUITE_END()
