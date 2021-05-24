//

//  main.cpp

//  P9_PartialOrderingandScheduling

//

//  Created by Niraj on 4/11/20.

//  Copyright � 2020 Niraj. All rights reserved.

//



#include <iostream>

#include <fstream>

using namespace std;



class Node{

public:

    int jobId; // jobid

    int jobTime;  //

    int dependentcount; //how many node is dependent on it

    Node* next;

    

    Node(){

    }

    

    Node(int jobId1, int jobTime1, int dependentcount1, Node* next1){

        jobId = jobId1;

        jobTime = jobTime1;

        dependentcount = dependentcount1;

        next = next1;

    }

    

    Node(int jobId3, int jobTime3, int dependentcount3){

           jobId = jobId3;

           jobTime = jobTime3;

           dependentcount = dependentcount3;

                  }

    

    Node(int jobId2, int dependentcount2, Node* next2){

        jobId= jobId2;

        dependentcount = dependentcount2;

        next = next2;

    }

    

void printNode(Node* head, ofstream& outFile1){

}

};



class JOBS{

public:

    int jobTime; //processing time required for the job

    int onwhichproc = 0; // means not on any processor

    int onOpen = 0; //means not on open

    int parentCount;

    int dependentCount;

    

    JOBS(){

        

    }

    

    JOBS(int jobtime1, int onwhichproc1, int onopen1, int parentcount1, int dependentcount1){

        jobTime = jobtime1;

        onwhichproc= onwhichproc1;

        onOpen = onopen1;

        parentCount = parentcount1;

        dependentCount = dependentcount1;

    }

    

};



class Proc{

public:

    int doWhichJob = -1; // which job it is processing, -1 means it is available

    int timeRemain; // time remain on a job;

    

    Proc(){

        

    }

};



class Scheduling{

public:

    int numNodes; //total number of nodes in the input graph

    int numProcs;// num of processors is availabe to use

    int procUsed=0; // num of processors that are used so far;

    JOBS* jobAry; // jobarray that is dynamically located with size numnodes+1

    Proc* procAry; //procarray of size numprocs+1, dynamically allocated

    Node* OPEN; // node pointer

    

    int** adjMatrix; //2-D array with size numnode+1 by numnode + 1;

    int* parentCountAry; //array to hold parent node count

    int* dependentCountAry; //array to hold dependent count

    int* onGraphAry; //array to indicate whether a node has been removed, 1 means on the graph, 0 means not .

    int totalJobTimes; //sum of all nodes jobtime;

    int** scheduleTable; //2-D array of scheduletable size numprocs+1 by totalJobTimes + 1;

    int currentTime = 1;

    Scheduling(){

    }

  //1. correct

    void initialization(ifstream& inFile1, ifstream& inFile2, int numProcs1){

        procUsed = 0;

        currentTime = 0;

        Node* Open = new Node(-1, 0, 0);

        OPEN = Open;

        numProcs = numProcs1;

        inFile1 >> numNodes;

        if(numProcs > numNodes) numProcs = numNodes; //  means unlimited procs

       // inFile1 >> numNodes;

        adjMatrix = new int*[numNodes + 1];

        for(int i = 0; i < numNodes + 1; i++){

            adjMatrix[i] = new int[numNodes + 1];

            for(int j = 0; j< numNodes + 1; j++){

                adjMatrix[i][j] =0;

            }
            
        }

        

        JOBS* jobary1 = new JOBS[numNodes + 1];

        jobAry = jobary1;

        

        Proc* procary1 = new Proc[numProcs + 1];

        procAry = procary1;

        

        int* dependentCountAry1 = new int[numNodes + 1];

        dependentCountAry = dependentCountAry1;

        

        int* parentCountAry1 = new int[numNodes + 1];

        parentCountAry = parentCountAry1;

        

        int* ongraphAry1 = new int[numNodes+1];

        onGraphAry = ongraphAry1;

        for(int i = 0; i < numNodes + 1; i++){

            onGraphAry[i] = 1;

        }

        

        loadMatrix(inFile1, adjMatrix);
        
        /*for(int i = 1; i < numNodes+1; i++){
            for(int j = 1; j < numNodes+1; j++){
                cout << adjMatrix[i][j] << " ";
            }cout << endl;
        }cout << endl;
        */
        
          computeDependentCount(adjMatrix, dependentCountAry);

        computeParentCount(adjMatrix, parentCountAry);

        totalJobTimes = constructJobAry(inFile2, adjMatrix);
        
        /*for(int i = 1; i < numNodes+1; i++){
            cout << i << " ParentCount : " << parentCountAry[i] << " DependentCount: " << dependentCountAry[i] << endl;
        }*/

        scheduleTable = new int*[numProcs + 1];

        for(int i = 0; i < numProcs + 1; i++){

            scheduleTable[i] = new int[totalJobTimes + 1];

            for(int j = 0; j< totalJobTimes + 1; j++) {

                scheduleTable[i][j] = 0;

            }

        }
        
        /*for(int i = 1; i < numProcs+1; i++){
            for(int j = 0; j < totalJobTimes+1; j++){
                cout << scheduleTable[i][j] << " ";
            }cout << endl;
        }*/

    }

    

