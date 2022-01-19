// Created by yxn on 2021/11/26.
//项目要求：
//（
//1）系统功能：从文件或键盘读入一串电文字符，实现赫夫曼编码和译码。
//
//2）密码文件以文件的形式进行存放。
//项目目的：
//
//1）考查二叉树存储表示及其基本操作实现。
//
//2）赫夫曼数的建立。
//
//3）赫夫曼树编码和译码算法。

#include <cstdio>
#include <cstdlib>
#include <direct.h>//目录操作头
#include <cmath>
#include <iostream>
#include <ctime>
#include <synchapi.h>//暂停Sleep（）
#include <conio.h>

#define OK 1
#define ERROR 0

using namespace std;//使用标准命名空间

typedef int Status;

/**链表元素的定义*/
typedef struct
{
    int char_ASCII;
    int value;               //字符的出现频率或权值
    char *Huffmancode;       //Huffman编码所在数组的指针；
    int Huffmancode_bit = 0; //Huffmancode数组的长度。
} ElemType;

/**链表的定义*/
typedef struct Node
{
    ElemType elem;
    struct Node *next;
} LinkNode, *LinkList;

/**链表初始化*/
Status List_Init(LinkList &L)
{
    if (!(L = (LinkList) malloc(sizeof(LinkNode))))
    {
        exit(ERROR);
    }//内存分配失败
    L->next = nullptr;
    return OK;
}

/**链表插入*/
Status List_Insert(LinkList &L, int i, ElemType e)
{
    LinkList p, s;
    p = L;
    int j = 0;
    while (p && j < i - 1)
    {
        p = p->next;
        j++;
    }
    if (j > i - 1 || !p) return ERROR;
    s = (LinkList) malloc(sizeof(LinkNode));
    s->elem = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

/**删除链表*/
Status List_Destroy(LinkList &L)
{
    LinkList p;
    while (L)
    {
        p = L;
        L = p->next;
        free(p);
    }
    return OK;
}

/**Huffman树的定义*/
typedef struct
{
    unsigned int value, ASCII_CODE;     //叶子结点的权值和所对应的字符的ASCII码
    unsigned int parent, lchid, rchild; //双亲节点和左右孩子节点
} HTNode, *HuffmanTree;

/**读取源文件信息，统计字符出现频率*/
Status File_sourceload()
{
    FILE *fp1 = fopen("testText.txt", "r");
    int char_num[256] = {0};  //根据ASCII表，有256个字符
    int text_length = 0, char_number = 0;
    int i, l = 0;
    char c;

    //初始化文件，若不存在则退出
    if (fp1 == nullptr)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        return (ERROR);
    }
    //统计字符频率及字符串长度
    while ((c = fgetc(fp1)) != EOF)
    {
        char_num[c]++;
        text_length++;
    }
    //统计字符串中有多少种不同字符
    for (i = 0; i < 256; i++)
    {
        if (char_num[i] != 0)
        {
            char_number++;
        }
    }
    fclose(fp1);

    //结果写入：字符串长度、出现的字符种类
    _mkdir("huffman_temp");
    FILE *fp2 = fopen("huffman_temp\\char_frequency.txt", "w+");
    fprintf(fp2, "%d\t%d", text_length, char_number);
    for (i = 0; i < 256; i++)
    {
        if (char_num[i] != 0)
        {
            fprintf(fp2, "\n%d %d", i, char_num[i]);
        }
    }
    //写入文本注释
    fprintf(fp2, "\n\n****注：该文件用来存储源文件中的字符数量、字符种类个数、每个字符对应的ASCII码及他们的出现频率！****\n");
    char fileadress[100];
    _getcwd(fileadress, 100);
    printf("文本字符统计完毕！\n信息保存在%s\\huffman_temp\\char_frequency.txt\n\n", fileadress);
    fclose(fp2);

    //结果输出
    printf("文本长度：%d    字符种类：%d\n", text_length, char_number);
    printf("按照ASCII码排列，字符出现频率如下：\n");
    for (i = 0; i < 256; i++)
    {
        if (char_num[i] != 0)
        {
            if (i == 32) { printf("空格：%d\n", char_num[i]); }
            else if (i == 13) { printf("回车：%d\n", char_num[i]); }
            else if (i == 10) { printf("换行：%d\n", char_num[i]); }
            else { printf("%c：%d\t\t", i, char_num[i]); }
            l++;
            if (l % 5 == 0) { printf("\n"); }
        }
    }
    printf("\n");
    return OK;
}

