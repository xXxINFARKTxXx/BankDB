echo "Введите имя пользователя:"
read user

cd Scripts/
psql -U "$user" postgres -f CreateDatabase.sql
psql -U "$user" bank -f Setup_base.sql
cd ..
