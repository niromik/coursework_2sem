#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <codecvt>
#include <string>
#include "Header.h"

using namespace std;

int studentIdentificator = 0;
int groupIdentificator = 0;
map<int, Student> students;
map<int, int> groups;
List studentsInGroups = List();

int searchGroupId(int groupNum) {
	int groupId = -1;
	for (auto elem = groups.begin(); elem != groups.end(); elem++) {
		if (elem->second == groupNum) {
			groupId = elem->first;
			break;
		}
	}
	return groupId;
}

void AddStudent(info sInfo) {
	int groupId = searchGroupId(sInfo.groupNum);
	if (groupId != -1) {
		for (int i = 0; i <= studentsInGroups.Size() / 2; i++) {
			if (studentsInGroups.Get(i).groupId == groupId && studentsInGroups.Get(i).recordBookNum == sInfo.recordBookNum) {
				throw StudentExist();
			}
			if (studentsInGroups.Get(studentsInGroups.Size() - 1 - i).groupId == groupId && studentsInGroups.Get(studentsInGroups.Size() - 1 - i).recordBookNum == sInfo.recordBookNum) {
				throw StudentExist();
			}
		}
		studentIdentificator++;
		Student student = Student(sInfo);
		students.insert(make_pair(studentIdentificator, student));
		studentsInGroups.Push(StoGMap(groupId, studentIdentificator, sInfo.recordBookNum));
	}
	else {
		groupIdentificator++;
		groups.insert(make_pair(groupIdentificator, sInfo.groupNum));
		studentIdentificator++;
		Student student = Student(sInfo);
		students.insert(make_pair(studentIdentificator, student));
		studentsInGroups.Push(StoGMap(groupIdentificator, studentIdentificator, sInfo.recordBookNum));
	}
}

void FileInput() {
	wcout << "WARNING!!!" << endl
		<< "File filling format:" << endl
		<< "1. Student's surname;" << endl
		<< "2. Student's name;" << endl
		<< "3. Student's patronymic;" << endl
		<< "4. Student's record book number;" << endl
		<< "5. Student's group number;" << endl
		<< "6. Amount of student's scholarship;" << endl
		<< "7. Student's marks (5 values);" << endl
		<< "If the wrong text is entered, the reading process will be interrupted." << endl;
	
	wstring filePath;
	wcout << "Specify absolute path of file:" << endl << "> ";
	wcin >> filePath;
	wifstream wfIn(filePath);
	while (true) {
		if (wfIn.is_open()) {
			break;
		}
		else wcout << "File cannot be opened using this path." << endl;
		wcout << "Specify new absolute path of file:" << endl << "> ";
		wcin >> filePath;
		wifstream wfIn(filePath);
	}
	
	wfIn.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>)); // установка кодировки чтения.
	
	wstring surname, name, patronymic;
	int recordBookNum, groupNum, scholarship, n;
	vector<int> marks;
	system("cls");
	wcout << "Start reading." << endl;
	while (true) {
		wfIn >> surname;
		wfIn >> name;
		wfIn >> patronymic;
		wfIn >> recordBookNum;
		wfIn >> groupNum;
		wfIn >> scholarship;
		for (int i = 0; i < 5; i++) {
			wfIn >> n;
			marks.push_back(n);
		}
		info studentInformation = info(surname, name, patronymic, recordBookNum, groupNum, scholarship, marks);
		while (true) {
			try {
				AddStudent(studentInformation);
			}
			catch (const StudentExist& ex) {
				wcout << "Student with record book number " << studentInformation.recordBookNum << " exist." << endl
					<< "Enter new student's record book number: ";
				wcin >> studentInformation.recordBookNum;
				continue;
			}
			catch (...) { throw; }
			break;
		}
		wcout << "Student with info:" << endl
			<< "full name: " << studentInformation.surname << " " << studentInformation.name << " " << studentInformation.patronymic << ";" << endl
			<< "record book number: " << studentInformation.recordBookNum << ";" << endl
			<< "group number: " << studentInformation.groupNum << ";" << endl
			<< "scholarship: " << studentInformation.scholarship << ";" << endl
			<< "marks:";
		for (int i = 0; i < 5; i++) wcout << " " << studentInformation.marks[i];
		wcout << ";" << endl << "successfully added." << endl;
		
		marks.clear();
		if (wfIn.eof()) {
			wcout << "Reading is over." << endl;
			wfIn.close();
			break;
		}
		system("pause");
	}
}

