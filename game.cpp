#include "game.h"

Game::Game()
{

}


void Game::startgame(GameType type)
{
    //棋盘上的所有点位清零
    for(int i=0; i<21; i++){
        for(int j=0; j<21; j++){
            board[i][j] = 0;
            boardban[i][j]=0;       //禁手点归零
            board_score[i][j] = 0;
        }
    }
    //禁手归零
    four_ban = 0;
    five_ban = 0;
    banprompt = false;  //默认关闭禁手提示
    startban = true;    //默认开启禁手
    //各种模式
    gametype = type;
}
//判断输赢
GameState Game::win_lose(int row, int col)
{
    int b, flag = 0;
    if(black_white)
        b = 1;
    else b = 0;
    //上左斜"\"
    for(int i=row,j=col; i>=row-5; i--,j--){
        if(i<0||j<0||i>GRID||j>GRID||board[i][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 6){
        if(black_white) return BLACK_WIN;
        else return WHITEWIN;
    }
    //上竖线
    for(int i=row; i>=row-5; i--){
        if(i<0||i>GRID||board[i][col] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 6){
        if(black_white) return BLACK_WIN;
        else return WHITEWIN;
    }
    //上右斜
    for(int i=row,j=col; i>=row-5; i--,j++){
        if(i<0||j<0||i>GRID||j>GRID||board[i][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 6){
        if(black_white) return BLACK_WIN;
        else return WHITEWIN;
    }
    //右横线
    for(int j=col; j<=row+5; j++){
        if(j<0||j>GRID||board[row][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 6){
        if(black_white) return BLACK_WIN;
        else return WHITEWIN;
    }
    //下左斜
    for(int i=row,j=col; i<=row+5; i++,j++){
        if(i<0||j<0||i>GRID||j>GRID||board[i][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 6){
        if(black_white) return BLACK_WIN;
        else return WHITEWIN;
    }
    //下竖线
    for(int i=row; i<=row+5; i++){
        if(i<0||i>GRID||board[i][col] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 6){
        if(black_white) return BLACK_WIN;
        else return WHITEWIN;
    }
    //下右斜
    for(int i=row,j=col; i<=row+5; i++,j--){
        if(i<0||j<0||i>GRID||j>GRID||board[i][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 6){
        if(black_white) return BLACK_WIN;
        else return WHITEWIN;
    }
    //左横线
    for(int j=col; j>=row-5; j--){
        if(j<0||j>GRID||board[row][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 6){
        if(black_white) return BLACK_WIN;
        else return WHITEWIN;
    }
    //和棋
    int i, j;
    for(i=0; i<=GRID; i++){
        int flag1 = 1;
        for(j=0; j<=GRID; j++){
            if(board[i][j] == 0){
                flag1 = 0;
                break;
            }
            if(flag1 == 0)
                break;
        }
    }
    if(i==GRID+1 && j==GRID+1){
        return STALEMATE;
    }
    return NOWINNER;        //没有赢家
}

//决定输赢、(AI)统计分数、找出禁手点位
void Game::checkboard()
{
    //判断禁手
    //遍历每一个空位
    for(int i=0; i<=GRID; i++){
        for(int j=0; j<=GRID; j++){
            //只处理了空位
            if(board[i][j] != 0) continue;
            four_ban = 0;
            five_ban = 0;
            //左斜"\"
            QString str = "";
            for(int ii=i-6, jj=j-6; ii<=i+6; ii++,jj++){
                if(ii < 0 || ii > GRID || jj < 0 || jj > GRID){
                    str += "|";
                }
                else if(ii == i&&jj == j){
                    if(black_white) str += "B";
                    else str += "W";
                }
                else if(board[ii][jj] == -1){
                    str += "w";
                }
                else if(board[ii][jj] == 0){
                    str += " ";
                }
                else if(board[ii][jj] == 1){
                    str += "b";
                }
                else continue;
            }
            //处理左斜"\"
            if(black_white == true && startban == true)
                processforbid(str, j, i);
            if(gametype == MAN_TO_AI){  //AI下棋了
                //关键是要统计该点的分数值
                chessbyai(str, i, j);
            }
            //竖线"|"
            str = "";
            for(int ii=i-6; ii<=i+6; ii++){
                if(ii < 0 || ii > GRID){
                    str += "|";
                }
                else if(ii == i){
                    if(black_white) str += "B";
                    else str += "W";
                }
                else if(board[ii][j] == -1){
                    str += "w";
                }
                else if(board[ii][j] == 0){
                    str += " ";
                }
                else if(board[ii][j] == 1){
                    str += "b";
                }
                else continue;
            }
            //处理"|"
            if(black_white == true && startban == true)
                processforbid(str, j, i);
            //右斜"/"
            str = "";
            for(int ii=i-6, jj=j+6; ii<=i+6; ii++, jj--){
                if(ii < 0 || ii > GRID || jj < 0 || jj > GRID){
                    str += "|";
                }
                else if(ii == i&&jj == j){
                    if(black_white) str += "B";
                    else str += "W";
                }
                else if(board[ii][jj] == -1){
                    str += "w";
                }
                else if(board[ii][jj] == 0){
                    str += " ";
                }
                else if(board[ii][jj] == 1){
                    str += "b";
                }
                else continue;
            }
            //处理"/"
            if(black_white == true && startban == true)
                processforbid(str, j, i);
            //横线"-"
            str = "";
            for(int jj=j-6; jj<=j+6; jj++){
                if(jj < 0 || jj > GRID){
                    str += "|";
                }
                else if(jj == j){
                    if(black_white) str += "B";
                    else str += "W";
                }
                else if(board[i][jj] == -1){
                    str += "w";
                }
                else if(board[i][jj] == 0){
                    str += " ";
                }
                else if(board[i][jj] == 1){
                    str += "b";
                }
                else continue;
            }
            //横线"-"
            if(black_white == true && startban == true)
                processforbid(str, j, i);
            //到此，所有的连线类型处理完毕
            if(i == 19 && j == 5){
            //    qDebug()<<"game->four_ban="<<game->four_ban;
            //    qDebug()<<"board["<<i<<"]["<<j<<"]="<<game->board[i][j]<<endl;
            }
            if(four_ban >1 || five_ban >1){
                boardban[i][j] = true;
            }
            else {
                boardban[i][j] = false;
            }
            //禁手信息录入完毕
        }
    }
}

//处理禁手
void Game::processforbid(QString &str, int col, int row)
//https://blog.csdn.net/kidults/article/details/80075896(QString 用法)
//若是连六就直接赢了，不再构成禁手
{
    int psame[2]={0, 0};
    int pempty[2]={0, 0};
    int jsame[2]={0, 0};
    int jempty[2]={0, 0};

    int t;
    //向左
    for(t=5; t>=0&&str[t]=='b'; psame[0]++,t--);
    for(; t>=0&&str[t]==' '; pempty[0]++,t--);
    for(; t>=0&&str[t]=='b'; jsame[0]++,t--);
    for(; t>=0&&str[t]==' '; jempty[0]++,t--);
    //向右
    for(t=7; t<=12&&str[t]=='b'; psame[1]++,t++);
    for(; t<=12&&str[t]==' '; pempty[1]++,t++);
    for(; t<=12&&str[t]=='b'; jsame[1]++,t++);
    for(; t<=12&&str[t]==' '; jempty[1]++,t++);
    //是否会赢
    if(psame[0]+psame[1]==5){//没有禁手
        return;
    }
    //开始判断禁手
    if(psame[0]+psame[1]==4){
        if(pempty[0]+pempty[1]>0){//五五禁手
            five_ban++;
        }
    }
    if(psame[0]+psame[1]==3){
        //先判断是不是四四禁手
        if(pempty[0]>0&&pempty[1]>0){//两边都要有空位
            if(pempty[0]>1 || pempty[1]>1){//四四禁手
                four_ban++;
            }
        }
        //在判断是不是五五禁手
        else{
            if(pempty[0]==1 && jsame[0]==1){//五五禁手//jsame[0]>1??//?b_bBbb?
                five_ban++;
            }
            else if(pempty[1]==1 && jsame[1]==1){
                five_ban++;
            }
        }
    }
    if(psame[0]+psame[1]==2){
        //先判断四四禁手
        if(pempty[0]>0 && pempty[1]>0){//两边都要有空位
            if(jsame[0]==1 && jempty[0]>0){ //_b_bBb_
                four_ban++;
            }
            else if(jsame[1]==1 && jempty[1]>0){
                four_ban++;
            }
        }
        //在判断五五禁手
        else{
            if(pempty[0]==0 && pempty[1]==1){
                if(jsame[1]==2){
                    five_ban++;
                }
            }
            else if(pempty[1]==0 && pempty[0]==1){
                if(jsame[0]==1){
                    five_ban++;
                }
            }
        }
    }
    if(psame[0]+psame[1]==1){
        //先判断四四禁手
        if(pempty[0]>0 && pempty[1]>0){
            if(jsame[0]==2 && jempty[0]==1){
                four_ban++;
            }
            else if(jsame[1]==2 && jempty[1]==1){
                four_ban++;
            }
        }
        //在判断五五禁手
        else{
            if(pempty[0]==0 && pempty[1]==1){
                if(jsame[1]==3){
                    five_ban++;
                }
            }
            else if(pempty[1]==0 && pempty[0]==1){
                if(jsame[0]==3){
                    five_ban++;
                }
            }
        }
    }
    if(psame[0]+psame[1]==0){
        //先四四
        if(pempty[0]>0 && pempty[1]>0){
            if(pempty[0]==1 && jsame[0]==3){
                if(jempty[0]>1){
                    four_ban++;
                }
                else if(jempty[0]==1 && pempty[1]>1){
                    four_ban++;
                }
            }
            else if(pempty[1]==1 && jsame[1]==3){
                if(jempty[1]>1){
                    four_ban++;
                }
                else if(jempty[1]==1 && pempty[0]>1){
                    four_ban++;
                }
            }
        }
    }
    //至此禁手判断完毕
}

//计算对自己最有优势的情况
//https://github.com/lihongxun945/myblog/issues/14#issue-341420244 （剪枝)
//https://blog.csdn.net/livingsu/article/details/104539741 (评估)
void Game::chessbyai(QString &str, int row, int col)
{
    bool chang = false;
    //进行一次转换
    if(str[6] == 'W'){
        chang = true;
        str[6] == 'B';
        for(int i=0; i<13; i++){
            if(str[i] == 'w'){
                str[i] = 'b';
            }
            else if(str[i] == 'b')
                str[i] = 'w';
        }
    }

    int psame[2]={0, 0};
    int pempty[2]={0, 0};
    int jsame[2]={0, 0};
    int jempty[2]={0, 0};

    int t;
    //向左
    for(t=5; t>=0&&str[t]=='b'; psame[0]++,t--);
    for(; t>=0&&str[t]==' '; pempty[0]++,t--);
    for(; t>=0&&str[t]=='b'; jsame[0]++,t--);
    for(; t>=0&&str[t]==' '; jempty[0]++,t--);
    //向右
    for(t=7; t<=12&&str[t]=='b'; psame[1]++,t++);
    for(; t<=12&&str[t]==' '; pempty[1]++,t++);
    for(; t<=12&&str[t]=='b'; jsame[1]++,t++);
    for(; t<=12&&str[t]==' '; jempty[1]++,t++);

    //开始判断
    if(pempty[0]+pempty[1] == 0){
        if(psame[0]+psame[1]==6)
            board_score[row][col]+=100000;      //可以获胜了
        else board_score[row][col]+=0;
    }
    else{
        //1个
        if(psame[0]+psame[1] == 0){
            if(pempty[0]==0 || pempty[1]==0){   //冲1
                board_score[row][col]++;
            }
            else board_score[row][col]+=2;      //活1
        }
        //2个
        else{}
    }
    //2个
    if(psame[0]+psame[1] == 1){
        if(pempty[0]+pempty[1]>0){}
    }
}
