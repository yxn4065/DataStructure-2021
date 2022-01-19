// Created by yxn on 2021/12/10.
//项目要求：
//
//1）系统功能：图书采编、图书查询、图书流通和个人信息等功能
//
//2）程序能对输入数据的容错性进行检查，保证数据的合法性。
//
//3）用户界面的友好性：程序可提供菜单供用户选择和相应的交互信息。
//项目目的：
//
//1）集合结构的表示及实现。
//
//2）查找和排序算法的实现。
//
//3）文件的存取操作。

#include <iostream>
#include <fstream>
#include<cstring>
#include <ctime>
#include <utility>
#include <conio.h>
#include <synchapi.h>

using namespace std;

//====================用于注册和登录的类===================//
/**用于注册和登录的类*/
class Syuser
{
    /**用于实现注册和登录功能的结构体*/
    typedef struct syuser
    {
        char userid[12];    //该系统默认学号和手机号都是11位的,方便操作
        char password[8];
        struct syuser *next;
    } User;
public:
    static bool check_userid(User *user);

    static bool checkUserValid(User *user);

    bool check_login_success = false;//在欢迎标题显示用户名称时会用到
    char user_number[12]{};            //在欢迎标题显示用户账号名时会用到

    void welcome_title();

    int create_user();

    int login();
};

/**检测用户账号是否已被注册:没被注册过返回true*/
bool Syuser::check_userid(User *user)
{
    int check = true;
    char usrno[12];
    strcpy(usrno, user->userid);
    char usrno_f[12];
    FILE *fp;
    fp = fopen("user.txt", "r+");
    if (fp == nullptr) fopen("user.txt", "w");
    while (fscanf(fp, "%s%*s", usrno_f) != EOF)
    {
        if (strcmp(usrno, usrno_f) == 0)
        {
            check = false;
            break;
        }
    }
    if (fclose(fp))
    {
        printf("Can not close the file!\n");
        exit(0);
    }
    return check;
}

/**登录时用于检测用户合法性:正确返回true*/
bool Syuser::checkUserValid(User *user)
{

    //打开存储用户账号密码的文件user.txt
    FILE *user_file_p;
    user_file_p = fopen("user.txt", "r");
    if (user_file_p == nullptr)
    {
        user_file_p = fopen("user.txt", "w");
    }
    char usr[30], pwd[10];  //usr[30]是用户即时输入的，user->userid和user->password都由本函数的形参user将值传入
    char usr_f[30];  //usr_f[30]是从user.txt里读取的（用户账号+空格+密码+\n）
    bool check = false;
    //将(用户名、空格、密码、\n)连接成新字符串usr，用于在user.txt文件中检查匹配
    strcpy(usr, user->userid);
    strcat(usr, " ");
    strcpy(pwd, user->password);
    strcat(usr, pwd);
    strcat(usr, "\n");
    //从文件中读入用户信息数据，遍历判断用户即时输入的和user.txt里的（用户账号+空格+密码+\n）是否匹配
    while (feof(user_file_p) == 0)
    {
        fgets(usr_f, 30, user_file_p); //从流中读一行或指定个字符

        if (strcmp(usr, usr_f) == 0)
        {
            cout << "check = true" << endl;
            check = true;
            break;
        }
    }
    //关闭文件
    if (fclose(user_file_p))
    {
        printf("Can not close the file!\n");
        exit(0);
    }
    return check;
}

