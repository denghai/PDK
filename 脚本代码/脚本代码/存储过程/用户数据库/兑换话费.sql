
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_GetExchangeHuaFei]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_GetExchangeHuaFei
GO


IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ExchangeDone]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_ExchangeDone
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- 查询资料
CREATE PROC GSP_GP_GetExchangeHuaFei
	
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

SELECT * FROM ExchangeHuaFeiList
		
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
-- 执行兑换
CREATE PROC GSP_GP_ExchangeDone
	@dwUserID 		INT,								-- 用户 I D
	@dwExchangeID 	INT,								-- 兑换 I D
	@strPassword	NCHAR(32),				-- 输出信息
	@strErrorDescribe NVARCHAR(127) OUTPUT				-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查询用户
	DECLARE @LogonPassword NCHAR(32) 
	SELECT @LogonPassword=LogonPass FROM AccountsInfo WHERE UserID=@dwUserID
	IF @LogonPassword IS NULL
	BEGIN
		SET @strErrorDescribe = N'您的用户信息不存在,请与我们的客服人员联系！'
		return 1				
	END

	-- 校验密码
	IF @LogonPassword <> @strPassword
	BEGIN
		SET @strErrorDescribe = N'您的登录密码不匹配！'
		return 2		
	END
	
	DECLARE @UseType int	--1金币 2砖石 3话费卡
	DECLARE @UseNum int
	DECLARE @GetType int	
	DECLARE @GetNum int
	
	SELECT @UseType = UseType,@UseNum = UseNum,@GetType = GetType,@GetNum = GetNum FROM ExchangeHuaFeiList WHERE ExchangeID=@dwExchangeID
	
	if @UseType is NULL
	BEGIN
		set @strErrorDescribe = N'没有找到该物品'
		return 1
	END

	DECLARE @InsureScore bigint
	DECLARE @Score bigint
	SELECT @Score = Score,@InsureScore = InsureScore FROM QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo WHERE UserID=@dwUserID
	
	if @Score is NULL
	BEGIN
		set @strErrorDescribe = N'没有找到玩家'
		return 1
	END
	
	if @UseType = 1
	BEGIN
		if @Score < @UseNum
		BEGIN
			set @strErrorDescribe = N'你的金币不够'
			return 1
		END
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score = Score - @UseNum WHERE UserID = @dwUserID
	END
	
	if @UseType = 2
	BEGIN
		if @InsureScore < @UseNum
		BEGIN
			set @strErrorDescribe = N'你的钻石不够'
			return 1
		END
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET InsureScore = InsureScore - @UseNum WHERE UserID = @dwUserID
	END
	
	
	if @GetType = 1
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score = Score + @GetNum WHERE UserID = @dwUserID
	END
	
	if @GetType = 2
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET InsureScore = InsureScore + @GetNum WHERE UserID = @dwUserID
	END
	
	set @strErrorDescribe = N'兑换成功'
	RETURN 0
END

RETURN 0

GO