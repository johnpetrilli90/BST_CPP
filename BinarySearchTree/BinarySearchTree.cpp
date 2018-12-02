// BinarySearchTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
// John Petrilli

#include "pch.h"			//VS 2017 needed to compile
#include <algorithm>		//??
#include <cassert>			//for assert calls
#include <ctime>			//for timing performance and efficiency
#include <fstream>			//for file stream
#include <iostream>			//For cin cout operations
#include <string>
#include <vector>


using namespace std;

// Filename to read words from.
const string PATH = "alad10.txt";

// Maximum number of words to process. You can set this to a smaller
// value, such as 10 or 100, for testing purposes. 
const int MAX_WORDS = std::numeric_limits<int>::max();

//////////////////////////////////////////////////////////////////////////////
// WordCount
//////////////////////////////////////////////////////////////////////////////

// Word count entry. Stores a word in a string, and an int count of
// how many times that word ocurred. 
class WordCount {
public:
	WordCount(string& word, int count) {
		_word = word;
		_count = count;
	}

	// Default constructor; empty string and zero count. 
	WordCount() {
		_word = "";
		_count = 0;
	}

	// Overloaded comparison operators to allow WordCount objects to
	// work as elements of binary search trees. 
	bool operator< (const WordCount& other) const {
		return _word < other._word;
	}
	bool operator== (const WordCount& other) const {
		return _word == other._word;
	}
	bool operator!= (const WordCount& other) const {
		return _word != other._word;
	}
	bool operator> (const WordCount& other) const {
		return _word > other._word;
	}

	// Overloaded stream insertion operator to allow WordCount objects
	// to be printed to cout.
	friend ostream& operator<<(ostream& out, const WordCount& s) {
		out << "'" << s._word << "'=" << s._count;
		return out;
	}

	// Getters and setters.
	int count() { return _count; }
	void set_count(int x) { _count = x; } 
	void increment_count() { _count++; } 
	string& word() { return _word; }
	void set_word(const string& word) { _word = word; }

private:
	string _word; 
	int _count;
};


//////////////////////////////////////////////////////////////////////////////
// 2-­‐3 Tree Node Template Class
////////////////////////////////////////////////////////////////////////////// 
template<class E>
// Node class for 2-­‐3 tree 
class Node {
public:
	//2-­‐Node constructor
	Node(Node<E>* left, Node<E>* right, E Lvalue) {
		_left = left;
		_right = right;
		_Lvalue = Lvalue;
		_middle = nullptr; 
		TwoNode = true; 
		ThreeNode = false;
	}
	//3-­‐Node constructor
	Node(Node<E>* left, Node<E>* right, Node<E>* middle, E Lvalue, E Rvalue) {
		_left = left;
		_right = right;
		_middle = middle;
		_Lvalue = Lvalue;
		_Rvalue = Rvalue; 
		TwoNode = false; 
		ThreeNode = true;
	}

	//two node flag
	bool isTwoNode() { return TwoNode; }
	
	//three node flag
	bool isThreeNode() { return ThreeNode; }
	
	//set leaf flag 
	bool isLeaf() {
		if (left() == nullptr || right() == nullptr) { return true; }
		else { return false; }
	}

	//set  leaf  value 
	void setLeafValue() {
		if (left() != nullptr || right() != nullptr) { Leaf = false; }
		else { Leaf = true; }
	}

	//set two node flag 
	void set_TwoNode() {
		TwoNode = true; 
		ThreeNode = false;
		_middle = nullptr;
	}

	//set three node flag  
	void set_threeNode() {
		ThreeNode = true; 
		TwoNode = false;
	}

	//return value(s) of node
	E Lvalue() { return _Lvalue; } 
	E Rvalue() { return _Rvalue; }

	//set values
	void set_Lvalue(E Lvalue) { _Lvalue = Lvalue; } 
	void set_Rvalue(E Rvalue) { _Rvalue = Rvalue; }

	//set left,right,& middle ptrs
	void set_left(Node<E>* left) { _left = left; } 
	void set_right(Node<E>* right) { _right = right; }
	void set_middle(Node<E>* middle) { _middle = middle; }

