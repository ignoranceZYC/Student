#include <stdio.h>
#include <winsock.h>
#include <mysql.h>
#include <string.h>
#define buf 1024
#pragma comment( lib, "libmysql.lib")

MYSQL_RES* res=NULL,*res2 = NULL;   //���ݲ�ѯ�����
MYSQL_FIELD* fd = NULL; //MySQL��ͷ������
MYSQL_ROW row;           //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ

//��½ҳ��
int chooseUser(int &user)
{
	printf("��ѡ��������\n1��ѧ��\n2������Ա\n");
	printf("���ѡ��");
	scanf_s("%d",&user,10);
	if (user!=1)
		if (user != 2)
		{
			printf("�������������");
			return 0;
		}
	system("cls");
	return 1;
}

//�˻���¼
int User(char* Sql, int user_A, MYSQL* conn, int &user_B)
{
	int i;
	MYSQL_RES* res = NULL;   //���ݲ�ѯ�����
	MYSQL_FIELD* fd = NULL; //MySQL��ͷ������
	//MYSQL_ROW row;
	char psd_B[20];
	printf("���������˺�\n");
	scanf_s("%d", &user_B, 20);
	printf("�������������\n");
	scanf_s("%s", psd_B, 20);
	sprintf(Sql, "select * from student.StudentT where (id='%d') and (pwd='%s') and( power>='%d');",user_B,psd_B,user_A );
	i = mysql_query(conn, Sql);
	res = mysql_store_result(conn);
	if ((i== 0) && (mysql_num_rows(res) != 0)) {       //ִ�гɹ���ѽ�����
		
		system("cls");
		printf("��½�ɹ�");
		mysql_free_result(res); //�ͷŽ������Դ
	}
	else {
		printf("��½ʧ��: %s\n", mysql_error(conn));
		exit(0);
		return 0;
	}
}

int Show_self(int user_B, MYSQL* conn) { 

	char Sql[256] = "";  //����������Ҫִ�е�SQL���
	system("cls");
	sprintf(Sql, "SELECT* FROM information_schema.`COLUMNS` where TABLE_SCHEMA='student' and TABLE_NAME='ScoreT' order by ORDINAL_POSITION;");
	if ((mysql_query(conn, Sql) == 0)) {       //ִ�гɹ���ѽ�����
		res = mysql_store_result(conn);      //�洢��ѯ�õ��Ľ����
		int fieldNums = mysql_num_fields(res); //��ȡ���ݽ��ÿ����¼������
		while ((row = mysql_fetch_row(res)) != NULL) { //���ϻ�ȡ��һ����
				printf("%-10s|", row[3]);
		}
		printf("\n");
		sprintf(Sql, "select * from student.ScoreT where (id='%d');", user_B);
		if ((mysql_query(conn, Sql) == 0)){       //ִ�гɹ���ѽ�����
		res = mysql_store_result(conn);      //�洢��ѯ�õ��Ľ����
		int fieldNums = mysql_num_fields(res); //��ȡ���ݽ��ÿ����¼������
		while ((row = mysql_fetch_row(res)) != NULL) { //���ϻ�ȡ��һ����
			for (int i = 0; i < fieldNums; i++) {
				printf("%-10s ", row[i]);
			}
			puts("");
		}
		mysql_free_result(res); //�ͷŽ������Դ
		//��ӡ��ȡ������
		}
	}
	else {
		printf("��ѯʧ��: %s\n", mysql_error(conn));
		return 0;
	}
	return 0;

}

