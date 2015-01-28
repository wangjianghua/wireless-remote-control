#include "includes.h"


void init_int0(void)
{
    int0f |= 0x02; //f8�����˲�
   
    int0pl = 1; //������
    pol_int0ic = 0; //�½���
    
    ir_int0ic = 0; //����жϱ�־λ
    ilvl_int0ic = 2; //�ж����ȼ�

    pd4_5 = 0; //IO������Ϊ����

    int0en = 1; //����
}

void init_ki2(void)
{
    ki2pl = 0; //�½���

    ir_kupic = 0; //����жϱ�־λ
    ilvl_kupic = 2; //�ж����ȼ�

    pd1_2 = 0; //IO������Ϊ����
    
    ki2en = 1; //����
}

void init_ki3(void)
{
    ki3pl = 0; //�½���

    ir_kupic = 0; //����жϱ�־λ
    ilvl_kupic = 2; //�ж����ȼ�

    pd1_3 = 0; //IO������Ϊ����
    
    ki3en = 1; //����
}

void init_ki(void)
{
    init_ki2();
    init_ki3();
}

