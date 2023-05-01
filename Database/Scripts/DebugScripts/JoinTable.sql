-- SELECT * FROM user_data;
-- SELECT * FROM users;

SELECT *
FROM users
INNER JOIN accounts
ON accounts.user_id = users.user_id
INNER JOIN user_data ud on ud.passport_id = accounts.user_id
ORDER BY account_id;