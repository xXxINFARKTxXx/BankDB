echo "Введите имя пользователя:"
read user

cd Scripts/
printf "$password" | psql -U "$user" postgres -f DropDatabase.sql
cd ..