/**读取huffman_temp\char_frequenc.txt中的信息*/
Status File_read_char_num(LinkList &L, int &text_length, int &char_number)
{
    int i;
    ElemType e;
    List_Init(L);
    FILE *fp = fopen("huffman_temp\\char_frequency.txt", "r");
    fscanf(fp, "%d\t%d", &text_length, &char_number);
    for (i = 1; i <= char_number; i++)
    {
        fscanf(fp, "\n%d %d", &e.char_ASCII, &e.value);
        List_Insert(L, i, e);
    }
    fclose(fp);
    return OK;
}

/**选择权值最小的两个节点*/
Status HuffmanTree_Select(HuffmanTree HT, int n, int &s1, int &s2)
{
    int i;
    for (i = 1; i <= n; i++)
    {
        if (HT[i].parent == 0)
        {
            s1 = i;
            break;
        }
    }
    for (i = 1; i <= n; i++)
    {
        if (HT[i].value < HT[s1].value && HT[i].parent == 0)
        {
            s1 = i;
        }
    }
    for (i = 1; i <= n; i++)
    {
        if (HT[i].parent == 0 && i != s1)
        {
            s2 = i;
            break;
        }
    }
    for (i = 1; i <= n; i++)
    {
        if (HT[i].value < HT[s2].value && HT[i].parent == 0 && i != s1)
        {
            s2 = i;
        }
    }
    return OK;
}

/**创建Huffman树*/
Status HuffmanTree_Create(HuffmanTree &HT, int n, LinkList L, int if_print)
{
    /*HuffmanTree的初始化*/
    if (n <= 1) return ERROR;
    int m = 2 * n - 1;
    int i;
    LinkList p = L->next;
    HT = (HuffmanTree) malloc((m + 1) * sizeof(HTNode));
    for (i = 1; i <= m; i++)
    {
        HT[i].parent = 0;
        HT[i].lchid = 0;
        HT[i].rchild = 0;
    }
    for (i = 1; i <= n; i++)
    {
        HT[i].value = p->elem.value;
        HT[i].ASCII_CODE = p->elem.char_ASCII;
        p = p->next;
    }
    /*HuffmanTree的创建*/
    int s1, s2;
    for (i = n + 1; i <= m; i++)
    {
        HuffmanTree_Select(HT, i - 1, s1, s2);
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lchid = s1;
        HT[i].rchild = s2;
        HT[i].value = HT[s1].value + HT[s2].value;
    }
    /*根据输入的if_print选择是否要输出树的信息*/
    if (if_print == 1)
    {
        printf("Huffman树的信息如下：\n");
        printf("结点\t字符\t权值\t双亲\t左孩子\t右孩子\t\n");
        for (i = 1; i <= m; i++)
        {
            if (HT[i].ASCII_CODE == 32)
            {
                printf("%d\t空格\t%d\t%d\t%d\t%d\t\n", i, HT[i].value, HT[i].parent, HT[i].lchid, HT[i].rchild);
            }
            else if (HT[i].ASCII_CODE == 13)
            {
                printf("%d\t回车\t%d\t%d\t%d\t%d\t\n", i, HT[i].value, HT[i].parent, HT[i].lchid, HT[i].rchild);
            }
            else if (HT[i].ASCII_CODE == 10)
            {
                printf("%d\t换行\t%d\t%d\t%d\t%d\t\n", i, HT[i].value, HT[i].parent, HT[i].lchid, HT[i].rchild);
            }
            else if (HT[i].lchid != 0 || HT[i].rchild != 0)
            {
                printf("%d\t无\t%d\t%d\t%d\t%d\t\n", i, HT[i].value, HT[i].parent, HT[i].lchid, HT[i].rchild);
            }
            else
            {
                printf("%d\t%c\t%d\t%d\t%d\t%d\t\n", i, HT[i].ASCII_CODE, HT[i].value, HT[i].parent, HT[i].lchid,
                       HT[i].rchild);
            }
        }
    }
    return OK;
}

