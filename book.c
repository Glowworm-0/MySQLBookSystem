#include<stdio.h>
#include<string.h>
#include<winSock.h>
#include<Windows.h>
#include<mysql.h>
#include<conio.h>
#pragma comment(lib,"libmysql.lib") 
/*MySQL DATALIBRARY TABLE:create table tb_book(
*                        ->ID char(10)NOT NULL,
* 						 ->bookname char(50)NOT NULL,
*						 ->author char(50)NOT NULL,
*						 ->bookconcern char(100)NOT NULL,
*						 ->PRIMARY KEY (ID)
*						 ->)ENGINE=MYISAM;
*/

MYSQL mysql;
MYSQL_RES*result;
MYSQL_ROW row;
char ch[2];

void ShowAll();
void AddBook();
void ModifyBook();
void DeleteBook();
void QueryBook();

void showmenu()
{
	system("cls");
	printf("\n\n\n\n\n");
	printf("\t||================================================||\n");
	printf("\t||             Welcome To Lemon Books System      ||\n");
	printf("\t||================================================||\n");
	printf("\t||\t 1 -- SHOW ALL BOOK  INFORMATION          ||\n");
	printf("\t||\t 2 -- ADD BOOK INFORMATION                ||\n");
	printf("\t||\t 3 -- MODIFY BOOK INFORMATION             ||\n");
	printf("\t||\t 4 -- DELETE BOOK INFORMATION             ||\n");
	printf("\t||\t 5 -- QUERY BOOK INFORMATION              ||\n");
	printf("\t||\t 6 -- EXIT                                ||\n");
	printf("\t||================================================||\n");
	printf("\n                ENTER YOUR CHOICE(1-6): ");
}

int main()
{
	int n;
	mysql_init(&mysql);/*Initialize mysql*/
	showmenu();
	scanf("%d", &n);
	while (n)
	{
		switch (n)
		{
		case 1:ShowAll();
			break;
		case 2:AddBook();
			break;
		case 3:ModifyBook();
			break;
		case 4:DeleteBook();
			break;
		case 5:QueryBook();
			break;
		case 6:exit(0);
		default:
			break;
		}
		scanf("%d", &n);
	}
}

void inquire()
{
	printf("\t Whether return showmenu?(Y/N):");
	scanf("%s", &ch);
	if(strcmp(ch, "Y") == 0|| strcmp(ch, "y") == 0)
	{
		showmenu();
	}
	else
	{
		exit(0);
	}
}

void ShowAll()
{
	if (!mysql_real_connect(&mysql, "localhost", "root", "password", "db_books", 0, NULL, 0))
	/*User connection to the Mysql server information:"&mysql" is an initializer connection handle;"localhost" is local host the name;
	"root" is for MySQL data library Account;"password" is user accounts the password;"db_books is the data library created by the user
	to connect to--Please refer to MySQL function prototype or standard for other specific parameters."*/
	{
		printf("\n\t Cant not connect db_books!\n");
	}
	else
	{
		if (mysql_query(&mysql, "select*from tb_book"))
		{
			printf("\n\tQuery tb_book the data table failed!\n");
		}
		else
		{
			result = mysql_store_result(&mysql);
			if (mysql_num_rows(result) != 0)
			{
				printf("\t ====================================================== \n");
				printf("\t            Show all the book information               \n");
				printf("\t ====================================================== \n");
				printf("\t BOOK NUMBER     BOOKNAME     AUTHOR      BOOK CONCERN  \n");
				printf("\t ----------------------------------------------------------------------- \n");
				while ((row = mysql_fetch_row(result)))
				{
					fprintf(stdout, "\t    %s    %s    %s    %s    %s    \n", row[0], row[1], row[2], row[3]);
				}
				printf("\t ====================================================== \n");
			}
			else
			{
				printf("\n\t Not information recorded !\n");
			}
			mysql_free_result(result);
		}
		mysql_close(&mysql);
	}
	inquire();
}

