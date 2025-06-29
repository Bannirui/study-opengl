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