/**从叶子到根逆向求每个字符的Huffman编码，储存在指针L.Huffmancode中*/
Status HuffmanTree_Code(HuffmanTree HT, int n, LinkList &L)
{
    int i, start, c, f;
    LinkList p = L->next;
    char *cd = (char *) malloc(n * sizeof(char));
    cd[n - 1] = '\0';
    for (i = 1; i <= n; i++)
    {
        start = n - 1;
        for (c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent)
        {
            if (HT[f].lchid == c) { cd[--start] = '0'; }
            else { cd[--start] = '1'; }
        }
        p->elem.Huffmancode = (char *) malloc((n - start) * sizeof(char));
        strcpy(p->elem.Huffmancode, &cd[start]);
        p->elem.Huffmancode_bit = n - start - 1;
        p = p->next;
    }
    free(cd);
    return OK;
}

/**存储Huffman编码为字典*/
Status HuffmanCode_Write(HuffmanTree HT, int n, LinkList &L)
{
    LinkList p = L->next;
    FILE *fp = fopen("huffman_temp\\huffman_codeinfo.txt", "w+");
    int i, line = 0;
    char fileadress[100];
    _getcwd(fileadress, 100);
    printf("Huffman编码完毕！\n信息保存在%s\\huffman_temp\\huffman_codeinfo.txt\n", fileadress);
    printf("各字符对应的Huffman编码为：\n");
    fprintf(fp, "code number:%d\n", n);
    for (i = 1; i <= n; i++, p = p->next)
    {
        fprintf(fp, "%d\t%d\t%d\t", p->elem.char_ASCII, p->elem.value, p->elem.Huffmancode_bit);
        if (p->elem.char_ASCII == 32)
        {
            printf("空格：");
        }
        else if (p->elem.char_ASCII == 13)
        {
            printf("回车：");
        }
        else if (p->elem.char_ASCII == 10)
        {
            printf("换行：");
        }
        else
        {
            printf("%c：", p->elem.char_ASCII);
        }
        p->elem.Huffmancode = &p->elem.Huffmancode[0];
        while (*p->elem.Huffmancode != '\0')
        {
            fprintf(fp, "%c", *p->elem.Huffmancode);
            printf("%c", *p->elem.Huffmancode);
            p->elem.Huffmancode++;
        }
        line++;  //line用来控制输出的格式
        fprintf(fp, "\n");
        printf("\t\t");
        if (line % 4 == 0)
        {
            printf("\n");
        }
    }
    //写入文件注释
    fprintf(fp, "\n****注：该文件用来存储所有字符对应的ASCII码以及他们的Huffman编码！****\n");
    printf("\n文件写入完毕！\n");
    fclose(fp);
    List_Destroy(L);
    return OK;
}

/**返回某个字符的Huffman编码所在的指针*/
char *HuffmanCode_CharPoint(LinkList L, char c)
{
    LinkList p = L->next;
    while (p->next && p->elem.char_ASCII != c)
    {
        p = p->next;
    }
    return p->elem.Huffmancode;
}

/**对文件进行Huffman编码 huffman_encode.txt*/
Status HuffmanCode_Encode(HuffmanTree HT, LinkList L, int if_print)
{
    FILE *fp = fopen("huffman_temp\\huffman_codeinfo.txt", "r");
    int char_number, i;
    ElemType e;
    List_Init(L);
    fscanf(fp, "code number:%d\n", &char_number);
    for (i = 1; i <= char_number; i++)
    {
        fscanf(fp, "%d\t%d\t%d\t", &e.char_ASCII, &e.value, &e.Huffmancode_bit);
        e.Huffmancode = (char *) malloc(e.Huffmancode_bit * sizeof(char));
        fscanf(fp, "%s\n", e.Huffmancode);
        List_Insert(L, i, e);
    }
    char c, *ch;
    FILE *sourcefile = fopen("testText.txt", "r");
    FILE *huffman_encode = fopen("huffman_temp\\huffman_encode.txt", "w+");
    if (if_print == 1)
    {
        printf("文件的Huffman编码为：\n");
    }
    while (!feof(sourcefile))
    {
        c = fgetc(sourcefile);
        ch = HuffmanCode_CharPoint(L, c);
        //写入Huffman 0/1编码
        while (*ch != '\0')
        {
            if (if_print == 1)
            {
                printf("%c", *ch);
            }
            fputc(*ch, huffman_encode);
            ch++;
        }
    }
    //根据输入的if_print判断是否输出文件的Huffman编码
    if (if_print == 1)
    {
        printf("\n");
    }
    char fileadress[100];
    _getcwd(fileadress, 100);
    printf("Huffman编码完毕！\n信息保存在%s\\huffman_temp\\huffman_encode.txt\n", fileadress);
    fprintf(huffman_encode, "\n\n****注：该文件用来存储testText.txt文件的原始Huffman编码信息！****\n");
    fclose(fp);
    fclose(sourcefile);
    fclose(huffman_encode);
    List_Destroy(L);
    return OK;
}

