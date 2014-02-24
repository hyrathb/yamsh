#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED
#include <gtk/gtk.h>

struct basic_guest_info;

struct basic_room_type_data;

struct basic_room_data
{
    char room_number_[6];
    char phone_number_[20];
    struct basic_room_type_data *type_;
    float area_;
    float price_;
    char is_left_;
    struct basic_guest_info *guest_;
    struct basic_room_data *next_;
};

struct room_interest
{
    char room_number_[6];
    char *room_type_;
    float fare_;
    float paid_;
    struct room_interest *next_;
};

struct basic_room_type_data
{
    char type_[20];
    int most_in_;
    int total_;
    int left_;
    struct basic_room_data *room_head_;
    struct basic_room_type_data *next_;
};


struct basic_guest_info
{
    char id_[20];
    char name_[20];
    struct basic_room_data *room_info_;
    char in_time_[18];
    char out_time_[18];
    float day_spent_;
    float fare_;
    float pay_;
    struct basic_guest_info *next_;
};

struct popup_menu
{
    GtkWidget *menu;
    GtkTreeSelection *selection;
    GtkTreeView *treeview;
    void **active;
};

struct room_dialogue_check
{
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
};

struct guest_dialogue_check
{
    GtkWidget *button1;
    GtkWidget *button2;
};

struct new_type_data
{
    GtkWidget *entry1;
    GtkWidget *entry2;
};

struct new_room_data
{
    GtkWidget *combo1;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *checkbutton;
};

struct new_guest_in_data
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
};

struct new_guest_out_data
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
};

struct new_guest_data
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *entry5;
    GtkWidget *entry6;
};

struct query_room
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry4;
    GtkWidget *entry5;
    GtkWidget *entry6;
    GtkWidget *entry7;
    GtkWidget *combo1;
    GtkWidget *checkbutton1;
    GtkWidget *checkbutton2;
    GtkWidget *checkbutton3;
    GtkWidget *checkbutton4;
    GtkWidget *checkbutton5;
    GtkWidget *checkbutton6;
    GtkWidget *checkbutton7;
    GtkListStore *store;
};

struct query_guest
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *checkbutton1;
    GtkWidget *checkbutton2;
    GtkWidget *checkbutton3;
    GtkListStore *store;
};
#endif // STRUCTS_H_INCLUDED
