//LinkedList.h
#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H
typedef signed long int Long;

template<typename T>
class LinkedList {
public:
	class Node {//Nested Class
		friend class LinkedList;

	    public:
			Node() {
				this->previous = this;
				this->next = this;
			}

			Node(T object) {
				this->previous = this;
				this->object = object;
				this->next = this;
			}
			Node(Node* previous, T object) {
				this->previous = previous;
				this->object = object;
				this->next = this;
			}
			Node(T object, Node* next) {
				this->previous = this;
				this->object = object;
				this->next = next;
			}
			Node(Node* previous, T object, Node* next) {
				this->previous = previous;
				this->object = object;
				this->next = next;
			}
			Node(const Node& source) {//복사생성자
				this->previous = source.previous;
				this->object = source.object;
				this->next = source.next;
			}
			Node& operator=(const Node& source) {//치환연산자
				this->previous = source.previous;
				this->object = source.object;
				this->next = source.next;

				return *this;
			}
			T& GetObject() const {
				return const_cast<T&>(this->object);
			}
			~Node() {//소멸자
			}
	    private:
			Node* previous;
			T object;
			Node* next;
	};
public:
	LinkedList();
	LinkedList(const LinkedList& source); //복사생성자
	LinkedList& operator=(const LinkedList& source);//치환연산자
	Node* AppendFromHead(T object);
	Node* AppendFromTail(T object);
	Node* InsertBefore(Node* index, T object);
	Node* InsertAfter(Node* index, T object);
	Node* Delete(Node* index);
	Node* DeleteFromHead();
	Node* DeleteFromTail();
	void DeleteAllItems();
	Node* LinearSearchUnique(void* key, int(*compare)(void*, void*));
	void LinearSearchDuplicate(void* key, Node** (*indexes), Long* count, int(*compare)(void*, void*));
	Node* First();
	Node* Previous();
	Node* Next();
	Node* Last();
	T& GetAt(Node* index);
	Long GetLength() const;
	Node* GetCurrent() const;

	~LinkedList();
private:
	Node* head;
	Node* tail;
	Long length;
	Node* current;
};

