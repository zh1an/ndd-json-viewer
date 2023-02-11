# NDD-JsonViewer

#### 介绍

该项目用于解决 [NDD](https://gitee.com/cxasm/notepad--) 软件的 JSON 格式化的插件。该项目是从 [JSONViewer](https://github.com/kapilratnani/JSON-Viewer) 项目移植的，目前仅支持 **JSON 的格式化和行化**，带有注释的 JSON 可以格式化或者行化，但是**注释的保存与恢复**；对于 JSON5，暂时仅支持部分解析。

#### 编译流程

**前提：**

插件的编译说明请参考[这里](https://gitee.com/cxasm/notepad--/blob/master/%E7%BC%96%E8%AF%91%E8%AF%B4%E6%98%8E.docx)。



该项目仅支持使用 `CMake` 编译。

依赖项：

* Qt Library
* QScint

-DCMAKE_PREFIX_PATH=D:\Qt5.12.11\Qt5.12.11\5.12.11\msvc2015_64\lib\cmake -DQSCINT_SDK_PATH=F:\gitee\QScint_SDK

编译参数需要两个参数：

* `CMAKE_PREFIX_PATH` ：表明 QT 的 SDK 存放位置
* `QSCINT_SDK_PATH` ：表明 QScint 的 SDK 的存放位置

示例：

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=D:\Qt5.12.11\Qt5.12.11\5.12.11\msvc2015_64\lib\cmake -DQSCINT_SDK_PATH=F:\gitee\QScint_SDK -G Ninja
ninja
```



#### 安装教程

将该项目生成的动态库拷贝至 `%NPP_INSTALL_PATH%/plugin`，重启 NPP 即可。

#### 使用说明

快捷键 `Ctrl+F8` 将会格式化选中的文本；也可以通过菜单栏的 `Json View` 中的 `Formatting Json(Ctrl+F8)`按钮格式化选中的文本。

快捷键 `Ctrl+F9` 将会行化选中的文本；也可以通过菜单栏的 `Json View` 中的 `Compress Json(Ctrl+F9)`按钮行化选中的文本。

#### 参与贡献


#### 特技

