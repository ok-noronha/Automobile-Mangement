//file handling KEVIN NORONHA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct Name {
								char first[12], middle[12], last[12];
} name;
typedef struct Date {
								int day, month, year;
} date;
typedef struct Automobile_Details {
								char name[12], variant[12], color[12];
								int count, booking_id;
} automobile;
typedef struct Customer {
								int cust_id;
								name cust_name;
								automobile veh;
								float net_amount;
								date booking_date;
								int flag;
} customer;
customer cst;
FILE *cust_db;
FILE * Search(FILE *fl, int low, int high, int s);
FILE * Sort(FILE *fl, int low, int high, int s);
int Input_Customer_Details(customer*c);
int Amount_Calculation(customer*c);
int Print_Eqoute(customer*c, char l[]);
int fill_defaults(customer*c);
customer* Read_details(FILE *fl, customer *cp);
int Write_Details(customer*c, FILE *fl);
FILE *Add_Space(FILE*fl,int num);
FILE *Delete_Space(FILE*fl,int num);
float Showroom_Balancesheet(FILE *fl, customer*c, float sum, FILE *bs);
int Copy (FILE*f1, FILE*f2, int n);
void wait();
int main() {
								char val,cust_type;
								int sw,temp_cust_id;
								long int size;
								customer cust;
								customer* cp = &cust;
								FILE *balsh;
								char l_balsh[40] = "Showroom Balancesheet.txt";
								char l_bill[40] = "Customer Bill.txt";
								char l_db[40] = "Customer.dat";
								printf("\nContinue with previous Execution ? (y/n)\t");
								scanf(" %c",&val);
								if(val=='y') {
																if((cust_db = fopen(l_db,"r+"))==NULL) {
																								printf("\nPrevious Execution Files Missing\nCreating New Ones");
																								cust_db = fopen(l_db,"w+");
																}
								}
								else if(val=='n') {
																cust_db = fopen(l_db,"w+");
								}
								else {
																char f[10] ={val,'.','d','a','t','\0'};
																strcpy(l_db,f);
																cust_db = fopen(l_db,"w+");
								}
								do {
																//wait();
																printf("\n1 - Input Customer Profile\n2 - Discount calculation\n3 - Print E-Qoute\n4 - Delete Customer Profile\n5 - Print Showroom Balance Sheet\n6 - Exit\n");
																scanf(" %d",&sw);
																cust_db=(cust_db==NULL)?fopen(l_db,"r+"):cust_db;
																fseek(cust_db,0,SEEK_END);
																size = ftell(cust_db);
																int cust_num = size/sizeof(customer);
																fseek(cust_db,0,SEEK_SET);
																printf("%d",size);
																switch(sw) {
																case 1: {
																								printf("\n***Inputting Customer Profile***\n");
																								printf("\nNew Customer(N) or Existing Customer(E)\t");
																								scanf(" %c",&cust_type);
																								if (cust_type=='N') {
																																printf("\nEnter Customer ID\t");
																																scanf(" %d", &temp_cust_id);
																																cp->cust_id = temp_cust_id;
																																fseek(cust_db,0,SEEK_SET);
																																cust_db = Sort(cust_db, 0, size, temp_cust_id);
																																if (cust_db==NULL) {
																																								printf("\n ERROR WHILE SORT");
																																								break;
																																}
																																cust_db=Add_Space(cust_db,cust_num);
																								}
																								else if (cust_type=='E') {
																																printf("\nEnter Customer ID\t");
																																scanf(" %d", &temp_cust_id);
																																fseek(cust_db,0,SEEK_SET);
																																cust_db = Search(cust_db, 0, cust_num, temp_cust_id);
																																if(cust_db == NULL) {
																																								printf("\nCustomer not Found");
																																								break;
																																}
																																cp->cust_id = temp_cust_id;
																								}
																								else{
																																printf("\nInvalid Input");
																																break;
																								}
																								fill_defaults(cp);
																								Input_Customer_Details(cp);
																								Write_Details(cp,cust_db);
																								break;
																}
																case 2: {
																								printf("\n***Calculating Bill Amount***\n");
																								printf("\nEnter Customer ID\t ");
																								scanf(" %d",&cp->cust_id);
																								fseek(cust_db,0,SEEK_SET);
																								cust_db = Search(cust_db, 0, cust_num, cp->cust_id);
																								if(cust_db == NULL) {
																																printf("\nCustomer not Found");
																																break;
																								}
																								fill_defaults(cp);
																								cp=Read_details(cust_db,cp);
																								Amount_Calculation(cp);
																								Write_Details(cp, cust_db);
																								break;
																}
																case 3: {
																								printf("\n***Printing Customer Eqoute***\n");
																								printf("\nEnter Customer ID\t ");
																								scanf(" %d",&cp->cust_id);
																								fseek(cust_db,0,SEEK_SET);
																								cust_db = Search(cust_db, 0, cust_num, cp->cust_id);
																								if(cust_db == NULL) {
																																printf("\nCustomer not Found");
																																break;
																								}
																								fill_defaults(cp);
																								cp=Read_details(cust_db,cp);
																								if(cp->flag<2) {
																																printf("\nVehicle Price not Calculated\nCalculating\n");
																																Amount_Calculation(cp);
																																Write_Details(cp,cust_db);
																																cp=Read_details(cust_db,cp);
																								}
																								Print_Eqoute(cp, l_bill);
																								Write_Details(cp,cust_db);
																								break;
																}
																case 4: {
																								printf("\n***Delete Customer Profile***");
																								printf("\nEnter Customer ID\t ");
																								scanf(" %d",&cp->cust_id);
																								fseek(cust_db,0,SEEK_SET);
																								cust_db = Search(cust_db, 0, cust_num, cp->cust_id);
																								if(cust_db == NULL) {
																																printf("\nCustomer not Found");
																																break;
																								}
																								cust_db=Delete_Space(cust_db,cust_num);
																								printf("\nCustomer Deleted");
																								break;
																}
																case 5: {
																								balsh=fopen(l_balsh,"w+");
																								printf("\n***Amount Balance Sheet***");
																								printf("\n ---------------------------------------------------------------------------------------------------");
																								printf("\n | %-11s | %-25s\t | %-23s\t | %-17s |", " ID", " Customer Name", " Vehicle Name", " Vehicle Price");
																								printf("\n ---------------------------------------------------------------------------------------------------");
																								fprintf(balsh,"\n ---------------------------------------------------------------------------------------------------");
																								fprintf(balsh,"\n | %-11s | %-25s\t | %-23s\t | %-17s |", " ID", " Customer Name", " Vehicle Name", " Vehicle Price");
																								fprintf(balsh,"\n ---------------------------------------------------------------------------------------------------");
																								fseek(cust_db, 0, SEEK_SET);
																								fill_defaults(cp);
																								float total = Showroom_Balancesheet(cust_db, cp, 0.0, balsh);
																								printf("\n ---------------------------------------------------------------------------------------------------");
																								fprintf(balsh,"\n ---------------------------------------------------------------------------------------------------");
																								printf("\n\nGRAND TOTAL : %.2f\n\n\n", total);
																								fprintf(balsh,"\n\nGRAND TOTAL : %.2f\n\n\n", total);
																								fclose(balsh);
																								break;
																}
																case 6: {
																								printf("\nExiting the Program\nThank You\n\n\nAUTOMOBILE MANAGEMENT by KEVIN NORONHA");
																								fclose(cust_db);
																								exit(1);
																}
																default: {
																								printf("\nInvalid Input");
																}
																}
								}while(1);
								return 0;
}
FILE * Search(FILE *fl, int low, int high, int s) {
								char val;
								customer *c, ct;
								c=&ct;
								if(high>=low) {
																int mid=(low+high)/2;
																fseek(fl, mid*sizeof(customer), SEEK_SET);
																c=Read_details(fl,c);
																if (c==NULL) {
																								return NULL;
																}
																if(c->cust_id==s) {
																								printf("\nAre you %s %s %s (y/n) : ", c->cust_name.first, c->cust_name.middle, c->cust_name.last);
																								scanf(" %c", &val);
																								if(val=='y') {
																																return fl;
																								}
																								else if(val=='n') {
																																return NULL;
																								}
																								else {
																																printf("\nPlease Enter y/n ....\nTry Again");
																																return Search(fl, low, high, s);
																								}
																}
																if(s>c->cust_id) {
																								return Search(fl, mid+1, high, s);
																}
																if(s<c->cust_id) {
																								return Search(fl, low, mid-1, s);
																}
								}
								return NULL;
}
FILE * Sort(FILE *fl, int low, int high, int s) {
								customer *c, ct;
								c=&ct;
								if(low==high) {
																fseek(fl,high,SEEK_SET);
																return fl;
								}
								else if(low<high) {
																fseek(fl, low, SEEK_SET);
																c=Read_details(fl,c);
																if(c->cust_id<s) {
																								return Sort(fl, low+sizeof(customer), high, s);
																}
																if(c->cust_id>s) {
																								return fl;
																}
																if(c->cust_id==s) {
																								printf("\nCustomer ID assigned");
																								return NULL;
																}
								}
								else {
																return NULL;
								}
								return NULL;
}
int Input_Customer_Details(customer*c) {
								printf("\nEnter Customer Full Name\t");
								scanf(" %s %s %s", &c->cust_name.first,&c->cust_name.middle,&c->cust_name.last);
								printf("\nEnter Booking Date in (DD/MM/YYYY)\t");
								scanf(" %d %d %d", &c->booking_date.day, &c->booking_date.month, &c->booking_date.year);
								c->booking_date.day %=100;
								c->booking_date.month %=100;
								c->booking_date.year %=10000;
								printf("\nEnter Vehicle name\t");
								scanf(" %s", &c->veh.name);
								printf("\nEnter Vehicle Variant:\tSedan - 428000.00\n \t\t\tConvertible - 471000.00\n\t\t");
								printf("\tSport - 589000.00\n \t\t\tSuper - 663000.00\n \t\t\tExotic - 828000.00\n");
								scanf(" %s", &c->veh.variant);
								printf("\nEnter Vehicle Colour\t");
								scanf(" %s", &c->veh.color);
								for(; (c->veh.count)<1;) {
																printf("\nEnter Number of Vehicles to Buy\t");
																scanf(" %d", &c->veh.count);
								}
								c->flag=1;
								return 0;
}
int Amount_Calculation(customer*c) {
								int cmp_var[5];
								char val;
								cmp_var[0] = strcmp(c->veh.variant,"Sedan");
								cmp_var[1] = strcmp(c->veh.variant,"Convertible");
								cmp_var[2] = strcmp(c->veh.variant,"Sport");
								cmp_var[3] = strcmp(c->veh.variant,"Super");
								cmp_var[4] = strcmp(c->veh.variant,"Exotic");
								printf("\nVehicle Selected : %s %s",c->veh.name, c->veh.variant);
								c->net_amount
																=(cmp_var[0]==0)?428000.00:(cmp_var[1]==0)?471000.00:(cmp_var[2]==0)?
																		589000.00:(cmp_var[3]==0)?663000.00:(cmp_var[4]==0)?828000.00:-1.00;
								if(c->net_amount==-1.00) {
																printf("\nNo Such Variant Available\nPlease Try Again\n");
																printf("\nEnter Vehicle Variant:\tSedan - 428000.00\n \t\t\tConvertible - 471000.00\n\t\t");
																printf("\tSport - 589000.00\n \t\t\tSuper - 663000.00\n \t\t\tExotic - 828000.00\n");
																scanf(" %s",&c->veh.variant);
																Amount_Calculation(c);
																return 0;
								}
								c->net_amount *= c->veh.count;
								printf("\nDo you work for the Defence or are you an ex-Defence Personnel? (y/n) : ");
								scanf(" %c", &val);
								if(val=='y') {
																printf("10/% Discount on %.2f\n",c->net_amount);
																c->net_amount *= 0.9;
								}
								else if(val=='n')
																printf("No Discount\n");
								else {
																printf("Please Enter y/n ....\nTry Again");
																Amount_Calculation(c);
								}
								c->flag=2;
								return 0;
}
int Print_Eqoute(customer*c, char l[]) {
								printf("\n___________________________________________________\n| %-48s|"," ");
								printf("\n| %-48s|\n| %-48s|"," FOCAL POINT AUTOMOBILE DEALERS"," ");
								printf("\n|_________________________________________________|");
								printf("\n|Customer ID | %-27d|",c->cust_id);
								printf("\tBooking Date - %-02d/ %-02d/ %-4d",c->booking_date.day, c->booking_date.month, c->booking_date.year);
								printf("\n|Customer Name | %-8s %-8s %-9s|",c->cust_name.first,c->cust_name.middle,c->cust_name.last);
								printf("\tBooking ID - %-8d",c->veh.booking_id+20210000);
								printf("\n|Vehicle Name | %-26s |",c->veh.name);
								printf("\n|Vehicle Variant | %-26s |",c->veh.variant);
								printf("\n|Vehicle Colour | %-26s |",c->veh.color);
								printf("\n|Number of Vehicles | %-26d |",c->veh.count);
								printf("\n|-------------------------------------------------|");
								printf("\n|Net Amount | %-26.2f |",c->net_amount);
								printf("\tCashier - Kevin Noronha");
								printf("\n|_________________________________________________|\n");
								printf("\n**Cars you've never driven before\n\n\n");
								c->flag=3;
								FILE *bl;
								char sid[10];
								sprintf(sid, "%d", c->cust_id);
								strcat(sid,l);
								bl=fopen(sid,"w");
								fprintf(bl,"\n___________________________________________________\n| %-48s|"," ");
								fprintf(bl,"\n| %-47s|\n| %-48s|"," FOCAL POINT AUTOMOBILE DEALERS"," ");
								fprintf(bl,"\n|_________________________________________________|");
								fprintf(bl,"\n|Customer ID | %-27d|",c->cust_id);
								fprintf(bl,"\tBooking Date - %-02d/ %-02d/ %-4d",c->booking_date.day, c->booking_date.month, c->booking_date.year);
								fprintf(bl,"\n|Customer Name | %-8s %-8s %-9s|",c->cust_name.first,c->cust_name.middle,c->cust_name.last);
								fprintf(bl,"\tBooking ID - %-8d",c->veh.booking_id+20210000);
								fprintf(bl,"\n|Vehicle Name | %-26s |",c->veh.name);
								fprintf(bl,"\n|Vehicle Variant | %-26s |",c->veh.variant);
								fprintf(bl,"\n|Vehicle Colour | %-26s |",c->veh.color);
								fprintf(bl,"\n|Number of Vehicles | %-26d |",c->veh.count);
								fprintf(bl,"\n|-------------------------------------------------|");
								fprintf(bl,"\n|Net Amount | %-26.2f |",c->net_amount);
								fprintf(bl,"\tCashier - Kevin Noronha");
								fprintf(bl,"\n|_________________________________________________|\n");
								fprintf(bl,"\n**Cars you've never driven before\n\n\n");
								fclose(bl);
								return 0;
}
int fill_defaults(customer*c) {
								//c->cust_id=99999999;
								c->flag=0;
								c->net_amount=0.00;
								c->booking_date.day=00;
								c->booking_date.month=00;
								c->booking_date.year=0000;
								c->veh.count=0;
								c->veh.booking_id=0;
								strcpy(c->cust_name.first,"NULL ");
								strcpy(c->cust_name.middle,"NULL ");
								strcpy(c->cust_name.last,"NULL ");
								strcpy(c->veh.name,"NULL ");
								strcpy(c->veh.variant,"NULL ");
								strcpy(c->veh.color,"NULL ");
								return c->cust_id;
}
customer* Read_details(FILE *fl,customer *c) {
								long int pos = ftell(fl);
								while(fread(c, sizeof(customer), 1, fl)) {
																fseek(fl, pos, SEEK_SET);
																return c;
								}
								return NULL;
}
int Write_Details(customer*c, FILE *fl) {
								long int pos = ftell(fl);
								fwrite (c, sizeof(customer), 1, fl);
								fseek(fl, pos, SEEK_SET);
								return 0;
}
FILE *Add_Space(FILE*fl,int num) {
								customer *c1, ct1;
								c1=&ct1;
								long int pos = ftell(fl)/sizeof(customer);
								if(pos==num) {
																printf("last element");
																return fl;
								}
								for(int n=num; n>pos; n--) {
																fseek(fl,(n-1)*sizeof(customer),SEEK_SET);
																c1 = Read_details(fl,c1);
																fseek(fl,n*sizeof(customer),SEEK_SET);
																Write_Details(c1,fl);
								}
								fseek(fl,pos*sizeof(customer),SEEK_SET);
								return fl;
}
FILE *Delete_Space(FILE*fl,int num) {
								FILE* tf;
								customer *c1, ct1;
								c1=&ct1;
								long int pos = ftell(fl)/sizeof(customer);
								for(int p=pos; p<num; p++) {
																fseek(fl,(p+1)*sizeof(customer),SEEK_SET);
																c1 = Read_details(fl,c1);
																if(c1==NULL) {
																								break;
																}
																fseek(fl,p*sizeof(customer),SEEK_SET);
																Write_Details(c1,fl);
								}
								tf = fopen("temp.dat","w+");
								Copy(fl, tf, num-1);
								fclose(fl);
								fl= fopen("Customer.dat","w+");
								Copy(tf, fl, num-1);
								fseek(fl,0,SEEK_SET);
								fclose(tf);
								return fl;
}
float Showroom_Balancesheet(FILE *fl, customer*c, float sum, FILE *bs) {//TODO: print into a readable file
								c=Read_details(fl, c);
								if(c!=NULL) {
																sum+=c->net_amount;
																printf("\n");
																(c->flag==1)?printf("!"):(c->flag==2)?printf("*"):(c->flag==3)?printf("$"):printf("E");
																printf(" | %8d\t | %-8s %-8s %-9s\t | %-9s %-12s\t | %17.2f |", c->cust_id, c->cust_name.first,
																							c->cust_name.middle, c->cust_name.last, c->veh.name, c->veh.variant, c->net_amount);
																fprintf(bs,"\n");
																(c->flag==1)?fprintf(bs,"!"):(c->flag==2)?fprintf(bs,"*"):(c->flag==3)?fprintf(bs,"$"):fprintf(bs,"E");
																fprintf(bs," | %8d\t | %-8s %-8s %-9s\t | %-9s %-12s\t | %17.2f |", c->cust_id, c->cust_name.first,
																								c->cust_name.middle, c->cust_name.last, c->veh.name, c->veh.variant, c->net_amount);
																fseek(fl, sizeof(customer), SEEK_CUR);
																return Showroom_Balancesheet(fl, c, sum, bs);
								}
								return sum;
}
int Copy (FILE*f1, FILE*f2, int n) {
								customer cst, *c;
								c = &cst;
								for (int i=0; i<n; i++) {
																fseek(f1,i*sizeof(customer),SEEK_SET);
																c = Read_details(f1,c);
																if(c==NULL) {
																								break;
																}
																fseek(f2,i*sizeof(customer),SEEK_SET);
																Write_Details(c,f2);
								}
								return 0;
}
void wait() {
								char*ptr,var;
								ptr=&var;
								scanf(" %c", ptr);
								fprintf(stdout,"w");
								system("cls");
}
