#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termio.h>

int stage, x = 0;
int y = -1;
int stage_y[5] = {0};
int stage_x[5] = {0};
int stage_num = 0;
int box_count[5], chest_count[5] = {0};
int clear_check = 0;
int char_x, char_y = 0;
int chest_pos[5][30][30] = {};
int undo = 5;
int undo_map[5][30][30] = {};
int cnt = 0;

char map_pos[5][30][30] = {};
char text;
char user[10];
char names[5][5][10];
int moves[5][5];

int mapload();
void username();
void mapload_undo();
void move();
void mapprint();
void mapprint_undo();
void clear();
void commandlist();
void rankinglist();
void ranksave();
void rankload();

int getch()
{
        int ch;

        struct termios buf;
        struct termios save;

        tcgetattr(0, &save);
        buf = save;

        buf.c_lflag&=~(ICANON|ECHO);
        buf.c_cc[VMIN] = 1;
        buf.c_cc[VTIME] = 0;

        tcsetattr(0, TCSAFLUSH, &buf);

        ch=getchar();
        tcsetattr(0, TCSAFLUSH, &save);

        return ch;
}

void move()
{
	system("clear");

    int check=1;
    int input_char;
	int xmv, ymv = 0;
	mapload_undo();

    while(check){
		xmv,ymv = 0;
		clear_check = 0;

		printf("Hello %s\n",user);
        
		mapprint();
		if (clear_check == chest_count[stage_num]){
			stage_num++;
			ranksave();
			system("clear");
			cnt = 0;
			undo = 5;
			continue;
		}
		
		if (stage_num >= 5){
			system("clear");
			printf("Congratulation!\n");
			check=0;
			continue;
		}

		printf("Move Attempts : %d\n",cnt);
		printf("%d / 5 Undo\n",undo);

        input_char = getch();
        switch(input_char){
            case 'h' :
                xmv = -1;
				ymv = 0;
                cnt++;
                break;
            case 'j' :
				xmv = 0;
				ymv = 1;
                cnt++;
                break;
            case 'k' :
				xmv = 0;
				ymv = -1;
                cnt++;
                break;
            case 'l' :
				xmv = 1;
				ymv = 0;
                cnt++;
                break;
	    case 't' :
			rankinglist();
			continue;

		break;
	    case 'd' :
				xmv, ymv = 0;
				commandlist();
				continue;
            case 'r' :
				xmv, ymv = 0;
                mapload();
                cnt++;
				system("clear");
                continue;
            case 'e' :
		ranksave();
                check=0;
                break;
			case 'u' :
				if (undo == 0){
					system("clear");
					printf("undo 횟수를 모두 사용하셨습니다.\n");
					continue;
				}
				else{
					mapprint_undo();
					cnt++;
					undo--;
					system("clear");
					continue;
				}
			case '5' :
				stage_num = 4;
				mapload();
				cnt = 0;
				system("clear");
				undo_map[0][0][0] = ' ';
				continue;
			case 'n' :
				stage_num = 0;
				mapload();
				cnt = 0;
				system("clear");
				undo_map[0][0][0] = ' ';
				continue;
			default :
                system("clear");
				continue;
		}
		mapload_undo();
		if (map_pos[stage_num][char_y+ymv][char_x+xmv] == '#')
			;
		else if (map_pos[stage_num][char_y+ymv][char_x+xmv] == '$'){
			if ((map_pos[stage_num][char_y+ymv*2][char_x+xmv*2] == '#') || (map_pos[stage_num][char_y+ymv*2][char_x+xmv*2] == '$'))
				;
			else{
				map_pos[stage_num][char_y+ymv*2][char_x+xmv*2] = '$';
				map_pos[stage_num][char_y+ymv][char_x+xmv] = '@';
				map_pos[stage_num][char_y][char_x] = '.';
			}
		}
		else{
			map_pos[stage_num][char_y][char_x] = '.';
			map_pos[stage_num][char_y+ymv][char_x+xmv] = '@';
		}
		system("clear");
	}
    return;
}


void username()
{
    printf("Start....\ninput name : ");
name :
    while (1){
        scanf("%s",&user);
        if (strlen(user) > 10) {
            printf("Only English, and maximum name is 10 words.\n");
            printf("input name : ");
            continue;
        }
        for (int i = 0; i<strlen(user); i++){
            if (((user[i]>='a')&&(user[i]<='z'))||((user[i]>='A')&&(user[i]<='Z')) ? 0 : 1){
                printf("Only English, and maximum name is 10 words.\n");
				printf("input name : ");
                goto name;
            }
        }
        break;
    }
    return;
}


