SELECT
    id,
    amount,
    from_id,
    to_id,
    date_and_time
FROM transfers
INNER JOIN accounts a ON a.account_id = transfers.to_id
WHERE from_id IS NULL AND user_id = 4012440109
UNION
SELECT
    id,
    amount,
    from_id,
    to_id,
    date_and_time
FROM transfers
INNER JOIN accounts a ON a.account_id = transfers.from_id
WHERE from_id IS NOT NULL AND user_id = 4012440109
ORDER BY id;