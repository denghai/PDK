
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_CheckInQueryInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_CheckInQueryInfo]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_CheckAward]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_CheckAward]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_CheckInDone]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_CheckInDone]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_LoadCheckInReward]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_LoadCheckInReward]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO


----------------------------------------------------------------------------------------------------

-- 加载奖励
CREATE PROC GSP_GP_LoadCheckInReward
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查询奖励
	SELECT * FROM QPPlatformDBLink.QPPlatformDB.dbo.SigninConfig	

END

RETURN 0

GO
----------------------------------------------------------------------------------------------------
-- 查询签到
CREATE PROC GSP_GR_CheckInQueryInfo
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
	DECLARE @wAwardDate INT	
	DECLARE @StartDateTime DateTime
	DECLARE @LastDateTime DateTime
	SELECT @StartDateTime=StartDateTime,@LastDateTime=LastDateTime,@wSeriesDate=SeriesDate,@wAwardDate=AwardDate FROM AccountsSignin 	
	WHERE UserID=@dwUserID
	IF @StartDateTime IS NULL OR @LastDateTime IS NULL OR @wSeriesDate IS NULL OR @wSeriesDate IS NULL OR @wAwardDate IS NULL
	BEGIN
		SELECT @StartDateTime=GetDate(),@LastDateTime=GetDate(),@wSeriesDate=0
		INSERT INTO AccountsSignin VALUES(@dwUserID,@StartDateTime,@LastDateTime,0,0)		
	END

	-- 日期判断
	DECLARE @TodayCheckIned TINYINT
	SET @TodayCheckIned = 0
	IF DateDiff(dd,@LastDateTime,GetDate()) = 0 	
	BEGIN
		IF @wSeriesDate > 0 SET @TodayCheckIned = 1
	END ELSE
	BEGIN	
		DECLARE @iMaxDay INT
		SELECT @iMaxDay =  MAX(RewardDay) FROM QPPlatformDBLink.QPPlatformDB.dbo.SigninConfig
		
		if @wAwardDate = @iMaxDay OR @wAwardDate > @iMaxDay
		BEGIN
			SELECT @StartDateTime = GetDate(),@LastDateTime = GetDate(),@wSeriesDate = 0,@wAwardDate = 0
			INSERT INTO AccountsSignin VALUES(@dwUserID,@StartDateTime,@LastDateTime,0,0)		
		END	
		--IF DateDiff(dd,@StartDateTime,GetDate()) <> @wSeriesDate
		--BEGIN
		--	SET @wSeriesDate = 0
		--	SET @wAwardDate = 0
		--	UPDATE AccountsSignin SET StartDateTime=GetDate(),LastDateTime=GetDate(),SeriesDate=0,AwardDate=0 WHERE UserID=@dwUserID									
		--END
	END

	
	
	-- 抛出数据
	SELECT @wSeriesDate AS SeriesDate,@wAwardDate AS AwardDate,@TodayCheckIned AS TodayCheckIned	
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 领取物品
CREATE PROC GSP_GR_CheckAward
	@dwUserID INT,								-- 用户 I D
	@strPassword NCHAR(32),						-- 用户密码
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strMachineID NVARCHAR(32),					-- 机器标识
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
	DECLARE @wAwardDate INT	
	DECLARE @StartDateTime DateTime
	DECLARE @LastDateTime DateTime
	SELECT @StartDateTime=StartDateTime,@LastDateTime=LastDateTime,@wSeriesDate=SeriesDate,@wAwardDate=AwardDate  FROM AccountsSignin 
	WHERE UserID=@dwUserID
	IF @StartDateTime IS NULL OR @LastDateTime IS NULL OR @wSeriesDate IS NULL
	BEGIN
		SELECT @StartDateTime = GetDate(),@LastDateTime = GetDate(),@wSeriesDate = 0,@wAwardDate = 0
		INSERT INTO AccountsSignin VALUES(@dwUserID,@StartDateTime,@LastDateTime,0,0)		
	END

	-- 签到判断
	IF @wAwardDate = @wSeriesDate AND @wAwardDate > 0
	BEGIN
		SET @strErrorDescribe = N'抱歉，您今天已经领取奖励了！'
		return 3		
	END
	
	-- 查询奖励
	DECLARE @lRewardGold BIGINT
	DECLARE @lRewardType BIGINT
	DECLARE @lRewardDay BIGINT
	SELECT @lRewardGold=RewardGold,@lRewardType=RewardType,@lRewardDay=RewardDay FROM QPPlatformDBLink.QPPlatformDB.dbo.SigninConfig WHERE DayID=(@wAwardDate+1)
	IF @lRewardGold IS NULL 
	BEGIN
		SELECT @StartDateTime = GetDate(),@LastDateTime = GetDate(),@wSeriesDate = 0,@wAwardDate = 0
		INSERT INTO AccountsSignin VALUES(@dwUserID,@StartDateTime,@LastDateTime,0,0)		
		SET @strErrorDescribe = N'抱歉，您还不能领取这个奖励！'
		return 3		
	END	
	if @lRewardDay > @wSeriesDate
	BEGIN
		SET @strErrorDescribe = N'抱歉，您还不能领取这个奖励！'
		return 3		
	END	
	
	-- 更新记录
	SET @wAwardDate = @wAwardDate+1
	UPDATE AccountsSignin SET AwardDate = @wAwardDate WHERE UserID = @dwUserID
	
	IF @lRewardType = 1
	BEGIN
		-- 奖励金币	
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score = Score + @lRewardGold WHERE UserID = @dwUserID
		IF @@rowcount = 0
		BEGIN
			-- 插入资料
			INSERT INTO QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo (UserID,Score, LastLogonIP, LastLogonMachine, RegisterIP, RegisterMachine)
			VALUES (@dwUserID, @lRewardGold, @strClientIP, @strMachineID, @strClientIP, @strMachineID)
		END
		
		SET @strErrorDescribe = N'领取成功！获得 '+convert(varchar,@lRewardGold)+N' 的金币！'
	END	


	-- 查询金币
	DECLARE @lScore BIGINT	
	SELECT @lScore=Score FROM QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo WHERE UserID = @dwUserID 	
	IF @lScore IS NULL SET @lScore = 0
	
		
	-- 抛出数据
	SELECT @lScore AS Score	
