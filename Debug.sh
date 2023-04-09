cd Scripts/
psql -U postgres postgres -f DropAll.sql
psql -U postgres bank -f Setup_base.sql
cd ..
