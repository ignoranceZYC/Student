
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mysql/mysql.h"

MYSQL* g_conn;//mysql ����
MYSQL_RES* g_res;//mysql ��¼��
MYSQL_ROW g_row;//�ַ������飬mysql ��¼��

#define MAX_BUF_SIZE 1024 //����������ֽ���
/*=================================================================*/
/**/const char* g_host_name = "localhost";
/**/const char* g_user_name = "yinxiaofei";
/**/const char* g_password = "yinxiaofei";
/**/const char* g_db_name = "test";
/**/const unsigned int g_db_port = 3306;
/*=================================================================*/
char sql[MAX_BUF_SIZE];
char Time[MAX_BUF_SIZE];

int iNum_rows = 0;//mysql���ִ�н��������������ֵ
int flag = 0;//����ԱȨ�޿���
int i = 1;//ϵͳ���п���

//��¼ʹ�õĽṹ��
struct Login
{
	char name[24];
	char password[20];
}login;
//����ʹ�õĽṹ��
struct Operation
{
	char tables[24];
	char name[24];
	char passwd[20];
	char role[24];
	char remark[20];
}ope;
//��ӡ������Ϣ
void print_mysql_error(const char* msg)
{
	if (msg)
		printf("%s: %s\n", msg, mysql_error(g_conn));
	else
		puts(mysql_error(g_conn));
}
//ִ��sql��䣬�ɹ�����0��ʧ�ܷ���-1
int executesql(const char* sql)
{
	//
	if (mysql_real_query(g_conn, sql, strlen(sql)))
		return -1;
	return 0;
}
//��ʼ������
int init_mysql()
{
	//init the database connection
	g_conn = mysql_init(NULL);
	//connection the database
	if (!mysql_real_connect(g_conn, g_host_name, g_user_name, g_password, g_db_name, g_db_port, NULL, 0))
		return -1;//����ʧ��
	//����Ƿ����ʹ��
	if (executesql("set names utf8"))
		return -1;
	return 0;//���سɹ�
}
//ѡ�����ݿ⣬û�е�ʱ�򴴽�
void create_database()
{
	sprintf(sql, "use project");
	if (executesql(sql) == -1)
	{
		puts("create database");
		executesql("create database project;");
		print_mysql_error(NULL);
		puts("choice database");
		executesql("use project;");
		print_mysql_error(NULL);
		puts("!!!Initialize the success!!!");
	}
	else
	{
		executesql("use project;");
		print_mysql_error(NULL);
	}
}
//�鿴���������
void create_table()
{
	//users��ļ���봴��
	sprintf(sql, "show tables;");
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	if (iNum_rows == 0)
	{
		puts("create users table");
		executesql("create table users(id_ smallint unsigned primary key auto_increment,name_ varchar(24) not null unique,password_ char(20) not null,create_time_ datetime not null,creator_id_ smallint unsigned,remark_ varchar(200),foreign key(creator_id_) references users(id_));");
		puts("create roles table");
		executesql("create table roles(id_ smallint unsigned primary key auto_increment,name_ varchar(24) not null unique,remark_ varchar(200));");
		puts("create authoritys table");
		executesql("create table authoritys(id_ smallint unsigned primary key auto_increment,name_ varchar(24) not null unique,remark_ varchar(200));");
		puts("create roleAuthority table");
		executesql("create table roleAuthority(role_id_ smallint unsigned,authority_id_ smallint unsigned,primary key(role_id_,authority_id_),foreign key(role_id_ ) references roles(id_ ),foreign key(authority_id_) references authoritys(id_));");
		puts("create userRole table");
		executesql("create table userRole(user_id_ smallint unsigned,role_id_ smallint unsigned,primary key(user_id_,role_id_),foreign key(user_id_) references users(id_),foreign key(role_id_ ) references roles(id_));");
	}
	mysql_free_result(g_res); // �ͷŽ����
}
//��ʼ������Ա�˻�
void init_Administrtor()
{
	//��ѯusers��
	sprintf(sql, "select * from users where id_='1' and name_='root';");
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	if (iNum_rows == 0)
	{
		puts("Init Administrtor User");
		//�������Ա�û�
		sprintf(sql, "insert into users values(1,'root','root','2017-08-18 12:21:11',1,'administrtor');");
		executesql(sql);
	}
	mysql_free_result(g_res); // �ͷŽ����
	//��ѯroles��
	sprintf(sql, "select * from roles;");
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	if (iNum_rows < 3)
	{
		puts("Init System Role");
		//����ϵͳ��ɫ
		sprintf(sql, "insert into roles values(1,'ADMINISTRTOR','administrtor');");
		executesql(sql);
		sprintf(sql, "insert into roles values(2,'USER','user');");
		executesql(sql);
		sprintf(sql, "insert into roles values(3,'OTHER','other');");
		executesql(sql);
	}
	mysql_free_result(g_res); // �ͷŽ����
	//��ѯauthoritys��
	sprintf(sql, "select * from authoritys;");
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	if (iNum_rows < 3)
	{
		puts("Init System Authority");
		//����ϵͳȨ��
		sprintf(sql, "insert into authoritys values(1,'QUERY','query');");
		executesql(sql);
		sprintf(sql, "insert into authoritys values(2,'ADD','addmsg');");
		executesql(sql);
		sprintf(sql, "insert into authoritys values(3,'ADD&QUERY','query and addmsg');");
		executesql(sql);
		sprintf(sql, "insert into authoritys values(4,'DEL&ALT','delete and alter');");
		executesql(sql);
		sprintf(sql, "insert into authoritys values(5,'QUERY&DEL&ALT','query and delete and alter');");
		executesql(sql);
		sprintf(sql, "insert into authoritys values(6,'ADD&DEL&ALT','addmsg and delete and alter');");
		executesql(sql);
		sprintf(sql, "insert into authoritys values(7,'QUERY&ADD&DEL&ALT','query and addmsg and delete and alter');");
		executesql(sql);
	}
	mysql_free_result(g_res); // �ͷŽ����
	//��ѯroleAuthority��
	sprintf(sql, "select * from roleAuthority;");
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	if (iNum_rows < 3)
	{
		puts("Init RoleAuthority");
		//�����ɫȨ��
		sprintf(sql, "insert into roleAuthority values(1,7);");
		executesql(sql);
		sprintf(sql, "insert into roleAuthority values(2,3);");
		executesql(sql);
		sprintf(sql, "insert into roleAuthority values(3,1);");
		executesql(sql);
	}
	mysql_free_result(g_res); // �ͷŽ����
	//��ѯuserRole��
	sprintf(sql, "select * from userRole where user_id_='1' and role_id_='1';");
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	if (iNum_rows == 0)
	{
		puts("Init User Role");
		//�������Ա�û�
		sprintf(sql, "insert into userRole values(1,1);");
		executesql(sql);
	}
	mysql_free_result(g_res); // �ͷŽ����
}
//�û���¼
void user_login()
{
	puts("Init success! Please press any key to continue");
	while (1)
	{
		while ((getchar()) != '\n');
		system("clear");
		puts("!!!Login System!!!");
		//�����˻�������
		printf("Name��"); scanf("%s", login.name);
		printf("Passwd��"); scanf("%s", login.password);
		//�����ݿ��в�ѯ���ɲ�ѯ����Ϣ������users�������˺���Ϣ����¼�ɹ�
		sprintf(sql, "select * from users where name_='%s' and password_='%s';", login.name, login.password);
		executesql(sql);
		g_res = mysql_store_result(g_conn);
		iNum_rows = mysql_num_rows(g_res);
		if (iNum_rows != 0)
		{
			puts("!!! Login Success !!! ");
			while ((getchar()) != '\n');
			getchar();
			break;
		}
		else
		{
			puts("!!!Login Failed!!! Check name or password!");
			while ((getchar()) != '\n');
		}
	}
	mysql_free_result(g_res); // �ͷŽ����
}
//��ѯ����
void query_msg()
{
	while (1) {
		int i, j;
		int choice;
		system("clear");
		puts("!!!    Query_msg    !!! ");
		puts("!!!   choice table  !!! ");
		puts("!!! 1:    users     !!! ");
		puts("!!! 2:    roles     !!! ");
		puts("!!! 3:    userRole  !!! ");
		puts("!!! 0:    Return    !!! ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			sprintf(ope.tables, "users");
			system("clear");
			puts("!!!   enter name  !!! ");
			printf("Name��"); scanf("%s", ope.name);
			//��ָ�����в�ѯ�û��������Ϣ
			sprintf(sql, "select * from %s where name_='%s';", ope.tables, ope.name);
			executesql(sql);
			g_res = mysql_store_result(g_conn);
			iNum_rows = mysql_num_rows(g_res); // �õ���¼������
			system("clear");
			if (iNum_rows == 0)
				puts("No such person!");
			else
			{
				int iNum_fields = mysql_num_fields(g_res); // �õ���¼������
				puts("id_  | name_ |password_| create_time_     |creator_id_ | remark_   ");
				while ((g_row = mysql_fetch_row(g_res)))
					printf("%s\t%s\t%s\t%s\t%s\t%s\n", g_row[0], g_row[1], g_row[2], g_row[3], g_row[4], g_row[5]);
			}
			mysql_free_result(g_res);
			while ((getchar()) != '\n');
			getchar();
			break;
		case 2:
			sprintf(ope.tables, "roles");
			system("clear");
			//��ѯ��ɫ��
			sprintf(sql, "select * from %s;", ope.tables);
			executesql(sql);
			g_res = mysql_store_result(g_conn);
			iNum_rows = mysql_num_rows(g_res); // �õ���¼������
			system("clear");
			if (iNum_rows == 0)
				puts("No such person!");
			else
			{
				int iNum_fields = mysql_num_fields(g_res); // �õ���¼������
				puts(" id_  | name_         | remark_      ");
				while ((g_row = mysql_fetch_row(g_res)))
					printf("%s\t%s\t\t%s\n", g_row[0], g_row[1], g_row[2]);
			}
			mysql_free_result(g_res);
			while ((getchar()) != '\n');
			getchar();
			break;
		case 3:
			sprintf(ope.tables, "userRole");
			system("clear");
			//��ѯ�û���ɫ��
			sprintf(sql, "select * from %s;", ope.tables);
			executesql(sql);
			g_res = mysql_store_result(g_conn);
			iNum_rows = mysql_num_rows(g_res); // �õ���¼������
			system("clear");
			if (iNum_rows == 0)
				puts("No such person!");
			else
			{
				int iNum_fields = mysql_num_fields(g_res); // �õ���¼������
				puts(" user_id_ | role_id_ ");
				while ((g_row = mysql_fetch_row(g_res)))
					printf("\t%s\t%s\n", g_row[0], g_row[1]);
			}
			mysql_free_result(g_res);
			while ((getchar()) != '\n');
			getchar();
			break;
		case 0:
			return;
		default:
			puts("!!! enter right choice !!! ");
			while ((getchar()) != '\n');
			getchar();
			break;
		}
	}
}
//Ȩ���ж�����
int judge(int authority)
{
	int auth = 0;
	//ͨ����ǰ��¼���û����ֻ�ȡ�û�id
	sprintf(sql, "select id_ from users where name_='%s';", login.name);
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	int iNum_fields = mysql_num_fields(g_res);
	while ((g_row = mysql_fetch_row(g_res))) {
		//ͨ����ǰ��¼�û���id��ѯ����û��Ľ�ɫid
		sprintf(sql, "select role_id_ from userRole where user_id_='%s';", g_row[0]);
	}
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	iNum_fields = mysql_num_fields(g_res);
	while ((g_row = mysql_fetch_row(g_res))) {
		//ͨ����ǰ�û��Ľ�ɫid��ѯ���û���Ȩ��id
		sprintf(sql, "select authority_id_ from roleAuthority where role_id_='%s';", g_row[0]);
	}
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	iNum_fields = mysql_num_fields(g_res);
	while ((g_row = mysql_fetch_row(g_res))) {
		//ͨ����ǰ�û���Ȩ��id��ѯȨ�޼���
		sprintf(sql, "select name_ from authoritys where id_='%s';", g_row[0]);
	}
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	iNum_fields = mysql_num_fields(g_res);
	while ((g_row = mysql_fetch_row(g_res))) {
		if (strcmp(g_row[0], "QUERY") == 0)
			auth = 1;
		if (strcmp(g_row[0], "ADD") == 0)
			auth = 2;
		if (strcmp(g_row[0], "ADD&QUERY") == 0)
			auth = 3;
		if (strcmp(g_row[0], "DEL&ALT") == 0)
			auth = 4;
		if (strcmp(g_row[0], "QUERY&DEL&ALT") == 0)
			auth = 5;
		if (strcmp(g_row[0], "ADD&DEL&ALT") == 0)
			auth = 6;
		if (strcmp(g_row[0], "QUERY&ADD&DEL&ALT") == 0)
			auth = 7;
		if (auth >= 4)
			flag = 1;//����ԱȨ�޿���
		//�жϸ��û�Ȩ�޼����ܷ�ִ�н�Ҫ���еĲ���
		if (auth < authority)
		{
			mysql_free_result(g_res); // �ͷŽ����
			return 0;
		}
		else
		{
			mysql_free_result(g_res); // �ͷŽ����
			return 1;
		}
	}
}
//��Ӻ���
void add_msg()
{
	int o;
	char ID[20];
	//ͨ��Ȩ���ж������ķ���ֵ�����Ƿ���Խ�������û�����
	if (judge(2) == 0) {
		puts("!!!Insufficient permissions!!! ");
		while ((getchar()) != '\n');
		getchar();
		//Ȩ�޲������˳�����
		return;
	}
	//����ִ��
	system("clear");
	puts("!!!    Add_user   !!! ");
	//���ݵ�ǰ�����û��������жϣ��½����û�idӦΪ����+1
	sprintf(sql, "select id_ from users;");
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res); // �õ���¼������
	int i = iNum_rows + 1;//���û�id
	//�����˻�������
	printf("    Name��"); scanf("%s", ope.name);
	printf("Password��"); scanf("%s", ope.passwd);
	//��ȡϵͳʱ�䣬��Ϊ����ʱ��
	time_t temp;
	struct tm* t;
	time(&temp);
	t = localtime(&temp);
	sprintf(Time, "%d-%d-%d %d:%d:%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	//ͨ����ǰ��¼���û����ֻ�ȡ�û�id����Ϊ������idʹ�� 		
	sprintf(sql, "select id_ from users where name_='%s';", login.name);
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res);
	int iNum_fields = mysql_num_fields(g_res);
	while ((g_row = mysql_fetch_row(g_res))) {
		sprintf(ID, "%s", g_row[0]);
	}
	//��ע		
	printf("  Remark��"); scanf("%s", ope.remark);
	//���û����в���һ���µ��û�����Ϣ
	sprintf(sql, "insert into users values(%d,'%s','%s','%s',%s,'%s');", i, ope.name, ope.passwd, Time, ID, ope.remark);
	executesql(sql);
	//����Ա�û��趨Ȩ��
	if (flag)
	{
		while (1) {
			system("clear");
			puts("!!!    ROLE   !!! ");
			puts("!!! 1: USER   !!! ");
			puts("!!! 2:OTHER   !!! ");
			printf("Choice the Role of user %s��", ope.name); scanf("%d", &o);
			switch (o) {
			case 1:
				sprintf(ope.role, "USER");
				break;
			case 2:
				sprintf(ope.role, "OTHER");
				break;
			default:
				puts("!!! enter right choice !!! ");
				while ((getchar()) != '\n');
				getchar();
			}
			break;
		}
		//ͨ����ɫ���ҵ���ɫid
		sprintf(sql, "select id_ from roles where name_='%s';", ope.role);
		executesql(sql);
		g_res = mysql_store_result(g_conn);
		iNum_rows = mysql_num_rows(g_res);
		iNum_fields = mysql_num_fields(g_res);
		while ((g_row = mysql_fetch_row(g_res))) {
			//���û�id�ͽ�ɫidд���û���ɫ��
			sprintf(sql, "insert into userRole values(%d,%s);", i, g_row[0]);
			executesql(sql);
		}
	}
	//���û�й���ԱȨ�ޣ�Ĭ����ӵ��û���ɫΪother
	else {
		sprintf(sql, "insert into userRole values(%d,3);", i);
		executesql(sql);
	}
	flag = 0;//����ԱȨ�޿���
	puts("!!! success !!! ");
	while ((getchar()) != '\n');
	getchar();
}
//ɾ�ĺ���
void del_alt_msg()
{
	int o, op;
	char p;
	char ID[20];
	//ͨ��Ȩ���ж������ķ���ֵ�����Ƿ���Խ���ɾ���û�����
	if (judge(4) == 0) {
		puts("!!!Insufficient permissions!!! ");
		while ((getchar()) != '\n');
		getchar();
		//Ȩ�޲������˳�����
		return;
	}
	//����ִ��
	system("clear");
	puts("!!!     del_alt_msg  !!! ");
	printf("    Name��"); scanf("%s", ope.name);
	printf("Password��"); scanf("%s", ope.passwd);
	//�ж�Ҫ����ɾ�ĵ��û��ǲ��ǹ���Ա�û�����ֹ�Թ���Ա�û�����ɾ�Ĳ���
	if (strcmp(ope.name, "root") == 0)
	{
		puts("Administrator user deletion is prohibited");
		while ((getchar()) != '\n');
		getchar();
		return;
	}
	//ͨ���û���������鿴�û������Ƿ��и��û�
	sprintf(sql, "select id_ from users where name_='%s' and password_='%s';", ope.name, ope.passwd);
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res); // �õ���¼������
	int iNum_fields = mysql_num_fields(g_res);
	//�����û�idȡ��������
	while ((g_row = mysql_fetch_row(g_res))) {
		sprintf(ID, "%s", g_row[0]);
	}
	//û�в鵽
	if (iNum_rows == 0)
	{
		puts("No such person!");
		puts("!!! enter right choice !!! ");
		while ((getchar()) != '\n');
		getchar();
	}
	//����ɾ��ѡ��
	else
	{
		system("clear");
		puts("!!!    del_alt_msg    !!! ");
		puts("!!!  1:change  name   !!! ");
		puts("!!!  2:change passwd  !!! ");
		puts("!!!  3:change  role   !!! ");
		puts("!!!  4:change remark  !!! ");
		puts("!!!  5:delete  user   !!! ");
		printf("!!!      choice��     !"); scanf("%d", &o);
		switch (o)
		{
		case 1:
			system("clear");
			puts("!!!    del_alt_msg    !!! ");
			printf("!!!    enter name: "); scanf("%s", ope.name);
			//�����û���
			sprintf(sql, "update users set name_='%s' where id_=%s;", ope.name, ID);
			executesql(sql);
			break;
		case 2:
			system("clear");
			puts("!!!    del_alt_msg    !!! ");
			printf("!!!    enter password: "); scanf("%s", ope.passwd);
			//��������
			sprintf(sql, "update users set password_='%s' where id_=%s;", ope.passwd, ID);
			executesql(sql);
			break;
		case 3:
			system("clear");
			puts("!!!    del_alt_msg    !!! ");
			puts("!!!      1.USER       !!! ");
			puts("!!!      2.OTHER      !!! ");
			printf("!!!      choice��     !"); scanf("%d", &op);
			switch (op)
			{
			case 1:
				//���ý�ɫΪ�û�
				sprintf(sql, "update userRole set role_id_=2 where user_id_=%s;", ID);
				executesql(sql);
				break;
			case 2:
				//���ý�ɫΪ������
				sprintf(sql, "update userRole set role_id_=3 where user_id_=%s;", ID);
				executesql(sql);
				break;
			default:
				puts("!!! enter right choice !!! ");
				while ((getchar()) != '\n');
				getchar();
			}
			break;
		case 4:
			system("clear");
			puts("!!!    del_alt_msg    !!! ");
			printf("!!!    enter remark: "); scanf("%s", ope.remark);
			//���±�ע
			sprintf(sql, "update users set remark_='%s' where id_=%s;", ope.remark, ID);
			executesql(sql);
			break;
		case 5:
			system("clear");
			puts("!!!    del_alt_msg    !!! ");
			printf("!!!    sure delete? (Y/N):"); scanf("%s", &p);
			switch (p)
			{
			case 'Y':
			case 'y':
				//��Ҫ��ɾ���û���ɫ���е���Ϣ���ſ�ɾ���û����е���Ϣ
				sprintf(sql, "delete from userRole where user_id_=%s;", ID);
				executesql(sql);
				sprintf(sql, "delete from users where id_=%s;", ID);
				executesql(sql);
				break;
			case 'N':
			case 'n':
				return;
			}
			break;
		default:
			puts("!!! enter right choice !!! ");
			while ((getchar()) != '\n');
			getchar();
		}
		puts("!!! success !!! ");
		mysql_free_result(g_res);
		while ((getchar()) != '\n');
		getchar();
	}
}
//��ʾ�����û����û���ɫ����
void display()
{
	//ͨ��Ȩ���ж������ķ���ֵ�����Ƿ���Խ�����ʾ�����û�������other�û�����ʹ�ô˹���
	if (judge(2) == 0) {
		puts("!!!Insufficient permissions!!! ");
		while ((getchar()) != '\n');
		getchar();
		//Ȩ�޲������˳�����
		return;
	}
	//����ִ��
	//��ѯusers��
	sprintf(sql, "select * from users;");
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res); // �õ���¼������
	int iNum_fields = mysql_num_fields(g_res); // �õ���¼������
	system("clear");
	puts("!!!      users table   !!! \n");
	puts("id_  | name_ |password_| create_time_     |creator_id_ | remark_   ");
	while ((g_row = mysql_fetch_row(g_res)))
		printf("%s\t%s\t%s\t%s\t%s\t%s\n", g_row[0], g_row[1], g_row[2], g_row[3], g_row[4], g_row[5]);
	//��ѯroles��
	sprintf(sql, "select * from roles;");
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res); // �õ���¼������
	iNum_fields = mysql_num_fields(g_res); // �õ���¼������
	puts("\n\n!!!      roles table   !!! \n");
	puts(" id_  | name_         | remark_      ");
	while ((g_row = mysql_fetch_row(g_res)))
		printf("%s\t%s\t\t%s\n", g_row[0], g_row[1], g_row[2]);
	//��ѯuserRole��
	sprintf(sql, "select * from userRole;");
	executesql(sql);
	g_res = mysql_store_result(g_conn);
	iNum_rows = mysql_num_rows(g_res); // �õ���¼������
	iNum_fields = mysql_num_fields(g_res); // �õ���¼������
	puts("\n\n!!!    userRole table  !!! \n");
	puts(" user_id_ | role_id_ ");
	while ((g_row = mysql_fetch_row(g_res)))
		printf("\t%s\t%s\n", g_row[0], g_row[1]);
	mysql_free_result(g_res);
	while ((getchar()) != '\n');
	getchar();
}
//�����˵�
void menu()
{
	while (i)
	{
		int choice;
		system("clear");
		puts("!!!     choice��  !!! ");
		puts("!!! 1:query   msg !!! ");
		puts("!!! 2:  add  user !!! ");
		puts("!!! 3:del|alt msg !!! ");
		puts("!!! 4:display all !!! ");
		puts("!!! 5:exit  login !!! ");
		puts("!!! 0:exit system !!! ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			query_msg();//�����ж�Ȩ�ޣ����н�ɫ����ʹ�ô˲�ѯ
			break;
		case 2:
			add_msg();//����û�����
			break;
		case 3:
			del_alt_msg();//ɾ���û�����
			break;
		case 4:
			display();//��ʾ�����û����û���ɫ
			break;
		case 5:
			//�˳���¼
			flag = 0;//����ԱȨ�޿���
			return;
		case 0:
			puts("!!! thank you for using !!! ");//�˳�ϵͳ
			i = 0;
			break;
		default:
			puts("!!! enter right choice !!! ");
			while ((getchar()) != '\n');
			getchar();
			break;
		}
	}
}
//������
int main(void)
{
	while (i)
	{
		puts("!!!The system is initializing!!!");
		//��ʼ������
		if (init_mysql())
			print_mysql_error(NULL);
		create_database();
		create_table();
		//��ʼ������Ա�˻�
		init_Administrtor();
		//�û���¼
		user_login();
		//�����˵�
		menu();
	}
	// �ر�����
	mysql_close(g_conn);
	return EXIT_SUCCESS;
}
��������������������������������
��Ȩ����������ΪCSDN������csdnXiaofei����ԭ�����£���ѭCC 4.0 BY - SA��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
ԭ�����ӣ�https ://blog.csdn.net/CSDNXiaofei/article/details/77993282