/**每8位转化为对应的ASCII码*/
char bit_number_transform(int n[])
{
    char num = 0;
    int i;
    for (i = 0; i < 8; i++)
    {
        //三目运算对一个byte进行移位操作
        n[i] == 1 ? (num |= (1 << 7 - i)) : (num &= ~(1 << (7 - i)));
    }
    return num;
}

/**生成.huf文件*/
Status text_huffmancode_create()
{
    FILE *fp = fopen("huffman_temp\\huffman_encode.txt", "r");
    FILE *temp = fopen("huffman_temp\\code_readytowrite.txt", "wb+");
    FILE *huf = fopen("text_huffmancode.huf", "wb+");
    int codeinfo[8] = {0}, infonum = -1;
    int code_length = 0, code_mod, i, t;
    char c;
    while (!feof(fp))
    {
        if (c = fgetc(fp) == '\n') { break; }
        code_length++;
    }
    //计算需要补1的个数
    code_mod = code_length % 8;
    fseek(fp, 0L, SEEK_SET);
    for (i = 1; i <= code_length; i++)
    {
        c = fgetc(fp);
        fputc(c, temp);
    }
    //对文件进行补1，使得总长度为8的整数倍
    if (code_mod != 0)
    {
        for (i = 1; i <= 8 - code_mod; i++)
        {
            fprintf(temp, "%d", 1);
        }
    }
    fseek(temp, 0L, SEEK_SET);
    fputc(8 - code_mod, huf);
    //对文件进行编码
    for (i = 1; i <= (code_mod == 0 ? code_length : code_length + 8 - code_mod); i++)
    {
        c = fgetc(temp);
        if (c == '1') { codeinfo[++infonum] = 1; }
        else { codeinfo[++infonum] = 0; }
        if (infonum == 7)
        {
            c = bit_number_transform(codeinfo);
            fputc(c, huf);
            for (t = 0; t < 8; t++)
            {
                codeinfo[t] = 0;
            }
            infonum = -1;
        }
    }
    char fileadress[100];
    _getcwd(fileadress, 100);
    printf("Huffman码压缩完毕！\n信息保存在%s\\text_huffmancode.huf中\n", fileadress);
    fclose(fp);
    fclose(temp);
    fclose(huf);
    return OK;
}

/**解压.huf文件*/
Status text_huffmancode_unzip()
{
    FILE *huf = fopen("text_huffmancode.huf", "rb");
    FILE *unzip = fopen("huffman_temp\\huf_decode.txt", "wb+");
    long huflength = 0;
    double code_mod = 0.0;
    char c;
    int i, t;
    long count = 0;
    //获取文件长度
    fseek(huf, 0L, SEEK_SET);
    fseek(huf, 0L, SEEK_END);
    huflength = ftell(huf);
    //重新定位文件指针，开始读取
    fseek(huf, 0L, SEEK_SET);
    c = fgetc(huf);
    //获取huf文件的第一个字符，计算得到补1的个数
    for (i = 0; i <= 4; i++)
    {
        code_mod += ((c >> i) & 1) * pow(2, i);
    }
    //对huf文件进行译码
    while (!feof(huf))
    {
        c = fgetc(huf);
        for (t = 0; t < 8; t++)
        {
            if (((c >> (7 - t)) & 1) == 1)
            {
                count++;
                fputc(49, unzip);
                if (count == (8 * (huflength - 1) - code_mod))
                {
                    break;
                }
            }
            else if (((c >> (7 - t)) & 1) == 0)
            {
                count++;
                fputc(48, unzip);
                if (count == (8 * (huflength - 1) - code_mod))
                {
                    break;
                }
            }
        }
        if (count == (8 * (huflength - 1) - code_mod))
        {
            break;
        }
    }
    char fileadress[100];
    _getcwd(fileadress, 100);
    printf("huf文件翻译完毕！\n信息保存在%s\\huffman_temp\\huf_decode.txt中\n", fileadress);
    fclose(huf);
    fclose(unzip);
    return OK;
}

