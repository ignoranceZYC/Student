#include <stdio.h>
#include <winsock.h>
#include <mysql.h>
#include <string.h>
#define buf 1024
#pragma comment( lib, "libmysql.lib")

//登陆页面
int chooseUser(int user)
{
	printf("请选择你的身份\n1、学生\n2、管理员\n");
	printf("你的选择：");
	scanf_s("%d",&user,10);
	if (user!=1)
		if (user != 2)
		{
			printf("您身份输入错误！");
			return 0;
		}
	return 1;
}



int main()
{
	char SqlText[256] = "";  //将用来保存要执行的SQL语句
	MYSQL* conn = NULL;      //MYSQL句柄类型(任何一个mysql操作都是基于MYSQL这个句柄来操作的)
	MYSQL_RES* res = NULL;   //数据查询结果集
	MYSQL_FIELD* fd = NULL; //MySQL表头域类型
	MYSQL_ROW row;           //一个行数据的类型安全(type-safe)的表示
	conn = mysql_init(NULL); //初始化MYSQL连接

	char  server[20] = "localhost"; //mysql服务器的IP
	char  user[20] = "root";        //用户名
	char  psd[20] = "123";       ///密码  需要改为自己的密码
	char  dbName[1024] = "student";    ///数据库名 需要改为自己的数据库名
	unsigned short port = 3306;     //服务器端口号,默认3306

	int user_C=0;

	//连接数据库
	if (mysql_real_connect(conn, server, user, psd, dbName, port, NULL, 0) == 0) {
		printf("Error connecting to database: %s\n", mysql_error(conn));
	}
	else {
		puts("数据库连接成功！");
		mysql_query(conn, "set names 'GBK'");//设置字符集,防止中文无法正常显示[可选操作]
	}
	if(chooseUser(user_C))
		return 0;

	return 0;
}