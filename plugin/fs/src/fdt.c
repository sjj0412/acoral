#define IN_FDT
#include "../include/fs.h"

/*********************************************************************************************************
** ��������: ClearClus
** ��������: ��ָ����������������
**
** �䡡��: Disk���߼�����Ϣ
**        Index���غ�
** �䡡��: RETURN_OK:�ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,OpenSec,WriteSec,CloseSec
********************************************************************************************************/
        acoral_u8 ClearClus(acoral_u8 Drive, acoral_u32 Index)
{
    acoral_u8 i, temp;
    acoral_u8 *Buf;
    acoral_u32 SecIndex;
    acoral_u16 j;
    Disk_Info *Disk;

    Disk = GetDiskInfo(Drive);
    if (Disk != NULL)
    {
        if (Index < Disk->ClusPerData)
        {
            temp = Disk->SecPerClus;
            Index -= 2;                     /* 2���������� */
            SecIndex = Disk->DataStartSec + Index * temp;
            for (i = 0; i < temp; i++)
            {
                Buf=OpenSec(Drive, SecIndex);
                if(Buf!=NULL)  //���������Ѿ��ڻ�����������Ҫ����
                {
                	for(j=0;j<Disk->BytsPerSec;j++)
                		Buf[j]=0;
                }
                WriteSec(Drive, SecIndex);
                CloseSec(Drive, SecIndex);
                SecIndex++;
            }
            return RETURN_OK;
        }
        else
        {
            return CLUSTER_NOT_IN_DISK;
        }
    }
    else
    {
        return NOT_FIND_DISK;
    }
}

/*********************************************************************************************************
** ��������: ReadFDTInfo
** ��������: ��ȡFDT��Ϣ
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        SecIndex��������
**        ByteIndex��ƫ����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: OpenSec,ReadSec,CloseSec
********************************************************************************************************/
        acoral_u8  ReadFDTInfo(FDT *Rt, acoral_u8 Drive, acoral_u32 SecIndex, acoral_u16 ByteIndex)
{
    acoral_u8 *Buf;
    acoral_u8 temp;
    
    Buf = OpenSec(Drive, SecIndex);
    temp = NOT_EMPTY_CACHE;
    if (Buf != NULL)
    {

        temp = ReadSec(Drive, SecIndex);
        if (temp == RETURN_OK)
        {
            Buf = Buf + ByteIndex;
            Rt->Name[0] = *Buf++;
            Rt->Name[1] = *Buf++;
            Rt->Name[2] = *Buf++;
            Rt->Name[3] = *Buf++;
            Rt->Name[4] = *Buf++;
            Rt->Name[5] = *Buf++;
            Rt->Name[6] = *Buf++;
            Rt->Name[7] = *Buf++;
            Rt->Name[8] = *Buf++;
            Rt->Name[9] = *Buf++;
            Rt->Name[10] = *Buf++;
            Rt->Attr = *Buf++;
            Rt->NTRes = *Buf++;
            Rt->CrtTimeTenth = *Buf++;
            Rt->CrtTime = Buf[0] | (Buf[1] << 8);
            Rt->CrtDate = Buf[2] | (Buf[3] << 8);
            Rt->LstAccDate = Buf[4] | (Buf[5] << 8);
            Rt->FstClusHI = Buf[6] | (Buf[7] << 8);
            Rt->WrtTime = Buf[8] | (Buf[9] << 8);
            Rt->WrtDate = Buf[10] | (Buf[11] << 8);
            Rt->FstClusLO = Buf[12] | (Buf[13] << 8);
            Rt->FileSize = Buf[14]  | (Buf[15] << 8);
            Rt->FileSize |= (((acoral_u32)Buf[16] << 16) | ((acoral_u32)Buf[17] << 24));
            temp = RETURN_OK;
        }
        CloseSec(Drive, SecIndex);
    }
    return temp;
}

