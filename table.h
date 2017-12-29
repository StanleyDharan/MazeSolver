/*************************************************************/
/*    V 1.1:                                                 */
/*      - added prototypes for numRecords() and isEmpty()    */
/*      - coded isEmpty() and numRecords() in SimpleTable    */
/*      - created empty functions for isEmpty() and          */
/*        numRecords() in LPTable                            */
/*************************************************************/

#include <string>
#include <utility>
#include <functional>
using namespace std;

template <class TYPE>
class Table{
public:
	Table(){}
	virtual bool update(const string& key, const TYPE& value)=0;
	virtual bool remove(const string& key)=0;
	virtual bool find(const string& key, TYPE& value)=0;
	virtual int numRecords() const = 0;
	virtual bool isEmpty() const = 0;
	virtual ~Table(){}
};

template <class TYPE>
class SimpleTable :public Table<TYPE>{

    struct Record{
        TYPE data_;
        string key_;
        Record(const string& key, const TYPE& data){
            key_ = key;
            data_ = data;
        }

    };

    Record** records_;   //the table
    int max_;           //capacity of the array
    int size_;          //current number of records held
    int search(const string& key);
    void sort();
    void grow();
public:
    SimpleTable(int maxExpected);
    SimpleTable(const SimpleTable& other);
    SimpleTable(SimpleTable&& other);
    virtual bool update(const string& key, const TYPE& value);
    virtual bool remove(const string& key);
    virtual bool find(const string& key, TYPE& value);
    virtual const SimpleTable& operator=(const SimpleTable& other);
    virtual const SimpleTable& operator=(SimpleTable&& other);
    virtual ~SimpleTable();
    virtual bool isEmpty() const{ return size_ == 0; }
    virtual int numRecords() const{ return size_; }
};


//returns index of where key is found.
template <class TYPE>
int SimpleTable<TYPE>::search(const string& key){
    int rc = -1;
    for (int i = 0; i<size_; i++){
        if (records_[i]->key_ == key){
            rc = i;
        }
    }
    return rc;
}
//sort the according to key in table
template <class TYPE>
void SimpleTable<TYPE>::sort(){
    int minIdx = 0;
    for (int i = 0; i<size_; i++){
        minIdx = i;
        for (int j = i + 1; j<size_; j++){
            if (records_[j]->key_ < records_[minIdx]->key_){
                minIdx = j;
            }
        }
        Record* tmp = records_[i];
        records_[i] = records_[minIdx];
        records_[minIdx] = tmp;
    }
}

//grow the array by one element
template <class TYPE>
void SimpleTable<TYPE>::grow(){
    Record** newArray = new Record*[max_ * 2];
    max_ = max_ * 2;
    for (int i = 0; i<size_; i++){
        newArray[i] = records_[i];
    }
    delete[] records_;
    records_ = newArray;
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int maxExpected) : Table<TYPE>(){
    records_ = new Record*[maxExpected];
    max_ = maxExpected;
    size_ = 0;
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other){
    records_ = new Record*[other.max_];
    max_ = other.max_;
    size_ = 0;
    for (int i = 0; i<other.size_; i++){
        update(other.records_[i]->key_, other.records_[i]->data_);
    }
}
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other){
    size_ = other.size_;
    max_ = other.max_;
    records_ = other.records_;
    other.records_ = nullptr;
    other.size_ = 0;
    other.max_ = 0;
}

template <class TYPE>
bool SimpleTable<TYPE>::update(const string& key, const TYPE& value){
    int idx = search(key);
    if (idx == -1){
        if (size_ == max_){
            grow();
        }
        records_[size_++] = new Record(key, value);
        sort();
    }
    else{
        records_[idx]->data_ = value;
    }
    return true;
}

template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key){
    int idx = search(key);
    if (idx != -1){
        delete records_[idx];
        for (int i = idx; i<size_ - 1; i++){
            records_[i] = records_[i + 1];
        }
        size_--;
        return true;
    }
    else{
        return false;
    }
}

template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value){
    int idx = search(key);
    if (idx == -1)
        return false;
    else{
        value = records_[idx]->data_;
        return true;
    }
}

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other){
    if (this != &other){
        if (records_){
            int sz = size_;
            for (int i = 0; i<sz; i++){
                remove(records_[0]->key_);
            }
            delete[] records_;
        }
        records_ = new Record*[other.max_];
        max_ = other.max_;
        size_ = 0;
        for (int i = 0; i<other.size_; i++){
            update(other.records_[i]->key_, other.records_[i]->data_);
        }

    }
    return *this;
}
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other){
    swap(records_, other.records_);
    swap(size_, other.size_);
    swap(max_, other.max_);
    return *this;
}
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable(){
    if (records_){
        int sz = size_;
        for (int i = 0; i<sz; i++){
            remove(records_[0]->key_);
        }
        delete[] records_;
    }
}


