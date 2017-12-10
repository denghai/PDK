USE QPTreasureDB
GO
-- 转账  
CREATE PROC GSP_GR_FengziZZJL  
 @dwUserID INT,        -- 用户标识  
 @strErrorDescribe NVARCHAR(127) OUTPUT  -- 输出信息  
WITH ENCRYPTION AS  
  
-- 属性设置  
SET NOCOUNT ON  
  
-- 执行逻辑  
BEGIN  

  SELECT t2.Accounts as SourceAccounts, t3.Accounts as TargetAccounts,t3.GameID , t1.SwapScore, t1.Revenue, t1.CollectDate
  FROM RecordInsure t1, QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo t2, QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo t3
  WHERE t1.SourceUserID = t2.UserID
  AND   t1.TargetUserID = t3.UserID
  AND   (t1.SourceUserID = @dwUserID or t1.TargetUserID = @dwUserID) Order by t1.CollectDate desc
  
END  
  
RETURN 0  
  