   //2. correct

    void loadMatrix(ifstream& inFile1, int** adjMatrix){//loading in adjacent matrix

        int y, z;

            for(int i = 0; i < numNodes + 1; i++){

                for( int j = 0; j < numNodes + 1; j++){

                    while(inFile1 >> y >> z){

                    adjMatrix[y][z] = 1;

                    }

                  //cout << "aj : "<< adjMatrix[i][j] << endl;

                }

                //outFile1 << endl;

            }
    }

    

    //3. correct

   void computeDependentCount(int** adjMatrix, int* dependentCountAry){

            for(int i = 0; i < numNodes + 1; i++){

                int count = 0;

                for(int j = 0; j<numNodes + 1; j++){

                    count += adjMatrix[i][j];

                }

                dependentCountAry[i]= count;

            }

      //outFile1 << "dependentcount :" <<endl;

       //for(int i = 0; i < numNodes + 1; i++){

          //outFile1 << i << " " <<  //dependentCountAry[i] << endl;

           

        //}

    

   }

    //4. correct

   void computeParentCount(int** adjMatrix, int* parentCountAry){

        int j;

        for(int i = 0; i < numNodes + 1; i++){

            int count = 0;

            for(j = 0; j<numNodes + 1; j++){

                count += adjMatrix[j][i];

            }

            parentCountAry[i]= count;

        }

      // outFile1 << "parentcount:" <<endl;;

      // for(int i = 0; i < numNodes + 1; i++){

        //  outFile1 << i << " " <<  parentCountAry[i] << endl;

          

       //}

    }

    //5  correct

        void loadOpen(ofstream& outFile1){

        //find orphan nodes and put them on open

            int orphanNode = findOrphan();

           //int jobId;

        while(orphanNode != -1 ){

            if(orphanNode > 0){
                //cout << "Orphan is: " << orphanNode << endl;
                
                int jobId = orphanNode;

                   int jobTime = jobAry[jobId].jobTime;

                Node* newNode = new Node(jobId,jobTime, dependentCountAry[jobId]);

                listInsert(newNode);

                jobAry[jobId].onOpen = 1;

            }
            
            orphanNode = findOrphan();

        }
    }
        
    //6. correct

   int findOrphan(){
       
       int x = -1;

        for( int i = 1; i <numNodes + 1; i++){

            if((parentCountAry[i] <= 0) &&  (jobAry[i].onOpen==0) && (jobAry[i].onwhichproc == -1)){

                x = i;

                return x;

            }else {
                
                x = -1;
                
            }

        }

        return x;
        }

    

    //7. correct

    int constructJobAry(ifstream& inFile2, int** adjMatrix){

        int totalTime = 0;

        int x, y;

        inFile2 >> numNodes;

        while(inFile2 >> x >> y){

            int nodeID = x;

            int jobTime = y;

            totalTime += jobTime;

            jobAry[nodeID].jobTime = jobTime;

            jobAry[nodeID].onwhichproc = -1;

            jobAry[nodeID].onOpen = 0;

            jobAry[nodeID].parentCount = parentCountAry[nodeID];

            jobAry[nodeID].dependentCount = dependentCountAry[nodeID];

        }

        return totalTime;

    }

    

    //8. correct

   void loadProcAry(){//find available processor to process noded on the open

       int jobId;

       int jobTime;
       
       Node* spot = OPEN;

       int availProc =  findProcessor();

        while((availProc > 0) && (spot->next != NULL) && (procUsed < numProcs)){

            if(availProc > 0){//there is processor available
            
                //cout << "Available Proc: " << availProc << endl;

                procUsed++;

                Node* newJob = spot->next;

                spot = spot->next;

                jobId = newJob->jobId;

                jobTime = newJob->jobTime;

                procAry[availProc].doWhichJob =jobId;

                procAry[availProc].timeRemain = jobTime;

                 putJobOnTable(availProc,currentTime, jobId, jobTime,scheduleTable);
                 
                 OPEN->next = spot->next;

        }
        
        availProc =  findProcessor();

    }
       

   }

    //9. correct

    int findProcessor(){

        int x = -1;

        for (int i = 1; i < numProcs + 1; i++){

            if(procAry[i].timeRemain <= 0){

                x = i;

                return x;

            }else{

                x = -1;

            }

        }

        return x;

    }

    

      //10.    correct  OPEN is listhead.

    void listInsert(Node* newNode){

        Node* spot = findSpot(OPEN, newNode);

        if (spot != NULL){

                  newNode->next = spot->next;

              spot->next = newNode;

          }

    }

    

    //11. correct

    Node* findSpot(Node* OPEN, Node* newNode){

        Node* spot = OPEN;

        while((spot->next!= NULL) && (spot->next->dependentcount > newNode->dependentcount)) {

            spot = spot->next;

        }

        return spot;

    }

    

