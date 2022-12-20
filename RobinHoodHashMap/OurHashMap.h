// OurMap.h
// Header file for our Map

#pragma once

#include <ostream>
#include "error.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

template <typename KeyType,typename ValueType>
class OurHashMap
{
public:
    OurHashMap();
    ~OurHashMap();

    OurHashMap(OurHashMap& map2); // todo
    OurHashMap& operator=(const OurHashMap& map2); // todo

    void clear();
    int size() const;
    bool isEmpty() const;

    void put(const KeyType& key, const ValueType& value);
    void remove(const KeyType& key);
    bool containsKey(const KeyType& key) const;

    Vector<KeyType> keys() const;
    Vector<ValueType> values() const;

    ValueType& operator[] (const KeyType& key);

private:
    struct HashNode
    {
        KeyType key;
        ValueType value;
        int hashcode = -1;  //初始化为-1，表示没有元素。因为如果有元素，哈希值的取值范围一定是0~n-1
    };

    HashNode* array;
    int numItems;
    int capacity;

    static const int INITIAL_CAPACITY = 64;

    int getHashCode(const KeyType& key) const;
    void rehash();
};

template <typename KeyType,typename ValueType>
const int OurHashMap<KeyType, ValueType>::INITIAL_CAPACITY;

template <typename KeyType,typename ValueType>
OurHashMap<KeyType, ValueType>::OurHashMap()
{
    numItems = 0;
    capacity = INITIAL_CAPACITY;
    array = new HashNode[capacity];

}

template <typename KeyType,typename ValueType>
OurHashMap<KeyType, ValueType>::~OurHashMap()
{
    delete [] array;
}

template <typename KeyType,typename ValueType>
OurHashMap<KeyType, ValueType>::OurHashMap(OurHashMap& map2)
{
    numItems = 0;
    capacity = max(INITIAL_CAPACITY,map2.capacity);
    array = new HashNode[capacity];
    for(auto key2: map2.keys())
    {
       put(key2,map2[key2]);
    }

}

template <typename KeyType,typename ValueType>
OurHashMap<KeyType, ValueType>& OurHashMap<KeyType, ValueType>::operator=(const OurHashMap& map2)
{
    clear();
    for(auto key: map2.keys())
    {
       put(key,map2[key]);
    }

    return *this;
}


template <typename KeyType,typename ValueType>
void OurHashMap<KeyType, ValueType>::clear()
{
    for(int i = 0; i < capacity; i++)
    {
        array[i].hashcode = -1;
    }
    numItems = 0;
}

template <typename KeyType,typename ValueType>
int OurHashMap<KeyType, ValueType>::size() const
{
    return numItems;
}

template <typename KeyType,typename ValueType>
bool OurHashMap<KeyType, ValueType>::isEmpty() const
{
    return numItems == 0;
}

template <typename KeyType,typename ValueType>
Vector<KeyType> OurHashMap<KeyType, ValueType>::keys() const
{
    Vector<KeyType> result;
    for(int i = 0; i < capacity; i++)
    {
        if(array[i].hashcode != -1)
        {
            result.add(array[i].key);
        }
    }
    return result;
}

template <typename KeyType,typename ValueType>
Vector<ValueType> OurHashMap<KeyType, ValueType>::values() const
{
    Vector<ValueType> result;
    for(int i = 0; i < capacity; i++)
    {
        if(array[i].hashcode != -1)
        {
            result.add(array[i].value);
        }
    }
    return result;
}


template <typename KeyType,typename ValueType>
void OurHashMap<KeyType, ValueType>::put(const KeyType& key, const ValueType& value)
{
    int i = getHashCode(key);
    HashNode newNode{key,value,i};

    int idx = i;
    while(true)
    {
        if(array[idx].hashcode == -1)
        {
            array[idx] = newNode;
            numItems ++;

            if(numItems > capacity * 0.5)
                rehash();
            return;
        }
        else if(array[idx].hashcode < i)
        {
            idx = (idx + 1) % capacity;
        }
        else if(array[idx].hashcode == i)
        {
            if(array[idx].key == key)
            {
                array[idx].value = value;
                return;
            }
            else
            {
                idx = (idx + 1) % capacity;
            }
        }
        else
        {
            swap(array[idx],newNode);
            idx = (idx + 1) % capacity;
        }


    }

}