/*********************************************************************************************************
** ��������: WriteFDTInfo
** ��������: дFDT��Ϣ
**
** �䡡��:
**        Drive����������
**        SecIndex��������
**        ByteIndex��ƫ����
**        FDT *FDTData:����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: OpenSec,ReadSec,CloseSec
********************************************************************************************************/
        acoral_u8  WriteFDTInfo(acoral_u8 Drive, acoral_u32 SecIndex, acoral_u16 ByteIndex, FDT *FDTData)
{
    acoral_u8 *Buf;
    acoral_u8 temp;
    
    temp = NOT_EMPTY_CACHE;
    Buf = OpenSec(Drive, SecIndex);
    if (Buf != NULL)
    {
        temp = ReadSec(Drive, SecIndex);
        if (temp == RETURN_OK)
        {
            Buf = Buf + ByteIndex;
            *Buf++ = FDTData->Name[0];
            *Buf++ = FDTData->Name[1];
            *Buf++ = FDTData->Name[2];
            *Buf++ = FDTData->Name[3];
            *Buf++ = FDTData->Name[4];
            *Buf++ = FDTData->Name[5];
            *Buf++ = FDTData->Name[6];
            *Buf++ = FDTData->Name[7];
            *Buf++ = FDTData->Name[8];
            *Buf++ = FDTData->Name[9];
            *Buf++ = FDTData->Name[10];
            *Buf++ = FDTData->Attr;
            *Buf++ = FDTData->NTRes;
            *Buf++ = FDTData->CrtTimeTenth;
            *Buf++ = FDTData->CrtTime;
            *Buf++ = (FDTData->CrtTime) >> 8;
            *Buf++ = FDTData->CrtDate;
            *Buf++ = (FDTData->CrtDate) >> 8;
            *Buf++ = FDTData->LstAccDate;
            *Buf++ = (FDTData->LstAccDate) >> 8;
            *Buf++ = FDTData->FstClusHI;
            *Buf++ = (FDTData->FstClusHI) >> 8;
            *Buf++ = FDTData->WrtTime;
            *Buf++ = (FDTData->WrtTime) >> 8;
            *Buf++ = FDTData->WrtDate;
            *Buf++ = (FDTData->WrtDate) >> 8;
            *Buf++ = FDTData->FstClusLO;
            *Buf++ = (FDTData->FstClusLO) >> 8;
            *Buf++ = (FDTData->FileSize);
            *Buf++ = (FDTData->FileSize) >> 8;
            *Buf++ = (FDTData->FileSize) >> 16;
            *Buf = (FDTData->FileSize) >> 24;
            WriteSec(Drive, SecIndex);
        }
        CloseSec(Drive, SecIndex);
    }
    return temp;
}

/*********************************************************************************************************
** ��������: GetRootFDTInfo
** ��������: ��ȡ��Ŀ¼ָ���ļ���Ŀ¼����Ϣ
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        Index���ļ���Ŀ¼����FDT�е�λ��
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,GetFDTInfo,ReadFDTInfo
********************************************************************************************************/
        acoral_u8 GetRootFDTInfo(FDT *Rt, acoral_u8 Drive, acoral_u32 Index)
{
    Disk_Info * Disk;
    acoral_u16 ByteIndex;
    acoral_u32 SecIndex;
    acoral_u8 temp;
    
    temp = NOT_FIND_DISK;
    Disk = GetDiskInfo(Drive);
    if (Disk != NULL)
    {
        temp = NOT_FAT_DISK;
        Index = Index * 32;        /* 32:sizeof(FDT ) */
        if (Disk->FATType == FAT12 || Disk->FATType == FAT16)
        {
            temp = FDT_OVER;
            if (Index < (Disk->RootSecCnt * Disk->BytsPerSec))
            {
                ByteIndex = Index % Disk->BytsPerSec;
                SecIndex = Index / Disk->BytsPerSec + Disk->RootDirTable;
                temp = ReadFDTInfo(Rt, Drive, SecIndex, ByteIndex);
            }
        }
    }
    return temp;
}

