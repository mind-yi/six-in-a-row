#include "game.h"
#include <QDebug>

Game::Game()
{
    player1.setchesschoic(true);    //1号默认选黑棋
    player2.setchesschoic(false);
}

void Game::startgame(GameType type, bool ban)
{
    //关于棋盘
        //棋盘上的所有点位清零
    for(int i=0; i<GRID+1; i++){
        for(int j=0; j<GRID+1; j++){
            board[i][j] = 0;
            boardban[i][j]=0;       //禁手点归零
            board_score[i][j] = 0;
        }
    }
        //禁手归零
    four_ban = 0;
    five_ban = 0;
    banprompt = false;  //默认关闭禁手提示
    startban = ban;
        //默认黑子先下
    black_white = true;
    //关于玩家
        //没有赢家
    gamestate = NOWINNER;

    //关于模式
    gametype = type;
    if(type == MAN_TO_AI || type == AI_TO_AI){
        initai();
    }
}

void Game::closegame()
{
    //棋盘上的所有点位清零
    for(int i=0; i<GRID+1; i++){
        for(int j=0; j<GRID+1; j++){
            board[i][j] = 0;
            boardban[i][j]=0;       //禁手点归零
            board_score[i][j] = 0;
        }
    }

    //禁手归零
    four_ban = 0;
    five_ban = 0;
    banprompt = false;  //默认关闭禁手提示
    //没有模式
    //gametype = NOTYPE;

    //玩家的信息也应清零
    player1.closeplayer();
    player2.closeplayer();
}
//判断输赢
GameState Game::win_lose(int row, int col, bool chess)
{
    int b, flag = 0;
    if(chess)
        b = 1;              //黑棋
    else b = 2;             //白棋
    //上左斜"\"
    for(int i=row-1,j=col-1; i>=row-5; i--,j--){
        if(i<0||j<0||i>GRID||j>GRID||board[i][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 5){
        if(chess) return BLACK_WIN;
        else return WHITEWIN;
    }
    //上竖线
    for(int i=row-1; i>=row-5; i--){
        if(i<0||i>GRID||board[i][col] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 5){
        if(chess) return BLACK_WIN;
        else return WHITEWIN;
    }
    //上右斜
    for(int i=row-1,j=col+1; i>=row-5; i--,j++){
        if(i<0||j<0||i>GRID||j>GRID||board[i][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 5){
        if(chess) return BLACK_WIN;
        else return WHITEWIN;
    }
    //右横线
    for(int j=col+1; j<=col+5; j++){
        if(j<0||j>GRID||board[row][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 5){
        if(chess) return BLACK_WIN;
        else return WHITEWIN;
    }
    //下左斜
    for(int i=row+1,j=col+1; i<=row+5; i++,j++){
        if(i<0||j<0||i>GRID||j>GRID||board[i][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 5){
        if(chess) return BLACK_WIN;
        else return WHITEWIN;
    }
    //下竖线
    for(int i=row+1; i<=row+5; i++){
        if(i<0||i>GRID||board[i][col] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 5){
        if(chess) return BLACK_WIN;
        else return WHITEWIN;
    }
    //下右斜
    for(int i=row+1,j=col-1; i<=row+5; i++,j--){
        if(i<0||j<0||i>GRID||j>GRID||board[i][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 5){
        if(chess) return BLACK_WIN;
        else return WHITEWIN;
    }
    //左横线
    for(int j=col-1; j>=col-5; j--){
        if(j<0||j>GRID||board[row][j] != b){
            flag = 0;
            break;
        }
        else flag++;
    }
    if(flag == 5){
        if(chess) return BLACK_WIN;
        else return WHITEWIN;
    }
    flag = 0;
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
            if(b[i][j] == 1 || b[i][j] == 2) continue;
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
                else if(b[ii][jj] == 2){
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
            processforbid(str);

            //竖线"|"
            str = "";
            for(int ii=i-6; ii<=i+6; ii++){
                if(ii < 0 || ii > GRID){
                    str += "|";
                }
                else if(ii == i){
                    str += "B";
                }
                else if(b[ii][j] == 2){
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
            processforbid(str);
            //右斜"/"
            str = "";
            for(int ii=i-6, jj=j+6; ii<=i+6; ii++, jj--){
                if(ii < 0 || ii > GRID || jj < 0 || jj > GRID){
                    str += "|";
                }
                else if(ii == i&&jj == j){
                    str += "B";
                }
                else if(b[ii][jj] == 2){
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
            processforbid(str);
            //横线"-"
            str = "";
            for(int jj=j-6; jj<=j+6; jj++){
                if(jj < 0 || jj > GRID){
                    str += "|";
                }
                else if(jj == j){
                    str += "B";
                }
                else if(b[i][jj] == 2){
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
            processforbid(str);
            //到此，所有的连线类型处理完毕
            if(four_ban >1 || five_ban >1){
                b[i][j] = 4;
            }
            //禁手信息录入完毕
        }
    }
}

//禁手程序有问题！！！
void Game::processforbid(QString &str)
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
GameState Game::chessbyai()
{
    //如果是禁手模式，先检查棋盘
    if(startban == true)
        checkboard(board);
    //
    bool flag = true;
    for(int i=0; i<=GRID; i++){
        for(int j=0; j<=GRID; j++){
            if(board[i][j]!=0){
                flag =false;
                break;
            }
        }
        if(flag == false) break;
    }
    if(flag){
        //添加音效
        QSound::play("E:\\yi\\programme\\qt\\personal programme\\chess.wav");
        board[10][10] = 1;
        //更新落子
        if(gametype == MAN_TO_AI){      //如果机器选黑子
            player1.pointchange(10, 10, 1);
        }
        black_white = 0;
        return NOWINNER;
    }
    int b[GRID+1][GRID+1];
    copyBoard(board, b);
    //先找必胜点（落子必赢点），如果有，就不用在分析了
    if(seekwinpoint(board) == false){
        gamestate = NOWINNER;
        int t = analyse(b, 0, -INTMAX, INTMAX);
        //debug ananlyse函数有问题，对于已经落下的点它还要再落下，这就离谱？？？
        //qDebug() << "经历了分析";
        if(black_white){
            //更新落子
            player1.pointchange(decision.pos.x(), decision.pos.y(), board[decision.pos.x()][decision.pos.y()]);
            board[decision.pos.x()][decision.pos.y()] = 1;
        }
        else{
            //更新落子
            player2.pointchange(decision.pos.x(), decision.pos.y(), board[decision.pos.x()][decision.pos.y()]);
            board[decision.pos.x()][decision.pos.y()] = 2;
        }
    }
    else{
        if(black_white){
            //更新落子
            player1.pointchange(decision.pos.x(), decision.pos.y(), board[decision.pos.x()][decision.pos.y()]);
            board[decision.pos.x()][decision.pos.y()] = 1;
            gamestate = BLACK_WIN;
        }
        else{
            //更新落子
            player2.pointchange(decision.pos.x(), decision.pos.y(), board[decision.pos.x()][decision.pos.y()]);
            board[decision.pos.x()][decision.pos.y()] = 2;
            gamestate = WHITEWIN;
        }
    }
    //添加音效
    QSound::play("E:\\yi\\programme\\qt\\personal programme\\chess.wav");
    //debug
    //qDebug() << "13 6的棋子:"<<board[13][6];
    black_white = !black_white;
    return gamestate;
}

bool Game::seekwinpoint(int (*b)[GRID+1])
{
    int chess = -1;
    int x, y;
    if(black_white) chess = 1;
    else chess = 2;
    GameState state = NOWINNER;
    bool flag = false;
    for(int i=0; i<=GRID; i++){
        for(int j=0; j<=GRID; j++){
            if(b[i][j] == chess){   //找到同类点
                if(i-1>=0 && (b[i-1][j]==0 || (b[i-1][j]==4 && chess==2))){
                    state = win_lose(i-1, j, black_white);
                    if(state == BLACK_WIN || state == WHITEWIN){
                        flag = true;
                        x = i-1;
                        y = j;
                        break;
                    }
                }
                if(i-1>=0 && j+1<=GRID && (b[i-1][j+1]==0 || (b[i-1][j+1]==4 && chess==2))){
                    state = win_lose(i-1, j+1, black_white);
                    if(state == BLACK_WIN || state == WHITEWIN){
                        flag = true;
                        x = i-1;
                        y = j+1;
                        break;
                    }
                }
                if(i-1>=0 && j-1>=0 && (b[i-1][j-1]==0 || (b[i-1][j-1]==4 && chess==2))){
                    state = win_lose(i-1, j-1, black_white);
                    if(state == BLACK_WIN || state == WHITEWIN){
                        flag = true;
                        x = i-1;
                        y = j-1;
                        break;
                    }
                }
                if(i+1<=GRID && (b[i+1][j]==0 || (b[i+1][j]==4 && chess==2))){
                    state = win_lose(i+1, j, black_white);
                    if(state == BLACK_WIN || state == WHITEWIN){
                        flag = true;
                        x = i+1;
                        y = j;
                        break;
                    }
                }
                if(i+1<=GRID && j+1<=GRID && (b[i+1][j+1]==0 || (b[i+1][j+1]==4 && chess==2))){
                    state = win_lose(i+1, j+1, black_white);
                    if(state == BLACK_WIN || state == WHITEWIN){
                        flag = true;
                        x = i+1;
                        y = j+1;
                        break;
                    }
                }
                if(i+1<=GRID && j-1>=0 && (b[i+1][j-1]==0 || (b[i+1][j-1]==4 && chess==2))){
                    state = win_lose(i+1, j-1, black_white);
                    if(state == BLACK_WIN || state == WHITEWIN){
                        flag = true;
                        x = i+1;
                        y = j-1;
                        break;
                    }
                }
                if(j+1<=GRID && (b[i][j+1]==0 || (b[i][j+1]==4 && chess==2))){
                    state = win_lose(i, j+1, black_white);
                    if(state == BLACK_WIN || state == WHITEWIN){
                        flag = true;
                        x = i;
                        y = j+1;
                        break;
                    }
                }
                if(j-1>=0 && (b[i][j-1]==0 || (b[i][j-1]==4 && chess==2))){
                    state = win_lose(i, j-1, black_white);
                    if(state == BLACK_WIN || state == WHITEWIN){
                        flag = true;
                        x = i;
                        y = j-1;
                        break;
                    }
                }
            }
        }
        if(flag) break;
    }
    if(flag){
        decision.pos.setX(x);
        decision.pos.setY(y);
        return true;
    }
    return false;
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
    typeall[2][2][2][2][2][2][4] = WWIN;
    typeall[0][2][2][2][2][2][2] = WWIN;
    typeall[1][2][2][2][2][2][2] = WWIN;
    typeall[3][2][2][2][2][2][2] = WWIN;
    typeall[4][2][2][2][2][2][2] = WWIN;
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
    //黑五五禁手（只统计了一遍）
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
    //黑四四禁手（只统计了一遍）
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
                                            typeall[p1][p2][p3][p4][p5][p6][p7]=BLOCK5;
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
    typeall[0][2][2][2][2][0][1] = FREE4;
    typeall[0][2][2][2][2][0][3] = FREE4;
    typeall[0][2][2][2][2][0][4] = FREE4;
    typeall[0][0][2][2][2][2][0] = FREE4;
    typeall[1][0][2][2][2][2][0] = FREE4;
    typeall[3][0][2][2][2][2][0] = FREE4;
    typeall[4][0][2][2][2][2][0] = FREE4;
    typeall[0][2][0][2][2][2][0] = FREE4;
    typeall[0][2][2][0][2][2][0] = FREE4;
    typeall[0][2][2][2][0][2][0] = FREE4;
    //黑活4
    typeall[0][1][1][1][1][0][0] = free4;
    typeall[0][1][1][1][1][0][2] = free4;
    typeall[0][1][1][1][1][0][3] = free4;
    typeall[0][1][1][1][1][0][4] = free4;
    typeall[0][0][1][1][1][1][0] = free4;
    typeall[2][0][1][1][1][1][0] = free4;
    typeall[3][0][1][1][1][1][0] = free4;
    typeall[4][0][1][1][1][1][0] = free4;
    typeall[0][1][0][1][1][1][0] = free4;
    typeall[0][1][1][0][1][1][0] = free4;
    typeall[0][1][1][1][0][1][0] = free4;
    //白活3
    typeall[0][2][2][2][0][0][0] = FREE3;
    typeall[0][2][2][2][0][1][0] = FREE3;
    typeall[0][2][2][2][0][1][1] = FREE3;
    typeall[0][2][2][2][0][1][2] = FREE3;
    typeall[0][2][2][2][0][1][3] = FREE3;
    typeall[0][2][2][2][0][1][4] = FREE3;
    typeall[0][2][2][2][0][1][1] = FREE3;
    typeall[0][2][2][2][0][3][1] = FREE3;
    typeall[0][2][2][2][0][4][1] = FREE3;

    typeall[0][2][0][2][2][0][0] = FREE3;
    typeall[0][2][0][2][2][0][1] = FREE3;
    typeall[0][2][0][2][2][0][3] = FREE3;
    typeall[0][2][0][2][2][0][4] = FREE3;
    typeall[0][0][2][0][2][2][0] = FREE3;
    typeall[1][0][2][0][2][2][0] = FREE3;
    typeall[3][0][2][0][2][2][0] = FREE3;
    typeall[4][0][2][0][2][2][0] = FREE3;
    typeall[0][2][2][0][2][0][0] = FREE3;
    typeall[0][2][2][0][2][0][1] = FREE3;
    typeall[0][2][2][0][2][0][3] = FREE3;
    typeall[0][2][2][0][2][0][4] = FREE3;
    typeall[0][0][2][2][0][2][0] = FREE3;
    typeall[1][0][2][2][0][2][0] = FREE3;
    typeall[3][0][2][2][0][2][0] = FREE3;
    typeall[4][0][2][2][0][2][0] = FREE3;
    //黑活3
    typeall[0][1][1][1][0][0][0] = free3;
    typeall[0][1][1][1][0][2][0] = free3;
    typeall[0][1][1][1][0][2][1] = free3;
    typeall[0][1][1][1][0][2][3] = free3;
    typeall[0][1][1][1][0][2][4] = free3;
    typeall[0][1][1][1][0][0][2] = free3;
    typeall[0][1][1][1][0][2][2] = free3;
    typeall[0][1][1][1][0][3][2] = free3;
    typeall[0][1][1][1][0][4][2] = free3;

    typeall[0][1][0][1][1][0][0] = free3;
    typeall[0][1][0][1][1][0][2] = free3;
    typeall[0][1][0][1][1][0][3] = free3;
    typeall[0][1][0][1][1][0][4] = free3;
    typeall[0][1][1][0][1][0][0] = free3;
    typeall[0][1][1][0][1][0][2] = free3;
    typeall[0][1][1][0][1][0][3] = free3;
    typeall[0][1][1][0][1][0][4] = free3;
    typeall[0][0][1][1][0][1][0] = free3;
    typeall[2][0][1][1][0][1][0] = free3;
    typeall[3][0][1][1][0][1][0] = free3;
    typeall[4][0][1][1][0][1][0] = free3;
    //白活2
    typeall[0][2][2][0][0][0][0] = FREE2;
    typeall[0][0][2][2][0][0][0] = FREE2;
    typeall[0][0][0][2][2][0][0] = FREE2;
    typeall[0][0][0][0][2][2][0] = FREE2;
    typeall[0][2][0][2][0][0][0] = FREE2;
    typeall[0][0][2][0][2][0][0] = FREE2;
    typeall[0][0][0][2][0][2][0] = FREE2;
    //黑活2
    typeall[0][1][1][0][0][0][0] = free2;
    typeall[0][0][1][1][0][0][0] = free2;
    typeall[0][0][0][1][1][0][0] = free2;
    typeall[0][0][0][0][1][1][0] = free2;
    typeall[0][1][0][1][0][0][0] = free2;
    typeall[0][0][1][0][1][0][0] = free2;
    typeall[0][0][0][1][0][1][0] = free2;
    //白活1
    typeall[0][2][0][0][0][0][0] = FREE1;
    typeall[0][0][2][0][0][0][0] = FREE1;
    typeall[0][0][0][2][0][0][0] = FREE1;
    typeall[0][0][0][0][2][0][0] = FREE1;
    typeall[0][0][0][0][0][2][0] = FREE1;
    //黑活1
    typeall[0][1][0][0][0][0][0] = free1;
    typeall[0][0][1][0][0][0][0] = free1;
    typeall[0][0][0][1][0][0][0] = free1;
    typeall[0][0][0][0][1][0][0] = free1;
    typeall[0][0][0][0][0][1][0] = free1;

    //debug
    //qDebug()<<"block5???:"<<typeall[1][1][1][1][1][0][0]<<endl;
}

//需要模板
//思考要不要考虑杀棋
/*
 算杀:

*/
EVALUATION Game::evaluate(int (*b)[GRID+1])
{
    //权重设计
    //AI下白棋
    //int weight_b[21]={0,1,-2,1,-2,25,-50,25,-50,400,-800,400,-1000,
    //                  5000,-10000,5000,-10000,100000,-500000,1000000,-10000000};
    int weight_b[21]={0,1,-2,2,-3,3,-3,3,-6,10,-20,10,-20,200,-100000,200,-400000,
                     20000,-400000,9000000,-10000000};
    //AI下黑棋
    //int weight_h[21]={0,-2,1,-2,1,-50,25,-50,25,-800,400,-1000,400,
    //                 -10000,5000,-10000,5000,-500000,100000,-10000000,1000000};
    int weight_h[21]={0,-2,1,-3,2,-3,3,-6,3,-20,10,-20,10,-100000,200,-400000,200,
                     -400000,20000,-10000000,9000000};
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
    for(int i=0; i<GRID+1; i++){
        for(int j=0; j<GRID+1; j++){
            B[i+1][j+1] = b[i][j];
        }
    }
    //判断横向棋形
    for(int i=0; i<GRID+1; i++){
        for(int j=0; j<17; j++){
           type = typeall[B[i][j]][B[i][j+1]][B[i][j+2]][B[i][j+3]][B[i][j+4]][B[i][j+5]][B[i][j+6]];
           state[0][type]++;
        }
    }
    //竖向
    for(int j=0; j<GRID+1; j++){
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
    memset(&eval, 0, sizeof(eval));
    int score = 0;
    for(int i=1; i<GRID+1; i++){
        if(black_white == false)
        //对于AI下白棋
            score += (state[0][i]+state[1][i]+state[2][i]+state[3][i])*weight_b[i];
        //对于AI下黑棋
        else
            score += (state[0][i]+state[1][i]+state[2][i]+state[3][i])*weight_h[i];
        int count = state[0][i]+state[1][i]+state[2][i]+state[3][i];
        if(i==BWIN){
            eval.STATE[BWIN] = count;
        }
        if(i==WWIN){
            eval.STATE[WWIN] = count;
        }
        if(i==FREE5){
            eval.STATE[FREE5] = count;
        }
        if(i==free5){
            eval.STATE[free5] = count;
        }
        if(i==BLOCK5){
            eval.STATE[BLOCK5] = count;
        }
        if(i==block5){
            eval.STATE[block5] = count;
        }
        if(i==FREE4){
            eval.STATE[FREE4] = count;
        }
        if(i==free4){
            eval.STATE[free4] = count;
        }
    }
    eval.result = NOWINNER;
    //白赢
    if(eval.STATE[19]>0) eval.result = WHITEWIN;
    //黑赢
    if(eval.STATE[20]>0) eval.result = BLACK_WIN;
    eval.score = score;
    return eval;
}

//该白子下时，为白子找最佳位置
//该黑子下时，为黑子找最佳位置
/*
  在找点时如果发现能置对手于死地的棋，自然其它的点位都不用再考虑了，这样找到了best可能只有1个或
  两个点
*/
POINT Game::seekpoint(int (*b)[GRID+1], bool chesstype) //chesstype: 要为那种子找，1表示敌方，0表示己方
{
    //每次模拟落子前都应检查禁手，如果startban=true;
    if(startban == true){
        checkboard(b);
    }

    POINT best;     //最好的点
    best.cnt = 0;   //初始化最优点的个数，以防它没有bestnum个
    bool B[GRID+1][GRID+1];     //该空位是否被选中
    memset(B, 0, sizeof(B));
    int worth[GRID+1][GRID+1];      //记录点位的价值

    for(int i=0 ;i<=GRID; i++){
        for(int j=0; j<=GRID; j++){        //每个空点附近延伸2个深度，不越界
            if(b[i][j]==1 || b[i][j]==2){       //首先要为非空
                for(int k=-2; k<=2; k++){
                    if(i+k>=0&&i+k<=GRID){
                        if(black_white && !chesstype && startban && b[i+k][j]==4)
                            B[i+k][j] = false;
                        else B[i+k][j] = true;
                        if(j+k>=0&&j+k<=GRID){
                            if(black_white && !chesstype && startban && b[i+k][j+k]==4)
                                B[i+k][j+k] = false;
                            else B[i+k][j+k]=true;
                        }
                        if(j-k>=0&&j-k<=GRID){
                            if(black_white && !chesstype && startban && b[i+k][j-k]==4)
                                B[i+k][j-k] = false;
                            else B[i+k][j-k]=true;
                        }
                    }
                    if(j+k>=0&&j+k<15){
                        if(black_white && !chesstype && startban && b[i][j+k]==4)
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
            if((b[i][j]==0 || b[i][j] == 4) && B[i][j]==true){      //空位或禁手位
                bool flag = false;
                if(chesstype){    //敌方
                    if(black_white){
                        if(b[i][j] == 4) flag = true;
                        b[i][j] = 2;
                    }
                    else{
                        if(b[i][j] == 4){       //模拟落黑子，但如果要落在禁手位，那就跳过
                            continue;
                        }
                        b[i][j] = 1;
                    }
                    worth[i][j] = evaluate(b).score;
                }
                else{
                    if(black_white){
                        if(b[i][j] == 4){       //模拟落黑子，但如果要落在禁手位，那就跳过
                            continue;
                        }
                        b[i][j] = 1;
                    }
                    else{
                        if(b[i][j] == 4) flag = true;
                        b[i][j] = 2;
                    }
                    worth[i][j] = evaluate(b).score;
                }
                best.cnt++;
                if(flag)
                    b[i][j] = 4;    //还原
                else b[i][j] = 0;
            }
        }
    }
    if(best.cnt > bestnum) best.cnt = bestnum;

    int w;
    for(int k=0; k<best.cnt; k++){
        if(chesstype) //如果是敌方
            w = INTMAX;
        else w = -INTMAX;
        for(int i=0; i<=GRID; i++){
            for(int j=0; j<=GRID; j++){
                if(chesstype){
                    if(worth[i][j]<w){
                        w=worth[i][j];
                        QPoint tmp(i, j);
                        best.pos[k] = tmp;
                    }
                }
                else{
                    if(worth[i][j]>w){
                        w=worth[i][j];
                        QPoint tmp(i, j);
                        best.pos[k] = tmp;
                    }
                }
            }
        }
        best.score[k] = w;
        if(chesstype)
            worth[best.pos[k].x()][best.pos[k].y()] = INTMAX;
        else worth[best.pos[k].x()][best.pos[k].y()] = -INTMAX;
    }

    return best;
}

int Game::analyse(int (*b)[GRID+1], int depth, int alpha, int beta)
{
    GameState RESULT = evaluate(b).result;      //对当前的局势进行分析，如果有胜负，就返回了。
    if(depth == 2 || RESULT == BLACK_WIN || RESULT == WHITEWIN){
        if(depth==2){
            POINT p;
            p = seekpoint(b, 0);
            //返回最小值
            return p.score[0];
        }
        else{
            //debug:
            //qDebug() << "有胜负了\n";
            //应该返回的是最小值
            return evaluate(b).score;       //score也是对于下棋者(black_white)的分数
        }
    }
    else if(depth%2==0){    //己方决策层
        //debug:
        //qDebug()<<"己方是:"<<black_white<<endl;
        int sameb[GRID+1][GRID+1];
        copyBoard(b, sameb);
        POINT p= seekpoint(sameb, 0);     //找出bestnum个以内的最优点，如果depth==0,那么就建立了bestnum个以内的博弈树
        //debug:看里面有没有8,8
        //for(int i=0; i<p.cnt; i++){
        //    qDebug()<<"该"<<black_white<<"落子"<<endl;
        //    qDebug()<<p.pos[i].x()<<"   "<<p.pos[i].y()<<endl;
        //}
        for(int i=0; i<bestnum; i++){        //遍历p里面的bestnum个点
            if(black_white)
                sameb[p.pos[i].x()][p.pos[i].y()] = 1;
            else sameb[p.pos[i].x()][p.pos[i].y()] = 2;
            int a=analyse(sameb, depth+1, alpha, beta);
            sameb[p.pos[i].x()][p.pos[i].y()] = 0;      //还原
            //debug:
            //qDebug()<<black_white<<" a= "<<a<<endl;
            if(a > alpha){  //经过分析后的最好下界大于当前的alpha，说明有在该处落子会有更好的结果
                //debug:
                //qDebug()<<"a > alpha了\n";
                alpha = a;
                if(depth==0){   //如果返回到了顶层，记录下bestnum里真正最优点的坐标作为最终的决定
                    decision.pos.setX(p.pos[i].x());
                    decision.pos.setY(p.pos[i].y());
                    decision.evalscore = a;             //返回了最深层对自己最有利的情况
                    //debug:
                    //qDebug()<<"decision的值被改变了\n";
                }
            }
            if(beta <= alpha) break;        //剪枝，返回上一层
            /*如果不剪枝，继续遍历该层剩余的最优点。从bestnum里所有点中挑出可以比当前局势(alpha)
             *还要好的落子点(思考者自然是要为自己争取最大的利益)然后返回给奇数层(敌方决策层)
             *因为敌方也要考虑到你是“理智的”，你会下对自己最有利的点，敌方在自己的决策层里考虑
             *下自己bestnum里那个点才能使得自己的利益最大(局势分值最小)然后通过递归返回给偶数层
             */
        }
        return alpha;       //返回的上一层是奇数层(Min)，所以应当返回最小值alpha
    }
    else{//敌方决策层
        int  sameb[GRID+1][GRID+1];
        copyBoard(b, sameb);
        POINT p = seekpoint(sameb, 1);

        for(int i=0; i<bestnum; i++){
            if(black_white)
                sameb[p.pos[i].x()][p.pos[i].y()] = 2;
            else sameb[p.pos[i].x()][p.pos[i].y()] = 1;
            int a = analyse(sameb, depth+1, alpha, beta);
            //如果depth+1 == 最终层，那么返回处理beta(Min层)
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

