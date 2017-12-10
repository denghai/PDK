/****** Object:  LinkedServer [QPNativeWebDBLink]    Script Date: 03/31/2016 03:38:20 ******/
IF  EXISTS (SELECT srv.name FROM sys.servers srv WHERE srv.server_id != 0 AND srv.name = N'QPNativeWebDBLink')EXEC master.dbo.sp_dropserver @server=N'QPNativeWebDBLink', @droplogins='droplogins'
GO

/****** Object:  LinkedServer [QPNativeWebDBLink]    Script Date: 03/31/2016 03:38:20 ******/
EXEC master.dbo.sp_addlinkedserver @server = N'QPNativeWebDBLink', @srvproduct=N'QPNativeWebDBLink', @provider=N'SQLNCLI', @datasrc=N'(local)', @catalog=N'QPNativeWebDB'
 /* For security reasons the linked server remote logins password is changed with ######## */
EXEC master.dbo.sp_addlinkedsrvlogin @rmtsrvname=N'QPNativeWebDBLink',@useself=N'True',@locallogin=NULL,@rmtuser=NULL,@rmtpassword=NULL

GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'collation compatible', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'data access', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'dist', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'pub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'rpc', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'rpc out', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'sub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'connect timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'collation name', @optvalue=null
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'lazy schema validation', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'query timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'use remote collation', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPNativeWebDBLink', @optname=N'remote proc transaction promotion', @optvalue=N'true'
GO


