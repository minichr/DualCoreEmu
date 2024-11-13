#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cstdio> //printf

#define DEBUG 0
#define REMOVED 0
#define EXTRA 0
/*
there seems to be extra line in input files
code works fine for the small inputs with that extra
but bigger input needs that extra line removed
so if bigger inputs please set mode to extra
but i've set it to 0 since my edits have seem to
make this no longer a problem
*/

class Task{
    private:
        std::string taskProperties;
    //properties
        #if REMOVED
        std::string name;
        #endif
        bool priority;
        int runtime;

    public:
    //constructors
        Task();
        Task(std::string task);
        ~Task();
    //getters
        #if REMOVED
        std::string getName();
        #endif
        bool getPriority();
        int getRuntime();
        void printTask(); //
    //modifiers
        void execute(int upTime); 

    friend class CpuQueue;
};

Task::Task(): priority(0), runtime(0) {
    //default const
    //name("void_task"), 
}

Task::Task(std::string task){
    this->taskProperties = task;

    int sPos = taskProperties.find_last_of(' ');
    #if REMOVED
    this->name = taskProperties.substr(0,sPos - 2);
    #endif
    this->priority = std::stoi( taskProperties.substr(sPos-1, 1) );
    this->runtime = std::stoi( taskProperties.substr(sPos+1) );

}

Task::~Task(){
}

#if REMOVED
std::string Task::getName(){
    return this->name;
}
#endif

bool Task::getPriority(){
    return this->priority;
}

int Task::getRuntime(){
    return this->runtime;
}

void Task::execute(int upTime){
    this->runtime -= upTime;
}

void Task::printTask(){
    std::cout << taskProperties << '\n';
}

class CpuQueue{
    private:
        std::queue<Task> expedited;
        std::queue<Task> regular;
        int exNum;
        int regNum;

        int estRuntime;

    public:
        CpuQueue();
        ~CpuQueue();
    //getters
        int getexNum();
        int getregNum();
        int gettaskTotal();
        int getestRuntime();
    //fcns
        void push_back(Task& cpuTask);
        Task load_front();
        void eject_front();
    //special fcn
        void load_all();
        void execute_front(int upTime);

    //debugging purposes
        void printQState();

};

CpuQueue::CpuQueue():exNum(0),regNum(0), estRuntime(0) {
    
}

CpuQueue::~CpuQueue(){
}

int CpuQueue::getregNum(){
    return this->regNum;
}

int CpuQueue::getexNum(){
    return this->exNum;
}

int CpuQueue::gettaskTotal(){
    return (this->exNum + this->regNum);
}

int CpuQueue::getestRuntime(){
    return this->estRuntime;
}

void CpuQueue::push_back(Task& cpuTask){
    if(cpuTask.priority){ //==1
        this->expedited.emplace(cpuTask);
        estRuntime += cpuTask.runtime;
        ++exNum;
        return;
    }
    else{
        this->regular.emplace(cpuTask);
        estRuntime += cpuTask.runtime;
        ++regNum;
        return;
    }
}

Task CpuQueue::load_front(){
    if(exNum > 0){
        return expedited.front();
    }
    else{
        return regular.front();
    }
}

void CpuQueue::eject_front(){
    if(exNum > 0){
        this->estRuntime -= expedited.front().runtime;
        expedited.pop();
        --exNum;
        return;
    }
    else if(exNum == 0 && regNum > 0){
        this->estRuntime -= regular.front().runtime;
        regular.pop();
        --regNum;
        return;
    }
    else{//exNum == 0 and regNum == 0
        return;
    }
}

void CpuQueue::load_all(){
    while(this->gettaskTotal()>0){
        this->eject_front();
    }
    return;
}

void CpuQueue::execute_front(int upTime){
    if(exNum > 0){
        expedited.front().execute(upTime);
    }
    else if(exNum == 0 && regNum > 0){
        regular.front().execute(upTime);
    }
    else{//exNum == 0 and regNum == 0
        return;
    }
    this->estRuntime -= upTime;
}

void CpuQueue::printQState(){
    std::cout << "#Tasks "<< this->exNum << '-' << this->regNum << '\n';
    std::cout << "EST Runtime: " << this->estRuntime << '\n';
}

