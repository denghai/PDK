
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_BeginnerQueryInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_BeginnerQueryInfo]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_BeginnerDone]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_BeginnerDone]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_BeginnerPlayGame]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_BeginnerPlayGame]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_LoadBeginnerConfig]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_LoadBeginnerConfig]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------
-- 新手奖励
CREATE PROC GSP_GP_LoadBeginnerConfig
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查询奖励
	SELECT * FROM QPPlatformDBLink.QPPlatformDB.dbo.BeginnerConfig	

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
	UPDATE AccountsBeginner SET LastGameTime = GetDate() WHERE UserID = @dwUserID
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------
-- 查询新手活动
CREATE PROC GSP_GR_BeginnerQueryInfo
	@dwUserID INT,								-- 用户 I D
	@strPassword NCHAR(32),						-- 用户密码
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查询用户
	IF not exists(SELECT * FROM AccountsInfo WHERE UserID=@dwUserID AND LogonPass=@strPassword)
	BEGIN
		SET @strErrorDescribe = N'抱歉，你的用户信息不存在或者密码不正确！'
		return 1
	END

	-- 签到记录
	DECLARE @wSeriesDate INT	
	DECLARE @StartDateTime DateTime
	DECLARE @LastDateTime DateTime
	DECLARE @LastGameTime DateTime
	SELECT @StartDateTime=StartDateTime,@LastDateTime=LastDateTime,@wSeriesDate=SeriesDate,@LastGameTime=LastGameTime FROM AccountsBeginner 	
	WHERE UserID=@dwUserID
	IF @StartDateTime IS NULL OR @LastDateTime IS NULL OR @wSeriesDate IS NULL OR @wSeriesDate IS NULL
	BEGIN
		SELECT @StartDateTime=GetDate(),@LastDateTime=0,@wSeriesDate=0
		INSERT INTO AccountsBeginner VALUES(@dwUserID,@StartDateTime,@LastDateTime,0,0)		
	END

	-- 日期判断
	DECLARE @TodayCheckIned TINYINT
	DECLARE @LastCheckIned TINYINT
	SET @TodayCheckIned = 0
	SET @LastCheckIned = 1
	IF @LastDateTime <> 0 and  DateDiff(dd,@LastDateTime,GetDate()) = 0 	
	BEGIN
		SET @TodayCheckIned = 1
	END
	
	if @LastGameTime <> 0 and  DateDiff(dd,@LastGameTime,@LastDateTime) < 0 and DateDiff(dd,@LastGameTime,GetDate()) <> 0
	BEGIN
		SET @LastCheckIned = 0;
	END
	
	-- 抛出数据
	SELECT @wSeriesDate AS SeriesDate,@TodayCheckIned AS TodayCheckIned,@LastCheckIned AS LastCheckIned
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 领取奖励
CREATE PROC GSP_GR_BeginnerDone
	@dwUserID INT,								-- 用户 I D
	@strPassword NCHAR(32),						-- 用户密码
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查询用户
	IF not exists(SELECT * FROM AccountsInfo WHERE UserID=@dwUserID AND LogonPass=@strPassword)
	BEGIN
		SET @strErrorDescribe = N'抱歉，你的用户信息不存在或者密码不正确！'
		return 1
	END


	-- 签到记录
	DECLARE @wSeriesDate INT	
	DECLARE @StartDateTime DateTime
	DECLARE @LastDateTime DateTime
	DECLARE @LastGameTime DateTime
	SELECT @StartDateTime=StartDateTime,@LastDateTime=LastDateTime,@wSeriesDate=SeriesDate,@LastGameTime=LastGameTime FROM AccountsBeginner 	
	WHERE UserID=@dwUserID
	IF @StartDateTime IS NULL OR @LastDateTime IS NULL OR @wSeriesDate IS NULL 
	BEGIN
		SELECT @StartDateTime=GetDate(),@LastDateTime=GetDate(),@wSeriesDate=0
		INSERT INTO AccountsBeginner VALUES(@dwUserID,@StartDateTime,@LastDateTime,0,0)		
	END

	-- 今天是否已经领取
	IF DateDiff(d,@LastDateTime,GetDate()) = 0 
	BEGIN
		SET @strErrorDescribe = N'抱歉，您今天已经领取奖励了！'
		return 3		
	END

	-- 今天是否已经已经玩了游戏
	 IF @LastGameTime = 0 OR DateDiff(d,@LastDateTime,@LastGameTime) = 0 
	 BEGIN
		SET @strErrorDescribe = N'抱歉，要先玩一盘游戏才能领奖！'
		return 4		
	 END
	
	-- 查询奖励
	DECLARE @lRewardGold BIGINT
	DECLARE @lRewardType BIGINT
	SELECT @lRewardGold=RewardGold,@lRewardType=RewardType FROM QPPlatformDBLink.QPPlatformDB.dbo.BeginnerConfig WHERE DayID=(@wSeriesDate+1)
	IF @lRewardGold IS NULL 
	BEGIN
		SET @strErrorDescribe = N'数据库错误'
		return 5		
	END	
	
	-- 更新记录
	UPDATE AccountsBeginner SET LastDateTime = @LastGameTime ,SeriesDate = SeriesDate + 1 WHERE UserID = @dwUserID
	
	IF @lRewardType = 1
	BEGIN
		-- 奖励金币	
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score = Score + @lRewardGold WHERE UserID = @dwUserID
		
		SET @strErrorDescribe = N'获得新手活动奖励 '+convert(varchar,@lRewardGold)+N' 的金币！'
	END	


	-- 查询金币
	DECLARE @lScore BIGINT	
	SELECT @lScore=Score FROM QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo WHERE UserID = @dwUserID 	
	IF @lScore IS NULL SET @lScore = 0
	
		
	-- 抛出数据
	SELECT @lScore AS AwardCout,@lRewardType AS AwardType		
END
RETURN 0

GO