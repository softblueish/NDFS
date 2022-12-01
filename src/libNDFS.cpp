#include <iostream>
#include <fstream>
#include <vector>

class NDFS{
    public:
        // Basic functionality
        bool faillock = false; // If the database has a lock present, this will become true.
        bool isLocked(){ // Checks if the database is locked and returns the value
            std::ifstream lockfile(lockfileDirectory);
            std::string locktest;
            lockfile >> locktest;
            lockfile.close();
            if(locktest.compare("lock")==0){
                return true;
            } 
            return false;
        }
        
        // Constructor
        NDFS(std::string databaseDirectory){ // Create or open a database
            storedDatabaseDirectory = databaseDirectory; // Store database directory to variable
            lockfileDirectory = storedDatabaseDirectory + ".lock"; // Makes a lockfile directory for locking the database
            if(isLocked()) {faillock = true; std::cerr << "Lockfile present\n";} else { // Checks if database is locked
                faillock = false;
                std::ifstream inputDatabaseFilestream(databaseDirectory); // Check if database is empty, if not, initialize one
                lock();
                std::string buffer;
                inputDatabaseFilestream >> buffer;
                inputDatabaseFilestream.close();
                if(buffer.size()<1){
                    std::ofstream outputDatabaseFilestream(databaseDirectory);
                    outputDatabaseFilestream << "$";
                    outputDatabaseFilestream.close();
                }
                unlock();
            }
        }

        // Column manipulation
        int createColumn(std::string columnName){ // Creates a column in the database
            if(isLocked()) {std::cerr << "Lockfile present\n"; return -2;} // Checks if database is locked
            if(getColumnPosition(columnName)>-1){std::cerr << "createColumn: Column already exists\n"; return -1;} // Checks that the column doesnt already exist
            lock();
            std::string readBuffer; // Stores the read string literal
            std::vector<std::string> writeBuffer; // Stores what should be written IN ORDER
            std::ifstream inputDatabaseFilestream(storedDatabaseDirectory);
            inputDatabaseFilestream >> readBuffer; // Read entire file
            inputDatabaseFilestream.close();
            std::string blockBuffer; // Stores blocks of data temporarily
            int cursor=0; // Selects the block in the string literal
            int section=0; // Selects the section
            for(int i = 0; i<readBuffer.size(); i++){ // Rereads the database an injects the new column
                if(readBuffer.at(i)=='$'&&section==0){ // If new section
                    writeBuffer.push_back(columnName);
                    writeBuffer.push_back(";");
                    section++;
                    writeBuffer.push_back("$");
                }
                if(readBuffer.at(i)==';'){ // If new block
                    cursor++;
                    writeBuffer.push_back(blockBuffer);
                    blockBuffer = "";
                    writeBuffer.push_back(";");
                }else if(readBuffer.at(i)==':'&&section==1){ // If new row
                    writeBuffer.push_back(":");
                }else{ // Write to blockbuffer
                    blockBuffer.append(readBuffer.substr(i, 1));
                }
            }

            std::string toFileBuffer;
            for(int i = 0; i<writeBuffer.size(); i++){ // Compile the file
                toFileBuffer.append(writeBuffer.at(i));
            }
            std::ofstream outputDatebaseFileStream(storedDatabaseDirectory);
            outputDatebaseFileStream << toFileBuffer; // Write to file
            outputDatebaseFileStream.close();

            unlock();
            return 0; // Success
        }
        int getColumnPosition(std::string columnName){ // Get position of column
            if(isLocked()) {std::cerr << "Lockfile present"; return -2;} // Checks if database is locked
            lock();
            std::string readBuffer; // Stores whole file
            std::ifstream inputDatabaseFilestream(storedDatabaseDirectory);
            inputDatabaseFilestream >> readBuffer; // Reads the file
            std::string blockBuffer; // Stores info as blocks
            int cursor;
            std::vector<std::string> separatedBuffer;
            for(int i = 0; i < readBuffer.size(); i++){
                if(readBuffer.at(i)=='$'){
                    break;
                }
                if(readBuffer.at(i)==';'){
                    cursor++;
                    separatedBuffer.push_back(blockBuffer);
                    blockBuffer = "";
                } else {
                    blockBuffer.append(readBuffer.substr(i, 1));
                }
            }

            for(int i = 0; i < separatedBuffer.size(); i++){
                if(separatedBuffer.at(i).compare(columnName)==0){
                    unlock();
                    return i;
                }
            }
            unlock();
            return -1; // Cant find it
        }
        int renameColumn(std::string oldColumnName, std::string newColumnName){ // Rename a column
            if(isLocked()) {std::cerr << "Lockfile present\n"; return -2;} // Checks if database is locked
            if(getColumnPosition(oldColumnName)<0){std::cerr << "renameColumn: Column doesn't exist\n"; return -1;} // Checks that the column doesnt already exist
            if(getColumnPosition(newColumnName)>-1){std::cerr << "renameColumn: Column already exists\n"; return -1;} // Checks that the column doesnt already exist
            lock();
            std::string readBuffer; // Stores the read string literal
            std::vector<std::string> writeBuffer; // Stores what should be written IN ORDER
            std::ifstream inputDatabaseFilestream(storedDatabaseDirectory);
            inputDatabaseFilestream >> readBuffer; // Read entire file
            inputDatabaseFilestream.close();
            std::string blockBuffer; // Stores blocks of data temporarily
            int cursor=0; // Selects the block in the string literal
            int section=0; // Selects the section
            for(int i = 0; i<readBuffer.size(); i++){ // Rereads the database an injects the new column
                if(readBuffer.at(i)=='$'&&section==0){ // If new section
                    section++;
                    writeBuffer.push_back("$");
                }
                if(readBuffer.at(i)==';'){ // If new block
                    cursor++;
                    if(blockBuffer.compare(oldColumnName)==0) blockBuffer = newColumnName;
                    writeBuffer.push_back(blockBuffer);
                    blockBuffer = "";
                    writeBuffer.push_back(";");
                }else if(readBuffer.at(i)==':'&&section==1){ // If new row
                    writeBuffer.push_back(":");
                }else{ // Write to blockbuffer
                    blockBuffer.append(readBuffer.substr(i, 1));
                }
            }

            std::string toFileBuffer;
            for(int i = 0; i<writeBuffer.size(); i++){ // Compile the file
                toFileBuffer.append(writeBuffer.at(i));
            }
            std::ofstream outputDatebaseFileStream(storedDatabaseDirectory);
            outputDatebaseFileStream << toFileBuffer; // Write to file
            outputDatebaseFileStream.close();

            unlock();
            return 0; // Success
        }
        int removeColumn(std::string columnName){ // NEEDS TO BE DONE WHEN ROWS ARE DONE

        return -1; // No Code
        }
        
        // Row manipulation

    private:
        std::string storedDatabaseDirectory;
        std::string lockfileDirectory;

        void lock(){ // Locks the database
            std::ofstream lockfile(lockfileDirectory);
            lockfile << "lock";
            lockfile.close();
        }
        void unlock(){ // Unlocks the database
            remove(lockfileDirectory.c_str());
        }
};