/**对文件进行Huffman解码 text_decode.txt*/
Status HuffmanCode_FileDecode(HuffmanTree HT, int char_number, int text_length)
{
    FILE *encodefile = fopen("huffman_temp\\huf_decode.txt", "r");
    FILE *decodefile = fopen("text_decode.txt", "w+");
    int q = 2 * char_number - 1;
    int k = 0, word = 0;
    char c;
    //从根开始，依次寻找Huffman编码对应的字符
    while (!feof(encodefile))
    {
        c = fgetc(encodefile);
        if (word == text_length) { break; }
        if (c == '0') { q = HT[q].lchid; }
        else if (c == '1') { q = HT[q].rchild; }
        if (HT[q].lchid == 0 && HT[q].rchild == 0)
        {
            fputc(HT[q].ASCII_CODE, decodefile);
            q = 2 * char_number - 1;
            word++;
        }
    }
    char fileadress[100];
    _getcwd(fileadress, 100);
    printf("压缩文件译码完毕！\n信息保存在%s\\text_decode.txt中！\n", fileadress);
    fclose(encodefile);
    fclose(decodefile);
    return OK;
}

/**对输入文件和输出文件进行比较，求正确率*/
Status File_Error_Percentage()
{
    int i;
    char ch1, ch2;
    float text_length = 3894.0, error_num = 0.0, percentage = 0.0;
    FILE *source = fopen("testText.txt", "r");
    FILE *decode = fopen("text_decode.txt", "r");
    //逐个字符进行比较，不一样则计数，一样则跳过看下一个字符
    for (i = 0; i <= text_length; i++)
    {
        ch1 = fgetc(source);
        ch2 = fgetc(decode);
        if (ch1 != ch2) { error_num++; }
    }
    percentage = (text_length - error_num) / text_length;
    printf("源文件与译码文件相比，错误个数为：%d，相似率为：%.2f%%\n",
           (int) error_num, percentage * 100);
    return OK;
}

/**计算压缩率*/
Status File_Zip_Percentage()
{
    long text_size, text_decode_size;
    float zippercentage;
    FILE *source = fopen("testText.txt", "r");
    FILE *decode = fopen("text_huffmancode.huf", "r");
    //获取文件testText.txt的大小
    fseek(source, 0L, SEEK_END);
    text_size = ftell(source);
    //获取文件text_huffmancode.huf的大小
    fseek(decode, 0L, SEEK_END);
    text_decode_size = ftell(decode);
    zippercentage = (float) text_decode_size / (float) text_size;
    printf("源文件大小为：%d字节  压缩后大小为：%d字节\n压缩率为：%.2f%%\n", text_size, text_decode_size, zippercentage * 100.0);
    return OK;
}

/**祖传登录界面*/
void menu_0()
{
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
    printf("\t★\t\t\t\t\t Huffman编码/译码程序\t\t\t\t\t\t★\n\n");
    printf("\t★\t\t\t  制作者:*** 专业:智能科学与技术 学号:***************\t\t\t★\n\n");
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
    Sleep(1000);
    printf("\n系统加载已成功，祝您使用愉快！\n\n");
    printf("请按任意键进入页面菜单！\n\n");
    system("pause");
    system("cls");    //清屏
}

/**显示菜单*/
void menu()
{
    printf("  ----------------------------------------------------\n");
    printf(" |                                                    |\n");
    printf(" |            Huffman编码/译码程序                    |\n");
    printf(" |                                                    |\n");
    printf(" |----快捷执行----------------------------------------|\n");
    printf(" |                                                    |\n");
    printf(" |              1.按流程执行所有操作                  |\n");
    printf(" |                                                    |\n");
    printf(" |----单步执行----------------------------------------|\n");
    printf(" |                                                    |\n");
    printf(" |              2.统计字符频率                        |\n");
    printf(" |              3.创建Huffman树                       |\n");
    printf(" |              4.求每个字符的Huffman编码             |\n");
    printf(" |              5.对文件进行Huffman编码               |\n");
    printf(" |              6.根据编码生成.huf文件                |\n");
    printf(" |              7.计算Huffman压缩率                   |\n");
    printf(" |              8.根据编码翻译.huf文件                |\n");
    printf(" |              9.对翻译得到的文件进行解码            |\n");
    printf(" |             10.比较输入文件与输出文件              |\n");
    printf(" |                                                    |\n");
    printf(" |             11.退出程序                            |\n");
    printf(" |                                                    |\n");
    printf("  ----------------------------------------------------\n");
    int choice = 0;
    while (choice < 1 || choice > 11)
    {
        printf("请选择需要执行的操作：  ");
        cin >> choice;
        if (choice < 1 || choice > 11)
        {
            printf("\n对不起，输入有误，请重新输入！\n\n");
        }
    }
    //对后文中菜单函数的申明
    void menu_function1();
    void menu_function2();
    void menu_function3();
    void menu_function4();
    void menu_function5();
    void menu_function6();
    void menu_function7();
    void menu_function8();
    void menu_function9();
    void menu_function10();
    void menu_function11();
    switch (choice)
    {
        case 1:menu_function1();
            break;
        case 2:menu_function2();
            break;
        case 3:menu_function3();
            break;
        case 4:menu_function4();
            break;
        case 5:menu_function5();
            break;
        case 6:menu_function6();
            break;
        case 7:menu_function7();
            break;
        case 8:menu_function8();
            break;
        case 9:menu_function9();
            break;
        case 10:menu_function10();
            break;
        case 11:menu_function11();
            break;
    }
}

