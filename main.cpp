#include <iostream>
#include "Task.h"
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

void readFromFile(std::vector<Task>& tasks, std::ifstream& inFile);
void writeToFile(Task taskToWrite, std::ofstream& outFile);
void printTasks(const std::vector<Task>& tasks, int modifier);
void printSingleTask(const Task& task, int modifier);
void getStringFromUser(std::string& input);
void getIntFromUser(int& input);
void trimString(std::string& str);
void stringToUpper(std::string& str);
time_t parseStringToTime(const std::string input);
std::string parseTimeToString(time_t input);
Task* findTaskInVector(std::vector<Task>& tasks, const int id);

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
        getStringFromUser(answer);
        stringToUpper(answer);

        if (answer == "ADD")
        {
            std::string tempTaskName;
            time_t tempTime = -1;

            std::cout << "Enter the task name: ";
            getStringFromUser(tempTaskName);

            while (tempTime == -1)
            {
                std::string tempDateString;

                std::cout << "Enter the time and date due (hh:mm dd/mm/yyyy in 24 hour time): ";
                getStringFromUser(tempDateString);

                const char* cTime = tempDateString.c_str();
                tempTime = parseStringToTime(cTime);
            }
            Task temp(tempTaskName, tempTime);
            tasks.push_back(temp);
        }
        else if (answer == "VIEW")
        {
            bool exitView = false;
            while (!exitView)
            {
                std::cout << "Would you like to view current, completed, all tasks or exit? ";
                getStringFromUser(answer);
                stringToUpper(answer);
                if (answer == "CURRENT")
                {
                    printTasks(tasks, todo);
                    exitView = true;
                }
                else if (answer == "COMPLETED")
                {
                    printTasks(tasks, completed);
                    exitView = true;
                }
                else if (answer == "ALL")
                {
                    printTasks(tasks, all);
                    exitView = true;
                }
                else if (answer == "EXIT")
                {
                    exitView = true;
                }
            }
        }
        else if (answer == "EDIT")
        {
            int idToFind = 0;


            std::cout << "Enter ID of task to find: ";
            getIntFromUser(idToFind);


            Task* taskToEdit = findTaskInVector(tasks, idToFind);
            if (taskToEdit == nullptr)
            {
                std::cout << "ID not found" << std::endl;
            }
            else if (!taskToEdit -> isCompleted())
            {
                bool exitEdit = false;

                while (!exitEdit)
                {
                    std::cout << "Would you like to mark this task as complete? (Y/n/exit) ";
                    getStringFromUser(answer);
                    stringToUpper(answer);

                    if (answer == "Y")
                    {
                        taskToEdit -> completeTask();
                        exitEdit = true;
                    }
                    else if (answer == "N")
                    {
                        bool exitInnerEdit = false;
                        std::cout << "Would you like to change the name or date, or exit? ";

                        while (!exitInnerEdit)
                        {
                            getStringFromUser(answer);
                            stringToUpper(answer);

                            if (answer == "NAME")
                            {
                                std::string newName;
                                getStringFromUser(newName);
                                taskToEdit -> setName(newName);
                                exitInnerEdit = true;
                            }
                            else if (answer == "DATE")
                            {
                                time_t newDate = -1;
                                while (newDate == -1)
                                {
                                    std::string newDateString;
                                    getStringFromUser(newDateString);
                                    newDate = parseStringToTime(newDateString);
                                }
                                taskToEdit -> setDate(newDate);
                                exitInnerEdit = true;
                            }
                            else if (answer == "EXIT")
                            {
                                exitInnerEdit = true;
                            }
                        }
                    }
                    else if (answer == "EXIT")
                    {
                        exitEdit = true;
                    }
                }

            }
            else
            {
                std::cout << "Task is already marked complete. Revert this to incomplete? (Y/n/exit) ";

            }

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
                printSingleTask(task, modifier);
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
    std::cout << "ID: " << task.getId();

    int i = 4 - (std::to_string(task.getId()).length());
    if (i < 0)
    {
        i = 0;
    }
    for (; i > 0; i--)
    {
        std::cout << " ";
    }

    std::cout << "Task: " << task.getName();

    i = 20 - task.getName().length();
    if (i < 0)
    {
        i = 0;
    }
    for (; i > 0; i--)
    {
        std::cout << " ";
    }
    std::cout << " Due: " << parseTimeToString(task.getDate());
    if (modifier == 0)
    {
        std::cout << "   ";
        std::cout << " Status: " << (task.isCompleted() ? "Complete" : "Incomplete");
    }
    std::cout << std::endl;
}

void getStringFromUser(std::string& input)
{
    getline(std::cin, input);
    trimString(input);
}

void getIntFromUser(int& input)
{
    //int test = 0;
    std::string idToFindString;
    while (true)
    {
        getline(std::cin, idToFindString);

        std::stringstream myStream(idToFindString);
        if (myStream >> input)
        {
            break;
        }
        std::cout << "Invalid number, please try again: ";
    }

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

Task* findTaskInVector(std::vector<Task>& tasks, const int id) //returns pointer to task, otherwise nullptr if id not found
{
    for (Task& task : tasks)
    {
        if (task.getId() == id)
        {
            return &task;
        }
    }

    return nullptr;
}