template <class TYPE>
class LPTable:public Table<TYPE>{
    struct Record {
        TYPE data_;
        string key_;
        Record(const string& key, const TYPE& data){
            key_ = key;
            data_ = data;
        }
    };
    Record** records_;   //the table
    int max_;           //capacity of the array
    int size_;          //current number of records held
    int maxExpected_;
    int search(const string& key);
public:
	LPTable(int maxExpected);
    friend void swap(LPTable<TYPE>& first, LPTable<TYPE>& second){
        using std::swap;
        swap(first.records_, second.records_);
        swap(first.max_, second.max_);
        swap(first.maxExpected_, second.maxExpected_);
        swap(first.size_, second.size_);
    }
    //int search(const string& key);
	LPTable(const LPTable& other);
	LPTable(LPTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const LPTable& operator=(const LPTable& other);
	virtual const LPTable& operator=(LPTable&& other);
	virtual ~LPTable();
    virtual bool isEmpty() const { return size_ == 0; }
    virtual int numRecords() const { return size_; }
    int hashedIndex(const string& key);

};
template <class TYPE>
int LPTable<TYPE>::hashedIndex(const string& key){
    std::hash<std::string>str_hash;
    size_t hash_key = str_hash(key);
    int indexie = hash_key%maxExpected_;
    return indexie;
}
//returns index of where key is found or index of first
//nullptr after where key should have been found
template <class TYPE>
int LPTable<TYPE>::search(const string& key){
    //creating indexie from key given
    int indexie = hashedIndex(key);
    //comparing key to value key @position indexie.
    if (records_[indexie] == nullptr){
        return indexie;
    }
    else if (records_[indexie]->key_ == key){
        return indexie;
    }
    else{
        int beginningToIndexie = indexie;
        bool indexToMaxExpectedFull = false;

        if (indexie != max_ - 1){
            indexie++;
        }
        //looking for a null element in records_
        while (records_[indexie] != nullptr){
            //First checks from indexie to maxExpected_.
            //if no null elements found & key not found
            //search from 0 to indexie;
            if (records_[indexie]->key_ == key){
                //Found the key in table
                return indexie;
            }
            else if (!indexToMaxExpectedFull && indexie != max_ - 1/*max_*/ || indexToMaxExpectedFull && indexie <= beginningToIndexie){
                indexie++;
            }
            else{
                //resets indexie to 0 and searches
                //from 0 to indexie to prevent overlapping
                //in search.
                if (indexToMaxExpectedFull){
                    //table is full returns -1 (false)
                    return -1;
                }
                indexie = 0;
                indexToMaxExpectedFull = true;
            }
        }
        //found a null element in table, can insert new data here!
        return indexie;
    }
}

template <class TYPE>
LPTable<TYPE>::LPTable(int maxExpected): Table<TYPE>(){
    maxExpected_ = maxExpected;
    max_ = maxExpected_ * 1.75;   
    //dont forget tonull records
    records_ = new Record*[max_]();   
    size_ = 0;
}

template <class TYPE>
LPTable<TYPE>::LPTable(const LPTable<TYPE>& other){
    max_ = other.max_;
    maxExpected_ = other.maxExpected_;
    size_ = other.size_;
    records_ = new Record*[max_]();
    std::copy(other.records_, other.records_ + max_, records_);
}

template <class TYPE>
LPTable<TYPE>::LPTable(LPTable<TYPE>&& other) {
    *this = other;
}

template <class TYPE>
bool LPTable<TYPE>::update(const string& key, const TYPE& value){
    // keyFound is 1 of 2 possible values
    // 1. keyFound = -1; the table is full, key was not found
    // 2. keyFound > -1; either an empty spot was found OR the key was found
    if (numRecords() != maxExpected_){
        int keyFound = search(key);
        records_[keyFound] = new Record(key, value);
        size_++;
        return true;
    }
    else{
        int keyFound = search(key);
        if (records_[keyFound] == nullptr){
            return false;
        }
        else{
            records_[keyFound]->data_ = value;
            return true;
        }
    }
}

template <class TYPE>
bool LPTable<TYPE>::remove(const string& key){
    int keyIndex = search(key); // retrieves the location of the key in the table
    if (keyIndex == -1 || records_[keyIndex] == nullptr){ //making sure the record exists in table
        return false;
    }
    else{
        delete records_[keyIndex];
        records_[keyIndex] = nullptr;
        size_--;
        int prob = keyIndex + 1; //setting position to 1 index after the deleted record
        int deleteFlag = keyIndex; //marking the index of the deleted record
        while (records_[prob] != nullptr){ //looping through until the end of the cluster
            int key = hashedIndex(records_[prob]->key_); // getting expected hash index of record's key
            if (deleteFlag >= key && deleteFlag <= prob){ // comparing the deleted record index to the expected hash index of the record and the records actual index (making sure its within range)
                records_[deleteFlag] = new Record(records_[prob]->key_, records_[prob]->data_); // inserting record into empty spot
                delete records_[prob]; //deleting record making it the new empty spot
                records_[prob] = nullptr;
                deleteFlag = prob; //marking the location
                if (prob == max_ - 1){ //if at the end of expected records go to beginning of array
                    prob = 0;
                }
                else{
                    prob++;
                }
            }
            else{
                prob++;
            }
        }
        return true;
    }
}

template <class TYPE>
bool LPTable<TYPE>::find(const string& key, TYPE& value){
    int keyIndex = search(key);

    if (keyIndex == -1 || records_[keyIndex] == nullptr){
        return false;
    }
    else{
        value = this->records_[keyIndex]->data_;
        return true;
    }
}

template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(const LPTable<TYPE>& other){
    if (this != &other){
        LPTable<TYPE> copy(other);
        std::swap(*this, copy);
    }
    return *this;
}
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(LPTable<TYPE>&& other){
    swap(*this, other);
    delete[] &other;
	return *this;
}

template <class TYPE>
LPTable<TYPE>::~LPTable(){
    delete[] records_;
    records_ = nullptr;
    max_ = 0;
    size_ = 0;
    maxExpected_ = 0;
}