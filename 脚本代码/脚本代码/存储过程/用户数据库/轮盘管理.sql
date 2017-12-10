
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_RouletteQueryInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_RouletteQueryInfo]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_RouletteDone]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_RouletteDone]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_LoadRouletteReward]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_LoadRouletteReward]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO


----------------------------------------------------------------------------------------------------

-- 加载奖励
CREATE PROC GSP_GR_LoadRouletteReward
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查询奖励
	SELECT * FROM QPPlatformDBLink.QPPlatformDB.dbo.RouletteConfig	

END

RETURN 0

GO
----------------------------------------------------------------------------------------------------
-- 查询签到
CREATE PROC GSP_GR_RouletteQueryInfo
	@dwUserID INT,								-- 用户 I D
	@dwVaildDay INT								-- 有效时间
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查询用户
	IF not exists(SELECT * FROM AccountsInfo WHERE UserID=@dwUserID)
	BEGIN
		return 1
	END
	
	DECLARE @HaveDone int
	set @HaveDone = 1
	
	-- 记录
	DECLARE @RouletteDateTime DateTime
	SELECT @RouletteDateTime=RouletteDateTime FROM AccountsRoulette 	
	WHERE UserID=@dwUserID
		
	IF @RouletteDateTime IS NULL 
	BEGIN
		set @HaveDone = 0
	END
	
	IF DATEDIFF(dd,@RouletteDateTime,GetDate()) > 1
	BEGIN
		set @HaveDone = 0
	END
	
	DECLARE @RouletteVaildDay AS INT
	SELECT @RouletteVaildDay =StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'RouletteVaildDay'
	if @RouletteVaildDay is not NULL
	BEGIN
		set @dwVaildDay = @RouletteVaildDay
	END


	if @dwVaildDay > 0 and @dwVaildDay < 8
	BEGIN
		set datefirst 1
		DECLARE @dwWeekDat int
		select @dwWeekDat = datepart(weekday, getdate())
		
		if @dwWeekDat <> @dwVaildDay
		BEGIN
			set @HaveDone = @dwVaildDay + 10
		END
	END
	
	-- 抛出数据
	SELECT @HaveDone AS HaveDone
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 领取物品
CREATE PROC GSP_GR_RouletteDone
	@dwUserID INT,							-- 用户 I D
	@RewardGold bigint,						-- 
	@RewardType smallint					-- 
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查询用户
	IF not exists(SELECT * FROM AccountsInfo WHERE UserID=@dwUserID)
	BEGIN
		return 1
	END
	
	-- 记录
	DECLARE @HaveDone int
	DECLARE @TempRouletteDateTime DateTime
	SELECT @TempRouletteDateTime=RouletteDateTime FROM AccountsRoulette 	
	WHERE UserID=@dwUserID
	
	set @HaveDone = 1
		
	IF @TempRouletteDateTime IS NULL 
	BEGIN
		set @HaveDone = 0
	END
	
	IF DATEDIFF(dd,@TempRouletteDateTime,GetDate()) > 1
	BEGIN
		set @HaveDone = 0
	END
	
	--if @HaveDone = 1
	--BEGIN
	--	return 1
	--END

	if @RewardType = 1	--金币
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score = Score + @RewardGold WHERE UserID = @dwUserID
	END

	if @RewardType = 2	--房卡
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET InsureScore = InsureScore + @RewardGold WHERE UserID = @dwUserID
	END
	
	DECLARE @RouletteDateTime DateTime
	SELECT @RouletteDateTime=RouletteDateTime FROM AccountsRoulette 	
	WHERE UserID=@dwUserID
		
	IF @RouletteDateTime IS NULL 
	BEGIN
		INSERT INTO AccountsRoulette VALUES(@dwUserID,GetDate())		
	END
	UPDATE AccountsRoulette SET RouletteDateTime = GetDate() WHERE UserID = @dwUserID
END
RETURN 0

GO