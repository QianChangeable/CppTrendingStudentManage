#include "student.h"

#include <cstring>  // strstr模糊查询
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "json.hpp"

using nlohmann::json;

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

void InitStudents(StudentArray& arr) {
	// TODO: 分配 INITIAL_CAPACITY 个 Student 空间，初始化 size/capacity
	arr.size_ = 0;                     // 初始学生是0
	arr.capacity_ = INITIAL_CAPACITY;  // 容量是8
	arr.data_ = new Student[arr.capacity_];
}

void FreeStudents(StudentArray& arr) {
	// TODO: 释放堆内存，把指针和 size/capacity 设为 0
	delete[] arr.data_;   // 释放
	arr.data_ = nullptr;  // 置空好习惯
	arr.size_ = 0;
	arr.capacity_ = 0;
}

bool ExpandCapacity(StudentArray& arr, int capacity) {
	// TODO: 如果 capacity < minCapacity，就扩容（new[] 或 realloc 思路）
	if (arr.capacity_ >= capacity) {
		return true;
	}

	int new_capacity = arr.capacity_ > 0 ? arr.capacity_ * 2 : INITIAL_CAPACITY;
	if (new_capacity < capacity) {
		new_capacity = capacity;
	}

	Student* new_data = new Student[new_capacity];
	if (new_data == nullptr) {
		return false;
	}

	for (int i = 0; i < arr.size_; ++i) {
		new_data[i] = arr.data_[i];
	}

	delete[] arr.data_;
	arr.data_ = new_data;
	arr.capacity_ = new_capacity;

	return true;
}

bool AddStudent(StudentArray& arr, const Student& student) {
	// TODO: 1. 检查学号是否唯一 2. ExpandCapacity
	//       3. 拷贝到 data[size]，size++ 4. 写日志
	if (!IsUniqueID(arr, student.id_)) {
		std::cout << "Add failed: duplicated id.\n";
		return false;
	}

	if (!ExpandCapacity(arr, arr.size_ + 1)) {
		std::cout << "Add failed: not enough memory.\n";
		return false;
	}

	arr.data_[arr.size_] = student;
	++arr.size_;

	return true;
}

bool RemoveStudentByID(StudentArray& arr, const char* id) {
	// TODO: 找到下标，用覆盖/前移方式删除，size--
	for (int i = 0; i < arr.size_; ++i) {
		if (std::strcmp(arr.data_[i].id_, id) == 0) {
			for (int j = i + 1; j < arr.size_; ++j) {
				arr.data_[j - 1] = arr.data_[j];
			}
			--arr.size_;  // 记得--当前容量
			return true;
		}
	}
	return false;
}

Student* FindStudentById(const StudentArray& arr, const char* id) {
	// TODO: 线性查找
	for (int i = 0; i < arr.size_; ++i) {
		if (std::strcmp(arr.data_[i].id_, id) == 0) {
			return &arr.data_[i];
		}
	}
	return nullptr;
}

void ModifyStudentMenu(Student& student) {
	int choice = -1;
	while (true) {
		std::cout << "\n====== Modify Student Menu ======\n";
		std::cout << "1. Modify age\n";
		std::cout << "2. Modify score\n";
		std::cout << "3. Modify class\n";
		std::cout << "0. Exit\n";
		std::cout << "Please enter your choice: ";
		std::cin >> choice;

		if (choice == 1) {
			int new_age;
			std::cout << "Enter new age: ";
			std::cin >> new_age;
			if (IsValidAge(new_age)) {
				student.age_ = new_age;
			} else {
				std::cout << "Invalid age! Age must be between 10 and 50.\n";
			}
		} else if (choice == 2) {
			std::cout << "Enter new score: ";
			std::cin >> student.score_;
		} else if (choice == 3) {
			int cls;
			std::cout << "Enter new class (1:A 2:B 3:C): ";
			std::cin >> cls;
			student.class_ = static_cast<ClassType>(cls);
		} else if (choice == 0) {
			break;
		} else {
			std::cout << "Invalid option, please try again.\n";
		}
	}
}

void SearchByID(const StudentArray& arr, const char* keyword) {
	// TODO: 使用 strstr 做模糊匹配，打印命中学生
	bool found = false;
	for (int i = 0; i < arr.size_; ++i) {
		if (std::strstr(arr.data_[i].id_, keyword)) {
			PrintStudent(arr.data_[i]);
			found = true;
		}
	}

	if (!found) {
		std::cout << "No student found with ID containing \"" << keyword << "\"! \n";
		LOG("查询学生，按学号，未找到，关键字：" + std::string(keyword));
	} else {
		LOG("查询学生，按学号，关键字：" + std::string(keyword));
	}
}

