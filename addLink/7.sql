/****** Object:  LinkedServer [QPGameScoreDBLink]    Script Date: 03/31/2016 03:38:10 ******/
IF  EXISTS (SELECT srv.name FROM sys.servers srv WHERE srv.server_id != 0 AND srv.name = N'QPGameScoreDBLink')EXEC master.dbo.sp_dropserver @server=N'QPGameScoreDBLink', @droplogins='droplogins'
GO

/****** Object:  LinkedServer [QPGameScoreDBLink]    Script Date: 03/31/2016 03:38:10 ******/
EXEC master.dbo.sp_addlinkedserver @server = N'QPGameScoreDBLink', @srvproduct=N'QPGameScoreDBLink', @provider=N'SQLNCLI', @datasrc=N'(local)', @catalog=N'QPGameScoreDB'
 /* For security reasons the linked server remote logins password is changed with ######## */
EXEC master.dbo.sp_addlinkedsrvlogin @rmtsrvname=N'QPGameScoreDBLink',@useself=N'True',@locallogin=NULL,@rmtuser=NULL,@rmtpassword=NULL

GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'collation compatible', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'data access', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'dist', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'pub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'rpc', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'rpc out', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'sub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'connect timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'collation name', @optvalue=null
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'lazy schema validation', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'query timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'use remote collation', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameScoreDBLink', @optname=N'remote proc transaction promotion', @optvalue=N'true'
GO


