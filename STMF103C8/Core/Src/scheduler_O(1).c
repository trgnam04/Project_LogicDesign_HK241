/*
 * scheduler_O(1).c
 *
 *  Created on: Oct 1, 2024
 *      Author: ASUS
 */


#include "scheduler_O(1).h"
#include <stdlib.h>

Container* container = NULL;

S_Task* addNode(void(*pFunction)(), int delay, int period){
	S_Task* newNode = (struct S_Task*)malloc(sizeof(struct S_Task));
	newNode->Delay = delay;
	newNode->Period = period;
	newNode->pTask = pFunction;
	newNode->next = NULL;
	return newNode;
}

void SCH_Init(void){
	container = (struct Container*)malloc(sizeof(struct Container));
	container->tail = NULL;
	container->numTask = 0;
	container->emptySlot = 0;
}

void SCH_Add_Task(void(*pFunction)(), int PERIOD, int DELAY){
    S_Task* newNode;

    if(container->tail == NULL){
        // add first node
        newNode = addNode(pFunction, DELAY, PERIOD);
        container->tail = newNode;
        container->tail->next =  newNode;
        container->numTask++;
    }
    else{
        S_Task* prevTail = container->tail->next;
        S_Task* temp = container->tail->next;
        int Total = 0;

        for(int i = 0; i < container->numTask; i++){
            Total += temp->Delay;
            if((Total + temp->next->Delay > DELAY) && temp != container->tail){
                // add head
                if(Total > DELAY){
                    if(container->emptySlot == 0){
                        newNode = addNode(pFunction, DELAY, PERIOD);
                        // update next node DELAY
                        temp->Delay -= DELAY;
                        // update newNode position
                        newNode->next = temp;
                        container->tail->next = newNode;
                        //update numTask
                        container->numTask++;
                        return;
                    }
                    else{
                        // update new head
                        container->tail->Delay = PERIOD;
                        container->tail->Period = PERIOD;
                        //update next to node DELAY
                        temp->Delay -= PERIOD;
                        // update tail
                        while(prevTail->next != container->tail){
                            prevTail = prevTail->next;
                        }
                        container->tail = prevTail;
                        // update emptySlot
                        container->emptySlot--;
                        return;
                    }
                }
                // add middle
                if(container->emptySlot == 0){
                    // update newNode DELAY
                    newNode = addNode(pFunction, DELAY - Total, PERIOD);
                    // update position
                    newNode->next = temp->next;
                    temp->next = newNode;
                    // update next to DELAY
                    newNode->next->Delay -=  newNode->Delay;

                    container->numTask++;
                    return;
                }
                else{
                    // update newNode DELAY
                    newNode = container->tail;
                    newNode->Delay = PERIOD - Total;
                    newNode->Period = PERIOD;
                    // update new tail
                    while(prevTail->next != container->tail){
                        prevTail = prevTail->next;
                    }
                    prevTail->next = newNode->next;
                    container->tail = prevTail;
                    // update new node position
                    newNode->next = temp->next;
                    temp->next = newNode;
                    // update next to DELAY
                    newNode->next->Delay -= newNode->Delay;

                    container->emptySlot--;
                    return;
                }
            }

            // add tail
            if(temp == container->tail){
                if(container->emptySlot == 0){
                    // update DELAY
                    newNode = addNode(pFunction, DELAY - Total, PERIOD);
                    // update position
                    newNode->next = temp->next;
                    temp->next = newNode;
                    container->tail = newNode;
                    // update num Task
                    container->numTask++;
                    return;
                }
                else{
                    // update DELAY
                    container->tail->Delay = PERIOD - Total;
                    container->tail->Period = PERIOD;
                    // update empty slot
                    container->emptySlot--;
                    return;
                }
            }
            temp = temp->next;
        }
    }
}

void SCH_Delete_Task(void){
	container->tail->next->Delay = 0;
	container->tail->next->Period = 0;
	container->tail->next->pTask = NULL;
	container->tail = container->tail->next;
	container->emptySlot++;
}

void SCH_Update_Task(void){
	if(container != NULL && container->tail != NULL){
		if(container->tail->next->Delay > 0){
			container->tail->next->Delay--;
		}
	}

}

void SCH_Dispatch_Task(void){
	if(container->tail->next->Delay == 0){
		(*(container -> tail -> next -> pTask))();
		struct S_Task temp = *(container -> tail -> next);
		SCH_Delete_Task();
		if(temp.Period != 0){
			SCH_Add_Task(temp.pTask, temp.Period, temp.Period);
		}
	}
}

