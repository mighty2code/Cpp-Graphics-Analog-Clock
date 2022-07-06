/*Program to Display Analog Clock in the Output*/
/*Author: Tushar	Date: 07/03/2022	Modified: 10/03/2022*/

#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<time.h>
#include<string.h>

#define PI 22.0/7.0
#define BGI_PATH "C:\\TC\\BGI"

struct point {
	float x,y;
};

struct line_seg {
	point a,b;
};

struct clock_attributes {
	point centre;
	float radius;
	int display_digital;
	int bgcolor,text_col,face_color,digit_color,rim_color,rim_border_col;
	int pin_col,pin_border_col,hour_needle_col,min_needle_col,sec_needle_col;
};

enum Period {MILLI_SEC,SEC,MIN,HOUR};

/*Function to draw line segment.*/
void draw_line(const line_seg l) {
	line(l.a.x,l.a.y,l.b.x,l.b.y);
};

/*Returns location of point on circle at a given angle.*/
point get_pt(point c,float r,float duration,float deg)
{
	float theta=(duration*(deg))-90;
	theta=theta*PI/180;
	point end_pt;
	end_pt.x=r*cos(theta)+c.x;
	end_pt.y=r*sin(theta)+c.y;
	return end_pt;
}

/*Return Time in specified period from tm structure.*/
float get_time_in(const int period,struct tm time)
{
	switch(period)
	{
		case HOUR:
			return time.tm_hour+time.tm_min/60.0+time.tm_sec/3600.0;
		case MIN:
			return time.tm_min+time.tm_sec/60.0;
		case SEC:
			return time.tm_sec;
		case MILLI_SEC:
			return time.tm_sec/1000.0;
	}
	return 0;
}

/*Pause or Delay the Program for specified time.*/
void Pause(const float duration, const char period='s')
{
	float seconds;
	if (period == 'm' || period == 'M')
		seconds = duration * 60;
	else if (period == 'h' || period == 'H')
		seconds = duration * 60 * 60;
	else
		seconds = duration;

	clock_t end_wait;
	end_wait = clock() + seconds * CLOCKS_PER_SEC;

	while (clock() < end_wait) {}
}

/*Returns initialized attributes.*/
clock_attributes initialize() {
	clock_attributes init_attr={{0,0},-1,-1,-1,-1,-1,-1,
				    -1,-1,-1,-1,-1,-1,-1};
	return init_attr;
}

/*Set Default values to attributes which are not set.*/
void set_defaults(clock_attributes &attr)
{
	if(attr.centre.x==0 && attr.centre.y==0) {
		point c={320,240};
		attr.centre=c;
	}
	if(attr.radius==-1)
		attr.radius=200;
	if(attr.bgcolor==-1)
		attr.bgcolor=BLACK;
	if(attr.text_col==-1)
		attr.text_col=WHITE;
	if(attr.face_color==-1)
		attr.face_color=BLUE;
	if(attr.rim_color==-1)
		attr.rim_color=RED;
	if(attr.rim_border_col==-1)
		attr.rim_border_col=YELLOW;
	if(attr.digit_color==-1)
		attr.digit_color=YELLOW;
	if(attr.hour_needle_col==-1)
		attr.hour_needle_col=WHITE;
	if(attr.min_needle_col==-1)
		attr.min_needle_col=WHITE;
	if(attr.sec_needle_col==-1)
		attr.sec_needle_col=WHITE;
	if(attr.pin_col==-1)
		attr.pin_col=YELLOW;
	if(attr.pin_border_col==-1)
		attr.pin_border_col=BROWN;
	if(attr.display_digital==-1)
		attr.display_digital=1;
}