/**询问是否进行下一步操作*/
int if_go_next()
{
    int c = 2;
    while (c != 1 && c != 0)
    {
        printf("是否要进行下一步操作？(【1】是【2】否):\n");
        cout << "=====>>>>>  ";
        cin >> c;
        getchar();
        if (c != 1 && c != 0)
        {
            printf("\n输入有误！请重新输入！\n");
        }
    }
    if (c == 1)
    {
        system("cls");
        return 1;
    }
    else { return 0; }
}

/**询问是否返回菜单*/
void if_backto_menu()
{
    int c = 2;
    while (c != 1 && c != 0)
    {
        printf("是否要返回菜单？(【1】是【2】否): \n");
        cout << "=====>>>>>  ";
        cin >> c;
        getchar();
        if (c != 1 && c != 0)
        {
            printf("\n输入有误！请重新输入！\n");
        }
    }
    if (c == 1)
    {
        system("cls");
        menu();
    }
    else { exit(OK); }
}

/**菜单功能1*/
void menu_function1()
{
    LinkList L;
    HuffmanTree HT;
    int text_length, char_number;
    int whether = 0;

    printf("\n已选择操作1！\n");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }

    printf("\n开始进行第1步：统计字符频率！\n");
    FILE *fp1 = fopen("testText.txt", "r");
    if (fp1 == nullptr)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        if_backto_menu();
    }
    File_sourceload();
    printf("\n统计字符完毕！");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }

    printf("\n开始进行第2步：创建Huffman树！\n");
    printf("在创建之后，是否要查看Huffman树？(【1】查看  【2】不查看): \n");
    cout << "=====>>>>>  ";
    scanf("%d", &whether);
    File_read_char_num(L, text_length, char_number);
    HuffmanTree_Create(HT, char_number, L, whether);
    printf("\n创建Huffman树完毕！");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }

    printf("\n开始进行第3步：求每个字符的Huffman编码！\n");
    HuffmanTree_Code(HT, char_number, L);
    HuffmanCode_Write(HT, char_number, L);
    printf("\n字符Huffman编码生成完毕！");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }

    printf("\n开始进行第4步：对文件进行Huffman编码！\n");
    printf("在创建之后，是否要查看文件的Huffman编码？(【1】查看  【2】不查看): \n");
    cout << "=====>>>>>  ";
    cin >> whether;
    HuffmanCode_Encode(HT, L, whether);
    printf("\n文件Huffman编码完毕！");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }

    printf("\n开始进行第5步：根据编码生成.huf文件！\n");
    text_huffmancode_create();
    printf("\n生成.huf文件完毕！");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }

    printf("\n开始进行第6步：计算Huffman压缩率！\n");
    File_Zip_Percentage();
    printf("\n计算Huffman压缩率完毕！");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }

    printf("\n开始进行第7步：根据编码翻译.huf文件！\n");
    text_huffmancode_unzip();
    printf("\n根据编码翻译.huf文件完毕！");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }

    printf("\n开始进行第8步：对翻译得到的文件进行解码！\n");
    HuffmanCode_FileDecode(HT, char_number, text_length);
    printf("\n对文件进行解码完毕！");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }

    printf("\n开始进行第9步：比较输入文件与输出文件！\n");
    File_Error_Percentage();

    printf("\n所有操作执行完毕！\n");
    if_backto_menu();
}

