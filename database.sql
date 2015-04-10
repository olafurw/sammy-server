create database cznp_database;

CREATE USER 'dummy'@'localhost' IDENTIFIED BY 'dummy';
GRANT ALL PRIVILEGES ON cznp_database.* TO 'dummy'@'localhost';

USE cznp_database;

CREATE TABLE blog (
    id INT(32) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    body TEXT,
    created_date INT(32) UNSIGNED NOT NULL,
    modified_date INT(32) UNSIGNED NOT NULL,
    votes INT(32) UNSIGNED NOT NULL
);
