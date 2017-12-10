
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_QueryUserIndividual]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_QueryUserIndividual]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_QueryUserAccountInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_QueryUserAccountInfo]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ModifyUserIndividual]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_ModifyUserIndividual]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_QueryUserInGameServerID]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_QueryUserInGameServerID]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ModifyUserStarValue]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_ModifyUserStarValue]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO


----------------------------------------------------------------------------------------------------

-- 查询资料
CREATE PROC GSP_GP_QueryUserAccountInfo
	@dwUserID INT,								-- 用户 I D
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 基本信息
DECLARE @UserID INT
DECLARE @CustomID INT
DECLARE @FaceID SMALLINT
DECLARE @Accounts NVARCHAR(31)
DECLARE @NickName NVARCHAR(31)
DECLARE @UnderWrite NVARCHAR(63)
DECLARE @SpreaderID INT
DECLARE @PlayTimeCount INT

-- 积分变量
DECLARE @Score BIGINT
DECLARE @Insure BIGINT

-- 扩展信息
DECLARE @GameID INT
DECLARE @Gender TINYINT
DECLARE @UserMedal INT
DECLARE @Experience INT
DECLARE @LoveLiness INT
DECLARE @MemberOrder SMALLINT
DECLARE @MemberOverDate DATETIME
DECLARE @MemberSwitchDate DATETIME
-- 执行逻辑
BEGIN
	-- 查询用户
	DECLARE @Nullity TINYINT
	DECLARE @StunDown TINYINT
	DECLARE @LogonPass AS NCHAR(32)
	DECLARE @strInsurePass AS NCHAR(32)
	DECLARE @strLogonIP AS NVARCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	SELECT @UserID=UserID, @GameID=GameID, @Accounts=Accounts, @NickName=NickName, @UnderWrite=UnderWrite, @LogonPass=LogonPass,@strInsurePass=InsurePass,
		@FaceID=FaceID, @CustomID=CustomID, @Gender=Gender, @Nullity=Nullity, @StunDown=StunDown, @UserMedal=UserMedal, @Experience=Experience,
		@LoveLiness=LoveLiness, @MemberOrder=MemberOrder, @MemberOverDate=MemberOverDate, @MemberSwitchDate=MemberSwitchDate,
		@MoorMachine=MoorMachine,@SpreaderID=SpreaderID,@PlayTimeCount=PlayTimeCount,@strLogonIP=LastLogonIP
	FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID
	
	-- 查询用户
	IF @UserID IS NULL
	BEGIN
		set @strErrorDescribe = N'未找到用户'
		return 1
	END
	
	SELECT @Score=Score, @Insure=InsureScore FROM QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo WHERE UserID=@UserID
	IF @Score IS NULL
	BEGIN
		set @Score = 0
		set @Insure = 0
		return 1
	END
	
	SELECT @UserID AS UserID, @GameID AS GameID, @Accounts AS Accounts, @NickName AS NickName, @UnderWrite AS UnderWrite,
		@FaceID AS FaceID, @CustomID AS CustomID, @Gender AS Gender, @UserMedal AS UserMedal, @Experience AS Experience,
		@Score AS Score, @Insure AS Insure, @LoveLiness AS LoveLiness, @MemberOrder AS MemberOrder, @MemberOverDate AS MemberOverDate,
		@MoorMachine AS MoorMachine,@SpreaderID AS SpreaderID,@strLogonIP AS LogonIP

END

RETURN 0

GO
----------------------------------------------------------------------------------------------------

