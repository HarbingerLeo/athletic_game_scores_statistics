//Copyright(c) 2019 180511306 / 180511317 , CUST. All rights reserved.
//Founded & Debuged on Visual Studio 2019
//
//运动会分数统计
//
//设计内容 :
//参加运动会有 n 个学校, 学校编号为 1……n.比赛分成m个男子项目, 和w个女子项目.
//项目编号为男子 1……m, 女子 m + 1……m + w.
//不同的项目取前五名或前三名积分 : 取前五名的积分别为 : 7, 5, 3, 2, 1,
//前三名的积分分别为 : 5, 3, 2
//哪些取前五名或前三名由学生自己设定.（m <= 20, n <= 20）
//设计要求 :
//输入各个项目的前三名或前五名的成绩, 能统计各学校总分;
//(1) 按学校编号或名称, 学校总分, 男女团体总分排序输出;
//(2) 按学校编号查询学校某个项目的情况, 按项目编号查询取得前三或前五名的学校;
//(3) 数据存入文件并能随时查询;
//(4) 规定; 输入数据形式和范围; 可以输入学校的名称, 运动项目的名称;
//(5) 输出形式; 有合理的提示, 各学校分数为整形;
//(6) 界面要求; 有合理的提示, 每个功能可以设立菜单, 根据提示, 完成相关的功能;
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
constexpr int MAX_MEN_EVENTS = 20;
constexpr int MAX_SCHOOL = 20;
using namespace std;
struct Event    //运动项目结构体
{
	int number;     //项目编号
	string name;    //项目名称
	bool is_top3;   //是否取前三名
	bool is_male;   //是否是男子
	int rank[6];    //排名
	Event() :number(0), name("Undefined"), is_top3(true), is_male(true), rank() {}
};
struct School   //学校结构体
{
	int number;     //学校编号
	string name;    //学校名字
	int total_score;    //总分
	int men_total_score;//男团总分
	int women_total_score;  //女团总分
	School() :number(0), name("Undefined"), total_score(0), men_total_score(0), women_total_score(0) {}
};
vector<Event*> events;     //为运动项目创建内存，存放的是指向各项目的指针
vector<School*> schools;     //为学校列表创建内存,存放的是指向各学校的指针
int men_events_amount = 0;  //男子项目数量（<=20）
void LoadFile()     //自文件读取入内存
{
	fstream fs;
	fs.open("event.txt", ios::in);//读入event
	fs >> men_events_amount;
	while (!fs.eof())	//若没到文件尾则继续读入
	{
		Event* pe = new Event;
		fs >> pe->number;
		fs >> pe->name;
		fs >> pe->is_top3;
		fs >> pe->is_male;
		for (int counter = 1; counter <= 3; counter++)
		{
			fs >> pe->rank[counter];
		}
		if (!pe->is_top3)
		{
			for (int counter = 4; counter <= 5; counter++)
			{
				fs >> pe->rank[counter];
			}
		}
		events.push_back(pe);
	}
	events.pop_back();  //项目文件需要多读入一轮而后弹出最后一个无用元素，可以优化
	fs.close();
	fs.open("school.txt", ios::in); //读入school
	while (!fs.eof())
	{
		School* ps = new School;
		fs >> ps->number;
		fs >> ps->name;
		schools.push_back(ps);
	}
	fs.close();
}
void InputEvent()      //输入项目并同步保存至文件
{
	system("cls");
	while (1)
	{
		std::cout << "输入男子项目个数：";
		std::cin >> men_events_amount;
		if (men_events_amount <= MAX_MEN_EVENTS)break;
		else std::cout << "项目过多.\n";
	}
	fstream fs;
	fs.open("event.txt", ios::out);
	fs << men_events_amount << endl;
	for (int i = 1;; i++)
	{
		Event* pe = new Event;
		pe->number = i;
		fs << pe->number << " ";
		std::cout << "输入项目" << i << (i <= men_events_amount ? "(男子)" : "(女子)") << "名称：";
		std::cin >> pe->name;
		fs << pe->name << " ";
		std::cout << "若该项目取前五名请输入0，输入其他数字则取前三名：";
		std::cin >> pe->is_top3;
		fs << pe->is_top3 << " ";
		pe->is_male = (i <= men_events_amount ? true : false);
		fs << pe->is_male << " ";
		for (int counter = 1; counter <= 3; counter++)
		{
			std::cout << "输入第" << counter << "名所属学校编号：";
			std::cin >> pe->rank[counter];
			fs << pe->rank[counter] << " ";
		}
		if (!pe->is_top3)
		{
			for (int counter = 4; counter <= 5; counter++)
			{
				std::cout << "输入第" << counter << "名所属学校编号：";
				std::cin >> pe->rank[counter];
				fs << pe->rank[counter] << " ";
			}
		}
		fs << endl;
		events.push_back(pe);
		bool continuer = true;
		std::cout << "结束输入请输入0，输入其他数字则继续：";
		std::cin >> continuer;
		if (!continuer) break;
	}
	fs.close();

}
void InputSchool()      //输入学校并同步保存至文件
{
	system("cls");
	fstream fs;
	fs.open("school.txt", ios::out);
	for (int i = 1; i <= MAX_SCHOOL; i++)
	{
		School* ps = new School;
		ps->number = i;
		fs << ps->number << " ";
		std::cout << "输入学校" << i << "校名：";
		std::cin >> ps->name;
		fs << ps->name << endl;
		schools.push_back(ps);
		bool continuer = true;
		std::cout << "结束输入请输入0，输入其他数字则继续：";
		std::cin >> continuer;
		if (!continuer) break;
	}
	fs.close();
}
void InputSelect()  //输入功能的内部分支
{
	system("cls");
	std::cout << "\n\t1.输入学校信息\n"
		<< "\t2.输入赛事信息\n"
		<< "\t0.返回\n"
		<< "\n请输入对应数字选择功能：";
	int choose = 0;
	while (choose != 1 && choose != 2)
	{
		std::cin >> choose;
		switch (choose)
		{
		case 2:
			InputEvent();
			break;
		case 1:
			InputSchool();
			break;
		case 0:
			return;
		default:
			std::cout << "输出有误，请重新输入：";
		}
	}
}
void ScoreCount()   //分数统计
{
	int score3[4] = { 0,5,3,2 };
	int score5[6] = { 0,7,5,3,2,1 };
	for (int i = 0; i < events.size(); i++)  //i为项目数
	{
		if (events[i]->is_top3)
		{
			for (int j = 1; j <= 3; j++)    //j为名次
			{
				int award = events[i]->rank[j];
				for (int k = 0; k < schools.size(); k++)
				{
					if (schools[k]->number == award)
					{
						events[i]->is_male ? schools[k]->men_total_score += score3[j] : schools[k]->women_total_score += score3[j];
						schools[k]->total_score += score3[j];
						break;
					}
				}
			}
		}
		else {
			for (int j = 1; j <= 5; j++)
			{
				int award = events[i]->rank[j];
				for (int k = 0; k < schools.size(); k++)
				{
					if (schools[k]->number == award)
					{
						events[i]->is_male ? schools[k]->men_total_score += score5[j] : schools[k]->women_total_score += score5[j];
						schools[k]->total_score += score5[j];
						break;
					}
				}
			}
		}
	}
}
bool CompareSchool(const School* s1, const School* s2) { return s1->number < s2->number; }  //学校编号升序规则
bool CompareTotal(const School* s1, const School* s2) { return s1->total_score > s2->total_score; }  //总分降序规则
bool CompareMan(const School* s1, const School* s2) { return s1->men_total_score > s2->men_total_score; }  //男团降序规则
bool CompareWoman(const School* s1, const School* s2) { return s1->women_total_score > s2->women_total_score; }  //女团降序规则
void AllSchoolOutput()      //遍历输出
{
	for (int i = 0; i < schools.size(); i++)
	{
		cout << "学校编号：" << schools[i]->number
			<< " 学校名称：" << schools[i]->name
			<< " 学校总分：" << schools[i]->total_score
			<< " 男团总分：" << schools[i]->men_total_score
			<< " 女团总分：" << schools[i]->women_total_score
			<< endl;
	}
}
void SortOutput()       //根据不同需求排序
{
	system("cls");
	cout << "\n\t1.按学校编号排序\n"
		<< "\t2.按学校总分排序\n"
		<< "\t3.按男团总分排序\n"
		<< "\t4.按女团总分排序\n"
		<< "\t0.返回\n"
		<< "\n请输入对应数字选择功能：";
	int choose = 0;
	while (choose != 1 && choose != 2 && choose != 3 && choose != 4)
	{
		cin >> choose;
		switch (choose)
		{
		case 1:
			system("cls");
			sort(schools.begin(), schools.end(), CompareSchool);
			AllSchoolOutput();
			system("pause");
			break;
		case 2:
			system("cls");
			sort(schools.begin(), schools.end(), CompareTotal);
			AllSchoolOutput();
			system("pause");
			break;
		case 3:
			system("cls");
			sort(schools.begin(), schools.end(), CompareMan);
			AllSchoolOutput();
			system("pause");
			break;
		case 4:
			system("cls");
			sort(schools.begin(), schools.end(), CompareWoman);
			AllSchoolOutput();
			system("pause");
			break;
		case 0:return;
		default:
			cout << "输出有误，请重新输入：";
		}
	}
}
void Search()   //按学校编号查询学校某个项目的情况, 按项目编号查询取得前三或前五名的学校（编号）
{
	system("cls");
	cout << "\n\t1.按学校编号查询\n"
		<< "\t2.按项目编号查询\n"
		<< "\t0.返回\n"
		<< "\n请输入对应数字选择功能：";
	int choose = 0;
	while (choose != 1 && choose != 2)
	{
		std::cin >> choose;
		switch (choose)
		{
		case 1:
			system("cls");
			cout << "请输入学校编号：";
			int target;
			cin >> target;
			if (target <= schools.size())
			{
				for (int j = 0; j < schools.size(); j++)
				{
					if (schools[j]->number == target)
					{
						cout << "\n\t学校编号：" << schools[j]->number
							<< "\n\t学校名称：" << schools[j]->name
							<< "\n\t学校总分：" << schools[j]->total_score
							<< "\n\t男团总分：" << schools[j]->men_total_score
							<< "\n\t女团总分：" << schools[j]->women_total_score
							<< endl << endl;
						system("pause");
						return;
					}
				}
			}
			cout << "查无此校.\n";
			system("pause");
			return;
			break;
		case 2:
			system("cls");
			cout << "请输入项目编号：";
			cin >> target;
			if (target <= events.size())
			{
				for (int j = 0; j < events.size(); j++)
				{
					if (events[j]->number == target)
					{
						cout << "\n\t项目编号：" << events[j]->number
							<< "\n\t项目名称：" << events[j]->name
							<< "\n\t性别：" << (events[j]->is_male ? "男子" : "女子")
							<< "\n\t获奖名次：" << (events[j]->is_top3 ? 3 : 5)
							<< "\n\t获奖学校编号（名次由高到低）： ";
						for (int k = 1; k <= 5; k++)
						{
							if (events[j]->rank[k] == 0)break;
							else cout << events[j]->rank[k] << " ";
						}
						cout << endl << endl;
						system("pause");
						return;
					}
				}
			}
			cout << "查无此项目.\n";
			system("pause");
			return;
			break;
		case 0:return;
		default:
			std::cout << "输出有误，请重新输入：";
		}
	}
}
void Menu()
{
	system("cls");
	std::cout << "\n\t\t运动会分数统计系统\n\n"
		<< "\t1.信息录入\n"
		<< "\t2.统计分数\n"
		<< "\t3.排名信息\n"
		<< "\t4.成绩查询\n"
		<< "\t0.退出系统\n"
		<< "\n请输入对应数字选择功能：";
	int choose = 0;
	while (choose != 1 && choose != 2 && choose != 3 && choose != 4)
	{
		std::cin >> choose;
		switch (choose)
		{
		case 1:
			InputSelect();
			break;
		case 2:
			ScoreCount();
			system("cls");
			cout << "已完成统计." << endl;
			system("pause");
			break;
		case 3:
			SortOutput();
			break;
		case 4:
			Search();
			break;
		case 0:
			exit(0);
		default:
			std::cout << "输出有误，请重新输入：";
		}
	}
}
int main()
{
	LoadFile();
	while (1) Menu();
	return 0;
}