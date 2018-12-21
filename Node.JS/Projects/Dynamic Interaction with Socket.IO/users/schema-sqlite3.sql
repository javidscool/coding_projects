CREATE TABLE IF NOT EXISTS users (
	id INT NOT NULL AUTO_INCREMENT,
	username VARCHAR(255),
	password VARCHAR(255),
	provider VARCHAR(255),
	familyName VARCHAR(255),
	givenName VARCHAR(255),
	middleName VARCHAR(255),
	emails TEXT,
	photos TEXT,
	createdAt DATETIME,
	updatedAt DATETIME
);