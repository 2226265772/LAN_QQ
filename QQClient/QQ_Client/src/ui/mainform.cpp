#include "mainform.h"
#include "ui_mainform.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>


//主窗口与屏幕边缘的距离
int MAIN_FORM_OADDING =10;

//MainForm::MainForm(QString userName, QString nickName, int port, QWidget *parent) :
//    MoveableFramelessWindow(parent),port_number(port),m_nickName(nickName),
//    ui(new Ui::MainForm)
//{
//    ui->setupUi(this);

//    ui->label_nick->setText(nickName);

//    page4_layout = new QVBoxLayout(this);
//    page4_layout->setContentsMargins(0,0,0,0);//设置上下左右的边距均为0
//    page4_layout->setSpacing(0); //This property holds the spacing between widgets inside the layout.
//    ui->page_4->setLayout(page4_layout);

//    //在布局中加入联系人列表
//    collapseView_p = new CollapseView(userName,nickName);
//    page4_layout->addWidget(collapseView_p);

//    //为每个好友项的双击信号进行关联
//    for(int i=0; i<collapseView_p->Get_item_contact_p()->Get_itemv_size(); ++i){
//        connect(collapseView_p->Get_item_contact_p()->Get_item_P(i),SIGNAL(wake_message_to(int,QString)),this,SLOT(WakeChat(int,QString)));
//    }

//    //关联切换皮肤颜色按钮事件
//    connect(ui->pushButton_skin,SIGNAL(clicked()),this,SLOT(doChangeColor()));

//    //获取屏幕
//    QDesktopWidget* desktopWidget = QApplication::desktop();

//    //得到屏幕宽度
//    int screenWidth = desktopWidget->width();

//    //转移到右边
//    this->move(screenWidth-MAIN_FORM_OADDING-this->width(),MAIN_FORM_OADDING);


//    connect(ui->pushButton_minimize,SIGNAL(pressed()),this,SLOT(on_PB_minimize_clicked()));
//    connect(ui->pushButton_shutdown,SIGNAL(pressed()),this,SLOT(on_PB_shutdown_clicked()));
//}

MainForm::MainForm(const UserInformation me, QWidget *parent) :
    MoveableFramelessWindow(parent),m_datebase(me.m_userID),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    m_flag = 0;
    m_myself = me;
    m_onlineCounter = 0;

    InitQQMainWidget();
    linkSignalWithSlot();
}


MainForm::~MainForm()
{
    if(m_datebase.isOpen()){
        m_datebase.close();
    }

    // 清空容器
    clearContainers();


    delete ui;
}

void MainForm::InitQQMainWidget(){
    //创建容纳所有好友列表的窗口
    m_friendListWidget = new CollapseView(this,this);
    page4_layout = new QVBoxLayout(this);
    page4_layout->setContentsMargins(0,0,0,0);//设置上下左右的边距均为0
    page4_layout->setSpacing(0);//This property holds the spacing between widgets inside the layout.
    ui->page_4->setLayout(page4_layout);
    page4_layout->addWidget(m_friendListWidget);


    //设置主界面用户昵称显示
    ui->label_nick->setText(m_myself.m_nickname);
    //获取屏幕
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //得到屏幕宽度
    int screenWidth = desktopWidget->width();
    //转移到右边
    this->move(screenWidth-MAIN_FORM_OADDING-this->width(),MAIN_FORM_OADDING);

    m_mainCtrl = new QQMainCtrl(m_myself.m_userID);

    //待完成


//    page4_layout = new QVBoxLayout(this);
//    page4_layout->setContentsMargins(0,0,0,0);//设置上下左右的边距均为0
//    page4_layout->setSpacing(0); //This property holds the spacing between widgets inside the layout.
//    ui->page_4->setLayout(page4_layout);

//    //在布局中加入联系人列表
//    collapseView_p = new CollapseView(m_myself.m_userID,m_myself.m_nickname);
//    page4_layout->addWidget(collapseView_p);

//    //为每个好友项的双击信号进行关联
//    for(int i=0; i<collapseView_p->Get_item_contact_p()->Get_itemv_size(); ++i){
//        connect(collapseView_p->Get_item_contact_p()->Get_item_P(i),SIGNAL(wake_message_to(int,QString)),this,SLOT(WakeChat(int,QString)));
//    }
}

