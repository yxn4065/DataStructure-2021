// Created by yxn on 2021/11/10.
//项目要求：
//（1）系统功能：从键盘或文件中读入通讯者信息，通常包括编号、姓名、性
//别、电话以及地址等数据项；可实现通讯者信息的插入、查询、删除、更新以及
//通讯录逆序的输出等功能。
//（2）程序可对输入数据的容错性进行检查，保证数据的合法性，如性别取
//值只能为男或女。
//（3）用户界面的友好性：程序可提供与用户之间合理的交互以及相应的菜
//单供用户选择。
//项目目的：
//（1）完成线性表的存储表示及其基本操作。
//（2）完成文件的打开、关闭操作。
#include <conio.h>
#include <fstream>//文件输入输出
#include <iomanip>//使用setw()函数
#include <iostream>
#include <cstdio>
#include <windows.h>


using namespace std;

/**通讯录节点数据定义*/
typedef struct
{
    string stuid;  //学号
    string name;   //姓名
    string sex;    //性别
    string phone;  //电话号码
    string addr;   //通讯地址
} DataType;

/**节点类型定义*/
typedef struct Node
{
    DataType data;     //节点数据域
    struct Node *next; //节点指针域
} ListNode, *LinkList;

/**类的定义*/
class StudentDirectory
{
public:
    StudentDirectory()      //构造器创建一个空链表
    {
        head = new ListNode;
        head->next = nullptr;
    }

    ~StudentDirectory();        //虚构函数

    void menu_0();              //初始登录页面

    void menu();                //初始登录页面

    void Build();               //建立通讯录

    void Add();                 //添加

    void Check();               //查找

    void Expurgate();           //删除

    void PrintList();           //通讯录输出

    void Cin_file();            //读取文件

    void Preservation_file();   //保存文件

    void D();                   //娱乐

    int ListLength();           //通讯录长度

    void Drop_out();            //退出界面

public:
    LinkList head;//listNode *head
};

/**通讯录链表的建立*/
void StudentDirectory::Build()
{
    system("cls"); //清屏
    fflush(stdin); //清空缓存区
    string StuID;
    bool isFlag = false;
    LinkList p;

    printf("建 立 通 讯 录\n");
    printf("****************************************************************\n");
    printf("请依次输入\t\t学号、\t姓名、 \t性别、\t电话号码、\t联系地址\n");
    printf("请按照此格式进行输入：\t0001\t张三\t男\t1**********\t重庆北碚\n\n");

    while (!isFlag)
    {
        char ch = 'Y';
        cout << "学号(按-1返回菜单)：";
        cin >> StuID;
        if (StuID != "-1")
        {
            p = new ListNode;
            p->data.stuid = StuID;
            printf("\n请输入姓名: ");
            cin >> p->data.name;
            printf("\n请输入性别: ");
            cin >> p->data.sex;
            printf("\n请输入电话: ");
            cin >> p->data.phone;
            printf("\n请输入地址: ");
            cin >> p->data.addr;

            p->next = nullptr;
            //头插法
            p->next = head->next;// 新结点的next保存首结点（头结点的下一个结点）
            head->next = p;// 头指针往前移
            printf("\n*********************\n");
            cout << "是否继续输入学生的通讯录信息？(Y/N):";
            cin >> ch;
            if (ch == 'N' || ch == 'n')
            {
                system("cls");
                isFlag = true;
            }
        }
        else
        {
            system("cls");
            break;
        }
    }
}

/**获取链表长度*/
int StudentDirectory::ListLength()
{
    LinkList p = head->next;
    int n = 0;
    while (p)
    {
        n++;
        p = p->next;
    }
    return n;
}

/**析构函数用于释放空间*/
StudentDirectory::~StudentDirectory()
{
    ListNode *p, *q; //p控制循环,q指向当前删除节点
    p = head;
    q = p->next;
    delete p; //释放空间
    while (q)
    {
        p = q;
        q = p->next;
        delete p;
    }
}

