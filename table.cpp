// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "silly.h"

//Implementations for Table class
SillyQL::Table::Table() : name(""), col_num(0){}

SillyQL::Table::Table(const string &name_in, const int &col_num_in, const bool &quiet_in) :
    name(name_in), col_num(col_num_in), quiet(quiet_in) {
        col_types.resize(col_num);
        string temp;

        for (int  i = 0; i < col_num; ++i) {
            cin >> temp;
            char c = temp[0];
            switch (c) {
                case 'd':
                col_types[i] = EntryType::Double;
                break;

            case 'i':
                col_types[i] = EntryType::Int;
                break;

            case 'b': 
                col_types[i] = EntryType::Bool;
                break;

            case 's':
                col_types[i] = EntryType::String;
                break;
            }
        }
        
        cout << "New table " << name << " with column(s) ";
        for (int  i = 0; i < col_num; ++i) {
            cin >> temp;
            cout << temp << " ";
            col_names[temp] = i;
            if(i == 0)  first_col_name = temp;
        }
        cout << "created\n";
    }

void SillyQL::Table::insert(int &num_rows) {

    int startN = (int)matrix.size();
    matrix.resize(startN + num_rows);

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < col_num; ++j) {
            matrix[startN + i].reserve(col_num);
            switch (col_types[j]) {
                case EntryType::Double:
                    double d;
                    cin >> d;
                    matrix[startN + i].emplace_back(d);
                    // if(index_state == Index::BST && (int)(matrix[startN + i].size() - 1) == indexed_column) {
                    //     bst[TableEntry(d)].push_back(startN + i);
                    // }
                    // if(index_state == Index::HASH && (int)(matrix[startN + i].size() - 1) == indexed_column) {
                    //     hash[TableEntry(d)].push_back(startN + i);
                    // }
                    break;

                case EntryType::Int:
                    int it;
                    cin >> it;
                    matrix[startN + i].emplace_back(it);
                    // if(index_state == Index::BST && (int)(matrix[startN + i].size() - 1) == indexed_column) {
                    //     bst[TableEntry(it)].push_back(startN + i);
                    // }
                    // if(index_state == Index::HASH && (int)(matrix[startN + i].size() - 1) == indexed_column) {
                    //     hash[TableEntry(it)].push_back(startN + i);
                    // }
                    break;

                case EntryType::Bool: {
                    string b;
                    cin >> b;
                    if (b == "true")    matrix[startN + i].emplace_back(true);
                    else                matrix[startN + i].emplace_back(false);
                    
                    // if(index_state == Index::BST && (int)(matrix[startN + i].size() - 1) == indexed_column) {
                    //     bst[TableEntry(b)].push_back(startN + i);
                    // }
                    // if(index_state == Index::HASH && (int)(matrix[startN + i].size() - 1) == indexed_column) {
                    //     hash[TableEntry(b)].push_back(startN + i);
                    // }
                    break;
                }     

                case EntryType::String:
                    string s;
                    cin >> s;
                    matrix[startN + i].emplace_back(s);
                    // if(index_state == Index::BST && (int)(matrix[startN + i].size() - 1) == indexed_column) {
                    //     bst[TableEntry(s)].push_back(startN + i);
                    // }
                    // if(index_state == Index::HASH && (int)(matrix[startN + i].size() - 1) == indexed_column) {
                    //     hash[TableEntry(s)].push_back(startN + i);
                    // }
                    break;
            }
        }
    }
    if(index_state != Index::NONE)    rehash();
    cout << "Added " << num_rows << " rows to " << name << " from position " 
         << startN << " to " << startN + num_rows - 1 << "\n";
}

void SillyQL::Table::print_lines() {
    int num_cols;
    cin >> num_cols;
    vector<int> col_indices(num_cols);
    vector<string> column_names(num_cols);

    for(int i = 0; i < num_cols; ++i) {     //print selected col_names
        string start;
        cin >> start;
        int bg = col_names[start];
        if(bg == 0 && start != first_col_name) {
            cout << "Error during PRINT: " << start << " does not name a column in " << name << "\n";
            getline(cin, start);
            return;
        }
        else {
            col_indices[i] = bg;
            column_names[i] = start;
        }     
    }
    string cmd;
    cin >> cmd;
    int rows = 0;
    if (cmd == "WHERE") {
        cin >> cmd;
        int index = col_names[cmd];
        if(index == 0 && cmd != first_col_name) {
            cout << "Error during PRINT: " << cmd << " does not name a column in " << name << "\n";
            getline(cin, cmd);
            return;
        }
        if(!quiet) {
            for(size_t i = 0; i < column_names.size(); ++i) 
                cout << column_names[i] << " ";
            cout << "\n";
        }
        rows = print_where(col_indices, index);
    }
    else {
        if(!quiet) {
            for(size_t i = 0; i < column_names.size(); ++i) 
                cout << column_names[i] << " ";
            cout << "\n";
        }
        rows = print_all(col_indices);
    }
    cout << "Printed " << rows <<" matching rows from " << name << "\n";
}

