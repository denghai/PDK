/****** Object:  LinkedServer [QPPlatformManagerDBLink]    Script Date: 03/31/2016 03:38:39 ******/
IF  EXISTS (SELECT srv.name FROM sys.servers srv WHERE srv.server_id != 0 AND srv.name = N'QPPlatformManagerDBLink')EXEC master.dbo.sp_dropserver @server=N'QPPlatformManagerDBLink', @droplogins='droplogins'
GO

/****** Object:  LinkedServer [QPPlatformManagerDBLink]    Script Date: 03/31/2016 03:38:39 ******/
EXEC master.dbo.sp_addlinkedserver @server = N'QPPlatformManagerDBLink', @srvproduct=N'QPPlatformManagerDBLink', @provider=N'SQLNCLI', @datasrc=N'(local)', @catalog=N'QPPlatformManagerDB'
 /* For security reasons the linked server remote logins password is changed with ######## */
EXEC master.dbo.sp_addlinkedsrvlogin @rmtsrvname=N'QPPlatformManagerDBLink',@useself=N'True',@locallogin=NULL,@rmtuser=NULL,@rmtpassword=NULL

GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'collation compatible', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'data access', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'dist', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'pub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'rpc', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'rpc out', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'sub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'connect timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'collation name', @optvalue=null
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'lazy schema validation', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'query timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'use remote collation', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPPlatformManagerDBLink', @optname=N'remote proc transaction promotion', @optvalue=N'true'
GO


