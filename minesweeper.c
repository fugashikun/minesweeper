#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define SIZE 9
#define NUM_BOMB 10

#define EMPTY 0
#define BOMB 1
#define OPENED 2

void map_init(int map[][SIZE],int coord[][SIZE]);
void cre_map(int map[][SIZE]);
void set_bomb(int coord[][SIZE]);
int coordinate(int map[][SIZE],int coord[][SIZE],int *x_pos,int *y_pos);
void open_cell(int map[][SIZE],int coord[][SIZE],int x_pos,int y_pos);
int bomb_check(int coord[][SIZE],int x_pos,int y_pos);
int game_status(int map[][SIZE],int coord[][SIZE],int xpos,int ypos);
void set_flag(int map[][SIZE],int coord[][SIZE],int x_pos,int y_pos);
void set_question(int map[][SIZE],int coord[][SIZE],int x_pos,int y_pos);
void disp_m(int map[][SIZE],int coord[][SIZE]);

int main(void)
{
  int map[SIZE][SIZE];
  int coord[SIZE][SIZE];
  int xpos,ypos;
  int status;
  int ch;
  int flag = 0;
  
  map_init(map,coord);
  set_bomb(coord);
  
  while(flag==0){
    cre_map(map);
    ch = coordinate(map,coord,&xpos,&ypos);
    switch(ch){
    case 0:
      if(map[ypos][xpos]!='.') status = game_status(map,coord,xpos,ypos);
      break;
    case 1:
      if(map[ypos][xpos]!='.') status = game_status(map,coord,xpos,ypos);
      break;
    default:
      status = game_status(map,coord,xpos,ypos);
      open_cell(map,coord,xpos,ypos);
    }
    if(status==-1 | status==-2){
      disp_m(map,coord);
      flag = 1;
    }
  }
}

void map_init(int map[][SIZE],int coord[][SIZE])
{
  int x,y;
  
  for(y=0;y<SIZE;y++){
    for(x=0;x<SIZE;x++){
      map[y][x] = '.';
      coord[y][x] = EMPTY;
    }
  }
}

void cre_map(int map[][SIZE])
{
  int x,y;
  
  printf("\n  ");
  
  for(x=1;x<SIZE;x++){
    printf("%3c",'@'+x);
  }
  printf("\n------------------------------\n");
  
  for(y=1;y<SIZE;y++){
    printf("%c|",'@'+y);
    for(x=1;x<SIZE;x++){
      printf("%3c",map[y][x]);
    }
    printf("\n");
  }   
}

void set_bomb(int coord[][SIZE])
{
  int count = 0;
  int xpos,ypos;
  
  srand((unsigned int)time(NULL));
  while(count<NUM_BOMB){
    xpos = rand()%(SIZE-1)+1;
    ypos = rand()%(SIZE-1)+1;
    if(coord[ypos][xpos]==EMPTY){
      coord[ypos][xpos] = BOMB;
      count++;
    }
  }
}

int coordinate(int map[][SIZE],int coord[][SIZE],int *x_pos,int *y_pos)
{
  int i = 0;
  int xpos,ypos;
  char input[100];

  while(i == 0){
    i = 1;
    printf("座標入力:");
    scanf("%s",input);
    xpos = input[0] - '@';
    ypos = input[1] - '@';
    if(input[2] != '\0'){
      if(input[2] == 'F'){
	set_flag(map,coord,xpos,ypos);
	*x_pos = xpos;
	*y_pos = ypos;
	return 0;
      }
      else if(input[2] == '?'){
	set_question(map,coord,xpos,ypos);
	*x_pos = xpos;
	*y_pos = ypos;
	return 1;
      }
      else{
	printf("ERROR\n");
	i = 0;
      }
    }
  }
  *x_pos = xpos;
  *y_pos = ypos;

  return 2;
}

void set_flag(int map[][SIZE],int coord[][SIZE],int x_pos,int y_pos)
{
  if(map[y_pos][x_pos]=='F') map[y_pos][x_pos] = '.';
  else map[y_pos][x_pos] = 'F';
}

void set_question(int map[][SIZE],int coord[][SIZE],int x_pos,int y_pos)
{
  if(map[y_pos][x_pos]=='?') map[y_pos][x_pos] = '.';
  else map[y_pos][x_pos] = '?';
}

void open_cell(int map[][SIZE],int coord[][SIZE],int x_pos,int y_pos)
{
  int i;
  int count;
  int x,y;
  int xpos,ypos;

  count = bomb_check(coord,x_pos,y_pos);
  if(coord[y_pos][x_pos]!=BOMB) map[y_pos][x_pos] = count+'0';
  if(count==0){
    for(x=-1;x<=1;x++){
      for(y=-1;y<=1;y++){
	xpos = x_pos+x;
	ypos = y_pos+y;
	if(1<=xpos<=SIZE && 1<=ypos<=SIZE && coord[ypos][xpos]==EMPTY){
	  coord[ypos][xpos] = OPENED;
	  count = bomb_check(coord,xpos,ypos);
	  map[ypos][xpos] = count+'0';
	  if(count==0) open_cell(map,coord,xpos,ypos);
	}
      }
    }
  }	
}

int bomb_check(int coord[][SIZE],int x_pos,int y_pos)
{
  int i;
  int x,y;
  int xpos,ypos;
  int count = 0;
  
  for(y=-1;y<=1;y++){
    for(x=-1;x<=1;x++){
      xpos = x_pos+x;
      ypos = y_pos+y;
      if(1<=xpos<=SIZE && 1<=ypos<=SIZE && coord[ypos][xpos]==BOMB) count++;
    }
  }
  return count;
}

int game_status(int map[][SIZE],int coord[][SIZE],int xpos,int ypos)
{
  int x,y;
  int count = 0;
  if(map[ypos][xpos]!='F' && map[ypos][xpos]!='?' && coord[ypos][xpos]==BOMB){
    printf("GAME OVER\n");
    return -1;
  }
  for(y=1;y<SIZE;y++){
    for(x=1;x<SIZE;x++){
      if(map[y][x]=='.') count++;
    }
  }
  if(count==0){
    printf("CLEAR\n");
    return -2;
  }
}

void disp_m(int map[][SIZE],int coord[][SIZE])
{
  int x,y;
  
  printf("\n  ");
  
  for(x=1;x<SIZE;x++){
    printf("%3c",'@'+x);
  }
  printf("\n------------------------------\n");
  
  for(y=1;y<SIZE;y++){
    printf("%c|",'@'+y);
    for(x=1;x<SIZE;x++){
      if(coord[y][x]==BOMB) map[y][x] = 'M';
      printf("%3c",map[y][x]);
    }
    printf("\n");
  }   
}
