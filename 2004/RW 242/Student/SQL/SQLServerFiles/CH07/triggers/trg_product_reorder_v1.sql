IF EXISTS (SELECT name FROM sysobjects
      WHERE name = 'TRG_PRODUCT_REORDER' AND type = 'TR')
   DROP TRIGGER TRG_PRODUCT_REORDER
GO
CREATE TRIGGER TRG_PRODUCT_REORDER
ON PRODUCT
FOR INSERT, UPDATE 
AS
BEGIN 
   IF UPDATE(P_ONHAND)
   BEGIN
   UPDATE PRODUCT
       	SET P_REORDER = 1 
       	WHERE P_ONHAND <= P_MIN;
   END
END;