void ConsoleInput() {
	wstring surname, name, patronymic;
	int recordBookNum, groupNum, scholarship, n;
	vector<int> marks;
	system("cls");
	wcout << "Start reading." << endl;
	while (true) {
		wcout << "Student's surname:" << endl << "> ";
		wcin >> surname;
		wcout << "Student's name:" << endl << "> ";
		wcin >> name;
		wcout << "Student's patronymic:" << endl << "> ";
		wcin >> patronymic;
		wcout << "Student's record book number:" << endl << "> ";
		wcin >> recordBookNum;
		wcout << "Student's group number (4-digit number):" << endl << "> ";
		wcin >> groupNum;
		wcout << "Amount of student's scholarship:" << endl << "> ";
		wcin >> scholarship;
		wcout << "Student's marks (5 values):" << endl << "> ";
		for (int i = 0; i < 5; i++) {
			wcin >> n;
			marks.push_back(n);
		}
		info studentInformation = info(surname, name, patronymic, recordBookNum, groupNum, scholarship, marks);
		while (true) {
			try {
				AddStudent(studentInformation);
			}
			catch (const StudentExist& ex) {
				wcout << "Student with record book number " << studentInformation.recordBookNum << " exist." << endl
					<< "Enter new student's record book number: ";
				wcin >> studentInformation.recordBookNum;
				continue;
			}
			catch (...) { throw; }
			break;
		}
		wcout << "Student successfully added." << endl;
		marks.clear();
		bool answer;
		wcout << "Do you want continue adding students? (1 or 0)";
		wcin >> answer;
		if (!answer) break;
		system("pause");
	}
}

void OutputStudentInfo(wostream& out, info studentInfo) {
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring whitespace = converter.from_bytes(" "), stringOfMarks;
	stringOfMarks = whitespace;
	for (int j = 0; j < 5; j++)
		stringOfMarks += to_wstring(studentInfo.marks[j]) + whitespace;
	out << setw(18) << studentInfo.recordBookNum << " "
		<< setw(10) << studentInfo.surname << " "
		<< setw(10) << studentInfo.name << " "
		<< setw(10) << studentInfo.patronymic << " "
		<< stringOfMarks << " "
		<< setw(11) << studentInfo.scholarship << endl;
}

