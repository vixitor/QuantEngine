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

