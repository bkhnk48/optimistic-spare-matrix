#include <stdio.h>
//#include "DataStructures.c"
#include "Host.c"


void insertSourceQueue(Host *hosts, int src, int dst, int numOfSwitches);

//void sendToOutPort(int host, int aSwitch, IntegratedPort **IntegratedPortOfSwitches, int time, Packet *p);

void sendToOutPort(Host host, Switch *switches)
{
    if(CanReceive(host, switches))
        printf("Can send packet from host");
    else
    {
        printf("Full of next buffer");
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
        ((Packet)(first))->next = NULL;//Vi Packet nay la dau tien nen truong next cua no tro den NULL
        ((Packet)last)->next = NULL;//Vi Packet nay la dau tien nen truong next cua no tro den NULL

        hosts[src - numOfSwitches] -> queue = first;//Nap vao trong mang hosts, hosts thu (src) dang chua Packet voi id = lastID
        hosts[src - numOfSwitches] -> last = first;
    }
    else{
        
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