// CSF211 DATA STRUCTURES AND ALGORITHMS COURSE PROJECT
// 2017AAPS0280H
// 2017AAPS0360H
// 2018A3PS0624H
// 2018AAPS0643H

// g++ game.cpp `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` -Wno-deprecated-declarations

#include <gtk/gtk.h>    // using gtk oepn-source and free widget toolking for GUI
#include <iostream>
#include <fstream>

using namespace std;

static int a[9][9]; // global variable for sudoku board values
static GtkWidget* grid[9][9];   // global variable for sudoku display board and entries
GtkWidget *window;  // global variable for toplevel window


bool solve(int depth);  // function to solve input sudoku board
void find_unfilled_box(int& x, int& y); // function to find empty or unfilled sudoku boxes
bool is_valid(int num, int x, int y);   // function to test if a sudoku entry is valid or not
bool bad_number(int i); // function to test input validity, allows only single digits 1-9
static void menu_event(GtkMenuItem *widget, gpointer data); // function performs actions based on menu selection
static void get_element(GtkWidget *widget, gpointer data);  // functions fetches values from gtk entries and stores them in 'a'
void open_dialog(); // displays error message if sudoku board is not solvable
void GUI(int* argc, char*** argv);  // GUI function to render all GUI


int main(int argc, char* argv[]) {
    // setting sudoku board initially to all zeros (empty)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            a[i][j] = 0;
        }
    }
    GUI(&argc, &argv);  // run GUI function to open graphics
    return 0;
}

void GUI(int* argc, char*** argv) {
    const char* file[] = {"New", "Open", "Quit"};   // three options to be displayed in menu
    gtk_init(argc, argv);   // initialises gtk
    GtkWidget *vbox, *hbox, *separator, *button, *file_menu, *menu_bar, *menu_item; // gtk widget variables for GUI components
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);   // top level window
    vbox = gtk_vbox_new(0, 0);  // vertical box widget, orders widgets in it vertically
    // menu bar
    file_menu = gtk_menu_new(); // file menu widget
    menu_bar = gtk_menu_bar_new();  // menu bar to store various file menus
    menu_item = gtk_menu_item_new_with_label("File");   // menu items under file menu - dropdown items
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), file_menu); // ading menu items to file menu
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item); // ading menu items to menu bar
    for (int i = 0; i < 3; i++) {
        menu_item = gtk_menu_item_new_with_label(file[i]);  // creates menu item with labels as above
        gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), menu_item); // appending this menu item to file menu  
        g_signal_connect(menu_item, "activate", G_CALLBACK(menu_event), NULL);  // connects menu item to a function operation
    }
    gtk_box_pack_start(GTK_BOX(vbox), menu_bar, 1, 1, 0); // "packing" menu bar to the vertical box
    // creating sudoku GUI grid
    for (int i = 0; i < 9; i++) {
        hbox = gtk_hbox_new(0, 0); // horizontal box to store a row of 9 boxes
        for (int j = 0; j < 9; j++) {
            grid[i][j] = gtk_entry_new();   // gtk entry widget, takes inputs
            gtk_entry_set_max_length(GTK_ENTRY(grid[i][j]), 1); // setting max entry length to 1 
            gtk_widget_set_size_request(grid[i][j], 20, 20);    // setting size
            gtk_box_pack_start(GTK_BOX(hbox), grid[i][j], 1, 1, 0); // packing gtk entry boxes to the horizontal box
            // adds vertical separator after 3 boxes
            if ((j + 1) % 3 == 0) {
                separator = gtk_vseparator_new();  
                gtk_box_pack_start(GTK_BOX(hbox), separator, 1, 1, 0);
            }
        }
        gtk_box_pack_start(GTK_BOX(vbox), hbox, 1, 1, 0); // packing horizontal row to vertical box
        // adds horizontal separator after three rows
        if ((i + 1) % 3 == 0) {
            separator = gtk_hseparator_new();
            gtk_box_pack_start(GTK_BOX(vbox), separator, 1, 1, 0);
        }
    }
    button = gtk_button_new_with_label("Solve Sudoku"); // solve sudoku board button
    g_signal_connect(button, "clicked", G_CALLBACK(get_element), NULL); // connecting sudoku button to get_element function
    gtk_box_pack_start(GTK_BOX(vbox), button, 1, 1, 0); // packing button to the vertical box

    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL); // connecting "close window" button to gtk_quit
    gtk_container_add(GTK_CONTAINER(window), vbox); // adding vertical box to main window
    gtk_window_set_title(GTK_WINDOW(window), "CSF211 Data Structures and Algorithms"); // name of the window
    gtk_widget_show_all(window); // shows all widgets after adding to toplevel window
    gtk_main();
}

