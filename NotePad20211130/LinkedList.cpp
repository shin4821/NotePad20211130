//LinkedList.cpp
#include"LinkedList.h"
#include<iostream>
using namespace std;

#if 0
int Compare(void* one, void* other);
int main(int argc, char* argv[]) {
	LinkedList<int> linkedList;
	LinkedList<int> other;
	LinkedList<int>::Node* index;
	LinkedList<int>::Node* previous=0;
	LinkedList<int>::Node* (*indexes);
	Long count;
	Long i = 0;
	int key;


	index = linkedList.AppendFromTail(100);
	cout << index->GetObject() << endl;

	index = linkedList.AppendFromHead(200);
	cout << index->GetObject() << endl;

	index = linkedList.Last();
	cout << index->GetObject() << endl;

	index = linkedList.InsertBefore(index, 300);
	cout << index->GetObject() << endl;

	index = linkedList.First();
	cout << index->GetObject() << endl;

	index = linkedList.InsertAfter(index, 400);
	cout << index->GetObject() << endl;

	index = linkedList.Next();
	cout << index->GetObject() << endl;

	index = linkedList.Delete(index);
	if (index == 0) {
		cout << "지워졌습니다." << endl;
	}

	index = linkedList.DeleteFromTail();
	if (index == 0) {
		cout << "지워졌습니다." << endl;
	}

	index = linkedList.DeleteFromHead();
	if (index == 0) {
		cout << "지워졌습니다." << endl;
	}

	index = linkedList.First();
	cout << index->GetObject() << endl;

	index = linkedList.InsertBefore(index, 500);
	cout << index->GetObject() << endl;

	index = linkedList.AppendFromHead(600);
	cout << index->GetObject() << endl;

	linkedList.DeleteAllItems();

	index = linkedList.AppendFromTail(100);
	cout << index->GetObject() << endl;

	index = linkedList.AppendFromHead(200);
	cout << index->GetObject() << endl;


	key = 100;
	index = linkedList.LinearSearchUnique(&key, Compare);
	cout << index->GetObject() << endl;

	index = linkedList.Previous();
	cout << index->GetObject() << endl;

	index = linkedList.InsertBefore(index, 100);
	cout << index->GetObject() << endl;

	cout << "LinearSearchDuplicate" << endl;
	linkedList.LinearSearchDuplicate(&key, &indexes, &count, Compare);
	while (i < count) {
		cout << indexes[i]->GetObject() << endl;
		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
		indexes = 0;
	}

	index=other.AppendFromTail(50);
	cout << index->GetObject() << endl;

	index = other.AppendFromTail(60);
	cout << index->GetObject() << endl;

	index = other.AppendFromTail(70);
	cout << index->GetObject() << endl;

	LinkedList<int> another(other);
	cout << "전체보기" << endl;
	index = another.First();
	while (index != previous) {
		cout << index->GetObject() << endl;
		previous = index;
		index = another.Next();
	}

	cout << "나머지확인" << endl;
	index = another.First();
	cout << another.GetAt(index) << endl;

	cout << another.GetCurrent()->GetObject() << endl;

	cout << another.GetLength() << endl;

#if 0
	cout << "전체보기" << endl;
	index = linkedList.First();
	while (index != previous) {
		cout << index->GetObject() << endl;

		previous = index;
		index = linkedList.Next();
	}
#endif


	return 0;
}

int Compare(void* one, void* other) {
	int* one_ = (int*)one;
	int* other_ = (int*)other;
	int ret;

	if (*one_ == *other_) {
		ret = 0;
	}
	else if (*one_ > * other_) {
		ret = 1;
	}
	else if (*one_ < *other_) {
		ret = -1;
	}
	return ret;
}

#endif