	//return left, right,& middle ptrs 
	Node<E>* left() {return _left; } 
	Node<E>* right() {return _right; } 
	Node<E>* middle() {return _middle;}

private:
	Node<E> *_left, *_middle, *_right;	// left, right, and middle ptrs 
	E _Lvalue, _Rvalue;					// Lvalue is default for 2 Node 
	bool TwoNode, ThreeNode;			// 2/3 -­‐ node flag variable 
	bool Leaf;							// leaf flag
};

//////////////////////////////////////////////////////////////////////////////
// Binary Search Tree (BST)
////////////////////////////////////////////////////////////////////////////// 
template<class E>
class BST {
public:
	//constructor 
	BST() {
	_root = nullptr; parent = nullptr;
	_size = 0;
	}

	// returns size of BST
	int size() { return _size; }

	// returns true if empty
	bool isEmpty() { return size() == 0; }

	//true if value is in tree 
	bool contains(E& x) {
	if (isEmpty())
		return false;
	else
		return find_rec(x, _root) != nullptr;
}

	//find function, uses recursive helper 
	E& find(E& x) {
		E* match = find_rec(x, _root); 
		assert(match != nullptr); 
		return *match;
	}

	//add function 
	void add(E& x) {
		if (isEmpty()) {
			_root = new Node<E>(nullptr, nullptr, x);
			_root -> setLeafValue();
			_size++;
		}
		else {
			assert(!contains(x));
			_root -> setLeafValue(); 
			add_helper(x, _root);
			_size++;
		}
	}

private:
	Node<E> *_root, *parent; 
	int _size;

	///////////////////////////////////////////////////////////
	// Helper: for find function (recursion)
	/////////////////////////////////////////////////////////// 
	E* find_rec(E& x, Node<E>* p){
	if (x == p->Lvalue())
		return &p->Lvalue();
	else if (x == p->Rvalue())
		return &p->Rvalue();
	else if (p->isLeaf())
		return nullptr; 
	else if (x < p->Lvalue())
		return find_rec(x, p -> left());
	else if (x < p->Rvalue() && p->isThreeNode())
		return find_rec(x, p -> middle());
	else if (x > p->Lvalue() && p->isTwoNode())
		return find_rec(x, p -> right());
	else if (x > p->Rvalue() && p->isThreeNode())
		return find_rec(x, p->right());
	}

