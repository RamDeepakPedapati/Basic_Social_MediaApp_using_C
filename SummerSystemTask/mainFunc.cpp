#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>


#define USER_START_OFFSET 16
#define POST_START_OFFSET 112900	
#define USER_BLOCK_SIZE 2264
#define	POST_BLOCK_SIZE 153600

int user_ids = 0;

#include "user_operations.h"
#include "posts.h"
	
#pragma once




int main()
{
	
	// intially creating the 100MB file ........................................
	FILE *fp = fopen("sample.bin", "wb");
	fseek(fp, 0, SEEK_SET);
	meta_data sample, sample1;
	sample.users_count = 0;
	sample.next_insert_position = 16;
	sample.posts_count = 0;
	sample.next_post_position = 112900;
	fwrite(&sample, sizeof(meta_data), 1, fp);

	fseek(fp, 0, SEEK_SET);
	fread(&sample1, sizeof(meta_data), 1, fp);
	fseek(fp, 104857600, SEEK_SET);
	fputc('\0', fp);
	
	fclose(fp);
	//FILE *fp;
	


	// from the next time above lines will be commented out so that the data wil be persistent for the next run..............

	fp = fopen("sample.bin", "rb+");
	printf("Enter user name:\n");
	char *name = (char*)malloc(sizeof(char) * 30);
	
	int option,flag=0;
	while (1)
	{

		// all user operations are given here like create, removee etc....................
		printf("\nUser Operations : \n[1]-create user \n[2]-remove user \n[3]-update user \n[4]- get user \n[5]-login \n[6] -exit \n");
		scanf("%d", &option);
		switch (option)
		{
		case 1:create_user(fp);
			break;
		case 2:printf("Enter a name:\n");
			fflush(stdin);
			gets(name);
			remove_user(name,fp);
			break;
		case 3:printf("Enter a name:\n");
			fflush(stdin);
			gets(name);
			update_user(name, fp);
			break;
		case 4:get_user(fp);
			break;
		case 5:printf("Enter user name:\n");
			fflush(stdin);
			gets(name);
		case 6:flag = 1;
			break;
		default:
			break;
		}
		fflush(fp);
		if (flag == 1)
			break;
	}
	flag = 0;
	while (1)
	{
		// after login such that giving a name all operations on posts are given here ......

		printf("[1] New Post \n[2] show posts \n[3]- delete post \n[4]- update post \n[5]-save post \n[6]-message user \n[7]- add comment \n[8]-get all messages \n [9] like \n[10]-exit:\n");
		scanf("%d", &option);
		switch (option)
		{
		case 1:postNEW(name, fp);
			break;
		case 2:show_posts(name, fp);
			break;
		case 3:remove_post(name, fp);
			break;
		case 4: update_post(name, fp);
			break;
		case 5: save_post(fp);
			break;
		case 6:message_user(fp);
			break;
		case 7:comment(fp);
			break;
		case 8:get_all_messages(fp);
			break;
		case 9:like(fp, name);
			break;
		case 10:flag = 1;
			break;
		default:
			break;
		}
		if (flag == 1)
			break;
		
	}

	fclose(fp);

	
	system("pause");

	return 0;

}

/*system("fsutil file createnew sample.bin 104857600");*/

