#pragma once

#include <string>
#include <fstream>
#include <optional>
#include <assert.h>
#include <string.h>
#include <vector>

const size_t IS_LEAF_NODE = std::numeric_limits<size_t>::max();
// const size_t UNDEFINED_NOONE_POS = std::numeric_limits<size_t>::max() - 1;

template <typename K>
struct BPlusRecord
{
    K key;
    size_t value; // for non-leaf nodes it is position of child node in index file
};

template <typename K, int SIZE>
class BPlusNode
{
public:
    // Leaf node defines 'count' pairs of (key, value)
    //
    // Non-leaf node defines 'count'+1 intervals:
    // (-inf, records[0].key) -> left_node_pos
    // [records[0].key, records[1].key) -> records[0].value
    // [records[1].key, records[2].key) -> records[1].value
    // ...
    // [records[count-1].key, +inf) -> records[count-1].value
    //
    // Empty node is always leaf.

    // Create empty leaf node
    BPlusNode() : count(0), left_node_pos(IS_LEAF_NODE) {}
    // Create non-leaf node with two children
    BPlusNode(const K &key, size_t left_nodepos, size_t right_nodepos) : count(1), left_node_pos(left_nodepos)
    {
        records[0].key = key;
        records[0].value = right_nodepos;
    }

    bool is_leaf() const
    {
        return left_node_pos == IS_LEAF_NODE;
    }

    size_t get_left_node_pos() const
    {
        assert(!is_leaf());
        return left_node_pos;
    }

    size_t get_keys_count() const
    {
        return count;
    }

    const BPlusRecord<K> &operator[](size_t pos) const
    {
        assert(pos < count);
        return records[pos];
    }

    std::optional<size_t> get_value(const K &key) const
    {
        assert(is_leaf());
        // return exact match
        auto pos = find_lower_bound(key);
        if (pos < count && records[pos].key == key)
            return records[pos].value;
        return std::nullopt;
    }

    size_t get_node_pos(const K &key) const
    {
        assert(!is_leaf());
        auto pos = find_upper_bound(key);
        return pos == 0 ? left_node_pos : records[pos - 1].value;
    }

    std::optional<std::pair<K, BPlusNode<K, SIZE>>> put_value(const K &key, size_t value)
    {
        auto pos = find_lower_bound(key);
        if (pos < count && records[pos].key == key)
        {
            records[pos].value = value;
            return std::nullopt;
        }
        put_at(pos, key, value);
        if (count == SIZE)
        {
            return split();
        }
        else
            return std::nullopt;
    }

    // print state to stream: overload << operator
    friend std::ostream &operator<<(std::ostream &os, const BPlusNode<K, SIZE> &node)
    {
        os << "Node: count=" << node.count << ", left_node_pos=" << node.left_node_pos << std::endl;
        for (size_t i = 0; i < node.count; ++i)
        {
            os << "  [" << i << "] key=" << node.records[i].key << ", value=" << node.records[i].value;
            if (!node.is_leaf())
                os << " node pos by key " << node.get_node_pos(node.records[i].key)
                   << " find key is " << node.find_upper_bound(node.records[i].key);
            os << std::endl;
        }
        return os;
    }

private:
    size_t count;
    size_t left_node_pos; // valid only for non-leaf nodes, otherwise max size_t
    BPlusRecord<K> records[SIZE];

    void put_at(size_t pos, const K &key, size_t value)
    {
        assert(pos <= count);
        assert(pos < SIZE);
        if (count > 0 && pos < count)
            memmove(records + pos + 1, records + pos, (count - pos) * sizeof(BPlusRecord<K>));
        records[pos].key = key;
        records[pos].value = value;
        ++count;
    }

    // Create new note by moving second half of records to it
    // I.e. first n/2 records will remain in original node and seconf n/2 records goes to constructed node
    std::pair<K, BPlusNode<K, SIZE>> split()
    {
        BPlusNode<K, SIZE> new_node;
        K split_key;
        if (is_leaf())
        {
            new_node.count = count / 2;
            count -= new_node.count;
            for (size_t i = 0; i < new_node.count; ++i)
                new_node.records[i] = records[count + i];
            split_key = new_node.records[0].key;
        }
        else
        {
            auto moved_count = count / 2;
            count -= moved_count;
            new_node.count = moved_count - 1;
            new_node.left_node_pos = records[count].value;
            split_key = records[count].key;
            for (size_t i = 0; i < new_node.count; ++i)
                new_node.records[i] = records[count + i + 1];
        }
        return {split_key, new_node};
    }

    size_t find_upper_bound(const K &key) const
    {
        // binary search for key
        size_t l = 0, r = count;
        while (l < r)
        {
            size_t m = (l + r) / 2;
            if (records[m].key <= key)
                l = m + 1;
            else
                r = m;
        }
        return l;
    }

    size_t find_lower_bound(const K &key) const
    {
        // binary search for key
        size_t l = 0, r = count;
        while (l < r)
        {
            size_t m = (l + r) / 2;
            if (records[m].key < key)
                l = m + 1;
            else
                r = m;
        }
        return l;
    }
};

template <typename K, int SIZE>
class BPlusIndex
{
public:
    virtual ~BPlusIndex()
    {
        file.close();
    }

