DROP FUNCTION IF EXISTS closeDebitAccount(bigint, bigint);
DROP FUNCTION IF EXISTS authcheck(text,text);
DROP FUNCTION IF EXISTS registration(   text, text, text, text, bool,
                                        text, text, text, text, text,
                                        text, text, text, text);
DROP FUNCTION IF EXISTS conductTransfer(bigint, bigint, bigint, bigint);
DROP FUNCTION IF EXISTS getTransactionsInfo(bigint);


CREATE OR REPLACE FUNCTION closeDebitAccount(account_id bigint, uid bigint)
RETURNS bool AS
$do$
DECLARE
    ACC_ID  bigint  :=  account_id;
    UID     bigint  :=  uid;
BEGIN
    IF EXISTS (
        SELECT * FROM public.accounts
        INNER JOIN public.users ON public.accounts.user_id = public.users.user_id
        WHERE public.accounts.account_id = ACC_ID AND public.users.user_id = UID
        )
    THEN
              DELETE FROM public.accounts
        WHERE
            public.accounts.account_id = ACC_ID;
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
SELECT closeDebitAccount(1000000000000000, 4012440109);
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