#pragma warning(disable:4996)
#include<stdio.h>
#include<Windows.h>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include<time.h>
#include<string.h>
//#pragma comment(lib, "winmm.lib")
using namespace std;
const static int TILE_LEFT = 0;
const static int TILE_RIGHT = 1;
const static int TILE_TOP = 2;
const static int TILE_BOTTOM =3;
struct TILE{
	bool active;
	bool available;
	char symbol;
	POINT point;
	int cost;
	struct TILE *parent;
	struct TILE *sideTile[4];
};
void insertSortedIntoVector(TILE *t,vector<TILE*>&openList){
	bool val = false;
	for(int i=0;i<openList.size();i++){
		if(openList[i]->cost > t->cost){
			val = true;
			openList.insert(openList.begin()+i,t);
			break;
		}
	}
	if(!val){
		openList.push_back(t);
	}
}
void setupPapan(TILE papan[100][100], vector<POINT> *snakes, POINT food){
	for(int i=0;i<24;i++){
		for(int j=0;j<79;j++){
			papan[i][j].point.x = j;
			papan[i][j].point.y = i;
			papan[i][j].symbol = ' ';
			papan[i][j].available = true;
			papan[i][j].parent = NULL;
			papan[i][j].cost = 999;
			if(i!=0)
				papan[i][j].sideTile[TILE_TOP] = &papan[i-1][j];
			else
				papan[i][j].sideTile[TILE_TOP] = NULL;
			if(i!=23)
				papan[i][j].sideTile[TILE_BOTTOM] = &papan[i+1][j];
			else
				papan[i][j].sideTile[TILE_BOTTOM] = NULL;
			if(j!=0)
				papan[i][j].sideTile[TILE_LEFT] = &papan[i][j-1];
			else
				papan[i][j].sideTile[TILE_LEFT] = NULL;
			if(j!=78)
				papan[i][j].sideTile[TILE_RIGHT] = &papan[i][j+1];
			else
				papan[i][j].sideTile[TILE_RIGHT] = NULL;
		}
	}
	for(int i=0;i<24;i++)for(int j=0;j<79;j++)if(i==0||i==23||j==0||j==78)papan[i][j].symbol='#';
	for(int i=0;i<snakes->size();i++){
		papan[snakes->at(i).y][snakes->at(i).x].symbol='*';
	}
	papan[food.y][food.x].symbol = '*';
	
	for (int i = 2; i < 23; i++){
		if (i%4==0 || (i>7 && i<15 && i%2==0))
			for (int j = 5; j < 70; j++){
				papan[i][j].symbol = '#';
			}
	}
	for (int i = 4; i <= 20; i++){
		papan[i][38].symbol = '#';
	}
}