END
RETURN 0

GO
----------------------------------------------------------------------------------------------------

-- 领取奖励
CREATE PROC GSP_GR_CheckInDone
	@dwUserID INT,								-- 用户 I D
	@strPassword NCHAR(32),						-- 用户密码
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strMachineID NVARCHAR(32),					-- 机器标识
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
	SELECT @StartDateTime=StartDateTime,@LastDateTime=LastDateTime,@wSeriesDate=SeriesDate FROM AccountsSignin 
	WHERE UserID=@dwUserID
	IF @StartDateTime IS NULL OR @LastDateTime IS NULL OR @wSeriesDate IS NULL
	BEGIN
		SELECT @StartDateTime = GetDate(),@LastDateTime = GetDate(),@wSeriesDate = 0
		INSERT INTO AccountsSignin VALUES(@dwUserID,@StartDateTime,@LastDateTime,0,0)		
	END

	-- 签到判断
	IF DateDiff(dd,@LastDateTime,GetDate()) = 0 AND @wSeriesDate > 0
	BEGIN
		SET @strErrorDescribe = N'抱歉，您今天已经签到了！'
		return 3		
	END
	
	
	-- 每天奖励数
	DECLARE @iSiginDayGold AS INT
	SELECT @iSiginDayGold=StatusValue FROM SystemStatusInfo WHERE StatusName=N'SiginDayGold'
	IF @iSiginDayGold IS NULL 
	BEGIN
		SET @strErrorDescribe = N'签到为配置SiginDayGold！'
		return 4		
	END

	-- 更新记录
	SET @wSeriesDate = @wSeriesDate+1
	UPDATE AccountsSignin SET LastDateTime = GetDate(),SeriesDate = @wSeriesDate WHERE UserID = @dwUserID
	
	-- 奖励金币	
	UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score = Score + @iSiginDayGold WHERE UserID = @dwUserID
	IF @@rowcount = 0
	BEGIN
		-- 插入资料
		INSERT INTO QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo (UserID,Score, LastLogonIP, LastLogonMachine, RegisterIP, RegisterMachine)
		VALUES (@dwUserID, @iSiginDayGold, @strClientIP, @strMachineID, @strClientIP, @strMachineID)
	END

	SET @strErrorDescribe = N'签到成功!获得 '+convert(varchar,@iSiginDayGold)+N' 的金币！'

	-- 查询金币
	DECLARE @lScore BIGINT	
	SELECT @lScore=Score FROM QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo WHERE UserID = @dwUserID 	
	IF @lScore IS NULL SET @lScore = 0
	
	-- 抛出数据
	SELECT @lScore AS Score	
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------