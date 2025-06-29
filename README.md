# study-opengl
study OpenGL

### 1 NDC(Normalized Device Coordinate)

当屏幕尺寸或者比例发生变化时，显示要跟随屏幕一起拉伸

用-1到1之间的数字表示顶点的坐标，本质就是比例

跟OpenGL交互的时候用NDC坐标

### 2 VBO

Vertex Buffer Object表示了在GPU显存上的一段存储空间对象

VBO的C++中表现为一个uint类型变量，理解为GPU端内存对象的一个id编号

#### 2.1 创建

```glsl
void glGenBuffers(GLsizei n, GLuint* buffers);
```

- n 创建多少个VBO
- buffers 创建出来的VBO编号都放到指向的数组中

VBO的创建仅仅是创建了描述符，还没有真正分配显存

#### 2.2 销毁

```glsl
void glDeleteBuffers(GLsizei n, GLuint* buffers);
```

- n 删除多少个VBO
- buffers 要删除的VBO编号数组

#### 2.3 绑定

将某个资源，与OpenGL状态机中的某个状态插槽进行关联

OpenGL本质就是个巨大的状态机

![](./doc/img/1751186646.png)

- 将从GPU申请到的VBO绑定到OpenGL状态机的插槽上
- CPU告诉OpenGL状态机把数据放到当前的VBO上
- OpenGL定位当前VBO插槽绑定的VBO是谁，就把数据放到了VBO上

```glsl
void glBindBuffer(GLenum target, GLuint buffer);
```

- target 要把当前的VBO绑定到OpenGL状态机的哪个状态插槽上
  - GL_ARRAY_BUFFER OpenGL状态机的插槽 当前VBO插槽 
- buffer 绑定的VBO编号 0表示不绑定任何buffer

#### 2.4 填入数据

```glsl
void glBufferData(GLenum target, GLsizeptr size, const void* data, GLenum usage);
```

- target CPU数据要给哪个VBO 告诉OpenGL状态机找哪个状态插槽 
- size 要装入的数据多大(Byte)
- data 要填入的数据指针
- usage 当前buffer的用法
  - GL_STATIC_DRAW VBO模型数据不会频繁改变
  - GL_DYNAMIC_DRAW VBO模型数据会频繁改变

CPU告诉OpenGL要把内存中数据填入到GPU中，直到这时候GPU才会分配显存用来接收这些数据

### 3 多属性数据

顶点中不单单是顶点坐标信息，还包括顶点的颜色数据，OpenGL会根据差值算法对颜色进行过渡

- single buffer策略 每个属性存储为一个VBO
- interleaved buffer交叉策略 所有属性存储为一个VBO

### 4 VAO

在图形学编程中，对于一组纯粹的数据，往往要给出一段描述结构

三角形顶点的某一个属性，我们需要知道的描述信息是
- size 每个顶点几个数字
- type 每个数字是float类型
- stride 每个顶点的数据步长是Byte 为什么有了size和type还需要stride 因为交叉存储的时候每个顶点存着多个属性信息 所以要明确知道每个顶点的跨度是多大
- offset 每个顶点中有多个属性 某个属性在顶点数据的偏移 比如一个顶点有8个float数据
  - 前3个float是位置 所以位置属性在顶点数据的offset是0个float
  - 中间3个是颜色 所以颜色属性在顶点数据的offset是3个float
  - 后2个是xx属性 所以xx属性在顶点数据的offset是6个float
- 此属性存储在xxx号VBO中

Vertex Array Object 顶点数组对象，用于存储一个Mesh网格所有的顶点属性描述信息

VBO中存储的顶点数组，VAO是个数组，装着的是描述信息
- 0号位是位置数据
- 1号位是颜色数据
- XXX数据

VAO是数组，数组里面状态很多描述信息，每个数组项对应一个描述信息，位置，颜色...

#### 4.1 创建

```glsl
void glGenVertexArrays(GLsizei n, GLuint* arrays);
```
- n 创建多少个VAO
- arrays 创建出来的VAO编号 都放到arrays指向的数组中

#### 4.2 绑定

```glsl
void glBindVertexArray(GLuint array);
```

- array 要绑定的VAO编号
- 
#### 4.3 删除

```glsl
void glDeleteVertexArrays(GLsizei n, GLuint* arrays);
```

- n 要删除多少个VAO
- arrays 要删除的VAO编号存放在数组中

#### 4.4 向VAO中加入VBO描述

```glsl
glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
```

- index VAO是个数组里面放了很多描述信息 要把描述信息放到VAO数组的哪个数组脚标上
- size 这个属性用多少个数字描述
- type 这个属性每个数字的大小是多大(Byte)
- normalized 是否归一化
- stride 每个顶点数据的步长
- pointer 属性在顶点数据内的偏移量

为什么这个API不需要显式指定向VAO中添加的描述信息是哪个VBO的，因为OpenGL的VBO插槽机制，在每时每刻至多只有一个VBO绑定到了OpenGL的VBO插槽上，所以在添加VAO的时候隐含的语义就是用OpenGL状态机VBO插槽的那个VBO