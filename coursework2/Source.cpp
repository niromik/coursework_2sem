#include "Header.h"

info::info() {}
info::info(wstring surname_, wstring name_, wstring patronymic_,
	int recordBookNum_, int groupNum_, int scholarship_, vector<int> marks_) {
	surname = surname_;
	name = name_;
	patronymic = patronymic_;
	recordBookNum = recordBookNum_;
	groupNum = groupNum_;
	scholarship = scholarship_;
	marks = marks_;
}

Student::Student() {}
Student::Student(info value_) {
	value = value_;
}

StoGMap::StoGMap() {}
StoGMap::StoGMap(int groupId_, int studentId_, int recordBookNum_) {
	groupId = groupId_;
	studentId = studentId_;
	recordBookNum = recordBookNum_;
}

List::Node::Node(StoGMap value) {
	info = value;
	next = nullptr;
	prev = nullptr;
}

void List::deleteElement(Node* element) {
	if (element->next != nullptr && element->prev == nullptr) {
		first = element->next;
		first->prev = nullptr;
		element->next = nullptr;
	}
	else if (element->next == nullptr && element->prev != nullptr) {
		last = element->prev;
		last->next = nullptr;
		element->prev = nullptr;
	}
	else if (element->next != nullptr && element->prev != nullptr) {
		Node* nextElement = element->next;
		Node* previousElement = element->prev;
		nextElement->prev = previousElement;
		previousElement->next = nextElement;
		element->next = nullptr;
		element->prev = nullptr;
	}
	else {
		first = nullptr;
		last = nullptr;
	}
	delete element;
	_size--;
}

List::List() {
	first = nullptr;
	last = nullptr;
}

bool List::Empty() {
	return first == nullptr;
}

void List::Push(StoGMap value) {
	Node* p = new Node(value);
	if (Empty()) {
		first = p;
		last = p;
	}
	else {
		last->next = p;
		p->prev = last;
		last = p;
	}
	_size++;
}

StoGMap List::Get(int index) {
	Node* p;
	if (index < (_size / 2)) {
		p = first;
		for (int i = 0; i < index; i++) p = p->next;
	}
	else {
		p = last;
		for (int i = 0; i < (_size - index - 1); i++) p = p->prev;
	}
	return p->info;
}

int List::Size() {
	return _size;
}

void List::Delete(int index) {
	Node* p;
	if (index < (_size / 2)) {
		p = first;
		for (int i = 0; i < index; i++) p = p->next;
	}
	else {
		p = last;
		for (int i = 0; i < (_size - index - 1); i++) p = p->prev;
	}
	deleteElement(p);
}

void List::Clear() {
	int size = _size;
	for (int i = 0; i < size; i++) deleteElement(first);
}