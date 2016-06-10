#include "simpleplayer.h"

SimplePlayer::SimplePlayer(QWidget *parent)
	: QWidget(parent)
{
    simple_init_ui();
    simple_init_player();
    simple_init_connections();
    simple_init_windows();
    simple_init_menu_actions();
    simpleDir = QApplication::applicationDirPath();
    simple_read_list();
}

SimplePlayer::~SimplePlayer()
{
    this->simple_close();
}

void SimplePlayer::simple_init_windows()
{
    aboutForm = new Simple_About_Form(this);
}

void SimplePlayer::simple_init_menu_actions()
{
    modeSingal = new QAction(QIcon(":/SimplePlayer/Resources/repeat_none.png"), QString::fromLocal8Bit("单曲播放"), this);
    modeListCircle = new QAction(QIcon(":/SimplePlayer/Resources/list_loop.png"), QString::fromLocal8Bit("列表循环"), this);
    modeSingalCircle = new QAction(QIcon(":/SimplePlayer/Resources/repeat_one.png"), QString::fromLocal8Bit("单曲循环"), this);
    modeRandom = new QAction(QIcon(":/SimplePlayer/Resources/shuffle.png"), QString::fromLocal8Bit("随机播放"), this);

    connect(modeSingal, SIGNAL(triggered()), this, SLOT(simple_set_play_mode()));
    connect(modeListCircle, SIGNAL(triggered()), this, SLOT(simple_set_play_mode()));
    connect(modeSingalCircle, SIGNAL(triggered()), this, SLOT(simple_set_play_mode()));
    connect(modeRandom, SIGNAL(triggered()), this, SLOT(simple_set_play_mode()));

	modeActionGroup = new QActionGroup(this);
	modeActionGroup->addAction(modeSingal);
    modeActionGroup->addAction(modeSingalCircle);
	modeActionGroup->addAction(modeListCircle);

	modeActionGroup->addAction(modeRandom);

	modeSingal->setCheckable(true);
	modeListCircle->setCheckable(true);
	modeSingalCircle->setCheckable(true);
	modeRandom->setCheckable(true);
	modeSingal->setChecked(true);

	playModeMenu = new QMenu(modeButton);
	playModeMenu->setStyleSheet(
		"QMenu{padding:5px;background:white;border:1px solid gray;}"
		"QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
		"QMenu::item:selected:enabled{background:#0096ff;color:white;}"
		"QMenu::item:selected:!enabled{background:transparent;}"
		"QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");
	playModeMenu->addActions(modeActionGroup->actions());


	contextMenuLess = new QMenu(playlistTable);
	contextMenuMore = new QMenu(playlistTable);
    addMusic = new QAction(QIcon(":/SimplePlayer/Resources/add_song.png"), QString::fromLocal8Bit("添加歌曲"),this);
    addFileDiv = new QAction(QIcon(":/SimplePlayer/Resources/add_folder.png"), QString::fromLocal8Bit("添加目录"),this);
    removeCurr = new QAction(QIcon(":/SimplePlayer/Resources/remove_song.png"), QString::fromLocal8Bit("移除本曲"),this);
    removeAll = new QAction(QIcon(":/SimplePlayer/Resources/remove_all_song.png"), QString::fromLocal8Bit("移除所有"),this);

    connect(addMusic, SIGNAL(triggered()), this, SLOT(simple_open_music()));
    connect(addFileDiv, SIGNAL(triggered()), this, SLOT(simple_open_dir()));
    connect(removeCurr, SIGNAL(triggered()), this, SLOT(simple_remove_current_music()));
    connect(removeAll, SIGNAL(triggered()), this, SLOT(simple_clear_list()));

	contextMenuLess->addAction(addMusic);
	contextMenuLess->addAction(addFileDiv);
	contextMenuLess->addSeparator();
	contextMenuLess->addAction(removeAll);

	contextMenuMore->addAction(addMusic);
	contextMenuMore->addAction(addFileDiv);
	contextMenuMore->addSeparator();
	contextMenuMore->addAction(removeCurr);
	contextMenuMore->addAction(removeAll);

	contextMenuLess->setStyleSheet(
		"QMenu{padding:5px;background:white;border:1px solid gray;}"
		"QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
        "QMenu::item:selected:enabled{background:#0096ff;color:gray;}"
		"QMenu::item:selected:!enabled{background:transparent;}"
		"QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");

	contextMenuMore->setStyleSheet(
		"QMenu{padding:5px;background:white;border:1px solid gray;}"
		"QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
        "QMenu::item:selected:enabled{background:#0096ff;color:gray;}"
		"QMenu::item:selected:!enabled{background:transparent;}"
		"QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");

}


