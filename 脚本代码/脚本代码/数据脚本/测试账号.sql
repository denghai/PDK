USE QPAccountsDB
GO
INSERT [dbo].[AccountsBack] ([userId], [spreaderID], [operationID], [flag], [pw],[operationTime],  [money]) VALUES ( 1, 1, 1, 1, N'123456',GETDATE(), 0)


USE QPAccountsDB
GO

INSERT AccountsInfo (Accounts,NickName,LogonPass,InsurePass,RegAccounts,SpreaderID,PassPortID,Compellation,Gender,FaceID,
	GameLogonTimes,LastLogonIP,LastLogonMachine,RegisterIP,RegisterMachine,UserRight)
VALUES (N'111111',N'111111',N'111111',N'111111',N'0',0,0,N'',0,0,1,N'',N'',N'',N'',536870912)

INSERT AccountsInfo (Accounts,NickName,LogonPass,InsurePass,RegAccounts,SpreaderID,PassPortID,Compellation,Gender,FaceID,
	GameLogonTimes,LastLogonIP,LastLogonMachine,RegisterIP,RegisterMachine,UserRight)
VALUES (N'222222',N'222222',N'111111',N'111111',N'0',0,0,N'',0,0,1,N'',N'',N'',N'',536870912)

INSERT AccountsInfo (Accounts,NickName,LogonPass,InsurePass,RegAccounts,SpreaderID,PassPortID,Compellation,Gender,FaceID,
	GameLogonTimes,LastLogonIP,LastLogonMachine,RegisterIP,RegisterMachine,UserRight)
VALUES (N'333333',N'333333',N'111111',N'111111',N'0',0,0,N'',0,0,1,N'',N'',N'',N'',536870912)

INSERT AccountsInfo (Accounts,NickName,LogonPass,InsurePass,RegAccounts,SpreaderID,PassPortID,Compellation,Gender,FaceID,
	GameLogonTimes,LastLogonIP,LastLogonMachine,RegisterIP,RegisterMachine,UserRight)
VALUES (N'444444',N'444444',N'111111',N'111111',N'0',0,0,N'',0,0,1,N'',N'',N'',N'',536870912)


DECLARE @UserID INT
SELECT @UserID=UserID	FROM AccountsInfo(NOLOCK) WHERE Accounts=N'111111'
INSERT QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo (UserID, Score,InsureScore, RegisterIP, LastLogonIP) VALUES (@UserID, 10000,10000, N'', N'') 
SELECT @UserID=UserID	FROM AccountsInfo(NOLOCK) WHERE Accounts=N'222222'
INSERT QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo (UserID, Score,InsureScore, RegisterIP, LastLogonIP) VALUES (@UserID, 10000,10000, N'', N'') 
SELECT @UserID=UserID	FROM AccountsInfo(NOLOCK) WHERE Accounts=N'333333'
INSERT QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo (UserID, Score,InsureScore, RegisterIP, LastLogonIP) VALUES (@UserID, 10000,10000, N'', N'') 
SELECT @UserID=UserID	FROM AccountsInfo(NOLOCK) WHERE Accounts=N'444444'
INSERT QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo (UserID, Score,InsureScore, RegisterIP, LastLogonIP) VALUES (@UserID, 10000,10000, N'', N'') 


INSERT AccountsWeakAddRank (UserID,LastLogonTime,UserPoint) VALUES (1,0,1)
INSERT AccountsWeakAddRank (UserID,LastLogonTime,UserPoint) VALUES (2,0,2)
INSERT AccountsWeakAddRank (UserID,LastLogonTime,UserPoint) VALUES (3,0,3)
INSERT AccountsWeakAddRank (UserID,LastLogonTime,UserPoint) VALUES (4,0,4)
INSERT AccountsWeakAddRank (UserID,LastLogonTime,UserPoint) VALUES (5,0,5)

INSERT AccountsQuarterAddRank (UserID,LastLogonTime,UserPoint) VALUES (1,0,100)
INSERT AccountsQuarterAddRank (UserID,LastLogonTime,UserPoint) VALUES (2,0,200)
INSERT AccountsQuarterAddRank (UserID,LastLogonTime,UserPoint) VALUES (3,0,3)
INSERT AccountsQuarterAddRank (UserID,LastLogonTime,UserPoint) VALUES (4,0,400)
INSERT AccountsQuarterAddRank (UserID,LastLogonTime,UserPoint) VALUES (5,0,5)
INSERT AccountsQuarterAddRank (UserID,LastLogonTime,UserPoint) VALUES (6,0,10000)

