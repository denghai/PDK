----------------------------------------------------------------------
-- 版权：2011
-- 时间：2011-09-1
-- 用途：在线订单
----------------------------------------------------------------------

USE [QPTreasureDB]
GO

-- 每日签到
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].NET_PW_WriteCheckIn') AND OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].NET_PW_WriteCheckIn
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

----------------------------------------------------------------------------------
-- 每日签到
CREATE PROCEDURE NET_PW_WriteCheckIn
	@dwUserID			INT,					-- 用户标识	
	@strClientIP		NVARCHAR(15),			-- 签到地址
	@strErrorDescribe	NVARCHAR(127) OUTPUT	-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 帐号资料
DECLARE @UserID INT
DECLARE @Nullity TINYINT
DECLARE @StunDown TINYINT

-- 记录信息
DECLARE @RecordID INT

-- 执行逻辑
BEGIN
	-- 验证用户
	SELECT @UserID=UserID,@Nullity=Nullity,@StunDown=StunDown
	FROM QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo
	WHERE UserID=@dwUserID

	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'抱歉！您要签到的用户账号不存在。'
		RETURN 1
	END

	IF @Nullity=1
	BEGIN
		SET @strErrorDescribe=N'抱歉！您要签到的用户账号暂时处于冻结状态，请联系客户服务中心了解详细情况。'
		RETURN 2
	END

	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'抱歉！您要签到的用户账号使用了安全关闭功能，必须重新开通后才能继续使用。'
		RETURN 3
	END

	-- 记录查询
	SELECT @RecordID=RecordID FROM RecordCheckIn WHERE UserID=@dwUserID AND DATEDIFF(D,CollectDate,GETDATE())=0 
	IF @RecordID IS NOT NULL
	BEGIN
		SET @strErrorDescribe=N'抱歉！您今天已签到,请勿重复操作。'
		RETURN 4
	END

	-- 签到逻辑
	DECLARE @dwPresentGold	INT  -- 本次领取金币
	DECLARE @dwPresentCount	INT  -- 本次领取次数
	DECLARE @dwLxCount		INT  -- 本次连续次数
	DECLARE @dwLxGold		INT  -- 本次连续金币		
	DECLARE @dwLastLxCount	INT  -- 上次连续次数
	DECLARE @dwLastLxGold	INT  -- 上次连续金币

	SET @dwPresentCount=1

	SELECT @dwLastLxCount=LxCount,@dwLastLxGold=LxGold FROM RecordCheckIn WHERE UserID=@dwUserID AND DATEDIFF(D,CollectDate,GETDATE())=1
	IF @dwLastLxCount IS NULL
	BEGIN
		SELECT @dwPresentGold=PresentGold FROM GlobalCheckIn WHERE ID=1
		SET @dwLxCount=1
		SET @dwLxGold=@dwPresentGold
	END
	ELSE
	BEGIN
		IF @dwLastLxCount=7
		BEGIN			
			SET @dwLxCount=1
			SELECT @dwPresentGold=PresentGold FROM GlobalCheckIn WHERE ID=@dwLxCount
			SET @dwLxGold=@dwPresentGold
		END
		ELSE
		BEGIN			
			SET @dwLxCount=@dwLastLxCount+1
			SELECT @dwPresentGold=PresentGold FROM GlobalCheckIn WHERE ID=@dwLxCount
			SET @dwLxGold=@dwLastLxGold+@dwPresentGold
		END
	END

	-- 签到记录
	INSERT INTO RecordCheckIn(UserID,PresentGold,PresentCount,LxCount,LxGold)
	VALUES(@dwUserID,@dwPresentGold,@dwPresentCount,@dwLxCount,@dwLxGold) 

	-- 更新金币
	UPDATE GameScoreInfo SET Score=Score+@dwPresentGold WHERE UserID=@dwUserID
	IF @@ROWCOUNT=0
	BEGIN
		INSERT GameScoreInfo(UserID,Score,RegisterIP,LastLogonIP)
		VALUES (@dwUserID,@dwPresentGold,@strClientIP,@strClientIP)
	END
	
END
RETURN 0
GO
