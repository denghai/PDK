
----------------------------------------------------------------------------------------------------
-- 
----------------------------------------------------------------------------------------------------


----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO


IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_EfficacyVisitorID]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_EfficacyVisitorID]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- I D 登录
CREATE PROC GSP_GR_EfficacyVisitorID
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strMachineSerial NCHAR(32)				-- 机器标识
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 基本信息
DECLARE @strAccounts AS NVARCHAR(31)

-- 辅助变量
DECLARE @EnjoinLogon BIGINT
DECLARE @ErrorDescribe AS NVARCHAR(128)

-- 执行逻辑
BEGIN
	-- 效验地址
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SELECT [ErrorDescribe]=N'抱歉地通知您，系统禁止了您所在的 IP 地址的游戏登录权限，请联系客户服务中心了解详细情况！'
		RETURN 4
	END
	
	-- 效验机器
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineSerial AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SELECT [ErrorDescribe]=N'抱歉地通知您，系统禁止了您的机器的游戏登录权限，请联系客户服务中心了解详细情况！'
		RETURN 7
	END
	
	SELECT @strAccounts = 'V'+RIGHT(1000001+ISNULL(RIGHT(MAX(VisitorIdex),6),0),6) FROM Visitor
	
	INSERT Visitor (VisitorIdex)
	VALUES (@strAccounts)
	
	-- 输出变量
	SELECT @strAccounts AS Accounts
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------