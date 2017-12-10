/****** Object:  LinkedServer [QPTreasureDBLink]    Script Date: 03/31/2016 03:39:11 ******/
IF  EXISTS (SELECT srv.name FROM sys.servers srv WHERE srv.server_id != 0 AND srv.name = N'QPTreasureDBLink')EXEC master.dbo.sp_dropserver @server=N'QPTreasureDBLink', @droplogins='droplogins'
GO

/****** Object:  LinkedServer [QPTreasureDBLink]    Script Date: 03/31/2016 03:39:11 ******/
EXEC master.dbo.sp_addlinkedserver @server = N'QPTreasureDBLink', @srvproduct=N'QPTreasureDBLink', @provider=N'SQLNCLI', @datasrc=N'(local)', @catalog=N'QPTreasureDB'
 /* For security reasons the linked server remote logins password is changed with ######## */
EXEC master.dbo.sp_addlinkedsrvlogin @rmtsrvname=N'QPTreasureDBLink',@useself=N'True',@locallogin=NULL,@rmtuser=NULL,@rmtpassword=NULL

GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'collation compatible', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'data access', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'dist', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'pub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'rpc', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'rpc out', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'sub', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'connect timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'collation name', @optvalue=null
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'lazy schema validation', @optvalue=N'false'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'query timeout', @optvalue=N'0'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'use remote collation', @optvalue=N'true'
GO

EXEC master.dbo.sp_serveroption @server=N'QPTreasureDBLink', @optname=N'remote proc transaction promotion', @optvalue=N'true'
GO


