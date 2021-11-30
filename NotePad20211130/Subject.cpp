//Subject.cpp
#include"Subject.h"
#include"Observer.h"

Subject::Subject(Long capacity)
   :observers(capacity){
	this->capacity = capacity;
	this->length = 0;
}

Subject::~Subject() {
}

Long Subject::Attach(Observer* observer) {
	Long index;

	if (this->length < this->capacity) {
		index = this->observers.Store(this->length, observer);
	}
	else {
		index = this->observers.AppendForRear(observer);
		this->capacity++;
	}
	this->length++;
	return index;
}

#if 0
Long Subject::Detach(Long index) {

	index = this->observers.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}
#endif

Long Subject::Detach(Observer* observer) {
	int index;

	index = this->observers.LinearSearchUnique(observer, CompareObservers);

	index = this->observers.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}



void Subject::Notify() {
	Observer* observer;
	Long i = 0;

	while (i < this->length) {
		observer = this->observers.GetAt(i);
		observer->Update();
		i++;
	}
}

int CompareObservers(void* one, void* other) {
	int ret;

	Observer** one_ = (Observer**)one;

	if (*one_ == other) {
		ret = 0;
	}
	else {
		ret = -1;
	}

	return ret;
}











