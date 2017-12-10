/****** Object:  LinkedServer [QPGameMatchDBLink]    Script Date: 03/31/2016 03:37:59 ******/
IF  EXISTS (SELECT srv.name FROM sys.servers srv WHERE srv.server_id != 0 AND srv.name = N'QPGameMatchDBLink')EXEC master.dbo.sp_dropserver @server=N'QPGameMatchDBLink', @droplogins='droplogins'
GO

/****** Object:  LinkedServer [QPGameMatchDBLink]    Script Date: 03/31/2016 03:37:59 ******/
EXEC master.dbo.sp_addlinkedserver @server = N'QPGameMatchDBLink', @srvproduct=N'QPGameMatchDBLink', @provider=N'SQLNCLI', @datasrc=N'(local)', @catalog=N'QPGameMatchDB'
 /* For security reasons the linked server remote logins password is changed with ######## */
EXEC master.dbo.sp_addlinkedsrvlogin @rmtsrvname=N'QPGameMatchDBLink',@useself=N'True',@locallogin=NULL,@rmtuser=NULL,@rmtpassword=NULL

GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'collation compatible', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'data access', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'dist', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'pub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'rpc', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'rpc out', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'sub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'connect timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'collation name', @optvalue=null
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'lazy schema validation', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'query timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'use remote collation', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPGameMatchDBLink', @optname=N'remote proc transaction promotion', @optvalue=N'true'
GO