void Output(wostream& out) {
	if (typeid(out) == typeid(wofstream)) out.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	int choice, groupNum, recordBookNum;
	while (true) {
		system("cls");
		wcout << "Choose action:" << endl
			<< "0. Exit;" << endl
			<< "1. Output information about group;" << endl
			<< "2. Output information about student;" << endl
			<< "3. Output all information about groups and students." << endl
			<< "> ";
		wcin >> choice;
		system("cls");
		int groupId;
		switch (choice) {
		case 0:
			return;
		case 1:
			wcout << "Enter group number:" << endl << "> ";
			wcin >> groupNum;
			groupId = searchGroupId(groupNum);
			if (groupId == -1 || groups[groupId] == 0) wcout << "Group with number " << groupNum << " nonexists." << endl;
			else {
				out << "Group " << groupNum << ":" << endl;
				out << setw(18) << "record book number"
					<< " " << setw(10) << "surname"
					<< " " << setw(10) << "name"
					<< " " << setw(10) << "patronymic"
					<< " " << setw(11) << "marks"
					<< " " << setw(11) << "scholarship" << endl;
				for (int i = 0; i < studentsInGroups.Size(); i++) {
					if (studentsInGroups.Get(i).groupId == groupId) {
						int studentId = studentsInGroups.Get(i).studentId;
						Student student = students[studentId];
						OutputStudentInfo(out, student.value);
					}
				}
			}
			break;
		case 2:
			wcout << "Enter group number: ";
			wcin >> groupNum;
			groupId = searchGroupId(groupNum);
			if (groupId == -1 || groups[groupId] == 0) wcout << "Group with number " << groupNum << " nonexists." << endl;
			else {
				wcout << "Enter student's record book number: ";
				wcin >> recordBookNum;
				out << "Student with record book number " << recordBookNum << ":" << endl;
				out << setw(18) << "record book number"
					<< " " << setw(10) << "surname"
					<< " " << setw(10) << "name"
					<< " " << setw(10) << "patronymic"
					<< " " << setw(11) << "marks"
					<< " " << setw(11) << "scholarship" << endl;
				int index = -1;
				for (int i = 0; i < studentsInGroups.Size(); i++) {
					if (studentsInGroups.Get(i).groupId == groupId && studentsInGroups.Get(i).recordBookNum == recordBookNum) {
						index = i;
						break;
					}
				}
				if (index != -1) {
					int studentId = studentsInGroups.Get(index).studentId;
					Student student = students[studentId];
					OutputStudentInfo(out, student.value);
				}
				else wcout << "Student with record book number " << recordBookNum
					<< " in group " << groupNum << " nonexists." << endl;
			}
			break;
		case 3:
			for (auto elem = groups.begin(); elem != groups.end(); elem++) {
				groupId = elem->first;
				if (groups[groupId] != 0) {
					out << "Group " << groups[groupId] << ":" << endl;
					out << setw(18) << "record book number"
						<< " " << setw(10) << "surname"
						<< " " << setw(10) << "name"
						<< " " << setw(10) << "patronymic"
						<< " " << setw(11) << "marks"
						<< " " << setw(11) << "scholarship" << endl;
					for (int i = 0; i < studentsInGroups.Size(); i++) {
						if (studentsInGroups.Get(i).groupId == groupId) {
							int studentId = studentsInGroups.Get(i).studentId;
							Student student = students[studentId];
							OutputStudentInfo(out, student.value);
						}
					}
				}
			}
			break;
		default:
			wcout << "Action with number " << choice << " does not exist." << endl;
		}
		system("pause");
	}
}

void EditingStudent() {
	int groupNum, recordBookNum;
	wcout << "Enter group number:" << endl << "> ";
	wcin >> groupNum;
	int groupId = searchGroupId(groupNum);
	if (groupId == -1 || groups[groupId] == 0) wcout << "Group with number " << groupNum << " nonexists." << endl;
	else {
		wcout << "Enter student's record book number: ";
		wcin >> recordBookNum;
		int index = -1;
		for (int i = 0; i < studentsInGroups.Size(); i++) {
			if (studentsInGroups.Get(i).groupId == groupId && studentsInGroups.Get(i).recordBookNum == recordBookNum) {
				index = i;
				break;
			}
		}
		if (index != -1) {
			int choice;
			while (true) {
				system("cls");
				wcout << "Choose action:" << endl
					<< "0. Exit;" << endl
					<< "1. Edeting full name;" << endl
					<< "2. Edeting record book number;" << endl
					<< "3. Edeting scholarship amount;" << endl
					<< "4. Edeting marks." << endl
					<< "> ";
				wcin >> choice;
				system("cls");
				int studentId = studentsInGroups.Get(index).studentId, recordBookNum;
				info sInfo = students[studentId].value;
				StoGMap elem;
				switch (choice) {
				case 0:
					return;
				case 1:
					wcout << "Student's full name: "
						<< sInfo.surname << " "
						<< sInfo.name << " "
						<< sInfo.patronymic << "." << endl;
					wcout << "Enter new full name:" << endl;
					wcout << "surname: ";
					wcin >> students[studentId].value.surname;
					wcout << "name: ";
					wcin >> students[studentId].value.name;
					wcout << "patronymic: ";
					wcin >> students[studentId].value.patronymic;
					sInfo = students[studentId].value;
					wcout << "Student's new full name: "
						<< sInfo.surname << " "
						<< sInfo.name << " "
						<< sInfo.patronymic << "." << endl;
					break;
				case 2:
					wcout << "Student's record book number: " << sInfo.recordBookNum << "." << endl;
					while (true) {
						wcout << "Enter new record book number:" << endl << "> ";
						wcin >> recordBookNum;
						int index_ = -1;
						for (int i = 0; i < studentsInGroups.Size(); i++) {
							if (studentsInGroups.Get(i).groupId == groupId && studentsInGroups.Get(i).recordBookNum == recordBookNum && index != i) {
								index_ = i;
								break;
							}
						}
						if (index_ == -1) break;
						else wcout << "Student with record book number " << recordBookNum << " exist in that group." << endl;
					}
					elem = studentsInGroups.Get(index);
					elem.recordBookNum = recordBookNum;
					students[studentId].value.recordBookNum = recordBookNum;
					sInfo = students[studentId].value;
					wcout << "Student's new record book number: " << sInfo.recordBookNum << "." << endl;
					break;
				case 3:
					wcout << "Amount of student's scholarship: " << sInfo.scholarship << "." << endl;
					wcout << "Enter new scholarship amount:" << endl << "> ";
					wcin >> students[studentId].value.scholarship;
					sInfo = students[studentId].value;
					wcout << "New amount of student's scholarship: " << sInfo.scholarship << "." << endl;
					break;
				case 4:
					wcout << "Student's marks:" << endl << ">";
					for (int i = 0; i < sInfo.marks.size(); i++) wcout << " " << sInfo.marks[i];
					wcout << endl;
					wcout << "Enter new marks:" << endl << "> ";
					for (int i = 0; i < sInfo.marks.size(); i++) wcin >> students[studentId].value.marks[i];
					sInfo = students[studentId].value;
					wcout << "New student's marks:" << endl << ">";
					for (int i = 0; i < sInfo.marks.size(); i++) wcout << " " << sInfo.marks[i];
					wcout << endl;
					break;
				default:
					wcout << "Action with number " << choice << " does not exist." << endl;
				}
				system("pause");
			}
		}
		else wcout << "Student with record book number " << recordBookNum
			<< " in group " << groupNum << " nonexists." << endl;
	}
}