int Show_Class( MYSQL* conn) {

	char Sql[256] = "";  //����������Ҫִ�е�SQL���
	system("cls");
	sprintf(Sql, "SELECT* FROM information_schema.`COLUMNS` where TABLE_SCHEMA='student' and TABLE_NAME='ClassT' order by ORDINAL_POSITION;");
	if ((mysql_query(conn, Sql) == 0)) {       //ִ�гɹ���ѽ�����
		res = mysql_store_result(conn);      //�洢��ѯ�õ��Ľ����
		int fieldNums = mysql_num_fields(res); //��ȡ���ݽ��ÿ����¼������
		while ((row = mysql_fetch_row(res)) != NULL) { //���ϻ�ȡ��һ����
			printf("%-10s|", row[3]);
		}
		printf("\n");
		sprintf(Sql, "select * from student.ClassT" );
		if ((mysql_query(conn, Sql) == 0)) {       //ִ�гɹ���ѽ�����
			res = mysql_store_result(conn);      //�洢��ѯ�õ��Ľ����
			int fieldNums = mysql_num_fields(res); //��ȡ���ݽ��ÿ����¼������
			while ((row = mysql_fetch_row(res)) != NULL) { //���ϻ�ȡ��һ����
				for (int i = 0; i < fieldNums; i++) {
					printf("%-10s ", row[i]);
				}
				puts("");
			}
			mysql_free_result(res); //�ͷŽ������Դ
			//��ӡ��ȡ������
		}
	}
	else {
		printf("��ѯʧ��: %s\n", mysql_error(conn));
		return 0;
	}
}

int Show_Stu(MYSQL* conn) {

	char Sql[256] = "";  //����������Ҫִ�е�SQL���
	system("cls");

	sprintf(Sql, "select a.id, a.name, c.No, c.name, b.Score from StudentT a, ScoreT b, ClassT c where(a.id = b.id) and (b.No = c.No)");
	printf("Id         |Name     |No       |ClassName |Score\n");
	if ((mysql_query(conn, Sql) == 0)) {       //ִ�гɹ���ѽ�����
		res = mysql_store_result(conn);      //�洢��ѯ�õ��Ľ����
		int fieldNums = mysql_num_fields(res); //��ȡ���ݽ��ÿ����¼������
		while ((row = mysql_fetch_row(res)) != NULL) { //���ϻ�ȡ��һ����

				for (int i = 0; i < fieldNums; i++) {
					printf("%-10s ", row[i]);
				}
				puts("");
		}
		mysql_free_result(res); //�ͷŽ������Դ
		//��ӡ��ȡ������
		
	}
	else {
		printf("��ѯʧ��: %s\n", mysql_error(conn));
		return 0;
	}
}