void SearchByName(const StudentArray& arr, const char* keyword) {
	// TODO: 使用 strstr 做模糊匹配，打印命中学生
	bool found = false;
	for (int i = 0; i < arr.size_; ++i) {
		if (std::strstr(arr.data_[i].name_, keyword)) {
			PrintStudent(arr.data_[i]);
			found = true;
		}
	}

	if (!found) {
		std::cout << "No student found with name containing \"" << keyword << "\"! \n";
		LOG("查询学生，按姓名，未找到，关键字：" + std::string(keyword));
	} else {
		LOG("查询学生，按姓名，关键字：" + std::string(keyword));
	}
}

void SearchByAge(const StudentArray& arr, int age) {
	bool found = false;
	for (int i = 0; i < arr.size_; ++i) {
		if (arr.data_[i].age_ == age) {
			PrintStudent(arr.data_[i]);
			found = true;
		}
	}

	if (!found) {
		std::cout << "No student found with age \"" << age << "\"! \n";
		LOG("查询学生，按年龄，未找到，关键字：" + age);
	} else {
		LOG("查询学生，按年龄，关键字：" + age);
	}
}

void SearchByScore(const StudentArray& arr, float score) {
	bool found = false;
	for (int i = 0; i < arr.size_; ++i) {
		if (arr.data_[i].score_ == score) {
			PrintStudent(arr.data_[i]);
			found = true;
		}
	}

	if (!found) {
		std::cout << "No student found with score \"" << score << "\"! \n";
		LOG("查询学生，按分数，未找到，关键字：" + std::to_string(score));
	} else {
		LOG("查询学生，按分数，关键字：" + std::to_string(score));
	}
}

void SearchByClass(const StudentArray& arr, ClassType cls) {
	bool found = false;
	for (int i = 0; i < arr.size_; ++i) {
		if (arr.data_[i].class_ == cls) {
			PrintStudent(arr.data_[i]);
			found = true;
		}
	}

	if (!found) {
		std::cout << "No student found with class \"" << static_cast<int>(cls) << "\"! \n";
		LOG("查询学生，按班级，未找到，关键字：" + std::to_string(static_cast<int>(cls)));
	} else {
		LOG("查询学生，按班级，关键字：" + std::to_string(static_cast<int>(cls)));
	}
}

void SortStudents(StudentArray& arr, CompareFunc cmp) {
	int len = arr.size_;
	for (int i = 0; i < len; ++i) {
		Student temp = arr.data_[i];  // 单间
		int j;
		for (j = i - 1; j >= 0; --j) {
			// if(arr.data_[i] > temp)
			if (cmp(arr.data_[j], temp) > 0) {
				arr.data_[j + 1] = arr.data_[j];
				continue;
			}
			break;
		}
		arr.data_[j + 1] = temp;
	}
}

int CompareIDAsend(const Student& a, const Student& b) {
	return std::strcmp(a.id_, b.id_);
}

int CompareIDDescend(const Student& a, const Student& b) {
	return std::strcmp(b.id_, a.id_);
}

int CompareNameAsend(const Student& a, const Student& b) {
	return std::strcmp(a.name_, b.name_);
}

int CompareNameDescend(const Student& a, const Student& b) {
	return std::strcmp(b.name_, a.name_);
}

int CompareClassAsend(const Student& a, const Student& b) {
	return static_cast<int>(a.class_) - static_cast<int>(b.class_);
}

int CompareClassDescend(const Student& a, const Student& b) {
	return static_cast<int>(b.class_) - static_cast<int>(a.class_);
}

int CompareScoreAsend(const Student& a, const Student& b) {
	// TODO: 返回 a.score 与 b.score 的比较结果
	if (a.score_ > b.score_) {
		return 1;
	}
	if (a.score_ < b.score_) {
		return -1;
	}
	return 0;
}

int CompareScoreDescend(const Student& a, const Student& b) {
	// TODO
	if (a.score_ < b.score_) {
		return 1;
	}
	if (a.score_ > b.score_) {
		return -1;
	}
	return 0;
}

int CompareAgeAsend(const Student& a, const Student& b) {
	// TODO
	return a.age_ - b.age_;
}

int CompareAgeDescend(const Student& a, const Student& b) {
	// TODO
	return b.age_ - a.age_;
}

float AverageScore(const StudentArray& arr) {
	// TODO: 累加成绩 / size，注意 size=0 情况
	if (arr.size_ == 0) {
		return 0.0f;
	}
	float sum = 0.0f;
	for (int i = 0; i < arr.size_; ++i) {
		sum += arr.data_[i].score_;
	}
	return sum / arr.size_;
}

