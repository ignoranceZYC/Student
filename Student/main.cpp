#include <stdio.h>
#include <winsock.h>
#include <mysql.h>
#include <string.h>
#define buf 1024
#pragma comment( lib, "libmysql.lib")

//��½ҳ��
int chooseUser(int user)
{
	printf("��ѡ��������\n1��ѧ��\n2������Ա\n");
	printf("���ѡ��");
	scanf_s("%d",&user,10);
	if (user!=1)
		if (user != 2)
		{
			printf("������������");
			return 0;
		}
	return 1;
}



int main()
{
	char SqlText[256] = "";  //����������Ҫִ�е�SQL���
	MYSQL* conn = NULL;      //MYSQL�������(�κ�һ��mysql�������ǻ���MYSQL��������������)
	MYSQL_RES* res = NULL;   //���ݲ�ѯ�����
	MYSQL_FIELD* fd = NULL; //MySQL��ͷ������
	MYSQL_ROW row;           //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ
	conn = mysql_init(NULL); //��ʼ��MYSQL����

	char  server[20] = "localhost"; //mysql��������IP
	char  user[20] = "root";        //�û���
	char  psd[20] = "123";       ///����  ��Ҫ��Ϊ�Լ�������
	char  dbName[1024] = "student";    ///���ݿ��� ��Ҫ��Ϊ�Լ������ݿ���
	unsigned short port = 3306;     //�������˿ں�,Ĭ��3306

	int user_C=0;

	//�������ݿ�
	if (mysql_real_connect(conn, server, user, psd, dbName, port, NULL, 0) == 0) {
		printf("Error connecting to database: %s\n", mysql_error(conn));
	}
	else {
		puts("���ݿ����ӳɹ���");
		mysql_query(conn, "set names 'GBK'");//�����ַ���,��ֹ�����޷�������ʾ[��ѡ����]
	}
	if(chooseUser(user_C))
		return 0;

	return 0;
}