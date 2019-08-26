#include <stdio.h>
#include "DataStructures.c"


void generatePacket(Packet *hosts, Packet *lasts, int src, int dst, int numOfSwitches, int *lastIDs);

/*
    Vi du minh hoa cua ham generatePacket:
    Dau vao: hosts[src - numOfSwitches] = NULL; lasts[src - numOfSwitches] = NULL; lastID[src - numOfSwitches] = 14;
    Lan 1 khi goi generatePacket(hosts, lasts, 32, 21, 20, lastIDs)
    thi sau khi thuc thi xong, ta co t = hosts[src - numOfSwitches] la mot packet
    co: (i) id = 15; (ii) t-> next = NULL; lasts[src - numOfSwitches] chua chinh t

    Lan 2 khi goi generatePacket(hosts, lasts, 32, 21, 20, lastIDs)
    thi sau khi thuc thi xong, ta co t = hosts[src - numOfSwitches] la mot packet
    co: (i) id = 15; (ii) t-> next = mot packet P nao do ma P->id = 16; 
    lasts[src - numOfSwitches] chua chinh P
 */
void generatePacket(Packet *hosts, Packet *lasts, int src, int dst, int numOfSwitches, int *lastIDs)
//Ham nay thuc hien viec tao ra Packet, roi luu tru vao phan tu hosts[src - numOfSwitches]
//cung nhu Packet moi se duoc luu vao trong lasts[src - numOfSwitches]
//Yeu cau dau vao:  (i) cac hosts, lasts, lastIDs khong bi NULL
//                  (ii) src, dst phai khong gay ra loi truy xuat phan tu ngoai bo nho
//Yeu cau dau ra: (i) neu ban dau hosts va lasts NULL thi sau khi ket thuc ham, hosts va lasts co chua 1 phan tu
//                (ii) neu hosts hoac lasts duoc cap nhat thi lastsID[src-numOfSwitches] phai duoc tang them 1 gia tri so voi luc truoc khi thuc hien ham
{
    if(hosts == NULL || lasts == NULL || lastIDs == NULL)//Neu hosts va lasts la 2 mang NULL
    {   
        nullPointerException(__LINE__, __FILE__, __func__);
        return;
    }

    if(src - numOfSwitches < 0 || src - numOfSwitches >= numOfSwitches)
    {
        outOfBoundException(__LINE__, __FILE__, __func__);
        return;
    }

    Packet first, last, current;
    int id;

    if(hosts[src - numOfSwitches] == NULL)//Neu mang hosts[src - numOfSwitches] khong chua Packet nao ca
    //noi cach khac, hosts thu (src) van chua tao ra packet nao ca hoac cac packet deu da len duong di roi
    {
        id = lastIDs[src - numOfSwitches] ++;
        current = createPacket(id, src, dst, 0);//Khoi tao Packet voi id = lastID
        first = current; last = current;
        first->next = NULL;//Vi Packet nay la dau tien nen truong next cua no tro den NULL
        last->next = NULL;//Vi Packet nay la dau tien nen truong next cua no tro den NULL

        hosts[src - numOfSwitches] = first;//Nap vao trong mang hosts, hosts thu (src) dang chua Packet voi id = lastID
        lasts[src - numOfSwitches] = last;//Nap vao trong mang lasts, hosts thu (src) dang chua Packet cuoi cung id = lastID
    }
    else{
        
        first = hosts[src - numOfSwitches];//Lay ra packet dung dau tien trong danh sach sourceQueue
        last = lasts[src - numOfSwitches];//Lay ra packet dung CUOI CUNG trong danh sach sourceQueue
        id = last->id; //Lay ra ID cua packet cuoi cung
        id++; //Tang gia tri ID len
        
        current = createPacket(id, src, dst, 0);//Tao ra packet voi id moi
        if(first->next == NULL)//Neu hien tai sourceQueue chi co mot packet
            first->next = current;//packet nay tro den packet tiep theo (vua moi tao)
        else{
            last-> next = current;//Nguoc lai neu sourceQueue da co nhieu packet ben trong
            //thi phan tu cuoi cung se tro den packet current (vua moi duoc tao ra)
        }
        current->next = NULL;
        lasts[src - numOfSwitches] = current;//mang lasts se luu tru packet current vua moi duoc tao ra
        
    }

    lastIDs[src - numOfSwitches] = id; //Cap nhat gia tri cua ID cuc dai
    return;
}