#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termio.h>

int stage, x, y = 0;
int stage_y[5] = {0};
int stage_x[5] = {0};
int box_count, chest_count = 0;
int char_x, char_y = 0;

char map_pos[5][30][30] = {};
char text;
char user[10];

void gamesave();
void username();
int mapload();
void testmove();
void mapprint();

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

void testmove()
{
    int check=1;
    int input_char;
    int cnt=0;
	int xmv, ymv = 0;

    system("clear\n");
    while(check){
		
		printf("Hello %s\n",user);
        mapprint();
		printf("Move Attempts : %d\n",cnt);

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
	    case 'd' :
		check=0;
                system("clear");
                printf("h(왼쪽),j(아래),k(위),l(오른쪽)\n");
                printf("u(undo)\n");
                printf("r(replay)\n");
                printf("n(new)\n");
                printf("e(exit)\n");
                printf("f(file load)\n");
                printf("d(display help)\n");
                printf("t(top)\n");
		break;
		if (input_char=='d')
			goto move;

            case 'r' :
                mapload();
                cnt=0;
                break;

            case 'e' :
                check=0;
                break;
            default :
                break;
		}
		if (map_pos[0][char_y+ymv][char_x+xmv] == '#')
			;
		else if (map_pos[0][char_y+ymv][char_x+xmv] == '$'){
			if (map_pos[0][char_y+ymv*2][char_x+xmv*2] == '#')
				;
			else{
				map_pos[0][char_y+ymv*2][char_x+xmv*2] = '$';
				map_pos[0][char_y+ymv][char_x+xmv] = '@';
				map_pos[0][char_y][char_x] = '.';
			}
		}
		else{
			map_pos[0][char_y+ymv][char_x+xmv] = '@';
			map_pos[0][char_y][char_x] = '.';
		}
        system("clear");
    }
    printf("\n");
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
    FILE * map = fopen("map.txt","r");
    while (fscanf(map, "%c", &text) != EOF){
        if(text == '1'){
            stage = 0;
            x = 0;
            y = -1;
        }
        else if((text >= '2') && (text <= '5')){
            stage_y[stage] = y+1;
            stage++;
            x = 0;
            y = -1;
        }
        else if(text == '\n'){
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
			box_count++;
			x++;
		}
		else if(text == 'O'){
			map_pos[stage][y][x] = text;
			chest_count++;
			x++;
		}
        else{
            map_pos[stage][y][x] = text;
            x++;
        }
    }
	if (box_count != chest_count){
		printf("박스의 개수와 보관장소의 개수가 같지 않습니다.\n프로그램을 종료합니다.");
		return 1;
	}
    int fclose(FILE * map);
    return 0;
}

void mapprint()
{
	int y_pos, x_pos;
	for (y_pos = 0; y_pos <= stage_y[0]; y_pos++){
		for (x_pos = 0; x_pos <= 30; x_pos++){
			printf("%c",map_pos[0][y_pos][x_pos]);
			if (map_pos[0][y_pos][x_pos] == '@'){
				char_x = x_pos;
				char_y = y_pos;
			}
		}
		printf("\n");
	}
}

void gamesave()
{
	FILE *save = fopen("sokoban.txt","a");
	if(save==NULL)
	{
		printf("save error\n");
		return;
	}
	fprintf(save,"%s",user);
int main()
{
	system("clear");
	username();
	if (mapload() == 1)
		return 0;
	testmove();
	printf("S E E  Y O U  %s . . . .\n",user);
	return 0;
}