/**菜单功能2*/
void menu_function2()
{
    printf("\n已选择操作2！请确保存在文件testText.txt！\n");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }
    FILE *fp1 = fopen("testText.txt", "r");
    if (fp1 == nullptr)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        if_backto_menu();
    }
    fclose(fp1);
    printf("\n开始统计字符频率！\n");
    File_sourceload();
    printf("\n统计字符完毕！\n");
    if_backto_menu();
}

/**菜单功能3*/
void menu_function3()
{
    printf("\n已选择操作3！请确保存在文件huffman_temp\\char_frequency.txt！\n");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }
    FILE *fp = fopen("huffman_temp\\char_frequency.txt", "r");
    if (fp == nullptr)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        if_backto_menu();
    }
    fclose(fp);
    printf("\n开始创建Huffman树！\n");
    int whether = 0;
    printf("在创建之后，是否要查看Huffman树？(【1】查看  【2】不查看): \n");
    cout << "=====>>>>>  ";
    cin >> whether;
    LinkList L;
    HuffmanTree HT;
    int text_length, char_number;
    File_read_char_num(L, text_length, char_number);
    HuffmanTree_Create(HT, char_number, L, whether);
    printf("\n创建Huffman树完毕！\n");
    if_backto_menu();
}

/**菜单功能4*/
void menu_function4()
{
    printf("\n已选择操作4！请确保存在文件huffman_temp\\char_frequency.txt！\n");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }
    FILE *fp = fopen("huffman_temp\\char_frequency.txt", "r");
    if (fp == NULL)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        if_backto_menu();
    }
    fclose(fp);
    LinkList L;
    HuffmanTree HT;
    int text_length, char_number;
    File_read_char_num(L, text_length, char_number);
    HuffmanTree_Create(HT, char_number, L, 0);
    HuffmanTree_Code(HT, char_number, L);
    HuffmanCode_Write(HT, char_number, L);
    printf("\n求每个字符的Huffman编码完毕！\n");
    if_backto_menu();
}

/**菜单功能5*/
void menu_function5()
{
    printf("\n已选择操作5！请确保存在文件huffman_temp\\huffman_codeinfo.txt！\n");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }
    FILE *fp = fopen("huffman_temp\\huffman_codeinfo.txt", "r");
    if (fp == nullptr)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        if_backto_menu();
    }
    fclose(fp);
    int whether = 0;
    LinkList L;
    HuffmanTree HT;
    printf("在创建之后，是否要查看文件的Huffman编码？(【1】查看  【2】不查看): \n");
    cout << "=====>>>>>  ";
    cin >> whether;
    int text_length, char_number;
    File_read_char_num(L, text_length, char_number);
    HuffmanTree_Create(HT, char_number, L, 0);
    HuffmanCode_Encode(HT, L, whether);
    printf("\n对文件进行Huffman编码完毕！\n");
    if_backto_menu();
}

/**菜单功能6*/
void menu_function6()
{
    printf("\n已选择操作6！请确保存在文件huffman_temp\\huffman_encode.txt！\n");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }
    FILE *fp = fopen("huffman_temp\\huffman_encode.txt", "r");
    if (fp == nullptr)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        if_backto_menu();
    }
    fclose(fp);
    text_huffmancode_create();
    printf("\n生成.huf文件完毕！\n");
    if_backto_menu();
}

/**菜单功能7*/
void menu_function7()
{
    printf("\n已选择操作7！\n请确保存在文件testText.txt和text_huffmancode.huf！\n");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }
    FILE *source = fopen("testText.txt", "r");
    FILE *decode = fopen("text_huffmancode.huf", "r");
    if (source == nullptr || decode == nullptr)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        if_backto_menu();
    }
    fclose(source);
    fclose(decode);
    File_Zip_Percentage();
    printf("\n计算Huffman压缩率完毕！\n");
    if_backto_menu();
}

/**菜单功能8*/
void menu_function8()
{
    printf("\n已选择操作8！\n请确保存在文件text_huffmancode.huf！\n");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }
    FILE *huf = fopen("text_huffmancode.huf", "rb");
    if (huf == nullptr)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        if_backto_menu();
    }
    fclose(huf);
    text_huffmancode_unzip();
    printf("\n根据编码翻译.huf文件完毕！\n");
    if_backto_menu();
}

