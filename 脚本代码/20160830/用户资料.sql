
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_GetShopInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_GetShopInfo]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_BuyOder]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_BuyOder]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_SellItem]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_SellItem]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_GetSellRecordList]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_GetSellRecordList]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- 查询资料
CREATE PROC GSP_GP_GetShopInfo
	
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

SELECT * FROM ShopInfoList
		
END

RETURN 0

GO
----------------------------------------------------------------------------------------------------

-- 查询资料
CREATE PROC GSP_GP_BuyOder
	@dwUserID INT,							-- 用户帐号
	@dwItemID INT,							-- 商品id
	@total_fee INT,							-- 金额
	@appid NVARCHAR(32),					-- 微信id
	@mch_id NVARCHAR(32),					-- 商户号
	@prepay_id NVARCHAR(128),					-- 准备订单号
	@strOSType NVARCHAR(32),					-- 系统号
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
	
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	DECLARE @ItemType 	INT
	DECLARE @Price 		INT
	DECLARE @GoodsNum 	INT
	SELECT @ItemType=ItemType, @GoodsNum=GoodsNum, @Price=Price
	FROM ShopInfoList WHERE ItemID=@dwItemID
	
	if @ItemType is NULL
	BEGIN
		set @strErrorDescribe = N'未找到商品'
		RETURN 1
	END
	
	DECLARE @LastRecordID INT
	SELECT @LastRecordID=RecordID
	FROM QPRecordDBLink.QPRecordDB.dbo.RecordBuyOder WHERE OderID=@prepay_id
	if @LastRecordID is not NULL
	BEGIN
		set @strErrorDescribe = N'未知错误'
		RETURN 2
	END
	
	if @ItemType = 0
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score=Score+@GoodsNum WHERE UserID=@dwUserID
	END
	
	if @ItemType = 1
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@GoodsNum WHERE UserID=@dwUserID
	END
	
	
	-- 查询用户
	DECLARE @NickName NCHAR(32)
	DECLARE	@SpreaderID INT
	SELECT @NickName=NickName, @SpreaderID=SpreaderID
	FROM AccountsInfo WHERE UserID=@dwUserID
	
	INSERT INTO QPRecordDBLink.QPRecordDB.dbo.RecordBuyOder(UserID,OderType,UserName,OderID,ShopID,CostMoney,GetNums,GetType,SpreaderID)
	VALUES(@dwUserID,N'',@NickName,@prepay_id,@dwItemID,@Price,@GoodsNum,@ItemType,@SpreaderID)
	
	UPDATE QPAccountsDBLink.QPAccountsDB.dbo.AccountsBack SET money=money+@Price WHERE userId=@SpreaderID
	
	set @strErrorDescribe = N'充值成功'
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 查询资料
CREATE PROC GSP_GP_SellItem
	@dwSrcUserID INT,							-- 用户帐号
	@strSrcPassword NVARCHAR(32),				
	@dwDestUserID INT,						
	@dwItemType INT,					
	@dwItemValue INT,				
	@strErrorDescribe NVARCHAR(127) OUTPUT		
	
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	DECLARE @UserID INT
	DECLARE @SrcScore INT
	DECLARE @SrcInsureScore INT
	DECLARE @LogonPass AS NCHAR(32)
	DECLARE @SrcNickName AS NCHAR(32)
	DECLARE @DestNickName AS NCHAR(32)
	-- 查询用户
	SELECT @UserID=UserID, @LogonPass=LogonPass,@SrcNickName=NickName
	FROM AccountsInfo(NOLOCK) WHERE UserID=@dwSrcUserID
	
	-- 查询用户
	SELECT @SrcScore=Score, @SrcInsureScore=InsureScore
	FROM  QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo WHERE UserID=@dwSrcUserID

	-- 查询用户
	IF @UserID IS NULL 
	BEGIN
		SET @strErrorDescribe=N'您的帐号不存在或者密码输入有误，请查证后再次尝试！'
		RETURN 1
	END	
	-- 查询用户
	IF @SrcScore IS NULL 
	BEGIN
		SET @strErrorDescribe=N'您的点数不足！'
		RETURN 1
	END	
	
	-- 密码判断
	IF @LogonPass<>@strSrcPassword
	BEGIN
		SET @strErrorDescribe=N'您的帐号不存在或者密码输入有误，请查证后再次尝试！'
		RETURN 3
	END
	
	-- 查询用户
	SELECT @UserID=UserID,@DestNickName=NickName
	FROM AccountsInfo(NOLOCK) WHERE UserID=@dwDestUserID
	-- 查询用户
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'你输入的用户不存在！'
		RETURN 1
	END	
	
	if @dwItemType = 0 and @SrcScore < @dwItemValue
	BEGIN
		SET @strErrorDescribe=N'您的金币不够！'
		RETURN 3
	End
	
	if @dwItemType = 1 and @SrcInsureScore < @dwItemValue
	BEGIN
		SET @strErrorDescribe=N'您的房卡不够！'
		RETURN 3
	End
	
	if @dwItemType = 0
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score=Score-@dwItemValue WHERE UserID=@dwSrcUserID
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score=Score+@dwItemValue WHERE UserID=@dwDestUserID
		IF @@ROWCOUNT=0
		BEGIN
			INSERT QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo (UserID,Score) 
			VALUES(@dwDestUserID,@dwItemValue)
		END
	END
	
	if @dwItemType = 1
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore-@dwItemValue WHERE UserID=@dwSrcUserID
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET InsureScore=InsureScore+@dwItemValue WHERE UserID=@dwDestUserID
		IF @@ROWCOUNT=0
		BEGIN
			INSERT QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo (UserID,InsureScore) 
			VALUES(@dwDestUserID,@dwItemValue)
		END
	END
	
	INSERT INTO QPRecordDBLink.QPRecordDB.dbo.UserSellOder(SrcUserID,SrcUserName,DestUserID,DestUserName,ItemType,ItemValue)
	VALUES(@dwSrcUserID,@SrcNickName,@dwDestUserID,@DestNickName,@dwItemType,@dwItemValue)
	
	set @strErrorDescribe = N'充值成功'
END

RETURN 0

GO