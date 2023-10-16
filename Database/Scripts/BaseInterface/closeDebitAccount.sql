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
        WHERE accounts.account_id = ACC_ID AND users.user_id = UID
        )
    THEN
        DELETE FROM accounts
        WHERE
        accounts.account_id = ACC_ID;
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

--closeDebitAccount(...) example
SELECT closeDebitAccount(1000000000000000,  4012440109);
commit;
-- >> takes account_id as bigint, uid as bigint
-- << returns false on account does not exists
-- << returns true on successful deleting


--authcheck(...) example
SELECT
    result  ::int,
    uid     ::bigint,
    name    ::text,
    surname ::text
FROM authCheck('login', 'password');
-- >> takes login as text, password as text
-- << returns empty table on false
-- << returns result, uid, name, surname on true

--getTransactionHistory(...) example
SELECT
    tr_amount           ::double precision  AS amount,
    tr_from_id          ::bigint            AS from_id,
    tr_to_id            ::bigint            AS to_id,
    tr_date_and_time    ::text              AS date_and_time
FROM getTransactionHistory(4339392899)
ORDER BY tr_date_and_time;
-- >> takes uid
-- << returns list of transactions bonded with uid's accounts