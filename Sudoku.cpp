#include <gtk/gtk.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>

// g++ game.cpp `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` -Wno-deprecated-declarations

using namespace std;

static int a[9][9];
static GtkWidget* grid[9][9];
GtkWidget *window;


bool solve(int depth);
void find_unfilled_box(int& x, int& y);
bool is_valid(int num, int x, int y);
bool bad_number(int i);
static void menu_event(GtkMenuItem *widget, gpointer data);
static void get_element(GtkWidget *widget, gpointer data);
void open_dialog();
void GUI(int* argc, char*** argv);


int main(int argc, char* argv[]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            a[i][j] = 0;
        }
    }
    GUI(&argc, &argv);
    return 0;
}

void GUI(int* argc, char*** argv) {
    const char* file[] = {"New", "Open", "Quit"};
    gtk_init(argc, argv);
    GtkWidget *vbox, *hbox, *separator, *button, *file_menu, *menu_bar, *menu_item;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    vbox = gtk_vbox_new(0, 0);
    // menu bar
    file_menu = gtk_menu_new();
    menu_bar = gtk_menu_bar_new();
    menu_item = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item);
    for (int i = 0; i < 3; i++) {
        menu_item = gtk_menu_item_new_with_label(file[i]);
        gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), menu_item);
        g_signal_connect(menu_item, "activate", G_CALLBACK(menu_event), NULL);
    }
    gtk_box_pack_start(GTK_BOX(vbox), menu_bar, 1, 1, 0);
    // grid
    for (int i = 0; i < 9; i++) {
        hbox = gtk_hbox_new(0, 0);
        for (int j = 0; j < 9; j++) {
            grid[i][j] = gtk_entry_new();
            gtk_entry_set_max_length(GTK_ENTRY(grid[i][j]), 1);
            gtk_widget_set_size_request(grid[i][j], 20, 20);
            gtk_box_pack_start(GTK_BOX(hbox), grid[i][j], 1, 1, 0);
            if ((j + 1) % 3 == 0) {
                separator = gtk_vseparator_new();
                gtk_box_pack_start(GTK_BOX(hbox), separator, 1, 1, 0);
            }
        }
        gtk_box_pack_start(GTK_BOX(vbox), hbox, 1, 1, 0);
        if ((i + 1) % 3 == 0) {
            separator = gtk_hseparator_new();
            gtk_box_pack_start(GTK_BOX(vbox), separator, 1, 1, 0);
        }
    }
    // button
    button = gtk_button_new_with_label("Solve Sudoku");
    g_signal_connect(button, "clicked", G_CALLBACK(get_element), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, 1, 1, 0);

    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_window_set_title(GTK_WINDOW(window), "CSF211");
    gtk_widget_show_all(window);
    gtk_main();
}

bool bad_number(int i) {
	if(!(i >= 0 && i < 10))
	{
		GtkWidget* dialog, *label;
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
    if (strcmp(gtk_menu_item_get_label(widget), "New") == 0) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                a[i][j] = 0;
                gtk_entry_set_text(GTK_ENTRY(grid[i][j]), "0");
            }
        }
    }
    else if (strcmp(gtk_menu_item_get_label(widget), "Open") == 0) {
        GtkWidget *dialog;
        dialog = gtk_file_chooser_dialog_new("Choose File", GTK_WINDOW(data), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
        gtk_widget_show_all(dialog);
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
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
        else if (response == GTK_RESPONSE_CANCEL) {
            gtk_widget_destroy(dialog);
        }
    }
    else if (strcmp(gtk_menu_item_get_label(widget), "Quit") == 0) {
        gtk_main_quit();
    }
}


bool solve(int depth) {
    int x = -1, y = -1;
    find_unfilled_box(x, y);
    if (x == -1 && y == -1) {
        for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				char c[2];
				sprintf(c,"%d",a[i][j]);
				gtk_entry_set_text(GTK_ENTRY(grid[i][j]),c);
			}
		}
        return true;
    }
    for (int i = 1; i < 10; i++) {
        if (is_valid(i, x, y)) {
            a[x][y] = i;
            if (solve(depth + 1)) {
                return true;
            }
            a[x][y] = 0;
        }
    }
    if (depth == 0) {
        open_dialog();
    }
    return false;
}


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

bool is_valid(int num, int x, int y) {
    for (int i = 0; i < 9; i++) {
        if (a[i][y] == num) {
            return false;
        }
    }
    for (int j = 0; j < 9; j++) {
        if (a[x][j] == num) {
            return false;
        }
    }
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

void open_dialog() {
	GtkWidget* dialog,*label;
	dialog=gtk_dialog_new_with_buttons("Error",GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);
	label=gtk_label_new("Unsolvable Board\n");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),label,0,0,0);
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}


static void get_element(GtkWidget *widget, gpointer data) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            a[i][j] = atoi(gtk_entry_get_text(GTK_ENTRY(grid[i][j])));
        }
    }
    solve(0);
}
