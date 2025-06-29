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