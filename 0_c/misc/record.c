#include <stdio.h>

#define MAX_EMPLOYESS 50



struct details {
	char name[50];
	int empid;
	struct {
		int date;
		int month;
		int year;
	}dob;
	float salary;
};

struct details emp[MAX_EMPLOYESS];

int n;

void read_details(int n)
{


	for(int i=0;i<n;i++)
	{
		printf("enter name : ");
		scanf("%s",emp[i].name);

		printf("enter empid : ");
		scanf("%d",&emp[i].empid);

		printf("enter dob DD");
		scanf("%d",&emp[i].dob.date);

		printf("enter dob MM");
		scanf("%d",&emp[i].dob.date);

		printf("enter dob YYYY");
	    scanf("%d",&emp[i].dob.date);

		printf("enter salary: ");
		scanf("%f",&emp[i].salary);

		printf("\n");

	}


}

void print_details(int n)
{
	for(int i=0;i<n;i++)
	{

		printf("%s \n",emp[i].name);

		printf("%d \n",emp[i].empid);

		printf("dob %d:%d:%d \n",emp[i].dob.date,
				              emp[i].dob.month,
							  emp[i].dob.year);

		printf("%f \n",emp[i].salary);

		printf("\n");

	}

}

int main()
{
	printf("enter number of emp entries \n");
	scanf("%d",&n);

	read_details(n);
	print_details(n);
}
