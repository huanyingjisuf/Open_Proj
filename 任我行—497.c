#define _TANGER_C_
#include "reg52.h"
#include "absacc.h"

#define uchar unsigned char
#define uint unsigned int
#define PA XBYTE[0XF8FF]
#define PB XBYTE[0XF9FF]
#define PC XBYTE[0XFAFF]
#define CON XBYTE[0XFBFF]


#define key PC
#define lcd P0
#define zuoyi pb&=0x7f
#define youyi pb&=0xbf
#define tingyi pb|=0xc0
#define xiashou pb&=0xef
#define shangshou pb|=0x10
#define jiaqiu pb&=0xdf
#define fangqiu pb&=0x20

#define xiawan pa&0x10
#define shangwan pa&0x08
#define hb pa&0x20
#define h1 pa&0x40
#define h2 pa&0x80

uchar pa,pb=0xff;//��е�ִ����������б�����pbĬ��ֵ����Ϊ0xff

void delay(uint p)	//һ������ʱ
{
	uchar q;
	while(p--)
	for(q =124;q>0;q--);
}

void dian_d()//�������ר�ö���ʱ������������÷�ʽ��2��3��4����1��������0
{
	uchar q =30;
	while(q--);
}

sbit cs2 =P1^0;
sbit cs1 =P1^1;
sbit e =P1^2;
sbit rs =P1^3;

void zhiling(uchar zl)//Һ��ָ������
{
	EA =0;//��ֹ���߶�Һ�����ݵ�Ӱ��
	e =0;rs =0;
	lcd =zl;
	e =1;e =0;
	EA =1;
}
void shuju(uchar zl)//Һ����������
{
	EA =0;
	e =0;rs =1;
	lcd =zl;
	e =1;e =0;
	EA =1;
}
//ֱ�����뺺��
uchar code zk[]={
	"��ʼ����"
};

