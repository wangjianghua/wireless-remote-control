#include "includes.h"


void init_lvi(void)
{
	prc3 = 1; //����дVCA2��VW1C��VW2C�Ĵ���
	
    /* VW2C �Ĵ��� */
	vw2c0 = 1; //��ѹ����2�ж�/��λ����
	vw2c1 = 0; //�����˲�����·��Ч
	vw2c2 = 0; //δ���
	vw2c3 = 0; //WDT��δ���
	vw2f0 = 0; //FRING-S��1��Ƶ
    vw2f0 = 0;
    vw2c6 = 0; //��ѹ����2�ж�ģʽ
    vw2c7 = 1; //VCC��Vdet2����ʱ�����ж�/��λ

	vca27 = 1; //��ѹ����·2��Ч
        
	prc3 = 0; //��ֹдVCA2��VW1C��VW2C�Ĵ���   
}