bool MaxMinScore(const StudentArray& arr, float& max_score, float& min_score) {
	// TODO: 遍历更新最大最小；若 size=0 返回 false
	if (arr.size_ == 0) {
		return false;
	}

	max_score = arr.data_[0].score_;
	min_score = arr.data_[0].score_;

	for (int i = 1; i < arr.size_; ++i) {
		if (arr.data_[i].score_ > max_score) {
			max_score = arr.data_[i].score_;
		}
		if (arr.data_[i].score_ < min_score) {
			min_score = arr.data_[i].score_;
		}
	}

	return true;
}

bool LoadFile(const char* filename, StudentArray& arr) {
	std::ifstream ifs(filename);
	if (!ifs) {
		return false;
	}
	json j;
	ifs >> j;
	if (!j.contains("students")) {
		return false;
	}
	arr.size_ = 0;
	for (const auto& item : j["students"]) {
		if (arr.size_ >= arr.capacity_) {
			ExpandCapacity(arr, arr.size_ + 1);
		}
		Student student;
		std::strncpy(student.id_, item.value("ID", "").c_str(), sizeof(student.id_) - 1);
		student.id_[sizeof(student.id_) - 1] = '\0';
		std::strncpy(student.name_, item.value("name", "").c_str(), sizeof(student.name_) - 1);
		student.name_[sizeof(student.name_) - 1] = '\0';
		student.age_ = item.value("age", 0);
		student.score_ = item.value("score", 0.0f);
		student.class_ = static_cast<ClassType>(item.value("class", 0));
		arr.data_[arr.size_++] = student;
	}
	return true;
}

bool SaveToFile(const char* filename, const StudentArray& arr) {
	json j;
	for (int i = 0; i < arr.size_; ++i) {
		const Student& student = arr.data_[i];
		j["students"].push_back({{"ID", student.id_},
		                         {"name", student.name_},
		                         {"age", student.age_},
		                         {"score", student.score_},
		                         {"class", student.class_}});
	}
	std::ofstream ofs(filename);
	if (!ofs) {
		return false;
	}
	ofs << j.dump(4);  // 美化缩进
	return true;
}

void LOG(const std::string& msg) {
	// TODO: 附带时间戳写到 system.log
	std::ofstream ofs("system.log", std::ios::app);
	if (!ofs) {
		return;
	}

	// 获取当前时间
	std::time_t t = std::time(nullptr);
	char buf[64];
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

	ofs << "[" << buf << "] " << msg << std::endl;
}

void PrintStudent(const Student& student) {
	std::cout << "ID: " << student.id_ << "\n";
	std::cout << "  Name: " << student.name_ << "\n";
	std::cout << "  Age: " << student.age_ << "\n";
	std::cout << "  Score: " << student.score_ << "\n";
	std::cout << "  Class: " << static_cast<int>(student.class_) << "\n";
}

void ListAllStudents(const StudentArray& arr) {
	if (arr.size_ == 0) {
		std::cout << "No students.\n";
		return;
	}

	std::cout << "Total " << arr.size_ << " students:\n";
	for (int i = 0; i < arr.size_; ++i) {
		std::cout << "---- #" << (i + 1) << " ----\n";
		PrintStudent(arr.data_[i]);
	}
}

void MainMenu() {
	std::cout << "\n====== Student Management System ======\n";
	std::cout << "1. Add student\n";
	std::cout << "2. Remove student\n";
	std::cout << "3. Modify student\n";
	std::cout << "4. Search student\n";
	std::cout << "5. Sort students\n";
	std::cout << "6. Score statistics\n";
	std::cout << "7. Show all students\n";
	std::cout << "0. Save and exit\n";
}

void HandleAdd(StudentArray& arr) {
	Student student{};
	std::cout << "Enter id: ";
	std::cin >> student.id_;
	std::cout << "Enter name: ";
	std::cin >> student.name_;
	std::cout << "Enter age: ";
	std::cin >> student.age_;
	if (!IsValidAge(student.age_)) {
		std::cout << "Invalid age! Age must be between 10 and 50.\n";
		LOG("添加学生失败，年龄非法，学号：" + std::string(student.id_));
		return;
	}
	std::cout << "Enter score: ";
	std::cin >> student.score_;
	std::cout << "Enter class (1:A 2:B 3:C): ";

	int cls;
	std::cin >> cls;
	student.class_ = static_cast<ClassType>(cls);

	if (AddStudent(arr, student)) {
		std::cout << "Add succeeded.\n";
		LOG("添加学生成功，学号：" + std::string(student.id_) + "，姓名：" + student.name_);
	} else {
		std::cout << "Add failed.\n";
		LOG("添加学生失败，学号重复，学号：" + std::string(student.id_));
	}
}

void HandleRemove(StudentArray& arr) {
	// TODO: 输入学号，调用 RemoveStudentByID
	char id[10];
	std::cout << "Please input ID: ";
	std::cin >> id;

	if (RemoveStudentByID(arr, id)) {
		std::cout << "Remove succeeded.\n";
		LOG("删除学生成功，学号：" + std::string(id));

	} else {
		std::cout << "Remove failed: ID not found.\n";
		LOG("删除学生失败，未找到学号：" + std::string(id));
	}
}

