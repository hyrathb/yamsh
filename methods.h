#ifndef METHODS_H_INCLUDED
#define METHODS_H_INCLUDED
#include <gtk/gtk.h>
#include "structs.h"
/****************************************************
Gtk
****************************************************/
/*to create the main window*/
void create_main();

/*to create the detailed room window for a room type*/
void create_room_win();

void room_dialogue_check(GtkMenuItem *ignore, struct room_dialogue_check *data);

/*to create the detailed guest window for a room*/
void create_guest_win();

void guest_dialogue_check(GtkMenuItem *ignore, struct guest_dialogue_check *data);

/*to show error message*/
void errmesg(const char *error);

/*to show a file-chooser-dialogue*/
char *choose_file(int type);

/*to close the current file*/
void file_close(GtkWidget *win);

/*to create a new file*/
void file_new(GtkWidget *win);

/*to open a existing file*/
void file_open(GtkWidget *win);

/*to save the current file*/
void file_save(GtkWidget *win);

/*to close the current file*/
void file_quit(GtkWidget *win);

/*open a new dialogue to insert a new room type*/
void edit_new_type(GtkWidget *win);

/*open a new dialogue to insert a new room*/
void edit_new_room(GtkWidget *win);

/*open a new dialogue to insert a new guest_in info*/
void edit_new_guest_in(GtkWidget *win);

void edit_new_guest_out(GtkWidget *win);

/*to show the about dialogue*/
void help_about();

/*to show the help dialogue*/
void help_help(GtkWidget *win);

/*to create the main menu*/
void create_menu(GtkWidget *win, GtkWidget *menubar);

/*to show or refresh the data from the current file*/
void access_type_data();

void access_room_data();

void access_guest_data();

/*the common type dialogue*/
void type_dialogue(int is_edit);

/*the common room dialogue*/
void room_dialogue(int is_edit, struct room_dialogue_check *checkdata);

/*the common guest in dialogue*/
void guest_in_dialogue(struct guest_dialogue_check *checkdata);

/*the common guest out dialogue*/
void guest_out_dialogue();

/*the common guest modify dialogue*/
void guest_modify_dialogue();

/*to show room info in a type*/
void type_view_details();

/*to show room info in a type*/
void room_view_detail();

/*to find the active item in the treeview*/
void handle_room_find_focus(GtkWidget *ignored, GtkTreeSelection *selection);

/*to show guest info in a room when click the button*/
void handle_room_view_detail();

/*to destroy the current dialogue*/
void handle_destroy_dialogue(GtkWidget *ignored, GtkWidget *dialogue);

/*to hide the current dialogue*/
void handle_dialogue_hide(GtkWidget *ignored, GtkWidget *dialogue);

/*to show the popup menu when right clicked*/
int handle_popup_menu(struct popup_menu *pop_struct, GdkEvent *event);

/*to handle the new type*/
void handle_new_type(GtkWidget *ignored, struct new_type_data *data);

/*to edit the display of modified type*/
void handle_edit_type(GtkWidget *ignored, struct new_type_data *data);

/*assistant function*/
void handle_edit_type_dialogue();

/*delete the selected type and remove it from the list*/
void handle_type_delete();

/*assistant function*/
void handle_room_new_assistant(void *ignored, struct room_dialogue_check *data);

/*assistant function*/
void handle_room_edit_assistant(void *ignored, struct room_dialogue_check *data);

/*to handle the new room*/
void handle_room_new(GtkWidget *ignored, struct new_room_data *data);

/*to edit the display of modified room*/
void handle_room_modify(GtkWidget *ignored, struct new_room_data *data);

/*to delete a room from the treeview and the list*/
void handle_room_delete();

/*to find the active item in the treeview*/
void handle_guest_find_focus(GtkWidget *ignored, GtkTreeSelection *selection);

/*to handle the new guest_in*/
void handle_guest_in_new(GtkWidget *ignored, struct new_guest_in_data *data);

/*to handle the new guest_out*/
void handle_guest_out_new(GtkWidget *ignored, struct new_guest_out_data *data);

/*to edit the display of modified guest info*/
void handle_guest_modify(GtkWidget *ignored, struct new_guest_data *data);