/*************************************************
Function Name： InitMainWidget
Description: 初始化
*************************************************/
void MainForm::InitMainWidget(){
    m_mainCtrl = new QQMainCtrl(m_myself.m_userID);
}

void MainForm::linkSignalWithSlot(){
    //关联切换皮肤颜色按钮事件
    connect(ui->pushButton_skin,SIGNAL(clicked()),this,SLOT(doChangeColor()));

    connect(ui->pushButton_minimize,SIGNAL(pressed()),this,SLOT(on_PB_minimize_clicked()));
    connect(ui->pushButton_shutdown,SIGNAL(pressed()),this,SLOT(on_PB_shutdown_clicked()));

    //m_mainCtrl相关
    connect(m_mainCtrl, SIGNAL(getFriendsSuccess(QVector<FriendInformation>)),
            this, SLOT(setFriendListWidget(QVector<FriendInformation>)));//设置好友列表


//    connect(m_mainCtrl, SIGNAL(getFriendTalkMessage(TalkMessage)),
//            this, SLOT(receiveFriendTalkMessage(TalkMessage))); //获取好友发来的消息

//    connect(m_mainCtrl, SIGNAL(getFriendChangedStatus(QString,int)),
//            this, SLOT(setFriendStatus(QString,int))); //设置好友状态
//    connect(m_mainCtrl, SIGNAL(getFriendChangedInformation(FriendInformation)),
//            this, SLOT(setFriendInformation(FriendInformation))); //设置好友信息

//    connect(m_mainCtrl, SIGNAL(getFriendInformationSuccess(UserInformation)),
//            this, SLOT(showFriendInformation(UserInformation)));//显示好友信息


//    connect(m_mainCtrl, SIGNAL(getMyInformationSuccess(UserInformation)),
//            this, SLOT(showMyInformation(UserInformation)));//显示自己信息

//    connect(m_mainCtrl, SIGNAL(changeMyInformationSuccess(UserInformation)),
//            this, SLOT(setMyInformation(UserInformation))); //设置个人信息

//    connect(m_cbStatus, SIGNAL(currentIndexChanged(int)),
//            this, SLOT(changeMyStatus(int)));//改变个人状态（发送至服务器）
//    connect(m_mainCtrl, SIGNAL(changeFriendRemarkSuccess(TempStrings)),
//            this, SLOT(setFriendRemark(TempStrings)));//设置好友备注
//    connect(m_mainCtrl, SIGNAL(deleteFriendSuccess(QString, QString, QString)),
//            this, SLOT(deletedFriend(QString, QString, QString)));//已删除好友

//    connect(m_mainCtrl, SIGNAL(getFriendRequest(TalkMessage, UserInformation)),
//            this, SLOT(receiveFriendRequest(TalkMessage, UserInformation)));//接收并且处理好友的请求

    connect(m_mainCtrl, SIGNAL(closeWindowSignal()),
            this, SLOT(closeWindow()));

//    connect(m_leSearch, SIGNAL(textChanged(QString)),
//            this, SLOT(searchFriend(QString)));//搜索好友
//    connect(m_toolBtnSearch, SIGNAL(clicked()),
//           this, SLOT(onClickSearchFriend()));//点击“搜索”按钮

//    connect(m_toolBtnAddFriend, SIGNAL(clicked()),
//            this, SLOT(onClickAddFriend()));//添加好友


//    connect(m_toolBtnMesBox, SIGNAL(clicked()),
//            this, SLOT(showMessageWidget()));//显示消息管理窗口
//    connect(m_toolBtnConfig, SIGNAL(clicked()),
//            this, SLOT(onClickRightButton()));//右键点击按键
//    connect(m_toolBtnNewMes, SIGNAL(clicked()),
//            this, SLOT(showLatestMessageWidget()));//显示最新消息窗口

//    connect(m_mainCtrl, SIGNAL(renameBoxSuccess(QString,QString)),
//            this, SLOT(renameBoxSuccess(QString, QString)));//重命名分组成功
//    connect(m_mainCtrl, SIGNAL(moveFriendToBoxSuccess(QString,QString,QString)),
//            this, SLOT(moveFriendToBoxSuccess(QString,QString,QString)));//移动好友至其他分组

//    connect(m_mainCtrl, SIGNAL(getNetworkHistoryMessagesSuccess(QString,QDate,QVector<TalkMessage>)),
//            this, SLOT(setNetworkMessageWidget(QString,QDate,QVector<TalkMessage>)));//设置消息界面

//    connect(m_messageListWidget, SIGNAL(timerStatus(bool)),
//            this, SLOT(setTimerStatus(bool)));//设置显示有新消息的定时器

}

