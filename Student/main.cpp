#include <stdio.h>
#include <winsock.h>
#include <mysql.h>
#include <string.h>
#define buf 1024
#pragma comment( lib, "libmysql.lib")

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
int User(char* Sql, int user_A, MYSQL* conn)
{
	MYSQL_RES* res = NULL;   //���ݲ�ѯ�����
	MYSQL_FIELD* fd = NULL; //MySQL��ͷ������
	MYSQL_ROW row;
	int user_B;
	char psd_B[20];
	printf("���������˺�\n");
	scanf_s("%d", &user_B, 20);
	printf("�������������\n");
	scanf_s("%s", psd_B, 20);
	sprintf(Sql, "select * from student.StudentT where (id='%d') and (pwd='%s') and( power>='%d');",user_B,psd_B,user_A );
	if (mysql_query(conn, Sql) == 0) {       //ִ�гɹ���ѽ�����
		system("cls");
		printf("��½�ɹ�");
		mysql_free_result(res); //�ͷŽ������Դ
	}
	else {
		printf("��½ʧ��: %s\n", mysql_error(conn));
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
//�û�����
int win(int user_A)
{
	int choose;
	printf("\n--------------------------------\n");
	printf("|        1 ��ѯ������Ϣ        |\n");
	printf("|        2 ��ѯ�γ���Ϣ        |\n");
	if (user_A == 2) {
		printf("|        3 ��ʾѧ����Ϣ        |\n");
		printf("|        4 ���ѧ����Ϣ        |\n");
		printf("|        5 ɾ��ѧ����Ϣ        |\n");
		printf("|        6 �޸�ѧ����Ϣ        |\n");
		printf("|        7 �˳�����            |\n");
	}
	printf("--------------------------------\n");
	printf("���������ѡ��");
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
			system("cls");  //�����������˳�
			exit(0);
		} break;
	default:  break;
	}
*/
	return 0;
}

int main()
{
	char Sql[256] = "";  //����������Ҫִ�е�SQL���
	MYSQL* conn = NULL;      //MYSQL�������(�κ�һ��mysql�������ǻ���MYSQL��������������)
	MYSQL_RES* res = NULL;   //���ݲ�ѯ�����
	MYSQL_FIELD* fd = NULL; //MySQL��ͷ������
	MYSQL_ROW row;           //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ
	conn = mysql_init(NULL); //��ʼ��MYSQL����

	char  server[20] = "39.106.76.148"; //mysql��������IP
	char  user[20] = "root";        //�û���
	char  psd[20] = "123";       ///����  ��Ҫ��Ϊ�Լ�������
	char  dbName[1024] = "student";    ///���ݿ��� ��Ҫ��Ϊ�Լ������ݿ���
	unsigned short port = 3306;     //�������˿ں�,Ĭ��3306

	int user_A=0;

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
	User( Sql, user_A, conn);
	win(user_A);

	return 0;
}