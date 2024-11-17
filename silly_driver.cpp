// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "silly.h"
#include <getopt.h>

void SillyQL::get_mode(int argc, char** argv) {
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;

    struct option longOpts[] = {{ "help", no_argument, nullptr, 'h' },
                                { "quiet", no_argument, nullptr, 'q' }};
    

    while ((option = getopt_long(argc, argv, "hq", longOpts, &option_index)) != -1) {
        switch (option) {
            case 'h':
                
                break;
        
            case 'q':
                quiet = true;
                break;

            default:
                cerr << "Error: invalid option\n";
                exit(1);
    }
  }
}

int main(int argc, char** argv) {
    
    ios_base::sync_with_stdio(false); //turn off sync
    SillyQL sql;
    sql.get_mode(argc, argv);
    string cmd;
    //int i = 0;
    do {
        cout << "% ";
        cin >> cmd;
        //process cmd
        sql.process_command(cmd);
    //     ++i;
    //     if (i == 40)    break;

    } while(cmd != "QUIT");
}