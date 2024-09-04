#ifndef HASHMAP_H
#define HASHMAP_H


#include <string>
#include <iostream>

template <typename T>
class HashMap
{
public:
	HashMap(double max_load = 0.75); // constructor
	~HashMap(); // destructor; deletes all of the items in the hashmap
	int size(); // return the number of associations in the hashmap

	// The insert method associates one item (key) with another (value).
	// If no association currently exists with that key, this method inserts
	// a new association into the hashmap with that key/value pair. If there is
	// already an association with that key in the hashmap, then the item
	// associated with that key is replaced by the second parameter (value).
	// Thus, the hashmap must contain no duplicate keys.
	void insert(const std::string& key, const T& value);

	// If no association exists with the given key, return nullptr; otherwise,
	// return a pointer to the value associated with that key. This pointer can be
	// used to examine that value or modify it directly within the map.
	T* find(const std::string& key) const;

	// Defines the bracket operator for HashMap, so you can use your map like this:
	// your_map["david"] = 2.99;
	// If the key does not exist in the hashmap, this will create a new entry in
	// the hashmap and map it to the default value of type T. Then it will return a
	// reference to the newly created value in the map.
	T& operator[](const std::string& key);
	//size_t h = std::hash<std::string>()(s);

	//void printMap() {
	//	for (int i = 0; i < m_bucketAmount; i++) {
	//		Node* current = m_hashArray[i];
	//		std::cerr << i << "th BUCKET";
	//		while (current != nullptr) {
	//			std::cerr << " -> " << current->key;
	//			current = current->next;
	//		}
	//		std::cerr << std::endl;
	//	}
	//	std::cerr << std::endl << std::endl;
	//}
private:
	struct Node {
		std::string key;
		T value;
		Node* next;
		Node(const std::string& key, const T& value) : key(key), value(value), next(nullptr) {}
	};

	double m_maxLoad;
	int m_associationCount;
	int m_bucketAmount;
	Node** m_hashArray;

	size_t hash(const std::string& key) const {
		return std::hash<std::string>()(key) % m_bucketAmount;
	}

	void rehash() {
		int old_bucketAmount = m_bucketAmount;
		m_bucketAmount *= 2;

		Node** new_hashArray = new Node * [m_bucketAmount];
		for (int i = 0; i < m_bucketAmount; i++) {
			new_hashArray[i] = nullptr;
		}

		for (int i = 0; i < old_bucketAmount; i++) {
			Node* current = m_hashArray[i];
			while (current != nullptr) {
				size_t index = hash(current->key);
				Node* next = current->next;
				current->next = new_hashArray[index];
				new_hashArray[index] = current;
				current = next;
			}
		}

		delete[] m_hashArray;
		m_hashArray = new_hashArray;
	}
};

template <typename T>
HashMap<T>::HashMap(double max_load) : m_maxLoad(max_load), m_associationCount(0), m_bucketAmount(10), m_hashArray(new Node* [m_bucketAmount]) {
	for (int i = 0; i < m_bucketAmount; i++) {
		m_hashArray[i] = nullptr;
	}
}

template <typename T>
HashMap<T>::~HashMap() {
	for (int i = 0; i < m_bucketAmount; i++) {
		Node* head = m_hashArray[i];
		while (head != nullptr) {
			Node* nodeToDelete = head;
			head = head->next;
			delete nodeToDelete;
		}
	}
	delete[] m_hashArray;
}

template<typename T>
inline int HashMap<T>::size()
{
	return m_associationCount;
}

template <typename T>
inline void HashMap<T>::insert(const std::string& key, const T& value) {
	size_t hashIndex = hash(key);

	T* k = find(key);

	if (k == nullptr) {
		Node* insertNode = new Node(key, value);
		insertNode->next = m_hashArray[hashIndex];
		m_hashArray[hashIndex] = insertNode;
		m_associationCount++;
	}
	else {
		*k = value;
	}

	if (size() > m_maxLoad * m_bucketAmount) rehash();
}


template <typename T>
inline T* HashMap<T>::find(const std::string& key) const {
	size_t hashIndex = hash(key);

	Node* current = m_hashArray[hashIndex];
	while (current != nullptr) {
		if (current->key == key) {
			return &(current->value);
		}
		current = current->next;
	}
	return nullptr;
}

template<typename T>
inline T& HashMap<T>::operator[](const std::string& key) {
	T* itemFromKey = find(key);

	if (itemFromKey == nullptr) {
		insert(key, T());
		return *find(key);
	}
	return *itemFromKey;
}

#endif