vector<TILE*> generatePath(POINT start, POINT end, TILE papanOri[100][100], bool &found){
	vector<TILE*>openList;
	openList.push_back(&papanOri[start.y][start.x]);
	openList[0]->cost = 0;
	while(openList.size()!=0 && found == false){
		for(int i=0;i<4;i++){
			if(openList[0]->sideTile[i]!=NULL)
			{
				//printf("%d %d\n",openList[0]->sideTile[i]->point.x, openList[0]->sideTile[i]->point.y);getchar();
				if ((openList[0]->sideTile[i]->symbol == ' '
					|| (openList[0]->sideTile[i]->point.x==start.x && openList[0]->sideTile[i]->point.y==start.y) 
					|| (openList[0]->sideTile[i]->point.x==end.x && openList[0]->sideTile[i]->point.y==end.y))
					&& openList[0]->sideTile[i]->available)
				{
					
					if(openList[0]->sideTile[i]->cost > openList[0]->cost+1)
					{
						openList[0]->sideTile[i]->cost = openList[0]->cost+1;
						openList[0]->sideTile[i]->parent = openList[0];
						insertSortedIntoVector(openList[0]->sideTile[i], openList);
					}
				}
			}
		}
		if(openList[0]->point.x == end.x && openList[0]->point.y == end.y)
		{
			found = true;
			break;
		}
		openList[0]->available = false;
		openList.erase(openList.begin());
	}
	vector<TILE*> backRoad;
	if (found)
	{
		TILE *curr = &papanOri[end.y][end.x];
		while (curr != &papanOri[start.y][start.x])
		{
			backRoad.insert(backRoad.begin(), curr);
			curr = curr->parent;
		}
	}
	return backRoad;
}
int main(){
	//PlaySound("music_mario_world_1-1.wav", NULL, SND_ASYNC);
	srand(time(NULL));
	//splash dikit
	char judul1[100][100] = {
		"  @@@@@@ @@@  @@@  @@@@@@  @@@  @@@ @@@@@@@@",
		" !@@     @@!@!@@@ @@!  @@@ @@!  !@@ @@!     ",
		"  !@@!!  @!@@!!@! @!@!@!@! @!@@!@!  @!!!:!  ",
		"     !:! !!:  !!! !!:  !!! !!: :!!  !!:     ",
		" ::.: :  ::    :   :   : :  :   ::: : :: :::",
		" @@@  @@@ @@@@@@@@  @@@@@@@ @@@@@@@@        ",
		" @@!  !@@ @@!      !@@      @@!             ",
		" @!@@!@!  @!!!:!   !@!      @!!!:!          ",
		" !!: :!!  !!:      :!!      !!:             ",
		"  :   ::: : :: :::  :: :: : : :: :::        "
	};

	int length = strlen(judul1[0]);
	bool stillNotComplete = true;
	int k = 0;
	while (stillNotComplete)
	{
		system("cls");
		stillNotComplete = true;
		for (int i = 0; i < 5; i++)printf("\n");
		for (int i = 0; i < 10; i++){
			for (int j = 0; j < 10; j++)printf(" ");
			for (int j = 0; j < k; j++){
				printf("%c", judul1[i][j]);
			}
			printf("\n");
			if (i == 4)printf("\n\n");
		}
		Sleep(100);
		k++;
		if (k == length)stillNotComplete = false;
	}
	printf("\n\n                    POWER 13-0 KeCe\n");
	printf("          PROVE OUR WILL, EMERGE OUR SPIRIT!!!\n");
	getchar();
	//watch game time
	
	POINT p;p.x = 5;p.y = 20;
	vector<POINT>snake;
	snake.push_back(p);
	POINT food; food.x = 70; food.y = 10;
	int deadLockTrial = 0;
	while(true){
		TILE papan[100][100];
		setupPapan(papan,&snake,food);
		bool found = false;
		vector<TILE*> path = generatePath(snake[0],food,papan, found);
		if (found)deadLockTrial = 0;
		for (int i = 0; i < path.size(); i++)
		{
			papan[path[i]->point.y][path[i]->point.x].symbol = '#';
		}
		if (snake[0].x == food.x && snake[0].y == food.y){
			do{
				food.x = 1 + rand() % 77; food.y = 1 + rand() % 22;
			} while (papan[food.y][food.x].symbol != ' ');
				
			POINT nwTail; 
			nwTail.x = snake[snake.size() - 1].x;
			nwTail.y = snake[snake.size() - 1].y;
			snake.push_back(nwTail);
			for (int i = snake.size() - 2; i > 0; i--)
			{
				snake[i].x = snake[i - 1].x;
				snake[i].y = snake[i - 1].y;
			}
			if (path.size()>0)
			{
				snake[0].x = path[0]->point.x;
				snake[0].y = path[0]->point.y;
			}
		}
		else{
			for (int i = snake.size() - 1; i > 0; i--)
			{
				snake[i].x = snake[i - 1].x;
				snake[i].y = snake[i - 1].y;
			}
			if (path.size()>0)
			{
				snake[0].x = path[0]->point.x;
				snake[0].y = path[0]->point.y;
			}
			else{
				vector<POINT>temp;
				for (int i = snake.size()-1; i >= 0; i--){
					temp.push_back(snake[i]);
				}
				snake = temp;
				deadLockTrial++; 
				
				do{
					setupPapan(papan, &snake, food);
					generatePath(snake[0], food, papan, found);
					if (!found)deadLockTrial++;
					if (deadLockTrial >= 2){
						do{
							food.x = 1 + rand() % 77; food.y = 1 + rand() % 22;
						} while (papan[food.y][food.x].symbol != ' ');
					}
				} while (!found);
			}
		}
		setupPapan(papan, &snake, food);

		//rendering
		char p[100][100];
		for (int i = 0; i < 24; i++)for (int j = 0; j < 79; j++)p[i][j] = papan[i][j].symbol;
		for (int i = 0; i < 24; i++)p[i][79] = '\0';
		for(int i=0;i<24;i++){
			printf("%s\n",p[i]);
		}
		Sleep(20);
		system("cls");
	}
	getchar();
	return 0;
}