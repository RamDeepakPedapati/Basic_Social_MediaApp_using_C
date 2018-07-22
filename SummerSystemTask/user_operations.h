
#include "Structures.h"

//return fseek of the user block stored in the file.............................
int return_user_fseek(char *name, FILE *fp)
{

	meta_data md;
	user_info user;
	fseek(fp, 0, SEEK_SET);
	fread(&md, sizeof(meta_data), 1, fp);
	for (int iter = 0; iter < md.users_count; iter++)
	{
		fread(&user, sizeof(user_info), 1, fp);
		if (!strcmp(user.username, name))
		{
			//printf("%d", ftell(fp));
			return ftell(fp) - USER_BLOCK_SIZE;
		}
	}
	return -1;
}



//show all the posts of the users ....................................................
void show_posts(char *username, FILE *fp)
{
	int offset = return_user_fseek(username, fp);
	user_info user;
	Post postitem;
	fseek(fp, offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);

	for (int iter = 0; iter < user.post_count; iter++)
	{
		int offset = user.posts[iter].post_offset;
		if (offset != -1)
		{
			fseek(fp, offset, SEEK_SET);
			fread(&postitem, sizeof(Post), 1, fp);
			printf("postid:%d   ", postitem.post_id);
			if (postitem.type == 2)
			{
				printf("%s\n", postitem.post_data);
			}
			else if (postitem.type == 1)
			{
				printf("filename :%s\n", postitem.filename);
			}
		}
	}
}










//prints the user info when a name is given........................................................
void get_user(FILE *fp)
{
	user_info user;
	char* name = (char*)malloc(sizeof(char) * 20);
	fflush(stdin);
	printf("Enter name of the user :\n");
	gets(name);
	int user_offset = return_user_fseek(name, fp);
	if (user_offset != -1)
	{
		fseek(fp, user_offset, SEEK_SET);
		fread(&user, sizeof(user_info), 1, fp);
		printf("User Deatails are :\n");
		printf("Name:%s\nEmail-ID:%s\n", user.username, user.email);
	}
	
}

//creates a new user.......................................................
void create_user(FILE *fp)
{
	int id,new_user_offset,users_count;
	user_info newuser;
	meta_data md;

	fseek(fp, 0, SEEK_SET);
	fread(&md, sizeof(meta_data), 1, fp);

	fseek(fp, md.next_insert_position,SEEK_SET);

	newuser.user_id = user_ids++;

	printf("Enter user NAME:\n");
	fflush(stdin);
	gets(newuser.username);

	printf("Enter user EMAIL:\n");
	fflush(stdin);
	gets(newuser.email);

	fwrite(&newuser, sizeof(user_info), 1, fp);
	
	
	md.next_insert_position = ftell(fp);


	fseek(fp, 0, SEEK_SET);
	md.users_count = md.users_count + 1;
	fwrite(&md, sizeof(meta_data), 1, fp);
	
	fflush(fp);

}