template <typename KeyType,typename ValueType>
bool OurHashMap<KeyType, ValueType>::containsKey(const KeyType& key) const
{
    int i = getHashCode(key);
    int idx = i;

    while(true)
    {
        if(array[idx].hashcode == -1)
        {
            return false;
        }

        else if(array[idx].hashcode < i)
        {
            idx = (idx + 1) % capacity;
        }

        else if(array[idx].hashcode == i)
        {
            if(array[idx].key == key)
                return true;
            idx = (idx + 1) % capacity;
        }
        else
            return false;

    }

}

template <typename KeyType,typename ValueType>
void OurHashMap<KeyType, ValueType>::remove(const KeyType& key)
{
    int i = getHashCode(key);
    int idx = i;

    while(true)
    {
        if(array[idx].hashcode == -1)
        {
            return;
        }
        else if(array[idx].hashcode < i)
        {
            idx = (idx + 1) % capacity;
        }
        else if(array[idx].hashcode == i)
        {
            if(array[idx].key == key)
            {
                int next = (idx + 1) % capacity;
                while(next != array[next].hashcode && array[next].hashcode != -1) //一定是不等于 因为存在63（60），0（60），1（60）这种情况） 是个环
                {
                    array[idx] = array[next];
                    idx = next;
                    next = (next + 1) % capacity;
                }
                array[idx].hashcode = -1;
                numItems--;
                return;
            }
            idx = (idx + 1) % capacity;
        }
        else
        {
            return;
        }
    }
}

template <typename KeyType,typename ValueType>
ValueType& OurHashMap<KeyType, ValueType>::operator[] (const KeyType& key)
{
    int i = getHashCode(key);
    int idx = i;
    int record;
    int flag = 0;

    HashNode newNode{key,ValueType(),i};

    while(true)
    {
        if(array[idx].hashcode == -1)
        {
            array[idx] = newNode;
            numItems++;

            if(numItems > capacity * 0.5)
                rehash();

            if(array[idx].key == key)
            {
                return array[idx].value;
            }
        }
        else if(array[idx].hashcode < i)
        {
            idx = (idx + 1) % capacity;
        }
        else if(array[idx].hashcode == i)
        {
            if(array[idx].key == key)
            {
                return array[idx].value;
            }
            idx = (idx + 1) % capacity;
        }
        else
        {
            if(flag == 0)
            {
                record = idx;
                swap(array[idx],newNode);
                idx = (idx + 1) % capacity;
                flag++;
            }
            else
            {
                swap(array[idx],newNode);
                idx = (idx + 1) % capacity;
            }
        }
    }
    return array[record].value;
}

template <typename KeyType,typename ValueType>
int OurHashMap<KeyType, ValueType>::getHashCode(const KeyType& key) const
{
    return hash<KeyType>{}(key) % capacity;
}

template <typename KeyType,typename ValueType>
void OurHashMap<KeyType, ValueType>::rehash()
{
    Vector<HashNode> hashnodes;
    for(int i=0; i<capacity;i++)
    {
        if(array[i].hashcode != -1)
        {
            hashnodes.add(array[i]);
        }
    }

    delete[] array;

    numItems = 0;
    capacity *= 2;
    array = new HashNode[capacity];

    for(auto hashnode: hashnodes)
    {
        put(hashnode.key,hashnode.value);
    }
}

//PROVIDED_TEST("Test OurSet constructor and destructor.")
//{
//    OurHashMap<string> ss;
//    EXPECT(ss.isEmpty());
//    EXPECT_EQUAL(ss.size(), 0);
//}

//PROVIDED_TEST("Test add and contains functions.")
//{
//    OurHashMap<string> ss;

//    /* Test add function to add some elements to the set. */
//    ss.add("cat");
//    EXPECT(!ss.isEmpty());
//    EXPECT_EQUAL(ss.size(), 1);

//    ss.add("dog");
//    EXPECT_EQUAL(ss.size(), 2);

//    Vector<string> animals = {"fish", "zebra", "aardvark", "lion", "meerkat", "marmot", "bird", "eagle"};
//    for (const string& animal: animals)
//    {
//        ss.add(animal);
//    }
//    EXPECT_EQUAL(ss.size(), 10);

//    /* Display current state of the set. */
//    cout << endl << "Full set: " << ss.values() << endl;

