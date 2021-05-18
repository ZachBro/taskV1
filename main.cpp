#include <iostream>
#include "Task.h"
#include <vector>
#include <fstream>
#include <string>
#include <ctime>

void readFromFile(std::vector<Task>& tasks, std::ifstream& inFile);
void writeToFile(Task taskToWrite, std::ofstream& outFile);
void printTasks(const std::vector<Task>& tasks, int modifier);
void printSingleTask(const Task& task, int modifier);
void getInputFromUser(std::string& str);
void trimString(std::string& str);
void stringToUpper(std::string& str);
time_t parseStringToTime(const std::string input);
std::string parseTimeToString(time_t input);

enum status
{
    all = 0,
    completed,
    todo,
};

int main()
{
    std::vector<Task> tasks;
    std::ifstream inFile("data.txt");

    readFromFile(tasks, inFile);

    inFile.close();

    bool exit = false;

    while (!exit)
    {
        std::string answer;
        std::cout << "Would you like to add, view or edit a task? (add/edit/view/exit): ";
        getInputFromUser(answer);

        if (answer == "ADD")
        {
            std::cout << "Please enter the task name: ";
            //getline()

        }
        else if (answer == "VIEW")
        {
            bool viewExit = false;
            while (!viewExit)
            {
                std::cout << "Would you like to view current, completed, all tasks or exit? ";
                getInputFromUser(answer);
                if (answer == "CURRENT")
                {
                    printTasks(tasks, todo);
                    viewExit = true;
                }
                else if (answer == "COMPLETED")
                {
                    printTasks(tasks, completed);
                    viewExit = true;
                }
                else if (answer == "ALL")
                {
                    printTasks(tasks, all);
                    viewExit = true;
                }
                else if (answer == "EXIT")
                {
                    viewExit = true;
                }
            }
        }
        else if (answer == "EDIT")
        {

        }
        else if (answer == "EXIT")
        {
            exit = true;
        }
    }

    std::ofstream outFile("data.txt");
    for (auto t : tasks)
    {
        writeToFile(t, outFile);
    }

    outFile.close();

    return 0;
}

void readFromFile(std::vector<Task>& tasks, std::ifstream& inFile)
{
    std::string tempLine;
    if (inFile.is_open())
    {
        while (getline(inFile, tempLine))
        {
            std::string tempTaskName, tempDateString, tempCreatedString;
            time_t tempTaskDate;
            auto iter = tempLine.begin();
            bool tempCompleted;
            for ( ; iter != tempLine.end() && *iter != ','; iter++)
            {
                tempTaskName += *iter;
            }
            if (iter != tempLine.end())
            {
                iter++;
            }
            for ( ; iter != tempLine.end() && *iter != ','; iter++)
            {
                tempDateString += *iter;
            }
            if (iter != tempLine.end())
            {
                iter++;
            }
            tempCompleted = ((char)*iter == '1');
            if (iter != tempLine.end())
            {
                iter++;
            }

            tempTaskDate = parseStringToTime(tempDateString);

            Task temp(tempTaskName, tempTaskDate, tempCompleted);

            tasks.push_back(temp);

        }
    }
}

void writeToFile(Task taskToWrite, std::ofstream& outFile)
{
    if (outFile.is_open())
    {
        outFile << taskToWrite.getName() << ',' << parseTimeToString(taskToWrite.getDate()) << ','
        << taskToWrite.isCompleted() << ',' << std::endl;
    }
}

void printTasks(const std::vector<Task>& tasks, int modifier)
{
    if (modifier == all)
    {
        for (auto task : tasks)
        {
            printSingleTask(task, modifier);
        }
    }
    else if (modifier == completed)
    {
        for (auto task : tasks)
        {
            if (task.isCompleted())
            {
                printSingleTask(task, modifier);  //this can be changed to a binary && || expression I think
            }
        }
    }
    else if (modifier == todo)
    {
        for (auto task : tasks)
        {
            if (!task.isCompleted())
            {
                printSingleTask(task, modifier);
            }
        }
    }
}

void printSingleTask(const Task& task, int modifier)
{
    std::cout << "\e[1mTask:\e[0m " << task.getName();
    int i = 20 - task.getName().length();
    if (i < 0)
    {
        i = 0;
    }
    for (; i > 0; i--)
    {
        std::cout << " ";
    }
    std::cout << " \e[1mDue:\e[0m " << parseTimeToString(task.getDate());
    if (modifier == 0)
    {
        std::cout << "   ";
        std::cout << " \e[1mStatus:\e[0m " << (task.isCompleted() ? "Complete" : "Not Complete");
    }
    std::cout << std::endl;
}

void getInputFromUser(std::string& str)
{
    getline(std::cin, str);
    trimString(str);
    stringToUpper(str);
}

void trimString(std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    if(first < last)
    {
        str = str.substr(first, (last-first + 1));
    }
}

void stringToUpper(std::string& str)
{
    for (char& c : str)
    {
        c = toupper(c);
    }
}

time_t parseStringToTime(const std::string input) //returns -1 if input in invalid. Requires format of "hh:mm dd/mm/yyyy" in 24 hour time
{
    time_t result = -1;
    int hour, minute, day, month, year;
    struct tm timeStruct;

    const char* cInput = input.c_str();

    sscanf(cInput, "%d:%d %d/%d/%d", &hour, &minute, &day, &month, &year);
    if (hour > 23 || minute > 59 || day > 31 || month > 12)
    {
        return result;
    }

    timeStruct.tm_hour = hour;
    timeStruct.tm_min = minute;
    timeStruct.tm_mday = day;
    timeStruct.tm_mon = month - 1;
    timeStruct.tm_year = year - 1900;
    timeStruct.tm_isdst = -1;
    timeStruct.tm_sec = 0;

    result = mktime(&timeStruct);

    return result;

}

std::string parseTimeToString(time_t input)
{
    struct tm* timeStruct = localtime(&input);
    char formattedTime[17];

    strftime(formattedTime, 17, "%H:%M %d/%m/%Y", timeStruct);

    std::string result = formattedTime;
    return result;
}