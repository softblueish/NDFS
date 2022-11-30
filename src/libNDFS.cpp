#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

class NDFS{
    public:
        NDFS(std::string databaseDirectory){ // Create or open a database
            storedDatabaseDirectory = databaseDirectory; // Store database directory to variable
            lockfileDirectory = storedDatabaseDirectory + ".lock"; // Makes a lockfile directory for locking the database

            std::ifstream inputDatabaseFilestream(databaseDirectory); // Check if database is empty, if not, initialize one
            lockDatabase();
            std::string buffer;
            inputDatabaseFilestream >> buffer;
            inputDatabaseFilestream.close();
            if(buffer.size()<1){
                std::ofstream outputDatabaseFilestream(databaseDirectory);
                outputDatabaseFilestream << "§";
                outputDatabaseFilestream.close();
            }
            unlockDatabase();
        }

        int createColumn(std::string columnName){ // Creates a column in the database
            lockDatabase();
            std::ifstream inputDatabaseFilestream(storedDatabaseDirectory);
            // Read all columns

        }

    private:
        std::string storedDatabaseDirectory;
        std::string lockfileDirectory;

        void lockDatabase(){ // Locks the database
            std::ofstream lockfile(lockfileDirectory);
            lockfile << " ";
            lockfile.close();
        }
        void unlockDatabase(){ // Unlocks the database
            remove(lockfileDirectory.c_str());
        }
};