/*to delete a guest info from the treeview and the list*/
void handle_guest_delete();

/*show type detail when double clicked*/
void handle_type_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, GtkTreeSelection* selection);

/*show room detail when double clicked*/
void handle_room_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, GtkTreeSelection* selection);

/*to show the result of the query*/
void handle_query_room(GtkWidget *ignored, struct query_room *data);

/*to show the result of the query*/
void handle_query_guest(GtkWidget *ignored, struct query_guest *data);

/**/
void handle_sta_by_year(GtkWidget *ignored, GtkEntry *entry);
/*****************************************************
Maintain Data
*****************************************************/
/*to load data from a file*/
struct basic_room_type_data *load_data();

/*to save data to a file*/
void save_data();

/*to insert a room type node*/
void type_insert(const char *type, int most_in);

/*to delete a room type node*/
void type_delete();

/*to change the info of a existing room type*/
void type_modify(const char *type, int most_in);

/*to insert a room node*/
void room_insert(struct basic_room_type_data *type, const char *room_number, const char *phone_number, float area, float price, int is_left);

/*to delete a room node*/
void room_delete();

/*to change the info of a existing room*/
void room_modify(struct basic_room_type_data *type, const char *room_number, const char *phone_number, float area, float price, int is_left);

/*to insert a guest-in info node, return the inserted node*/
void guest_in_insert(struct basic_room_data *room, const char *id, const char *name, const char *in_time);

/*to insert a guest-out info node, return the inserted node*/
void guest_out_insert(struct basic_guest_info *guest, const char *out_time, float pay);

/*to modify the guest info*/
void guest_modify(struct basic_room_data *room, const char *id, const char *name, const char *in_time, const char *out_time, float pay);

/*to delete a guest node*/
void guest_delete();

/*dialogue which query for room that meets with the given conditions*/
void query_room();

/*dialogue which query for guest info that meets with the given conditions*/
void query_guest();

/*a dialogue shows the statistics of every room type in month this year*/
void sta_by_month();

/*a dialogue shows the statistics of every room in the given year*/
void sta_by_year();

/*a dialogue show the top 10 guests*/
void sta_by_guest();
/*****************************************************
Query
*****************************************************/
/*query for room type by room type name*/
//void q_room_type_by_type(char *type, struct basic_room_type_data *type_liset);

/*query for room that meets with the given conditions*/
void q_room_by_conditions(GtkListStore *store,  int is_room_number_limited, int is_phone_number_limited, int is_type_limited, int is_left_limited, int is_price_limited, int is_area_limited, struct basic_room_type_data *type, const char *room_number, const char *phone_number, int is_left, float uprice, float lprice, float uarea, float larea);

/*query for guest info that meets with the given conditions*/
void q_guest_info_by_conditions(GtkListStore *store, int is_id_limited, int is_name_limited, int is_in_time_limited, const char *id, const char *name_piece, const char *lin_time, const char *uin_time);




/*****************************************************
Statistics
*****************************************************/

void statistics_room_type_by_room_type(struct basic_room_type_data *typelist);

void statistics_room_type_by_month(struct basic_room_type_data *typelist);

void statistics_room_by_year(struct basic_room_type_data *typelist);

void statistics_most_in_by_id(struct basic_guest_info *guestlist);


#endif // METHODS_H_INCLUDED

/*****************************************************
Assistant
*****************************************************/
int get_hour(const char timech[]);

int get_num(const char **chs, char ch);

int is_year(int year);

int day_of_year(int year);

int day_of_month(int year, int month);

int date_2_day(int year, int month, int day);

float cal_day(const char time1[], const char time2[]);

void free_data(GtkWidget *ignored, void *data);

int cal_types();

void fill_type(int types, GType *type_list);

float cal_value(struct basic_room_type_data *type, int year, int month);

void cal_values(int types, int year, int month, int *column_num, GValue *value, float *total);

void cal_room_sta(GtkListStore *store, struct basic_room_data *room, int year, float *total_w, float *total_day);

void cal_rooms_sta(GtkListStore *store, int year);

void cal_top_10_guest(GtkListStore *store);
