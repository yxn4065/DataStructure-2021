//
// Created by yxn on 2021/12/10.
//由于前面IO流没用到它的精髓,所以本实验我又把它改成C的文件操作了
//项目要求：
//
//1）系统功能：要求设计一个简易的交通咨询系统，可让用户咨询任意两
//个城市之间的最短距离、最低花费或最少时间等问题。
//
//2）对于不同的咨询要求，输入咨询的内容。
//
//3）用户界面的友好性：程序能提供 菜单供用户选择以及相应的交互信息。
//项目目的：
//
//1）图的存储表示及其基本操作实现。
//
//2）最短路径算法。

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <conio.h>

#define   M 40      //最大数据规模
#define   INF 99999 //以99999代表无穷

using namespace std;

/**定义结构体类型存储弧结构*/
typedef struct node
{
    float distance;  //距离
    int vexNode;     //顶点
    struct node *next; //指向下一条弧的指针
} Arcnode;

/**交通图节点元素类型*/
typedef struct vertexnode
{
    char name[30];         //地名
    char information[100]; //相应信息
    Arcnode *head;         //下一个路径
} Line;

/**图的存储结构定义*/
typedef struct adjlist
{
    Line point[M];  //顶点集合
    int arcnum;     //顶点数
    int vexnum;     //弧数
} map;

/**队列结构体用于实现迪杰斯特拉算法*/
typedef struct quene
{
    int father;
    int son;
    struct quene *next;
} Quene;

/**最小生成树节点定义:两点间最短路径*/
typedef struct mst_point
{
    int father;
    int son;
    struct mst_point *next;
} MST_point;

/**初始化矩阵*/
void init_matrix(float (*matrix)[M])
{
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            matrix[i][j] = INF;
        }
    }
}

/**开辟空间存放地点名和地点信息*/
void init_map(map *g)
{
    for (auto &i: g->point)
    {
        //void *memset(void *s,int c,size_t n)
        //总的作用：将已开辟内存空间 s 的首 n 个字节的值设为值 c
        memset(i.name, 0, 30 * sizeof(char));
        memset(i.information, 0, 100 * sizeof(char));
    }
    for (auto &i: g->point)
    {
        i.name[29] = '\n';
        i.information[29] = '\n';
    }
}

/**地图的创建*/
void creat_map(map *g, float (*matrix)[M], FILE *fp)
{
    Arcnode *temp;
    int vexnum;
    int w;
    int vexnode;
    float distance;
    init_map(g);
    cout << "请输入要创建的节点(地点:下同)个数: " << endl;
    cin >> vexnum;
    fprintf(fp, "%d\n", vexnum);
    g->vexnum = vexnum;
    for (int i = 0; i < vexnum; ++i)
    {
        g->point[i].head = (Arcnode *) malloc(sizeof(Arcnode));
        g->point[i].head->next = nullptr;
        printf("\n---正在创建第 %d/%d 个根节点---\n", i + 1, vexnum);
        printf("===>>>请输入第[%d]个根节点的名字:", i + 1);
        cin >> g->point[i].name;
        fwrite(g->point[i].name, 30, 1, fp);
        printf("===>>>请输入地点'%s'根的基本信息\n", g->point[i].name);
        cin >> g->point[i].information;
        fwrite(g->point[i].information, 100, 1, fp);
        printf("\n请问有多少节点连接到根节点'%s': ", g->point[i].name);
        cin >> w;
        fprintf(fp, "%d\n", w);//格式化保存至文件

        for (int j = 0; j < w; ++j)
        {
            printf("**开始创建连接到'%s'的节点,当前: %d/%d 个节点**\n", g->point[i].name, j + 1, w);
            temp = (Arcnode *) malloc(sizeof(Arcnode));
            printf("请输入第%d个连接到 '%s' 的节点对应的数字编号===>>> ", j + 1, g->point[i].name);
            cin >> vexnode;
            printf("请输入该节点到 '%s' 的距离===>>> ", g->point[i].name);
            cin >> distance;
            fprintf(fp, "%d %f\n", vexnode, distance);
            temp->vexNode = vexnode;
            temp->distance = distance;

            temp->next = g->point[i].head->next;
            g->point[i].head->next = temp;

            matrix[i][vexnode] = distance;
            printf("\n地点(根节点):'%s'==>第%d个节点点创建成功!\n", g->point[i].name, j + 1);
        }
        printf("---创建第 %d个节点创建成功,名称:'%s',编号:%d!---\n", i + 1, g->point[i].name, i);
        cout << "//********************************************************//" << endl;
        system("pause");
    }
}

