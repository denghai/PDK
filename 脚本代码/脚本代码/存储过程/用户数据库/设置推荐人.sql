
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ModifySpreader]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_ModifySpreader]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- 查询资料
CREATE PROC GSP_GP_ModifySpreader
	@dwUserID INT,								-- 用户 I D
	@strPassword NCHAR(32),						-- 用户密码
	@dwSpreader INT,					-- 推荐人
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 变量定义
	DECLARE @LogonPass AS NCHAR(32)
	DECLARE @SpreaderID AS NCHAR(32)

	-- 查询用户
	SELECT @SpreaderID=SpreaderID ,@LogonPass=LogonPass FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID

	
	DECLARE @SpreaderType AS INT
	SELECT @SpreaderType=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'SpreaderType'
	IF @SpreaderType IS NULL
	BEGIN
		set @SpreaderType = 0
	END
	
	
	IF @SpreaderType = 1
	BEGIN
		-- 密码判断
		IF @LogonPass<>@strPassword
		BEGIN
			SET @strErrorDescribe=N'您的用户密码不正确，个人信息查询失败！'
			RETURN 1
		END
		-- 推荐人
		IF @SpreaderID<>0
		BEGIN
			SET @strErrorDescribe=N'您已经设置了推荐人！'
			RETURN 3
		END
		
		-- 查询用户
		IF not exists(SELECT * FROM AccountsInfo WHERE UserID=@dwSpreader)
		BEGIN
			SET @strErrorDescribe=N'推荐人ID错误。无此推荐人'
			RETURN 4
		END
		
		-- 查询用户
		IF not exists(SELECT * FROM AccountsBack WHERE UserID=@dwSpreader)
		BEGIN
			SET @strErrorDescribe=N'推荐人ID错误。无此推荐人'
			RETURN 4
		END
	END
	
	
	UPDATE AccountsInfo SET SpreaderID=@dwSpreader
	WHERE UserID=@dwUserID
	
	SET @strErrorDescribe=N'设置推荐人成功!'
	
	SELECT @dwSpreader AS SpreaderID
		
END

RETURN 0

GO