/**通讯录的添加*/
void StudentDirectory::Add()
{
    LinkList p;
    bool isFlag = true;
    while (isFlag)
    {
        p = new ListNode;
        printf("请依次输入\t\t学号、姓名、性别、电话号码、联系地址\n");
        printf("请输入学号: ");
        cin >> p->data.stuid;
        printf("\n请输入姓名: ");
        cin >> p->data.name;
        printf("\n请输入性别: ");
        cin >> p->data.sex;
        printf("\n请输入电话: ");
        cin >> p->data.phone;
        printf("\n请输入地址: ");
        cin >> p->data.addr;

        p->next = nullptr;
        p->next = head->next;//新节点的指针域指向下一个节点
        head->next = p;//头节点的指针指向新节点,新节点插入头节点之后

        printf("\n是否继续添加?(Y/N): ");
        char YN;
        cin >> YN;
        system("cls");
        if (YN == 'Y' || YN == 'y') { isFlag = true; }
        else { isFlag = false; }
    }
}

/**通讯录的查询*/
void StudentDirectory::Check()
{
    LinkList c;
    int i;
    string StuID, NAME;
    c = head->next;
    bool isFlag;
    while (isFlag)
    {
        bool isExist = false;
        if (head->next != nullptr)//链表非空
        {
            printf("\n请选择要查询的方式(【1】学号 【2】姓名): \n");
            printf("=====>>>>退出请输入【0】\n");
            cin >> i;
            if (i == 0)
            {
                isFlag = false;
                system("cls");
            }
            else if (i == 1)
            {
                cout << "请输入学号:" << endl;
                cin >> StuID;
                while (c)
                {
                    if (c->data.stuid == StuID)
                    {
                        isExist = true;
                        printf("查询到的通讯录信息如下: \n");
                        printf("**************************************************************\n");
                        printf("学号\t姓名\t性别   \t电话     \t通信地址\n");
                        printf("**************************************************************\n");
                        cout << c->data.stuid << setw(8)
                             << c->data.name << setw(6)
                             << c->data.sex << setw(16)
                             << c->data.phone << setw(16)
                             << c->data.addr << endl;
                        break;
                    }//找到对应ID
                    c = c->next;
                }
                if (!isExist)
                {
                    cout << ">>>很抱歉,查无此人!<<<" << endl;
                }
            }
            else if (i == 2)
            {
                cout << "请输入姓名:" << endl;
                cin >> NAME;
                while (c)
                {
                    if (c->data.name == NAME)
                    {
                        isExist = true;
                        printf("查询到的通讯录信息如下: \n");
                        printf("**************************************************************\n");
                        printf("学号\t姓名\t性别   \t电话     \t通信地址\n");
                        printf("**************************************************************\n");
                        cout << c->data.stuid << setw(8)
                             << c->data.name << setw(6)
                             << c->data.sex << setw(16)
                             << c->data.phone << setw(16)
                             << c->data.addr << endl;
                        break;
                    }//找到对应名字
                    c = c->next;
                }
                if (!isExist)
                {
                    cout << ">>>很抱歉,查无此人!<<<" << endl;
                }
            }
            else
            {
                printf("输错啦,你个大笨蛋...哎\n");
            }
        }
        else//链表为空
        {
            printf("\n很抱歉，未能找到该学生\n\n");
            printf("先去建个通讯录再进行查找哦~\n");
        }
    }

}

/**通讯录的删除*/
void StudentDirectory::Expurgate()
{
    LinkList p, q, s;
    string StuID;
    bool flag = true;
    int i = 0;
    char YN = 'Y';
    p = head;
    q = p->next;
    while (flag)
    {
        if (!q)//头节点不存在
        {
            cout << "通讯录已为空,按任意键返回主界面...";
            while (getch())
            {
                system("cls");
                return;
            }
        }
        cout << "请输入要删除学生的学号: " << endl;
        cin >> StuID;
        while (q->data.stuid != StuID && q->next != nullptr)
        {
            s = q;
            q = q->next;
        }
        if (StuID == q->data.stuid)
        {
            if (q == p)//删除的是头节点
            {
                p = q->next;
                i++;
                delete p;
            }
            else//一般情况
            {
                s->next = q->next;

                printf("删除学生信息如下: \n");
                printf("**************************************************************\n");
                printf("学号\t姓名\t性别\t电话\t  通信地址\n");
                printf("**************************************************************\n");
                cout << q->data.stuid << "\t"
                     << q->data.name << "\t"
                     << q->data.sex << "\t"
                     << q->data.phone << "\t"
                     << q->data.addr << endl << endl;
                delete q;//释放空节点
                q->next = s;
                i++;
            }

            cout << "删除操作已成功!" << endl;
            if (p)
            {
                printf("\n共成功删除%d条记录...", i);
                cout << "是否继续删除?(Y/N): ";
                cin >> YN;
                system("cls");
                if (YN == 'Y' || YN == 'y')
                {
                    p = head;
                    q = p->next;
                    flag = true;
                }
                else if (YN == 'N' || YN == 'n')
                {
                    system("cls");
                    flag = false;
                }
                else
                {
                    p = head;
                    q = p->next;
                    flag = true;
                    printf("这都输错啦。。。哎\n");
                }
            }
        }
        else
        {
            cout << "小本本都翻遍了" << endl << " 查无此人呐!  " << endl;
            if (p)
            {
                printf("\n共成功删除%d条记录...", i);
                cout << "是否继续删除?(Y/N): ";
                cin >> YN;
                system("cls");
                if (YN == 'Y' || YN == 'y')
                {
                    p = head;
                    q = p->next;
                    flag = true;
                }
                else if (YN == 'N' || YN == 'n')
                {
                    system("cls");
                    flag = false;
                }
                else
                {
                    p = head;
                    q = p->next;
                    flag = true;
                    printf("这都输错啦。。。哎\n");
                }
            }
        }
    }
}