void HandleModify(StudentArray& arr) {
	// TODO: 先按学号查找，再调用 update_student
	char id[10];
	std::cout << "Please input ID: ";
	std::cin >> id;

	Student* modify_student = FindStudentById(arr, id);
	if (modify_student != nullptr) {
		ModifyStudentMenu(*modify_student);  // 把实际指向的对象当引用传进去
		std::cout << "Modify succeeded.\n";
		LOG("修改学生信息，学号：" + std::string(modify_student->id_) + "，姓名：" + modify_student->name_);

	} else {
		std::cout << "Student not found.\n";
		LOG("修改学生失败，未找到学号：" + std::string(id));
	}
}

void HandleSearch(const StudentArray& arr) {
	// TODO: 做一个小菜单：按学号 / 姓名 搜索
	int choice = -1;
	while (true) {
		std::cout << "====== Search Student Menu ======";
		std::cout << "1. Search by ID\n";
		std::cout << "2. Search by name\n";
		std::cout << "3. Search by age\n";
		std::cout << "4. Search by score\n";
		std::cout << "5. Search by class\n";
		std::cout << "0. Exit\n";
		std::cout << "Please enter your choice: ";
		std::cin >> choice;

		if (choice == 1) {
			std::cout << "Enter ID: ";
			char id[10];
			std::cin >> id;
			SearchByID(arr, id);
		} else if (choice == 2) {
			std::cout << "Enter Name: ";
			char name[MAX_NAME_LEN];
			std::cin >> name;
			SearchByName(arr, name);
		} else if (choice == 3) {
			std::cout << "Enter age: ";
			int age;
			std::cin >> age;
			SearchByAge(arr, age);
		} else if (choice == 4) {
			std::cout << "Enter Score: ";
			float score;
			std::cin >> score;
			SearchByAge(arr, score);
		} else if (choice == 5) {
			std::cout << "Enter Class: ";
			int cls;
			std::cin >> cls;
			SearchByAge(arr, static_cast<ClassType>(cls));
		} else if (choice == 0) {
			break;
		} else {
			std::cout << "Invalid option, please try again.\n";
		}
	}
}

void HandleSort(StudentArray& arr) {
	int field = 0, order = 0;
	std::cout << "\n====== Sort Students Menu ======\n";
	std::cout << "1. Sort by score\n";
	std::cout << "2. Sort by age\n";
	std::cout << "3. Sort by ID\n";
	std::cout << "4. Sort by name\n";
	std::cout << "5. Sort by class\n";
	std::cout << "Please select field to sort by: ";
	std::cin >> field;

	std::cout << "1. Ascending\n";
	std::cout << "2. Descending\n";
	std::cout << "Please select order: ";
	std::cin >> order;

	CompareFunc cmp = nullptr;
	if (field == 1 && order == 1)
		cmp = CompareScoreAsend;
	else if (field == 1 && order == 2)
		cmp = CompareScoreDescend;
	else if (field == 2 && order == 1)
		cmp = CompareAgeAsend;
	else if (field == 2 && order == 2)
		cmp = CompareAgeDescend;
	else if (field == 3 && order == 1)
		cmp = CompareIDAsend;
	else if (field == 3 && order == 2)
		cmp = CompareIDDescend;
	else if (field == 4 && order == 1)
		cmp = CompareNameAsend;
	else if (field == 4 && order == 2)
		cmp = CompareNameDescend;
	else if (field == 5 && order == 1)
		cmp = CompareClassAsend;
	else if (field == 5 && order == 2)
		cmp = CompareClassDescend;
	else {
		std::cout << "Invalid option.\n";
		return;
	}

	SortStudents(arr, cmp);
	ListAllStudents(arr);
	std::cout << "Sort finished.\n";
}

void HandleStat(StudentArray& arr) {
	// TODO: 调用 AverageScore / calc_max_min_score，打印结果
	float avg = AverageScore(arr);
	float max_score, min_score;
	if (MaxMinScore(arr, max_score, min_score)) {
		std::cout << "Average score: " << avg << "\n";
		std::cout << "Max score: " << max_score << ", Min score: " << min_score << "\n";
	} else {
		std::cout << "No students.\n";
	}
}

bool IsValidAge(int age) {
	if (age >= 10 && age <= 50) {
		return true;
	}
	return false;
}

bool IsUniqueID(const StudentArray& arr, const char* id) {
	// TODO: 遍历检查是否已存在相同学号
	for (int i = 0; i < arr.size_; ++i) {
		if (std::strcmp(arr.data_[i].id_, id) == 0) {
			return false;
		}
	};
	return true;
}