#include <stdio.h>
#include <winsock.h>
#include <mysql.h>
#include <string.h>
#define buf 1024
#pragma comment( lib, "libmysql.lib")

MYSQL_RES* res=NULL,*res2 = NULL;   //数据查询结果集
MYSQL_FIELD* fd = NULL; //MySQL表头域类型
MYSQL_ROW row;           //一个行数据的类型安全(type-safe)的表示

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
int User(char* Sql, int user_A, MYSQL* conn, int &user_B)
{
	int i;
	MYSQL_RES* res = NULL;   //数据查询结果集
	MYSQL_FIELD* fd = NULL; //MySQL表头域类型
	//MYSQL_ROW row;
	char psd_B[20];
	printf("请输入你账号\n");
	scanf_s("%d", &user_B, 20);
	printf("请输入你的密码\n");
	scanf_s("%s", psd_B, 20);
	sprintf(Sql, "select * from student.StudentT where (id='%d') and (pwd='%s') and( power>='%d');",user_B,psd_B,user_A );
	i = mysql_query(conn, Sql);
	res = mysql_store_result(conn);
	if ((i== 0) && (mysql_num_rows(res) != 0)) {       //执行成功则把结果输出
		
		system("cls");
		printf("登陆成功");
		mysql_free_result(res); //释放结果集资源
	}
	else {
		printf("登陆失败: %s\n", mysql_error(conn));
		exit(0);
		return 0;
	}
}

int Show_self(int user_B, MYSQL* conn) { 

	char Sql[256] = "";  //将用来保存要执行的SQL语句
	system("cls");
	sprintf(Sql, "SELECT* FROM information_schema.`COLUMNS` where TABLE_SCHEMA='student' and TABLE_NAME='ScoreT' order by ORDINAL_POSITION;");
	if ((mysql_query(conn, Sql) == 0)) {       //执行成功则把结果输出
		res = mysql_store_result(conn);      //存储查询得到的结果集
		int fieldNums = mysql_num_fields(res); //获取数据结果每条记录的列数
		while ((row = mysql_fetch_row(res)) != NULL) { //不断获取下一组结果
				printf("%-10s|", row[3]);
		}
		printf("\n");
		sprintf(Sql, "select * from student.ScoreT where (id='%d');", user_B);
		if ((mysql_query(conn, Sql) == 0)){       //执行成功则把结果输出
		res = mysql_store_result(conn);      //存储查询得到的结果集
		int fieldNums = mysql_num_fields(res); //获取数据结果每条记录的列数
		while ((row = mysql_fetch_row(res)) != NULL) { //不断获取下一组结果
			for (int i = 0; i < fieldNums; i++) {
				printf("%-10s ", row[i]);
			}
			puts("");
		}
		mysql_free_result(res); //释放结果集资源
		//打印获取的数据
		}
	}
	else {
		printf("查询失败: %s\n", mysql_error(conn));
		return 0;
	}
	return 0;

}

int Show_Class( MYSQL* conn) {

	char Sql[256] = "";  //将用来保存要执行的SQL语句
	system("cls");
	sprintf(Sql, "SELECT* FROM information_schema.`COLUMNS` where TABLE_SCHEMA='student' and TABLE_NAME='ClassT' order by ORDINAL_POSITION;");
	if ((mysql_query(conn, Sql) == 0)) {       //执行成功则把结果输出
		res = mysql_store_result(conn);      //存储查询得到的结果集
		int fieldNums = mysql_num_fields(res); //获取数据结果每条记录的列数
		while ((row = mysql_fetch_row(res)) != NULL) { //不断获取下一组结果
			printf("%-10s|", row[3]);
		}
		printf("\n");
		sprintf(Sql, "select * from student.ClassT" );
		if ((mysql_query(conn, Sql) == 0)) {       //执行成功则把结果输出
			res = mysql_store_result(conn);      //存储查询得到的结果集
			int fieldNums = mysql_num_fields(res); //获取数据结果每条记录的列数
			while ((row = mysql_fetch_row(res)) != NULL) { //不断获取下一组结果
				for (int i = 0; i < fieldNums; i++) {
					printf("%-10s ", row[i]);
				}
				puts("");
			}
			mysql_free_result(res); //释放结果集资源
			//打印获取的数据
		}
	}
	else {
		printf("查询失败: %s\n", mysql_error(conn));
		return 0;
	}
}

