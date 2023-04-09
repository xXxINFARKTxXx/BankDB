DROP TABLE IF EXISTS transactions CASCADE;
DROP TABLE IF EXISTS user_data CASCADE;
DROP TABLE IF EXISTS users CASCADE;
DROP TABLE IF EXISTS accounts CASCADE;
DROP TABLE IF EXISTS transfers CASCADE;
DROP SEQUENCE IF EXISTS accounts_id_seq;

-- takes string from app, registrate it ID and type of operarion
-- 0 - registration of user's bank account
-- 1 - registration of user's debit account
-- 2 - conduct money transfere 

CREATE TABLE user_data (
	passport_id 				bigint CONSTRAINT user_data_passport_id_unique_pkey PRIMARY KEY UNIQUE,
	first_name 					text NOT NULL,
	second_name 				text NOT NULL
-- 	last_name 					text DEFAULT NULL,
-- 	sex 						bool NOT NULL,
-- 	date_of_birth 				date NOT NULL,
-- 	place_of_birth 				text NOT NULL,
-- 	country 					text NOT NULL,
-- 	passport_date_of_issue 		date NOT NULL,
-- 	passport_date_of_expire 	date NOT NULL,
-- 	passport_place_of_issue 	text NOT NULL,
-- 	passport_authority 			text NOT NULL,
-- 	adress_of_living			text NOT NULL,
-- 	email						text NOT NULL
);


CREATE TABLE users (
	user_id 				bigint REFERENCES user_data(passport_id),
	login 					text CONSTRAINT users_login_uniqueness UNIQUE,
	password_string 		text DEFAULT random_string(20),
	date_of_registration 	timestamp DEFAULT NOW()
);

CREATE SEQUENCE accounts_id_seq
	INCREMENT 11
START 1000000000000000;

CREATE TABLE accounts (
	account_id 			bigint CONSTRAINT accounts_account_pkey NOT NULL PRIMARY KEY DEFAULT nextval('accounts_id_seq'),
	user_id 			bigint REFERENCES user_data(passport_id),
	deposite 			double precision DEFAULT 0.0,
	date_of_creating 	timestamp DEFAULT NOW()
);

CREATE TABLE transfers (
	id 			serial CONSTRAINT transfers_id_pkey PRIMARY KEY,
	-- from_id(NULL) -> to_id | amount > 0: "add money to account №to_id"
	-- from_id(NULL) -> to_id | amount < 0: "get money from account №to_id"
	from_id 	bigint REFERENCES accounts(account_id),
	to_id 		bigint CONSTRAINT transfers_to_id_not_null NOT NULL REFERENCES accounts(account_id),
	amount 		double precision CONSTRAINT transfers_amount_not_null_not_zero NOT NULL CHECK (amount <> 0)
);









