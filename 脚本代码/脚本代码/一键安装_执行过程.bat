@echo off

TITLE 棋牌数据库【Ver6.6_Spreader】 建立脚本启动中... [期间请勿关闭]

md D:\数据库

Rem 建主数据库三个
set rootPath=数据库脚本\

Rem 存储过程
set rootPath=存储过程\用户数据库\
osql -E  -i "%rootPath%绑定机器.sql"
osql -E  -i "%rootPath%标识登录.sql"
osql -E  -i "%rootPath%设置权限.sql"
osql -E  -i "%rootPath%修改密码.sql"
osql -E  -i "%rootPath%修改签名.sql"
osql -E  -i "%rootPath%用户资料.sql"
osql -E  -i "%rootPath%帐号登录.sql"
osql -E  -i "%rootPath%注册帐号.sql"
osql -E  -i "%rootPath%自定头像.sql"
osql -E  -i "%rootPath%游客帐号.sql"
osql -E  -i "%rootPath%轮盘管理.sql"
osql -E  -i "%rootPath%签到管理.sql"
osql -E  -i "%rootPath%低保管理.sql"
osql -E  -i "%rootPath%自定义数据.sql"
osql -E  -i "%rootPath%设置推荐人.sql"
osql -E  -i "%rootPath%兑换话费.sql"
osql -E  -i "%rootPath%商城.sql"
osql -E  -i "%rootPath%新手活动.sql"

set rootPath=存储过程\平台数据库\
osql -E  -i "%rootPath%房间管理.sql"
osql -E  -i "%rootPath%加载节点.sql"
osql -E  -i "%rootPath%加载类型.sql"
osql -E  -i "%rootPath%加载页面.sql"
osql -E  -i "%rootPath%加载种类.sql"
osql -E  -i "%rootPath%连接信息.sql"
osql -E  -i "%rootPath%模块管理.sql"
osql -E  -i "%rootPath%在线信息.sql"
osql -E  -i "%rootPath%加载敏感词.sql"
osql -E  -i "%rootPath%比赛管理.sql"
osql -E  -i "%rootPath%比赛费用.sql"
osql -E  -i "%rootPath%私人场管理.sql"

set rootPath=存储过程\金币数据库\
osql -E  -i "%rootPath%标识登录.sql"
osql -E  -i "%rootPath%道具管理.sql"
osql -E  -i "%rootPath%道具配置.sql"
osql -E  -i "%rootPath%加载机器.sql"
osql -E  -i "%rootPath%加载配置.sql"
osql -E  -i "%rootPath%加载消息.sql"
osql -E  -i "%rootPath%离开房间.sql"
osql -E  -i "%rootPath%列表描述.sql"
osql -E  -i "%rootPath%列表配置.sql"
osql -E  -i "%rootPath%设置权限.sql"
osql -E  -i "%rootPath%查询用户.sql"
osql -E  -i "%rootPath%银行服务.sql"
osql -E  -i "%rootPath%游戏记录.sql"
osql -E  -i "%rootPath%游戏写分.sql"
osql -E  -i "%rootPath%赠送日志.sql"
osql -E  -i "%rootPath%签到配置.sql"
osql -E  -i "%rootPath%排名服务.sql"

set rootPath=存储过程\积分数据库\
osql -E  -i "%rootPath%标识登录.sql"
osql -E  -i "%rootPath%道具管理.sql"
osql -E  -i "%rootPath%道具配置.sql"
osql -E  -i "%rootPath%加载机器.sql"
osql -E  -i "%rootPath%加载配置.sql"
osql -E  -i "%rootPath%加载消息.sql"
osql -E  -i "%rootPath%离开房间.sql"
osql -E  -i "%rootPath%列表描述.sql"
osql -E  -i "%rootPath%列表配置.sql"
osql -E  -i "%rootPath%游戏写分.sql"

set rootPath=存储过程\比赛数据库\
osql -E  -i "%rootPath%标识登录.sql"
osql -E  -i "%rootPath%道具管理.sql"
osql -E  -i "%rootPath%加载机器.sql"
osql -E  -i "%rootPath%加载配置.sql"
osql -E  -i "%rootPath%加载消息.sql"
osql -E  -i "%rootPath%开始结束.sql"
osql -E  -i "%rootPath%离开房间.sql"
osql -E  -i "%rootPath%列表描述.sql"
osql -E  -i "%rootPath%列表配置.sql"
osql -E  -i "%rootPath%设置权限.sql"
osql -E  -i "%rootPath%写入费用.sql"
osql -E  -i "%rootPath%写入奖励.sql"
osql -E  -i "%rootPath%游戏记录.sql"
osql -E  -i "%rootPath%游戏写分.sql"


pause

CLS
@echo off
@echo 正在生成游戏ID ……
set rootPath=数据脚本\
osql -E  -i "%rootPath%标识生成.sql"

COLOR 0A
CLS
@echo off
cls
echo ------------------------------
echo.
echo	主要数据库建立完成，请根据自己平台的积分游戏执行 
echo.
echo.
echo	版权所有： 深圳市网狐科技有限公司
echo ------------------------------

pause