/*Function to Display Digital as well as the Analog Clock*/
void CLOCK(clock_attributes attr,const float duration,const char period)
{
	int DT = 0;
	float Diplay_Time = 0;
	
	if (period == 'm' || period == 'M')
		Diplay_Time = duration * 60;
	else if (period == 'h' || period == 'H')
		Diplay_Time = duration * 60 * 60;
	else
		Diplay_Time = duration;
	
	int gd= DETECT, gm;
	initgraph(&gd,&gm,BGI_PATH);
	
	// Setting Default Attributes.
	set_defaults(attr);
	
	// Setting Background color.
	setbkcolor(attr.bgcolor);
	
	if(attr.display_digital) {
		setcolor(attr.text_col);
		outtext("CLOCK:");
	}
	
	// Drawing Rim and Face of Clock.
	setcolor(attr.rim_border_col);
	
	setfillstyle(INTERLEAVE_FILL,attr.rim_color);
	circle(attr.centre.x,attr.centre.y,attr.radius*1.08);
	floodfill(attr.centre.x,attr.centre.y,attr.rim_border_col);
	
	setfillstyle(SOLID_FILL,attr.face_color);
	circle(attr.centre.x,attr.centre.y,attr.radius);
	floodfill(attr.centre.x,attr.centre.y,attr.rim_border_col);
	
	setfillstyle(0,0);  // Reset Default.
	
	// Display Clock Frames: 1. Till Display_Time duration,
	// 2. Till no key has been pressed.
	while (DT < Diplay_Time && !kbhit())
	{
		time_t current_time = time(NULL);
		struct tm *dt = localtime(&current_time);
		
		// Displaying clock Digits & ticks/dashes.
		setcolor(attr.digit_color);
		for(int deg=1;deg<=60;deg++)
		{
			// Displaying Digits.
			if(deg%5==0)
			{
				point digi_pt=get_pt(attr.centre,attr.radius*0.85,deg,6);
				char digit[5];
				int txt_width=NORM_WIDTH;
				if(attr.radius>100)
					txt_width=THICK_WIDTH;
				
				sprintf(digit,"%d",deg/5);
				settextstyle(1,0,txt_width);
				settextjustify(CENTER_TEXT, CENTER_TEXT);
				outtextxy(digi_pt.x,digi_pt.y,digit);
				setlinestyle(0,0,THICK_WIDTH);
			}
			// Displaying dashes.
			point dash_pt1=get_pt(attr.centre,attr.radius*0.94,deg,6);
			point dash_pt2=get_pt(attr.centre,attr.radius*0.98,deg,6);
			line_seg dash;
			dash.a=dash_pt1;
			dash.b=dash_pt2;
			draw_line(dash);
			
			// Resetting Defaults.
			settextstyle(0,0,NORM_WIDTH);
			settextjustify(0,2);
			setlinestyle(0,0,NORM_WIDTH);
		}

		// Getting Coordinates/position of Needles.
		point h_end=get_pt(attr.centre,attr.radius*0.50,get_time_in(HOUR,*dt),30);
		point m_end=get_pt(attr.centre,attr.radius*0.90,get_time_in(MIN,*dt),6);
		point s_strt=get_pt(attr.centre,-attr.radius*0.15,get_time_in(SEC,*dt),6);
		point s_end=get_pt(attr.centre,attr.radius*0.80,get_time_in(SEC,*dt),6);
		 
		line_seg hour_hand,min_hand,sec_hand;
		hour_hand.a=min_hand.a=attr.centre;
		sec_hand.a=s_strt;
		
		hour_hand.b=h_end;
		min_hand.b=m_end;
		sec_hand.b=s_end;
		
		// Drawing Clock Needles.
		setcolor(attr.hour_needle_col);
		setlinestyle(0,0,THICK_WIDTH);
		draw_line(hour_hand);
		setcolor(attr.min_needle_col);
		draw_line(min_hand);
		setcolor(attr.sec_needle_col);
		setlinestyle(0,0,NORM_WIDTH);
		draw_line(sec_hand);
		
		// Drawing Clock Pin.
		setfillstyle(1,attr.pin_col);
		setcolor(attr.pin_border_col);
		circle(attr.centre.x,attr.centre.y,attr.radius*0.02);
		floodfill(attr.centre.x,attr.centre.y,attr.pin_border_col);
		setfillstyle(0,0);
		
		char time[10];
		if(attr.display_digital)
		{
			// Getting digital time in string format.
			if (DT % 2 == 0)
				strftime(time, 10, "%H:%M:%S", dt);
			else
				strftime(time, 10, "%H %M %S", dt);
			
			// Displaying Digital time.
			setcolor(attr.text_col);
			outtextxy(55,0,time);
		}
		
		Pause(1);
		
		// Displaying the same state of clock in background color.
		// It Creates Animation effect.
		if(attr.display_digital) {
			setcolor(attr.bgcolor);
			outtextxy(55,0,time);
		}
		setcolor(attr.face_color);
		setlinestyle(0,0,THICK_WIDTH);
		draw_line(hour_hand);
		draw_line(min_hand);
		setlinestyle(0,0,NORM_WIDTH);
		draw_line(sec_hand);
		
		DT++;   // Incrementing displayed time DT.
	}
	closegraph();
}

void main()
{
	// Initialization is Neccesary.
	clock_attributes clock_attr=initialize();
	
	// Can Provide attributes else defaults selected.
	/*clock_attr.face_color=RED;
	clock_attr.rim_color=BLUE;
	clock_attr.display_digital=0;
	*/
	// Calling CLOCK Function.
	CLOCK(clock_attr,5,'m');
}
