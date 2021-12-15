//TASKS CLI Application challenge

// #include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <vector>
#include <ctime>
using namespace std;

// Prints the CLI usage.
void help()
{
    cout << "Usage :-\n$ ./task add 2 hello world    # Add a new item with priority 2 and text \"hello world\" to the list\n$ ./task ls                   # Show incomplete priority list items sorted by priority in ascending order\n$ ./task del INDEX            # Delete the incomplete item with the given index\n$ ./task done INDEX           # Mark the incomplete item with the given index as complete\n$ ./task help                 # Show usage\n$ ./task report               # Statistics";
}


//See all the tasks that are not yet complete
void listtasks()
{
    ifstream fin("task.txt");
    if (fin)
    {
        //Check if there are no tasks
        string temp;
        if (!getline(fin, temp))
        {
            cout << "There are no pending tasks!";
        }
        else
        {
            //Prints tasks in order of priority ( increasing )
            fin.clear();
            fin.seekg(0, ios::beg);
            int num = 1;
            // printLines(fin, num);

            vector<string> values;
            string curline;
            while(getline(fin,curline)){
                values.push_back(curline);
            }

            sort(values.begin(),values.end(),[](string a,string b){
                return a[0] <= b[0];
            });

            for(int i=0;i<values.size();i++){
                cout << i+1 << ". " << values[i].substr(2) << " [" << values[i][0] << "]" << endl; 
            }
        }
        fin.close();
    }
    else
    { //If 'task.txt' file doesn't exists
        cout << "There are no pending tasks!";
    }
}

// Adds task to 'task.txt'
void add(string todopriority, string NewTodo)
{
    ofstream fout("task.txt", ios::app);
    if (fout)
    {
        if (fout << todopriority << " " <<   NewTodo  << "\n")
            cout << "Added task: \"" << NewTodo << "\" with priority " << todopriority;
    }
    fout.close();
}

// Deletes task based upon number.
void del(int todoNum, bool checker)
{
    ofstream fout("temp.txt");
    ifstream fin("task.txt");
    if (fin && fout)
    {
        int i = 1;
        string line;
        bool flag = false;

        while (getline(fin, line))
        {
            if (i == todoNum)
            {
                flag = true;
            }
            else
            {
                fout << line << endl;
            }
            i++;
        }

        fin.close();
        fout.close();
        remove("task.txt");
        rename("temp.txt", "task.txt");

        if (flag && checker)
            cout << "Deleted task #" << todoNum;
        else
        {
            cout << "Error: task with index #" << todoNum << " does not exist. Nothing deleted.";
        }
    }
}

// Marking task as done (by adding task from 'task.txt' to 'completed.txt' and deleting the task)
void markDone(int doneNum)
{
    ofstream fout("completed.txt", ios::app);
    ifstream fin("task.txt");
    if (fin && fout)
    {
        int i = 1;
        string line;
        bool flag = false;

        while (getline(fin, line))
        {
            if (i == doneNum)
            {

                fout << line << endl;
                flag = true;
            }
            i++;
        }

        fin.close();
        fout.close();

        if (flag){
            cout << "Marked item as done.";
            del(doneNum,false);
        }else
        {
            cout << "Error: no incomplete item with index #" << doneNum << " exists.";
        }

    }
}

// Generating report
void GenReport()
{
    ifstream finTodo("task.txt");
    ifstream finDone("completed.txt");
    int pend = 0, comp = 0;
    string tempLine;

    vector<string> pendingtasks, completedtasks;

    if (finTodo)
    {
        while (getline(finTodo, tempLine))
        {
            pendingtasks.push_back(tempLine);
            pend++;
        }
    }
    if (finDone)
    {
        while (getline(finDone, tempLine))
        {
            completedtasks.push_back(tempLine);
            comp++;
        }
    }
    

    cout << "Pending : " << pend << "\n";

    if(pend > 0){
        sort(pendingtasks.begin(),pendingtasks.end(),[](string a,string b){
            return a[0] <= b[0];
        });
        
        for(int i=0;i<pendingtasks.size();i++){
            cout << i+1 << ". " << pendingtasks[i].substr(2) << " [" << pendingtasks[i][0] << "]" << "\n";
        }
    }

    cout << "\nCompleted : " << comp << "\n";

    if(comp > 0){
        for(int i=0;i<completedtasks.size();i++){
            cout << i+1 << ". " << completedtasks[i].substr(2) << "\n";
        }
    }
}


// strvec starts counting after ./task, i.e, ./task add 10 "task1" is strven[0] = add and argc = 4
int main(int argc, char *argv[])
{
    //Get the arguments into vector of string
    vector<string> strvec(argv + 1, argv + argc);

    if (argc == 1 || strvec[0] == "help")
    {
        help();
    }
    else if (strvec[0] == "ls")
    {
        listtasks();
    }

    else if (strvec[0] == "add")
    {
        if (argc == 2 || argc == 3)
            cout << "Error: Missing tasks string. Nothing added!";
        else
            add(strvec[1], strvec[2]);
    }
    else if (strvec[0] == "del")
    {
        if (argc == 2)
            cout << "Error: Missing NUMBER for deleting tasks.";
        else
            del(stoi(strvec[1]), true);
    }
    else if (strvec[0] == "done")
    {
        if (argc == 2)
            cout << "Error: Missing NUMBER for marking tasks as done.";
        else
            markDone(stoi(strvec[1]));
    }
    else if (strvec[0] == "report")
    {
        GenReport();
    }

    return 0;
}
