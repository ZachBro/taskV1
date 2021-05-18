#ifndef TASK_H
#define TASK_H
#include <string>
#include <ctime>


class Task
{
    public:
        Task(std::string name, time_t date);
        Task(std::string name, time_t date, bool complete);

        static int num_of_tasks;

        int getId() const;
        void setId(int new_id);

        std::string getName() const;
        void setName(std::string new_name);

        time_t getDate() const;
        void setDate(time_t new_date);

        void completeTask();
        bool isCompleted() const;


    protected:

    private:
        int id;
        std::string task_name;
        time_t task_date;
        bool completed;


};

#endif // TASK_H