void SillyQL::Table::print_helper(const vector<int> &data, int matrix_row) {

    for(int j = 0; j < (int)data.size(); ++j) { //col
        if(col_types[data[j]] != EntryType::Bool) {
            cout << matrix[matrix_row][data[j]] << " ";
        }
        else {
            if(matrix[matrix_row][data[j]] == true)  cout << "true ";
            else                            cout << "false ";
        }
    }
    cout << "\n";
}

int SillyQL::Table::print_all(const vector<int> &data) {
    if(!quiet) {
        for(int i = 0; i < (int)matrix.size(); ++i) {   //row
            print_helper(data, i);
        }
    }
    
    return (int)matrix.size();
}

int SillyQL::Table::print_where_helper(const vector<int> &data, TableEntry &tbe, char &comp, int &index) {

    int rows_printed = 0;
    if (comp == '<') {
        if (index_state == Index::BST && indexed_column == index) {
            auto it = bst.lower_bound(tbe);
            for(auto first = bst.begin(); first != it; ++first) {
                if(!quiet)  {
                    for(size_t elts : first->second) {
                        print_helper(data, (int)elts);
                    }
                }
                rows_printed += (int)first->second.size();
            }
        }
        else {
            Entry_less comp(tbe, index);
            for (int i = 0; i < (int)matrix.size(); ++i) {
                if(comp(matrix[i])) {
                    if(!quiet)  print_helper(data, i);
                    ++rows_printed;
                }
            }
        }
    }
    else if (comp == '>') {
        if (index_state == Index::BST && indexed_column == index) {
            for(auto it = bst.upper_bound(tbe); it != bst.end(); ++it) {
                if(!quiet)  {
                    for(size_t elts : it->second) {
                        print_helper(data, (int)elts);
                    }
                }
                
                rows_printed += (int)it->second.size();
            }
        }
        else {
            Entry_greater comp(tbe, index);
            for (int i = 0; i < (int)matrix.size(); ++i) {
                if(comp(matrix[i])) {
                    if(!quiet)  print_helper(data, i);
                    ++rows_printed;
                }
            }
        }
        
    }
    else {
        if (index_state == Index::BST && indexed_column == index) {
            if(!quiet) {
                for(size_t elts : bst[tbe]) {
                    print_helper(data, (int)elts);
                }
            }
            rows_printed += (int)bst[tbe].size();
        }
        else if (index_state == Index::HASH && indexed_column == index) {
            if(!quiet) {
                for(size_t elts : hash[tbe]) {
                    print_helper(data, (int)elts);
                }
            }
            rows_printed += (int)hash[tbe].size();
        }
        else {
            Entry_equal comp(tbe, index);
            for (int i = 0; i < (int)matrix.size(); ++i) {
                if(comp(matrix[i])) {
                    if(!quiet)  print_helper(data, i);
                    ++rows_printed;
                }
            }
        }
        
    }
    return rows_printed;
}

int SillyQL::Table::print_where(const vector<int> &data, int index) {
    
    char comp;
    cin >> comp;   //<colname> <OP>
    EntryType et = col_types[index];
    int num_printed_rows = 0;

    switch (et) {
        case EntryType::Double: {
            double d;
            cin >> d;
            TableEntry tbe(d);
            num_printed_rows = print_where_helper(data, tbe, comp, index);
            break;
        }

        case EntryType::Bool: {
            string b;
            bool bo = false;
            cin >> b;
            if (b == "true")   bo = true;
            TableEntry tbe(bo);
            num_printed_rows = print_where_helper(data, tbe, comp, index);
            break;
        }

        case EntryType::Int: {
            int i;
            cin >> i;
            TableEntry tbe(i);
            num_printed_rows = print_where_helper(data, tbe, comp, index);
            break;
        }

        case EntryType::String: {
            string s;
            cin >> s;
            TableEntry tbe(s);
            num_printed_rows = print_where_helper(data, tbe, comp, index);
            break;
        }
            
        default:
            break;
    }
    return num_printed_rows;
}

