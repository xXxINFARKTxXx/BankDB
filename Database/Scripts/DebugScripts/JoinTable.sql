SELECT *
FROM users us
INNER JOIN accounts ac
ON ac.user_id = us.user_id
INNER JOIN user_data ud
on ud.passport_id = ac.user_id
ORDER BY account_id;