/*********************************************************************************************************
** ��������: GetFDTInfo
** ��������: ��ȡָ��Ŀ¼ָ���ļ���Ŀ¼����Ϣ
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        Index���ļ���Ŀ¼����FDT�е�λ��
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,ReadFDTInfo
********************************************************************************************************/
        acoral_u8 GetFDTInfo(FDT *Rt,acoral_u8 Drive, acoral_u32 ClusIndex, acoral_u32 Index)
{
    Disk_Info * Disk;
    acoral_u16 ByteIndex;
    acoral_u32 SecIndex, i;

    Disk = GetDiskInfo(Drive);
    if (Disk == NULL)
    {
        return NOT_FIND_DISK;
    }

    if (ClusIndex == EMPTY_CLUS)
    {
        if (Disk->FATType == FAT32)
        {
            ClusIndex = Disk->RootDirTable;
        }
        else
        {
            return GetRootFDTInfo(Rt, Drive, Index);
        }
    }

    if (Disk->FATType == FAT12 ||
        Disk->FATType == FAT16 ||
        Disk->FATType == FAT32)
    {
        Index = Index * 32;
        ByteIndex = Index % Disk->BytsPerSec;
        SecIndex = Index / Disk->BytsPerSec;
        /* ����Ŀ¼���������� */
        i = Disk->SecPerClus;
        while(SecIndex >= i)
        {
            ClusIndex = FATGetNextClus(Drive, ClusIndex);
            if (ClusIndex <= EMPTY_CLUS_1 ||
                ClusIndex >= BAD_CLUS) 
            {
                return FDT_OVER;
            }
            SecIndex -= i;
        }
        SecIndex = (ClusIndex - 2) * i + SecIndex + Disk->DataStartSec;
        return ReadFDTInfo(Rt, Drive, SecIndex, ByteIndex);
    }
    return NOT_FAT_DISK;
}

/*********************************************************************************************************
** ��������: SetRootFDTInfo
** ��������: ���ø�Ŀ¼ָ���ļ���Ŀ¼����Ϣ
**
** �䡡��: FDTData��Ҫд�����Ϣ
**        Drive����������
**        Index���ļ���Ŀ¼����FDT�е�λ��
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,SetFDTInfo,WriteFDTInfo
********************************************************************************************************/
        acoral_u8 SetRootFDTInfo(acoral_u8 Drive, acoral_u32 Index, FDT *FDTData)
{
    Disk_Info * Disk;
    acoral_u16 ByteIndex;
    acoral_u32 SecIndex;
    acoral_u8 Rt;
    
    Rt = NOT_FIND_DISK;
    Disk = GetDiskInfo(Drive);
    if (Disk != NULL)
    {
        Index = Index * 32;        /* 32:sizeof(FDT ) */
        if (Disk->FATType == FAT12 || Disk->FATType == FAT16)
        {
            Rt = FDT_OVER;
            if (Index < (Disk->RootSecCnt * Disk->BytsPerSec))
            {
                ByteIndex = Index % Disk->BytsPerSec;
                SecIndex = Index / Disk->BytsPerSec + Disk->RootDirTable;
                Rt = WriteFDTInfo(Drive, SecIndex, ByteIndex, FDTData);
            }
        }
    }
    return Rt;
}

/*********************************************************************************************************
** ��������: SetFDTInfo
** ��������: ����ָ��Ŀ¼ָ���ļ���Ŀ¼����Ϣ
**
** �䡡��: FDTData��Ҫд�����Ϣ
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        Index���ļ���Ŀ¼����FDT�е�λ��
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,WriteFDTInfo
********************************************************************************************************/
        acoral_u8 SetFDTInfo(acoral_u8 Drive, acoral_u32 ClusIndex, acoral_u32 Index, FDT *FDTData)
{
    Disk_Info * Disk;
    acoral_u16 ByteIndex;
    acoral_u32 SecIndex;
    acoral_u8 i;
    
    Disk = GetDiskInfo(Drive);
    if (Disk == NULL)
    {
        return NOT_FIND_DISK;
    }

    if (ClusIndex == EMPTY_CLUS)
    {
        if (Disk->FATType == FAT32)
        {
            ClusIndex = Disk->RootDirTable;
        }
        else
        {
            return SetRootFDTInfo(Drive, Index, FDTData);
        }
    }

    if (Disk->FATType == FAT12 ||
        Disk->FATType == FAT16 ||
        Disk->FATType == FAT32)
    {
        Index = Index * 32;
        ByteIndex = Index % Disk->BytsPerSec;
        SecIndex = Index / Disk->BytsPerSec;
        /* ����Ŀ¼���������� */
        i = Disk->SecPerClus;
        while(SecIndex >= i)
        {
            ClusIndex = FATGetNextClus(Drive, ClusIndex);
            if (ClusIndex <= EMPTY_CLUS_1 ||
                ClusIndex >= BAD_CLUS) 
            {
                return FDT_OVER;
            }
            SecIndex -= i;
        }
        SecIndex = (ClusIndex - 2) * i + SecIndex + Disk->DataStartSec;

        return WriteFDTInfo(Drive, SecIndex, ByteIndex, FDTData);
    }
    return NOT_FAT_DISK;
}