-- 查询资料
CREATE PROC GSP_GP_QueryUserIndividual
	@dwUserID INT,								-- 用户 I D
	@strClientIP NVARCHAR(15)					-- 连接地址
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN
	-- 变量定义
	DECLARE @UserID INT
	DECLARE @QQ NVARCHAR(16)
	DECLARE @EMail NVARCHAR(33)
	DECLARE @UserNote NVARCHAR(256)
	DECLARE @SeatPhone NVARCHAR(32)
	DECLARE @MobilePhone NVARCHAR(16)
	DECLARE @Compellation NVARCHAR(16)
	DECLARE @DwellingPlace NVARCHAR(128)
	DECLARE @HeadHttp NVARCHAR(256)
	DECLARE @UserChannel NVARCHAR(32)
	DECLARE @UserIP NVARCHAR(32)
	DECLARE @UserGps NVARCHAR(32)
	DECLARE @UserStarValue INT
	DECLARE @UserStarCout INT

	-- 查询用户
	SELECT @UserID=UserID, @QQ=QQ, @EMail=EMail, @UserNote=UserNote, @SeatPhone=SeatPhone,
		@MobilePhone=MobilePhone, @Compellation=Compellation, @DwellingPlace=DwellingPlace, @HeadHttp=HeadHttp, @UserChannel=UserChannel,
		@UserGps=UserGps,@UserStarValue=UserStarValue, @UserStarCout=UserStarCout
	FROM IndividualDatum(NOLOCK) WHERE UserID=@dwUserID
	
	-- 查询用户
	SELECT @UserIP=LastLogonIP
	FROM AccountsInfo WHERE UserID=@dwUserID

	-- 查询用户
	IF @UserID IS NULL
	BEGIN
		SET @QQ=N''	
		SET @EMail=N''	
		SET @UserNote=N''	
		SET @SeatPhone=N''	
		SET @MobilePhone=N''	
		SET @Compellation=N''	
		SET @DwellingPlace=N''	
		SET @HeadHttp=N''	
		SET @UserChannel=N''	
		SET @UserIP=N''	
		SET @UserGps=N''	
		SET @UserStarValue=0	
		SET @UserStarCout=0	
	END

	-- 输出信息
	SELECT @dwUserID AS UserID, @Compellation AS Compellation, @QQ AS QQ, @EMail AS EMail, @SeatPhone AS SeatPhone,
		@MobilePhone AS MobilePhone, @DwellingPlace AS DwellingPlace, @UserNote AS UserNote, @HeadHttp AS HeadHttp, @UserChannel AS UserChannel, @UserIP AS UserIP, @UserGps AS UserGps, @UserStarValue AS UserStarValue, @UserStarCout AS UserStarCout

	RETURN 0

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 更新资料
CREATE PROC GSP_GP_ModifyUserIndividual
	@dwUserID INT,								-- 用户 I D
	@strPassword NCHAR(32),						-- 用户密码
	@cbGender TINYINT,							-- 用户性别
	@strNickName NVARCHAR(32),					-- 用户昵称
	@strUnderWrite NVARCHAR(63),				-- 个性签名
	@strCompellation NVARCHAR(16),				-- 真实名字
	@strQQ NVARCHAR(16),						-- Q Q 号码
	@strEMail NVARCHAR(33),						-- 电子邮电
	@strSeatPhone NVARCHAR(32),					-- 固定电话
	@strMobilePhone NVARCHAR(16),				-- 移动电话
	@strDwellingPlace NVARCHAR(128),			-- 详细地址
	@strUserNote NVARCHAR(256),					-- 用户备注		
	@strHeadHttp NVARCHAR(256),					-- 头像HTTP		
	@strUserChannel NVARCHAR(32),				-- 渠道号			
	@strUserGps NVARCHAR(32),					-- GPS	
	@strClientIP NVARCHAR(15),					-- 连接地址		
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 变量定义
	DECLARE @UserID INT
	DECLARE @NickName NVARCHAR(31)
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE @LogonPass AS NCHAR(32)

	-- 查询用户
	SELECT @UserID=UserID, @NickName=NickName, @LogonPass=LogonPass, @Nullity=Nullity, @StunDown=StunDown
	FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID

	-- 查询用户
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'您的帐号不存在或者密码输入有误，请查证后再次尝试！'
		RETURN 1
	END	

	-- 帐号禁止
	IF @Nullity<>0
	BEGIN
		SET @strErrorDescribe=N'您的帐号暂时处于冻结状态，请联系客户服务中心了解详细情况！'
		RETURN 2
	END	

	-- 帐号关闭
	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'您的帐号使用了安全关闭功能，必须重新开通后才能继续使用！'
		RETURN 2
	END	
	
	-- 密码判断
	IF @LogonPass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'您的帐号不存在或者密码输入有误，请查证后再次尝试登录！'
		RETURN 3
	END

	-- 效验昵称
	IF (SELECT COUNT(*) FROM ConfineContent(NOLOCK) WHERE CHARINDEX(String,@strNickName)>0)>0
	BEGIN
		SET @strErrorDescribe=N'您所输入的游戏昵称名含有限制字符串，请更换昵称名后再次修改！'
		RETURN 4
	END

	-- 存在判断
	-- IF EXISTS (SELECT UserID FROM AccountsInfo(NOLOCK) WHERE NickName=@strNickName AND UserID<>@dwUserID)
	-- BEGIN
	-- 	SET @strErrorDescribe=N'此昵称已被其他玩家使用了，请更换昵称名后再次修改！'
	-- 	RETURN 4
	-- END
	IF @strNickName = N''
	BEGIN
		SET @strNickName = @NickName
	END
	

	-- 修改资料
	-- UPDATE AccountsInfo SET NickName=@strNickName, UnderWrite=@strUnderWrite, Gender=@cbGender WHERE UserID=@dwUserID
	-- 修改资料
	UPDATE AccountsInfo SET NickName=@strNickName,Gender=@cbGender WHERE UserID=@dwUserID
	
	-- 修改昵称记录
	IF @NickName<>@strNickName
	BEGIN
		INSERT INTO QPRecordDBLink.QPRecordDB.dbo.RecordAccountsExpend(UserID,ReAccounts,ClientIP)
		VALUES(@dwUserID,@strNickName,@strClientIP)
	END

	IF @strNickName = N''
	BEGIN
		SET @strNickName = @NickName
	END
	
	-- 变量定义
	DECLARE @UserChannel AS NVARCHAR(32)
	DECLARE @UserGps AS NVARCHAR(32)
	DECLARE @HeadHttp AS NVARCHAR(256)
	DECLARE @UserStarValue AS INT
	DECLARE @UserStarCout AS INT
	SELECT @UserChannel=UserChannel, @HeadHttp=HeadHttp,@UserGps=UserGps,@UserStarValue=UserStarValue,@UserStarCout=UserStarCout
	FROM IndividualDatum(NOLOCK) WHERE UserID=@dwUserID
	
	-- 修改资料
	IF @UserChannel IS NULL
	BEGIN
		set @UserChannel =  N''
		set @HeadHttp =  N''
		set @UserGps =  N''
		INSERT IndividualDatum (UserID, Compellation, QQ, EMail, SeatPhone, MobilePhone, DwellingPlace, HeadHttp,UserChannel,UserGps,UserStarValue,UserStarCout)
		VALUES (@dwUserID, @strCompellation, @strQQ, @strEMail, @strSeatPhone, @strMobilePhone, @strDwellingPlace, @strHeadHttp, @strUserChannel,@strUserGps,0,0)
	END

	IF @strNickName = N''
	BEGIN
		SET @strNickName = @NickName
	END

	IF @strHeadHttp = N''
	BEGIN
		SET @strHeadHttp = @HeadHttp
	END
	
	IF @strUserChannel = N''
	BEGIN
		SET @strUserChannel = @UserChannel
	END
	IF @strUserGps = N''
	BEGIN
		SET @strUserGps = @UserGps
	END
	-- UPDATE IndividualDatum SET Compellation=@strCompellation, QQ=@strQQ, EMail=@strEMail, SeatPhone=@strSeatPhone,
	--	MobilePhone=@strMobilePhone, DwellingPlace=@strDwellingPlace, UserNote=@strUserNote WHERE UserID=@dwUserID
	-- 修改资料
	UPDATE IndividualDatum SET HeadHttp=@strHeadHttp,UserChannel=@strUserChannel,UserGps=@strUserGps WHERE UserID=@dwUserID
		

	-- 设置信息
	IF @@ERROR=0 SET @strErrorDescribe=N'服务器已经接受了您的新资料！'

	RETURN 0