void EditingGroup() {
	int groupNum;
	wcout << "Enter group number:" << endl << "> ";
	wcin >> groupNum;
	int groupId = searchGroupId(groupNum);
	if (groupId == -1 || groups[groupId] == 0) wcout << "Group with number " << groupNum << " nonexists." << endl;
	else {
		int choice;
		while (true) {
			system("cls");
			wcout << "Choose action:" << endl
				<< "0. Exit;" << endl
				<< "1. Move student to another group;" << endl
				<< "2. Delete student;" << endl
				<< "3. Delete group." << endl
				<< "> ";
			wcin >> choice;
			system("cls");
			int recordBookNum, newGroupNum, index;
			bool flag;
			info studentInfo;
			switch (choice) {
			case 0:
				return;
			case 1:
				wcout << "Enter student's record book number: " << endl << "> ";
				wcin >> recordBookNum;
				index = -1;
				for (int i = 0; i < studentsInGroups.Size(); i++) {
					if (studentsInGroups.Get(i).groupId == groupId && studentsInGroups.Get(i).recordBookNum == recordBookNum) {
						index = i;
						break;
					}
				}
				if (index != -1) {
					wcout << "Enter new student's group number: " << endl << "> ";
					wcin >> newGroupNum;
					studentInfo = students[studentsInGroups.Get(index).studentId].value;
					flag = false;
					for (int i = 0; i < studentsInGroups.Size(); i++) {
						if (studentsInGroups.Get(i).groupId == studentsInGroups.Get(index).groupId && index != i) {
							flag = true;
							break;
						}
					}
					if (!flag) groups.erase(studentsInGroups.Get(index).groupId);
					students.erase(studentsInGroups.Get(index).studentId);
					studentsInGroups.Delete(index);
					studentInfo.groupNum = newGroupNum;
					while (true) {
						try {
							AddStudent(studentInfo);
						}
						catch (const StudentExist& ex) {
							wcout << "Student with record book number " << studentInfo.recordBookNum << " exist." << endl
								<< "Enter new student's record book number: ";
							wcin >> studentInfo.recordBookNum;
							continue;
						}
						catch (...) { throw; }
						break;
					}
					wcout << "Student moved." << endl;
				}
				else wcout << "Student with record book number " << recordBookNum
					<< " in group " << groupNum << " nonexists." << endl;
				break;
			case 2:
				wcout << "Enter student's record book number: " << endl << "> ";
				wcin >> recordBookNum;
				index = -1;
				for (int i = 0; i < studentsInGroups.Size(); i++) {
					if (studentsInGroups.Get(i).groupId == groupId && studentsInGroups.Get(i).recordBookNum == recordBookNum) {
						index = i;
						break;
					}
				}
				if (index != -1) {
					flag = false;
					for (int i = 0; i < studentsInGroups.Size(); i++) {
						if (studentsInGroups.Get(i).groupId == studentsInGroups.Get(index).groupId && index != i) {
							flag = true;
							break;
						}
					}
					if (!flag) groups.erase(studentsInGroups.Get(index).groupId);
					students.erase(studentsInGroups.Get(index).studentId);
					studentsInGroups.Delete(index);
					wcout << "Student deleted." << endl;
				}
				else wcout << "Student with record book number " << recordBookNum
					<< " in group " << groupNum << " nonexists." << endl;
				break;
			case 3:
				for (int i = 0; i < studentsInGroups.Size(); i++) {
					if (studentsInGroups.Get(i).groupId == groupId) {
						students.erase(studentsInGroups.Get(i).studentId);
						studentsInGroups.Delete(i);
					}
				}
				groups.erase(groupId);
				wcout << "Group deleted." << endl;
				break;
			default:
				wcout << "Action with number " << choice << " does not exist." << endl;
			}
			system("pause");
		}
	}
}