//�����ֿ�
uchar code hanzi[]={
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x08,0x89,0xEA,0x18,0x88,0x00,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
0x02,0x01,0x00,0xFF,0x01,0x86,0x40,0x20,0x18,0x07,0x40,0x80,0x40,0x3F,0x00,0x00,

/*--  ����:  ʼ  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x10,0x10,0xF0,0x1F,0x10,0xF0,0x00,0x40,0xE0,0x58,0x47,0x40,0x50,0x60,0xC0,0x00,
0x40,0x22,0x15,0x08,0x16,0x21,0x00,0x00,0xFE,0x42,0x42,0x42,0x42,0xFE,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x80,0x60,0xF8,0x07,0x00,0x00,0x00,0xFF,0x40,0x20,0x10,0x08,0x04,0x00,0x00,
0x01,0x00,0x00,0xFF,0x00,0x04,0x02,0x01,0x3F,0x40,0x40,0x40,0x40,0x40,0x78,0x00,

/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0xC0,0xC0,0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


};
//�����ֿ�
uchar code shuzi[]={
	/*--  ����:  0  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,

/*--  ����:  1  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,

/*--  ����:  2  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,

/*--  ����:  3  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,

/*--  ����:  4  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,

/*--  ����:  5  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,

/*--  ����:  6  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,

/*--  ����:  7  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,

/*--  ����:  8  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,

/*--  ����:  9  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,


};

//ͨ�ù���������ܶ���
uchar code tab[]={
0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff	
};

//Һ��ָ��
#define on zhiling(0x3f)
#define op zhiling(0xc0)

//Ѱ��ָ�һ�����������ֽ�
uchar tr(uchar *s)
{
	uchar m1,m2,i;
	m1 =*s;
	s++;
	m2 =*s;
	i =0;
	while (i--)
	{
		if((zk[i]==m1)&&(zk[i+1]==m2))
		return i/2;
		else i+=2;
	}
}
//����д�������������ֺţ��������һЩ����
void han(uchar x,uchar y,uchar *s)
{
	uint m1,y1,i;
	x =0xb6+x*2;
	y =0x38+y*8;
	while (*s!='\0')
	{
		m1 =*s;
		m1*=32;
		for (i =16;i>0;i--)
		{
			if (y>0x7f)
			{
				cs1 =0;cs2 =1;
				y1 =y-0x40;
			}
			else
			{
				cs1 =1;cs2 =0;
				y1 =y;
			}
			zhiling(x);
			zhiling(y1);
			shuju(hanzi[m1]);
			zhiling(x+1);
			zhiling(y1);
			shuju(hanzi[m1+16]);
			y++;m1++;
		}
		s+=2;
	}
	cs1 =0;cs2 =0;
}


void shu(uchar x,uchar y,uint num)
{
	uchar i,y1;
	x =0xb6+x*2;
	y =0x38+y*8;
	num*=16;
	for (i =8;i>0;i--)
	{
		if (y>0x7f)
		{
			cs1 =0;cs2 =1;
			y1 =y-0x40;
		}
		else
		{
			cs1 =1;cs2 =0;
			y1 =y;
		}
		zhiling(x);
		zhiling(y1);
		shuju(shuzi[num]);
		zhiling(x+1);
		zhiling(y1);
		shuju(shuzi[num+8]);
		y++;num++;
	}
	cs1 =0;cs2 =0;
}

//�����Ӻ���
void qingping()
{
	uchar i=8,j=0xb8,k;
	on;op;
	cs1 =1;cs2 =1;
	while (i--)
	{
		zhiling(j);
		zhiling(0x40);
		for(k =64;k>0;k--)shuju(0);
		j++;
	}
	cs1 =0;cs2 =0;
}

#define DM XBYTE[0X7FFF]
#define WM XBYTE[0XBFFF]
uchar d[8];
void did()
{
	d[0]=0;
	d[1]=0;
	d[2]=0;
	d[3]=0;
	d[4]=0;
	d[5]=0;
	d[6]=0;
	d[7]=0;
}

uchar a,b =0xfe;
void disp()
{
	DM =0XFF;
	DM =tab[d[a++]];
	WM =b;
	b =b<<1|0x01;
	if (b==0xff)
	{
		a =0;b =0xfe;
	}
}

bit zt;
void stop()//�����д�while�Ľϳ�ָ���в���ۣ����ڼ�ʱ��Ӧ��ͣ��ֹͣ����ѭ������ҲҪ���������
{
	if (zt)
	{
		tingyi;
		while(zt);
	}
}
#define stop stop()//���ڲ���
sbit cp =P1^4;
sbit dir =P1^5;
sbit rl =P1^6;

uchar dj;//���������ǰλ��
void dian_i()//��ʼ��������۰��Ų�������ν
{
	uint i =1500;
	dir =1;
	while (!rl)
	{
		delay(10);
		while (!rl)
		{
			cp =~cp;
			dian_d();
		}
	}
	dir =0;
	while (i--)
	{
		cp =~cp;
		dian_d();
	}
	dj =0;
}

void dj_run(uchar gr)//��������Ӻ������ع���
{
	uint i;
	if (gr>dj)
	{
		i =gr-dj;
		i*=1380;
		dir =0;
		while (i--)
		{
			cp =~cp;
			dian_d();
			stop;
		}
	}
	else
	{
		i =dj-gr;
		i*=1380;
		dir =1;
		while (i--)
		{
			cp =~cp;
			dian_d();
			stop;
		}
	}
	dj =gr;
}

uchar gx;
void going(uchar gr)//��е������going now
{
	if(gx==3)while(shangwan)shangshou;
	if (gr>gx)
	{		
		switch (gr)
		{
			case 1:while(pa&0x01){zuoyi;stop;}tingyi;	break;
			case 2:while(pa&0x01){zuoyi;stop;}delay(200);tingyi;	break;
			case 3:while(pa&0x01){zuoyi;stop;}tingyi;	break;
			
			default:	break;
		}
	}
	else
	{		
		switch (gr)
		{
			case 1:while(pa&0x01){youyi;stop;}tingyi;	break;
			case 2:while(pa&0x01){youyi;stop;}tingyi;	break;
			case 3:while(pa&0x01){youyi;stop;}tingyi;	break;
			
			default:	break;
		}
	}
	gx =gr;
}

void going_j(uchar gr,bit jf)//��е�ּ��������let's read
{
	going(gr);
	if (jf)
	{
		if(gx!=3)
		{
			while(xiawan)xiashou;
			jiaqiu;
			delay(300);
		}
		else
		{
			jiaqiu;
			delay(300);
		}
		while(shangwan)shangshou;
	}
	else
	{
		fangqiu;
		delay(300);
	}
}

uchar going_pd()//�ж�����ɫ���ҵĴ򿪷�ʽ����ֻ������
{
	if (h1||h2)
	{
		if (h1)
		{
			going(2);
			while(xiawan)xiashou;
			if (hb)return 23;else return 13;
		}
		else
		if (h2)
		{
			going(1);
			while(xiawan)xiashou;
			if(hb)return 32;else return 31;
		}
	}
	else
	{
		going(2);
		while(xiawan)xiashou;
		if (hb)return 21;else return 12;
	}
}

void going_init()//��е�ֳ�ʼ��
{
	while(shangwan)shangshou;//��ײ
	while(pa&0x40)zuoyi;tingyi;
	gx =3;
}

void time_init()//���ߺͰ����Ϳ�������
{
	TMOD =0X01;//����Ϊֵ��0x01�Ͳ��Ѷ�ʱ��1�����£���ֻ���������¸�ֵ������ͦ���
	TH0 =0XFC;
	TL0 =0X18;
	EA =1;
	TR0 =1;
	ET0 =1;
	TR1 =1;
	ET1 =1;
}

void time0_interrupt() interrupt 1//������Ӧ����ר�Ƹ��ַ�Ӧ������Ҫ��̫��
{
	TH0 =0XFC;
	TL0 =0X18;
	PB =pb;
}


sbit sb1 =P3^5;//���������ж�ɨ���ʽ����
bit rk,ks;
#define ADC XBYTE[0XF7FF]//ADC���߾�����ʵ�ʵ�ַ����Ӳ������
uint adc;//�������㣬adcʵʱֵӦ�����α���
uchar du;//ADCֵת�¶��㷨ʵʱֵ�洢λ��
uchar dianya;//ADCֵת��ѹ�㷨ʵʱֵ�洢λ��
bit shuaadc;//ˢadc��־λ
void time1_interrupt() interrupt 3//�����ж���Ӧ����������ż�����delayһ�ᣬ
{
	pa =PA;
	if(shuaadc)
	{
		shuaadc =0;
		adc =ADC;//��Ȼ���˵�ѹת������Ӧ�ö�����������
		du =adc*100.0/255;
		dianya =adc*500.0/255;
	}
	else
	{
		shuaadc =1;
		ADC =0X00;//����ת��
	}
	if (!sb1)
	{
		if (rk)
		{
			rk =0;
			ks =1;
			//���º��ָ��
		}
		else
		rk =1;
	}
	else
	ks =0;
}

void main()
{
	time_init();
	dian_i();
	going_init();
	while (1)
	{//Let's going now ������
	}
}