INSERT AccountsYearAddRank (UserID,LastLogonTime,UserPoint) VALUES (1,0,1121)
INSERT AccountsYearAddRank (UserID,LastLogonTime,UserPoint) VALUES (2,0,4452)
INSERT AccountsYearAddRank (UserID,LastLogonTime,UserPoint) VALUES (3,0,676573)
INSERT AccountsYearAddRank (UserID,LastLogonTime,UserPoint) VALUES (4,0,76674)
INSERT AccountsYearAddRank (UserID,LastLogonTime,UserPoint) VALUES (5,0,887775)
INSERT AccountsYearAddRank (UserID,LastLogonTime,UserPoint) VALUES (6,0,587775)

INSERT ExchangeHuaFeiList (ExchangeID,UseType,UseNum,GetType,GetNum,GoodsName,ExchangeDesc,ImgIcon,Flag) VALUES (1,0,100,1,10,N'10元话费',N'',N'',0)
INSERT ExchangeHuaFeiList (ExchangeID,UseType,UseNum,GetType,GetNum,GoodsName,ExchangeDesc,ImgIcon,Flag) VALUES (2,0,200,1,20,N'20元话费',N'',N'',0)
INSERT ExchangeHuaFeiList (ExchangeID,UseType,UseNum,GetType,GetNum,GoodsName,ExchangeDesc,ImgIcon,Flag) VALUES (3,0,300,1,30,N'30元话费',N'',N'',0)
INSERT ExchangeHuaFeiList (ExchangeID,UseType,UseNum,GetType,GetNum,GoodsName,ExchangeDesc,ImgIcon,Flag) VALUES (4,0,500,1,50,N'50元话费',N'',N'',0)
INSERT ExchangeHuaFeiList (ExchangeID,UseType,UseNum,GetType,GetNum,GoodsName,ExchangeDesc,ImgIcon,Flag) VALUES (5,0,1000,1,100,N'100元话费',N'',N'',0)

USE QPAccountsDB
GO
truncate table ShopInfoList
INSERT ShopInfoList (ItemID,ItemType,OrderID_IOS,OrderID_Android,Price,GoodsNum,ItemTitle,ItemDesc,ItemIcon,ItemName) VALUES (1,1,1001,2001,1,2000,N'10元',N'',N'',N'2000')
INSERT ShopInfoList (ItemID,ItemType,OrderID_IOS,OrderID_Android,Price,GoodsNum,ItemTitle,ItemDesc,ItemIcon,ItemName) VALUES (2,1,1002,2002,25,50000,N'25元',N'',N'',N'50000')
INSERT ShopInfoList (ItemID,ItemType,OrderID_IOS,OrderID_Android,Price,GoodsNum,ItemTitle,ItemDesc,ItemIcon,ItemName) VALUES (3,1,1003,2003,50,11,N'50元',N'',N'',N'8:3')
INSERT ShopInfoList (ItemID,ItemType,OrderID_IOS,OrderID_Android,Price,GoodsNum,ItemTitle,ItemDesc,ItemIcon,ItemName) VALUES (3,1,1003,2003,50,11,N'50元',N'',N'',N'8:3')
INSERT ShopInfoList (ItemID,ItemType,OrderID_IOS,OrderID_Android,Price,GoodsNum,ItemTitle,ItemDesc,ItemIcon,ItemName) VALUES (3,1,1003,2003,50,11,N'50元',N'',N'',N'8:3')

USE QPAccountsDB
GO
truncate table ShopInfoList
INSERT ShopInfoList (ItemID,ItemType,OrderID_IOS,OrderID_Android,Price,GoodsNum,ItemTitle,ItemDesc,ItemIcon,ItemName) VALUES (1,0,1001,2001,1,2000,N'10元',N'',N'',N'2000')
INSERT ShopInfoList (ItemID,ItemType,OrderID_IOS,OrderID_Android,Price,GoodsNum,ItemTitle,ItemDesc,ItemIcon,ItemName) VALUES (2,0,1002,2002,25,50000,N'25元',N'',N'',N'50000')

USE QPTreasureDB
GO

INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (1,2,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (2,2,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (3,4,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (4,4,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (5,4,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (6,5,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (7,5,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (8,5,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (9,2,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (10,4,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (11,5,0,0,0,0,0,0,16777215,4)

INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (20,3,0,0,0,0,0,0,16777215,1)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (21,3,0,0,0,0,0,0,16777215,2)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (22,3,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (23,3,0,0,0,0,0,0,16777215,1)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (24,3,0,0,0,0,0,0,16777215,2)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (25,3,0,0,0,0,0,0,16777215,4)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (26,3,0,0,0,0,0,0,16777215,1)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (27,3,0,0,0,0,0,0,16777215,2)
INSERT AndroidManager (UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,ServiceTime,ServiceGender)
VALUES (28,3,0,0,0,0,0,0,16777215,4)

