
----------------------------------------------------------------------------------------------------

USE QPTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_GameRecord_List_RecordID]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_GameRecord_List_RecordID]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_PrivateGameRecord]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_PrivateGameRecord]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_PrivateGameRecord_Child]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_PrivateGameRecord_Child]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_RecordDrawInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_RecordDrawInfo]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_RecordDrawScore]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_RecordDrawScore]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_BeginnerPlayGame]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_BeginnerPlayGame]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_PrivateGameRecord]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_PrivateGameRecord]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_PrivateGameRecordChild]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_PrivateGameRecordChild]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_PrivateGameRecordUserRecordID]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_PrivateGameRecordUserRecordID]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_BeginnerPlayGame]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_BeginnerPlayGame]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------
-- 游戏记录
CREATE PROC GSP_GP_GameRecord_List_RecordID
	@dwUserID INT								-- 用户 I D
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	SELECT  TOP 10 RecordID FROM PrivateGameRecordUserRecordID WHERE UserID = @dwUserID ORDER BY InsertTime DESC
END

RETURN 0

GO
----------------------------------------------------------------------------------------------------
-- 游戏记录
CREATE PROC GSP_GP_PrivateGameRecord
	@dwRecordID INT								-- 用户 I D
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN
	SELECT UserDate FROM PrivateGameRecord WHERE RecordID=@dwRecordID
END

RETURN 0

GO
----------------------------------------------------------------------------------------------------
-- 游戏记录
CREATE PROC GSP_GP_PrivateGameRecord_Child
	@dwRecordID INT								-- 用户 I D
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN
	SELECT * FROM PrivateGameRecordChild WHERE RecordID=@dwRecordID
END

RETURN 0

GO

GO
----------------------------------------------------------------------------------------------------

-- 游戏记录
CREATE PROC GSP_GR_RecordDrawInfo

	-- 房间信息
	@wKindID INT,								-- 游戏 I D
	@wServerID INT,								-- 房间 I D

	-- 桌子信息
	@wTableID INT,								-- 桌子号码
	@wUserCount INT,							-- 用户数目
	@wAndroidCount INT,							-- 机器数目

	-- 税收损耗
	@lWasteCount BIGINT,						-- 损耗数目
	@lRevenueCount BIGINT,						-- 游戏税收

	-- 统计信息
	@dwUserMemal BIGINT,						-- 损耗数目
	@dwPlayTimeCount INT,						-- 游戏时间

	-- 时间信息
	@SystemTimeStart DATETIME,					-- 开始时间
	@SystemTimeConclude DATETIME,				-- 结束时间

	@dataUserDefine image						-- 游戏自定义数据
	
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 插入记录
	INSERT RecordDrawInfo(KindID,ServerID,TableID,UserCount,AndroidCount,Waste,Revenue,UserMedal,StartTime,ConcludeTime,DrawCourse)
	VALUES (@wKindID,@wServerID,@wTableID,@wUserCount,@wAndroidCount,@lWasteCount,@lRevenueCount,@dwUserMemal,@SystemTimeStart,@SystemTimeConclude,@dataUserDefine)
	
	-- 读取记录
	SELECT SCOPE_IDENTITY() AS DrawID

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 游戏记录
CREATE PROC GSP_GR_RecordDrawScore

	-- 房间信息
	@dwDrawID INT,								-- 局数标识
	@dwUserID INT,								-- 用户标识
	@wChairID INT,								-- 椅子号码

	-- 用户信息
	@dwDBQuestID INT,							-- 请求标识
	@dwInoutIndex INT,							-- 进出索引

	-- 成绩信息
	@lScore BIGINT,								-- 用户积分
	@lGrade BIGINT,								-- 用户成绩
	@lRevenue BIGINT,							-- 用户税收
	@dwUserMedal INT,							-- 奖牌数目
	@dwPlayTimeCount INT						-- 游戏时间

WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 插入记录
	INSERT RecordDrawScore(DrawID,UserID,ChairID,Score,Grade,Revenue,UserMedal,PlayTimeCount,DBQuestID,InoutIndex,InsertTime)
	VALUES (@dwDrawID,@dwUserID,@wChairID,@lScore,@lGrade,@lRevenue,@dwUserMedal,@dwPlayTimeCount,@dwDBQuestID,@dwInoutIndex,GetDate())
	
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------

-- 游戏记录
CREATE PROC GSP_GR_PrivateGameRecord
	@dataUserDefine image						-- 游戏自定义数据
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 插入记录
	INSERT PrivateGameRecord(UserDate)
	VALUES (@dataUserDefine)
	
	-- 读取记录
	SELECT SCOPE_IDENTITY() AS RecordID

END

RETURN 0

GO
----------------------------------------------------------------------------------------------------

-- 游戏记录
CREATE PROC GSP_GR_PrivateGameRecordChild
	@dwRecordID INT,
	@dataUserDefine image						-- 游戏自定义数据
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 插入记录
	INSERT PrivateGameRecordChild(UserDate,RecordID)VALUES (@dataUserDefine,@dwRecordID)
	
END

RETURN 0

GO
----------------------------------------------------------------------------------------------------


-- 游戏记录
CREATE PROC GSP_GR_PrivateGameRecordUserRecordID
	@dwRecordID INT,
	@dwUserID INT	
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 插入记录
	INSERT PrivateGameRecordUserRecordID(RecordID,UserID)VALUES (@dwRecordID,@dwUserID)
	
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
-- 完成一局游戏
CREATE PROC GSP_GR_BeginnerPlayGame
	@dwUserID INT								-- 用户 I D
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 变量定义	
	DECLARE	@return_value int

	EXEC @return_value = QPAccountsDBLink.QPAccountsDB.dbo.GSP_GR_BeginnerPlayGame
		 @dwUserID = @dwUserID

	RETURN @return_value
END

GO