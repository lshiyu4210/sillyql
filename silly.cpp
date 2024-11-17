// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "silly.h"

//Implementations for SillyQL class
void SillyQL::process_command(string cmd){
    char c = cmd[0];
    switch (c) {
        case 'C':
            create();
            break;

        case 'Q':
            quit();
            break;

        case '#':
            comment();
            break;

        case 'R':
            remove();
            break;

        case 'I': {
            string table_name;
            string next;
            int num_rows;
            cin >> next >> table_name >> num_rows >> next;  // INTO <tablename> <N> ROWS
            if (database.find(table_name) == database.end()) {
                cout << "Error during INSERT: " << table_name << " does not name a table in the database\n";
                comment();
            }
            else{
                database[table_name].insert(num_rows);
            }
            break;
        }

        case 'P': {
            string table_name;
            cin >> table_name >> table_name;
            if (database.find(table_name) == database.end()) {
                cout << "Error during PRINT: " << table_name << " does not name a table in the database\n";
                comment();
            }
            else{
                database[table_name].print_lines();
            }
            break;
        }

        case 'D': {
            string table_name;
            cin >> table_name >> table_name;
            if (database.find(table_name) == database.end()) {
                cout << "Error during DELETE: " << table_name << " does not name a table in the database\n";
                comment();
            }
            else{
                database[table_name].delete_data();
            }
            break;
        }

        case 'J': {
            string table_name1;
            string table_name2;
            string trash;
            cin >> table_name1 >> trash >> table_name2 >> trash;    //<tablename1> AND <tablename2> WHERE
            if (database.find(table_name1) == database.end()) {
                cout << "Error during JOIN: " << table_name1 << " does not name a table in the database\n";
                comment();
            }
            else if (database.find(table_name2) == database.end()) {
                cout << "Error during JOIN: " << table_name2 << " does not name a table in the database\n";
                comment();
            }
            else{
                Table &t1 = database[table_name1];
                Table &t2 = database[table_name2];
                t1.join(t2);
            }
            break;
        }

        case 'G': {
            string temp;
            string col_name;
            cin >> temp >> temp;    //FOR <tablename> 
            if (database.find(temp) == database.end()) {
                cout << "Error during GENERATE: " << temp << " does not name a table in the database\n";
                comment();
            }
            else{
                Table &t = database[temp];
                cin >> temp >> col_name >> col_name >> col_name;  //<indextype> INDEX ON <colname>
                if(temp == "hash") {
                    t.generate_hash(col_name);
                }
                else {
                    t.generate_bst(col_name);
                }
            }
            
            break;
        }
            
        default:
            cout << "Error: unrecognized command\n";
            comment();
            break;
    }
}

void SillyQL::create(){
    string table_name;
    int col_num;
    cin >> table_name >> col_num;
    Table tb(table_name, col_num, quiet);
    if(database.find(table_name) != database.end()){
        cout << "Error during CREATE: Cannot create already existing table " << table_name << "\n";
        comment();
    }
    else {
        database.emplace(table_name, tb);
    }
}

void SillyQL::quit(){
    database.clear();
    cout << "Thanks for being silly!\n";
}

void SillyQL::comment(){
    string trash;
    getline(cin, trash);
}

void SillyQL::remove(){
    string target;
    cin >> target;
    if(database.find(target) == database.end()){
        cout << "Error during REMOVE: " << target << " does not name a table in the database\n";
        comment();
    }
    else {
        database.erase(target);
        cout << "Table " << target << " deleted\n";
    }
    
}

