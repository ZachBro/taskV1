#include "Task.h"

int Task::num_of_tasks = 0;

Task::Task(std::string name, time_t date)
{
    id = ++(Task::num_of_tasks);
    task_name = name;
    task_date = date;
    completed = false;
}

Task::Task(std::string name, time_t date, bool complete)
{
    id = ++(Task::num_of_tasks);
    task_name = name;
    task_date = date;
    completed = complete;
}

int Task::getId() const
{
    return id;
}

void Task::setId(int new_id)
{
    id = new_id;
}

std::string Task::getName() const
{
    return task_name;
}

void Task::setName(std::string new_name)
{
    task_name = new_name;
}

time_t Task::getDate() const
{
    return task_date;
}

void Task::setDate(time_t new_date)
{
    task_date = new_date;
}

void Task::completeTask()
{
    completed = true;
}

bool Task::isCompleted() const
{
    return completed;
}