void AddBook()
{
	int rowcount;
	char id[10];
	char *bookname;
	char *author;
	char *bookconcern;
	char *sql;
	char dest[100] = { " " };
	/*Connect MySql*/
	if (!mysql_real_connect(&mysql, "localhost", "root", "password", "db_books", 0, NULL, 0))
	{
		printf("\n\t Can not connect db_books!\n");
	}
	else
	{
		/*MySql connection successful,insert data*/
		printf("\t ============================================================= \n");
		printf("\t                    ADDITION BOOK INFORMATION                  \n");
		printf("\t ============================================================= \n");
		if (mysql_query(&mysql, "select*from tb_book"))/*If query failed!*/
		{
			printf("\n\tQuery tb_book the data table failed!\n");
		}
		else
		{
			result = mysql_store_result(&mysql);/*Get result set*/
			rowcount = mysql_num_rows(result);/*Get rows number*/
			row = mysql_fetch_row(result);/*Get the number of rows int the result set*/
			printf("\t ID:");
			scanf("%s", &id);
			printf("\tbookname:");
			scanf("%s", &bookname);
			printf("\tauthor:");
			scanf("%s", &author);
			printf("\tbookconcern:");
			scanf("%s", &bookconcern);
			if (mysql_num_rows(result)!=0)
			{
				/*Judge whether the entered number exists*/
				do
				{
					if (!strcmp(id, row[0]))
					{
						printf("\tRecored exists,press any key to continue!");
						_getch();
						mysql_free_result(result);
						mysql_close(&mysql);/*Free connect*/
						return;
					}
				} while (row = mysql_fetch_row(result));
				/*The same number does not exist*/
				sql = "INSERT INTO tb_book (id,bookname,author,bookconcern)values";
				strcat(dest, id);
				strcat(dest, "");
				strcat(dest, bookname);
				strcat(dest, "");
				strcat(dest, author);
				strcat(dest, "");
				strcat(dest, bookconcern);
				strcat(dest, "");
				printf("%s", dest);
				if (mysql_query(&mysql, dest) != 0)
				{
					fprintf(stderr, "Can't insert record!", mysql_error(&mysql));
				}
				else
				{
					printf("\tInsert success!\n");
				}
				mysql_free_result(result);
			}
			mysql_close(&mysql);
		}
		inquire();
	}
}

void ModifyBook()
{
	char id[10];
	char *bookname;
	char *author;
	char *bookconcern;
	char *sql;
	char dest[100] = { " " };
	char dest1[100] = { " " };
	if (!mysql_real_connect(&mysql, "localhost", "root", "password", "db_books", 0, NULL, 0))
	{
		printf("\n\t Can not connect db_books!\n");
	}
	else
	{
		/*MySql connection successful*/
		printf("\tPlease enter the book number you want to modify:");
		scanf("%s", &id);
		sql = "SELECT*from tb_book where id=";
		strcat(dest,sql);
		strcat(dest,id);
		if (mysql_query(&mysql, dest))/*Query the book information whether exists*/
		{
			printf("\nQuery tb_book the data table failed!\n");
		}
		else
		{
			result = mysql_store_result(&mysql);
			if (mysql_num_rows(result) != 0)
			{/*whether there a record?Only when there is a record,it is meaningful to get data*/
				printf("\tFind record information,whether show?(y/n):");
				scanf("%s", &ch);
				if (strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)
				{
					printf("\t ==================================================================== \n");
					printf("\t                ******SHOW BOOK INFORMATION******                     \n");
					printf("\t ==================================================================== \n");
					printf("\t BOOKNUMBER        BOOKNAME         AUTHOR           PUBLISHER        \n");
					printf("\t ------------------------------------------------- \n");
					while ((row = mysql_fetch_row(result)))/*Get records in the result set*/
					{
						fprintf(stdout, "\t     %s     %s     %s     %s     \n", row[0], row[1], row[2], row[3]);
					}
					printf("\t ==================================================================== \n");
					printf("\tModify?(y/n):");
					scanf("%s", &ch);
					if (strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)
					{
						printf("\tBookname:");
						scanf("%s", &bookname);
						sql = "UPDATE tb_book set bookname=";
						strcat(dest1,sql);
						strcat(dest1,bookname);
						printf("\tAuthor:");
						scanf("%s", &author);
						strcat(dest1,"author=");
						strcat(dest1,author);
						printf("\tpublisher:");
						scanf("%s", &bookconcern);
						strcat(dest1,"bookconcern=");
						strcat(dest1,bookconcern);
						strcat(dest1,"where id=");
						strcat(dest1,id);
						if (mysql_query(&mysql, dest1) != 0)
						{
							fprintf(stderr, "\tCan't modify record!\n", mysql_error(&mysql));
						}
						else
						{
							printf("\t Modify success!\n");
						}
					}
				}
			}
			else
			{
				printf("No information to modify was found!\n");
			}
		}
		mysql_free_result(result);
	}
	mysql_close(&mysql);
	inquire();
}

