开始参考muduo库进行编写，之前已经阅读过一遍muduo的源码，但是从整体去看难免会在细节的理解上有缺失，打算从基础开始，参考muduo的实现，逐步地去完善一个服务器，来深化自己的理解。这也是“step-up“的含义所在。



**代码外的一些改动**

- 添加了命名空间
- 添加了Uncopyable基类，其与muduo的noncopyable功能一致，换名字是因为看了Effective C++（...），并且觉得首字母大写看起来规整些
- 使用.clang-format来规范代码格式，使用的是谷歌代码规范



## Channel 

实现了channel类，该类基本按照muduo的思想来实现的，其所有权归属于一个EventLoop，封装了一个文件描述符，文件描述符可以是socket，也可以是eventfd，timerfd等。

其可以：

- 设置文件描述符对应的关注事件（用于epoll）
- 向epoll中注册监听事件（通过所属EventLoop找到对应的epoll）
- 注册不同事件的回调函数
- 处理事件，根据返回的事件类型调用相应的回调函数

也就是说Channel负责几乎一切其所属文件描述符上的动作，对比之前实现的版本，当事件发生时我们是在Epoll\Poll\Select中执行相应事件，这与这些类自己的抽象概念并不符合，也缺乏灵活性，现在将这些动作抽出来就显得合理许多，同时这也为后续muduo的**one loop per thread + thread pool**模型奠定了基础，只有将文件描述符上对应的事件处理\设置等功能抽象出来，才能实现在不同的Event Loop中做不同的任务，否则就只能像之前的版本一样在一个线程中包揽所有的事件处理。

## EventLoop

EventLoop是事件循环类，在muduo的**one loop per thread + thread pool**中，一个IO线程会持续地运行EventLoop中的循环方法，通过其包含的一个Epoll的对象来进行事件监听，随后逐次地调用活动的channel自身的回调函数进行处理。

这里与muduo的区别在于采用了deque来替代vector作为存储活动通道的容器，好处在于deque有更高的内存利用率，至于时间效率方面可能没有什么提升，这里可以后续做测试。

## Epoll

删除了对Poll和Select的支持，即便Epoll不是在所有情况下都优于另外两者，但总的来说Epoll还是更强大的。

Epoll和Channel通过EventLoop关联了起来，每一个Epoll也是属于一个EventLoop的，Channel更新监听事件，Epoll返回活动事件都通过EventLoop进行传递。

## Acceptor

通过EventLoop、Channel和Epoll类的支持，服务器上的事件就可以被抽象为一个个独立的概念，之前服务器接收连接的事件需要通过条件语句进行判断，如今用Acceptor这个类，通过回调函数就可以优雅的实现上述操作，不得不感叹面对对象的巧妙。

Acceptor类包含一个监听Socket，通过它创建一个Channel，保管它的生存周期，以及新连接建立时的回调函数（用于处理接收到的对等方文件描述符），以及自身的handleRead函数（用于向监听Channel中注册回调函数）。

这里Acceptor用了一个vector保存所有接收到的客户端Channel，维护其生命周期

## Echo

类似于一个TcpServer，相当于特化成只能进行回射服务的服务器，同时由于没有对TcpConnection进行抽象，因此其回调函数echoOnClient还必须要接收文件描述符作为参数。

## Sockaddr
封装了一个`sockaddr_in`类型的成员，向外界提供`getSockaddr`, `getSockaddrIn`, `getlen`的接口。

## Socket
设计了一个Socket类，封装了socket对应的文件描述符，目前创建的socket均为阻塞模式，向外部提供了`toBind`，`toListen`，`toAccept`等方法，其主要用于服务器端程序创建监听连接请求的socket，或者客户端程序创建socket向服务器发送链接。

增加了端口服用的设置

与muduo的区别在于

- Accept，bind等实现细节放到了相应的Socket中
- 当`toBind`被调用时，Socket类会保存一个`Sockaddr`的指针，使得在`toAccept`时无需重新传入`Sockaddr`

## Select、Poll、Epoll
Select类，将客户的文件描述符全部存放在`clients_`数组中，在对象初始化时必须要提供监听连接请求的Socket对象，通过该对象来初始化类
中其他成员。提供`toSelect`方法封装`::select`调用，当有事件返回时通过`handleActiveEvents`方法对事件进行处理（目前仅仅是完成回
射服务）。现阶段在调用`toSelect`时需要在外部进行循环，这里可能结构也有一些问题。

Poll类和Epoll类也与Select类保持一致的接口

## 存在的问题

在参考了大神的实现后，整体的框架变得逐渐合理了起来，目前还缺少TcpConnection，也就是对连接这个概念的抽象，目前的连接，对端口的读写操作也用着最原始的接口，没有定时器，没有实现非阻塞IO、没有日志信息、仍然是单线程reactor的结构，下一步希望能先引入多线程的一些接口，只有这样对后续功能的进一步抽象才是合理的