/*********************************************************************************************************
** ��������: FindFDTInfo
** ��������: ��ָ��Ŀ¼����ָ���ļ���Ŀ¼��
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        FileName���ļ���Ŀ¼����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,GetFDTInfo
********************************************************************************************************/
        acoral_u8 FindFDTInfo(FDT *Rt,acoral_u8 Drive, acoral_u32 ClusIndex, acoral_char FileName[])
{
    acoral_u32 i;
    acoral_u8 temp;
    

    i = 0;
    if (FileName[0] == DEL_FDT)
    {
        FileName[0] = ESC_FDT;
    }
    while (1)
    {
        temp = GetFDTInfo(Rt, Drive, ClusIndex, i);
        if (temp != RETURN_OK)
        {
            break;
        }

        if (Rt->Name[0] == EMPTY_FDT)
        {
            temp = NOT_FIND_FDT;
            break;
        }
        if ((Rt->Attr & ATTR_VOLUME_ID) == 0)
        if (FileName[0] == Rt->Name[0])
        if (FileName[1] == Rt->Name[1])
        if (FileName[2] == Rt->Name[2])
        if (FileName[3] == Rt->Name[3])
        if (FileName[4] == Rt->Name[4])
        if (FileName[5] == Rt->Name[5])
        if (FileName[6] == Rt->Name[6])
        if (FileName[7] == Rt->Name[7])
        if (FileName[8] == Rt->Name[8])
        if (FileName[9] == Rt->Name[9])
        if (FileName[10] == Rt->Name[10])
        {
            temp = RETURN_OK;
            break;
        }
        i++;
    }
    if (FileName[0] == ESC_FDT)
    {
        FileName[0] = DEL_FDT;
    }
    return temp;
}

/*********************************************************************************************************
** ��������: AddFDT
** ��������: ��ָ��Ŀ¼������ָ���ļ���Ŀ¼��
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        FileName���ļ���Ŀ¼����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: FindFDTInfo,GetFDTInfo,SetFDTInfo
********************************************************************************************************/
        acoral_u8 AddFDT(acoral_u8 Drive, acoral_u32 ClusIndex, FDT *FDTData)
{
    Disk_Info * Disk;
    acoral_u32 i;
    FDT TempFDT;
    acoral_u8 temp;

    Disk = GetDiskInfo(Drive);
    if (Disk == NULL)
    {
        return NOT_FIND_DISK;
    }

    if (ClusIndex == EMPTY_CLUS)
    if (Disk->FATType == FAT32)
    {
        ClusIndex = Disk->RootDirTable;
    }

    temp = FindFDTInfo(&TempFDT, Drive, ClusIndex, FDTData->Name);
    if (temp == RETURN_OK)
    {
        return FDT_EXISTS;
    }
    if (temp != NOT_FIND_FDT)
    if (temp != FDT_OVER)
    {
        return temp;
    }

    if (FDTData->Name[0] == DEL_FDT)
    {
        FDTData->Name[0] = ESC_FDT;
    }

    i = 0;
    temp = RETURN_OK;
    while (temp == RETURN_OK)
    {
        temp = GetFDTInfo(&TempFDT, Drive, ClusIndex, i);
        if (temp == RETURN_OK)
        {
            if (TempFDT.Name[0] == DEL_FDT || TempFDT.Name[0] == EMPTY_FDT)
            {
                temp = SetFDTInfo(Drive, ClusIndex, i, FDTData);
                break;
            }
        }
        i++;
    }
    if (temp == FDT_OVER)
    if (ClusIndex != EMPTY_CLUS)
    {
        i = FATAddClus(Drive, ClusIndex);
        temp = DISK_FULL;
        if (i != BAD_CLUS)
        {
            ClearClus(Drive, i); 
            temp = SetFDTInfo(Drive, i, 0, FDTData);
        }
    }
    if (FDTData->Name[0] == ESC_FDT)
    {
        FDTData->Name[0] = DEL_FDT;
    }
    return temp;
}

