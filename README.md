# Find-An-Instance
This project was given as a part of course on "Oject oriented programming" on Faculty of Electrical Engineering in Belgrade. 

The given program is able to load database from .csv file (tested on databases with 2^20^ instances, command LOAD), and then extract informations from it:
  - It can find the unique instance based on the exact description, when user determines all of the characteristics. (GUESS)
  - It can find all the instances in database that share some set of characteristics. (TRY)
  - It can list all the characteristics of an instance given it's name. (INFO)
  - User can enter a new instance into the database, if it's name or set of characteristicsis not present in it. (NEW)
  - User can delete an existing instance from database, by entering it's name. (DELETE)
  - Lastly, user can finish end the program. (EXIT)
Note: if user loads new database, he will be able to work with it, but **the previous database is going to be deleted**.
