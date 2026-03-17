#pragma once

#include <string>

constexpr int MAX_NAME_LEN = 32;
constexpr int INITIAL_CAPACITY = 8;

enum ClassType { CLASS_A = 1, CLASS_B, CLASS_C };

class Student {
   public:
	char id_[10];              // 学号（唯一）
	char name_[MAX_NAME_LEN];  // 姓名
	int age_;                  // 年龄
	float score_;              // 成绩
	ClassType class_;          // 班级
};

struct StudentArray {
	Student* data_ = nullptr;
	int size_ = 0;      // 当前元素个数
	int capacity_ = 0;  // 当前容量
};

// 排序比较函数类型
typedef int (*CompareFunc)(const Student&, const Student&);

// 动态数组管理
void InitStudents(StudentArray& arr);
void FreeStudents(StudentArray& arr);
bool ExpandCapacity(StudentArray& arr, int minCapacity);

bool AddStudent(StudentArray& arr, const Student& s);
bool RemoveStudentByID(StudentArray& arr, const char* id);
Student* FindStudentById(const StudentArray& arr, const char* id);

// 排序与查询
void SortStudents(StudentArray& arr, CompareFunc cmp);

int CompareScoreAsend(const Student& a, const Student& b);
int CompareScoreDescend(const Student& a, const Student& b);
int CompareAgeAsend(const Student& a, const Student& b);
int CompareAgeDescend(const Student& a, const Student& b);
int CompareIDAsend(const Student& a, const Student& b);
int CompareIDDescend(const Student& a, const Student& b);
int CompareNameAsend(const Student& a, const Student& b);
int CompareNameDescend(const Student& a, const Student& b);
int CompareClassAsend(const Student& a, const Student& b);
int CompareClassDescend(const Student& a, const Student& b);

void SearchByID(const StudentArray& arr, const char* keyword);
void SearchByName(const StudentArray& arr, const char* keyword);
void SearchByAge(const StudentArray& arr, int age);
void SearchByScore(const StudentArray& arr, float score);
void SearchByClass(const StudentArray& arr, ClassType cls);

// 统计功能
float AverageScore(const StudentArray& arr);
bool MaxMinScore(const StudentArray& arr, float& maxScore, float& minScore);
void StatAll(const StudentArray& arr);
void StatByClass(const StudentArray& arr, ClassType cls);
void HandleStat(StudentArray& arr);

// 文件与日志
bool LoadFile(const char* filename, StudentArray& arr);
bool SaveToFile(const char* filename, const StudentArray& arr);

void LOG(const std::string& msg);

// 打印与菜单
void PrintStudent(const Student& s);
void ListAllStudents(const StudentArray& arr);

void MainMenu();
void HandleAdd(StudentArray& arr);
void HandleRemove(StudentArray& arr);
void HandleModify(StudentArray& arr);
void HandleSearch(const StudentArray& arr);
void HandleSort(StudentArray& arr);
void HandleStat(StudentArray& arr);

bool IsValidAge(int age);
bool IsUniqueID(const StudentArray& arr, const char* id);
