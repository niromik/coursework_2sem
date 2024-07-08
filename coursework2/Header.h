#pragma once
#include <vector>
#include <fstream>

using namespace std;

struct info {
	wstring surname, name, patronymic;
	int recordBookNum{ -1 }, groupNum{ -1 }, scholarship{ -1 };
	vector<int> marks;
	info();
	info(wstring surname_, wstring name_, wstring patronymic_,
		int recordBookNum_, int groupNum_, int scholarship_, vector<int> marks_);
};

struct Student {
	info value;
	Student();
	Student(info value_);
};

struct StoGMap {
	int groupId{ -1 }, studentId{ -1 }, recordBookNum{ -1 };
	StoGMap();
	StoGMap(int groupId_, int studentId_, int recordBookNum_);
};

class List {
private:
	struct Node {
		StoGMap info;
		Node* next;
		Node* prev;
		Node(StoGMap value);
	};
	int _size = 0;
	Node* first;
	Node* last;

	void deleteElement(Node* element);
public:
	List();

	bool Empty();

	void Push(StoGMap value);

	StoGMap Get(int index);

	int Size();

	void Delete(int index);

	void Clear();
};

struct StudentExist {};