int Add_Stu(MYSQL* conn)
{
	int choose;
	char tableName[30];
	char Sql[256] = "";  //����������Ҫִ�е�SQL���
	int j=0;
	char type[256];
	system("cls");

	printf("\n--------------------------------\n");
	printf("|        1 ����γ���Ϣ        |\n");
	printf("|        2 ����ѧ����Ϣ        |\n");
	printf("|        3 ����ɼ���Ϣ        |\n");
	printf("|        4 �˳�����            |\n");
	printf("--------------------------------\n");

	printf("--------------------------------\n");
	printf("���������ѡ��");
	scanf_s("%d", &choose);
	switch (choose)
	{
	case 1:  sprintf(tableName, "ClassT"); break;
	case 2:  sprintf(tableName, "StudentT"); break;
	case 3:  sprintf(tableName, "ScoreT"); break;
	default:  return 0;
	}

	sprintf(Sql, "SELECT* FROM information_schema.`COLUMNS` where TABLE_SCHEMA='student' and TABLE_NAME='%s' order by ORDINAL_POSITION;", tableName);
	if ((mysql_query(conn, Sql) == 0)) {       //ִ�гɹ���ѽ�����
		res = mysql_store_result(conn);      //�洢��ѯ�õ��Ľ����
		int fieldNums = mysql_num_fields(res); //��ȡ���ݽ��ÿ����¼������
		j=sprintf(Sql, "insert into %s values(", tableName);
		while ((row = mysql_fetch_row(res)) != NULL) { //���ϻ�ȡ��һ����
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
	printf("���ʧ��: %s\n", mysql_error(conn));
	return 0;
	}
}

int Del_Stu(MYSQL* conn)
{
	int choose;
	char tableName[30];
	char Sql[256] = "";  //����������Ҫִ�е�SQL���
	int j = 0;
	char type[256];
	system("cls");

	printf("\n--------------------------------\n");
	printf("|        1 ɾ���γ���Ϣ        |\n");
	printf("|        2 ɾ��ѧ����Ϣ        |\n");
	printf("|        3 �˳�����            |\n");
	printf("--------------------------------\n");

	printf("--------------------------------\n");
	printf("���������ѡ��");
	scanf_s("%d", &choose);
	switch (choose)
	{
	case 1:  sprintf(tableName, "ClassT"); break;
	case 2:  sprintf(tableName, "StudentT"); break;
	default:  return 0;;
	}
	printf("����������");
	if (scanf_s("%s", type, 256))
	{
		sprintf(Sql, "delete from %s where %s", tableName, type);
		if ((mysql_query(conn, Sql) != 0))
		{
			printf("ɾ��ʧ��: %s\n", mysql_error(conn));
			return 0;
		}
		else
			printf("ɾ���ɹ�");
	}
	return 0;
}

int Change_Stu(MYSQL* conn)
{
	int choose;
	char tableName[30];
	char Sql[256] = "";  //����������Ҫִ�е�SQL���
	int j = 0;
	char type[256];
	char type1[256];
	system("cls");

	printf("\n--------------------------------\n");
	printf("|        1 �޸Ŀγ���Ϣ        |\n");
	printf("|        2 �޸�ѧ����Ϣ        |\n");
	printf("|        3 �˳�����            |\n");
	printf("--------------------------------\n");

	printf("--------------------------------\n");
	printf("���������ѡ��");
	scanf_s("%d", &choose);
	switch (choose)
	{
	case 1:  sprintf(tableName, "ClassT"); break;
	case 2:  sprintf(tableName, "StudentT"); break;
	default:  return 0;;
	}
	printf("����������");
	if (scanf_s("%s", type, 256))
	{
		printf("�������������:");
		scanf_s("%s", type1, 256);
		sprintf(Sql, "update %s set %s where %s ;", tableName, type1, type);
		printf("%s", Sql);
		if ((mysql_query(conn, Sql) != 0))
		{
			printf("�޸�ʧ��: %s\n", mysql_error(conn));
			return 0;
		}
		else
			printf("�޸ĳɹ�");
	}
	return 0;
}

//�û�����
int win(int user_A,int user_B, MYSQL* conn, char*  server)
{
	int choose;
	printf("\n--------------------------------\n");
	printf("|        1 ��ʾ������Ϣ        |\n");
	printf("|        2 ��ѯ�γ���Ϣ        |\n");
	if (user_A == 2) {
		printf("|        3 ��ʾѧ����Ϣ        |\n");
		printf("|        4 ���ѧ����Ϣ        |\n");
		printf("|        5 ɾ��ѧ����Ϣ        |\n");
		printf("|        6 �޸�ѧ����Ϣ        |\n");
	}
		printf("|        7 �˳�����            |\n");
	
	printf("--------------------------------\n");
	printf("���������ѡ��");
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
			system("cls");  //�����������˳�
			exit(0);
		} break;
	default:  break;
	}

	return 0;
}

int main()
{
	char Sql[256] = "";  //����������Ҫִ�е�SQL���
	MYSQL* conn = NULL;      //MYSQL�������(�κ�һ��mysql�������ǻ���MYSQL��������������)
	 res = NULL;   //���ݲ�ѯ�����
	 fd = NULL; //MySQL��ͷ������

	conn = mysql_init(NULL); //��ʼ��MYSQL����

	char  server[20] = "39.106.76.148"; //mysql��������IP
	char  user[20] = "root";        //�û���
	char  psd[20] = "123";       ///����  ��Ҫ��Ϊ�Լ�������
	char  dbName[1024] = "student";    ///���ݿ��� ��Ҫ��Ϊ�Լ������ݿ���
	unsigned short port = 3306;     //�������˿ں�,Ĭ��3306

	int user_A=0;
	int id = 0;
	//�������ݿ�
	if (mysql_real_connect(conn, server, user, psd, dbName, port, NULL, 0) == 0) {
		printf("Error connecting to database: %s\n", mysql_error(conn));
	}
	else {
		puts("���ݿ����ӳɹ���");
		mysql_query(conn, "set names 'GBK'");//�����ַ���,��ֹ�����޷�������ʾ[��ѡ����]
	}
	if(!chooseUser(user_A))
		return 0;
	User( Sql, user_A, conn,id);
	while (1)
	{
		win(user_A, id,conn,server);
	}
	mysql_close(conn); //�ر����ӣ����ͷ�����
	return 0;
}


