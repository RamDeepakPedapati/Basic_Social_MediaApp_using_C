

// to save posts info for each user and likes and comments of each post
struct user_posts
{
	int post_offset;
	int likes=0;
	int comment_count=0;
	char comments[5][20];
};

//show messages info among the users 
struct message_info
{
	int sender_id;
	int reciever_id;
	char message[100];
};

//post frame is for storing the post data like type of post along with required fields like userinfo
struct Post
{
	int type;
	char filename[32];
	int user_id;
	int post_id;
	char username[20];
	char post_data[153536];
};


//metadata containg the number of users and nextuser insert position , number of posts and next post position...................
struct meta_data
{
	int users_count;
	int next_insert_position;
	int posts_count;
	int next_post_position;
};

typedef struct user_info
{
	int user_id;
	char username[20];
	char email[30];
	int post_count=0;
	int message_count=0;
	message_info messages[10];
	user_posts posts[10];
};