bool bad_number(int i) {
	if(!(i >= 0 && i < 10)) // if i is not in 1 and 9
	{
		GtkWidget* dialog, *label; // dialog widget launches a pop-up
		dialog = gtk_dialog_new_with_buttons("Error",GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
		label = gtk_label_new("This number is bigger than 9 or less than 0\nOr not a number");
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), label, 0, 0, 0);
		gtk_widget_show_all(dialog);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return 1;
	}
	return 0;
}


static void menu_event(GtkMenuItem *widget, gpointer data) {
    // if selected option is "New"
    if (strcmp(gtk_menu_item_get_label(widget), "New") == 0) {
        // sets the board to empty i.e. all zeros
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                a[i][j] = 0;
                gtk_entry_set_text(GTK_ENTRY(grid[i][j]), "0"); // entries gtk_entry widgets to "0" char
            }
        }
    }
    // if selected option is "Open"
    else if (strcmp(gtk_menu_item_get_label(widget), "Open") == 0) {
        GtkWidget *dialog; // launches a dialog window to choose a file
        // macros represent responses given by user
        dialog = gtk_file_chooser_dialog_new("Choose File", GTK_WINDOW(data), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
        gtk_widget_show_all(dialog); // shows the dialog widget
        gint response = gtk_dialog_run(GTK_DIALOG(dialog)); // waits for the response given by user
        // if response is OK i.e. file selected, opens the file and entries into gtk_entry and a
        if (response == GTK_RESPONSE_OK) {
            ifstream in (gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (in.eof() || bad_number(a[i][j])) {
                        break;
                    }
                    in >> a[i][j];
                    char c[2];
                    sprintf(c, "%d", a[i][j]);
                    gtk_entry_set_text(GTK_ENTRY(grid[i][j]), c);
                }
            }
            in.close();
            gtk_widget_destroy(dialog);
            solve(0);
        }
        // if response is cancel, closes widget
        else if (response == GTK_RESPONSE_CANCEL) {
            gtk_widget_destroy(dialog);
        }
    }
    // if selected option is "Quit"
    else if (strcmp(gtk_menu_item_get_label(widget), "Quit") == 0) {
        gtk_main_quit(); // closes all gtk widgets and windows
    }
}


void open_dialog() {
	GtkWidget* dialog,*label;
	dialog=gtk_dialog_new_with_buttons("Error",GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);
	label=gtk_label_new("Unsolvable Board\n"); // displays error message
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),label,0,0,0);
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}


// converts entries from GUI from char to int and inputs in "a"
static void get_element(GtkWidget *widget, gpointer data) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            a[i][j] = atoi(gtk_entry_get_text(GTK_ENTRY(grid[i][j])));
        }
    }
    solve(0); // calling solve function
}

bool solve(int depth) {
    int x = -1, y = -1;
    find_unfilled_box(x, y); // finds any box yet to be filled
    // if no box is found, board is solved
    if (x == -1 && y == -1) {
        for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				char c[2];
				sprintf(c,"%d",a[i][j]); // converts int to char
				gtk_entry_set_text(GTK_ENTRY(grid[i][j]),c); // entries char to gtk_entry widgets in GUI
			}
		}
        return true; // returns true
    }
    // backtracking algorithm
    for (int i = 1; i < 10; i++) {
        // if the digit placement is valid, it continues further
        if (is_valid(i, x, y)) {
            a[x][y] = i;
            if (solve(depth + 1)) {
                return true;
            }
            a[x][y] = 0;
        }
    }
    // if all combinations tried return false, board is not solvable. Displays error.
    if (depth == 0) {
        open_dialog();
    }
    return false;
}

// finds unfilled boxes
void find_unfilled_box(int& x, int& y) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (a[i][j] == 0) {
                x = i;
                y = j;
                return;
            }
        }
    }
    return;
}

// tests if num placed at (x, y) is valid or not
bool is_valid(int num, int x, int y) {
    // checks entire column
    for (int i = 0; i < 9; i++) {
        if (a[i][y] == num) {
            return false;
        }
    }
    // checks entire row
    for (int j = 0; j < 9; j++) {
        if (a[x][j] == num) {
            return false;
        }
    }
    // checks it's respective 3X3 box
    x = x - x % 3;
    y = y - y % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (a[i + x][j + y] == num) {
                return false;
            }
        }
    }
    return true;
}
