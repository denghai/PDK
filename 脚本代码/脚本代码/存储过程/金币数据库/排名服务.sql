----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_AddPointConfig]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_AddPointConfig]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_UserAddPoint]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_UserAddPoint]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_UserGetAddRank]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_UserGetAddRank]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_UpAddRankAward]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_UpAddRankAward]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON
GO


-- 增加积分点数
CREATE PROC GSP_GR_AddPointConfig
	@iRankIdex INT								-- 金币数目
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN
	
	IF @iRankIdex = 0
	BEGIN
		SELECT * FROM QPPlatformDBLink.QPPlatformDB.dbo.WeakAddRankConfig	
	END
	
	IF @iRankIdex = 1
	BEGIN
		SELECT * FROM QPPlatformDBLink.QPPlatformDB.dbo.QuarterAddRankConfig	
	END
	
	IF @iRankIdex = 2
	BEGIN
		SELECT * FROM QPPlatformDBLink.QPPlatformDB.dbo.YearAddRankConfig	
	END
	
	RETURN 0
END

GO


-- 增加积分点数
CREATE PROC GSP_GR_UserAddPoint
	@dwUserID INT,								-- 用户 I D
	@iAddPoint INT								-- 金币数目
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN
	
	IF NOT EXISTS (SELECT UserID FROM AccountsWeakAddRank(NOLOCK) WHERE UserID=@dwUserID)
	BEGIN
		INSERT INTO [dbo].[AccountsWeakRank] ([UserID], [LastLogonTime],[UserPoint]) VALUES (@dwUserID, GETDATE(),0)
	END
	
	IF NOT EXISTS (SELECT UserID FROM AccountsQuarterAddRank(NOLOCK) WHERE UserID=@dwUserID)
	BEGIN
		INSERT INTO [dbo].[AccountsMonthRank] ([UserID], [LastLogonTime],[UserPoint]) VALUES (@dwUserID, GETDATE(),0)
	END
	
	IF NOT EXISTS (SELECT UserID FROM AccountsYearAddRank(NOLOCK) WHERE UserID=@dwUserID)
	BEGIN
		INSERT INTO [dbo].[AccountsYearRank] ([UserID], [LastLogonTime],[UserPoint]) VALUES (@dwUserID, GETDATE(),0)
	END
	
	-- 更新数据
	UPDATE AccountsWeakAddRank SET UserPoint=UserPoint+@iAddPoint	
	WHERE UserID=@dwUserID
	
	-- 更新数据
	UPDATE AccountsQuarterAddRank SET UserPoint=UserPoint+@iAddPoint	
	WHERE UserID=@dwUserID
	
	-- 更新数据
	UPDATE AccountsYearAddRank SET UserPoint=UserPoint+@iAddPoint	
	WHERE UserID=@dwUserID
	
	RETURN 0
END

GO


