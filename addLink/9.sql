/****** Object:  LinkedServer [QPAccountsDBLink]    Script Date: 03/31/2016 03:37:47 ******/
IF  EXISTS (SELECT srv.name FROM sys.servers srv WHERE srv.server_id != 0 AND srv.name = N'QPAccountsDBLink')EXEC master.dbo.sp_dropserver @server=N'QPAccountsDBLink', @droplogins='droplogins'
GO

/****** Object:  LinkedServer [QPAccountsDBLink]    Script Date: 03/31/2016 03:37:47 ******/
EXEC master.dbo.sp_addlinkedserver @server = N'QPAccountsDBLink', @srvproduct=N'QPAccountsDBLink', @provider=N'SQLNCLI', @datasrc=N'(local)', @catalog=N'QPAccountsDB'
 /* For security reasons the linked server remote logins password is changed with ######## */
EXEC master.dbo.sp_addlinkedsrvlogin @rmtsrvname=N'QPAccountsDBLink',@useself=N'True',@locallogin=NULL,@rmtuser=NULL,@rmtpassword=NULL

GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'collation compatible', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'data access', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'dist', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'pub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'rpc', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'rpc out', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'sub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'connect timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'collation name', @optvalue=null
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'lazy schema validation', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'query timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'use remote collation', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPAccountsDBLink', @optname=N'remote proc transaction promotion', @optvalue=N'true'
GO


