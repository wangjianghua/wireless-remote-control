/** 
 * @file     aes_lib
 * @brief    AES�����㷨 
 * @details  AES(Advanced Encryption Standard)�����㷨
 * @author   ����
 * @email    suqiniat@163.com
 * @date     
 * @version  vX.XX
 * @par Copyright (c):  
 *           ���ݼҺ����ܿ������޹�˾
 * @par History:          
 *     vX.XX: 
 *           @author  ����
 *           @email   591881218@qq.com
 *           @date    2013
 *           @note    ��֧��128λ���ܱ�׼
 */

#ifndef __AES_LIB_TINY_H__
#define __AES_LIB_TINY_H__

#ifdef __cplusplus
extern "C" {
#endif


/* �������� */
typedef unsigned char AES_U8;
typedef unsigned short AES_U16;

/* �궨�� */
#define Nc                          (4) //state�������չ��Կ������
#define Nr                         (10) //��������
#define Nk                          (4) //һ����Կ������

extern const AES_U8 g_AesExpKey[4 * Nc * (Nr + 1)]; //��չ��Կ

/* ����ԭ�� */
void AES_ExpandKey(AES_U8 *key, AES_U8 *expKey); //��չ��Կ�Ӳ������൱�ڳ�ʼ��AES�㷨
void AES_Encrypt(AES_U8 *in, AES_U8 *expKey, AES_U8 *out); //128λ���ܺ���
void AES_Decrypt(AES_U8 *in, AES_U8 *expKey, AES_U8 *out); //128λ���ܺ���


/*************************************************************************************
                                       ��ʹ�÷�����
                                                                            
#include <stdio.h>
#include <string.h>
#include "aes_lib.h"


AES_U8 in[16]  = {        //����16�ֽڣ�128λ������
    0x00,0x11,0x22,0x33,
    0x44,0x55,0x66,0x77,  
    0x88,0x99,0xaa,0xbb,
    0xcc,0xdd,0xee,0xff
};  

AES_U8 key[16] = {        //����16�ֽڣ�128λ����Կ
    0x00,0x01,0x02,0x03,
    0x04,0x05,0x06,0x07,  
    0x08,0x09,0x0a,0x0b,
    0x0c,0x0d,0x0e,0x0f
};  
                  
AES_U8 out[16]; //����һ������16��Ԫ�أ�128λ�������飬���������


void main(void) //����Ӵ˴���ʼ����
{
    AES_U8 expKey[4 * Nc * (Nr + 1)]; //����һ������չ��Կ�����飬����4*4*(10+1)��Ԫ��
    AES_U16 index; //����һ���������ͱ�������Ϊ��������
    

    memset(expKey, 0, sizeof(expKey)); //��expKey��������
    AES_ExpandKey(key, expKey); //��Կ����չ���㷨�ĳ�ʼ����

    memset(out, 0, sizeof(out)); //��out��������
    AES_Encrypt(in, expKey, out); //��in����������м��ܣ���ͨ��out�������

    for(index=0; index<16; index++) //ͨ��out�����������
    {
        printf("%.2x ", out[index]);
    }
    printf("\n"); //�س���

    memset(in, 0, sizeof(in)); //��in��������
    AES_Decrypt(out, expKey, in); //��out���Ľ��н��ܣ���ͨ��in�������

    for(index=0; index<16; index++) //ͨ��in���齫���ܺ���������
    {
        printf("%.2x ", in[index]);
    }
    printf("\n"); //�س���

    while(1);
}

*************************************************************************************/


#ifdef __cplusplus
}
#endif

#endif