/**该模块本来用于测试,你看到的时候它已经用来娱乐了*/
void StudentDirectory::D()
{
    cout << "你不会以为自己发现了什么新大陆吧!输错啦,傻x!哈哈哈哈哈...." << endl;
    cout << "\n按任意键返回主界面...\n";
    cout << "并没有返回,对吧 哈哈哈哈哈...." << endl;
    Sleep(1000);
    cout << "那就再按一下试试吧...." << endl;
    system("purse");
    while (getch())
    {
        system("cls");
        return;
    }
}

/**通讯录的输出*/
void StudentDirectory::PrintList()
{
    ListNode *p, *q, *s, *Max, *Min, *first;
    int count = 0;
    printf("#—————————————————————————————————————————————#\n");
    printf("|       +++          通讯录全部信息          +++         |\n");
    printf("学号\t姓名\t 性别\t    电话\t      通讯地址\n");
    printf("#—————————————————————————————————————————————#\n");
    if (head->next)
    {
        first = new ListNode;
        s = first;
        while (head->next)
        {
            Min = head->next;
            Max = Min->next;
            q = head;
            while (Max && Min)
            {
                if (Max->data.stuid < Min->data.stuid)
                {
                    Min = Max;
                    Max = Max->next;
                }
                else { Max = Max->next; }
            }
            while (q->next != Min) { q = q->next; }

            q->next = Min->next;
            s->next = Min;
            s = Min;
            s->next = nullptr;
        }
        delete head;
        head = first;
        p = head->next;
        while (p)
        {
            cout << p->data.stuid
                 << setw(8) << p->data.name
                 << setw(8) << p->data.sex
                 << setw(16) << p->data.phone
                 << setw(16) << p->data.addr << endl;
            p = p->next;
            count++;
            if (p)
            {
                printf("|--------------------------------------------------------|\n");
            }
        }
        printf("\n共有 %d 条记录...\n", count - 1);
        printf("#—————————————————————————————————————————————#\n");
        printf("\n按任意键返回主界面....\n");
    }
    else
    {
        printf("|-----                  通讯录为空               -----|\n");
        printf("#—————————————————————————————————————————————#\n");
        printf("\n共有 %d 条记录...\n", count);
        printf("按任意键返回主界面....\n");
    }
    while (char a = getch())
    {
        system("cls");
        return;
    }
}

/**保存文件信息*/
void StudentDirectory::Preservation_file()
{
    ofstream outfile("Address_book.txt", ios::out);//文件写操作 内存写入存储设备
    if (!outfile)//如果文件不存在
    {
        cerr << "Open ERROR!" << endl;
        exit(1);
    }
    LinkList p;
    p = head->next;
    while (p)
    {
        outfile << p->data.stuid
                << setw(8) << p->data.name
                << setw(8) << p->data.sex
                << setw(16) << p->data.phone
                << setw(16) << p->data.addr << endl;
        p = p->next;
    }
    outfile.close();
    cout << "记录已保存..." << "按任意键继续..." << endl;
    while (char a = getch())
    {
        system("cls");
        return;
    }
}

