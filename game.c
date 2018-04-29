//////////////////////////////////////////////////////////////
vision 1.0
build 2018-3-18
/////////////////////////////////////////////////////////////
#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<stdio.h>

#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c)&0x8000)  //宏定义

typedef struct postion
{
	int x;   //x轴坐标
	int y;   //y轴坐标
}posType;


void init(void);   //初始化界面
void gamebegin(void);  //游戏的开始界面
void gameplay(void);   //玩游戏的具体过程
void close(void);   //释放资源
void drawcursor(posType,COLORREF);  //绘制光标
void getsamecolorball(posType cur,COLORREF c1);  //获取相邻同色小球的坐标
int isValid(posType,COLORREF);  //判读是否合法
void ballFall();  //小球下落
void drawscore(int score); //绘制分数
void drawtime(int sec);  //绘制时间
void turn();  //互换数组


COLORREF colorArr[6]={RGB(255,0,0),RGB(0,255,0),RGB(0,0,255),
                       RGB(255,255,0),RGB(255,0,255),RGB(0,255,255)};

posType cur;
posType ballArr[180];
int index;

void main()
{
	init();
	gamebegin();
	gameplay();
	close();
}

void init()
{
    initgraph(1000,700);
}

void gamebegin()
{
	//绘制边框
   	setlinecolor(RGB(255,0,255));  //边框颜色
	setlinestyle(PS_SOLID,5);      //边框粗细程度
        rectangle(255,45,745,655);     //边框位置及大小
	//绘制小球
	srand((unsigned) time(NULL));   //提供随机种子
	setlinestyle(PS_SOLID,8);   
	for(int x=280;x<740;x+=40)
	{
		for(int y=70;y<650;y+=40)
		{
			COLORREF c1=colorArr[rand()%6];  //rand()%6提供0~5共6个随机数
			setlinecolor(c1);    
	                setfillcolor(c1);  
			fillcircle(x,y,10);
		}
	}
   	//绘制光标
	cur.x=480;          //坐标x,y为光标所在的初始坐标，位于界面中心
	cur.y=390;
 	drawcursor(cur,RGB(255,255,255));
}


void  gameplay()
{
    cur.x=480;
    cur.y=390;
    int score=0;    //分数初始化
	for(int i=299;i>=0;i--)    //游戏时间
	{
		if(i%10==0)
		{
 	    	drawtime(i/10);   //每隔1s更新一次
		}
       	//按键控制(如VK_RETURN为回车的虚拟键值)	
     	if(KEY_DOWN(VK_UP)&&cur.y>70)           //上
		{
		    drawcursor(cur,RGB(0,0,0));
		    cur.y-=40;
		   	drawcursor(cur,RGB(255,255,255));
		}
	    else if(KEY_DOWN(VK_DOWN)&&cur.y<610)   //下
		{
		    drawcursor(cur,RGB(0,0,0));
	    	cur.y+=40;
		    drawcursor(cur,RGB(255,255,255));
		}
	    else if(KEY_DOWN(VK_LEFT)&&cur.x>280)   //左
		{
		    drawcursor(cur,RGB(0,0,0));
		    cur.x-=40;
		    drawcursor(cur,RGB(255,255,255));
		}
	    else if(KEY_DOWN(VK_RIGHT)&&cur.x<700)  //右
		{
		    drawcursor(cur,RGB(0,0,0));
		    cur.x+=40;
		    drawcursor(cur,RGB(255,255,255));
		}
	    else if(KEY_DOWN(VK_RETURN)||KEY_DOWN(VK_SPACE))  //空格或回车
		{
		    //获取相邻同色小球的坐标
		    getsamecolorball(cur,getpixel(cur.x,cur.y));
		    //小球消失
		    if(index>1)
			{
			    for(int k=0;k<=index;k++)
				{
         	        setlinecolor(RGB(0,0,0));
                    setlinestyle(PS_SOLID,8);
                   	setfillcolor(RGB(0,0,0));  
                   	fillcircle(ballArr[k].x ,ballArr[k].y,10);
				}
			    Sleep(500);
			   	ballFall(); 	//小球下落
                score+=index;
			   	drawscore(index);
			}
		index=0;
		}
    	Sleep(100);   //按键监控
	}
}


void close()
{
		getch();
		closegraph();
}

void drawcursor(posType pos,COLORREF c1)
{
	setlinecolor(c1);
	setlinestyle(PS_SOLID,2);
	rectangle(pos.x-20,pos.y-20,pos.x+20,pos.y+20);
}

void getsamecolorball(posType cur,COLORREF c1)   //把相邻同色小球的坐标加入到同一数组内
{
	ballArr[index].x=cur.x;
    ballArr[index].y=cur.y;
	index++;

    posType tmpPos;
	for(int k=0;k<4;k++)
	{
		switch(k)
		{
			case 0:tmpPos.x=cur.x;tmpPos.y=cur.y-40;break;  //上   
			case 1:tmpPos.x=cur.x;tmpPos.y=cur.y+40;break;  //下
			case 2:tmpPos.x=cur.x-40;tmpPos.y=cur.y;break;  //左
			case 3:tmpPos.x=cur.x+40;tmpPos.y=cur.y;break;  //右
		} 
		if(isValid(tmpPos,c1))
		{
	        getsamecolorball(tmpPos,c1);
		}
 
	}
}

void drawscore(int score)
{
	char str[20];
	settextcolor(RGB(255,0,0));
	settextstyle(16,0,_T("宋体"));
	sprintf(str,"分数:%d",score);
	outtextxy(30,600,str);
}

void drawtime(int sec)
{
	char str[30];
	settextcolor(RGB(255,255,0));
	settextstyle(24,0,_T("宋体"));
	sprintf(str,"剩余时间: %2d s",sec);
	outtextxy(20,50,str);
}

int isValid(posType cur,COLORREF c1)
{
	if(getpixel(cur.x ,cur.y)!=c1)   //函数getpixel()用来获取当前坐标的颜色
	{
		return 0;
	}
	else
	{
		for(int i=0;i<index;i++)
		{
			if(cur.x==ballArr[i].x&&cur.y==ballArr[i].y)
			{
				return 0;
			}
		}
		return 1;
	}

}

void ballFall()
{
	turn();
	for(int i=0;i<index;i++)
	{
    
	    for(int k=ballArr[i].y;k>70;k-=40)
		{
	        COLORREF c1=getpixel(ballArr[i].x,k-40);
            setlinecolor(c1);
            setlinestyle(PS_SOLID,8);
            setfillcolor(c1);  
            fillcircle(ballArr[i].x,k,10);
		}
		COLORREF c1=colorArr[rand()%6]; 
		setlinecolor(c1);    
	    setfillcolor(c1);  
		fillcircle(ballArr[i].x,70,10);
   }
}

void turn()
{
	int i,j;
	posType temp;
	for(j=0;j<index-1;j++)
	for(i=0;i<index-1;i++)
	{
		if(ballArr[i].x>ballArr[i+1].x)   
		{
			temp=ballArr[i];
			ballArr[i]=ballArr[i+1];
			ballArr[i+1]=temp;
		}
		if(ballArr[i].y>ballArr[i+1].y)
		{
			temp=ballArr[i];
			ballArr[i]=ballArr[i+1];
			ballArr[i+1]=temp;
		}
	}	
}