void DeleteBook()
{
	char id[10];
	char *sql;
	char dest[100] = { " " };
	char dest1[100] = { " " };
	if (!mysql_real_connect(&mysql, "localhost", "root", "password", "db_books", 0, NULL, 0))
	{
		printf("\n\t Can not connect db_books!\n");
	}
	else
	{
		printf("\tPlease enter the book number you want to delete:");
		scanf("%s", &id);
		sql = "SELECT*from tb_book where id=";
		strcat(dest,sql);
		strcat(dest,id);
		if (mysql_query(&mysql, dest))
		{
			printf("\nQuery tb_book the data table failed!\n");
		}
		else
		{
			result = mysql_store_result(&mysql);
			if (mysql_num_rows(result) != 0)
			{
				printf("\tFind record information,whether show?(y/n):");
				scanf("%s", &ch);
				if (strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)
				{
					printf("\t ==================================================================== \n");
					printf("\t                ******SHOW BOOK INFORMATION******                     \n");
					printf("\t ==================================================================== \n");
					printf("\t BOOKNUMBER        BOOKNAME         AUTHOR           PUBLISHER        \n");
					printf("\t ------------------------------------------------- \n");
					while ((row = mysql_fetch_row(result)))
					{
						fprintf(stdout, "\t     %s     %s     %s     %s     \n", row[0], row[1], row[2], row[3]);
					}
					printf("\t ==================================================================== \n");
				}
				printf("\tWhether delete?(y/n):");
				scanf("%s", &ch);
				if (strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)
				{
					sql = "DELETE from tb_book where ID=";
					printf("%s", dest1);
					strcat(dest1,sql);
					strcat(dest1,id);
					printf("%s", dest1);
					if (mysql_query(&mysql, dest1) != 0)
					{
						fprintf(stderr, "Can't delete record!\n", mysql_error(&mysql));
					}
					else
					{
						printf("\tDelete success!\n");
					}
				}
			}
			else
			{
				printf("\tNo information to delete was found!\n");
			}
		}
		mysql_free_result(result);
	}
	mysql_close(&mysql);
	inquire();
}

void QueryBook()
{
	char id[10];
	char *sql;
	char dest[100] = { " " };
	if (!mysql_real_connect(&mysql, "localhost", "root", "password", "db_books", 0, NULL, 0))
	{
		printf("\n\t Can not connect MySql!\n");
	}
	else
	{
		printf("\tPlease enter the book number you want to query:");
		scanf("%s", &id);
		sql = "SELECT*from tb_book where id=";
		strcat(dest,sql);
		strcat(dest,id);
		if (mysql_query(&mysql, dest))
		{
			printf("\nQuery tb_book the data table failed!\n");
		}
		else
		{
			result = mysql_store_result(&mysql);
			if (mysql_num_rows(result) != 0)
			{
				printf("\tFind record information,whether show?(y/n):");
				scanf("%s", &ch);
				if (strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)
				{
					printf("\t ==================================================================== \n");
					printf("\t                ******SHOW BOOK INFORMATION******                     \n");
					printf("\t ==================================================================== \n");
					printf("\t BOOKNUMBER        BOOKNAME         AUTHOR           PUBLISHER        \n");
					printf("\t ------------------------------------------------- \n");
					while ((row = mysql_fetch_row(result)))
					{
						fprintf(stdout, "\t     %s     %s     %s     %s     \n", row[0], row[1], row[2], row[3]);
					}
					printf("\t ==================================================================== \n");
				}
				else
				{
					printf("\tNo information to query was found!\n");
				}
				mysql_free_result(result);
			}
		}
		mysql_close(&mysql);
	}
	inquire();
}