/**导入文件*/
void StudentDirectory::Cin_file()
{
    ifstream inFile;
    inFile.open("Address_book.txt", ios::in); // 以读的方式打开文件
    if (!inFile.is_open())
    {
        printf("打开文件失败!");
        inFile.close();
        system("cls");
        return;
    }
    //当达到文件尾
    while (!inFile.eof())
    { // 当未到文件尾
        auto newNode = new Node(); // 新建结点
        // 从文件中输入数据
        inFile >> newNode->data.stuid >> setw(8)
               >> newNode->data.name >> setw(8)
               >> newNode->data.sex >> setw(16)
               >> newNode->data.phone >> setw(16)
               >> newNode->data.addr;

        newNode->next = head->next; // 新结点的next保存首结点（头结点的下一个结点）
        head->next = newNode; // 头指针往前移
    }
    inFile.close(); // 关闭文件
    printf("\n导入文件已成功，祝您使用愉快！\n\n");
    printf("请按任意键进入页面菜单！\n");
    while (char a = getch())
    {
        system("cls");
        return;
    }
}

/**初始登录页面*/
void StudentDirectory::menu_0()
{
    time_t timep;    //系统时间
    struct tm *p_1;
    time(&timep);
    p_1 = gmtime(&timep);

    printf("\t\t\t\t\t\t  当前时间%02d时%02d分\n", 8 + p_1->tm_hour, p_1->tm_min);
    printf("\t\t\t\t\t\t   %d年%02d月%02d日\n", 1900 + p_1->tm_year, 1 + p_1->tm_mon, p_1->tm_mday);
    printf("\t★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n\n");
    printf("\t★\t\t\t\t\t有朋自远方来，不亦乐乎！ \t\t\t\t\t★\n\n");
    printf("\t★\t\t\t*****************************************************\t\t\t\t★\n\n");
    printf("\t★\t\t\t\t\t Huffman编码/译码程序\t\t\t\t\t\t★\n\n");
    printf("\t★\t\t\t  制作者:*** 专业:智能科学与技术 学号:***************\t\t\t★\n\n");
    printf("\t★\t\t\t*****************************************************\t\t\t\t★\n\n");
    printf("\t★\t\t\t  欢迎使用学生通讯录系统,祝您使用愉快! \t\t\t\t\t\t★\n\n");
    printf("\t★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n\n");
    printf("请输入六位登陆密码！(123456)\n");

    int i = 4;
    while (1)
    {
        int x = 0;
        char c;
        char code[20] = {'1', '2', '3', '4', '5', '6'}, passwoSD[20]; //储存密码
        while (x < 6 && c != 13)
        {
            c = getch();
            putchar('*');
            passwoSD[x] = c;
            x++;
        }
        passwoSD[x] = '\0';
        if (strcmp(passwoSD, code) != 0)
        {
            fflush(stdin);
            printf("\n输入密码有误，您还有%d次机会，请重新输入：", i--);
            if (i == -1)
            {
                cout << "\n登录失败!请联系系统管理员..." << endl;
                Sleep(1500);
                exit(0);
            }
        }
        else { break; }
    }
    Sleep(1000);
    printf("\n通讯录系统加载已成功，祝您使用愉快！\n\n");
    printf("请按任意键进入页面菜单！\n\n");
    system("pause");
    system("cls");    //清屏
}

/**退出界面,打印一个心形图案*/
void StudentDirectory::Drop_out()
{
    int save = 0;
    cout << "是否保存本次运行期间所做的修改?" << endl;
    cout << "【1】是  【2】否" << endl;
    cin >> save;
    if (save == 1) { this->Preservation_file(); }
    system("cls");
    //心形图案
    float y, x, b;
    for (y = 1.5f; y > -1.5f; y -= 0.1f)
    {
        for (x = -1.5f; x < 1.5f; x += 0.05f)
        {
            b = x * x + y * y - 1;
            putchar(b * b * b - x * x * y * y * y <= 0.0f ? '*' : ' ');
        }
        putchar('\n');
    }
    Sleep(1000);  //休眠
    printf("欢迎下次继续使用本产品!\n");
    printf("正在退出，请稍后.");
    Sleep(500);
    printf(".");
    Sleep(500);
    printf(".");
    Sleep(500);
    printf(".");
    Sleep(500);
    printf(".");
    Sleep(500);
    printf(".");
    Sleep(500);
    cout << endl;
}

