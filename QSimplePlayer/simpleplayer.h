#ifndef SIMPLEPLAYER_H
#define SIMPLEPLAYER_H

//文字滚动速度
#define TEXT_SPEED 500 

//安全回收内存
#ifndef SIMPLE_SAFE_RELEASE
#define SIMPLE_SAFE_RELEASE(p) if(p) { delete p; p = NULL;}
#endif //ICE_SAFE_RELEASE

#include <QString>
#include <QVector>
#include <QMap>

#include <QtWidgets>
#include <QSystemTrayIcon>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QMediaPlayer>

#include "simplemusicbutton.h"
#include "simpleaboutform.h"
#include "simplevolbutton.h"
#include "simplelabel.h"
#include "nofocusdelegate.h"

#include "songinfo.h"
#include <QTextCodec>
#include <QFileInfo>

//枚举播放模式
enum Simple_Play_Mode{SINGAL, LISTCIRCLE, SINGALCIRCLE, RANDOM};

//网络音乐信息，专辑图片，歌词获取状态
enum { RECEIVE_INFO, RECEIVE_LINK, RECEIVE_PIC, RECEIVE_LRC };

class SimplePlayer : public QWidget
{
	Q_OBJECT

public:
    explicit SimplePlayer(QWidget *parent = 0);
    ~SimplePlayer();
    void Simple_Open_Music(const QString& filePath);
	
private slots:

	//访问目录结构 
    void simple_open_dir();

	//添加mp3格式的文件 
    void simple_open_music();

	//删除指定行的歌曲
    void simple_remove_current_music();

	//清空播放列表
    void simple_clear_list();

	//请求右键菜单
    void simple_playlisttable_menu_requested(const QPoint &pos);

	//双击播放歌曲 
    void simple_playlisttable_cell_double_clicked(int row, int);

	//更新音乐信息 
    void simple_update_meta_data();

	//更新进度条 
    void simple_update_duration(qint64 duration);

	//更新播放状态 
    void simple_update_state(QMediaPlayer::State state);

	//更新播放进度 
    void simple_set_position(/*int position*/);

	//更新歌词、时间显示 
    void simple_update_position(qint64 position);

	//设置各按钮的单击事件，激活各按钮
    void simple_play_button_clicked();
    void simple_logo_button_clicked();
    void simple_mode_button_clicked();
    void simple_pause_button_clicked();
    void simple_next_button_clicked();
    void simple_last_button_clicked();

	//设置播放模式
    void simple_set_play_mode();

	//程序打开时读取播放列表文件 
    void simple_read_list();

    //程序关闭时
    void simple_close();
	
	//把现有播放列表写入列表文件 
    void simple_write_list();

	//设置当前播放位置
    void simple_set_play_position(int pos);

	//获取当前播放位置
    int simple_get_play_position();

    //激活托盘按钮
    void iconActivated(QSystemTrayIcon::ActivationReason reason);


private:

	//初始化主界面 
    void simple_init_ui();

	//初始化播放模块 
    void simple_init_player();

	//初始化信号-槽连接 
    void simple_init_connections();

	//初始化窗口 
    void simple_init_windows();

	//初始化菜单项
    void simple_init_menu_actions();

	//添加到播放列表 
    void simple_add_list(QStringList list);

	//绘制窗体
	void paintEvent(QPaintEvent *event); 

	//拖拽添加
	void dragEnterEvent(QDragEnterEvent *event); 
	void dropEvent(QDropEvent *event); 



	//定制tablewidget外观
    void simple_color_table();

    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    QPoint dPos; //保存相对偏移值
    QSystemTrayIcon *iTrayIcon;
    QMenu *iTraymenu;
    QAction *quitAction;


	QStringList playList;
	QString playingFile;
    QString simpleDir;
	
	int preIndex;
	int currentIndex;
	int playMode;

    Simple_Music_Button *exitButton;
    Simple_Music_Button *addButton;
    Simple_Music_Button *lastButton;
    Simple_Music_Button *nextButton;
    Simple_Music_Button *playButton;
    Simple_Music_Button *pauseButton;
    Simple_Music_Button *modeButton;
    Simple_Music_Button *logoButton;

    SimpleLabel *nameLabel;
	QLabel *musicianLabel;
	QLabel *albumLabel;
	QLabel *timeLabel;
	QLabel *picLabel;

	QSlider *playSlider;
	QSlider *volSlider;

	QTableWidget *playlistTable;

	QMediaPlayer *mediaPlayer;
	QMediaPlaylist *mediaList;

	QMenu *contextMenuLess;
	QMenu *contextMenuMore;
	QMenu *playModeMenu;

	QActionGroup *modeActionGroup;


	QAction *modeSingal;
	QAction *modeListCircle;
	QAction *modeSingalCircle;
	QAction *modeRandom;
	QAction *addMusic;
	QAction *addFileDiv;
	QAction *removeCurr;
	QAction *removeAll;

    Simple_Vol_Button *volButton;

    Simple_About_Form *aboutForm;

	QString songName;
	QString songArtist;
	QString songId;

	int receiveState;

	int playPosition;
};

#endif 
