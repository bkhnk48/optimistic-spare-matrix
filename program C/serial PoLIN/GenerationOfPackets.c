#include <stdio.h>
#include "IntegratedPort.c"


void insertSourceQueue(Host *hosts, int src, int dst, int numOfSwitches);

//void sendToOutPort(int host, int aSwitch, IntegratedPort **IntegratedPortOfSwitches, int time, Packet *p);

void sendToOutPort(Host host, Switch *switches)
{
    //IntegratedPort ports = getOutPort(host, switches);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
    int t = //ports->bufferIn;
            (int)((Packet)((host->outPort)->outputPort[4]));//TRONG C, NULL luon mang gia tri 0
            //Nguon: https://stackoverflow.com/questions/1296843/what-is-the-difference-between-null-0-and-0
            /* Note: This concept applies to the C language, not C++.*/
    #pragma GCC diagnostic pop

    int temp;
    Packet first = NULL;
    unsigned char stFlag = 0;
    switch (t)//chinh la cau lenh kiem tra if (outPort.canReceive()) 
    {
        case 0:
            //ports->swFlag = 0;
            temp = dequeue(host, &first);//lay Packet ra tu source queue cua host
            switch(temp)
            {
                case 1:
                    addPacketToBuffer(first, host);
                    //if ((outPort.getBuffer().size() == 1) && (!outPort.stFlag)) { outPort.stFlag = true;//tuc la duoc phep gui
                    //                  forwardToLink();//se gui du lieu tu outport sang LINK     //    }
                    int emptySlots = getEmptySlot((host->outPort)->outputPort);
                    switch (emptySlots)
                    {
                        case 1://emptySlot co index = 1, tuc la if ((outPort.getBuffer().size() == 1)
                            stFlag = (host->outPort) -> stFlag;
                            switch (stFlag)
                            {
                                case 0:
                                    (host->outPort) -> stFlag = 1;
                                    forwardToLink(host);
                                break;
                            
                            default:
                                break;
                            }
                        break;
                    }
                    break;
            }
            break;
        default:    
            host-> outPort -> swFlag = 0;
            break;

    }
}

void insertSourceQueue(Host *hosts, int src, int dst, int numOfSwitches)
//Ham nay thuc hien viec tao ra Packet, roi luu tru vao phan tu hosts[src - numOfSwitches] -> queue
//cung nhu Packet moi se duoc luu vao trong hosts[src - numOfSwitches] -> last
//Yeu cau dau vao:  (i) cac hosts khong bi NULL
//                  (ii) src, dst phai khong gay ra loi truy xuat phan tu ngoai bo nho
//Yeu cau dau ra: (i) neu ban dau hosts[src - numOfSwitches] -> queue == NULL thi sau khi ket thuc ham, 
//                    hosts[src - numOfSwitches] -> queue co chua 1 phan tu
//                (ii) neu hosts[src - numOfSwitches] -> queue duoc cap nhat thi 
//                    hosts[src - numOfSwitches] -> last phai duoc tang them 1 gia tri so voi luc 
//                    truoc khi thuc hien ham
{
    
    if(hosts == NULL)//Neu hosts la mang NULL
    {   
        nullPointerException(__LINE__, __FILE__, __func__);
        return;
    }
    

    if(src - numOfSwitches < 0 || src - numOfSwitches >= numOfSwitches
        || dst - numOfSwitches < 0 || dst - numOfSwitches > numOfSwitches
    )
    {
        outOfBoundException(__LINE__, __FILE__, __func__);
        return;
    }

    Packet first, last, current;
    
    int id = (hosts[src - numOfSwitches] -> lastID);
    id++; //Tang gia tri ID len
    current = createPacket(id, src, dst, 0);//Khoi tao Packet voi id = lastID + 1
    
    if(hosts[src - numOfSwitches] -> queue == NULL)//Neu phan tu hosts[src - numOfSwitches] khong chua Packet nao ca
    //noi cach khac, hosts thu (src) van chua tao ra packet nao ca hoac cac packet deu da len duong di roi
    {
        first = current; last = current;
        ((Packet)(first))-> next = NULL;//Vi Packet nay la dau tien nen truong next cua no tro den NULL
        ((Packet)last)-> next = NULL;//Vi Packet nay la dau tien nen truong next cua no tro den NULL

        hosts[src - numOfSwitches] -> queue = first;//Nap vao trong mang hosts, hosts thu (src) dang chua Packet voi id = lastID
        hosts[src - numOfSwitches] -> last = first;
    }
    else{
        printf("\nCall it %s at line: %d\n", __func__, __LINE__);
        first = hosts[src - numOfSwitches] -> queue;//Lay ra packet dung dau tien trong danh sach sourceQueue
        last = hosts[src - numOfSwitches] -> last;//Lay ra packet dung CUOI CUNG trong danh sach sourceQueue
        
        if(((Packet)first)->next == NULL)//Neu hien tai sourceQueue chi co mot packet
            ((Packet)first)->next = current;//packet nay tro den packet tiep theo (vua moi tao)
        else{
            ((Packet)last)-> next = current;//Nguoc lai neu sourceQueue da co nhieu packet ben trong
            //thi phan tu cuoi cung se tro den packet current (vua moi duoc tao ra)
        }
        
        hosts[src - numOfSwitches] -> last = current;//mang lasts se luu tru packet current vua moi duoc tao ra
        
    }

    (hosts[src - numOfSwitches] -> lastID) = id; //Cap nhat gia tri cua ID cuc dai
    return;
}

//void sendToOutPort(int host, int aSwitch, IntegratedPort **IntegratedPortOfSwitches, int time, Packet *p)
//{
    
//}