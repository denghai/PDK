USE QPPlatformDB
GO

-- «©µΩ≈‰÷√
TRUNCATE TABLE SigninConfig
GO

INSERT INTO [dbo].[SigninConfig] ([DayID], [RewardGold],[RewardType],[RewardDay]) VALUES (1, 1000,1,1)
INSERT INTO [dbo].[SigninConfig] ([DayID], [RewardGold],[RewardType],[RewardDay]) VALUES (2, 2000,1,6)
INSERT INTO [dbo].[SigninConfig] ([DayID], [RewardGold],[RewardType],[RewardDay]) VALUES (3, 4000,1,9)
INSERT INTO [dbo].[SigninConfig] ([DayID], [RewardGold],[RewardType],[RewardDay]) VALUES (4, 8000,1,13)
INSERT INTO [dbo].[SigninConfig] ([DayID], [RewardGold],[RewardType],[RewardDay]) VALUES (5, 16000,1,16)

GO