-- 获得排行
CREATE PROC GSP_GR_UserGetAddRank
	@dwUserID INT,								-- 用户 I D
	@strPassword NCHAR(32),						-- 用户密码
	@iRankIdex INT,								-- 排行索引 1 周排行 2 季度排行 3 年排行
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN
	-- 查询用户
	IF not exists(SELECT * FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID AND LogonPass=@strPassword)
	BEGIN
		SET @strErrorDescribe = N'抱歉，你的用户信息不存在或者密码不正确！'
		return 1
	END
	DECLARE @userID AS INT
	DECLARE @UserPoint AS BIGINT
	DECLARE @FaceID AS INT
	DECLARE @CustomID AS INT
	DECLARE @haveResoult AS INT
	DECLARE @NickName NVARCHAR(32) 
	
	DECLARE @tempRankBack TABLE
	(
      UserID INT,
      NickName NVARCHAR(32) ,
      UserPoint BIGINT,
      FaceID INT,
      CustomID INT
	);
	
	SET @haveResoult = 2;
	
	
	IF @iRankIdex = 0
	BEGIN
		-- 声明游标
		DECLARE C_Employees CURSOR FAST_FORWARD FOR
			SELECT  TOP 50 UserID,UserPoint 
			FROM AccountsWeakAddRank
			ORDER BY UserPoint DESC
			
		OPEN C_Employees;

		-- 取第一条记录
		FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint;

		WHILE @@FETCH_STATUS=0
		BEGIN
			
			SELECT @NickName=NickName,@FaceID=FaceID,@CustomID=CustomID FROM AccountsInfo(NOLOCK) WHERE UserID=@UserID
			
			IF @NickName IS NOT NULL
			BEGIN
				INSERT INTO @tempRankBack ([UserID],[NickName], [UserPoint],[FaceID],[CustomID]) VALUES (@UserID,@NickName,@UserPoint,@FaceID,@CustomID)
			END
			
			-- 取下一条记录
			FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint
		END

		-- 关闭游标
		CLOSE C_Employees;

		-- 释放游标
		DEALLOCATE C_Employees;
		
		SELECT * FROM @tempRankBack
		
		return 0
	END
	
	IF @iRankIdex = 1
	BEGIN
	
		-- 声明游标
		DECLARE C_Employees CURSOR FAST_FORWARD FOR
			SELECT  TOP 50 UserID,UserPoint 
			FROM AccountsQuarterAddRank
			ORDER BY UserPoint DESC
			
		OPEN C_Employees;

		-- 取第一条记录
		FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint;

		WHILE @@FETCH_STATUS=0
		BEGIN
			
			SELECT @NickName=NickName,@FaceID=FaceID,@CustomID=CustomID FROM AccountsInfo(NOLOCK) WHERE UserID=@UserID
			
			IF @NickName IS NOT NULL
			BEGIN
				INSERT INTO @tempRankBack ([UserID],[NickName], [UserPoint],[FaceID],[CustomID]) VALUES (@UserID,@NickName,@UserPoint,@FaceID,@CustomID)
			END
			
			-- 取下一条记录
			FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint
		END

		-- 关闭游标
		CLOSE C_Employees;

		-- 释放游标
		DEALLOCATE C_Employees;
		
		SELECT * FROM @tempRankBack
		
		return 0
	END
	
	IF @iRankIdex = 2
	BEGIN
		-- 声明游标
		DECLARE C_Employees CURSOR FAST_FORWARD FOR
			SELECT  TOP 50 UserID,UserPoint 
			FROM AccountsYearAddRank
			ORDER BY UserPoint DESC
			
		OPEN C_Employees;

		-- 取第一条记录
		FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint;

		WHILE @@FETCH_STATUS=0
		BEGIN
			
			SELECT @NickName=NickName,@FaceID=FaceID,@CustomID=CustomID FROM AccountsInfo(NOLOCK) WHERE UserID=@UserID
			
			IF @NickName IS NOT NULL
			BEGIN
				INSERT INTO @tempRankBack ([UserID],[NickName], [UserPoint],[FaceID],[CustomID]) VALUES (@UserID,@NickName,@UserPoint,@FaceID,@CustomID)
			END
			
			-- 取下一条记录
			FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint
		END

		-- 关闭游标
		CLOSE C_Employees;

		-- 释放游标
		DEALLOCATE C_Employees;
		
		SELECT * FROM @tempRankBack
		
		return 0
	END

	
	IF @iRankIdex = 3
	BEGIN
		-- 声明游标
		DECLARE C_Employees CURSOR FAST_FORWARD FOR
			SELECT  TOP 50 UserID,Score 
			FROM QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo
			ORDER BY Score DESC
			
		OPEN C_Employees;

		-- 取第一条记录
		FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint;

		WHILE @@FETCH_STATUS=0
		BEGIN
			
			SELECT @NickName=NickName,@FaceID=FaceID,@CustomID=CustomID FROM AccountsInfo(NOLOCK) WHERE UserID=@UserID
			
			IF @NickName IS NOT NULL
			BEGIN
				INSERT INTO @tempRankBack ([UserID],[NickName], [UserPoint],[FaceID],[CustomID]) VALUES (@UserID,@NickName,@UserPoint,@FaceID,@CustomID)
			END
			
			-- 取下一条记录
			FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint
		END

		-- 关闭游标
		CLOSE C_Employees;

		-- 释放游标
		DEALLOCATE C_Employees;
		
		SELECT * FROM @tempRankBack
		
		return 0
	END
	RETURN 0
END


GO