/**欢迎标题*/
void Syuser::welcome_title()
{
    time_t time_login;
    struct tm *p;
    time(&time_login);
    p = gmtime(&time_login);

    if (!check_login_success)
    {
        printf("--------------------------------------------------------------------------------\n");
        printf("欢迎您，亲爱的用户！\n");
        cout << "\t              ╔══════════════════════════╗" << endl;
        cout << "\t              ║    欢迎进入图书管理系统  ║" << endl;
        cout << "\t              ╚══════════════════════════╝" << endl;
        printf("                        现在时间：%d年%d月%d日 %d:%d:%d\n", 1900 + p->tm_year,
               1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
        printf("                                                                                \n");
        printf("                              Author :---    ***************          \n");
        printf("                                                                                \n");
        printf("--------------------------------------------------------------------------------\n");
    }
    else
    {
        printf("--------------------------------------------------------------------------------\n");
        printf("欢迎您，%s！\n", user_number);
        cout << "\t              ╔══════════════════════════════╗" << endl;
        cout << "\t              ║    欢迎进入图书管理系统V1.0  ║" << endl;
        cout << "\t              ╚══════════════════════════════╝" << endl;
        printf("                        现在时间：%d年%d月%d日 %d:%d:%d\n", 1900 + p->tm_year,
               p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
        printf("                                                                                \n");
        printf("                              Author :---    ***************          \n");
        printf("                                                                                \n");
        printf("--------------------------------------------------------------------------------\n");
    }

}

/**注册*/
int Syuser::create_user()
{
    User user;//创建user变量并分配空间
    FILE *fp;
    fp = fopen("user.txt", "a"); //追加方式打开文件
    //若user.txt不存则创建user.txt文件
    if (fp == nullptr) fopen("user.txt", "w"); //只写方式打开文件
a:
    system("cls");
    //打印注册的提示信息
    printf("\n");
    printf("===================================注册新用户===================================");
    printf("\n\n");
    printf("  【温馨提醒:】\n\n");
    printf("  【用户账号】请输入11位手机号,若注册过请直接登录\n\n");
    printf("  【密码】长度<=8位，仅允许使用英文字符或数字，不允许空格\n\n");
    printf("           p.s.为保护您的隐私，输入的密码不会显示在屏幕上\n\n");
    printf("================================================================================");
    printf("\n\n");
    printf("==========>>>>>>请输入新的手机号，按回车结束：");
    scanf("%s", user.userid);
    printf(user.userid);
    printf("\n");

    //判断输入的用户账号是否被注册过
    if (this->check_userid(&user))
    {
        //提示输入并接收用户首次输入的密码数组user.password[i]
        printf("==========>>>>>>请输入新的密码，按回车结束：");
        int i = 0;
        while (0 < i < 8 && (user.password[i] = getch()) != '\r')//当用户输完密码并确认（按下了Enter键）
        {
            //用户在输入密码的过程中有删除行为（按下了backspace退格键）
            if (user.password[i] == '\b')//user.password[i]数组长度减少i位
            {
                if (i <= 0) {}
                else { i--; }
            }
                //正常输入密码
            else
            {
                i++;
                //输入的密码超出了8位，提示超出8位，并且自动删除超出8位后的输入，只取前八位
                if (i > 8)
                {
                    printf("\n           注意：已经达到8位字符上限，只有前8位键入的字符有效！\n");
                    i = i - 1;
                }
            }
        }
        user.password[i] = '\0';//插入字符串结尾符至user.password[i]数组

        //提示并接收用户再次输入的密码 check_password[8]
        char check_password[8];
        printf("\n\n==========>>>>>>请再次输入密码，按回车结束：");
        i = 0;
        for (i = 0; i < 8; i++)
        {
            check_password[i] = '\0';
        }
        i = 0;
        while (0 < i < 8 && (check_password[i] = getch()) != '\r')
        {
            if (check_password[i] == '\b')
            {
                if (i <= 0) {}
                else { i--; }
            }
            else
            {
                i++;
                if (i > 8)
                {
                    printf("\n       注意：已经达到8位字符上限，只有前8位键入的字符有效哦\n");
                    i = i - 1;
                }
            }
        }
        check_password[i] = '\0';
        printf(user.userid);

        //比较user.password[i]和check_password[8]
        if (strcmp(check_password, user.password) == 0) {}
        else
        {
            printf("\n\n========>>!!!!!!!两次密码输入不一致！请重新注册...\n");
            Sleep(1000);
            goto a;
        }

        printf(user.userid);
        //将user变量的值(user.userid和user.password)格式化输出到user.txt文件
        fprintf(fp, "%s ", user.userid);
        fprintf(fp, "%s", user.password);
        fprintf(fp, "\n");
        fclose(fp);
        //打印注册成功的提示信息
        printf("\n\n\n\n");
        printf("注册成功!请记住您的用户名：%s", user.userid);
        printf("，还有请牢记您的密码！\n\n");
        //按下任意键清屏并且返回首页主菜单
        printf("\n\n\n\n按任意键返回主菜单，选登录即可");
        system("pause");
        system("cls");
        return 1;
    }
    else
    {
        printf("%s 这个手机号被注册过了哦~\n", user.userid);
        cout << "是否继续注册?[1]是 [2]否:" << endl;
        int ch;
        cin >> ch;
        if (ch == 1) { goto a; }
        else { return 0; }
    }
}

/**登录*/
int Syuser::login()
{
    User user;
    int check = 0;
    system("cls");
    this->welcome_title();//打印主标题
    //提示输入并接收su.userid
    printf("\n\n=======>>>>>>请输入11位手机号，按回车键结束：");
    scanf("%s", user.userid);// TODO 后面文件的写入user.id没有写进去,未知原因
    printf("\n\n");
    //提示输入并接收su.password
    printf("                       请输入小于等于8位的密码\n\n");
    printf("         该密码输入时不会显示在屏幕上，输完时请按回车键结束哦\n\n");
    printf("======================>>>>>>>>>请输入：");
//    scanf("%s", user.password);
    int i = 0;
    while ((0 < i < 8) && (user.password[i] = getch()) != '\r')
    {
        if (user.password[i] == '\b')
        {
            if (i <= 0) {}
            else { i--; }
        }
        else
        {
            i++;
            if (i > 8)
            {
                printf("\n===>>!!!!注意：已经达到8位字符上限，本次键入的字符将无效！\n\n");
                i = i - 1;
            }
        }
    }
    user.password[i] = '\0';
    //检测用户合法性
    // TODO 这里弄反了,但是没办法,问题出在文件操作相关,还没来得及改
    if (!this->checkUserValid(&user))
    {
        strcpy(user_number, user.userid);
        check = 1;
        printf("\n\n\n");
        printf("\t\t<<<<<<<<<<<<<登录成功!>>>>>>>>>>\n");
        printf("\n");
        printf("按任意键进入主菜单...");
        char ch = getch();
        system("cls");
    }
    else
    {
        printf("\n\n\n");
        printf("======!!!!该用户不存在或密码错误，请核对后重新登录，或注册个新账号！\n\n");
        printf("                   如需帮助，请与管理员联系!\n\n");
        printf("\n\n\n\n按任意键返回主菜单...");
        char ch = getch();
        system("cls");
    }
    return check;
}

//=======================Book图书管理抽象类=================//
/**创建一个Books抽象类:接收图书种类*/
class Books
{
public:
    //虚函数
    virtual void showInfo() = 0;  //显示书籍信息

//    virtual string Category() = 0;//图书类别

    int book_ID;        //图书编号

    string book_Name;   //图书名

    int book_kindId;    //图书种类

    double book_Price;  //图书价格

    int book_Num;       //图书数量
};

//=======================图书种类的类=================//
class BookKind : public Books
{

public:
    void showInfo() override;

    static string Category(int x_id);

    BookKind(int id, string name, int x_id, double price, int num)
    {
        this->book_ID = id;
        this->book_Name = std::move(name);
        this->book_kindId = x_id;
        this->book_Price = price;
        this->book_Num = num;
    }
};

/**完成图书类别输入*/
void BookKind::showInfo()
{
    cout << "图书编号： " << this->book_ID
         << "\t 图书名：" << this->book_Name
         << "\t 图书分类： " << BookKind::Category(book_kindId)
         << "\t 图书价格：" << this->book_Price
         << "\t 图书库存：" << this->book_Num << endl;
}

/**返回对应的图书类别*/
string BookKind::Category(int x_id)
{
    if (x_id == 1) { return "计算机类"; }
    else if (x_id == 2) { return "哲学类"; }
    else if (x_id == 3) { return "自然科学类"; }
    else if (x_id == 4) { return "社会科学类"; }
    else if (x_id == 5) { return "小说"; }
    else if (x_id == 6) { return "其它类别"; }
    else { return "未知分类"; }
}

//=======================实现系统主要功能的类=================//
/**工具类:完成图书管理相关操作*/
class Library
{
public:
    Library();//构造函数:初始化

    Books **book_Array; //用于存放图书的数组

    int book_Total;     //图书总量

    bool isFile = false;        //判断文件是否为空,空:false

    void Add_book();    //添加图书

    void Show_book() const;     //图书信息显示

    void Borrow_and_return();   //图书归还与借阅

    void Find_book() const;     //按编号或书名查找图书

    void Revise_book();         //修改图书

    void Delete_book();         //删除图书

    void Sort_book();           //升降序排序

    void Save_file();           //保存文件

    void Clean_file();          //清空图书列表

    void Cin_file();            //文件导入

    int Total_num();            //统计人数

    int Exist_or_not(const string &name) const;//判断借阅人是否存在

    static void menu_1();      //读者菜单
    static void menu_2();      //图书管理员菜单
    static void menu_3();      //系统管理员菜单

    ~Library()
    {
        if (this->book_Array != nullptr)
        {
            delete[]this->book_Array;
            this->book_Array = nullptr;
        }
    }           //虚构函数用于释放空间

};

/**构造函数进行类Library的初始化*/
Library::Library()
{
//初始化属性
    fstream file("library.txt");
//    file.open("library.txt");
    if (!file.is_open()) //文件不存在
    {
        cout << "文件不存在 \n";
        this->book_Total = 0;
        this->isFile = false;
        this->book_Array = nullptr;
        file.close();
        return;
    }
    //文件存在
    char ch;
    file >> ch;
    if (file.eof())//判断文件是否为空
    {
        cout << "文件为空! \n";
        this->book_Total = 0;
        this->isFile = false;
        this->book_Array = nullptr;
        file.close();
        return;
    }
    if (file)
    {
        //当文件存在且文件有内容
        int num = this->Total_num();
        cout << "现有图书总数为：" << num << endl;
        this->book_Total = num;
        //根据图书数创建数组并开辟空间
        cout << "图书文件加载成功...." << endl;
        this->book_Array = new Books *[this->book_Total];
        //文件数据导入放入数组中
        int id;
        string name;
        int x_id;
        double price;
        int number;

        int index = 0;
        while (file >> id && file >> name && file >> x_id && file >> price && file >> number)
        {
            Books *Book;
            Book = new BookKind(id, name, x_id, price, number);
            this->book_Array[index] = Book;
            index++;
        }
        file.close();
        isFile = true;
        printf("\n导入文件已成功，祝您使用愉快！\n\n");
//        this->Cin_file();//行不通
        Sleep(1000);
    }
}

/**读者菜单*/
void Library::menu_1()
{
    system("cls");
    printf("\t       ++++图书管理系统++++\n");
    printf("\t\t\n");
    cout << "	***************************************************" << endl;
    cout << "	*           请选择对应功能                        *" << endl;
    cout << "	***************************************************" << endl;
    cout << "	* 1.显示所有图书信息                              *" << endl;
    cout << "	* 2.图书借阅与归还                                *" << endl;
    cout << "	* 3.图书查找                                      *" << endl;
    cout << "	* 4.图书排序查看                                  *" << endl;
    cout << "	* 5.返回上级菜单                                  *" << endl;
    cout << "	* 6.返回主菜单                                    *" << endl;
    cout << "	* 0.直接退出该程序                                *" << endl;
    cout << "	***************************************************" << endl;
    cout << "=======>>>>>>  " << endl;
}

/**图书管理员菜单*/
void Library::menu_2()
{
    printf("\t\t 图书管理系统\n");
    printf("\t\t\n");
    cout << "	***************************************************" << endl;
    cout << "	*           请输入对应功能的数字完成操作           *" << endl;
    cout << "	* 1.采编入库                                     *" << endl;
    cout << "	* 2.显示所有图书信息                              *" << endl;
    cout << "	* 3.查看图书借阅与归还信息                        *" << endl;
    cout << "	* 4.图书查找                                     *" << endl;
    cout << "	* 5.图书修改                                     *" << endl;
    cout << "	* 6.图书删除                                     *" << endl;
    cout << "	* 7.图书排序                                     *" << endl;
    cout << "	* 8.清空所有图书                                  *" << endl;
    cout << "	* 9.返回上级菜单                                  *" << endl;
    cout << "	* 0.直接退出该程序                                *" << endl;
    cout << "	***************************************************" << endl;
}

/**系统管理员菜单*/
void Library::menu_3()
{
    system("cls");
    cout << ">>>>功能正在开发中,敬请期待<<<<" << endl;
    cout << "按任意键返回上级菜单...." << endl;
    while (getch()) { return; }
}

/**实现采编入库功能*/
void Library::Add_book()
{
    cout << "请输入添加图书的数量： \n";
    int addNum = 0;
    cin >> addNum;
    if (addNum > 0)
    {
        //添加
        int newSize = this->book_Total + addNum;
        //开辟新空间
        auto **newSpace = new Books *[newSize];
        if (this->book_Array != nullptr)
        {
            for (int i = 0; i < this->book_Total; ++i)
            {
                newSpace[i] = this->book_Array[i];
            }
        }
        //添加新数据
        for (int i = 0; i < addNum; ++i)
        {
            int id;
            string name;
            int dSelect;
            double price;
            int num;

            cout << "请输入第 " << i + 1 << " 个新图书的编号：" << endl;
            cin >> id;
            cout << "----------------------------" << endl;
            cout << "请输入第 " << i + 1 << " 个新图书名：" << endl;
            cin >> name;
            cout << "----------------------------" << endl;
            cout << "1、计算机类 " << endl
                 << "2、哲学类 " << endl
                 << "3、自然科学类 " << endl
                 << "4、社会科学类 " << endl
                 << "5、小说 " << endl
                 << "6、其它类别 " << endl
                 << "请选择该图书的类别:" << endl;
            cin >> dSelect;
            cout << "----------------------------" << endl;
            cout << "请输入第 " << i + 1 << " 个新图书的价格：" << endl;
            cin >> price;
            cout << "----------------------------" << endl;
            cout << "请输入第 " << i + 1 << " 个新图书的库存：" << endl;
            cin >> num;
            cout << "----------------------------" << endl;
            Books *Book = nullptr;
            Book = new BookKind(id, name, dSelect, price, num);

            //创建的指针保存在数组中
            newSpace[this->book_Total + i] = Book;
        }
        //释放原有的空间
        delete[]this->book_Array;
        //改变空间指向
        this->book_Array = newSpace;
        //更改图书不为空标志
        this->isFile = true;
        //改变指针长度
        this->book_Total = newSize;
        //添加到文件中
        cout << "成功添加 " << addNum << " 名新图书 \n";
        this->Save_file();//保存
    }
    else { cout << "输入数据有误！ \n"; }
    system("pause");
    system("cls");
}

/**显示所有图书信息*/
void Library::Show_book() const
{
    if (!this->isFile)
    {
        cout << "\n    ===>>>文件不存在或已为空！<<<===" << endl;
    }
    else
    {
        for (int i = 0; i < book_Total; ++i)
        {
            this->book_Array[i]->showInfo();
        }
    }
    system("pause");
    system("cls");
}

/**借阅/归还*/
void Library::Borrow_and_return()
{
    if (!this->isFile)
    {
        cout << "!!!!>>>>>>文件不存在或记录为空！ \n";
        system("pause");
        system("cls");
    }
    else
    {
        cout << "======1.借书 ======\n";
        cout << "======2.还书 ======\n";
        int select = 0;
        cin >> select;

        cout << "======>>>请输入书名：\n";
        string name;
        cin >> name;
        int ret = this->Exist_or_not(name);
        if (ret != -1)
        {
            this->book_Array[ret]->showInfo();
            if (select == 1)
            {
                if (this->book_Array[ret]->book_Num != 0)
                {
                    this->book_Array[ret]->book_Num--;
                    cout << "借书成功！ \n";
                }
                else { cout << "该书已被借完！ \n"; }
            }
            else if (select == 2)
            {
                this->book_Array[ret]->book_Num++;
                cout << "还书成功！ \n";
            }
            else { cout << "错误操作！ \n"; }
            this->book_Array[ret]->showInfo();
        }
        else { cout << "该书不属于图书馆！ \n"; }
    }
    this->Save_file();//保存
    system("pause");
    system("cls");
}

/**图书查找*/
void Library::Find_book() const
{
    if (!this->isFile)
    {
        cout << "文件不存在或记录为空！ \n";
    }
    else
    {//进行简单的顺序查找
        cout << "请输入查找的方式： \n";
        cout << "=====1.按图书编号查找 =====\n";
        cout << "=====2.按图书姓名查找 =====\n";
        int select = 0;
        cin >> select;
        if (select == 2)
        {
            string name;
            cout << "请输入查找的图书姓名： \n";
            cin >> name;
            int ret;
            ret = this->Exist_or_not(name);

            if (ret != -1)
            {
                cout << "查找成功！该图书信息如下： \n";
                this->book_Array[ret]->showInfo();
            }
            else { cout << "查找失败，查无此书 \n"; }
        }
        else if (select == 1)
        {
            int id = 0;
            cout << "请输入查找的图书编号： \n";
            cin >> id;
            bool flag = false;//找到设为true
            for (int i = 0; i < this->book_Total; ++i)
            {
                if (this->book_Array[i]->book_ID == id)
                {
                    cout << "查找成功，图书编号为： "
                         << this->book_Array[i]->book_ID
                         << "号图书信息如下： \n";
                    flag = true;
                    this->book_Array[i]->showInfo();
                }
            }
            if (!flag) { cout << "查找失败，查无此书！ \n"; }
        }
        else { cout << "输入选项有误！ \n"; }
    }
    system("pause");
    system("cls");
}

/**图书信息修改*/
void Library::Revise_book()
{
    if (!this->isFile) { cout << "文件不存在或记录为空！ \n"; }
    else
    {
        cout << "请输入修改图书的名字： \n";
        string name;
        cin >> name;

        int ret = this->Exist_or_not(name);
        if (ret != -1)
        {
            //查找
//            delete this->book_Array[ret];
            int newId = 0;
            string newName;
            int dSelect = 0;
            double price;
            int number;

            this->book_Array[ret]->showInfo();
            cout << "请输入新图书的编号： \n";
            cin >> newId;
            cout << "----------------------------" << endl;
            cout << "请输入新图书名： \n";
            cin >> newName;
            cout << "----------------------------" << endl;
            cout << "1、计算机类 " << endl
                 << "2、哲学类 " << endl
                 << "3、自然科学类 " << endl
                 << "4、社会科学类 " << endl
                 << "5、小说 " << endl
                 << "6、其它类别 " << endl
                 << "请选择该图书的类别:" << endl;
            cin >> dSelect;
            cout << "----------------------------" << endl;
            cout << "请输入新图书的价格： \n";
            cin >> price;
            cout << "----------------------------" << endl;
            cout << "请输入新图书的库存： \n";
            cin >> number;
            cout << "----------------------------" << endl;

            Books *Book = nullptr;
            Book = new BookKind(newId, name, dSelect, price, number);
            //更改数据到数组中
            this->book_Array[ret] = Book;
            cout << "修改成功！ \n";
            //保存到文件中
            this->Save_file();
        }
        else
        {
            cout << "修改失败，查无此人 \n";
        }
    }
    system("pause");
    system("cls");
}

/**删除对应图书*/
void Library::Delete_book()
{
//数组数据前移（找到位置的后一位）
    if (!this->book_Array) { cout << "文件不存在或文件为空! \n"; }
    else
    {
        //按照图书的姓名删除
        cout << "请输入想要删除的图书名字： \n";
        string name;
        cin >> name;
        int index = this->Exist_or_not(name);
        if (index != -1)
        {
            //book_Total = 1时不进入循坏
            for (int i = 0; i < this->book_Total - 1; ++i)
            {
                //数据前移
                this->book_Array[i] = this->book_Array[i + 1];
            }
            //数量更新
            this->book_Total--;
            //数据同步到文件中
            this->Save_file();

            cout << "删除成功！ \n";
        }
        else { cout << "删除失败，未找到该图书！ \n"; }
    }
    this->Save_file();
    system("pause");
    system("cls");
}

/**图书排序*/
void Library::Sort_book()
{
    if (!this->isFile)
    {
        cout << "文件不存在或记录为空！ \n";
        system("pause");
        system("cls");
    }//文件不存在
    else//进行简单选择排序
    {
        cout << "请选择排序方式： \n";
        cout << "1.按图书编号进行升序 \n";
        cout << "2.按图书编号进行降序 \n";

        int select = 0;
        cin >> select;
        for (int i = 0; i < book_Total; ++i)
        {
            int minOrMax = i;
            for (int j = i + 1; j < book_Total; ++j)
            {
                if (select == 1)//按图书编号进行升序
                {
                    if (book_Array[minOrMax]->book_ID > book_Array[j]->book_ID)
                    {
                        minOrMax = j;
                    }
                }
                else            //按图书编号进行降序
                {
                    if (book_Array[minOrMax]->book_ID < book_Array[j]->book_ID)
                    {
                        minOrMax = j;
                    }
                }
            }
            if (i != minOrMax)
            {
                Books *temp = book_Array[i];
                book_Array[i] = book_Array[minOrMax];
                book_Array[minOrMax] = temp;
            }
        }
        cout << "排序成功！排序后结果为： \n";
        this->Save_file();
        this->Show_book();
    }
}

/**文件保存*/
void Library::Save_file()
{
    ofstream ofs;
    ofs.open("library.txt", ios::out);

    for (int i = 0; i < this->book_Total; ++i)
    {
        ofs << this->book_Array[i]->book_ID << " "
            << this->book_Array[i]->book_Name << " "
            << this->book_Array[i]->book_kindId << " "
            << this->book_Array[i]->book_Price << " "
            << this->book_Array[i]->book_Num << endl;
    }
    ofs.close();
}

/**清空图书*/
void Library::Clean_file()
{
    cout << "确认清空？ \n";
    cout << "!!!!>>>1. 确认 <<<!!!" << endl;
    cout << "!!!!>>>2. 返回 <<<!!!" << endl;
    int select = 0;
    cin >> select;
    if (select == 1)
    {
        //trunc 如果存在文件 则删除并重建
        ofstream ofs("library.txt", ios::trunc);
        ofs.close();

        //清空内容
        if (this->book_Array != nullptr)
        {
            for (int i = 0; i < this->book_Total; ++i)
            {
                if (this->book_Array[i] != nullptr)
                {
                    delete this->book_Array[i];
                }
            }
            this->book_Total = 0;
            delete[]this->book_Array;
            this->book_Array = nullptr;
            this->isFile = false;
        }
        cout << "清空成功！ \n";
    }
    this->Save_file();
    system("pause");
    system("cls");
}

/**文件导入*/
void Library::Cin_file()
{
    ifstream ifs;
    ifs.open("library.txt", ios::in);

    int id;
    string name;
    int x_id;
    double price;
    int number;

    int index = 0;
    while (ifs >> id && ifs >> name && ifs >> x_id && ifs >> price && ifs >> number)
    {
        Books *Book;
        Book = new BookKind(id, name, x_id, price, number);
        this->book_Array[index] = Book;
        index++;
    }
    ifs.close();
    printf("\n导入文件已成功，祝您使用愉快！\n\n");
}

/**统计总人数*/
int Library::Total_num()
{
    ifstream ifs;
    ifs.open("library.txt", ios::in);

    int id;
    string name;
    int x_id;
    double price;
    int number;

    int num = 0;

    while (ifs >> id && ifs >> name && ifs >> x_id && ifs >> price && ifs >> number)
    {
        num++;
    }
    ifs.close();

    return num;
}

/**判断借阅人是否存在*/
int Library::Exist_or_not(const string &name) const
{
    int index = -1;

    for (int i = 0; i < book_Total; ++i)
    {
        if (this->book_Array[i]->book_Name.find(name) != string::npos)
        {
            index = i;
            break;
        }
    }
    return index;
}

//==================Person类实现不同身份操作=========================//
/**定义Person类实现不同身份操作*/
class Person : public Library
{
public:
    bool back_2 = true;

    Person() = default;//

    static void menu_0();      //祖传登录界面

    static void menu_start();  //开始

    void function_1();  //提供功能接口1:读者注册登录
    void function_2();  //提供功能接口2:图书管理员
    void function_3();  //提供功能接口3:系统管理员
    void function_4();  //提供功能接口4:读者具体操作
    void function_5();  //提供功能接口4:读者随便逛逛


    static void Drop_out();//全局退出界面
//    ~ Person();
};

/**开始菜单:身份选择*/
void Person::menu_start()
{
    system("cls");
    time_t time_login;
    struct tm *p;
    time(&time_login);
    p = gmtime(&time_login);
    printf("--------------------------------------------------------------------------\n");
    printf("+++++++++++>>>>>\n");
    cout << "\t              ╔══════════════════════════╗" << endl;
    cout << "\t              ║    欢迎来到图书管理系统  ║" << endl;
    cout << "\t              ╚══════════════════════════╝" << endl;
    printf("                        现在时间：%d年%d月%d日 %d:%d:%d\n", 1900 + p->tm_year,
           1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
    printf("                                                                                \n");
    printf("                              Author :---    ***************          \n");
    printf("                                                                                \n");
    printf("--------------------------------------------------------------------------\n");
    printf("\n\n===========================================================\n");
    printf("             1、我是【读者】\n\n");
    printf("             2、我是【图书管理员】\n\n");
    printf("             3、我是【系统管理员】\n\n");
    printf("             0、退出系统\n\n");
    printf("=======>>>>>>>>>请选择您的身份：");

}

/**祖传登录界面*/
void Person::menu_0()
{
    system("cls");
    system("color b");
    time_t timep;    //系统时间
    struct tm *p_1;
    time(&timep);
    p_1 = gmtime(&timep);

    printf("\t\t\t\t\t\t  当前时间%02d时%02d分\n", 8 + p_1->tm_hour, p_1->tm_min);
    printf("\t\t\t\t\t\t   %d年%02d月%02d日\n", 1900 + p_1->tm_year, 1 + p_1->tm_mon, p_1->tm_mday);
    printf("\t★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n\n");
    printf("\t★\t\t\t\t\t有朋自远方来，不亦乐乎！ \t\t\t\t\t★\n\n");
    printf("\t★\t\t\t*****************************************************\t\t\t\t★\n\n");
    printf("\t★\t\t\t\t\t 图书管理系统 V1.0\t\t\t\t\t\t★\n\n");
    printf("\t★\t\t\t  制作者:--- 专业:智能科学与技术 学号:--------------\t\t\t★\n\n");
    printf("\t★\t\t\t*****************************************************\t\t\t\t★\n\n");
    printf("\t★\t\t\t  欢迎使用学生通讯录系统,祝您使用愉快! \t\t\t\t\t\t★\n\n");
    printf("\t★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n\n");
    printf("请输入六位登陆密码！(123456)\n");

    int i = 4;
    while (true)
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
    Sleep(500);
    printf("\n图书管理系统加载已成功，亲爱的系统管理员 祝您使用愉快！\n\n");
    printf("请按任意键进入页面菜单！\n\n");
    system("pause");
    system("cls");    //清屏
}

/**提供功能接口1:读者注册界面*/
void Person::function_1()
{
    Syuser US;//实列化一个用户用于注册登录
    int choice = 0;
    while (back_2)
    {
        system("cls");
        US.welcome_title();//欢迎标题
        printf("\n\n");
        printf(" *++*     *++*  1、注册\n\n");
        printf("  *++++++++*    2、登录\n\n");
        printf("      *+*       3、随便逛逛\n\n");
        printf("    *+++++*     4、返回上级菜单\n\n");
        printf(" *+++++*+++++*  0、退出该系统\n\n");
        printf("\n\n");
        printf("==================>>>>>请输入选项前的编号，按回车键结束：");
        cin >> choice;
        if (choice == 0)
        {
            Person::Drop_out();//退出界面
            break;
        }
        if (choice == 1)
        {
            US.create_user();
        }
        else if (choice == 2)
        {
            if (US.login() == 1)
            {
                this->function_4();
            }
        }
        else if (choice == 3)
        {
            this->function_5();
        }
        else if (choice == 5)
        {
            back_2 = false;
            break;
        }
        else
        {
            cout << "=====>>>>输错啦...,还未开发该功能1" << endl
                 << "====>>>请重新输入" << endl << flush;
        }
    }
}

/**提供功能接口2:图书管理员*/
void Person::function_2()
{
//    Library MA;//实列化一个图书管理员对象
//    this->Cin_file();//自动导入文件
    int choice = 0;
    bool flag = true;
    while (flag)
    {
        system("cls");
        this->menu_2();
        cin >> choice;
        switch (choice)
        {
            case 0://直接退出该程序
                this->Save_file();//退出前保存
                Drop_out();
                break;
            case 1://采编入库
                this->Add_book();
                break;
            case 2://显示所有图书信息
                this->Show_book();
                break;
            case 3: //查看图书借阅与归还信息
                cout << "暂未开发,敬请期待" << endl;
                system("pause");
                break;
            case 4://图书查找
                this->Find_book();
                break;
            case 5://图书修改
                this->Revise_book();
                break;
            case 6://图书删除
                this->Delete_book();
                break;
            case 7://图书排序
                this->Sort_book();
                break;
            case 8://清空所有图书
                this->Clean_file();
                break;
            case 9://返回上级菜单
                flag = false;
                break;
            default://other
                cout << "选错了,选错了,该功能还未开发..." << endl;
                break;
        }
    }
}

/**提供功能接口3:系统管理员*/
void Person::function_3()
{
    this->menu_3();
}

/**提供功能接口4:读者具体操作*/
void Person::function_4()
{
    int choice;
    bool isFlag = true;
    while (isFlag)
    {
        this->menu_1();
        cin >> choice;
        switch (choice)
        {
            case 1://显示所有图书信息
                this->Show_book();
                break;
            case 2://图书借阅与归还
                this->Borrow_and_return();
                break;
            case 3://图书查找
                this->Find_book();
                break;
            case 4://图书排序查看
                this->Sort_book();
                break;
            case 5://返回上级菜单
                return;
                break;
            case 6://直接返回主菜单
                back_2 = isFlag = false;
                break;
            case 0://直接退出该程序
                Person::Drop_out();
                break;
            default://other
                cout << "选错了,选错了,该功能可能还处于开发阶段..." << endl;
                system("cls");
                break;
        }
    }
}

/**提供功能接口4:读者随便逛逛*/
void Person::function_5()
{
    int choice;
    bool isFlag = true;
    while (isFlag)
    {
        system("cls");
        printf("\t       ++++图书管理系统++++\n");
        printf("\t\t\n");
        cout << "	***************************************************" << endl;
        cout << "	*           请选择对应功能                        *" << endl;
        cout << "	***************************************************" << endl;
        cout << "	* 1.显示所有图书信息                              *" << endl;
        cout << "	* 2.图书查找                                      *" << endl;
        cout << "	* 3.图书排序查看                                  *" << endl;
        cout << "	* 4.返回上级菜单                                  *" << endl;
        cout << "	* 5.返回主菜单                                    *" << endl;
        cout << "	* 0.直接退出该程序                                *" << endl;
        cout << "	***************************************************" << endl;
        cout << "=======>>>>>>  " << endl;
        cin >> choice;
        switch (choice)
        {
            case 1://显示所有图书信息
                this->Show_book();
                break;
            case 2://图书查找
                this->Find_book();
                break;
            case 3://图书排序查看
                this->Sort_book();
                break;
            case 4://返回上级菜单
                return;
                break;
            case 5://直接返回主菜单
                back_2 = isFlag = false;
                break;
            case 0://直接退出该程序
                Person::Drop_out();
                break;
            default://other
                cout << "选错了,选错了,该功能可能还处于开发阶段..." << endl;
                system("cls");
                break;
        }
    }
}

/**全局退出界面*/
void Person::Drop_out()
{
//    system("cls");
    cout << "\t\t                      ╔══════════════════════════╗" << endl;
    cout << "\t\t                      ║    欢迎再次使用！再见    ║" << endl;
    cout << "\t\t                      ╚══════════════════════════╝" << endl;
    system("pause");//按任意键继续
    exit(0);//不管是在哪个部分调用这个函数 退出程序 0 - 退出参数
}

//===================================================//
//主函数
int main(int argc, char const *argv[])
{
//    system("chcp 936");
    system("color a");
    Person person;
    int choice;
    while (true)
    {
        person.menu_start();//身份界面选择
        cin >> choice;
        if (choice == 0)
        {
            person.Drop_out();//退出界面
            break;
        }//退出
        if (choice == 1) { person.function_1(); } //读者
        else if (choice == 2)
        {
            person.function_2();
        }//图书管理员
        else if (choice == 3)
        {
            person.menu_0();//管理员登录界面
            person.function_3();//管理员操作界面
        }//系统管理员
        else
        {
            cout << "***************************************" << endl;
            cout << "\n=====>>>>还未开发该功能,敬请期待" << endl
                 << "====>>>请重新输入" << endl << flush;
            cout << "***************************************" << endl;
            continue;
        }
    }
    return 0;
}




