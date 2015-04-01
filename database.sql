create database cznp_database;

CREATE USER 'dummy'@'localhost' IDENTIFIED BY 'dummy';
GRANT ALL PRIVILEGES ON cznp_database.* TO 'dummy'@'localhost';

CREATE TABLE button (
    counter INT(32) UNSIGNED
);