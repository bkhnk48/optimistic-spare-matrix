int swap(int *type, int idElementInGroup,
                int *portID,
                unsigned long *endTime,
                int haveEmptySlot,
                int *idNewNode,
                CalendarQueue* q){
    //leftBound:   1 vs 0
    //middle:      3 vs 0
    //rightBound:  5 vs 0
    if(haveEmptySlot == 0)
    {
        if(q == NULL)  return 0;
        enqueue(new_node(*type,
                            idElementInGroup, *portID, *endTime), q);
        return 0;
    }
    else{
        if(q == NULL)  return 1;
        if(q->qsize == 0)
        {
            return 1;
        }
        else{
            if(q->last_prio >= *endTime)//neu tham so can them be hon hoac bang phan
            //tu min cua calendar queue
            {
                return 1;
            }
            else{
                enqueue(new_node(*type,
                            idElementInGroup, *portID, *endTime), q);
                Node* temp = dequeue(q);
                *type = temp->type;
                *portID = temp->portID;
                *endTime = temp->endTime;
                free(temp);

                return 1;
            }
        }
    }

    return 1;
}

void push(CalendarQueue* q, unsigned long arr[20250][7], unsigned long idElementInGroup ){

}