/*************************************************
Function Name： getLocalMyInformation()
Description: 获取本地个人信息
*************************************************/
UserInformation MainForm::getLocalMyInformation() const{
    return m_myself;
}

/*************************************************
Function Name： getLocalMyInformation()
Description: 判断是否包含该好友
*************************************************/
bool MainForm::isMyFriend(const QString & id) const
{
    if (m_friendMap.contains(id))
        return true;
    return false;
}

/*************************************************
Function Name： getFriendInformation
Description: 获取好友信息（发送至服务器）
*************************************************/
void MainForm::getFriendInformation(const QString &id)
{
    if (NULL != m_mainCtrl)
        m_mainCtrl->getFriendInformation(id);
}

/*************************************************
Function Name： changeMyInformation
Description: 改变自己信息（发送至服务器）
*************************************************/
void MainForm::changeMyInformation(const UserInformation &me)
{
    if (NULL != m_mainCtrl)
        m_mainCtrl->changeMyInformation(me);
}


/*************************************************
Function Name： changeFriendRemark
Description: 改变好友备注（发送至服务器）
*************************************************/
void MainForm::changeFriendRemark(const QString & friendID,
                                      const QString & remark)
{
    if (NULL != m_mainCtrl)
        m_mainCtrl->changeFriendRemark(m_myself.m_userID, friendID, remark);
}

/*************************************************
Function Name： deleteFriend
Description: 删除好友（发送至服务器
*************************************************/
void MainForm::deleteFriend(const QString & friendID)
{
    if (NULL != m_mainCtrl)
        m_mainCtrl->deleteFriend(m_myself.m_userID, friendID);
}

// 获取某个分组里所有好友的备注名列表
QStringList MainForm::getGroupFriendList(const QString & groupName)
{
    m_friendsMarkList.clear();
    m_friendsIDList.clear();
    if (!m_indexFriendsGroupMap.contains(groupName))
    {
        return m_friendsMarkList;
    }

    int groupIndex = m_indexFriendsGroupMap[groupName];
    //待完成
    CollapseViewItem *group = m_listItemsFriendsVec[groupIndex];
    const QList<QWidget *>* friendList = group->getWidgetList();

    QString mark;
    FriendInformation friendInfor;

    for (int i=0; i<friendList->size(); i++)
    {
        LitterIem *friendBtn = (LitterIem *)friendList->value(i);
        friendInfor = friendBtn->getInformation();

        if(friendInfor.m_remarkName.isEmpty())
            mark = friendInfor.m_nickname;
        else
            mark = friendInfor.m_remarkName;
        m_friendsMarkList.append(mark);
        m_friendsIDList.append(friendInfor.m_userID);
    }
    return m_friendsMarkList;
}

/*************************************************
Function Name： setFriendListWidget
Description: 设置好友列表
*************************************************/
void MainForm::setFriendListWidget(const QVector<FriendInformation> &friendsVec)
{
    qDebug()<<"MainForm::setFriendListWidget 289行";
    int num = 0;
    int len = friendsVec.size();
    for(int i=0; i<len; i++)
    {
        //为相应好友列表添加好友按钮，如果不存在该列表则创建列表后添加
        addFriendButton(friendsVec[i]);
        if (OFFLINE != friendsVec[i].m_status && INVISIBLE != friendsVec[i].m_status)
            num++;
    }

    //设置在线人数
    setOnlineCounter(num);
}