void Task20() {
	map<int, int> mp;
	for (int i = 0; i < studentsInGroups.Size(); i++) {
		int studentId = studentsInGroups.Get(i).studentId,
			groupId = studentsInGroups.Get(i).groupId,
			count = 0,
			counter = 0;
		if (groups[groupId] != 0) {
			bool flag = false;
			vector<int> marks = students[studentId].value.marks;
			for (int i = 0; i < marks.size(); i++) {
				if (marks[i] <= 2) {
					flag = true;
					counter = 0;
					break;
				}
				if (marks[i] == 5) count++;
			}
			if (!flag && count >= 3) counter++;
			if (mp.count(groups[groupId]) == 0) mp.insert(make_pair(groups[groupId], counter));
			else mp[groups[groupId]] += counter;
			marks.clear();
		}
	}
	wcout << setw(5) << "Group" << " " << setw(8) << "Count of students" << endl;
	for (auto elem = mp.begin(); elem != mp.end(); elem++)
		wcout << setw(5) << elem->first << " " << setw(8) << elem->second << endl;
}

wofstream definingOutputFilePath() {
	wstring filePath;
	wcout << "Specify absolute path of file:" << endl << "> ";
	wcin >> filePath;
	wofstream wfOut(filePath);
	while (true) {
		if (wfOut.is_open()) {
			break;
		}
		else wcout << "File cannot be opened using this path." << endl;
		wcout << "Specify new absolute path of file:" << endl << "> ";
		wcin >> filePath;
		wofstream wfOut(filePath);
	}
	return wfOut;
}

void Menu() {
	int choice;
	while (true) {
		system("cls");
		wcout << "Choose action:" << endl
			<< "0. Exit;" << endl
			<< "1. Input from file;" << endl
			<< "2. Output in file;" << endl
			<< "3. Input from console;" << endl
			<< "4. Output in console;" << endl
			<< "5. Editing info about student;" << endl
			<< "6. Editing group;" << endl
			<< "7. Performe task 20." << endl
			<< "> ";
		wcin >> choice;
		system("cls");
		wofstream wfOut;
		switch (choice) {
		case 0:
			return;
		case 1:
			FileInput();
			break;
		case 2:
			wfOut = definingOutputFilePath();
			Output(wfOut);
			wfOut.close();
			break;
		case 3:
			ConsoleInput();
			break;
		case 4:
			Output(wcout);
			break;
		case 5:
			EditingStudent();
			break;
		case 6:
			EditingGroup();
			break;
		case 7:
			Task20();
			break;
		default:
			wcout << "Action with number " << choice << " does not exist." << endl;
		}
		system("pause");
	}
}

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
	Menu();
	studentsInGroups.Clear();
	students.clear();
	groups.clear();
	return 0;
}