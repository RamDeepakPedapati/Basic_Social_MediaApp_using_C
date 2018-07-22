

//returns all the messages that a user got when he logins into his account..............................................
void get_all_messages(FILE *fp)
{
	char *username = (char *)malloc(sizeof(char) * 100);
	printf("enter username for messages: ");
	scanf("%s", username);
	int offset = return_user_fseek(username,fp);
	user_info user;
	fseek(fp, offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	for (int iter = 0; iter < user.message_count; iter++)
	{
		if (user.messages[iter].reciever_id == user.user_id)
		{
			printf("Message %d - %s\n",iter, user.messages[iter].message);
		}
	}

}


//this module is for sending messages from one user to another user and storing them for further displaying when the other user logged in........................
void message_user(FILE *fp)
{
	char *sender = (char *)malloc(sizeof(char) * 30);
	char *receiver = (char *)malloc(sizeof(char) * 30);
	char *message = (char *)malloc(sizeof(char) * 100);
	user_info user;
	printf("enter sender username:\t");
	scanf("%s", sender);
	printf("enter receiver username:\t");
	scanf("%s", receiver);
	printf("enter message:\t");
	scanf("%s", message);


	int sender_offset =return_user_fseek(sender,fp);
	int receiver_offset = return_user_fseek(receiver,fp);


	fseek(fp, receiver_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	int receiverid = user.user_id;


	fseek(fp, sender_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	user.message_count += 1;
	int senderid = user.user_id;
	user.messages[user.message_count - 1].sender_id = user.user_id;
	user.messages[user.message_count - 1].reciever_id = receiverid;
	strcpy(user.messages[user.message_count - 1].message, message);
	fseek(fp, sender_offset, SEEK_SET);
	fwrite(&user, sizeof(user_info), 1, fp);


	fseek(fp, receiver_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	user.message_count += 1;
	user.messages[user.message_count - 1].sender_id = senderid;
	user.messages[user.message_count - 1].reciever_id = user.user_id;
	strcpy(user.messages[user.message_count - 1].message, message);
	fseek(fp, receiver_offset, SEEK_SET);
	fwrite(&user, sizeof(user_info), 1, fp);
	fflush(fp);
}


// adding comment to particular requested user ..............................
void addcoment(FILE *fp, char *username)
{
	int postid;
	printf("enter post id to coment:\t");
	scanf("%d", &postid);
	int offset = return_user_fseek(username,fp);
	user_info user;
	fread(&user, sizeof(user_info), 1, fp);
}


//showing the comments requested for a user........................................................
void print_coments(user_info user, int postid)
{
	for (int iter = 0; iter < user.posts[postid].comment_count; iter++)
	{
		printf("coment index: %d coment %s\n", iter, user.posts[postid].comments);
	}
}



// any comment modifications on the post are done here like add,remove,view all comments ...............................................
void post_operations(FILE *fp, char *username, int postid)
{
	int offset = return_user_fseek(username,fp);
	user_info user;
	fseek(fp, offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	int option;
	printf("enter [1] ->ADD  [2] REMOVE [3] VIEW for coments:\t");
	scanf("%d", &option);
	if (option == 1)
	{
		char *coment = (char *)malloc(sizeof(char) * 20);
		printf("enter coment:");
		scanf("%s", coment);
		int presentcoment = user.posts[postid].comment_count;
		
		strcpy(user.posts[postid].comments[presentcoment], coment);
		user.posts[postid].comment_count = user.posts[postid].comment_count + 1;
		fseek(fp, offset, SEEK_SET);
		fwrite(&user, sizeof(user_info), 1, fp);

		fflush(fp);
	}
	else if (option == 2)
	{
		print_coments(user, postid);
		printf("enter comentindex to remove:\t");
		int index;
		scanf("%d", &index);
		int count = user.posts[postid].comment_count - 1;
		strcpy(user.posts[postid].comments[index], user.posts[postid].comments[count]);
		user.posts[postid].comment_count--;

		fseek(fp, offset, SEEK_SET);
		fwrite(&user, sizeof(user_info), 1, fp);
		fflush(fp);
	}
	else if (option == 3)
	{
		print_coments(user, postid);
	}

}

// call post operatins ....................
void comment(FILE *fp)
{

	int postid;
	char *username = (char *)malloc(sizeof(char) * 20);
	printf("enter username:\t");
	scanf("%s", username);
	show_posts(username,fp);
	printf("enter post id shown in above to do coment operations:\t");
	scanf("%d", &postid);
	post_operations(fp, username, postid);

}


//viewing all the posts of a particular user and liking there pictures ............................................
void like(FILE* fp, char* name)
{
	user_info user;
	int user_offset = return_user_fseek(name,fp),postid;
	
	fseek(fp, user_offset, SEEK_SET);
	fread(&user, sizeof(user), 1, fp);
	show_posts(name, fp);
	printf("Enter a post id you want to like:\n");
	scanf("%d", &postid);
	user.posts[postid].likes = user.posts[postid].likes + 1;
	fseek(fp, user_offset, SEEK_SET);
	fwrite(&user, sizeof(user), 1, fp);
	fflush(fp);
}