int Show_Stu(MYSQL* conn) {

	char Sql[256] = "";  //将用来保存要执行的SQL语句
	system("cls");

	sprintf(Sql, "select a.id, a.name, c.No, c.name, b.Score from StudentT a, ScoreT b, ClassT c where(a.id = b.id) and (b.No = c.No)");
	printf("Id         |Name     |No       |ClassName |Score\n");
	if ((mysql_query(conn, Sql) == 0)) {       //执行成功则把结果输出
		res = mysql_store_result(conn);      //存储查询得到的结果集
		int fieldNums = mysql_num_fields(res); //获取数据结果每条记录的列数
		while ((row = mysql_fetch_row(res)) != NULL) { //不断获取下一组结果

				for (int i = 0; i < fieldNums; i++) {
					printf("%-10s ", row[i]);
				}
				puts("");
		}
		mysql_free_result(res); //释放结果集资源
		//打印获取的数据
		
	}
	else {
		printf("查询失败: %s\n", mysql_error(conn));
		return 0;
	}
}

int Add_Stu(MYSQL* conn)
{
	int choose;
	char tableName[30];
	char Sql[256] = "";  //将用来保存要执行的SQL语句
	int j=0;
	char type[256];
	system("cls");

	printf("\n--------------------------------\n");
	printf("|        1 插入课程信息        |\n");
	printf("|        2 插入学生信息        |\n");
	printf("|        3 插入成绩信息        |\n");
	printf("|        4 退出界面            |\n");
	printf("--------------------------------\n");

	printf("--------------------------------\n");
	printf("请输入你的选择：");
	scanf_s("%d", &choose);
	switch (choose)
	{
	case 1:  sprintf(tableName, "ClassT"); break;
	case 2:  sprintf(tableName, "StudentT"); break;
	case 3:  sprintf(tableName, "ScoreT"); break;
	default:  return 0;
	}

	sprintf(Sql, "SELECT* FROM information_schema.`COLUMNS` where TABLE_SCHEMA='student' and TABLE_NAME='%s' order by ORDINAL_POSITION;", tableName);
	if ((mysql_query(conn, Sql) == 0)) {       //执行成功则把结果输出
		res = mysql_store_result(conn);      //存储查询得到的结果集
		int fieldNums = mysql_num_fields(res); //获取数据结果每条记录的列数
		j=sprintf(Sql, "insert into %s values(", tableName);
		while ((row = mysql_fetch_row(res)) != NULL) { //不断获取下一组结果
			printf("%-5s:", row[3]);
			scanf_s("%s", type,256);
			j += sprintf(Sql + j, "\'%s\',",type);
		}
		sprintf(Sql + j-1, ");");
		printf("%s", Sql);
		//system("cls");
	}
	if ((mysql_query(conn, Sql) != 0))
	{
	printf("添加失败: %s\n", mysql_error(conn));
	return 0;
	}
}