-- 获得排行
CREATE PROC GSP_GR_UpAddRankAward
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

	DECLARE @LastWeakRank datetime
	SELECT @LastWeakRank=DayValue FROM QPPlatformDBLink.QPPlatformDB.dbo.AddRankConfig WHERE RankName=N'LastWeakRank'
	IF @LastWeakRank = NULL
	BEGIN
		SET @LastWeakRank = 0
	END
	
	DECLARE @LastQuarterRank datetime
	SELECT @LastQuarterRank=DayValue FROM QPPlatformDBLink.QPPlatformDB.dbo.AddRankConfig WHERE RankName=N'LastQuarterRank'
	IF @LastQuarterRank = NULL
	BEGIN
		SET @LastQuarterRank = 0
	END
	
	DECLARE @LastYearWeakRank datetime
	SELECT @LastYearWeakRank=DayValue FROM QPPlatformDBLink.QPPlatformDB.dbo.AddRankConfig WHERE RankName=N'LastYearWeakRank'
	IF @LastYearWeakRank = NULL
	BEGIN
		SET @LastYearWeakRank = 0
	END
	

	DECLARE @WeakIdex INT
	DECLARE @userID AS INT
	DECLARE @UserPoint AS INT
	DECLARE @DayIdex AS INT
	DECLARE @RewardGold int
	DECLARE @RewardType int
	
	set datefirst 1
	select @WeakIdex = datepart(weekday, getdate())
	
	if DATEDIFF(dd,@LastWeakRank,GETDATE()) <> 0
	BEGIN
		if @WeakIdex = 7 
		BEGIN--周奖励
		
			SET @DayIdex = 1
			-- 声明游标
			DECLARE C_Employees CURSOR FAST_FORWARD FOR
				SELECT  TOP 50 UserID,UserPoint 
				FROM AccountsWeakAddRank
				ORDER BY UserPoint DESC
				
			OPEN C_Employees;

			-- 取第一条记录
			FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint;

			WHILE @@FETCH_STATUS=0
			BEGIN
				
				SELECT @RewardGold=RewardGold,@RewardType=RewardType FROM QPPlatformDBLink.QPPlatformDB.dbo.WeakAddRankConfig WHERE DayID=@DayIdex
				IF @RewardGold <> NULL and @RewardType <> NULL
				BEGIN
					IF @RewardType = 1
					BEGIN
						UPDATE GameScoreInfo SET Score= Score+RewardGold WHERE UserID = @userID
					END
				END
				
				SET @DayIdex = @DayIdex+1
				-- 取下一条记录
				FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint
			END

			-- 关闭游标
			CLOSE C_Employees;

			-- 释放游标
			DEALLOCATE C_Employees;
			
			--------------------------------------------
			--
			UPDATE QPPlatformDBLink.QPPlatformDB.dbo.AddRankConfig SET DayValue=getdate()	
			WHERE RankName=N'LastWeakRank'
		END
	END
	
	DECLARE @MonthEndDay datetime
	select @MonthEndDay = (DATEADD(MONTH,DATEDIFF(MONTH,'19911231',GETDATE()),'19911231'))
	
	DECLARE @MonthValue INT
	DECLARE @DayValue INT
	DECLARE @MonthEndDayValue INT
	select @DayValue = DAY(GETDATE())
	select @MonthValue = MONTH(DATEADD(MONTH,DATEDIFF(MONTH,'19911231',GETDATE()),'19911231'))
	select @MonthEndDayValue = DAY(DATEADD(MONTH,DATEDIFF(MONTH,'19911231',GETDATE()),'19911231'))
	
	IF @DayValue <> @MonthEndDayValue 
	BEGIN
		return 0
	END
	
	if DATEDIFF(dd,@LastQuarterRank,GETDATE()) <> 0
	BEGIN
		IF @MonthValue = 3 OR @MonthValue = 6 OR @MonthValue = 9 OR @MonthValue = 12 
		BEGIN --季度奖励
			
			SET @DayIdex = 1
			-- 声明游标
			DECLARE C_Employees CURSOR FAST_FORWARD FOR
				SELECT TOP 50 UserID,UserPoint 
				FROM AccountsQuarterAddRank
				ORDER BY UserPoint DESC
				
			OPEN C_Employees;

			-- 取第一条记录
			FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint;

			WHILE @@FETCH_STATUS=0
			BEGIN
				
				SELECT @RewardGold=RewardGold,@RewardType=RewardType FROM QPPlatformDBLink.QPPlatformDB.dbo.QuarterAddRankConfig WHERE DayID=@DayIdex
				IF @RewardGold <> NULL and @RewardType <> NULL
				BEGIN
					IF @RewardType = 1
					BEGIN
						UPDATE GameScoreInfo SET Score= Score+RewardGold WHERE UserID = @userID
					END
				END
				
				SET @DayIdex = @DayIdex+1
				-- 取下一条记录
				FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint
			END

			-- 关闭游标
			CLOSE C_Employees;

			-- 释放游标
			DEALLOCATE C_Employees;

			--------------------------------------------
			--
		
			UPDATE QPPlatformDBLink.QPPlatformDB.dbo.AddRankConfig SET DayValue=getdate()	
			WHERE RankName=N'LastQuarterRank'
		END
	END
	
	if DATEDIFF(dd,@LastYearWeakRank,GETDATE()) <> 0
	BEGIN
		IF @MonthValue = 12
		BEGIN --年奖励
			
			SET @DayIdex = 1
			-- 声明游标
			DECLARE C_Employees CURSOR FAST_FORWARD FOR
				SELECT TOP 50 UserID,UserPoint 
				FROM AccountsYearAddRank
				ORDER BY UserPoint DESC
				
			OPEN C_Employees;

			-- 取第一条记录
			FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint;

			WHILE @@FETCH_STATUS=0
			BEGIN
				
				SELECT @RewardGold=RewardGold,@RewardType=RewardType FROM QPPlatformDBLink.QPPlatformDB.dbo.YearAddRankConfig WHERE DayID=@DayIdex
				IF @RewardGold <> NULL and @RewardType <> NULL
				BEGIN
					IF @RewardType = 1
					BEGIN
						UPDATE GameScoreInfo SET Score= Score+RewardGold WHERE UserID = @userID
					END
				END
				
				SET @DayIdex = @DayIdex+1
				-- 取下一条记录
				FETCH NEXT FROM C_Employees INTO @UserID,@UserPoint
			END

			-- 关闭游标
			CLOSE C_Employees;

			-- 释放游标
			DEALLOCATE C_Employees;

			
			--------------------------------------------
			--
			
			UPDATE QPPlatformDBLink.QPPlatformDB.dbo.AddRankConfig SET DayValue=getdate()	
			WHERE RankName=N'LastYearWeakRank'
		END
	END
	return 0
END

GO