int main(void){
    //open file process content
    std::ifstream fh("randcase1.txt"); 
    //IF USING BIG INPUT PLEASE SET #define EXTRA to 1
    //^^ if problem on input read

    std::string cmtLine; 
    std::getline(fh, cmtLine);
    int sPos1 = cmtLine.find_first_of(' ');
    int T = std::stoi(cmtLine.substr(0, sPos1));
    int sPos2 = cmtLine.find_last_of(' ');
    int C = std::stoi(cmtLine.substr(sPos1 + 1, sPos2 - (sPos1 + 1)));
    bool M = std::stoi(cmtLine.substr(sPos2+1, 1));

    int cnt = 0;
    int outTime = 0; //running total
    std::string taskLine; //temp frequently overwritten
    CpuQueue CPU1 = CpuQueue();
    CpuQueue CPU2 = CpuQueue();
    //Time Balancing
    if(M == 1){
        bool normal = true;
        bool did = true;
        while(cnt < T || CPU1.gettaskTotal() > 0 || CPU1.gettaskTotal() > 0){
            //filling up CPUs
            while(CPU1.gettaskTotal() <= C && CPU2.gettaskTotal() <= C && cnt < T) {
                //while there are tasks to load
                if(CPU1.getestRuntime() == CPU2.getestRuntime()){
                    //load task from file
                    std::getline(fh, taskLine);
                    #if EXTRA
                    if(taskLine.size() < 7){
                        break;
                    }
                    #endif
                    Task fh_task1 = Task(taskLine);
                    CPU1.push_back(fh_task1);
                    cnt++;

                    if(cnt < T){
                        std::getline(fh, taskLine);
                        #if EXTRA
                        if(taskLine.size() < 7){
                            break;
                        }
                        #endif
                        Task fh_task2 = Task(taskLine);
                        CPU2.push_back(fh_task2);
                        cnt++;
                    }

                }
                else if(CPU1.getestRuntime() < CPU2.getestRuntime()){
                    //load task from file
                    std::getline(fh, taskLine);
                    #if EXTRA
                    if(taskLine.size() < 7){
                        break;
                    }
                    #endif
                    Task fh_task1 = Task(taskLine);
                    ++cnt;
                    CPU1.push_back(fh_task1);
                }

                else if(CPU1.getestRuntime() > CPU2.getestRuntime()){
                    //load task from file
                    std::getline(fh, taskLine);
                    #if EXTRA
                    if(taskLine.size() < 7){
                        break;
                    }
                    #endif
                    Task fh_task2 = Task(taskLine);
                    ++cnt;
                    CPU2.push_back(fh_task2);
                }
            }

            //once filled start loading and cycle
            //BUG HERE awile ago
            if(normal){
                int cRun1 = CPU1.load_front().getRuntime();
                int cRun2 = CPU2.load_front().getRuntime();

                if(cRun1 == cRun2){
                    outTime += cRun1;
                    CPU1.eject_front();
                    CPU2.eject_front();
                }
                else if(cRun1 < cRun2){
                    outTime += cRun1;
                    CPU1.eject_front();
                    CPU2.execute_front(cRun1);
                }
                else{ //crun1 > cRun2
                    outTime += cRun2;
                    CPU2.eject_front();
                    CPU1.execute_front(cRun2);
                }
            }
            #if 1
            bool to_emptyC1 = CPU1.gettaskTotal() == 1;
            bool to_emptyC2 = CPU2.gettaskTotal() == 1;

            if((CPU1.gettaskTotal() <= 1 || CPU2.gettaskTotal() <= 1) && cnt == T ){
                normal = false;
            }

            if(normal == false){
                if(to_emptyC1 || to_emptyC2){
                    if(to_emptyC1 && !to_emptyC2){ //1 and >1

                        if(CPU1.getestRuntime() >= CPU2.getestRuntime()){
                            outTime += CPU1.getestRuntime();
                        }
                        else{ //CPU2 > CPU1
                            outTime += CPU2.getestRuntime();
                        }
                        CPU2.load_all();
                        CPU1.eject_front();
                    }

                    else if(to_emptyC1 && to_emptyC2){

                        if(CPU1.getestRuntime() >= CPU2.getestRuntime()){
                            outTime += CPU1.getestRuntime();
                        }
                        else{ //CPU2 > CPU1
                            outTime += CPU2.getestRuntime();
                        }
                        CPU1.eject_front();
                        CPU2.eject_front();
                    }

                    else if(!to_emptyC1 && to_emptyC2){ //>1 and 1

                        if(CPU1.getestRuntime() >= CPU2.getestRuntime()){
                            outTime += CPU1.getestRuntime();
                        }
                        else{ //CPU2 > CPU1
                            outTime += CPU2.getestRuntime();
                        }
                        CPU1.load_all();
                        CPU2.eject_front();
                    }
                    
                }

                else if(CPU1.gettaskTotal() == 0 && CPU2.gettaskTotal() > 0){
                        outTime += CPU2.getestRuntime();
                        CPU2.load_all();
                    }
                else if(CPU1.gettaskTotal() > 0 && CPU2.gettaskTotal() == 0){
                        outTime += CPU1.getestRuntime();
                        CPU1.load_all();
                    }
            #endif
            }
            
        }

    }

    #if 1
    if(M == 0){
        std::string taskLine1;
        std::string taskLine2;
        bool c1 = true;
        while(cnt < 2*C && cnt < T){
            #if DEBUG
            std::cout << "Q STATE:" << '\n';
            CPU1.printQState();
            CPU2.printQState();
            std::cout << '\n';
            #endif
            if(c1){
                std::getline(fh, taskLine1);
                Task T1 = Task(taskLine1);
                CPU1.push_back(T1);
                cnt++;
                c1 = false;
            }
            else{
                std::getline(fh, taskLine2);
                Task T2 = Task(taskLine2);
                CPU2.push_back(T2);
                cnt++;
                c1 = true;
            }

        }
        

        while(cnt < T){
            int cRun1 = CPU1.load_front().getRuntime();
            int cRun2 = CPU2.load_front().getRuntime();
            if(cRun1 == cRun2){
                outTime += cRun1;
                //I EJECTED 2
                CPU1.eject_front();
                CPU2.eject_front(); 
                //reloading 
                std::getline(fh, taskLine1);
                Task T1 = Task(taskLine1);
                CPU1.push_back(T1);
                cnt++;
                if(cnt < T){
                    std::getline(fh, taskLine2);
                    Task T2 = Task(taskLine2);
                    CPU2.push_back(T2);
                    cnt++;
                }
                
            }

            else if(cRun1 < cRun2){
                //ejected
                outTime += cRun1;
                CPU1.eject_front();
                CPU2.execute_front(cRun1); //1
                //reload
                std::getline(fh, taskLine1);
                Task T1 = Task(taskLine1);
                CPU1.push_back(T1);
                cnt++;
                c1 = false;
            }

            else if(cRun1 > cRun2){
                outTime += cRun2;
                CPU2.eject_front();
                CPU1.execute_front(cRun2); //1

                //reload
                std::getline(fh, taskLine2);
                Task T2 = Task(taskLine2);
                CPU2.push_back(T2);
                cnt++;
                c1 = true;
            }
        }

        while(CPU1.gettaskTotal() > 0 && CPU2.gettaskTotal() > 0){
            int cRun1 = CPU1.load_front().getRuntime();
            int cRun2 = CPU2.load_front().getRuntime();
            if(cRun1 == cRun2){
                outTime += cRun1;
                CPU1.eject_front();
                CPU2.eject_front(); 
            }
            else if(cRun1 < cRun2){
                outTime += cRun1;
                CPU1.eject_front();
                CPU2.execute_front(cRun1);
            }
            else if(cRun1 > cRun2){
                outTime += cRun2;
                CPU2.eject_front();
                CPU1.execute_front(cRun2);
            }
        }

        if(CPU1.gettaskTotal() > 0 && CPU2.gettaskTotal() == 0){
            outTime += CPU1.getestRuntime();
            CPU1.load_all();
        }
        else if(CPU1.gettaskTotal() == 0 && CPU2.gettaskTotal() > 0){
            outTime += CPU2.getestRuntime();
            CPU2.load_all();
        }

    }
    #endif


    #if DEBUG
    std::cout << "FINAL\n";
    std::cout << "C1:\n";
    CPU1.printQState();
    
    std::cout << "C2:\n";
    CPU2.printQState();
    
    std::cout << "OUTPUT: ";
    #endif

    printf("%d\n", outTime);
    //td::cout << outTime << '\n';

    return 0;
}