/*********************************************************************************************************
** ��������: DelFDT
** ��������: ��ָ��Ŀ¼ɾ��ָ���ļ���Ŀ¼��
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        FileName���ļ���Ŀ¼����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,GetFDTInfo,SetFDTInfo
********************************************************************************************************/
        acoral_u8 DelFDT(acoral_u8 Drive, acoral_u32 ClusIndex, acoral_char FileName[])
{
    acoral_u32 i;
    FDT TempFDT;
    acoral_u8 temp;
    
    i = 0;
    if (FileName[0] == DEL_FDT)
    {
        FileName[0] = ESC_FDT;
    }
        
    while (1)
    {
        temp = GetFDTInfo(&TempFDT, Drive, ClusIndex, i);
        if (temp != RETURN_OK)
        {
            break;
        }
            
        if (TempFDT.Name[0] == EMPTY_FDT)
        {
            temp = NOT_FIND_FDT;
            break;
        }
        if ((TempFDT.Attr & ATTR_VOLUME_ID) == 0)
        if (FileName[0] == TempFDT.Name[0])
        if (FileName[1] == TempFDT.Name[1])
        if (FileName[2] == TempFDT.Name[2])
        if (FileName[3] == TempFDT.Name[3])
        if (FileName[4] == TempFDT.Name[4])
        if (FileName[5] == TempFDT.Name[5])
        if (FileName[6] == TempFDT.Name[6])
        if (FileName[7] == TempFDT.Name[7])
        if (FileName[8] == TempFDT.Name[8])
        if (FileName[9] == TempFDT.Name[9])
        if (FileName[10] == TempFDT.Name[10])
        {
            TempFDT.Name[0] = DEL_FDT;
            temp = SetFDTInfo(Drive, ClusIndex, i, &TempFDT);
            break;
        }
        i++;
    }
    if (FileName[0] == ESC_FDT)
    {
        FileName[0] = DEL_FDT;
    }
    return temp;
}

/*********************************************************************************************************
** ��������: ChangeFDT
** ��������: �ı�ָ��Ŀ¼ָ���ļ���Ŀ¼��������
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        FileName���ļ���Ŀ¼����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,GetFDTInfo,SetFDTInfo
********************************************************************************************************/
        acoral_u8 ChangeFDT(acoral_u8 Drive, acoral_u32 ClusIndex, FDT *FDTData)
{
    acoral_u32 i;
    acoral_u8 temp;
    FDT TempFDT;

    i = 0;
    if (FDTData->Name[0] == DEL_FDT)
    {
        FDTData->Name[0] = ESC_FDT;
    }
    while (1)
    {
        temp = GetFDTInfo(&TempFDT, Drive, ClusIndex, i);
        if (temp != RETURN_OK)
        {
            break;
        }
            
        if (TempFDT.Name[0] == EMPTY_FDT)
        {
            temp = NOT_FIND_FDT;
            break;
        }
        if ((TempFDT.Attr & ATTR_VOLUME_ID) == 0)
        if (FDTData->Name[0] == TempFDT.Name[0])
        if (FDTData->Name[1] == TempFDT.Name[1])
        if (FDTData->Name[2] == TempFDT.Name[2])
        if (FDTData->Name[3] == TempFDT.Name[3])
        if (FDTData->Name[4] == TempFDT.Name[4])
        if (FDTData->Name[5] == TempFDT.Name[5])
        if (FDTData->Name[6] == TempFDT.Name[6])
        if (FDTData->Name[7] == TempFDT.Name[7])
        if (FDTData->Name[8] == TempFDT.Name[8])
        if (FDTData->Name[9] == TempFDT.Name[9])
        if (FDTData->Name[10] == TempFDT.Name[10])
        {
            temp = SetFDTInfo(Drive, ClusIndex, i, FDTData);
            break;
        }
        i++;
    }
    if (FDTData->Name[0] == ESC_FDT)
    {
        FDTData->Name[0] = DEL_FDT;
    }
    return temp;
}

