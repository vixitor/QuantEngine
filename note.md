static_cast 在编译时做转化
static cast比c风格的cast要好 c风格的cast可以理解为在尝试各种各样的cast static cast条件更加严格更加安全

Curiously Recurring Template Pattern 奇异递归模板模式 
把运行时多态转化成编译时 具体例子看runtime.cpp和crtp.cpp

编译器编译主要经历四个阶段
1 预处理阶段 展开一些头文件和#开头的语句 
2 编译阶段 把语言转化成汇编码
3 汇编阶段 把汇编码转化成机器码 其中会替换掉一些已经知道的地址 但是有许多地址还不知道
4 链接阶段 把不同的汇编文件汇总起来 替换掉那些在汇编阶段不知道的地址 

buildin_expect() 和 [[likely]]的区别 
buildin_expect限制一个语句的可能性 likely限制一个分支的可能性 
if (buildin_expect) {} if() [[likely]] {} reopen
行为上来讲都是对编译器的建议不一定被优化执行 
通俗的理解cpu在取指令的时候会预先取likely修饰的指令来增快运行速度 

左值和右值
左值通俗的理解成可以放在表达式左边的值 有持续稳定可以被引用的身份 
右值可以理解为表达式右边的值 只是一个值 相当于一个临时变量 写代码时 不能依赖他的身份和地址 
引入左值和右值可以实现移动语义和完美转发 让编译器做更多的优化 

为什么需要内存池 
在低延迟系统中 动态分配内存是很慢的 我们可以在启动的时候申请大量内存然后自定义使用的方式来达到更快的效果

指针减法像是同一个数组内的地址相减 返回的是元素地址之间的差 不同类型的指针之间的减法是非法的

Nagle算法会把tcp的很多小包积攒在一起发送 发小包会带来很多tcp消耗 因为每个tcp都有ip头 tcp头之类的东西 Nagle会带来额外的等待所以在低延迟应用中会把他关掉

在传一个tcp包的时候内核会加重一些control message 用CMSG_SPACE可以得到一段数据加上内核数据的真实大小

如果要直接拿信息的话可以recv(fd_, buf, sizeof(buf)) 但是通过recvmsg 并且使用msg可以控制更多的东西 可以通过msg_control拿到内核写的信息 通过msg_name拿到对端端口的信息 通过iov控制数据写到哪里

lambda一般情况下会比std::function要好 因为std::function内部可以装很多东西 比如普通函数 bind的结果 lambda 所以内部做了一些统一接口的操作 lambda的调用链更加简单 编译器更好做优化 

回顾一下reactor框架 reactor是io线程和worker线程分离的典型 有一个acceptr专门负责监听