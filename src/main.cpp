#include <iostream>

#include "../include/Student.h"

int main() {
	StudentArray students;
	InitStudents(students);

	LoadFile("students.json", students);  // 启动时读文件

	int choice = -1;
	while (true) {
		MainMenu();
		std::cout << "Please enter your choice: ";
		if (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(1024, '\n');
			std::cout << "Invalid input, please try again.\n";
			continue;
		}

		switch (choice) {
			case 1:
				HandleAdd(students);
				break;
			case 2:
				HandleRemove(students);
				break;
			case 3:
				HandleModify(students);
				break;
			case 4:
				HandleSearch(students);
				break;
			case 5:
				HandleSort(students);
				break;
			case 6:
				HandleStat(students);
				break;
			case 7:
				ListAllStudents(students);
				break;
			case 0:
				SortStudents(students, CompareIDAsend);  // 退出的时候按id重新排序一遍再退出
				SaveToFile("students.json", students);
				FreeStudents(students);
				std::cout << "Data saved. Bye.\n";
				return 0;
			default:
				std::cout << "Invalid option, please try again.\n";
				break;
		}
	}
}