/*************************************************
Function Name： addFriendButton
Description: 为相应好友列表添加好友按钮，如果不存在该列表则创建列表后添加
*************************************************/
bool MainForm::addFriendButton(const FriendInformation & friInfo)
{
    if (m_friendMap.contains(friInfo.m_userID)) //已经添加过该好友与好友按钮的映射
        return false;
    QString groupName = friInfo.m_groupName;
    if (!m_friendsGroupList.contains(groupName)) //如果添加的这个好友所在的分组号还未创建过
    {
        // 存储新的分组号
        m_friendsGroupList.append(groupName);

        // 新建并存储分组

        CollapseViewItem *listItem = new CollapseViewItem(groupName);

        //待完成 first
//        connect(listItem, SIGNAL(removeBoxSignal(QString)),
//                this, SLOT(removeBox(QString)));
//        connect(listItem, SIGNAL(renameBoxSignal(QString)),
//                this, SLOT(renameBox(QString)));

        m_listItemsFriendsVec.push_back(listItem);
//        toolBox->addItem(toolItem);
//        m_friendListWidget->addItem(toolBox);
        m_friendListWidget->addItem(listItem);

        // 存储映射关系
        m_indexFriendsGroupMap.insert(groupName, m_listItemsFriendsVec.size()-1);
    }

    LitterIem *button = new LitterIem(friInfo,&m_friendsGroupList,this, this);
    //为选项卡添加好友项目
    m_listItemsFriendsVec[m_indexFriendsGroupMap.value(groupName)]->addSubItem(button);
    m_friendMap.insert(friInfo.m_userID, button);
    return true;
}

/*************************************************
Function Name： setOnlineCounter
Description: 设置在线人数
*************************************************/
void MainForm::setOnlineCounter(int num)
{
    m_onlineCounter = num;
//    m_labelOnlineNum->setText(QString(tr("在线人数: %1"))
//                              .arg(QString::number(num)));

}

/*************************************************
Function Name： closeWindow
Description: 关闭主窗口
*************************************************/
void MainForm::closeWindow()
{
    m_flag = 1;
    closeAllWindows();
    m_mainCtrl->closeConnect();
    close();
}

/*************************************************
Function Name： closeAllWindows
Description: 关闭所有别的窗口
*************************************************/
void MainForm::closeAllWindows()
{
    //需要关闭的别的窗口(如聊天窗口之类的）

}


/*************************************************
Function Name： clearContainers
Description:  清理容器
*************************************************/
void MainForm::clearContainers()
{
    m_friendsGroupList.clear();

    m_indexFriendsGroupMap.clear();
    //Deletes all the items in the range [begin, end) using the C++ delete operator. The item type must be a pointer type
    qDeleteAll(m_friendMap.begin(), m_friendMap.end());
    m_friendMap.clear();

    qDeleteAll(m_listItemsFriendsVec.begin(), m_listItemsFriendsVec.end());
    m_listItemsFriendsVec.clear();

}

/*************************************************
Function Name： getFriendsInformation()
Description: 获取好友信息列表
*************************************************/
void MainForm::getFriendsInformation()
{
    if (NULL != m_mainCtrl)
        m_mainCtrl->getFriendsInformation(m_myself.m_userID, m_myself.m_status);
}


QWidget *MainForm::getDragnWidget()
{
    return ui->main_top;
}

void MainForm::on_PB_minimize_clicked()
{
    this->showMinimized();
}

void MainForm::on_PB_shutdown_clicked()
{
    this->deleteLater();
}

void MainForm::doChangeColor()
{

    MY_COLOR_PICK_FORM.show();
    MY_COLOR_PICK_FORM.raise(); //Raises this widget to the top of the parent widget's stack.
    MY_COLOR_PICK_FORM.setFocus(); //Gives the keyboard input focus to this widget (or its focus proxy) if this widget or one of its parents is the active window.

    ColorPickForm *f= &(MY_COLOR_PICK_FORM);

    connect(f,SIGNAL(themColorChanged(QString)),this,SLOT(onThemeColorChange(QString)));

}

void MainForm::onThemeColorChange(QString colorStr)
{
    //组合成qcc样式表
    QString style=QString("#%1{background-color:%2;}").arg(ui->centralwidget->objectName(),colorStr);
    //定义样式
    ui->centralwidget->setStyleSheet(style);

}
