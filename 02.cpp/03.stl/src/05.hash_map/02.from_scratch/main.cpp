#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <iterator>

//yet!

struct IntHash {
    //Q. what is size_t?
    //A. size_t는 unsigned integer type이고, 0 ~ 2^64 - 1 사이의 값들을 표현할 수 있다.
    size_t operator()(int key) const { //const indicates that this function wont change state of variable.
        return key;
    }
};

template<typename Key, typename Value, typename Hash = IntHash>
class HashMap {
private:
    //Q. object that saves list of <Object, Object>
    typedef std::pair<Key, Value> KeyValuePair;
    typedef std::list<KeyValuePair> Bucket;
    typedef typename std::vector<Bucket> Table; //Vector<List<Key,Value>> -> Table
    Table table;

    //Q. hash function?
    Hash hashFunction;

    size_t hash(const Key& key) const {
        return hashFunction(key) % table.size();
    }
    
public:
    HashMap(size_t size = 10) : table(size) {};
    void insert(const Key& key, const Value& value) {
        //calculate key using hash()
        size_t index = hash(key);

        //find key in table
        Bucket& bucket = table[index];
        typename Bucket::iterator it = std::find_if(bucket.begin(), bucket.end(), [key](const KeyValuePair& pair) {
            return pair.first == key;
        });
        //if key exists, update value.
        //Q. does it override? or how to collision handling?
        if(it != bucket.end()) {
            it->second = value; //update existing key
        } else {
            //if key doesn't exist, insert key and value.
            bucket.push_back(std::make_pair(key, value)); //insert new key-value pair
        }

    };
    bool find(const Key& key, Value& value) {
        //calculate key using hash()
        size_t index = hash(key);
        Bucket& bucket = table[index];

        //find key in table
        typename Bucket::const_iterator it = std::find_if(bucket.begin(), bucket.end(), [key](const KeyValuePair& pair) {
            return pair.first == key;
        });
        if(it != bucket.end()) {
            value = it->second;
            return true;
        }
        return false;
    };
    bool remove(const Key& key) {
        size_t index = hash(key);
        Bucket& bucket = table[index];
        typename Bucket::iterator it = std::find_if(bucket.begin(), bucket.end(), [key](const KeyValuePair& pair) {
            return pair.first == key;
        });
        if(it!= bucket.end()) {
            bucket.erase(it);
            return true;
        }
        return false;
    };
};

int main(){
    HashMap<int, std::string> map;
    map.insert(1, "One");
    map.insert(2, "Two");
    map.insert(3, "Three");

    std::string value;
    if(map.find(2, value)) {
        std::cout << value << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    map.remove(2);

    if(map.find(2, value)) {
        std::cout << value << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    return 0;
}