/**显示矩阵存储信息*/
void show_matrix(map *g, float (*matrix)[M])
{
    Arcnode *temp;
    printf("当前共有节点个数: %d\n", g->vexnum);
    printf("\t===矩阵如下===\n");
    for (int i = 0; i < g->vexnum; ++i)
    {
        for (int j = 0; j < g->vexnum; ++j)
        {
            printf("%10.2f  ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n %d \n", g->vexnum);
    for (int i = 0; i < g->vexnum; ++i)
    {
        temp = g->point[i].head->next;
        printf("连接的 '%s' 的地点有:", g->point[i].name);
        while (temp)
        {
            printf(" (%s,距离:%.1f) ", g->point[temp->vexNode].name, temp->distance);
            temp = temp->next;
        }
        printf("\n详细信息为: %s\n\n\n", g->point[i].information);
    }
}

/**初始化队列*/
Quene *init_Quene()
{
    auto *head = (Quene *) malloc(sizeof(Quene));
    head->next = nullptr;
    return head;
}

/**判断队列是否为空*/
int isEmpty(Quene *head)
{
    if (head->next) { return 0; }
    else { return -1; }
}

/**入队操作 插入元素,i:父亲节点   j:孩子节点*/
void push(Quene *head, int i, int j)
{

    auto *temp = (Quene *) malloc(sizeof(Quene));
    if (temp)
    {
        temp->father = i;
        temp->son = j;
        temp->next = head->next;
        head->next = temp;
    }
}

/**出队操作*/
void pop(Quene *head, int &i, int &j)
{
    Quene *temp;
    if (!isEmpty(head))
    {
        temp = head->next;
        i = temp->father;
        j = temp->son;
        head->next = temp->next;
        free(temp);
    }
}

/**显示当前队列*/
void print_Quene(Quene *head)
{
    Quene *temp;
    temp = head->next;
    printf("当前队列如下: \n");
    while (temp)
    {
        printf("父节点:%d ,孩子节点:%d ", temp->father, temp->son);
        temp = temp->next;
    }
}

/**找到两点之间的所有路径*/
void print_rout(map *g, Quene *head, int end)
{
    Quene *temp = head->next;
    printf("两点之间的所有路径如下: \n");
    printf("%s <- ", g->point[end].name);
    while (temp)
    {
        printf("%s <- ", g->point[temp->father].name);
        temp = temp->next;
    }
    printf("\n");
}

/**是否存在下一个节点*/
int get_next(map *g, int &father, int &son)
{
    Arcnode *temp = g->point[father].head->next;
    if (son == -1)
    {
        if (temp)
        {
            son = temp->vexNode;
            return 1;
        }
        else { return 0; }
    }
    else
    {
        while (temp->vexNode != son)
        {
            temp = temp->next;
        }
        if (temp->next)
        {
            son = temp->next->vexNode;
            return 1;
        }
        else { return 0; }
    }
}

/**查看father是否还有son节点,有返回0*/
int continue_find(map *g, int father, int son, int start)
{

    Arcnode *temp = g->point[father].head->next;
    while (temp->vexNode != son)
    {
        temp = temp->next;
    }
    if (temp->next != nullptr) { return 1; }
    else { return 0; }
}

/**求两点间的所有路径*/
void all_routes(map *g, Quene *head, int start, int end)
{
    head = init_Quene();
    int in_stack[M];
    int father = start;
    int son;
    int count = 0;

    for (int &i: in_stack) i = 0;
    if (g->point[start].head->next)
    {
        son = g->point[start].head->next->vexNode;
        push(head, father, son);
        in_stack[start] = 1;
        if (g->point[start].head->next->vexNode == end)
        {
            print_rout(g, head, end);
            count++;
            in_stack[father] = 0;
        }
        else
        {
            father = son;
            son = -1;
        }
    }

    else
    {
        printf("这两个地点暂时不支持直接到达哟~~~ \n");
        return;
    }
    while ((isEmpty(head) == 0) || (continue_find(g, father, son, start)))
    {
        if (get_next(g, father, son))
        {
            if (in_stack[father] == 0)
            {
                if (son != end)
                {
                    push(head, father, son);
                    in_stack[father] = 1;
                    father = son;
                    son = -1;
                }
                else
                {
                    push(head, father, son);
                    in_stack[father] = 1;
                    print_rout(g, head, son);
                    count++;
                }
            }
        }
        else
        {
            pop(head, father, son);
            in_stack[father] = 0;
        }
    }
    printf("总共查询到 %d 种到达方式 \n", count);
}

/**dijkstra算法求两点之间的最短路径*/
void dijkstra(map *g, float (*matrix)[M], int start, int end, int dist[M], int path[M][M + 1])
{
    cout << "正在调用dijkstra算法" << endl;
    int mindist;
    int t, k;
    for (int i = 0; i < g->vexnum; ++i)
    {
        path[i][0] = 0;
    }
    for (int i = 0; i < g->vexnum; ++i)
    {
        for (int j = 1; j < M + 1; ++j)
        {
            path[i][j] = -1;
        }
    }

    for (int i = 0; i < g->vexnum; ++i)
    {
        dist[i] = matrix[start][i];
        if (matrix[start][i] != INF)
        {
            path[i][1] = start;
        }
    }
    path[start][0] = 1;
    for (int i = 1; i < g->vexnum; ++i)
    {
        mindist = INF;
        for (int j = 0; j < g->vexnum; ++j)
        {
            if (!path[j][0] && dist[j] < mindist)
            {
                k = j;
                mindist = dist[j];
            }
        }

        if (mindist == INF)
        {
            printf("暂且未查询这两点间路径!\n");
            return;
        }
        path[k][0] = 1;
        for (int j = 1; j < M; ++j)
        {
            if (!path[j][0] && matrix[k][j] < INF && (dist[k] + matrix[k][j] < dist[j]))
            {
                dist[j] = dist[k] + matrix[k][j];
                t = 1;
                while (path[k][t] != -1)
                {
                    path[j][t] = path[k][t];
                    t++;
                }
                path[j][t] = k;
            }
        }
    }
    printf("%s 与 %s 之间的最短路径为: \n", g->point[start].name, g->point[end].name);
    t = 1;
    while ((k = path[end][t]) != -1)
    {
        printf("%s ->", g->point[k].name);
        t++;
    }
    printf("%s\n", g->point[end].name);
    printf("\n距离为: %d\n", dist[end]);
}

/**显示dijkstra存储的数组示意图*/
void print_dijs_matrix(int *dist, int path[][M + 1])
{
    printf("\t====储存距离的数组如下所示====\n");
    for (int i = 0; i < M; ++i)
    {
        printf("%d ", dist[i]);
    }
    printf("\n\t====存储路径的数组如下所示====\n");
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < M + 1; ++j)
        {
            printf("%d  ", path[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/**Prim算法获取最小生成树*/
MST_point *prim(map *g, float(*matrix)[M], int start)
{
    auto *head = (MST_point *) malloc(sizeof(MST_point));
    MST_point *temp;
    struct
    {
        int adjvex;
        int lowcost;
    } closedge[M];

    int s, min;
    head->next = nullptr;
    closedge[start].lowcost = 0;
    for (int i = 0; i < g->vexnum; ++i)
    {
        if (i != start)
        {
            closedge[i].adjvex = start;
            closedge[i].lowcost = matrix[start][i];
        }
    }

    for (int i = 0; i < g->vexnum - 1; ++i)
    {
        min = INF;
        for (int j = 0; j < g->vexnum; ++j)
        {
            if (closedge[j].lowcost != 0 && closedge[j].lowcost < min)
            {
                s = j;
                min = closedge[j].lowcost;
            }
        }
        temp = (MST_point *) malloc(sizeof(MST_point));
        temp->father = closedge[s].adjvex;
        temp->son = s;
        temp->next = head->next;
        head->next = temp;
        closedge[s].lowcost = 0;

        for (int j = 0; j < g->vexnum; ++j)
        {
            if (j != s && matrix[s][j] < closedge[j].lowcost)
            {
                closedge[j].lowcost = matrix[s][j];
                closedge[j].adjvex = s;
            }
        }
    }
    return head;
}

/**查看最小生成树*/
void print_mst(MST_point *head)
{
    MST_point *temp;
    temp = head->next;
    printf("最小生成树如下: \n");
    while (temp)
    {
        printf("(%d,%d)  ", temp->father, temp->son);
        temp = temp->next;
    }
    printf("\n");
}

/**文件读入*/
void cin_file(map *g, float (*matrix)[M], FILE *fp)
{
    Arcnode *temp;
    int vexnum;
    int w;
    int vexnode;
    float distance;
    fscanf(fp, "%d\n", &vexnum);
    g->vexnum = vexnum;
    for (int i = 0; i < vexnum; ++i)
    {
        g->point[i].head = (Arcnode *) malloc(sizeof(Arcnode));
        g->point[i].head->next = nullptr;
        fread(g->point[i].name, 30, 1, fp);
        fread(g->point[i].information, 100, 1, fp);
        fscanf(fp, "%d\n", &w);
        for (int j = 0; j < w; ++j)
        {
            temp = (Arcnode *) malloc(sizeof(Arcnode));
            fscanf(fp, "%d %f\n", &vexnode, &distance);
            temp->vexNode = vexnode;
            temp->distance = distance;
            temp->next = g->point[i].head->next;
            g->point[i].head->next = temp;
            matrix[i][vexnode] = distance;
        }
    }
    show_matrix(g, matrix);
}

/**节点信息查询模块*/
void point_display(map *g)
{
    int choice;
    printf("\n+++++++++当前可供查询地点信息如下+++++++++    \n");
    for (int i = 0; i < g->vexnum; ++i)
    {
        printf("\t%d : %s\n", i, g->point[i].name);
    }
    cout << "请输入要查询的地点对应的数字: " << endl;
    cout << "=======>>> ";
    cin >> choice;
    printf("查询到 '%s'对应信息如下: \n", g->point[choice].information);
    printf("省会城市: %s\n", g->point[choice].information);
    cout << "\n-----查询结束-----" << endl;
}

/**对应节点选择模块*/
void select_point(map *g)
{
    for (int i = 0; i < g->vexnum; ++i)
    {
        printf("%d: %s\n", i, g->point[i].name);
    }
}

/**显示最短连通路径*/
void display_mst(map *g, MST_point *head)
{
    print_mst(head);
    MST_point *temp;
    temp = head->next;
    printf("查询到的最短连通路径如下:\n");
    while (temp)
    {
        printf("%s --> %s\n", g->point[temp->father].name, g->point[temp->son].name);
        temp = temp->next;
    }
}

/**节点添加*/
void point_add(map *g, float(*matrix)[M], int start, int end, float distance)
{
    auto *temp = (Arcnode *) malloc(sizeof(Arcnode));
    temp->vexNode = end;
    temp->distance = distance;
    temp->next = g->point[start].head->next;
    g->point[start].head->next = temp;
    matrix[start][end] = distance;
}

/**节点撤销*/
void point_revoke(map *g, float(*matrix)[M], int start, int end)
{
    Arcnode *temp = g->point[start].head;
    Arcnode *p;
    while (temp->next->vexNode != end)
    {
        temp = temp->next;
    }
    p = temp->next;
    temp->next = temp->next->next;
    free(p);
    matrix[start][end] = INF;

}

/**根节点的添加*/
void add_point(map *g, float (*matrix)[M], FILE *fp)
{
    Arcnode *temp;
    int vexnum;
    int w;
    int vexnode;
    float distance;
    init_map(g);
    cout << "请输入要添加的节点(地点:下同)个数: " << endl;
    cin >> vexnum;
    fprintf(fp, "%d\n", vexnum);
    g->vexnum = vexnum;
    for (int i = 0; i < vexnum; ++i)
    {
        g->point[i].head = (Arcnode *) malloc(sizeof(Arcnode));
        g->point[i].head->next = nullptr;
        printf("\n---正在添加第 %d/%d 个根节点---\n", i + 1, vexnum);
        printf("===>>>请输入第[%d]个根节点的名字:", i + 1);
        cin >> g->point[i].name;
        fwrite(g->point[i].name, 30, 1, fp);
        printf("===>>>请输入地点'%s'根的基本信息\n", g->point[i].name);
        cin >> g->point[i].information;
        fwrite(g->point[i].information, 100, 1, fp);
        printf("\n请问有多少节点连接到根节点'%s': ", g->point[i].name);
        cin >> w;
        fprintf(fp, "%d\n", w);//格式化保存至文件

        for (int j = 0; j < w; ++j)
        {
            printf("**开始创建连接到'%s'的节点,当前: %d/%d 个节点**\n", g->point[i].name, j + 1, w);
            temp = (Arcnode *) malloc(sizeof(Arcnode));
            printf("请输入第%d个连接到 '%s' 的节点对应的数字编号===>>> ", j + 1, g->point[i].name);
            cin >> vexnode;
            printf("请输入该节点到 '%s' 的距离===>>> ", g->point[i].name);
            cin >> distance;
            fprintf(fp, "%d %f\n", vexnode, distance);
            temp->vexNode = vexnode;
            temp->distance = distance;

            temp->next = g->point[i].head->next;
            g->point[i].head->next = temp;

            matrix[i][vexnode] = distance;
            printf("\n地点(根节点):'%s'==>第%d个节点点创建成功!\n", g->point[i].name, j + 1);
        }
        printf("---添加第 %d个节点创建成功,名称:'%s',编号:%d!---\n", i + 1, g->point[i].name, i);
        cout << "//********************************************************//" << endl;
        system("pause");
    }
    show_matrix(g, matrix);
}

/**主界面菜单*/
void menu_0()
{
    printf("                        交 通 查 询 系 统 V1.0       \n\n ");
    printf("                                   功能页面         \n");
    printf("      0. 地图的基本信息.                             \n");
    printf("      1. 可供查询的地点及其基本信息介绍                \n");
    printf("      2. 查询任意两点间所有的路径                      \n");
    printf("      3. 查询两点间最短路径                           \n");
    printf("      4. 查询从某点出发到其它位置的最短连通路径         \n");
    printf("      5. 路径的扩充                                  \n");
    printf("      6. 路径的撤销                                  \n");
    printf("      7. 新建整张地图                                \n");
    printf("      8. 添加节点                                    \n");
    printf("      9. 删除节点                                    \n");
    printf("      10.退出该系统                                   \n");
    printf("\n\n 根据序号输入你的选择：   ");
}

/**实现主要逻辑*/
int main(int argc, char const *argv[])
{
//    system("chcp 65001");
    system("color e");
    system("cls");
    map g; //初始化图
    Quene *head = nullptr;
    float matrix[M][M]; //创建一个40*40矩阵存放地点信息
    int path[M][M + 1];
    int start, end;
    int dist[M];
    MST_point *Head;
    init_matrix(matrix); //初始化数组
    int choice = 0;
    FILE *fp;
    float distance;
    while (true)
    {
        menu_0();//显示菜单
        cin >> choice;
        if (choice == 10)
        {
            printf("\n\n感谢你的使用,再见...\n\n");
            system("pause");
            system("cls");
            break;
        }//退出该系统
        switch (choice)
        {
            case 0://显示地图的基本信息
            {
                fp = fopen("map.txt", "rt");//t是txt的简写,可以省略不写
                cin_file(&g, matrix, fp);
                fclose(fp);
                cout << "按任意键继续..." << endl;
                while (getch())
                {
                    system("cls");
                    break;
                }
            }
                break;
            case 1://可供查询的地点及其基本信息介绍
            {
                fp = fopen("map.txt", "rt");
                cin_file(&g, matrix, fp);
                fclose(fp);
                point_display(&g);
                cout << "按任意键继续..." << endl;
                while (getch())
                {
                    system("cls");
                    break;
                }
            }
                break;
            case 2://查询任意两点间所有的路径
            {
                fp = fopen("map.txt", "rt");
                cin_file(&g, matrix, fp);
                fclose(fp);
                select_point(&g);
                cout << "====>>>请输入起始点: " << endl;
                cin >> start;
                cout << "====>>>请输入终点: " << endl;
                cin >> end;
                all_routes(&g, head, start, end);
                cout << "按任意键继续..." << endl;
                while (getch())
                {
                    system("cls");
                    break;
                }
            }
                break;
            case 3://查询两点间最短路径
            {
                fp = fopen("map.txt", "rt");
                cin_file(&g, matrix, fp);
                fclose(fp);
                select_point(&g);
                cout << "====>>>请输入起始点: " << endl;
                cin >> start;
                cout << "====>>>请输入终点: " << endl;
                cin >> end;
                dijkstra(&g, matrix, start, end, dist, path);
                cout << "按任意键继续..." << endl;
                while (getch())
                {
                    system("cls");
                    break;
                }
            }
                break;
            case 4://查询从某点出发的最短连通路径
            {
                fp = fopen("map.txt", "rt");
                cin_file(&g, matrix, fp);
                fclose(fp);
                select_point(&g);
                cout << "====>>>请输入要查询的起点: " << endl;
                cin >> start;
                Head = prim(&g, matrix, start);
                display_mst(&g, Head);
                cout << "按任意键继续..." << endl;
                while (getch())
                {
                    system("cls");
                    break;
                }
            }
                break;
            case 5://路径的扩充
            {
                fp = fopen("map.txt", "rt");
                cin_file(&g, matrix, fp);
                select_point(&g);
                fclose(fp);
                cout << "====>>>请输入要扩充的路径编号" << endl;
                cout << "开始节点: ";
                cin >> start;
                cout << "结束节点: ";
                cin >> end;
                cout << "两点间距离距离为: ";
                cin >> distance;
                printf("\n");
                point_add(&g, matrix, start, end, distance);
                cout << "扩充后的地图如下所示:" << endl;
                show_matrix(&g, matrix);
                cout << "按任意键继续..." << endl;
                while (getch())
                {
                    system("cls");
                    break;
                }
            }
                break;
            case 6://路径的撤销
            {
                fp = fopen("map.txt", "rt");
                cin_file(&g, matrix, fp);
                select_point(&g);
                fclose(fp);
                cout << "====>>>请输入需要撤销的路径编号:" << endl;
                cout << "开始节点: ";
                cin >> start;
                cout << "结束节点: ";
                cin >> end;
                printf("\n");
                point_revoke(&g, matrix, start, end);
                cout << "撤销路径后的地图如下所示:" << endl;
                show_matrix(&g, matrix);
            }
                break;
            case 7://新建整张地图
            {
                int yn;
                cout << "!!!!>>> 该操作会覆盖原有文件 <<<!!!!,是否继续?1=是,2=否" << endl;
                cin >> yn;
                if (yn == 2)
                {
                    cout << "正在返回..." << endl;
                    while (getch())
                    {
                        system("cls");
                        break;
                    }
                    break;
                }
                else
                {
                    int password = 0;
                    cout << "请输入管理员密码!!!" << endl;
                    cin >> password;
                    if (password == 123456)
                    {
                        fp = fopen("map.txt", "wt");
                        creat_map(&g, matrix, fp);
                        fclose(fp);
                        show_matrix(&g, matrix);
                        cout << "按任意键继续..." << endl;
                        while (getch())
                        {
                            system("cls");
                            break;
                        }
                    }
                    else
                    {
                        cout << "输入错误,请联系管理员...." << endl;
                    }
                }
            }
                break;
            case 8://添加根节点
            {
                fp = fopen("map.txt", "at");
                cin_file(&g, matrix, fp);
                fclose(fp);
                add_point(&g, matrix, fp);
                show_matrix(&g, matrix);
                fclose(fp);
                cout << "按任意键继续..." << endl;
                while (getch())
                {
                    system("cls");
                    break;
                }
            }
                break;
            case 9://删除根节点
            {
                fp = fopen("map.txt", "rt");
                cin_file(&g, matrix, fp);
                select_point(&g);
                fclose(fp);
                show_matrix(&g, matrix);
                cout << "按任意键继续..." << endl;
                while (getch())
                {
                    system("cls");
                    break;
                }
            }
                break;
            default:printf("输错啦,查无此功能!\n");
                break;
        }
    }
    return 0;
}

