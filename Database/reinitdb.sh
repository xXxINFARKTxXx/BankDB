PASSWORD=postgres
MAINTENANCE_DB=postgres
AIM_DB=bank
USER=postgres


cd Scripts/
PGPASSWORD=$PASSWORD psql -U $USER $MAINTENANCE_DB -f FullyDropDatabase.sql
cd setup_db/
PGPASSWORD=$PASSWORD psql -U $USER $MAINTENANCE_DB -f CreateDatabase.sql
PGPASSWORD=$PASSWORD psql -U $USER $AIM_DB -f Setup_base.sql
cd ..
cd DebugScripts
PGPASSWORD=$PASSWORD psql -U $USER $AIM_DB -f InsertValues.sql
cd .. 
cd .