###### Paranoya chat server

### Screenshots
![Screen](https://github.com/wair92/paranoyaServer/blob/master/pictures/server.png?raw=true)

### Config
Configuration is set in config.json, which must be loacated in the same directory as running binary of application. On the server side 3 variables cen be set:
    {
        "Ip": "127.0.0.1",
        "Port": 9000,
        "HeartBeatTimer:" 1000
    }

### Messages to Server

#Login
    {
        "Id": "Login",
        "Login": "UserName"
    }

#Message
    {
        "Id": "Message",
        "Sender": "UserName1"
        "Receiver": "UserName2"
        "Message": "Message to send"
    }

#Logout
    {
        "Id": "Logout",
        "Logout": "UserName"
    }

#HearBeat
    {
        "Id": "HeartBeat",
        "UserName": "UserName"
    }

### Messages from Server

#Message
    {
        "Id": "Message",
        "Sender": "UserName1"
        "Receiver": "UserName2"
        "Message": "Message to send"
    }

#LoginConfirm
    {
        "Id": "Message",
        "UserName": "UserName"
        "HeartBeatTimer": 1000
    }

#HeartBeatBack
    {
        "Id": "HeartBeatBack",
        "UserName": "UserName"
    }

### Author

martin.bednar@yandex.ru
