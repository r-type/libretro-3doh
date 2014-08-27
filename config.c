
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


	FILE* config;
	long fsize;
	int readcount;


void configOpen(char *file)
{


	printf("INFO: openning config file in %s\n",file);
	config=fopen(file,"rb");
	if (config==NULL)printf("ERROR: Config load error\n");
	printf("INFO: Config file opened\n");
    fseek (config , 0 , SEEK_END);
	fsize = ftell(config);
	rewind (config);



}


void configClose()
{

	fclose(config);

}



int configReadInt(char *section,char *name)
{

	char value[20];
	char temp[20];
	char stemp[20];
//	char ẗemp[20];
	int sectionfound=0;

	//search section

	memset(stemp,0,sizeof(stemp));
	memset(temp,0,sizeof(temp));
//	fgets(temp,20,config);

	while((!feof(config))&&(!sectionfound))
	{
		fscanf(config,"%[^\n]",temp);
		sprintf(stemp,"[%s]",section);

		//if section found
		if(strcmp(stemp,temp)==0)
		{
			printf("encontrada sección %s %s %d\n",temp,stemp,strcmp(stemp,temp));
			sectionfound=1;

		}


	}

	//search value
	if(sectionfound)
	{

		while(!feof(config))
		{

			fgets(temp,20,config);

			//buscar etiqueta
			fscanf(config,"%[^=]%[^\n]",temp,value);

				if(strcmp(temp,name)==0)
				{
					printf("encontrada etiqueta %s %s\n",temp, value);

					return 1;
				}


		}


	}
	rewind (config);
	return 0;

}


char *configReadString(char *section,char *name)
{

	char *value;
	char temp[20];
	char stemp[20];
//	char ẗemp[20];
	int sectionfound=0;

	//search section

	value=(char *)malloc(sizeof(char)*20);
	memset(stemp,0,sizeof(stemp));
	memset(temp,0,sizeof(temp));
//	fgets(temp,20,config);

	while((!feof(config))&&(!sectionfound))
	{
		fscanf(config,"%[^\n] ",temp);
		sprintf(stemp,"[%s]",section);

		//printf("%s\n",temp);
		

		//if section found
		if(strcmp(stemp,temp)==0)
		{
			printf("encontrada sección %s\n",temp);
			sectionfound=1;

		}
		fflush(stdout);
	//	fscanf(config," ",temp);


	}

	//search value
	if(sectionfound)
	{

		while(!feof(config))
		{

		//	fgets(temp,20,config);
			fscanf(config,"%*[\n]",temp);

			//buscar etiqueta
			fscanf(config,"%[^=]=%[^\n]",temp,value);

				if(strcmp(temp,name)==0)
				{
					printf("encontrada etiqueta %s %s\n",temp, value);
					rewind (config);
					return value;
				}


		}


	}
	rewind (config);
	return "JOY_BUTTON0";

}



void configWrite()
{




}


void configInitDefaults()
{





}