int Del_Stu(MYSQL* conn)
{
	int choose;
	char tableName[30];
	char Sql[256] = "";  //将用来保存要执行的SQL语句
	int j = 0;
	char type[256];
	system("cls");

	printf("\n--------------------------------\n");
	printf("|        1 删除课程信息        |\n");
	printf("|        2 删除学生信息        |\n");
	printf("|        3 退出界面            |\n");
	printf("--------------------------------\n");

	printf("--------------------------------\n");
	printf("请输入你的选择：");
	scanf_s("%d", &choose);
	switch (choose)
	{
	case 1:  sprintf(tableName, "ClassT"); break;
	case 2:  sprintf(tableName, "StudentT"); break;
	default:  return 0;;
	}
	printf("请输入条件");
	if (scanf_s("%s", type, 256))
	{
		sprintf(Sql, "delete from %s where %s", tableName, type);
		if ((mysql_query(conn, Sql) != 0))
		{
			printf("删除失败: %s\n", mysql_error(conn));
			return 0;
		}
		else
			printf("删除成功");
	}
	return 0;
}

int Change_Stu(MYSQL* conn)
{
	int choose;
	char tableName[30];
	char Sql[256] = "";  //将用来保存要执行的SQL语句
	int j = 0;
	char type[256];
	char type1[256];
	system("cls");

	printf("\n--------------------------------\n");
	printf("|        1 修改课程信息        |\n");
	printf("|        2 修改学生信息        |\n");
	printf("|        3 退出界面            |\n");
	printf("--------------------------------\n");

	printf("--------------------------------\n");
	printf("请输入你的选择：");
	scanf_s("%d", &choose);
	switch (choose)
	{
	case 1:  sprintf(tableName, "ClassT"); break;
	case 2:  sprintf(tableName, "StudentT"); break;
	default:  return 0;;
	}
	printf("请输入条件");
	if (scanf_s("%s", type, 256))
	{
		printf("请输入更改内容:");
		scanf_s("%s", type1, 256);
		sprintf(Sql, "update %s set %s where %s ;", tableName, type1, type);
		printf("%s", Sql);
		if ((mysql_query(conn, Sql) != 0))
		{
			printf("修改失败: %s\n", mysql_error(conn));
			return 0;
		}
		else
			printf("修改成功");
	}
	return 0;
}

//用户界面
int win(int user_A,int user_B, MYSQL* conn, char*  server)
{
	int choose;
	printf("\n--------------------------------\n");
	printf("|        1 显示个人信息        |\n");
	printf("|        2 查询课程信息        |\n");
	if (user_A == 2) {
		printf("|        3 显示学生信息        |\n");
		printf("|        4 添加学生信息        |\n");
		printf("|        5 删除学生信息        |\n");
		printf("|        6 修改学生信息        |\n");
	}
		printf("|        7 退出界面            |\n");
	
	printf("--------------------------------\n");
	printf("请输入你的选择：");
	scanf_s("%d",&choose );
	switch (choose)
	{
	case 1: Show_self(user_B,conn); break;
	case 2: Show_Class(conn);break;
	case 3: Show_Stu(conn); break;
	case 4: Add_Stu(conn);break;
	case 5:	Del_Stu(conn); break;
	case 6: Change_Stu(conn); break;
	case 7:
		{
			system("cls");  //先清屏，再退出
			exit(0);
		} break;
	default:  break;
	}

	return 0;
}

int main()
{
	char Sql[256] = "";  //将用来保存要执行的SQL语句
	MYSQL* conn = NULL;      //MYSQL句柄类型(任何一个mysql操作都是基于MYSQL这个句柄来操作的)
	 res = NULL;   //数据查询结果集
	 fd = NULL; //MySQL表头域类型

	conn = mysql_init(NULL); //初始化MYSQL连接

	char  server[20] = "39.106.76.148"; //mysql服务器的IP
	char  user[20] = "root";        //用户名
	char  psd[20] = "123";       ///密码  需要改为自己的密码
	char  dbName[1024] = "student";    ///数据库名 需要改为自己的数据库名
	unsigned short port = 3306;     //服务器端口号,默认3306

	int user_A=0;
	int id = 0;
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
	User( Sql, user_A, conn,id);
	while (1)
	{
		win(user_A, id,conn,server);
	}
	mysql_close(conn); //关闭连接，即释放连接
	return 0;
}


