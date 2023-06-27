#include "../src/BPlusIndex.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#undef NDEBUG
#include "assert.h"

void test_index_int_single()
{
    BPlusIndex<int, 2> index;
    assert(index.create("test_index_single.dat"));
    assert(index.get_nodes_count() == 1);
    index.put(0xBADCAFE, 0xDEADBEEF);
    auto v = index.get(0xBADCAFE);
    assert(v.has_value());
    assert(v.value() == 0xDEADBEEF);
}

void test_index_int_split()
{
    BPlusIndex<int, 3> index;
    assert(index.create("test_index_split.dat"));
    assert(index.get_nodes_count() == 1);
    index.put(0, 'A');
    index.put(1, 'B');
    assert(index.get_nodes_count() == 1);
    index.put(2, 'C');
    index.put(3, 'D');
    assert(index.get_nodes_count() == 3);
    assert(index.get_keys_count() == 4);
    auto v = index.get(0);
    assert(v.has_value());
    assert(v.value() == 'A');
    v = index.get(1);
    assert(v.has_value());
    assert(v.value() == 'B');
    v = index.get(2);
    assert(v.has_value());
    assert(v.value() == 'C');
    v = index.get(3);
    assert(v.has_value());
    assert(v.value() == 'D');
}

void test_index_int_sequential()
{
    BPlusIndex<int, 101> index;
    assert(index.create("test_index_int_sequential.dat"));
    assert(index.get_nodes_count() == 1);
    for (int i = 0; i < 100; ++i)
        index.put(i, i);
    assert(index.get_nodes_count() == 1);
    for (int i = 0; i < 100; ++i)
    {
        auto v = index.get(i);
        assert(v.has_value());
        assert(v.value() == i);
    }
}

void test_index_int_shuffled_and_reopen()
{
    std::vector<int> keys;
    for (int i = 0; i < 10000; ++i)
        keys.push_back(i);
    auto rng = std::default_random_engine{};
    std::shuffle(keys.begin(), keys.end(), rng);

    {
        BPlusIndex<int, 10> index;
        assert(index.create("test_index_int_shuffled.dat"));
        assert(index.get_nodes_count() == 1);
        for (auto key : keys)
            index.put(key, key);
    }

    {
        BPlusIndex<int, 10> index;
        assert(index.open("test_index_int_shuffled.dat"));
        for (size_t i = 0; i < keys.size(); ++i)
        {
            auto v = index.get(i);
            assert(v.has_value());
            assert(v.value() == i);
        }
    }
}

void test_index_char()
{
    BPlusIndexCharBuf<256, 16> index;
    assert(index.create("test_index_char.dat"));
    index.put("foo", 1);
    index.put("bar", 2);
    index.put("buzz", 3);
    index.put("aaaa", 4);
    assert(index.get("foo").value() == 1);
    assert(index.get("bar").value() == 2);
    assert(index.get("buzz").value() == 3);
    assert(index.get("aaaa").value() == 4);
}

int main()
{
    test_index_int_single();
    test_index_int_sequential();
    test_index_int_shuffled_and_reopen();
    test_index_char();
    test_index_int_split();
}