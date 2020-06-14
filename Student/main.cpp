#include <stdio.h>
#include <winsock.h>
#include <mysql.h>
#include <string.h>
#define buf 1024
#pragma comment( lib, "libmysql.lib")

//登陆页面
int chooseUser(int &user)
{
	printf("请选择你的身份\n1、学生\n2、管理员\n");
	printf("你的选择：");
	scanf_s("%d",&user,10);
	if (user!=1)
		if (user != 2)
		{
			printf("你的身份输入错误！");
			return 0;
		}
	system("cls");
	return 1;
}

//账户登录
int User(char* Sql, int user_A, MYSQL* conn)
{
	MYSQL_RES* res = NULL;   //数据查询结果集
	MYSQL_FIELD* fd = NULL; //MySQL表头域类型
	MYSQL_ROW row;
	int user_B;
	char psd_B[20];
	printf("请输入你账号\n");
	scanf_s("%d", &user_B, 20);
	printf("请输入你的密码\n");
	scanf_s("%s", psd_B, 20);
	sprintf(Sql, "select * from student.StudentT where (id='%d') and (pwd='%s') and( power>='%d');",user_B,psd_B,user_A );
	if (mysql_query(conn, Sql) == 0) {       //执行成功则把结果输出
		system("cls");
		printf("登陆成功");
		mysql_free_result(res); //释放结果集资源
	}
	else {
		printf("登陆失败: %s\n", mysql_error(conn));
		return 0;
	}
}
/*
int Show_self()
int Show_Class()
int Show_Stu()
int Add_Stu() 
int Del_Stu()
int Change_Stu()
*/
//用户界面
int win(int user_A)
{
	int choose;
	printf("\n--------------------------------\n");
	printf("|        1 查询个人信息        |\n");
	printf("|        2 查询课程信息        |\n");
	if (user_A == 2) {
		printf("|        3 显示学生信息        |\n");
		printf("|        4 添加学生信息        |\n");
		printf("|        5 删除学生信息        |\n");
		printf("|        6 修改学生信息        |\n");
		printf("|        7 退出界面            |\n");
	}
	printf("--------------------------------\n");
	printf("请输入你的选择：");
/*	scanf_s("%d",&choose );
	switch (choose)
	{
	case 1: Show_self(); break;
	case 2: Show_Class(); break;
	case 3: Show_Stu(); break;
	case 4: Add_Stu(); break;
	case 5: Change_Stu(); break;
	case 6:
	case 7:
		{
			system("cls");  //先清屏，再退出
			exit(0);
		} break;
	default:  break;
	}
*/
	return 0;
}

int main()
{
	char Sql[256] = "";  //将用来保存要执行的SQL语句
	MYSQL* conn = NULL;      //MYSQL句柄类型(任何一个mysql操作都是基于MYSQL这个句柄来操作的)
	MYSQL_RES* res = NULL;   //数据查询结果集
	MYSQL_FIELD* fd = NULL; //MySQL表头域类型
	MYSQL_ROW row;           //一个行数据的类型安全(type-safe)的表示
	conn = mysql_init(NULL); //初始化MYSQL连接

	char  server[20] = "39.106.76.148"; //mysql服务器的IP
	char  user[20] = "root";        //用户名
	char  psd[20] = "123";       ///密码  需要改为自己的密码
	char  dbName[1024] = "student";    ///数据库名 需要改为自己的数据库名
	unsigned short port = 3306;     //服务器端口号,默认3306

	int user_A=0;

	//连接数据库
	if (mysql_real_connect(conn, server, user, psd, dbName, port, NULL, 0) == 0) {
		printf("Error connecting to database: %s\n", mysql_error(conn));
	}
	else {
		puts("数据库连接成功！");
		mysql_query(conn, "set names 'GBK'");//设置字符集,防止中文无法正常显示[可选操作]
	}
	if(!chooseUser(user_A))
		return 0;
	User( Sql, user_A, conn);
	win(user_A);

	return 0;
}