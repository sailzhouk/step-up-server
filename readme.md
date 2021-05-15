该服务器为最基础的多路I/O复用服务器，实现的功能只有简单的回射服务，没有提供注册回调函数的功能。
并且目前也仅能处理读取事件。
## Sockaddr
封装了一个`sockaddr_in`类型的成员，向外界提供`getSockaddr`, `getSockaddrIn`, `getlen`的接口。

## Socket
设计了一个Socket类，封装了socket对应的文件描述符，目前创建的socket均为阻塞模式，向外部提供了`toBind`，`toListen`，`toAccept`
等方法，其主要用于服务器端程序创建监听连接请求的socket，或者客户端程序创建socket向服务器发送链接。`toAccept`返回的文件描述符
则直接以文件描述符的形式进行读写操作，未进行封装，有对文件描述符做进一步抽象的必要。

当`toBind`被调用时，Socket类会保存一个`Sockaddr`的指针，使得在`toAccept`时无需重新传入`Sockaddr`

## Select
Select类，将客户的文件描述符全部存放在`clients_`数组中，在对象初始化时必须要提供监听连接请求的Socket对象，通过该对象来初始化类
中其他成员。提供`toSelect`方法封装`::select`调用，当有事件返回时通过`handleActiveEvents`方法对事件进行处理（目前仅仅是完成回
射服务）。现阶段在调用`toSelect`时需要在外部进行循环，这里可能结构也有一些问题。

## Poll
Poll类，基本保持与Select类一致的接口