# Basic_Social_MediaApp_using_C

Commands on user data:
- Create User
- Remove User
- Update User
- Get User
 User operations:
- Post post_name “./Path
- Comment on post
- Like post
- Remove Post
- Update Post
- Save Post (Downloads that particular file into local disk
- Message User
- Get all messages

## Basic Description
-	Paging of the data is used(into blocks). Such that a specified page block is assigned or allocated for particular user. 
-	Messages of the user are also stored in the user block 
-	But posts are separately stored from a particular file location. From there each post is stored in a particular block and page offset is stored in users posts info. 
-	For our limitations size of the file is restricted.
-	Posts info in the user block contains all the info about a post (comments, likes etc).
-	A user can only remove himself. No user can remove other user.
-	All the user command on user data are done.
-	All the operations were done like viewing other persons posts, commenting them, liking them, messaging the other user, Retrieving all the messages he got.
-	Only user himself can update, remove his posts, delete comments 
-	METADATA is maintained in the first line to know the number of users, totals number of posts present in the file.
-	Reusing the removed posts space is done here.
-	Any user can download the other user files and save into his local file system.
-	Offset maintenance in the user posts info is used for retrieving the posts of the user. Post IDs are maintained and each time when the other users sees posts I will ask post ID such that likes comments are made.