int mapload()
{
	for (int i = 0; i <= 4; i++){
		box_count[i] = 0;
		chest_count[i] = 0;
	}

    FILE * map = fopen("map.txt","r");
    while (fscanf(map, "%c", &text) != EOF){
        if(text == '1'){
			stage = 0;
			x = 0;
			y = -1;
		}
		else if((text >= '2')&&(text <= '5')){
			stage_y[stage] = y+1;
            stage++;
            x = 0;
            y = -1;
        }
        else if(text == '\n'){
            stage_x[stage] = x-1;
			y++;
            x = 0;
        }
        else if(text == 'e'){
            stage_y[stage] = y+1;
            break;
        }
		else if(text == '@'){
			map_pos[stage][y][x] = text;
			x++;
		}
		else if(text == '$'){
			map_pos[stage][y][x] = text;
			box_count[stage]++;
			x++;
		}
		else if(text == 'O'){
			map_pos[stage][y][x] = text;
			chest_pos[stage][y][x] = text;
			chest_count[stage]++;
			x++;
		}
        else{
            map_pos[stage][y][x] = text;
            x++;
        }
    }
	for (int i = 0; i <= 4; i++){
		if (box_count[i] != chest_count[i]){
			printf("박스의 개수와 보관장소의 개수가 같지 않습니다.\n프로그램을 종료합니다.");
			return 1;
		}
	}

    int fclose(FILE * map);
    return 0;
}

void mapprint()
{
	int y_pos, x_pos;
	for (y_pos = 0; y_pos <= stage_y[stage_num]; y_pos++){
		for (x_pos = 0; x_pos <= stage_x[stage_num]; x_pos++){
			if (map_pos[stage_num][y_pos][x_pos] == '@'){
				char_x = x_pos;
				char_y = y_pos;
			}
			if ((map_pos[stage_num][y_pos][x_pos] == '.')&&(chest_pos[stage_num][y_pos][x_pos] == 'O'))
				map_pos[stage_num][y_pos][x_pos] = 'O';
			if ((map_pos[stage_num][y_pos][x_pos] == '$')&&(chest_pos[stage_num][y_pos][x_pos] == 'O'))
				clear_check++;
			printf("%c",map_pos[stage_num][y_pos][x_pos]);
		}
		printf("\n");
	}
}

void mapload_undo()
{
	int y_pos, x_pos;
    for (y_pos = 0; y_pos <= stage_y[stage_num]; y_pos++){
        for (x_pos = 0; x_pos <= stage_x[stage_num]; x_pos++){
			undo_map[4][y_pos][x_pos] = undo_map[3][y_pos][x_pos];
			undo_map[3][y_pos][x_pos] = undo_map[2][y_pos][x_pos];
			undo_map[2][y_pos][x_pos] = undo_map[1][y_pos][x_pos];
			undo_map[1][y_pos][x_pos] = undo_map[0][y_pos][x_pos];
			undo_map[0][y_pos][x_pos] = map_pos[stage_num][y_pos][x_pos];
		}
    }
}

void mapprint_undo()
{
    int y_pos, x_pos;

    for (y_pos = 0; y_pos <= stage_y[stage_num]; y_pos++){
        for (x_pos = 0; x_pos <= stage_x[stage_num]; x_pos++){
            if (undo_map[0][0][0] != '.')
				;
			else{
				map_pos[stage_num][y_pos][x_pos] = undo_map[0][y_pos][x_pos];
				undo_map[0][y_pos][x_pos] = undo_map[1][y_pos][x_pos];
				undo_map[1][y_pos][x_pos] = undo_map[2][y_pos][x_pos];
				undo_map[2][y_pos][x_pos] = undo_map[3][y_pos][x_pos];
				undo_map[3][y_pos][x_pos] = undo_map[4][y_pos][x_pos];
			}
		}
    }
}

void commandlist()
{
	 system("clear");
	 	printf("Hello %s\n",user);
		printf("\n");
                printf("h(왼쪽),j(아래),k(위),l(오른쪽)\n");
                printf("u(undo)\n");
                printf("r(replay)\n");
                printf("n(new)\n");
                printf("e(exit)\n");
                printf("f(file load)\n");
                printf("d(display help)\n");
                printf("t(top)\n");

		if(getch()=='\n')
			system("clear");
			return;
}
void rankinglist()
{
	char rank;
	int i=0;
	int input=getch();
	FILE *load = fopen("ranking.txt","r");
	if (input=='\n'){
		system("clear");
		while(fscanf(load,"%c",&rank) != EOF)
			printf("%c",rank);
		if (input == '\n')
			return;
		else
			return rankinglist();
	}
	else if (input == ' '){
		switch(input){
			case '1' :
				printf("map 1");
				break;
			case '2' :
				break;
			case '3' :
				break;
			case '4' :
				break;
			case '5' :
				break;
			default :
				printf("wrong input");
				break;
		}
	}
}

void ranksave()
{
	FILE *fp = fopen("ranking.txt", "w");
	for (int i=0; i<5; i++){
		fprintf(fp, "map %d\n",i+1);
		for (int j=0; j<5; j++)
			fprintf(fp, "%s %d move\n",user,cnt);
	}
	fclose(fp);
	return;
}

void rankload()
{
}

int main()
{
	system("clear");
	username();
	if (mapload() == 1)
		return 0;
	move();
	printf("\n");
	printf("S E E  Y O U  %s . . . .\n",user);
	return 0;
}