void SimplePlayer::closeEvent(QCloseEvent *event)
{
//    switch (QMessageBox::information(this, "退出提示", "确认退出客户端", "确认", "取消", 0, 1))
//    {
//    case 0:
//        event->accept();
//        break;
//    case 1:
//    default:
//        event->ignore();
//        break;
//    }

    event->ignore();
    this->hide();
}

void SimplePlayer::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
//    iTrayIcon->showMessage(tr("提示"), tr("已经最小化到系统托盘，双击还原窗口"));
}

void SimplePlayer::mousePressEvent(QMouseEvent *event)
{
    this->dPos =  event->globalPos() - this->pos();
}

void SimplePlayer::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()- this->dPos);
}

void SimplePlayer::simple_init_player()
{
	mediaPlayer = new QMediaPlayer(this);
	mediaList = new QMediaPlaylist(this);
	mediaPlayer->setPlaylist(mediaList);

    playMode = LISTCIRCLE;
	preIndex = -1;
	currentIndex = 0;

    mediaList->setPlaybackMode(QMediaPlaylist::Loop);
}

void SimplePlayer::simple_init_connections()
{
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(simple_update_position(qint64)));
    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(simple_update_duration(qint64)));
    connect(mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(simple_update_state(QMediaPlayer::State)));
    connect(mediaPlayer, SIGNAL(metaDataChanged()), this, SLOT(simple_update_meta_data()));
    connect(playSlider, SIGNAL(sliderMoved(int)), this, SLOT(simple_set_play_position(int)));
    connect(playSlider, SIGNAL(sliderReleased()), this, SLOT(simple_set_position()));
    connect(volButton, SIGNAL(Simple_Volume_Changed(int)), mediaPlayer, SLOT(setVolume(int)));

    connect(playlistTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(simple_playlisttable_cell_double_clicked(int, int)));
    connect(playlistTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(simple_playlisttable_menu_requested(const QPoint &)));
	
    connect(addButton, SIGNAL(clicked()), this, SLOT(simple_open_music()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(playButton, SIGNAL(clicked()), this, SLOT(simple_play_button_clicked()));
    connect(logoButton, SIGNAL(clicked()), this, SLOT(simple_logo_button_clicked()));
    connect(modeButton, SIGNAL(clicked()), this, SLOT(simple_mode_button_clicked()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(simple_next_button_clicked()));
    connect(lastButton, SIGNAL(clicked()), this, SLOT(simple_last_button_clicked()));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(simple_pause_button_clicked()));

}

void SimplePlayer::simple_init_ui()
{
	picLabel = new QLabel( this);
	picLabel->setGeometry(QRect(40, 45, 100, 100));
	picLabel->setScaledContents(true);
    picLabel->setPixmap(QPixmap(":/SimplePlayer/Resources/default_album.png"));
	
    nameLabel = new SimpleLabel(this);
	nameLabel->setGeometry(QRect(150, 55, 220, 20));

	musicianLabel = new QLabel(this);
	musicianLabel->setGeometry(QRect(150, 85, 220, 15));

	albumLabel = new QLabel(this);
	albumLabel->setGeometry(QRect(150, 115, 220, 15));

	playSlider = new QSlider( this);
	playSlider->setObjectName(QStringLiteral("playSlider"));
    playSlider->setStyleSheet("QSlider::groove:horizontal{border:0px;height:4px;}"
        "QSlider::sub-page:horizontal{background:#0096ff;}"
        "QSlider::add-page:horizontal{background:lightgray;} "
        "QSlider::handle:horizontal{background:white;width:10px;border:#51b5fb 10px;border-radius:5px;margin:-3px 0px -3px 0px;}");
	playSlider->setGeometry(QRect(40, 150, 290, 20));
	playSlider->setOrientation(Qt::Horizontal);

	timeLabel = new QLabel( this);
	timeLabel->setObjectName(QStringLiteral("timeLabel"));
	timeLabel->setGeometry(QRect(340, 150, 51, 16));

	playlistTable = new QTableWidget( this);
	playlistTable->setObjectName(QStringLiteral("playlistTable"));
    playlistTable->setGeometry(QRect(25, 260, 350, 320));
	playlistTable->setItemDelegate(new NoFocusDelegate());

    addButton = new Simple_Music_Button( this);
	addButton->setObjectName(QStringLiteral("addButton"));
    addButton->setGeometry(QRect(40, 190, 31, 31));

    lastButton = new Simple_Music_Button( this);
    lastButton->setObjectName(QStringLiteral("lastButton"));
    lastButton->setGeometry(QRect(95, 185, 41, 41));

    playButton = new Simple_Music_Button( this);
    playButton->setObjectName(QStringLiteral("playButton"));
    playButton->setGeometry(QRect(145, 175, 61, 61));

    pauseButton = new Simple_Music_Button( this);
    pauseButton->setObjectName(QStringLiteral("pauseButton"));
    pauseButton->setGeometry(QRect(145, 175, 61, 61));
    pauseButton->setVisible(false);


    nextButton = new Simple_Music_Button( this);
    nextButton->setObjectName(QStringLiteral("nextButton"));
    nextButton->setGeometry(QRect(215, 185, 41, 41));

    modeButton = new Simple_Music_Button( this);
	modeButton->setObjectName(QStringLiteral("modeButton"));
    modeButton->setGeometry(QRect(270, 190, 31, 31));

    volButton = new Simple_Vol_Button( this);
	volButton->setObjectName(QStringLiteral("volButton"));
	volButton->setGeometry(QRect(333, 193, 31, 31));
    volButton->Simple_Set_Volume(100);

    exitButton = new Simple_Music_Button( this);
	exitButton->setObjectName(QStringLiteral("exitButton"));
	exitButton->setGeometry(QRect(364, 3, 29, 31));
    logoButton = new Simple_Music_Button( this);
	logoButton->setObjectName(QStringLiteral("logoButton"));
	logoButton->setGeometry(QRect(8, 3, 120, 48));

	logoButton->raise();
	picLabel->raise();
	playSlider->raise();
	timeLabel->raise();
	playlistTable->raise();
	addButton->raise();
	modeButton->raise();
	nextButton->raise();
	playButton->raise();
	volButton->raise();
	exitButton->raise();

	timeLabel->setText(tr("00:00"));
    nameLabel->setText(tr("歌曲名 " ));
    musicianLabel->setText(tr("音乐家" ));
    albumLabel->setText(tr("专辑" ));

	//设置图标
	QIcon icon_play, icon_play_focus;
    icon_play.addFile(QStringLiteral(":/SimplePlayer/Resources/播放按钮-主界面.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon_play_focus.addFile(QStringLiteral(":/SimplePlayer/Resources/播放按钮-主界面2.png"), QSize(), QIcon::Normal, QIcon::Off);
    playButton->Simple_Set_Button_Icons(icon_play, icon_play_focus);
	playButton->setIconSize(QSize(60,60));
	playButton->setFlat(true);
	playButton->setFocusPolicy(Qt::NoFocus);
	playButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_pause, icon_pause_focus;
    icon_pause.addFile(QStringLiteral(":/SimplePlayer/Resources/暂停按钮-主界面.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon_pause_focus.addFile(QStringLiteral(":/SimplePlayer/Resources/暂停按钮-主界面2.png"), QSize(), QIcon::Normal, QIcon::Off);
    pauseButton->Simple_Set_Button_Icons(icon_pause, icon_pause_focus);
	pauseButton->setIconSize(QSize(60,60));
	pauseButton->setFlat(true);
	pauseButton->setFocusPolicy(Qt::NoFocus);
	pauseButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_exit, icon_exit_focus;
    icon_exit.addFile(QStringLiteral(":/SimplePlayer/Resources/关闭按钮.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon_exit_focus.addFile(QStringLiteral(":/SimplePlayer/Resources/关闭按钮2.png"), QSize(), QIcon::Normal, QIcon::Off);
    exitButton->Simple_Set_Button_Icons(icon_exit, icon_exit_focus);
	exitButton->setIconSize(QSize(20,20));
	exitButton->setFlat(true);
	exitButton->setFocusPolicy(Qt::NoFocus);
	exitButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_logo, icon_logo_focus;
    icon_logo.addFile(QStringLiteral(":/SimplePlayer/Resources/左上角logo按钮.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon_logo_focus.addFile(QStringLiteral(":/SimplePlayer/Resources/左上角logo按钮2.png"), QSize(), QIcon::Normal, QIcon::Off);
    logoButton->Simple_Set_Button_Icons(icon_logo, icon_logo_focus);
	logoButton->setIconSize(QSize(120, 48));
	logoButton->setFlat(true);
	logoButton->setFocusPolicy(Qt::NoFocus);
	logoButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_next, icon_next_focus;
    icon_next.addFile(QStringLiteral(":/SimplePlayer/Resources/下一曲.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon_next_focus.addFile(QStringLiteral(":/SimplePlayer/Resources/下一曲2.png"), QSize(), QIcon::Normal, QIcon::Off);
    nextButton->Simple_Set_Button_Icons(icon_next, icon_next_focus);
	nextButton->setIconSize(QSize(40,40));
	nextButton->setFlat(true);
	nextButton->setFocusPolicy(Qt::NoFocus);
	nextButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_last, icon_last_focus;
    icon_last.addFile(QStringLiteral(":/SimplePlayer/Resources/上一曲.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon_last_focus.addFile(QStringLiteral(":/SimplePlayer/Resources/上一曲2.png"), QSize(), QIcon::Normal, QIcon::Off);
    lastButton->Simple_Set_Button_Icons(icon_last, icon_last_focus);
	lastButton->setIconSize(QSize(40,40));
	lastButton->setFlat(true);
	lastButton->setFocusPolicy(Qt::NoFocus);
	lastButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_mode, icon_mode_focus;
    icon_mode.addFile(QStringLiteral(":/SimplePlayer/Resources/播放模式.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon_mode_focus.addFile(QStringLiteral(":/SimplePlayer/Resources/播放模式2.png"), QSize(), QIcon::Normal, QIcon::Off);
    modeButton->Simple_Set_Button_Icons(icon_mode, icon_mode_focus);
	modeButton->setIconSize(QSize(25,25));
	modeButton->setFlat(true);
	modeButton->setFocusPolicy(Qt::NoFocus);
	modeButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");
	
	QIcon icon_add, icon_add_focus;
    icon_add.addFile(QStringLiteral(":/SimplePlayer/Resources/添加歌曲.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon_add_focus.addFile(QStringLiteral(":/SimplePlayer/Resources/添加歌曲2.png"), QSize(), QIcon::Normal, QIcon::Off);
    addButton->Simple_Set_Button_Icons(icon_add, icon_add_focus);
	addButton->setIconSize(QSize(25,25));
	addButton->setFlat(true);
	addButton->setFocusPolicy(Qt::NoFocus);
	addButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	//去除标题栏
	this->setWindowFlags(Qt::FramelessWindowHint);
	//半透明
	this->setWindowOpacity(0.9);
	setAttribute(Qt::WA_TranslucentBackground);

	//固定大小
    this->setFixedSize(400, 600);

	//设置音乐信息字体颜色
	QColor fontcolor(1, 149, 255);
	QPalette fontPalette;
	fontPalette.setColor(QPalette::WindowText, fontcolor);
	nameLabel->setPalette(fontPalette);
	musicianLabel->setPalette(fontPalette);
	albumLabel->setPalette(fontPalette);

	QColor timeColor(106, 182, 240);
	QPalette timePalette;
	timePalette.setColor(QPalette::WindowText, timeColor);
	timeLabel->setPalette(timePalette);

    nameLabel->setFont(QFont(QString::fromLocal8Bit("msyh"), 12));

    QFont othersfont(QString::fromLocal8Bit("msyh"), 10);
	musicianLabel->setFont(othersfont);
	albumLabel->setFont(othersfont);
	timeLabel->setFont(othersfont);

	volSlider = new QSlider(volButton);
	volSlider->setObjectName(QStringLiteral("volSlider"));
    volSlider->setStyleSheet("QSlider::groove:horizontal{border:0px;height:4px;}"
        "QSlider::sub-page:horizontal{background:#4b0082;}"
        "QSlider::add-page:horizontal{background:lightgray;} "
        "QSlider::handle:horizontal{background:white;width:10px;border:#51b5fb 10px;border-radius:5px;margin:-3px 0px -3px 0px;}");
    volSlider->setGeometry(QRect(40, 150, 290, 20));
    volSlider->setOrientation(Qt::Horizontal);

	if (playlistTable->columnCount() < 1)
		playlistTable->setColumnCount(2);
	QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
	playlistTable->setHorizontalHeaderItem(0, __qtablewidgetitem);

	//设置列宽
	playlistTable->setColumnWidth(0,300);
	playlistTable->setColumnWidth(1,45);
	//禁止编辑单元格内容
	playlistTable->setEditTriggers(QAbstractItemView::NoEditTriggers); 
	//选中一行
	playlistTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	//去掉水平滚动条
	playlistTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//垂直滚动条按项移动
	playlistTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
	//去除水平表头
	playlistTable->horizontalHeader()->setFixedHeight(0);
	//去除垂直表头
	playlistTable->verticalHeader()->setFixedWidth(0);
	//设置无边框
	playlistTable->setFrameShape(QFrame::NoFrame); 
	//设置不显示格子线
	playlistTable->setShowGrid(false); 
	//设置右键菜单
	playlistTable->setContextMenuPolicy(Qt::CustomContextMenu);
	//设置背景颜色
	QPalette pal;
    pal.setBrush(QPalette::Base, QBrush(QColor(199, 237, 204)));
	playlistTable->setPalette(pal);
	//设置竖直滚动条样式
    playlistTable->setStyleSheet("QScrollBar{background:#9400d3; width: 5px;}"
        "QScrollBar::handle{background:orange; border:2px solid transparent; border-radius:5px;}"
        "QScrollBar::handle:hover{background:#ff1093;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	//使拖动操作生效
	playlistTable->setAcceptDrops(true);
	setAcceptDrops(true);

    quitAction = new QAction(tr("退出极简播放器"), this);
    quitAction->setIcon(QIcon(":/SimplePlayer/Resources/close.png"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    iTraymenu=new QMenu();
    iTraymenu->addAction(quitAction);
    iTraymenu->addSeparator();

    iTrayIcon = new QSystemTrayIcon;

    iTrayIcon->setIcon(QIcon(":/SimplePlayer/Resources/logo.png"));
    iTrayIcon->setVisible(true);
    iTrayIcon->setContextMenu(iTraymenu);
    iTrayIcon->setToolTip(tr("极简音乐播放器"));
    connect(iTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    iTrayIcon->showMessage(tr("提示"), tr("欢迎使用极简音乐播放器"), QSystemTrayIcon::Information, 5000);
}

void SimplePlayer::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    //触发后台图标执行相应事件
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }

}


void SimplePlayer::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if(urls.isEmpty())
		return;

	QStringList dropfilename;
	foreach(QUrl u, urls){
		if (u.toString().right(4) == QString(".mp3")){
			dropfilename.append(u.toLocalFile());
		}
	}

	if(!dropfilename.isEmpty()){
        simple_add_list(dropfilename);
	}
}

void SimplePlayer::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
}

void SimplePlayer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.drawPixmap(0, 0 , QPixmap(":/SimplePlayer/Resources/background.png"));
}

void SimplePlayer::simple_playlisttable_cell_double_clicked(int row, int )
{
	QFile file(playList.at(row));
    if (!file.open(QIODevice::ReadOnly)) {
		playlistTable->item(row,1)->setText(QString::fromLocal8Bit("失效"));
		return ;
	}
	file.close();
	mediaList->setCurrentIndex(row);
	mediaPlayer->play();
	playlistTable->item(row, 1)->setText(QString::fromLocal8Bit(""));
}

void SimplePlayer::simple_remove_current_music()
{
	playList.removeAt(currentIndex);
	playlistTable->removeRow(currentIndex);
	mediaList->removeMedia(currentIndex);
	
    simple_color_table();
}

void SimplePlayer::simple_open_music()
{
	QStringList fileList = QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("添加音乐"),QString(),QString("MP3 (*.mp3)"));
    simple_add_list(fileList);
}

void SimplePlayer::simple_open_dir()
{
	QString dirPath = QFileDialog::getExistingDirectory(this,QString::fromLocal8Bit("选择目录"));
	if(dirPath.size() == 0)
		return;

	QDir dir(dirPath);
	QStringList fileList = dir.entryList(QStringList()<<"*.mp3",QDir::Files);
	for(int i = 0;i < fileList.size();i ++)
		fileList[i] = dir.absoluteFilePath(fileList.at(i));
    simple_add_list(fileList);
}

void SimplePlayer::simple_clear_list()
{
	mediaPlayer->stop();
	playList.clear();
	mediaList->clear();
	while(playlistTable->rowCount())
		playlistTable->removeRow(0);

	timeLabel->setText(tr("00:00"));
	nameLabel->setText(tr("Name " ));
    nameLabel->Simple_Stop();
	musicianLabel->setText(tr("Musician" ));
	albumLabel->setText(tr("Album" ));
}

void SimplePlayer::simple_add_list(QStringList list)
{

	foreach(QString fileName,list)
	{
		if(playList.contains(fileName))
			continue;
		playList.append(fileName);

		mediaList->addMedia(QUrl::fromLocalFile(fileName));
		playlistTable->insertRow(playlistTable->rowCount());

		for(int i = 0;i < playlistTable->columnCount();i ++){
			QTableWidgetItem *item  = new QTableWidgetItem;
            item->setFont(QFont(QString::fromLocal8Bit("msyh"), 12));

			playlistTable->setItem(playlistTable->rowCount() - 1,i,item);
			
		}
		playlistTable->item(playlistTable->rowCount() - 1,0)->setText(QFileInfo(fileName).baseName());

	}
    simple_color_table();
    simple_write_list();
}

void SimplePlayer::simple_playlisttable_menu_requested(const QPoint &pos)
{
	if(playlistTable->itemAt(pos))
	{
		currentIndex = playlistTable->rowAt(pos.y());
		contextMenuMore->exec(QCursor::pos());
	}
	else contextMenuLess->exec(QCursor::pos());
}

void SimplePlayer::simple_set_position(/*int position*/)
{
    int pos = simple_get_play_position();

	mediaPlayer->setPosition(pos);

	mediaPlayer->play();
	playButton->setVisible(false);
	pauseButton->setVisible(true);
}

void SimplePlayer::simple_update_position(qint64 position)
{
    if (!playSlider->isSliderDown())
    {
		playSlider->setValue(position);
	}

	//获取该音频文件的总时长参数，单位为毫秒
	qint64 total_time_value = mediaPlayer->duration();

	//这3个参数分别代表了时，分，秒；60000毫秒为1分钟，所以分钟第二个参数是先除6000,第3个参数是直接除1s
	QTime total_time(0, (total_time_value/60000)%60, (total_time_value/1000)%60);

	//传进来的time参数代表了当前的时间
	QTime current_time(0, (position/60000)%60, (position/1000)%60);
	timeLabel->setText(current_time.toString(tr("mm:ss")));

}

void SimplePlayer::simple_update_duration(qint64 duration)
{
	playSlider->setRange(0, duration);
	playSlider->setEnabled(duration > 0);
	playSlider->setPageStep(duration / 10);
}

void SimplePlayer::simple_update_meta_data()
{
    currentIndex = mediaList->currentIndex();
    if (currentIndex < 0)
    {
        return;
    }
	playingFile = playList.at(currentIndex);

	songName.clear();
	songArtist.clear();
    picLabel->setPixmap(QPixmap(":/SimplePlayer/Resources/default_album.png"));


	songName = mediaPlayer->metaData(QMediaMetaData::Title).toString();
	if (songName.size() > 28)
        nameLabel->Simple_Set_Text(songName, TEXT_SPEED);
	else{
        nameLabel->Simple_Stop();
		nameLabel->setText(songName);
	}

	songArtist = mediaPlayer->metaData(QMediaMetaData::Author).toString();
	musicianLabel->setText(songArtist);
	albumLabel->setText(mediaPlayer->metaData(QMediaMetaData::AlbumTitle).toString());	

	if(nameLabel->text() == "")
		nameLabel->setText(QString::fromLocal8Bit("未知歌曲"));
	if(musicianLabel->text() == "")
		musicianLabel->setText(QString::fromLocal8Bit("未知音乐家"));
	if(albumLabel->text() == "")
		albumLabel->setText(QString::fromLocal8Bit("未知专辑"));
	
	if(playlistTable->rowCount() == 0){
		timeLabel->setText(tr("00:00"));
		nameLabel->setText(tr("Name " ));
		musicianLabel->setText(tr("Musician" ));
		albumLabel->setText(tr("Album" ));
	}

}

void SimplePlayer::simple_update_state(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState)
    {
		playButton->setVisible(false);
		pauseButton->setVisible(true);
		
    }
    else
    {
		playButton->setVisible(true);
		pauseButton->setVisible(false);
	}

    if (state != QMediaPlayer::StoppedState)
    {
		playingFile = playList.at(mediaList->currentIndex());
	} 
}

void SimplePlayer::simple_play_button_clicked()
{
    if(mediaPlayer->state() == QMediaPlayer::PausedState)
    {
		mediaPlayer->play();
	}
}

void SimplePlayer::simple_pause_button_clicked()
{
	
    if(mediaPlayer->state() == QMediaPlayer::PlayingState)
    {
		mediaPlayer->pause();
	}
}

void SimplePlayer::simple_next_button_clicked()
{
    if (mediaList->isEmpty())
    {
        return;
    }
    mediaList->setPlaybackMode(QMediaPlaylist::Loop);

	if (mediaList->currentIndex() != mediaList->nextIndex())
    {
        mediaList->setCurrentIndex(mediaList->nextIndex());
    }
	else if (mediaList->currentIndex() != mediaList->previousIndex())
    {
        mediaList->setCurrentIndex(mediaList->previousIndex());
    }

	mediaPlayer->play();
	playlistTable->item(mediaList->currentIndex(), 1)->setText(QString::fromLocal8Bit(""));
    simple_set_play_mode();
}

void SimplePlayer::simple_last_button_clicked()
{
    if (mediaList->isEmpty())
    {
        return;
    }
	mediaList->setPlaybackMode(QMediaPlaylist::Loop);

	if(mediaList->currentIndex() != mediaList->previousIndex())
		mediaList->setCurrentIndex(mediaList->previousIndex());
	else
		mediaList->setCurrentIndex(mediaList->nextIndex());

    if (!mediaPlayer->isAudioAvailable())
    {
		playlistTable->item(mediaList->currentIndex(), 1)->setText(QString::fromLocal8Bit("失效"));
	}
	
	mediaPlayer->play();
	playlistTable->item(mediaList->currentIndex(), 1)->setText(QString::fromLocal8Bit(""));
    simple_set_play_mode();
}


void SimplePlayer::simple_logo_button_clicked()
{
    aboutForm->show();
    aboutForm->move((QApplication::desktop()->width() - aboutForm->width()) / 2,
                    (QApplication::desktop()->height() - aboutForm->height()) / 2);
}

void SimplePlayer::simple_mode_button_clicked()
{
	playModeMenu->exec(QCursor::pos());
}

void SimplePlayer::simple_set_play_mode()
{
    if(modeSingal->isChecked())
    {
        playMode = SINGAL;
		mediaList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
	}
    else if(modeListCircle->isChecked())
    {
        playMode = LISTCIRCLE;
		mediaList->setPlaybackMode(QMediaPlaylist::Loop);
	}
    else if(modeSingalCircle->isChecked())
    {
        playMode = SINGALCIRCLE;
		mediaList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	}
    else if(modeRandom->isChecked())
    {
        playMode = RANDOM;
		mediaList->setPlaybackMode(QMediaPlaylist::Random);
	}
}


void SimplePlayer::simple_read_list()
{
    QFile text(simpleDir + "/SongList.bin");
	QStringList inList;

    if(text.open(QIODevice::ReadOnly)){
		QDataStream in(&text);
        in.setVersion(QDataStream::Qt_5_5);
		in >> inList;
        simple_add_list(inList);
		text.close();
	}

}

void SimplePlayer::simple_write_list()
{
    QFile text(simpleDir + "/SongList.bin");
	QStringList outList = playList;

    if (text.open(QIODevice::WriteOnly))
    {
		QDataStream out(&text);
        out.setVersion(QDataStream::Qt_5_5);
		out << outList;
		text.close();
	}

	
}

void SimplePlayer::Simple_Open_Music(const QString& filePath)
{
	QStringList name;
	name.append(filePath);

    simple_add_list(name);

    if (playList.contains(filePath))
    {
		mediaList->setCurrentIndex(playList.indexOf(filePath));
		mediaPlayer->play();
	}

}

void SimplePlayer::simple_color_table()
{
    for (int i = 0; i < playlistTable->rowCount(); i++)
    {
        for (int j = 0; j < playlistTable->columnCount(); j++)
        {
			QTableWidgetItem *item = playlistTable->item(i, j);
            if (item)
            {
				const QColor color = QColor(255, 255, 255);
				item->setBackgroundColor(color);
			}
		}
        if (i % 2 == 0)
        {
			for (int j = 0; j < playlistTable->columnCount(); j++){
				QTableWidgetItem *item = playlistTable->item(i, j);
                if (item)
                {
					const QColor color = QColor(213, 228, 242);
					item->setBackgroundColor(color);
				}
			}
		}
	}
}

void SimplePlayer::simple_close()
{
    simple_write_list();

    SIMPLE_SAFE_RELEASE(exitButton);
    SIMPLE_SAFE_RELEASE(addButton);
    SIMPLE_SAFE_RELEASE(lastButton);
    SIMPLE_SAFE_RELEASE(nextButton);
    SIMPLE_SAFE_RELEASE(playButton);
    SIMPLE_SAFE_RELEASE(pauseButton);
    SIMPLE_SAFE_RELEASE(modeButton);
    SIMPLE_SAFE_RELEASE(logoButton);

    SIMPLE_SAFE_RELEASE(nameLabel);
    SIMPLE_SAFE_RELEASE(musicianLabel);
    SIMPLE_SAFE_RELEASE(albumLabel);
    SIMPLE_SAFE_RELEASE(timeLabel);
    SIMPLE_SAFE_RELEASE(picLabel);

    SIMPLE_SAFE_RELEASE(playSlider);
    SIMPLE_SAFE_RELEASE(volSlider);

    SIMPLE_SAFE_RELEASE(playlistTable);
    SIMPLE_SAFE_RELEASE(mediaPlayer);
    SIMPLE_SAFE_RELEASE(mediaList);
    SIMPLE_SAFE_RELEASE(contextMenuLess);
    SIMPLE_SAFE_RELEASE(contextMenuMore);

    SIMPLE_SAFE_RELEASE(modeSingal);
    SIMPLE_SAFE_RELEASE(modeListCircle);
    SIMPLE_SAFE_RELEASE(modeSingalCircle);
    SIMPLE_SAFE_RELEASE(modeRandom);
    SIMPLE_SAFE_RELEASE(addMusic);
    SIMPLE_SAFE_RELEASE(addFileDiv);
    SIMPLE_SAFE_RELEASE(removeCurr);
    SIMPLE_SAFE_RELEASE(removeAll);
    SIMPLE_SAFE_RELEASE(modeActionGroup);

    SIMPLE_SAFE_RELEASE(volButton);
    SIMPLE_SAFE_RELEASE(aboutForm);

}

void SimplePlayer::simple_set_play_position(int pos)
{
	playPosition = pos;
}

int SimplePlayer::simple_get_play_position()
{
	return playPosition;
}
