#include "includes.h"


void init_wdt(void)
{
    cspro = 0;
    cspro = 1; //����Դ����ģʽ��Ч���������ڲ�����ʱ��Ϊ125KHzʱ������ԼΪ32.8ms

    prc1 = 1; //������ģʽ�Ĵ���1д����
    pm12 = 1; //���Ź������λ
    prc1 = 0; //������ģʽ�Ĵ���1д����

    wdtr = 0x00; //��ʼ�����Ź�������
    wdtr = 0xff;

    wdts = 1; //�������Ź�
}

void restart_wdt(void)
{
    wdtr = 0x00;
    wdtr = 0xff;
}