template<typename T>
LinkedList<T>::LinkedList() {
	this->head = 0;
	this->tail = 0;
	this->length = 0;
	this->current = 0;
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList& source) {//복사생성자
	Node* it;
	Node* previous = 0;

	this->tail = 0;
	it = source.head;
	while (it != previous) {
		if (this->tail != 0) {
			this->current = new Node(this->tail, it->object);
		}
		else {
			this->current = new Node(it->object);
		}
		if (this->tail != 0) {
			this->tail->next = this->current;
		}
		else {
			this->head = this->current;
		}
		this->tail = this->current;

		previous = it;
		it = it->next;
	}
	this->length = source.length;
	this->current = this->head;

	it = source.head;
	while (it != source.current) {
		this->current = this->current->next;
		it = it->next;
	}
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& source) {//치환연산자
	Node* it;
	Node* previous = 0;

	it = this->head;
	while (it != previous) {
		this->current = it->next;
		previous = it;
		if (it != 0) {
			delete it;
		}
		it = this->current;
	}
	this->tail = 0;
	it = source.head;
	previous = 0;
	while (it != previous) {
		if (this->tail != 0) {
			this->current = new Node(this->tail, it->object);
		}
		else {
			this->current = new Node(it->object);
		}
		if (this->tail != 0) {
			this->tail->next = this->current;
		}
		else {
			this->head = this->current;
		}
		this->tail = this->current;

		previous = it;
		it = it->next;
	}
	this->length = source.length;

	this->current = this->head;
	it = source.head;
	while (it != source.current) {
		this->current = this->current->next;
		it = it->next;
	}

	return *this;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::AppendFromHead(T object) {
	if (this->head != 0) {
		this->current = new Node(object, this->head);
	}
	else {
		this->current = new Node(object);
	}
	if (this->head != 0) {
		this->head->previous = this->current;
	}
	else {
		this->tail = this->current;
	}
	this->head = this->current;
	this->length++;
	
	return this->current;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::AppendFromTail(T object) {
	if (this->tail != 0) {
		this->current = new Node(this->tail, object);
	}
	else {
		this->current = new Node(object);
	}
	if (this->tail != 0) {
		this->tail->next = this->current;
	}
	else {
		this->head = this->current;
	}
	this->tail = this->current;
	this->length++;
	
	return this->current;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::InsertBefore(Node* index, T object) {
	if (this->head != index) {
		this->current = new Node(index->previous, object, index);
	}
	else {
		this->current = new Node(object, index);
	}
	if (this->head != index) {
		index->previous->next = this->current;
	}
	else {
		this->head = this->current;
	}
	index->previous = this->current;
	this->length++;

	return this->current;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::InsertAfter(Node* index, T object) {
	if (this->tail != index) {
		this->current = new Node(index, object, index->next);
	}
	else {
		this->current = new Node(index, object);
	}
	if (this->tail != index) {
		index->next->previous = this->current;
	}
	else {
		this->tail = this->current;
	}
	index->next = this->current;
	this->length++;

	return this->current;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::Delete(Node* index) {
	if (this->head != index && this->tail != index) {
		index->next->previous = index->previous;
		index->previous->next = index->next;
		this->current = index->next;
	}
	else if (this->head == index && this->tail != index) {
		index->next->previous = index->next;
		this->head = index->next;
		this->current = index->next;
	}
	else if (this->tail == index && this->head != index) {
		index->previous->next = index->previous;
		this->tail = index->previous;
		this->current = index->previous;
	}
	else {
		this->head = 0;
		this->tail = 0;
		this->current = 0;
	}

	if (index != 0) {
		delete index;
		index = 0;
	}
	this->length--;
	
	return index;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::DeleteFromHead() {
	Node* index;

	index = this->head;
	if (this->tail != index) {
		index->next->previous = index->next;
		this->head = index->next;
		this->current = index->next;
	}
	else {
		this->head = 0;
		this->tail = 0;
		this->current = 0;
	}
	if (index != 0) {
		delete index;
		index = 0;
	}

	this->length--;
	return index;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::DeleteFromTail(){
	Node* index;

	index = this->tail;
	if (this->head != index) {
		index->previous->next = index->previous;
		this->tail = index->previous;
		this->current = index->previous;
	}
	else {
		this->head = 0;
		this->tail = 0;
		this->current = 0;
	}
	if (index != 0) {
		delete index;
		index = 0;
	}
	this->length--;

	return index;
}

template<typename T>
void LinkedList<T>::DeleteAllItems() {
	Node* it;
	Node* previous = 0;

	it = this->head;
	while (it != previous) {
		this->current = it->next;
		previous = it;
		if (it != 0) {
			delete it;
		}
		it = this->current;
	}
	this->head = 0;
	this->tail = 0;
	this->current = 0;
	this->length = 0;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::LinearSearchUnique(void* key, int(*compare)(void*, void*)) {
	Node* index = 0;
	Node* it;
	Node* previous = 0;

	it = this->head;
	while (it != previous && compare(&it->object, key) != 0) {
		previous = it;
		it = it->next;
	}
	if (it != previous) {
		index = it;
	}

	return index;
}

template<typename T>
void LinkedList<T>::LinearSearchDuplicate(void* key, Node** (*indexes), Long* count, int(*compare)(void*, void*)) {
	Node* it;
	Node* previous = 0;
	Long i = 0;
	*count = 0;

	*indexes = new Node * [this->length];
	it = this->head;
	while (it != previous) {
		if (compare(&it->object, key) == 0) {
			(*indexes)[i] = it;
			(*count)++;
			i++;
		}
		previous = it;
		it = it->next;
	}
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::First() {
	this->current = this->head;
	return this->current;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::Previous() {
	this->current = this->current->previous;
	return this->current;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::Next() {
	this->current = this->current->next;
	return this->current;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::Last() {
	this->current = this->tail;
	return this->current;
}

template<typename T>
T& LinkedList<T>::GetAt(Node* index) {
	return index->object;
}

template<typename T>
inline Long LinkedList<T>::GetLength() const {
	return this->length;
}

template<typename T>
inline typename LinkedList<T>::Node* LinkedList<T>::GetCurrent() const {
	return this->current;
}


template<typename T>
LinkedList<T>::~LinkedList() {
	Node* it;
	Node* previous = 0;

	it = this->head;
	while (it != previous) {
		this->current = it->next;
		previous = it;
		if (it != 0) {
			delete it;
		}
		it = this->current;
	}
}


#endif//_LINKEDLIST_H