/**链表的逆序输出*/
void Reverse_output(Node *h)
{//由于时间关系,这里仅对id做了一个简单的逆序输出
    auto *temp = new ListNode;//新建节点
    //遍历输出
    ListNode *current, *next;
    current = h->next;

    while (current != nullptr)
    {
        next = current->next;
        current->next = temp->next;
        temp->next = current;
        current = next;
    }
    h->next = temp->next;
    delete temp;
    while (h->next != nullptr)
    {
        cout << h->next->data.stuid << endl;
        h = h->next;
    }
    system("pause");
    system("cls");
}

/**主要功能菜单显示*/
void StudentDirectory::menu()
{
    time_t timep;
    time(&timep);
    auto p_1 = gmtime(&timep);
    printf("\t\t\t\t\t\t   当前时间%02d时%02d分\n", 8 + p_1->tm_hour, p_1->tm_min);
    printf("\t\t\t\t\t\t    %d年%02d月%02d日\n", 1900 + p_1->tm_year, 1 + p_1->tm_mon, p_1->tm_mday);
    printf("\n");
    printf("\t\t\t\t\t\t    页  面  菜  单\n");
    printf("\n");
    printf("\t\t★————————————————————————————————————————————————★\n\n");
    printf("\t\t\t   | 1.建立通讯录\t\t  2.添加联系人    \t\t3.联系人查找     |\n\n");
    printf("\t\t\t   | 4.删除联系人\t\t  5.打印联系人列表\t\t6.保存已有联系人  |\n\n");
    printf("\t\t\t   | 7.导入联系人信息\t\t  8.逆序输出(测试)  \t\t0.退出系统       |\n\n");
    printf("\t\t★————————————————————————————————————————————————★\n");
    printf("\t\t\t\t\t\t          * \n");
    printf("\t\t\t\t\t\t         *** \n");
    printf("\t\t\t\t\t\t        ***** \n");
    printf("\t\t\t\t\t\t       *******\n");
    printf("\t\t\t\t\t\t      *********\n");
    printf("\t\t\t\t\t\t     *********** \n");
    printf("\t\t\t\t\t\t ********************\n");
    printf("\t\t\t\t\t\t  ******************\n");
    printf("\t\t\t\t\t\t   ******* ********\n");
    printf("\t\t\t\t\t\t  *******   ********\n");
    printf("\t\t\t\t\t\t ******       *******\n");
    printf("\t\t\t\t\t\t******          ******\n");
    printf("请选择相应序号：");
}

/**实现主要逻辑*/
int main(int argc, char const *argv[])
{
//    system("chcp 65001");//乱码的时候把我打开或者改成 chcp 936试试
    system("color b");//更好看点
    StudentDirectory SD;//实例化对象
    int n = 1;
    ofstream file("Address_book.txt", ios::app | ios::binary);
    if (!file)
    {
        cerr << "Open ERROR!" << endl;
        abort();
    }
    file.close();
    SD.Cin_file();
    SD.menu_0();//密码登录界面,没有实用价值
    while (n)
    {
        SD.menu();//显示菜单
        int choose;
        cin >> choose;
//        if (choose == 000) exit(0);//调试用
        system("cls");
        switch (choose)
        {
            case 1:SD.Build();//建立
                break;
            case 2:SD.Add();//添加
                break;
            case 3:SD.Check();//查询
                break;
            case 4:SD.Expurgate();//删除
                break;
            case 5:SD.PrintList();//打印
                break;
            case 6:SD.Preservation_file();//保存
                break;
            case 7:SD.Cin_file();//导入
                break;
            case 8:
            {
                try
                {
                    LinkList head = SD.head;
                    Reverse_output(head);//链表的逆序
                }
                catch (runtime_error e)
                {
                    cout << "当你看到这句话的时候,证明逆序输出(测试)已经出了问题,非常抱歉\n" << flush;
                    system("pause");
                    while (getch())
                    {
                        system("cls");
                        break;
                    }
                    break;
                }
                break;
            }//逆序输出
            case 9:SD.D();//娱乐
                cout << "恭喜你发现新大陆,此时链表长为" << endl << SD.ListLength()
                     << endl << "按任意键返回主界面....." << endl;
                system("pause");//等待用户输出继续
                system("cls");
                break;
            case 0:SD.Drop_out();//打印退出界面
                n = 0;
                printf("======<<<<<<<<<>>>>>>>>>>>>>>>>>>>>=======\n");
                printf("\t感谢你的使用!山水有相逢!\n有缘再会...\n");
                break;
            default:printf("没有该序号！请重新输入：");
        }
    }
    return 0;
}