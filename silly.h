// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#ifndef SILLY_H
#define SILLY_H
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <map>
#include "TableEntry.h"
#include <vector>
#include <cassert>
using namespace std;

class SillyQL{
public:

    enum class Index {
        NONE,
        HASH,
        BST
    };

    class Table {
    public:
        Table();
        Table(const string &name_in, const int &col_num_in, const bool &quiet_in);
        void insert(int &num_rows);                   //INSERT
        void print_lines();                           //PRINT
        int print_all(const vector<int> &data);       //PRINT ... ALL
        int print_where(const vector<int> &data, int index);     //PRINT ... WHERE
        int print_where_helper(const vector<int> &data, TableEntry &tbe, char &comp, int &index);
        void print_helper(const vector<int> &data, int matrix_row);
        void delete_data();                           //DELETE
        int delete_helper(TableEntry &tbe, char &comp, int &index);
        void join(Table &other);                      //JOIN
        void join_helper(int i, int j, Table &other, vector<int> &print_indices, vector<int> &which_table);
        void generate_bst(string &col_name);          //GENERATE
        void generate_hash(string &col_name);
        void rehash();

    private:
        string name;
        string first_col_name = "";
        int col_num;
        bool quiet = false;
        unordered_map<string, int> col_names;
        vector<EntryType> col_types;
        vector<vector<TableEntry>> matrix;
        //index related members
        Index index_state = Index::NONE;
        int indexed_column = -1;
        //string indexed_name = "";
        map<TableEntry, vector<size_t>> bst;
        unordered_map<TableEntry, vector<size_t>> hash;
    };

    class Entry_less {
    public:
        Entry_less(const TableEntry &val_in, int &col_in) : val(val_in), col(col_in){}

        bool operator()(vector<TableEntry> &tbe) const {
            return tbe[col] < val;
        }

    private:
        TableEntry val;
        int col;
    };

    class Entry_equal {
    public:
        Entry_equal(const TableEntry &val_in, int &col_in) : val(val_in), col(col_in) {}

        bool operator()(vector<TableEntry> &tbe) const {
            return tbe[col] == val;
        }

    private:
        TableEntry val;
        int col;
    };

    class Entry_greater {
    public:
        Entry_greater(const TableEntry &val_in, int &col_in) : val(val_in), col(col_in) {}

        bool operator()(vector<TableEntry> &tbe) const {
            return tbe[col] > val;
        }

    private:
        TableEntry val;
        int col;
    };

    SillyQL () : quiet(false) {}
    void process_command(string cmd);
    void get_mode(int argc, char** argv);
    void create();                          
    void quit();
    void comment();
    void remove();

private:
    unordered_map<string, Table> database;
    bool quiet;
};

#endif