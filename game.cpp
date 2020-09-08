#include "game.h"
#include <QDebug>

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
    if(type == MAN_TO_AI || type == AI_TO_AI){
        initai();
    }
}
//判断输赢
GameState Game::win_lose(int row, int col)
{
    int b, flag = 0;
    if(black_white)
        b = 1;              //黑棋
    else b = 2;             //白棋
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

//找出禁手点位
//棋盘应为虚拟棋盘
void Game::checkboard(int (*b)[GRID+1])
{
    //每次调用前都应该初始化
    four_ban = 0;
    five_ban = 0;
    //判断禁手
    //遍历每一个空位
    for(int i=0; i<=GRID; i++){
        for(int j=0; j<=GRID; j++){
            //只处理了空位
            if(b[i][j] != 0) continue;
            four_ban = 0;
            five_ban = 0;
            //左斜"\"
            QString str = "";
            for(int ii=i-6, jj=j-6; ii<=i+6; ii++,jj++){
                if(ii < 0 || ii > GRID || jj < 0 || jj > GRID){
                    str += "|";
                }
                else if(ii == i&&jj == j){
                    str += "B";
                }
                else if(b[ii][jj] == -1){
                    str += "w";
                }
                else if(b[ii][jj] == 0){
                    str += " ";
                }
                else if(b[ii][jj] == 1){
                    str += "b";
                }
                else continue;
            }
            //处理左斜"\"
            processforbid(str, j, i);

            //竖线"|"
            str = "";
            for(int ii=i-6; ii<=i+6; ii++){
                if(ii < 0 || ii > GRID){
                    str += "|";
                }
                else if(ii == i){
                    str += "B";
                }
                else if(b[ii][j] == -1){
                    str += "w";
                }
                else if(b[ii][j] == 0){
                    str += " ";
                }
                else if(b[ii][j] == 1){
                    str += "b";
                }
                else continue;
            }
            //处理"|"
            processforbid(str, j, i);
            //右斜"/"
            str = "";
            for(int ii=i-6, jj=j+6; ii<=i+6; ii++, jj--){
                if(ii < 0 || ii > GRID || jj < 0 || jj > GRID){
                    str += "|";
                }
                else if(ii == i&&jj == j){
                    str += "B";
                }
                else if(b[ii][jj] == -1){
                    str += "w";
                }
                else if(b[ii][jj] == 0){
                    str += " ";
                }
                else if(b[ii][jj] == 1){
                    str += "b";
                }
                else continue;
            }
            //处理"/"
            processforbid(str, j, i);
            //横线"-"
            str = "";
            for(int jj=j-6; jj<=j+6; jj++){
                if(jj < 0 || jj > GRID){
                    str += "|";
                }
                else if(jj == j){
                    str += "B";
                }
                else if(b[i][jj] == -1){
                    str += "w";
                }
                else if(b[i][jj] == 0){
                    str += " ";
                }
                else if(b[i][jj] == 1){
                    str += "b";
                }
                else continue;
            }
            //横线"-"
            processforbid(str, j, i);
            //到此，所有的连线类型处理完毕
            if(four_ban >1 || five_ban >1){
                b[i][j] = 4;
            }
            //禁手信息录入完毕
        }
    }
}

//禁手程序有问题！！！
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
void Game::chessbyai()
{
    qDebug() << "AI开始下\n";
    int b[21][21];
    copyBoard(board, b);
    int t = analyse(b, 4, -INTMAX, INTMAX);
    qDebug() << "black_white = "<<black_white<<endl;
    if(black_white){
        board[decision.pos.x()][decision.pos.y()] = 1;
    }
    else board[decision.pos.x()][decision.pos.y()] = 2;
    black_white = !black_white;
}

void Game::initai()
{
    memset(typeall,0,sizeof(typeall));
    //先初始化所有的棋形
    //白赢
    typeall[2][2][2][2][2][2][2] = WWIN;
    typeall[2][2][2][2][2][2][0] = WWIN;
    typeall[2][2][2][2][2][2][1] = WWIN;
    typeall[2][2][2][2][2][2][3] = WWIN;
    typeall[0][2][2][2][2][2][2] = WWIN;
    typeall[1][2][2][2][2][2][2] = WWIN;
    typeall[3][2][2][2][2][2][2] = WWIN;
    //黑赢
    typeall[1][1][1][1][1][1][1] = BWIN;
    typeall[1][1][1][1][1][1][0] = BWIN;
    typeall[1][1][1][1][1][1][2] = BWIN;
    typeall[1][1][1][1][1][1][3] = BWIN;
    typeall[1][1][1][1][1][1][4] = BWIN;
    typeall[0][1][1][1][1][1][1] = BWIN;
    typeall[2][1][1][1][1][1][1] = BWIN;
    typeall[3][1][1][1][1][1][1] = BWIN;
    typeall[4][1][1][1][1][1][1] = BWIN;
    //黑五五禁手
    typeall[4][1][1][1][1][0][0] = block5;
    typeall[4][1][1][1][1][0][2] = block5;
    typeall[4][1][1][1][1][0][3] = block5;
    typeall[1][4][1][1][1][0][0] = block5;
    typeall[1][4][1][1][1][0][2] = block5;
    typeall[1][4][1][1][1][0][3] = block5;
    typeall[1][1][4][1][1][0][0] = block5;
    typeall[1][1][4][1][1][0][2] = block5;
    typeall[1][1][4][1][1][0][3] = block5;
    typeall[1][1][1][4][1][0][0] = block5;
    typeall[1][1][1][4][1][0][2] = block5;
    typeall[1][1][1][4][1][0][3] = block5;
    typeall[1][1][1][1][4][0][0] = block5;
    typeall[1][1][1][1][4][0][2] = block5;
    typeall[1][1][1][1][4][0][3] = block5;
    //黑四四禁手
    typeall[4][1][1][1][0][0][0] = block4;
    typeall[4][1][1][1][0][2][0] = block4;
    typeall[4][1][1][1][0][0][2] = block4;
    typeall[4][1][1][1][0][0][3] = block4;
    typeall[4][1][1][1][0][2][2] = block4;
    typeall[4][1][1][1][0][2][3] = block4;
    typeall[1][4][1][1][0][0][0] = block4;
    typeall[1][4][1][1][0][2][0] = block4;
    typeall[1][4][1][1][0][0][2] = block4;
    typeall[1][4][1][1][0][2][2] = block4;
    typeall[1][4][1][1][0][0][3] = block4;
    typeall[1][4][1][1][0][2][3] = block4;
    typeall[1][1][4][1][0][0][0] = block4;
    typeall[1][1][4][1][0][2][0] = block4;
    typeall[1][1][4][1][0][2][2] = block4;
    typeall[1][1][4][1][0][0][2] = block4;
    typeall[1][1][4][1][0][0][3] = block4;
    typeall[1][1][4][1][0][2][3] = block4;
    typeall[1][1][1][4][0][0][0] = block4;
    typeall[1][1][1][4][0][2][0] = block4;
    typeall[1][1][1][4][0][0][2] = block4;
    typeall[1][1][1][4][0][0][3] = block4;
    typeall[1][1][1][4][0][2][2] = block4;
    typeall[1][1][1][4][0][2][3] = block4;

    //补全棋型
    int p1,p2,p3,p4,p5,p6,p7;
    int lh, lb, rh, rb;
    for(p1=0; p1<4; p1++){
        for(p2=0; p2<3; p2++){
            for(p3=0; p3<3; p3++){
                for(p4=0; p4<3; p4++){
                    for(p5=0; p5<3; p5++){
                        for(p6=0; p6<3; p6++){
                            for(p7=0; p7<4; p7++){
                                lh=rh=lb=rb=0;

                                if(p1==1) lh++;
                                else if(p1==2) lb++;

                                if(p2==1) lh++,rh++;
                                else if(p2==2) lb++,rb++;

                                if(p3==1) lh++,rh++;
                                else if(p3==2) lb++,rb++;

                                if(p4==1) lh++,rh++;
                                else if(p4==2) lb++,rb++;

                                if(p5==1) lh++,rh++;
                                else if(p5==2) lh++,rb++;

                                if(p6==1) lh++,rh++;
                                else if(p6==2) lb++,rb++;

                                if(p7==1) rh++;
                                else if(p7==2) rb++;

                                if(p1==3 || p7==3){//有边界
                                    if(p1==3 && p7!=3){//左边界
                                        //白"冲"5
                                        if(rh==0 && rb==5){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK5;
                                            }
                                        }
                                        //黑"冲"5
                                        if(rh==5 && rb==0){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=block5;
                                            }
                                        }
                                        //白"冲"4
                                        if(rh==0 && rb==4){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK4;
                                            }
                                        }
                                        //黑"冲"4
                                        if(rh==4 && rb==0){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=block4;
                                            }
                                        }
                                        //白眠3
                                        if(rh==0 && rb==3){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK3;
                                            }
                                        }
                                        //黑眠3
                                        if(rh==3 && rb==0){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=block3;
                                            }
                                        }
                                        //白眠2
                                        if(rh==0 && rb==2){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK2;
                                            }
                                        }
                                        //黑眠2
                                        if(rh==2 && rb==0){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=block2;
                                            }
                                        }
                                    }
                                    else if(p3!=3 && p7==3){
                                        //白"冲"5
                                        if(lh==0 && lb==5){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK5;
                                            }
                                        }
                                        //黑"冲"5
                                        if(lh==5 && lb==0){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=block5;
                                            }
                                        }
                                        //白"冲"4
                                        if(lh==0 && lb==4){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK4;
                                            }
                                        }
                                        //黑"冲"4
                                        if(lh==4 && lb==0){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=block4;
                                            }
                                        }
                                        //白眠3
                                        if(lh==0 && lb==3){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK3;
                                            }
                                        }
                                        //黑眠3
                                        if(lh==3 && lb==0){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=block3;
                                            }
                                        }
                                        //白眠2
                                        if(lh==0 && lb==2){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK2;
                                            }
                                        }
                                        //黑眠2
                                        if(lh==2 && lb==0){
                                            if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                                typeall[p1][p2][p3][p4][p5][p6][p7]=block2;
                                            }
                                        }
                                    }
                                }
                                else{//无边界
                                    //白冲5
                                    if((lh==0&&lb==5)||(rh==0&&rb==5)){
                                        if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                            typeall[p1][p2][p3][p4][p5][p6][p7]==BLOCK5;
                                        }
                                    }
                                    //黑冲5
                                    if((lh==5&&lb==0)||(rh==5&&rb==0)){
                                        if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                            typeall[p1][p2][p3][p4][p5][p6][p7]=block5;
                                        }
                                    }
                                    //白冲4
                                    if((lh==0&&lb==4)&&(rh==0&&rb==4)){
                                        if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                            typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK4;
                                        }
                                    }
                                    //黑冲4
                                    if((lh==4&&lb==0)||(rh==4&&rb==0)){
                                        if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                            typeall[p1][p2][p3][p4][p5][p6][p7]=block4;
                                        }
                                    }
                                    //白眠3
                                    if((lh==0&&lb==3)||(rh==0&&rb==3)){
                                        if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                            typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK3;
                                        }
                                    }
                                    //黑眠3
                                    if((lh==3&&lb==0)||(rh==3&&rb==0)){
                                        if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                            typeall[p1][p2][p3][p4][p5][p6][p7]=block3;
                                        }
                                    }
                                    //白眠2
                                    if((lh==0&&lb==2)||(rh==0&&rb==2)){
                                        if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                            typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK2;
                                        }
                                    }
                                    //黑眠2
                                    if((lh==2&&lb==0)||(rh==2&&rb==0)){
                                        if(typeall[p1][p2][p3][p4][p5][p6][p7]==0){
                                            typeall[p1][p2][p3][p4][p5][p6][p7]=block2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //白活5
    typeall[0][2][2][2][2][2][0] = FREE5;
    //黑活5
    typeall[0][1][1][1][1][1][0] = free5;
    //白活4
    typeall[0][2][2][2][2][0][0] = FREE4;
    typeall[0][0][2][2][2][2][0] = FREE4;
    typeall[0][2][0][2][2][2][0] = FREE4;
    typeall[0][2][2][0][2][2][0] = FREE4;
    typeall[0][2][2][2][0][2][0] = FREE4;
    //黑活4
    typeall[0][1][1][1][1][0][0] = free4;
    typeall[0][0][1][1][1][1][0] = free4;
    typeall[0][1][0][1][1][1][0] = free4;
    typeall[0][1][1][0][1][1][0] = free4;
    typeall[0][1][1][1][0][1][0] = free4;
    //白活3
    typeall[0][2][2][2][0][0][0] = FREE3;
    typeall[0][0][2][2][2][0][0] = FREE3;
    typeall[0][0][0][2][2][2][0] = FREE3;
    typeall[0][2][0][2][2][0][0] = FREE3;
    typeall[0][0][2][0][2][2][0] = FREE3;
    typeall[0][2][2][0][2][0][0] = FREE3;
    typeall[0][0][2][2][0][2][0] = FREE3;
    //黑活3
    typeall[0][1][1][1][0][0][0] = free3;
    typeall[0][0][1][1][1][0][0] = free3;
    typeall[0][0][0][1][1][1][0] = free3;
    typeall[0][1][0][1][1][0][0] = free3;
    typeall[0][0][1][0][1][1][0] = free3;
    typeall[0][1][1][0][1][0][0] = free3;
    typeall[0][0][1][1][0][1][0] = free3;
    //白活2
    typeall[0][2][2][0][0][0][0] = free3;
    typeall[0][0][2][2][0][0][0] = free3;
    typeall[0][0][0][2][2][0][0] = free3;
    typeall[0][0][0][0][2][2][0] = free3;
    typeall[0][2][0][2][0][0][0] = free3;
    typeall[0][0][2][0][2][0][0] = free3;
    typeall[0][0][0][2][0][2][0] = free3;
    //黑活2
    typeall[0][1][1][0][0][0][0] = free3;
    typeall[0][0][1][1][0][0][0] = free3;
    typeall[0][0][0][1][1][0][0] = free3;
    typeall[0][0][0][0][1][1][0] = free3;
    typeall[0][1][0][1][0][0][0] = free3;
    typeall[0][0][1][0][1][0][0] = free3;
    typeall[0][0][0][1][0][1][0] = free3;
    //白活1
    typeall[0][2][0][0][0][0][0] = free3;
    typeall[0][0][2][0][0][0][0] = free3;
    typeall[0][0][0][2][0][0][0] = free3;
    typeall[0][0][0][0][2][0][0] = free3;
    typeall[0][0][0][0][0][2][0] = free3;
    //黑活1
    typeall[0][1][0][0][0][0][0] = free3;
    typeall[0][0][1][0][0][0][0] = free3;
    typeall[0][0][0][1][0][0][0] = free3;
    typeall[0][0][0][0][1][0][0] = free3;
    typeall[0][0][0][0][0][1][0] = free3;
}

//需要模板
//思考要不要考虑杀棋
EVALUATION Game::evaluate(int (*b)[21])
{
    //权重设计
    //AI下白棋
    int weight_b[21]={0,1,-2,1,-2,25,-50,25,-50,400,-800,400,-1000,
                      5000,-10000,5000,-10000,100000,-500000,1000000,-10000000};
    //AI下黑棋
    int weight_h[21]={0,-2,1,-2,1,-50,25,-50,25,-800,400,-1000,400,
                     -10000,5000,-10000,5000,-500000,100000,-10000000,1000000};
    int type;
    int state[4][21];
    memset(state, 0, sizeof(state));
    int STATE[21];
    memset(STATE, 0, sizeof(STATE));

    int B[23][23];//多出两行两列
    for(int i=0; i<23; i++){
        B[0][i]=3;
        B[22][i]=3;
        B[i][0]=3;
        B[i][22]=3;
    }
    for(int i=0; i<21; i++){
        for(int j=0; j<21; j++){
            B[i+1][j+1] = b[i][j];
        }
    }
    //判断横向棋形
    for(int i=0; i<21; i++){
        for(int j=0; j<17; j++){
           type = typeall[B[i][j]][B[i][j+1]][B[i][j+2]][B[i][j+3]][B[i][j+4]][B[i][j+5]][B[i][j+6]];
           state[0][type]++;
        }
    }
    //竖向
    for(int j=0; j<21; j++){
        for(int i=0; i<17; i++){
            type = typeall[B[i][j]][B[i+1][j]][B[i+2][j]][B[i+3][j]][B[i+4][j]][B[i+5][j]][B[i+6][j]];
            state[1][type]++;
        }
    }
    //'\'
    for(int i=0; i<17; i++){
        for(int j=0; j<17; j++){
            type = typeall[B[i][j]][B[i+1][j+1]][B[i+2][j+2]][B[i+3][j+3]][B[i+4][j+4]][B[i+5][j+5]][B[i+6][j+6]];
            state[2][type]++;
        }
    }
    //'/'
    for(int i=0; i<17; i++){
        for(int j=6; j<23; j++){
            type = typeall[B[i][j]][B[i+1][j-1]][B[i+2][j-2]][B[i+3][j-3]][B[i+4][j-4]][B[i+5][j-5]][B[i+6][j-6]];
            state[3][type]++;
        }
    }
    EVALUATION eval;
    int score = 0;
    for(int i=1; i<21; i++){
        if(black_white == false)
        //对于AI下白棋
            score += (state[0][i]+state[1][i]+state[2][i]+state[3][i])*weight_b[i];
        //对于AI下黑棋
        else
            score += (state[0][i]+state[1][i]+state[2][i]+state[3][i])*weight_h[i];
        if(i==BWIN){    //黑赢
            eval.result = BLACK_WIN;
        }
        if(i==WWIN){    //白赢
            eval.result = WHITEWIN;
        }
    }
    eval.score = score;
    return eval;
}

//该白子下时，为白子找最佳位置
//该黑子下时，为黑子找最佳位置
//如果有问题，只可能是落子评估那块儿的问题
POINT Game::seekpoint(int (*b)[21], bool chesstype) //chesstype: 要为那种子找，1表示黑，0表示白
{
    POINT best;     //最好的点
    bool B[21][21];     //该空位是否被选中
    memset(B, 0, sizeof(B));
    int worth[21][21];      //记录点位的价值

    for(int i=0 ;i<=GRID; i++){
        for(int j=0; j<=GRID; j++){        //每个空点附近延伸4个深度，不越界
            if(b[i][j]!=0 && b[i][j]!=4){       //首先要为非空
                for(int k=-4; k<=4; k++){
                    if(i+k>=0&&i+k<=GRID){
                        if(chesstype && startban && b[i+k][j]==4)
                            B[i+k][j] = false;
                        else B[i+k][j] = true;
                        if(j+k>=0&&j+k<=GRID){
                            if(chesstype && startban && b[i+k][j+k]==4)
                                B[i+k][j+k] = false;
                            else B[i+k][j+k]=true;
                        }
                        if(j-k>=0&&j-k<=GRID){
                            if(chesstype && startban && b[i+k][j-k]==4)
                                B[i+k][j-k] = false;
                            else B[i+k][j-k]=true;
                        }
                    }
                    if(j+k>=0&&j+k<15){
                        if(chesstype && startban && b[i][j+k]==4)
                            B[i][j+k] = false;
                        else B[i][j+k]=true;
                    }
                }
            }
        }
    }
    //为每个可能落子的点进行落子后的局势分值评估
    for(int i=0; i<=GRID; i++){
        for(int j=0; j<=GRID; j++){
            worth[i][j] = -INTMAX;
            if(b[i][j]==0 && B[i][j]==true){
                if(chesstype){    //黑子
                    b[i][j] = 1;
                    worth[i][j] = evaluate(b).score;
                }
                else{
                    b[i][j] = 2;
                    worth[i][j] = evaluate(b).score;
                }
                b[i][j] = 0;    //还原
            }
        }
    }
    int w;
    for(int k=0; k<10; k++){
        w = -INTMAX;
        for(int i=0; i<=GRID; i++){
            for(int j=0; j<=GRID; j++){
                if(worth[i][j]>w){
                    w=worth[i][j];
                    QPoint tmp(i, j);
                    best.pos[k] = tmp;
                }
            }
        }
        best.score[k] = w;
        worth[best.pos[k].x()][best.pos[k].y()] = -INTMAX;
    }
    return best;
}

int Game::analyse(int (*b)[21], int depth, int alpha, int beta)
{
    GameState RESULT = evaluate(b).result;      //对当前的局势进行分析，如果有胜负，就返回了。
    if(depth == 0 || RESULT == BLACK_WIN || RESULT == WHITEWIN){
        if(depth==0){
            POINT p;
            p = seekpoint(b, black_white);
            return p.score[0];
        }
        else return evaluate(b).score;
    }
    else if(depth%2==0){
        int sameb[21][21];
        copyBoard(b, sameb);
        POINT p= seekpoint(sameb, black_white);

        for(int i=0; i<10; i++){
            if(black_white)
                sameb[p.pos[i].x()][p.pos[i].y()] = 1;
            else sameb[p.pos[i].x()][p.pos[i].y()] = 2;
            int a=analyse(sameb, depth-1, alpha, beta);
            sameb[p.pos[i].x()][p.pos[i].y()] = 0;      //还原
            if(a > alpha){
                alpha = a;
                if(depth==4){
                    decision.pos.setX(p.pos[i].x());
                    decision.pos.setY(p.pos[i].y());
                    decision.evalscore = a;             //返回了最深层对自己最有利的情况
                }
            }
            if(beta <= alpha) break;
        }
        return alpha;
    }
    else{//敌方决策层
        int  sameb[21][21];
        //对吗？？？
        copyBoard(b, sameb);
        POINT p = seekpoint(sameb, !black_white);

        for(int i=0; i<10; i++){
            if(black_white)
                sameb[p.pos[i].x()][p.pos[i].y()] = 2;
            else sameb[p.pos[i].x()][p.pos[i].y()] = 1;
            int a = analyse(sameb, depth-1, alpha, beta);
            sameb[p.pos[i].x()][p.pos[i].y()] = 0;
            if(a < beta) beta = a;
            if(beta<=alpha) break;
        }
        return beta;
    }
}

void Game::copyBoard(int (*s)[GRID+1], int (*d)[GRID+1])
{
    for(int i=0; i<=GRID; i++){
        for(int j=0; j<=GRID; j++){
            d[i][j] = s[i][j];
        }
    }
}