/*********************************************************************************************************
** ��������: DirIsEmpty
** ��������: 
**
** �䡡��: Drive����������
**        ClusIndex��Ŀ¼�״غ�
** �䡡��: DIR_EMPTY����
**        DIR_NOT_EMPTY������
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetFDTInfo
********************************************************************************************************/
        acoral_u8 DirIsEmpty(acoral_u8 Drive, acoral_u32 ClusIndex)
{
    acoral_u32 i;
    FDT TempFDT;
    acoral_u8 temp;
    Disk_Info * Disk;

    
    if (ClusIndex == EMPTY_CLUS)
    {
        return DIR_NOT_EMPTY;
    }
/* �ж��Ƿ���fat32 �ĸ�Ŀ¼ */
    Disk = GetDiskInfo(Drive);
    if (Disk == NULL)
    {
        return NOT_FIND_DISK;
    }
    if (Disk->FATType == FAT32)
    if (ClusIndex == Disk->RootDirTable )
    {
        return DIR_NOT_EMPTY;
    }
    
    i = 2;
    while (1)
    {
        temp = GetFDTInfo(&TempFDT, Drive, ClusIndex, i);
        if (temp != RETURN_OK)
        {
           return temp; 
        }
    
        if ((TempFDT.Attr & ATTR_VOLUME_ID) == 0)
        if (TempFDT.Name[0] != DEL_FDT)
        {
            break;
        }
        i++;
    }
    if (TempFDT.Name[0] == EMPTY_FDT)
    {
        return DIR_EMPTY;
    }
    else
    {
        return DIR_NOT_EMPTY;
    }
}

/*********************************************************************************************************
** ��������: FDTIsLie
** ��������: ��ָ��Ŀ¼�鿴ָ���ļ���Ŀ¼���Ƿ����
**
** �䡡��: Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        FileName���ļ���Ŀ¼����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,GetFDTInfo
********************************************************************************************************/
        acoral_u8 FDTIsLie(acoral_u8 Drive, acoral_u32 ClusIndex, acoral_char FileName[])
{
    acoral_u32 i;
    acoral_u8 temp;
    FDT temp1;
    
    i = 0;
    if (FileName[0] == DEL_FDT)
    {
        FileName[0] = ESC_FDT;
    }
    while (1)
    {
        temp = GetFDTInfo(&temp1, Drive, ClusIndex, i);
        if (temp == FDT_OVER)
        {
            temp = NOT_FIND_FDT;
            break;
        }

        if (temp != RETURN_OK)
        {
            break;
        }

        if (temp1.Name[0] == EMPTY_FDT)
        {
            temp = NOT_FIND_FDT;
            break;
        }
        if ((temp1.Attr & ATTR_VOLUME_ID) == 0)
        if (FileName[0] == temp1.Name[0])
        if (FileName[1] == temp1.Name[1])
        if (FileName[2] == temp1.Name[2])
        if (FileName[3] == temp1.Name[3])
        if (FileName[4] == temp1.Name[4])
        if (FileName[5] == temp1.Name[5])
        if (FileName[6] == temp1.Name[6])
        if (FileName[7] == temp1.Name[7])
        if (FileName[8] == temp1.Name[8])
        if (FileName[9] == temp1.Name[9])
        if (FileName[10] == temp1.Name[10])
        {
            temp = FDT_EXISTS;
        }
        i++;
    }
    if (FileName[0] == ESC_FDT)
    {
        FileName[0] = DEL_FDT;
    }
    return temp;
}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/