/**菜单功能9*/
void menu_function9()
{
    printf("\n已选择操作9！\n请确保存在文件huffman_temp\\huf_decode.txt！\n");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }
    FILE *encodefile = fopen("huffman_temp\\huf_decode.txt", "r");
    if (encodefile == nullptr)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        if_backto_menu();
    }
    fclose(encodefile);
    int text_length, char_number;
    HuffmanTree HT;
    LinkList L;
    File_read_char_num(L, text_length, char_number);
    HuffmanTree_Create(HT, char_number, L, 0);
    HuffmanCode_FileDecode(HT, char_number, text_length);
    printf("\n对文件进行解码完毕！\n");
    if_backto_menu();
}

/**菜单功能10*/
void menu_function10()
{
    printf("\n已选择操作10！\n请确保存在文件testText.txt和text_decode.txt！\n");
    if (if_go_next() == 0)
    {
        if_backto_menu();
    }
    FILE *decode = fopen("text_decode.txt", "r");
    if (decode == nullptr)
    {
        printf("读取文件失败或文件不存在！请重试！\n");
        if_backto_menu();
    }
    fclose(decode);
    File_Error_Percentage();
    printf("\n输入文件与输出文件比较完毕！\n");
    if_backto_menu();
}

/**菜单功能11*/
void menu_function11()
{
    exit(OK);
}

/**任务恢复，如果选择是则直接跳过读取字符频率，继续执行*/
void Project_Recovery()
{
    int whether = 0;
    LinkList L;
    HuffmanTree HT;
    int text_length, char_number;
    printf("是否继续执行该任务？(【1】是 【2】否)\n");
    cout << "=====>>>>>  ";
    cin >> whether;
    if (whether == 1)
    {
        File_sourceload();
        printf("已经重新加载该任务！\n");
        if (if_go_next() == 0)
        {
            if_backto_menu();
        }

        printf("\n开始进行第2步：创建Huffman树！\n");
        printf("在创建之后，是否要查看Huffman树？(【1】查看  【2】不查看): \n");
        cin >> whether;
        File_read_char_num(L, text_length, char_number);
        HuffmanTree_Create(HT, char_number, L, whether);
        printf("\n创建Huffman树完毕！");
        if (if_go_next() == 0)
        {
            if_backto_menu();
        }

        printf("\n开始进行第3步：求每个字符的Huffman编码！\n");
        HuffmanTree_Code(HT, char_number, L);
        HuffmanCode_Write(HT, char_number, L);
        printf("\n字符Huffman编码生成完毕！");
        if (if_go_next() == 0)
        {
            if_backto_menu();
        }

        printf("\n开始进行第4步：对文件进行Huffman编码！\n");
        printf("在创建之后，是否要查看文件的Huffman编码？(【1】查看  【2】不查看): \n");
        cout << "=====>>>>>  ";
        cin >> whether;
        HuffmanCode_Encode(HT, L, whether);
        printf("\n文件Huffman编码完毕！");
        if (if_go_next() == 0)
        {
            if_backto_menu();
        }

        printf("\n开始进行第5步：根据编码生成.huf文件！\n");
        text_huffmancode_create();
        printf("\n生成.huf文件完毕！");
        if (if_go_next() == 0)
        {
            if_backto_menu();
        }

        printf("\n开始进行第6步：计算Huffman压缩率！\n");
        File_Zip_Percentage();
        printf("\n计算Huffman压缩率完毕！");
        if (if_go_next() == 0)
        {
            if_backto_menu();
        }

        printf("\n开始进行第7步：根据编码翻译.huf文件！\n");
        text_huffmancode_unzip();
        printf("\n根据编码翻译.huf文件完毕！");
        if (if_go_next() == 0)
        {
            if_backto_menu();
        }

        printf("\n开始进行第8步：对翻译得到的文件进行解码！\n");
        HuffmanCode_FileDecode(HT, char_number, text_length);
        printf("\n对文件进行解码完毕！");
        if (if_go_next() == 0)
        {
            if_backto_menu();
        }

        printf("\n开始进行第9步：比较输入文件与输出文件！\n");
        File_Error_Percentage();

        printf("\n所有操作执行完毕！\n");
        if_backto_menu();
    }
    else
    {
        system("cls");
        menu();
    }
}

/**实现主要逻辑*/
int main(int argc, char const *argv[])
{
    menu_0();//祖传登录界面
    system("color a");
    FILE *fp = fopen("huffman_temp\\char_frequency.txt", "r");
    //判断是否存在未完成任务
    if (fp != nullptr)
    {
        cout << "发现上一次程序运行时存在结束的任务！" << endl;
        Project_Recovery();
    }
    else { menu(); }//主要菜单界面
    system("pause");
}






