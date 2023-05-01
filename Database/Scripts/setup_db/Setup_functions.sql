CREATE OR REPLACE FUNCTION random_string(length integer) RETURNS text AS
$$
DECLARE
  chars text[] := '{0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z}';
  result text := '';
  i integer := 0;
BEGIN
  for i in 1..length loop
    result := result || chars[1+random()*(array_length(chars, 1)-1)];
  end loop;
  return result;
END;
$$ LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION create_account_by_user_id(
	USER_ID bigint 
) 	RETURNS bigint AS 
$accountNum$
DECLARE 
	tmp bigint;
BEGIN
	INSERT INTO accounts(USER_ID)
		VALUES(USER_ID)
	RETURNING account_id INTO tmp;
	RETURN tmp;
END;
$accountNum$ LANGUAGE plpgsql;


DROP FUNCTION IF EXISTS closeDebitAccount(bigint, bigint);
DROP FUNCTION IF EXISTS authcheck(text,text);
DROP FUNCTION IF EXISTS registration(   text, text, text, text, bool,
                                        text, text, text, text, text,
                                        text, text, text, text);
DROP FUNCTION IF EXISTS conductTransfer(bigint, bigint, bigint, bigint);
DROP FUNCTION IF EXISTS getTransactionHistory(bigint);


CREATE OR REPLACE FUNCTION getTransactionHistory(uid bigint)
RETURNS TABLE(
            tr_id INT,
            tr_amount DOUBLE PRECISION,
            tr_from_id BIGINT,
            tr_to_id BIGINT,
            tr_date_and_time timestamp
             )
AS
$do$
    BEGIN
        RETURN QUERY (SELECT
            id,
            amount,
            from_id,
            to_id,
            date_and_time
        FROM transfers
        INNER JOIN accounts a ON a.account_id = transfers.to_id
        WHERE from_id IS NULL AND user_id = uid
        UNION
        SELECT
            id,
            amount,
            from_id,
            to_id,
            date_and_time
        FROM transfers
        INNER JOIN accounts a ON a.account_id = transfers.from_id
        WHERE from_id IS NOT NULL AND user_id = uid
        ORDER BY id);
    END;
$do$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION closeDebitAccount(account_id bigint, uid bigint)
RETURNS bool AS
$do$
DECLARE
    ACC_ID  bigint  :=  account_id;
    UID     bigint  :=  uid;
BEGIN
    IF EXISTS (
        SELECT accounts.account_id, users.user_id FROM accounts
        INNER JOIN users ON accounts.user_id = users.user_id
        WHERE accounts.account_id = 4339392899 AND users.user_id = 1000000000000154
        )
    THEN
        DELETE FROM accounts
        WHERE
        accounts.account_id = ACC_ID;
        COMMIT;
        RETURN true;
    END IF;
    RETURN false;
END;
$do$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION authCheck(Ulogin text, Upassword text)
RETURNS TABLE(
    result int,
    uid bigint,
    name text,
    surname text
             )
AS
$do$
BEGIN
    RETURN QUERY (SELECT
      count(*)::int,
      user_id::bigint,
      first_name::text,
      second_name::text
    FROM users
    INNER JOIN user_data
    ON users.user_id = user_data.passport_id
    WHERE login = Ulogin
    AND password = Upassword
    GROUP BY user_id, first_name, second_name);
END;
$do$ LANGUAGE plpgsql;