	///////////////////////////////////////////////////////////
	// Helper: for add/insert function
	/////////////////////////////////////////////////////////// 
	Node<E>* add_helper(E& x, Node<E>* root){
	///////////////////////////////////////////////////////
	// base cases
	/////////////////////////////////////////////////////// 
		root->setLeafValue();
		if (root->isLeaf()) {
			if (root->isTwoNode()) {
				if (x > root->Lvalue()) {
					root->set_Rvalue(x);
					root->set_middle(nullptr);
				}
				else {
					root->set_Rvalue(root->Lvalue());
					root->set_Lvalue(x);
					root->set_middle(nullptr);
				}
				return nullptr;
			}
			// for three node 
			else {
				if (x < root ->Lvalue()) {
					Node<E>* insert; 
					Node<E>* split;
					insert = new Node<E>(nullptr, nullptr, x); 
					split = new Node<E>(nullptr, nullptr, root->Rvalue()); 
					root->set_left(insert);
					root->set_right(split); 
					root->set_TwoNode(); 
					parent = root;
					delete root;
				}
				else if (x < root->Rvalue()) {
					Node<E>* split;
					parent = root;
					split = new Node<E>(nullptr, nullptr, root->Rvalue());

					root->set_TwoNode();
					root->set_left(nullptr);
					root->set_right(nullptr);
					parent->set_left(root);
					parent->set_right(split);
					parent->set_Lvalue(x);
					parent->set_TwoNode();
				}
				else {
					Node<E>* insert; Node<E>* split;
					insert = new Node<E>(nullptr, nullptr, x);
					split = new Node<E>(nullptr, nullptr, root->Lvalue());
					root->set_left(split);
					root->set_right(insert);
					root->set_Lvalue(root->Rvalue()); 
					root->set_TwoNode();
					parent = root; 
					delete root;
				}
				return parent;
			}
		}
		//////////////////////////////////////////////////////
		// downward phase
		////////////////////////////////////////////////////// 
		Node<E>* below;
		if (x < root->Lvalue()) {
			below = add_helper(x, root->left());
		}
		else if (root->isThreeNode() && x < root->Rvalue()) {
			below = add_helper(x, root->middle());
		}
		else {
			below = add_helper(x, root->right());
		}

		//////////////////////////////////////////////////////
		// upward phase
		////////////////////////////////////////////////////// 
		if(below->isLeaf()) { // was below == nullptr
			return nullptr;
		}
		else if (root->isTwoNode()) {
		//absorb below into root
		//put value in either L or R value and make three node 
			if(below->Lvalue() < root->Lvalue()){
				root->set_Rvalue(root->Lvalue());
				root->set_Lvalue(below->Lvalue());
				root->set_left(below->left());
				root->set_middle(below->right());
				root->set_threeNode();
			}
			else {
				root->set_Rvalue(below->Lvalue());
				root->set_middle(below->left()); 
				root->set_right(below->right()); 
				root->set_threeNode();
			}
			return nullptr;
		}
		else {	
			//split root like p.5 of turbak
			//this root is a three node, a split is necessary 
			if(below->Lvalue() < root->Lvalue()) {
				Node<E>* split;
				split = new Node<E>(root->middle(), root->right(), root->Rvalue());
				root->set_left(below); 
				root->set_right(split); 
				root->set_TwoNode(); 
				parent = root;
				delete root;
			}
			else if (below->Lvalue() < root->Rvalue()) {
				Node<E>* split;
				parent = root;
				split = new Node<E>(root->left(), below->left(), root->Lvalue());
				root->set_Lvalue(root->Rvalue());
				root->set_left(below->right()); 
				root->set_TwoNode();
				parent->set_left(split); 
				parent->set_right(root);
				parent->set_Lvalue(below->Lvalue()); 
				parent->set_TwoNode();
			}
			else {
				Node<E>* split;
				split = new Node<E>(root->left(), root->middle(), root->Lvalue()); 
				root->set_left(split);
				root->set_right(below);
				root->set_Lvalue(root->Rvalue());
				root->set_TwoNode();
				parent = root; 
				delete root;
			}
			return parent; //node that gets kicked up
		}
	}
};

//////////////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////////////

void report_count(BST<WordCount>& tree, const char* query) {
	string s(query);
	WordCount like(s, 0), &match = tree.find(like);
	cout << "'" << query << "' appears " << match.count() << " times" << endl;
}

//////////////////////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////////////////////

int main() {
	// Load the words out of the text file at PATH. 
	vector<string> tokens;
	cout << "Loading words... "; 
	ifstream  f(PATH.c_str()); 
	if (!f) {
		cout << "error: could not open '" << PATH << "'" << endl; 
		return 1;
	}
	while (f) {
		string token; 
		f >> token;
		if (!token.empty()) 
			tokens.push_back(token);
	}
	f.close();
	cout << tokens.size() << " tokens in document" << endl;

	// prepare to count the occurences of each token 
	cout << "counting word frequencies..." << endl;

	// start a timer
	clock_t start = clock();

	// create an empty BST 
	BST<WordCount> counts;

	// calculate how many words to read
	int n = min(static_cast<int>(tokens.size()), MAX_WORDS);

	// main loop to handle each token 
	for (int i = 0; i < n; i++) {

		string& token = tokens[i];

		// create a WordCount object to serve as the "x" in the
		// contains(...)	function 
		WordCount like(token, 1);
		if (!counts.contains(like)) {
			// We haven't seen this word before; add the WordCount with a
			// count of 1 to the BST. 
			counts.add(like);
		}
		else {
			// Otherwise the BST already contains a WordCount for this
			// word. Obtain a reference to the WordCount object already
			// inside the data structure. 
			WordCount& match = counts.find(like);

			// Now increment the count on that object. 
			match.increment_count();
		}
	}
	
	// Stop the timer. 
	clock_t end = clock();
	double elapsed_sec = (end - start) / static_cast<double>(CLOCKS_PER_SEC); 
	cout << "elapsed time = " << elapsed_sec << " sec" << endl;

	// Print results.
	cout << "distinct words = " << counts.size() << endl << endl; 

	return 0;
}