    bool create(const std::string &filename)
    {
        file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open())
            return false;
        BPlusNode<K, SIZE> node;
        store_node(0, node);
        file.close();
        return open(filename);
    }
    bool open(const std::string &filename)
    {
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        return file.is_open();
    }

    size_t get_nodes_count()
    {
        file.seekg(0, std::ios::end);
        auto length = file.tellg();
        auto nodes_count = length / sizeof(BPlusNode<K, SIZE>);
        return (size_t)nodes_count;
    }

    size_t get_keys_count();
    std::optional<size_t> get(const K &key);
    void put(const K &key, size_t value);
    // void remove(const K &key);

    friend std::ostream &operator<<(std::ostream &os, BPlusIndex<K, SIZE> &index)
    {
        auto nodes_count = index.get_nodes_count();
        os << "Index: nodes_count=" << nodes_count << std::endl;
        for (size_t i = 0; i < nodes_count; ++i)
        {
            auto node = index.load_node(i);
            os << "[" << i << "] :" << std::endl
               << node << std::endl;
        }
        return os;
    }

private:
    BPlusNode<K, SIZE> load_node(size_t pos);
    void store_node(size_t pos, const BPlusNode<K, SIZE> &node);
    size_t count_keys_in_node(size_t pos);

    std::fstream file;
};

template <typename K, int SIZE>
inline BPlusNode<K, SIZE> BPlusIndex<K, SIZE>::load_node(size_t pos)
{
    BPlusNode<K, SIZE> node;
    std::streampos offset = pos * sizeof(node);
    file.seekg(offset, std::ios::beg);
    file.read(reinterpret_cast<char *>(&node), sizeof(node));
    return node;
}

template <typename K, int SIZE>
inline void BPlusIndex<K, SIZE>::store_node(size_t pos, const BPlusNode<K, SIZE> &node)
{
    std::streampos offset = pos * sizeof(node);
    file.seekp(offset, std::ios::beg);
    file.write(reinterpret_cast<const char *>(&node), sizeof(node));
    file.flush();
}

template <typename K, int SIZE>
inline size_t BPlusIndex<K, SIZE>::count_keys_in_node(size_t pos)
{
    auto node = load_node(pos);
    if (node.is_leaf())
    {
        return node.get_keys_count();
    }
    else
    {
        size_t count = count_keys_in_node(node.get_left_node_pos());
        for (size_t n = 0; n < node.get_keys_count(); n++)
            count += count_keys_in_node(node[n].value);
        return count;
    }
}

template <typename K, int SIZE>
inline size_t BPlusIndex<K, SIZE>::get_keys_count()
{
    return count_keys_in_node(0);
}

template <typename K, int SIZE>
inline std::optional<size_t> BPlusIndex<K, SIZE>::get(const K &key)
{
    auto node = load_node(0);
    while (!node.is_leaf())
        node = load_node(node.get_node_pos(key));
    return node.get_value(key);
}

template <typename K, int SIZE>
inline void BPlusIndex<K, SIZE>::put(const K &key, size_t value)
{
    std::vector<size_t> path;
    auto node = load_node(0);
    path.push_back(0);
    while (!node.is_leaf())
    {
        auto pos = node.get_node_pos(key);
        path.push_back(pos);
        node = load_node(pos);
    }

    auto k = key;
    auto v = value;
    do
    {
        auto pos = path.back();
        node = load_node(pos);
        auto opt_new_node = node.put_value(k, v);
        store_node(pos, node);
        if (!opt_new_node)
            return;
        k = opt_new_node.value().first;
        auto new_node = opt_new_node.value().second;
        v = get_nodes_count(); // position of new node
        store_node(v, new_node);
        path.pop_back();
    } while (!path.empty());

    // root node splitted, need to create new root
    // save old root to new position
    auto pos = get_nodes_count();
    store_node(pos, node);
    // create new root
    BPlusNode<K, SIZE> new_root(k, pos, v);
    store_node(0, new_root);
}

template <int BUF_SIZE>
struct BPlusCharBuf
{
    // Constructors
    BPlusCharBuf()
    {
        buf[0] = '\0';
    }
    BPlusCharBuf(const char *str)
    {
        strncpy(buf, str, BUF_SIZE);
    }
    // Assignment operator
    BPlusCharBuf &operator=(const char *str)
    {
        strncpy(buf, str, BUF_SIZE);
        return *this;
    }
    // Comparison operators
    bool operator==(const BPlusCharBuf &rhs) const
    {
        return strncmp(buf, rhs.buf, BUF_SIZE) == 0;
    }
    bool operator!=(const BPlusCharBuf &rhs) const
    {
        return strncmp(buf, rhs.buf, BUF_SIZE) != 0;
    }
    bool operator<(const BPlusCharBuf &rhs) const
    {
        return strncmp(buf, rhs.buf, BUF_SIZE) < 0;
    }
    bool operator>(const BPlusCharBuf &rhs) const
    {
        return strncmp(buf, rhs.buf, BUF_SIZE) > 0;
    }
    bool operator<=(const BPlusCharBuf &rhs) const
    {
        return strncmp(buf, rhs.buf, BUF_SIZE) <= 0;
    }
    bool operator>=(const BPlusCharBuf &rhs) const
    {
        return strncmp(buf, rhs.buf, BUF_SIZE) >= 0;
    }
    // stream operator
    friend std::ostream &operator<<(std::ostream &os, const BPlusCharBuf &rhs)
    {
        os << rhs.buf;
        return os;
    }
    char buf[BUF_SIZE];
};

template <int BUF_SIZE, int SIZE>
class BPlusIndexCharBuf : public BPlusIndex<BPlusCharBuf<BUF_SIZE>, SIZE>
{
};