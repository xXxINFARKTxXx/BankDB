-- SELECT * FROM user_data;
-- SELECT * FROM users;

SELECT *
FROM user_data
INNER JOIN users
ON user_data.passport_id = users.user_id
INNER JOIN accounts
ON user_data.passport_id = accounts.user_id
ORDER BY passport_id;

