#!/bin/bash


USER=sign
PASSWORD=ofthetimes
DB=sign

for i in `ls *.sql`
do
echo -n "Loading $i .. "
cat $i | mysql -u $USER -p$PASSWORD $DB
echo "Done"
done

