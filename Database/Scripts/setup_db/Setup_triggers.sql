CREATE OR REPLACE FUNCTION create_user_acc() 
   RETURNS TRIGGER
   LANGUAGE PLPGSQL
AS $$
DECLARE
	tempLogin text;
	myKey int := 1;
	logcheck int;
BEGIN
	
	DROP TABLE IF EXISTS logins;
	CREATE TEMP TABLE logins AS
	SELECT login FROM users;
	
	while myKey = 1 loop
		tempLogin := random_string(20);

		SELECT COUNT(*) 
		FROM logins INTO logcheck 
			WHERE logins.login = tempLogin;
			
		if logcheck = 0 then
			myKey := 0;
		end if;
			
	end loop;
	
	INSERT INTO users(user_id, login)
		VALUES(NEW.passport_id, tempLogin);
    RETURN NULL;
END;
$$;

CREATE OR REPLACE TRIGGER add_user
	AFTER INSERT
	ON user_data
	FOR EACH ROW
	EXECUTE PROCEDURE create_user_acc();
	
