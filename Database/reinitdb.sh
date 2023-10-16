cd Scripts/
psql -U postgres postgres -f FullyDropDatabase.sql
cd setup_db/
psql -U postgres postgres -f CreateDatabase.sql
psql -U postgres bank -f Setup_base.sql
cd ..
cd DebugScripts
psql -U postgres bank -f InsertValues.sql
cd .. 
cd ..