int SillyQL::Table::delete_helper(TableEntry &tbe, char &comp, int &index) {
    
    int prev_size = (int)matrix.size();
    if (comp == '<') {
        auto it = remove_if(matrix.begin(), matrix.end(), Entry_less(tbe, index));
        matrix.erase(it, matrix.end());
        return prev_size - (int)matrix.size();
    }
    else if (comp == '>') {
        auto it = remove_if(matrix.begin(), matrix.end(), Entry_greater(tbe, index));
        matrix.erase(it, matrix.end());
        return prev_size - (int)matrix.size();
    }
    else {
        auto it = remove_if(matrix.begin(), matrix.end(), Entry_equal(tbe, index));
        matrix.erase(it, matrix.end());
        return prev_size - (int)matrix.size();
    }
}

void SillyQL::Table::delete_data() {
    string temp;
    char comp;
    cin >> temp >> temp >> comp;   //WHERE <colname> <OP>
    int index = col_names[temp];
    if(index == 0 && temp != first_col_name) {
        cout << "Error during DELETE: " << temp << " does not name a column in " << name << "\n";
        getline(cin, temp);
        return;
    }
    EntryType et = col_types[index];
    int num_deleted_rows = 0;

    switch (et) {
        case EntryType::Double: {
            double d;
            cin >> d;
            TableEntry tbe(d);
            num_deleted_rows = delete_helper(tbe, comp, index);
            break;
        }

        case EntryType::Bool: {
            string b;
            bool bo = false;
            cin >> b;
            if (b == "true")   bo = true;
            TableEntry tbe(bo);
            num_deleted_rows = delete_helper(tbe, comp, index);
            break;
        }

        case EntryType::Int: {
            int i;
            cin >> i;
            TableEntry tbe(i);
            num_deleted_rows = delete_helper(tbe, comp, index);
            break;
        }

        case EntryType::String: {
            string s;
            cin >> s;
            TableEntry tbe(s);
            num_deleted_rows = delete_helper(tbe, comp, index);
            break;
        }
            
        default:
            break;
    }
    if(index_state != Index::NONE)    rehash();
    cout << "Deleted " << num_deleted_rows << " rows from " << name << "\n";
}

void SillyQL::Table::join(Table &other) {

    //read all the command
    string trash;
    string t_col1;
    string t_col2;
    int num_col;
    
    cin >> t_col1 >> trash >> t_col2 >> trash >> trash >> num_col;  //<colname1> = <colname2> AND PRINT <N>
    vector<int> print_indices(num_col);
    vector<int> which_table(num_col);
    vector<string> column_names(num_col);
    int t1_index = col_names[t_col1];
    int t2_index = other.col_names[t_col2];
    
    if(t1_index == 0 && t_col1 != first_col_name) {
        cout << "Error during JOIN: " << t_col1 << " does not name a column in " << name << "\n";
        getline(cin, t_col1);
        return;
    }
    if(t2_index == 0 && t_col2 != other.first_col_name) {
        cout << "Error during JOIN: " << t_col2 << " does not name a column in " << other.name << "\n";
        getline(cin, t_col2);
        return;
    }
    for(int i = 0; i < num_col; ++i) {
        string p_colname;
        int which;
        cin >> p_colname >> which;      //<print_colname1> <1|2>
        if(which == 1) {
            int bg = col_names[p_colname];
            if(bg == 0 && p_colname != first_col_name) {
                cout << "Error during JOIN: " << p_colname << " does not name a column in " << name << "\n";
                getline(cin, p_colname);
                return;
            }
            print_indices[i] = bg;
            which_table[i] = 1;
        }
        else {
            int bg = other.col_names[p_colname];
            if(bg == 0 && p_colname != other.first_col_name) {
                cout << "Error during JOIN: " << p_colname << " does not name a column in " << other.name << "\n";
                getline(cin, p_colname);
                return;
            }
            print_indices[i] = bg;
            which_table[i] = 2;
        }
        column_names[i] = p_colname;
    }
    if(!quiet) {
        for (size_t i = 0; i < column_names.size(); ++i) {
            cout << column_names[i] << " ";
        }
        cout << "\n";
    }  

    //find and print each matching rows
    int rows_joined = 0;
    //generate temporary hash for t2_index
    unordered_map<TableEntry, vector<size_t>> temp_hash;
    if(t2_index != other.indexed_column) {
        for(size_t i = 0; i < other.matrix.size(); ++i) {
            temp_hash[other.matrix[i][t2_index]].push_back(i);
        }
    }

    for(int i = 0; i < (int)matrix.size(); ++i) {
        TableEntry tbe(matrix[i][t1_index]);
        if (other.index_state == Index::BST && t2_index == other.indexed_column) {
            if(!quiet) {
                for(int j = 0; j < (int)other.bst[tbe].size(); ++j) {
                    join_helper(i, (int)other.bst[tbe][j], other, print_indices, which_table);
                    cout << "\n";
                }
            }
            rows_joined += (int)other.bst[tbe].size();
        }
        else if (other.index_state == Index::HASH && t2_index == other.indexed_column) {
            if(!quiet) {
                for(int j = 0; j < (int)other.hash[tbe].size(); ++j) {
                    join_helper(i, (int)other.hash[tbe][j], other, print_indices, which_table);
                    cout << "\n";
                }
            }
            rows_joined += (int)other.hash[tbe].size();
        }
        else {
            if(!quiet) {
                for(int j = 0; j < (int)temp_hash[tbe].size(); ++j) {
                    join_helper(i, (int)temp_hash[tbe][j], other, print_indices, which_table);
                    cout << "\n";
                }
            }
            rows_joined += (int)temp_hash[tbe].size();
            // Entry_equal ee(tbe, t2_index);
            // for(int j = 0; j < (int)other.matrix.size(); ++j) {
            //     if(ee(other.matrix[j])) {
            //         if (!quiet) {
            //             join_helper(i, j, other, print_indices, which_table);
            //             cout << "\n";
            //         }
            //         rows_joined++;
            //     }
            // }
        }
        
    }

    cout << "Printed " << rows_joined << " rows from joining " << name << " to " << other.name << "\n";

}

