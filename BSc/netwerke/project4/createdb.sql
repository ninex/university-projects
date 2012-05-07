-- Your databases have already been created. If your student number is 12345678, your 
-- username is s12345678, your database is called s12345678 and your password is 
-- s12345678. You can change your password with the SQL command
--	SET PASSWORD FOR s12345678=PASSWORD('mynewpassword');

-- You can't run the next 2 lines in the lab, but this is how you would create a new database
-- if you had the access rights.
--DROP DATABASE IF EXISTS proj1;
--CREATE DATABASE proj1;

-- Change the following database_name to reflect your database name
USE database_name;

CREATE TABLE Finances(
	fieldId VARCHAR(10) UNIQUE NOT NULL,
	amount FLOAT(2) NOT NULL,
	PRIMARY KEY(fieldId));

INSERT INTO Finances VALUES('Balance', 1000.00);

CREATE TABLE FavouriteActivities(
	id INT NOT NULL AUTO_INCREMENT,
	name VARCHAR(50),
	PRIMARY KEY(id));

CREATE TABLE Appointments(
	id INT NOT NULL AUTO_INCREMENT,
	startTime DATETIME NOT NULL,
	endTime DATETIME NOT NULL,
	friendName VARCHAR(40) NOT NULL,
	activityId INT NOT NULL REFERENCES FavouriteActivities(id),
	cost FLOAT(2) NOT NULL, 
	PRIMARY KEY(id));

-- To run this script, use the PHPMyAdmin portal at bach.sun.ac.za, or in the shell type
--	mysql -u s12345678 -p < createdb.sql