END

RETURN 0

GO


----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------

-- 更新资料
CREATE PROC GSP_GP_ModifyUserStarValue
	@dwUserID INT,								-- 用户 I D
	@dwStarValue INT,							-- 评分	
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN
	DECLARE @UserStarValue AS INT
	DECLARE @UserStarCout AS INT
	SELECT @UserStarValue=UserStarValue,@UserStarCout=UserStarCout
	FROM IndividualDatum(NOLOCK) WHERE UserID=@dwUserID
	IF @dwStarValue <> 0
	BEGIN
		SET @UserStarValue = @UserStarValue + @dwStarValue
		SET @UserStarCout = @UserStarCout + 1
	END  
	UPDATE IndividualDatum SET  UserStarValue=@UserStarValue,UserStarCout=@UserStarCout  WHERE UserID=@dwUserID
	IF @@ERROR=0 SET @strErrorDescribe=N'评分成功'
END

RETURN 0

GO
----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------

-- 更新资料
CREATE PROC GSP_GP_QueryUserInGameServerID
	@dwUserID INT								-- 用户 I D
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN
	DECLARE @LockKindID1 INT
	DECLARE @LockServerID1 INT
	SELECT @LockKindID1=KindID, @LockServerID1=ServerID FROM QPTreasureDBLink.QPGameMatchDB.dbo.GameScoreLocker WHERE UserID=@dwUserID
	
	DECLARE @LockKindID2 INT
	DECLARE @LockServerID2 INT
	SELECT @LockKindID2=KindID, @LockServerID2=ServerID FROM QPTreasureDBLink.QPTreasureDB.dbo.GameScoreLocker WHERE UserID=@dwUserID
	
	IF @LockKindID1 IS NOT NULL
	BEGIN
		SELECT @LockKindID1 AS LockKindID, @LockServerID1 AS LockServerID
		RETURN 0
	END
	IF @LockKindID2 IS NOT NULL
	BEGIN
		SELECT @LockKindID2 AS LockKindID, @LockServerID2 AS LockServerID
		RETURN 0
	END
	
	SELECT 0 AS LockKindID, 0 AS LockServerID
	RETURN 0

END

RETURN 0

GO