void SillyQL::Table::join_helper(int i, int j, Table &other, vector<int> &print_indices, vector<int> &which_table) {
    for(int k = 0; k < (int)print_indices.size(); ++k) {
        if(which_table[k] == 1) {
            if(col_types[print_indices[k]] == EntryType::Bool) {
                if(matrix[i][print_indices[k]] == true)  cout << "true ";
                else                            cout << "false ";
            }
            else {
                cout << matrix[i][print_indices[k]] << " ";
            }
        }                    
        else {
            if(other.col_types[print_indices[k]] == EntryType::Bool) {
                if(other.matrix[j][print_indices[k]] == true)  cout << "true ";
                else                            cout << "false ";
            }
            else {
                cout << other.matrix[j][print_indices[k]] << " ";
            }
        }
    }
}

void SillyQL::Table::generate_bst(string &col_name) {
    if(col_names[col_name] == 0 && col_name != first_col_name) {
        cout << "Error during GENERATE: " << col_name << " does not name a column in " << name << "\n";
        return;
    }
    indexed_column = col_names[col_name];
    //indexed_name = col_name;
    index_state = Index::BST;
    if(!bst.empty())   bst.clear();
    if(!hash.empty())   hash.clear();
    for(size_t i = 0; i < matrix.size(); ++i) {
        bst[matrix[i][indexed_column]].push_back(i);
    }
    
    cout << "Created bst index for table " << name << " on column " << col_name << "\n";
}

void SillyQL::Table::generate_hash(string &col_name) {
    if(col_names[col_name] == 0 && col_name != first_col_name) {
        cout << "Error during GENERATE: " << col_name << " does not name a column in " << name << "\n";
        return;
    }
    indexed_column = col_names[col_name];
    //indexed_name = col_name;
    index_state = Index::HASH;
    if(!bst.empty())    bst.clear();
    if(!hash.empty())   hash.clear();
    for(size_t i = 0; i < matrix.size(); ++i) {
        hash[matrix[i][indexed_column]].push_back(i);
    }

    
    cout << "Created hash index for table " << name << " on column " << col_name << "\n";
}

void SillyQL::Table::rehash() {
    if(index_state == Index::BST) {
        bst.clear();
        for(size_t i = 0; i < matrix.size(); ++i) {
            bst[matrix[i][indexed_column]].push_back(i);
        }
    }
    else {
        hash.clear();
        for(size_t i = 0; i < matrix.size(); ++i) {
            hash[matrix[i][indexed_column]].push_back(i);
        }
    }
}