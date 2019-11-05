voxel-based-global-illumination
==

## 程序框架
### Engine
- 负责glfw窗口、glad等初始化
- 负责接收各种事件，注册回调各类回调函数
- 渲染循环

### AssetsManager
- 负责加载程序所需的所有资源：包括相机、模型、Shader程序、渲染器

### Model
- 模型类保存了模型的所有数据：包括网格、材质、纹理贴图
- 网格类包含：顶点、索引以及材质指针
- 材质类包含：各种材质属性数据以及纹理贴图指针
- 网格绘制时，会调用当前Renderer类的SetMaterialUniforms函数，并向其传递当前网格的材质，供其设置

### Program
- Shader程序类封装了shader创建、附加、链接、使用的所有流程
- 提供了与Shader程序传递各类数据的接口

### Renderer
- 组织调用Shader程序，并为其设置数据


## 渲染流程
### 体素化
1. 计算得到最小包围盒
2. 正交投影
3. 混合在同一个体素内的颜色