//updates the user details when name of the user is given......................................
void update_user(char *name,FILE *fp)
{
	user_info user;
	char *newdata = (char*)malloc(sizeof(char) * 20);
	int user_offset = return_user_fseek(name, fp);
	fseek(fp, user_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	fflush(stdin);
	printf("update - ?");

	gets(newdata);
	if (!strcmp(newdata, "name"))
	{
		fflush(stdin);
		printf("Enter new NAME:\n");
		gets(user.username);
	}
	else if (!strcmp(newdata, "email"))
	{
		fflush(stdin);
		printf("Enter new EMAIL:\n");
		gets(user.email);
	}

	fseek(fp, user_offset, SEEK_SET);
	fwrite(&user, sizeof(user_info), 1, fp);
	fflush(fp);
}

//removes the user and his info from the database.................................................

void remove_user(char *name, FILE *fp)
{
	int old_user_offset;

	meta_data md;
	fseek(fp, 0, SEEK_SET);
	fread(&md, sizeof(meta_data), 1, fp);

	user_info user, end_user;
	old_user_offset = return_user_fseek(name, fp);

	fseek(fp, md.next_insert_position - USER_BLOCK_SIZE, SEEK_SET);
	fread(&end_user, sizeof(user_info), 1, fp);

	fseek(fp, old_user_offset, SEEK_SET);
	fwrite(&end_user, sizeof(user_info), 1, fp);

	fseek(fp, 0, SEEK_SET);
	md.users_count = md.users_count - 1;
	md.next_insert_position = md.next_insert_position - USER_BLOCK_SIZE;
	fwrite(&md, sizeof(meta_data), 1, fp);
	fflush(fp);
}


//showing the metadata of the file .......................

void show_metadata(FILE *fp)
{
	fseek(fp, 0, SEEK_SET);
	meta_data md;
	fread(&md, sizeof(meta_data), 1, fp);
	printf("\n%d %d %d %d\n", md.users_count, md.next_insert_position, md.posts_count, md.next_post_position);
	return;
}

//posting a new post when the user login into his account post may be FILE or a  some TEXT
void postNEW(char *name, FILE *fp)
{
	meta_data md;
	user_info user;
	Post new_post;
	int post_type;
	user_posts post_info;

	char *text, *image_path;

	int user_offset = return_user_fseek(name, fp);
	fseek(fp, user_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);

	strcpy(new_post.username, user.username);
	new_post.user_id = user.user_id;
	new_post.post_id = user.post_count;

	printf("Enter post type: [1]-> File \t [2]-> Text \n");

	scanf("%d", &post_type);
	if (post_type == 1)
	{
		image_path = (char*)malloc(sizeof(char) * 50);
		printf("Enter path of the File:\n");
		fflush(stdin);
		gets(image_path);
		FILE  *fp1;
		fp1 = fopen(image_path, "rb+");
		char *image;
		int size;


		image = (char*)malloc(sizeof(char) * 153600);

		
		int i = 0;
		while (!feof(fp1))
		{
			fread(&new_post.post_data[i], 1, 1, fp1);
			i++;
		}
		
		new_post.type = 1;
		strcpy(new_post.filename, image_path);
		fclose(fp1);

	}
	else if (post_type == 2)
	{
		printf("Enter text post:\n");
		fflush(stdin);
		text = (char*)malloc(sizeof(char) * 500);
		gets(text);
		strcpy(new_post.post_data, text);
		new_post.type = 2;
	}

	fseek(fp, 0, SEEK_SET);
	fread(&md, sizeof(meta_data), 1, fp);

	fseek(fp, md.next_post_position, SEEK_SET);
	fwrite(&new_post, sizeof(Post), 1, fp);


	user_offset = return_user_fseek(name, fp);

	user.posts[user.post_count].post_offset = md.next_post_position;
	user.post_count = user.post_count + 1;

	md.posts_count = md.posts_count + 1;
	md.next_post_position = md.next_post_position + POST_BLOCK_SIZE;


	fseek(fp, user_offset, SEEK_SET);
	fwrite(&user, sizeof(user_info), 1, fp);

	fseek(fp, 0, SEEK_SET);
	fwrite(&md, sizeof(meta_data), 1, fp);
	fflush(fp);
	show_posts(name, fp);
}


//updating the post by changing the previous text .............................................
void update_post(char *name,FILE *fp)
{
	char *newtext = (char*)malloc(sizeof(char) * 50);
	int post_id,post_offset;
	Post present_post;
	user_info user;
	int user_offset = return_user_fseek(name, fp);

	show_posts(name,fp);

	fseek(fp, user_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	
	printf("Enter postID from above you want to update:\n");
	scanf("%d",&post_id);
	post_offset = user.posts[post_id].post_offset;

	fseek(fp, post_offset, SEEK_SET);
	fread(&present_post, sizeof(Post), 1, fp);
	
	
	if (present_post.type == 2)
	{
		fflush(stdin);
		printf("Enter new text you want to update:\n");
		gets(newtext);
		strcpy(present_post.post_data, newtext);
	}
	fseek(fp, post_offset, SEEK_SET);
	fwrite(&present_post, sizeof(Post), 1, fp);
	fflush(fp);
	//show_posts(name, fp);

}



//remove the post from the user profille...........................................................
void remove_post(char *name, FILE *fp)
{
	char *newtext;
	int post_id, post_offset;
	Post present_post,last_post;
	user_info user,last_user;
	meta_data md;
	fseek(fp, 0, SEEK_SET);
	fread(&md, sizeof(meta_data), 1, fp);

	int user_offset = return_user_fseek(name, fp);

	show_posts(name,fp);

	fseek(fp, user_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);

	printf("Enter postID from above you want to rempve:\n");
	scanf("%d", &post_id);
	

	post_offset = user.posts[post_id].post_offset;
	user.posts[post_id].post_offset = -1; 
	user.post_count = user.post_count - 1;


	fseek(fp, user_offset, SEEK_SET);
	fwrite(&user, sizeof(user_info), 1, fp);	


	fseek(fp, post_offset, SEEK_SET);
	fread(&present_post, sizeof(Post), 1, fp);

	fseek(fp, md.next_post_position - POST_BLOCK_SIZE, SEEK_SET);
	fread(&last_post, sizeof(Post), 1, fp);

	fseek(fp, post_offset, SEEK_SET);
	fwrite(&last_post, sizeof(Post), 1, fp);

	md.next_post_position = md.next_post_position - POST_BLOCK_SIZE;
	md.posts_count = md.posts_count - 1;

	fseek(fp, 0, SEEK_SET);
	fwrite(&md, sizeof(meta_data), 1, fp);

	int last_user_offset = return_user_fseek(last_post.username, fp);
	fseek(fp, last_user_offset, SEEK_SET);
	fread(&last_user, sizeof(user_info), 1, fp);
	last_user.posts[last_post.post_id].post_offset = md.next_post_position - POST_BLOCK_SIZE;
	fseek(fp, last_user_offset, SEEK_SET);
	fwrite(&last_user, sizeof(user_info), 1, fp);
	fflush(fp);
	//show_posts(name, fp);
}


//saving the posts such the IMAGES AUDIO or VIDEOS are all stored into the local disc(saving name should be explicitly given) ..............................................
void save_post(FILE *fp)
{
	int post_id,user_offset,post_offset;
	user_info user;
	Post post;
	char *user_name = (char*)malloc(sizeof(char)* 20);
	printf("Enter user name :\n");
	fflush(stdin);
	gets(user_name);
	show_posts(user_name, fp);
	printf("Enter post ID you want to save:\n");
	scanf("%d", &post_id);

	user_offset = return_user_fseek(user_name, fp);
	fseek(fp, user_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);

	post_offset = user.posts[post_id].post_offset;
	fseek(fp, post_offset, SEEK_SET);
	fread(&post, sizeof(Post), 1, fp);

	char *data = (char*)malloc(sizeof(char) * 153536);

	strcpy(data, post.post_data);
	if (post.type == 1)
	{
		char *filePath = (char*)malloc(sizeof(char) * 30);
		printf("Enter file name you want to save:\n");
		fflush(stdin);
		gets(filePath);
		/*strcpy(filePath, post.filename);*/
		strcpy(data, post.post_data);
		//strcat(filePath, "_1");
		FILE *fp1 = fopen(filePath, "wb");
		fseek(fp1, 0, SEEK_SET);
		fwrite(post.post_data, sizeof(post.post_data), 1, fp1);
		fclose(fp1);

	}
	fflush(fp);
}