//    /* Test contains function to see if add operations were successful. */
//    EXPECT(ss.contains("cat"));
//    EXPECT(ss.contains("dog"));
//    EXPECT(ss.contains("fish"));
//    EXPECT(ss.contains("marmot"));

//    EXPECT(!ss.contains("hamster"));
//    EXPECT(!ss.contains("platypus"));
//}

//Vector<string> sorted(Vector<string> x)
//{
//    x.sort();
//    return x;
//}

//PROVIDED_TEST("Test remove function")
//{
//    OurHashMap<int,string> ss;

//    Vector<string> animals = {"cat", "dog", "fish", "zebra", "aardvark", "lion", "meerkat", "marmot", "bird", "eagle"};
//    for (string animal: animals)
//    {
//        ss.add(animal);
//    }
//    EXPECT_EQUAL(ss.size(), 10);
//    Vector<string> ans = {"aardvark", "bird", "cat", "dog", "eagle", "fish", "lion", "marmot", "meerkat", "zebra"};
//    EXPECT_EQUAL(sorted(ss.values()), ans);

//    ss.remove("bird");
//    EXPECT(!ss.contains("bird"));
//    EXPECT_EQUAL(ss.size(), 9);
//    ans = {"aardvark", "cat", "dog", "eagle", "fish", "lion", "marmot", "meerkat", "zebra"};
//     EXPECT_EQUAL(sorted(ss.values()), ans);

//    ss.remove("lion");
//    EXPECT(!ss.contains("lion"));
//    EXPECT_EQUAL(ss.size(), 8);
//    ans = {"aardvark", "cat", "dog", "eagle", "fish", "marmot", "meerkat", "zebra"};
//    EXPECT_EQUAL(sorted(ss.values()), ans);

//    ss.remove("fish");
//    EXPECT(!ss.contains("fish"));
//    EXPECT_EQUAL(ss.size(), 7);
//    ans = {"aardvark", "cat", "dog", "eagle", "marmot", "meerkat", "zebra"};
//    EXPECT_EQUAL(sorted(ss.values()), ans);

//    ss.remove("cat");
//    EXPECT(!ss.contains("cat"));
//    EXPECT_EQUAL(ss.size(), 6);
//    ans = {"aardvark", "dog", "eagle", "marmot", "meerkat", "zebra"};
//     EXPECT_EQUAL(sorted(ss.values()), ans);

//    ss.clear();
//    ans = {};
//    EXPECT_EQUAL(ss.size(), 0);
//    EXPECT_EQUAL(sorted(ss.values()), ans);
//}

//PROVIDED_TEST("Test OurHashSet constructor and destructor.")
//{
//    OurHashMap<string> ss;
//    EXPECT(ss.isEmpty());
//    EXPECT_EQUAL(ss.size(), 0);
//}

//PROVIDED_TEST("Test OurHashSet methods.")
//{
//    OurHashMap<int> ss;

//    /* Test add function to add some elements to the set. */
//    ss.add(2000);
//    EXPECT(!ss.isEmpty());
//    EXPECT_EQUAL(ss.size(), 1);

//    ss.add(25);
//    EXPECT_EQUAL(ss.size(), 2);

//    Vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 25};
//    for (int num: nums)
//    {
//        ss.add(num);
//    }
//    EXPECT_EQUAL(ss.size(), 10);

//    /* Test contains function to see if add operations were successful. */
//    EXPECT(ss.contains(1));
//    EXPECT(ss.contains(2000));
//    EXPECT(ss.contains(3));
//    EXPECT(ss.contains(5));

//    EXPECT(!ss.contains(244));
//    EXPECT(!ss.contains(11));

//    // Test resize
//    nums = {98, 97, 196, 195, 82, 61, 27, 100, 108, 237, 125};
//    for (int num: nums)
//    {
//        ss.add(num);
//    }
//    EXPECT(ss.contains(1));
//    EXPECT(ss.contains(125));
//    EXPECT_EQUAL(ss.size(), 21);

//    ss.remove(125);
//    EXPECT_EQUAL(ss.size(), 20);
//    EXPECT(!ss.contains(125));

//    ss.remove(125);
//    EXPECT_EQUAL(ss.size(), 20);

//    ss.remove(1);
//    EXPECT_EQUAL(ss.size(), 19);

//    ss.clear();
//    EXPECT_EQUAL(ss.size(), 0);
//}