    //12. correct

   void printNode(Node* OPEN, ofstream& outFile2){

        while(OPEN != NULL){

            outFile2 << "(";

            outFile2 << OPEN->jobId << ",";

            outFile2 << OPEN->dependentcount;

            outFile2 << ",";

            outFile2 << OPEN->jobTime;

            outFile2 << ")";

            outFile2 << "->";

            OPEN = OPEN->next;

        }

      if(OPEN == NULL) outFile2<< "(" << "NULL" <<"," << "0" << "," << "0" << ")" << "->";

       //OPEN = OPEN->next;

    }

    

    //13. correct

   void printList(Node* OPEN, ofstream& outFile2){

        Node* list;

        list = OPEN;

        outFile2 << "OPEN ->";

        //while(list != NULL){

            printNode(OPEN->next, outFile2);

            //list = list->next;
        //}

        outFile2 << "NULL" <<endl;;

    }

    

    //14. correct

    void printScheduleTable(ofstream& outFile1){

        outFile1<< "--";

        for(int i = 0; i < totalJobTimes + 1; i++){

            outFile1 << i << "--" ;

        }

        outFile1 << endl;

        for(int i=1; i < numProcs + 1; i++){

            outFile1 << "P(" << i << ")";

             for (int j = 0; j < totalJobTimes + 1; j++){

                 outFile1 << "|" << scheduleTable[i][j]<< " ";

             }

            outFile1 << endl;

            outFile1 << "----------------------------" <<endl;

         }

    }

    

    //15. correct

    void putJobOnTable(int availProc, int currentTime, int jobId, int JobTime, int** scheduleTable){//putting job on schedule table

        int Time = currentTime;

        int EndTime = Time + JobTime;

        while(Time < EndTime){

            scheduleTable[availProc][Time] = jobId;

            Time++;

        }

    }

    

    //16.

    bool checkCycle(){

    bool flag = false;

        if((OPEN->next == NULL) && (!graphIsEmpty()) && (processorsdone())){ //(procAry[i].timeRemain <= 0)){

            flag = true;

        }else {

            flag = false;

        }
        
        return flag;

    }

    

    //temp

    bool processorsdone(){

        bool flag = true;

        for(int i = 1; i < numProcs + 1; i++){

            if(procAry[i].timeRemain <= 0) {

                flag = true;

            }else {

                flag = false;
                
                return flag;

            }
        }

        return flag;

    }

    

    //17.

  bool graphIsEmpty(){

        bool flag = true;

        for (int i = 1; i < numNodes + 1; i++){

            if(onGraphAry[i] == 0){

                flag = true;

            }else{

                flag = false;
                
                return flag;

            }

        }

        return flag;

    }


    

    //18.

   void updateProcTime(){

        for(int i = 1; i< numProcs + 1; i++){

            if(procAry[i].timeRemain > 0){

                procAry[i].timeRemain -= 1;

            }
        }
    }

    

    //19.

    int findDoneProc(){

        for(int i = 1; i< numProcs + 1; i++){

            if(procAry[i].doWhichJob >0 && procAry[i].timeRemain <= 0){

                int jobID = procAry[i].doWhichJob;

                procAry[i].doWhichJob = -1;

                procUsed--;

                return jobID;

            }

        }

        return -1;

    }

    

    //20.

    void deleteFinishedNodes(){

        int jobId = findDoneProc();

        while (jobId > 0 ){

            if(jobId > 0){

                onGraphAry[jobId] = 0;

                deleteEdge(jobId);

            }
            
            jobId = findDoneProc();

        }
        
    }

        

   //21.

        void deleteEdge(int jobId){

            for( int j = 1; j < numNodes + 1; j++){

                if(adjMatrix[jobId][j] > 0){

                    parentCountAry[j]--;

                }

            }
            
        }

    

};







int main(int argc, const char * argv[]) {

    ifstream inFile1, inFile2;

    ofstream outFile1, outFile2;

    int numProcs1;

    inFile1.open(argv[1]);

    inFile2.open(argv[2]);

    numProcs1 = atoi(argv[3]);

    outFile1.open(argv[4]);

    outFile2.open(argv[5]);

    Scheduling* test = new Scheduling();

    test->initialization(inFile1, inFile2, numProcs1);

//while
    while(!test->graphIsEmpty()){
        
        //cout << "Current Time::::: " << test->currentTime << endl;

        test->loadOpen(outFile1);
    
        test->printList(test->OPEN, outFile2);

        test->loadProcAry();

        bool hasCycle = test->checkCycle();
        
        //cout << hasCycle << endl;

        if (hasCycle){

            cout << "there is cycle in the program" << endl;

            exit(0);

        }

        test->printScheduleTable(outFile1);
        
        test->currentTime++;

        test->updateProcTime();

        test->deleteFinishedNodes();
    
    }
    

    inFile1.close();

    inFile2.close();

    outFile1.close();

    outFile2.close();

    return 0;

}
