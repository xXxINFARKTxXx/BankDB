echo "Введите имя пользователя:"
read user

cd Scripts/
psql -U "$user" postgres -f DropDatabase.sql
psql -U "$user" postgres -f CreateDatabase.sql
psql -U "$user" bank -f Setup_base.sql
psql -U "$user" bank -f DebugScripts/InsertValues.sql
cd ..
