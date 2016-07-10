#解决QT5.5无法使用搜狗输入法的问题

1. git clone git@github.com:fcitx/fcitx-qt5.git
2. cd fcitx-qt5 
3. 修改根目录文件CMakeLists.txt, 在set(CMAKE_MODULE_PATH这一行之后添加
set(Qt5Core_DIR /opt/Qt5.5.1/5.5/gcc_64/lib/cmake/Qt5Core/)
set(Qt5Gui_DIR /opt/Qt5.5.1/5.5/gcc_64/lib/cmake/Qt5Gui/)
set(Qt5Widgets_DIR /opt/Qt5.5.1/5.5/gcc_64/lib/cmake/Qt5Widgets/)
set(Qt5DBus_DIR /opt/Qt5.5.1/5.5/gcc_64/lib/cmake/Qt5DBus/)

4. 编译：
cmake .
make
5. 最后把编译得到的libfcitxplatforminputcontextplugin.so 拷贝到QT安装目录
cp libfcitxplatforminputcontextplugin.so /opt/Qt5.5.1/Tools/QtCreator/bin/plugins/platforminputcontexts/
cp libfcitxplatforminputcontextplugin.so /opt/Qt5.5.1/5.5/gcc_64/plugins/platforminputcontexts/ 
6.  在~/.bash_profile文件中添加:
GTK_IM_MODULE=fcitx
export XIM_PROGRAM=fcitx
export XIM=fcitx
export QT_IM_MODULE=fcitx
export GTK_IM_MODULE=fcitx
export XMODIFIERS="@im=fcitx"
