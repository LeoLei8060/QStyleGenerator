# QStyleGenerator

QStyleGenerator 是一个强大的 Qt 样式表编辑器，提供实时预览和智能代码补全功能，帮助开发者更高效地设计 Qt 应用程序的外观。

## 功能特点

- 📝 智能代码编辑器
  - 语法高亮
  - 代码自动补全
  - 实时错误检查
  - UTF-8 支持

- 👀 实时预览
  - 分屏实时预览
  - 多种控件类型展示
  - 支持伪状态预览

- 🎨 预览控件分类
  - 基础控件（按钮、输入框、滑块等）
  - 表格和列表控件
  - 其他高级控件（文本编辑、日期时间、分组框等）

## 支持平台

| Qt 版本 | 编译器 | 状态 | 备注 |
|---------|--------|------|------|
| Qt 5.15.2 | MinGW 8.1 64-bit | ✅ 支持 | 当前版本 |
| Qt 6.x | MSVC | 🚧 开发中 | 计划支持 |
| Qt 6.x | MinGW | 🚧 开发中 | 计划支持 |

## 构建要求

- Qt 5.15.2 或更高版本
- Qt Creator
- QScintilla 2.x

## 构建步骤

1. 使用 Qt Creator 打开：
   - 启动 Qt Creator
   - 选择 `文件` -> `打开文件或项目`
   - 选择项目目录中的 `CMakeLists.txt` 文件

2. 构建项目：
   - 在 Qt Creator 中选择合适的构建套件（Kit）
   - 点击底部的 `构建` 按钮或使用快捷键 `Ctrl + B` 进行构建

构建完成后，可执行文件和所需的 DLL 将位于项目根目录的 `out` 文件夹中。

## 项目结构

```
QStyleGenerator/
├── 3rdparty/           # 第三方库
│   ├── include/        # 头文件
│   ├── lib/           # 库文件
│   └── dll/           # 动态链接库
├── src/               # 源代码
│   ├── main.cpp
│   ├── mainwindow.h
│   └── mainwindow.cpp
├── CMakeLists.txt    # CMake 构建配置
└── README.md         # 项目文档
```

## 使用方法

1. 启动程序
2. 在左侧编辑器中编写 Qt 样式表
3. 右侧预览区域会实时显示样式效果
4. 使用不同的标签页查看各类控件的样式效果

## 贡献

欢迎提交 Issue 和 Pull Request！

## 许可证

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。
