// This is a correct but horribly inefficient implementation of Dictionary
// functionality.  Your job is to change DictionaryImpl to a more efficient
// correct implementation.  You can change it any way you like, subject to
// restrictions in the spec (e.g., that the only C++ library container you
// are allowed to use are vector, list, stack, and queue (and string); if you
// want anything fancier, implement it yourself).
#include "Dictionary.h"
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
using namespace std;

void removeNonLetters(string& s);

// This class does the real work of the implementation.
class DictionaryImpl
{
public:
    DictionaryImpl(int maxBuckets);
    ~DictionaryImpl() {};
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
private:
    int numBuckets;
    unsigned int mapFunct(const string word) const;
    vector<vector<string>> h_table;
};

DictionaryImpl::DictionaryImpl(int maxBuckets) : numBuckets(maxBuckets) {
    for (int i = 0; i != maxBuckets; i++) {
        vector<string> bucket;
        h_table.push_back(bucket);
    }
}

unsigned int DictionaryImpl::mapFunct(string word) const {
    hash<string> str_hash;
    unsigned int hashVal = str_hash(word);
    unsigned int bucket = hashVal % numBuckets;
    return bucket;
}

void DictionaryImpl::insert(string word) {
    if (!word.empty()) {
        removeNonLetters(word);
        string temp = word;
        sort(temp.begin(), temp.end());
        int bucket = mapFunct(temp); 
        for (int tries = 0; tries < numBuckets; tries++) {
            h_table[bucket].push_back(word);
            return;        
        }
    }
}

void DictionaryImpl::lookup(string letters, void callback(string)) const {
    if (callback == nullptr) { return; }
    if (letters.empty()) { return; }
    removeNonLetters(letters);
    string temp = letters;
    sort(temp.begin(), temp.end());
    int bucket = mapFunct(temp);
    for (int i = 0; i < h_table[bucket].size(); i++) {
        string temp1 = h_table[bucket][i];
        sort(temp1.begin(), temp1.end());
        if (temp == temp1) {
            callback(h_table[bucket][i]);
        }
    }
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end()); 
}

//******************** Dictionary functions ******************************

// These functions simply delegate to DictionaryImpl's functions
// You probably don't want to change any of this code

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters, callback);
}
