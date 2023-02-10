# Medlink 
## Build and Installation
### 1. Build the Medlink app to the win32 release 
### 2. Build the TestUpdate Solution
### 3. Copy the TestUpdate\bin\Release to the release folder of Medlink app
### 4. Open the Med_Link_Client.iss or Med_Link_Host.iss and udpated the outdir and the release folder
### 5. Complie and generate the installation.
## DevOps information
#### Prod Management System https://47.116.77.151:8888/#/meetings/currentMeeting, user:bscadmin,password,bscadmin
#### Prod env use the appid a7947e910b78485f96d63e890017d067
#### Dev Management System https://47.116.77.151:5555/#/meetings/currentMeeting, user:bscadmin,password,bscadmin
#### Dev env use the appid b158865ce08b43f788e909aceda9fb7f
#### the ALIYUN SERVER SSH user:lihao, pwd: qwe123
### the mysql database, user:root,pwd:admin,port:3306, there is both medlink and medlink_dev for prod and dev.
### to update the database, use ssh tunnel, as above.

### Tips
#### new host registion
##### 1. install the host application
##### 2. select the hospital,department and equipment 
##### 3. there will be a UUID generated by the host pc with its disk serial and other, the backend will look for this UUID to identify the host record and returned to the host with its related meeting channel id and later the related client group.

### to remove the host, you can just update the UUID to add "-deleted", and its status to invalid. 

